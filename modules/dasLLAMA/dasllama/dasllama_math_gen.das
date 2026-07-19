options gen2
options _comment_hygiene = true

module dasllama_math_gen shared public

require dasllama/dasllama_math
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_default  // dot_q8q8_f16s — the row-major tails of the s16 twins
require llvm/daslib/f16_cvt  // the kq reference bodies widen the f16 (s, o) scale pairs
require llvm/daslib/llvm_tune public  // [tune]/[tune_perm]/[tune_companion] + [llvm_code]; PUBLIC so the auto-policy infer pass and tune_status reach app roots that require this module
require dasllama/dasllama_gemm_schema
require daslib/jobque_boost public
require dasllama/dasllama_par
require math

// The GEMM family as a [tune] client (gemm_generator_plan.md part 3; promoted from the M2
// GO/NO-GO experiment): the perm grid below stamps dasllama_gemm_gen::q8q8_tile perms through
// [llvm_code], and the [tune_companion] rows stamp q8q8_layout_gen + q8q8_wbias_gen +
// q8q8_kgroup_gen + q8q8_tokstep_gen + q8q8_gemv_gen + mx4q8_gemv_gen + q8q8_family_live from
// the same entry — the eight-function stamp: the runtime repacks (Q8 AND mx4 planes) ask the
// layout companion which interleave the stamped cores read, the wbias companion how the plane
// bytes are baked (bias128), the kgroup companion how the plane groups its k-bytes (kg8 on
// smmla stamps), the tokstep companion how many tokens one tile call covers (16·nrsplit on
// amx stamps — the batch wrapper walks its space off it), the witness answers "did the family
// actually emit", the q8q8_tile_s16_gen/q8q8_gemv_s16_gen twins serve the wscale_f16 rail
// (same perm, only the scale-fold load widens binary16), and ALL the generators share one
// decline predicate, so kernels, repacks, walk shape and availability cannot desync on any
// tier. Normal runs stamp exactly one winner (manifest > the per-ISA fallback chain:
// biased busd512 on AVX512-VNNI boxes / biased busd256 on VEX-VNNI / maddubs on plain AVX2 /
// kstep2 elsewhere — each ISA's measured tune winner as the manifest-less default);
// DAS_TUNE_MODE=tune/test stamps the grid + the *_variants() registries for the harness
// (gen_parity_probe / gen_tune_probe). The K-quant kernels are their OWN four [tune]
// families (k4/k5/k6/q40q8_tile_gen + gemv/layout companions, one manifest entry per format) —
// the kq tiles measurably prefer different (mr, nrsplit) than the q8 crown (M1 2026-07-12:
// k5 tile +30% on grp4/single-unpack vs the q8 grp8/nrsplit2 stamp), and per-format layout
// companions let each format's planes take their own interleave. This module registers TWO
// backends over the SAME
// ISA-agnostic slot traversals: "arm64-gen" (priority 25, the arm64 load-select tier since
// slice C) and "x64-gen" (priority 25, witness-gated — only selectable when the stamped
// family emitted). dasllama_common pulls it in behind a path guard
// (`require ?llvm/daslib/llvm_tune`), so any Q8/mx4 model load on a gen-capable box runs the
// generated family. The generators live in dasllama/dasllama_gemm_gen.das (llvm-jit context,
// wired via llvm/daslib/llvm_user_modules.das).

// The library-owned tune scope: winners land in the app tune sidecar, and the policy rail
// (auto default / --tune / daspkg release) runs dasllama_tuner — the wrapper that sweeps
// BOTH halves: the [tune] generator grid (gen_tune_probe; point DASLLAMA_CONFIRM_MODEL at a
// q8 gguf to arm its e2e confirm gate) and the [tuned] loop-hint grid + runtime snapshot
// (tune_kernels). covers= names the modules whose [tuned] kernels this scope demands sidecar
// entries for (the completeness set — drift shows up as a loud "missing:" list at startup).
[tune_scope(name = "dasllama", tuner = "../harness/dasllama_tuner.das",
            covers = "dasllama_math;dasllama_math_default;dasllama_quant;dasllama_math_aarch64_neon")]
struct private DasllamaTuneScope {}

//! The layout companion (six-function stamp): the repack interleave (mr) the stamped tile reads.
//! Reference body = the reference tile's grp4; generator declines in lockstep with the tile
//! generator, so a perm the tile can't emit keeps the grp4 reference pair on both sides.
def q8q8_layout_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).interleave
}

//! The wbias companion (slice H): additive bias baked into the grp<mr> plane's GROUP-row bytes —
//! 128 for bias128 vpdpbusd perms (w^0x80, −128·Σx acc-init correction), 0 otherwise. Reference
//! body = the unbiased grp4 pair, declining in lockstep so plane bytes and kernels never disagree.
def q8q8_wbias_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).wbias
}

//! The kgroup companion (slice J): k-bytes per row per interleave group — 8 for smmla perms
//! (row-pair × 8-k MMA operands), 4 for every vector-dot perm. Reference body = the kg4 grp4
//! pair, declining in lockstep so plane byte order and kernels never disagree.
def q8q8_kgroup_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).kgroup
}

//! The tokstep companion (slice I): tokens one stamped tile call covers — 16·nrsplit for amx
//! perms (TMUL macro is square in tile units), 4 for every vector perm; the batch wrapper walks
//! its (token, row-group) space off it. Reference body = the 4-token contract.
def q8q8_tokstep_gen() : int {
    return 4
}

//! The tile-config companion (slice K I1): a latch=1 raw-tile stamp carries no per-call
//! ldtilecfg/tilerelease, so the batch wrapper calls this once per dispatch chunk to load the
//! family's static TMUL palette. Reference body — and every non-latch stamp — is a no-op.
def q8q8_amx_cfg_gen() : void {}

//! The witness companion: did the stamped family actually EMIT here? true ⟺ every slot below is
//! generated code. Read at backend SELECTION time only, never [init] (the slice C rule) — a
//! declined stamp keeps x64-gen unavailable (slow NEON fallback) but arm64-gen stays available.
def q8q8_family_live() : bool {
    return false
}

//! The GEMV companion (nr=1 sub-family, slice D): a rows-range core over rows [rb, re) of one
//! region, no internal dispatch, rb/re multiples of the stamped mr. Reference body = the hand
//! laneq rows walk over grp4, declining in lockstep with the tile and layout.
[hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_gemv_gen(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    let nb = n / 32l
    unsafe {
        for (g in range64(rb / 4l, re / 4l)) {
            let f = dot_q8q8_laneq4(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
            yp[g * 4l + 0l] = f.x
            yp[g * 4l + 1l] = f.y
            yp[g * 4l + 2l] = f.z
            yp[g * 4l + 3l] = f.w
        }
    }
}

//! The mx4 GEMV companion (slice E): the MXFP4·Q8 twin of q8q8_gemv_gen, rows [rb, re) of one
//! region off the grp<mr>-interleaved nibble/exponent planes (repack_mx4_grp — SAME mr as Q8).
//! Reference body = the hand laneq mx4 rows walk over grp4, declining in lockstep.
[hint(unsafe_range_check, noalias = wn, noalias = we, noalias = xqp, noalias = xsp)]
def mx4q8_gemv_gen(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    let nrow = n / 2l
    let nbb = n / 32l
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])
        for (g in range64(rb / 4l, re / 4l)) {
            let f = dot_mx4q8_laneq4(lutp, wn + g * 4l * nrow, we + g * 4l * nbb, xqp, xsp, n)
            yp[g * 4l + 0l] = f.x
            yp[g * 4l + 1l] = f.y
            yp[g * 4l + 2l] = f.z
            yp[g * 4l + 3l] = f.w
        }
    }
}

//! The mx4 TILE companion (mx4 batch family): 4 tokens x mr rows per call off the grp<mr>-
//! interleaved nibble/exponent planes — mx4q8_gemv_gen's batch twin, dequanting each weight
//! block-group once and reusing it across the 4 activation rows. Ignores xbsp (mx4 never biased).
[unused_argument(xbsp), hint(unsafe_range_check, noalias = wn, noalias = we, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4q8_tile_gen(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let nb = n / 32l
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])
        for (t in range64(4l)) {
            let f = dot_mx4q8_laneq4(lutp, wn, we, xqp + (t0 + t) * n, xsp + (t0 + t) * nb, n)
            let r = g * 4l
            yp[(t0 + t) * d + r + 0l] = f.x
            yp[(t0 + t) * d + r + 1l] = f.y
            yp[(t0 + t) * d + r + 2l] = f.z
            yp[(t0 + t) * d + r + 3l] = f.w
        }
    }
}

//! One row's dot off the grp<mr> K-quant planes, scalar — the kq stubs' reference body (runs
//! only when the stamp declined) and the tests' repack oracle; bit-exact vs the portable disk
//! dots. Activations are Q8_K-form: xsp per-superblock scale, xbsp per-16 quant sums.
def kq_grp_row_dot(fmt : int64; kqg : uint8 const?; ksg : uint8 const?; r, mr : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64) : float {
    var acc = 0.0
    let nsb = n / 256l
    let qsb = fmt == 4l ? 128l : (fmt == 5l ? 160l : 192l)
    let ssb = fmt == 6l ? 18l : 20l
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * qsb * mr
            let sb = sbi * ssb * mr
            var isum = 0   // superblock-wide Σ sc·dot (k4/k5: sc·(ilo+ihi); k6: sc0·ilo + sc1·ihi)
            var bsum = 0   // superblock-wide Σ mn·Σa32 (k4/k5) / Σ sc·Σa16 (k6: the -32 offset term)
            for (blk in range64(8l)) {
                let b = sbi * 8l + blk
                var ilo = 0
                var ihi = 0
                for (j in range64(4l)) {
                    let hb = fmt == 5l ? int(kqg[qb + 128l * mr + (blk * 4l + j) * mr + r]) : 0
                    let g2 = uint((blk % 4l) * 2l)
                    for (t in range64(4l)) {
                        let nib = uint(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        var qlo = int(nib & 15u)
                        var qhi = int(nib >> 4u)
                        if (fmt == 5l) {
                            qlo |= ((hb >> int(t)) & 1) << 4
                            qhi |= ((hb >> int(4l + t)) & 1) << 4
                        } elif (fmt == 6l) {
                            let hbase = qb + 128l * mr + (blk / 4l) * 32l * mr
                            let h0 = uint(kqg[hbase + (j * mr + r) * 4l + t])
                            let h1 = uint(kqg[hbase + ((j + 4l) * mr + r) * 4l + t])
                            qlo |= int(((h0 >> g2) & 3u) << 4u)
                            qhi |= int(((h1 >> g2) & 3u) << 4u)
                        }
                        ilo += qlo * int(xqp[b * 32l + j * 4l + t])
                        ihi += qhi * int(xqp[b * 32l + 16l + j * 4l + t])
                    }
                }
                if (fmt == 6l) {
                    let s0 = int(int8(ksg[sb + 2l * blk * mr + r]))
                    let s1 = int(int8(ksg[sb + (2l * blk + 1l) * mr + r]))
                    isum += s0 * ilo + s1 * ihi
                    bsum += s0 * xbsp[b * 2l] + s1 * xbsp[b * 2l + 1l]
                } else {
                    let sc = int(ksg[sb + 4l * mr + blk * mr + r])
                    let mn = int(ksg[sb + 12l * mr + blk * mr + r])
                    isum += sc * (ilo + ihi)
                    bsum += mn * (xbsp[b * 2l] + xbsp[b * 2l + 1l])
                }
            }
            if (fmt == 6l) {
                let dk6 = f16_to_f32(uint(ksg[sb + 16l * mr + 2l * r]) | (uint(ksg[sb + 16l * mr + 2l * r + 1l]) << 8u))
                acc += float(isum - 32 * bsum) * (dk6 * xsp[sbi])
            } else {
                let dd = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
                let dm = f16_to_f32(uint(ksg[sb + 2l * mr + 2l * r]) | (uint(ksg[sb + 2l * mr + 2l * r + 1l]) << 8u))
                let d8 = xsp[sbi]
                acc += float(isum) * (dd * d8)
                acc -= float(bsum) * (dm * d8)
            }
        }
    }
    return acc
}

//! The per-format K-quant layout companions: plane interleave (mr) each format's stamped kernels
//! read. Each format tunes separately and takes its own winner's mr — k5/k6 tiles prefer grp4
//! while q8 crowns grp8 (M1, 2026-07-12). Reference bodies = grp4, declining in lockstep.
def k4q8_layout_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).interleave
}

def k5q8_layout_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).interleave
}

def k6q8_layout_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).interleave
}

def q40q8_layout_gen() : int {
    return q8q8_repack_type(GEMM_REFERENCE_MR).interleave
}

// the KernelBackend.kq_layout slot form of kq_layout_of (registered on both gen backends)
def private kq_layout_fmt_gen(fmt : int) : int => int(kq_layout_of(fmt))

//! The stamped plane interleave for a runtime-dispatched format (the fmt-branched wrappers
//! below take fmt as a value).
def kq_layout_of(fmt : int) : int64 {
    if (fmt == 4) {
        return int64(k4q8_layout_gen())
    }
    if (fmt == 5) {
        return int64(k5q8_layout_gen())
    }
    if (fmt == 40) {
        return int64(q40q8_layout_gen())
    }
    return int64(k6q8_layout_gen())
}

//! One row's dot off the grp<mr> q40 planes, scalar — the q40 stubs' reference body and the
//! tests' repack oracle (kq_grp_row_dot's q40 sibling; quant plane IS the k4 tiling, only the
//! scale plane differs). Same float fold order as dot_q40q8.
def q40_grp_row_dot(kqg : uint8 const?; ksg : uint8 const?; r, mr : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64) : float {
    var acc = 0.0
    let nsb = n / 256l
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 128l * mr
            let sb = sbi * 16l * mr
            var facc = 0.0
            for (blk in range64(8l)) {
                let b = sbi * 8l + blk
                var ilo = 0
                var ihi = 0
                for (j in range64(4l)) {
                    for (t in range64(4l)) {
                        let nib = uint(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        ilo += int(nib & 15u) * int(xqp[b * 32l + j * 4l + t])
                        ihi += int(nib >> 4u) * int(xqp[b * 32l + 16l + j * 4l + t])
                    }
                }
                let isub = ilo + ihi - 8 * (xbsp[b * 2l] + xbsp[b * 2l + 1l])
                let d = f16_to_f32(uint(ksg[sb + blk * 2l * mr + 2l * r]) | (uint(ksg[sb + blk * 2l * mr + 2l * r + 1l]) << 8u))
                facc += float(isub) * d
            }
            acc += facc * xsp[sbi]
        }
    }
    return acc
}

//! The K-quant GEMV kernels (kq stage 4): rows [rb, re) of one plane-region pair off the grp<mr>
//! kq planes (each format's OWN layout companion). Reference bodies = the scalar grp walk above,
//! declining in lockstep. Not `private`: they ARE the kq_rows_* slots.
[hint(unsafe_range_check, noalias = kqp, noalias = ksp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def k4q8_gemv_gen(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    let mr = int64(k4q8_layout_gen())
    let nsb = n / 256l
    unsafe {
        for (i in range64(rb, re)) {
            let g = i / mr
            yp[i] = kq_grp_row_dot(4l, kqp + g * mr * nsb * 128l, ksp + g * mr * nsb * 20l, i % mr, mr, xqp, xsp, xbsp, n)
        }
    }
}

[hint(unsafe_range_check, noalias = kqp, noalias = ksp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def k5q8_gemv_gen(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    let mr = int64(k5q8_layout_gen())
    let nsb = n / 256l
    unsafe {
        for (i in range64(rb, re)) {
            let g = i / mr
            yp[i] = kq_grp_row_dot(5l, kqp + g * mr * nsb * 160l, ksp + g * mr * nsb * 20l, i % mr, mr, xqp, xsp, xbsp, n)
        }
    }
}

[hint(unsafe_range_check, noalias = kqp, noalias = ksp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def k6q8_gemv_gen(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    let mr = int64(k6q8_layout_gen())
    let nsb = n / 256l
    unsafe {
        for (i in range64(rb, re)) {
            let g = i / mr
            yp[i] = kq_grp_row_dot(6l, kqp + g * mr * nsb * 192l, ksp + g * mr * nsb * 18l, i % mr, mr, xqp, xsp, xbsp, n)
        }
    }
}

[hint(unsafe_range_check, noalias = kqp, noalias = ksp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q40q8_gemv_gen(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    let mr = int64(q40q8_layout_gen())
    let nsb = n / 256l
    unsafe {
        for (i in range64(rb, re)) {
            let g = i / mr
            yp[i] = q40_grp_row_dot(kqp + g * mr * nsb * 128l, ksp + g * mr * nsb * 16l, i % mr, mr, xqp, xsp, xbsp, n)
        }
    }
}

// The kq tune grids (one [tune] family per format): kstep pinned 1, grid is (mr, nrsplit) per ISA
// tier. nrsplit token-splits the 4-token tile; nrsplit-4 (single unpack) wins wherever q-reg
// budget admits it. bias128/smmla/amx rows are absent — kq planes are natively unsigned.

//! The K-quant TILE kernels (kq batch family): 4 tokens x mr rows per call — kqg is the group's
//! QUANT panel (k4 the packed grp<mr> plane; k5/k6 a BYTE-EXPANDED scratch unpacked once per
//! (group, token-block)). One call is bit-exact vs 4 per-token GEMVs over the same weights.
[tune_perm(mr = 4), tune_perm(mr = 4, nrsplit = 2), tune_perm(mr = 8, nrsplit = 2),
 tune_perm(dot = "maddubs", width = 256, mr = 8, requires = "avx2"), tune_perm(dot = "maddubs", width = 256, mr = 8, nrsplit = 2, requires = "avx2"),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, requires = "avxvnni|avx512vnni"), tune_perm(dot = "vpdpbusd", width = 256, mr = 8, nrsplit = 2, requires = "avxvnni|avx512vnni"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, requires = "avx512vnni,avx512bw"), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, nrsplit = 2, requires = "avx512vnni,avx512bw"),
 tune_companion(fn = "k4q8_gemv_gen", gen = "dasllama_gemm_gen::k4_gemv"),
 tune_companion(fn = "k4q8_layout_gen", gen = "dasllama_gemm_gen::q8q8_layout"),
 tune(gen = "dasllama_gemm_gen::k4_tile",
      fallback = "dot_vpdpbusd_width512_mr16;dot_vpdpbusd_width256_mr8;dot_maddubs_width256_mr8;mr4"),
 hint(unsafe_range_check, noalias = kqg, noalias = ksg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def k4q8_tile_gen(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let mr = int64(k4q8_layout_gen())
    unsafe {
        for (t in range64(4l)) {
            for (r in range64(mr)) {
                yp[(t0 + t) * d + g * mr + r] = kq_grp_row_dot(4l, kqg, ksg, r, mr, xqp + (t0 + t) * n, xsp + (t0 + t) * (n / 256l), xbsp + (t0 + t) * (n / 16l), n)
            }
        }
    }
}

[tune_perm(mr = 4), tune_perm(mr = 4, nrsplit = 2), tune_perm(mr = 8, nrsplit = 2),
 tune_perm(dot = "maddubs", width = 256, mr = 8, requires = "avx2"), tune_perm(dot = "maddubs", width = 256, mr = 8, nrsplit = 2, requires = "avx2"),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, requires = "avxvnni|avx512vnni"), tune_perm(dot = "vpdpbusd", width = 256, mr = 8, nrsplit = 2, requires = "avxvnni|avx512vnni"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, requires = "avx512vnni,avx512bw"), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, nrsplit = 2, requires = "avx512vnni,avx512bw"),
 tune_companion(fn = "k5q8_gemv_gen", gen = "dasllama_gemm_gen::k5_gemv"),
 tune_companion(fn = "k5q8_layout_gen", gen = "dasllama_gemm_gen::q8q8_layout"),
 tune(gen = "dasllama_gemm_gen::k5_tile",
      fallback = "dot_vpdpbusd_width512_mr16;dot_vpdpbusd_width256_mr8;dot_maddubs_width256_mr8;mr4"),
 hint(unsafe_range_check, noalias = kqg, noalias = ksg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def k5q8_tile_gen(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let mr = int64(k5q8_layout_gen())
    unsafe {
        for (t in range64(4l)) {
            for (r in range64(mr)) {
                yp[(t0 + t) * d + g * mr + r] = kq_grp_row_dot_b(5l, kqg, ksg, r, mr, xqp + (t0 + t) * n, xsp + (t0 + t) * (n / 256l), xbsp + (t0 + t) * (n / 16l), n)
            }
        }
    }
}

[tune_perm(mr = 4), tune_perm(mr = 4, nrsplit = 2), tune_perm(mr = 8, nrsplit = 2),
 tune_perm(dot = "maddubs", width = 256, mr = 8, requires = "avx2"), tune_perm(dot = "maddubs", width = 256, mr = 8, nrsplit = 2, requires = "avx2"),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, requires = "avxvnni|avx512vnni"), tune_perm(dot = "vpdpbusd", width = 256, mr = 8, nrsplit = 2, requires = "avxvnni|avx512vnni"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, requires = "avx512vnni,avx512bw"), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, nrsplit = 2, requires = "avx512vnni,avx512bw"),
 tune_companion(fn = "k6q8_gemv_gen", gen = "dasllama_gemm_gen::k6_gemv"),
 tune_companion(fn = "k6q8_layout_gen", gen = "dasllama_gemm_gen::q8q8_layout"),
 tune(gen = "dasllama_gemm_gen::k6_tile",
      fallback = "dot_vpdpbusd_width512_mr16;dot_vpdpbusd_width256_mr8;dot_maddubs_width256_mr8;mr4"),
 hint(unsafe_range_check, noalias = kqg, noalias = ksg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def k6q8_tile_gen(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let mr = int64(k6q8_layout_gen())
    unsafe {
        for (t in range64(4l)) {
            for (r in range64(mr)) {
                yp[(t0 + t) * d + g * mr + r] = kq_grp_row_dot_b(6l, kqg, ksg, r, mr, xqp + (t0 + t) * n, xsp + (t0 + t) * (n / 256l), xbsp + (t0 + t) * (n / 16l), n)
            }
        }
    }
}

[tune_perm(mr = 4), tune_perm(mr = 4, nrsplit = 2), tune_perm(mr = 8, nrsplit = 2),
 tune_perm(dot = "maddubs", width = 256, mr = 8, requires = "avx2"), tune_perm(dot = "maddubs", width = 256, mr = 8, nrsplit = 2, requires = "avx2"),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, requires = "avxvnni|avx512vnni"), tune_perm(dot = "vpdpbusd", width = 256, mr = 8, nrsplit = 2, requires = "avxvnni|avx512vnni"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, requires = "avx512vnni,avx512bw"), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, nrsplit = 2, requires = "avx512vnni,avx512bw"),
 tune_companion(fn = "q40q8_gemv_gen", gen = "dasllama_gemm_gen::q40_gemv"),
 tune_companion(fn = "q40q8_layout_gen", gen = "dasllama_gemm_gen::q8q8_layout"),
 tune(gen = "dasllama_gemm_gen::q40_tile",
      fallback = "dot_vpdpbusd_width512_mr16;dot_vpdpbusd_width256_mr8;dot_maddubs_width256_mr8;mr4"),
 hint(unsafe_range_check, noalias = kqg, noalias = ksg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q40q8_tile_gen(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let mr = int64(q40q8_layout_gen())
    unsafe {
        for (t in range64(4l)) {
            for (r in range64(mr)) {
                yp[(t0 + t) * d + g * mr + r] = q40_grp_row_dot(kqg, ksg, r, mr, xqp + (t0 + t) * n, xsp + (t0 + t) * (n / 256l), xbsp + (t0 + t) * (n / 16l), n)
            }
        }
    }
}

//! The f16-scale GEMV companion (wscale_f16 rail): q8q8_gemv_gen's twin over the raw binary16
//! group-scale plane. Same rows-range contract; the generated body differs only in the scale
//! fold's widen (fcvtl/vcvtph2ps). Not `private`: it IS the mm_rows_s16 slot.
[hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_gemv_s16_gen(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    let nb = n / 32l
    unsafe {
        for (g in range64(rb / 4l, re / 4l)) {
            let f = dot_q8q8_laneq4_f16s(wp + g * 4l * n, sp + g * 4l * nb, xqp, xsp, n)
            yp[g * 4l + 0l] = f.x
            yp[g * 4l + 1l] = f.y
            yp[g * 4l + 2l] = f.z
            yp[g * 4l + 3l] = f.w
        }
    }
}

//! The f16-scale tile companion: q8q8_tile_gen's twin over the binary16 group-scale plane — same
//! 4-token contract, same stamped perm (bias128/amx/smmla included), only the scale-fold load
//! differs. Not `private`: called from inside the lifted parallel_for worker lambdas.
[unused_argument(xbsp), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q8q8_tile_s16_gen(var yp : float?; wg : int8 const?; sg : uint16 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let nb = n / 32l
    unsafe {
        for (t in range64(4l)) {
            let f = dot_q8q8_laneq4_f16s(wg, sg, xqp + (t0 + t) * n, xsp + (t0 + t) * nb, n)
            let r = g * 4l
            yp[(t0 + t) * d + r + 0l] = f.x
            yp[(t0 + t) * d + r + 1l] = f.y
            yp[(t0 + t) * d + r + 2l] = f.z
            yp[(t0 + t) * d + r + 3l] = f.w
        }
    }
}

//! The generated tile: mr interleaved rows x 4 tokens (the q8q8_repack_type(mr) layout). Reference
//! body = 4 single-token laneq4 dots, runs on interp/AOT/non-arm64 JIT/generator-declined; ignores
//! xbsp unless a biased stamp reads it. dot=/width= rows are the x64 legs (slice F), one grid per ISA.
[tune_perm(kstep = 1), tune_perm(kstep = 2), tune_perm(kstep = 4), tune_perm(kstep = 2, nrsplit = 2),
 tune_perm(kstep = 1, nrsplit = 2), tune_perm(kstep = 4, nrsplit = 2),
 tune_perm(kstep = 1, nrsplit = 2, mr = 8), tune_perm(kstep = 2, nrsplit = 2, mr = 8), tune_perm(kstep = 4, nrsplit = 2, mr = 8),
 tune_perm(kstep = 2, gkstep = 2), tune_perm(kstep = 2, gkstep = 4),
 tune_perm(kstep = 4, nrsplit = 2, mr = 8, gkstep = 2), tune_perm(kstep = 4, nrsplit = 2, mr = 8, gkstep = 4),
 tune_perm(dot = "maddubs", width = 256, mr = 8, kstep = 2, requires = "avx2"), tune_perm(dot = "maddubs", width = 256, mr = 8, kstep = 4, nrsplit = 2),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, kstep = 2), tune_perm(dot = "vpdpbusd", width = 256, mr = 8, kstep = 4, nrsplit = 2),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 16, kstep = 2, nrsplit = 2), tune_perm(dot = "vpdpbssd", width = 256, mr = 8, kstep = 2),
 tune_perm(dot = "maddubs", width = 512, mr = 16, kstep = 2), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, kstep = 2),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, kstep = 4, nrsplit = 2), tune_perm(dot = "vpdpbusd", width = 512, mr = 32, kstep = 2, nrsplit = 2),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, kstep = 2, gkstep = 2), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, gkstep = 2),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8, kstep = 2, bias = 128), tune_perm(dot = "vpdpbusd", width = 256, mr = 8, kstep = 2, gkstep = 2, bias = 128, requires = "avxvnni|avx512vnni"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, bias = 128), tune_perm(dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, gkstep = 2, bias = 128, requires = "avx512vnni,avx512bw"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, kstep = 4, nrsplit = 2, bias = 128),
 tune_perm(dot = "maddubs", width = 256, mr = 8, kstep = 2, bias = 128),
 tune_perm(dot = "smmla", mr = 4, kstep = 2, requires = "i8mm"), tune_perm(dot = "smmla", mr = 4, kstep = 2, nrsplit = 2, requires = "i8mm"),
 tune_perm(dot = "smmla", mr = 8, kstep = 2, nrsplit = 2, requires = "i8mm"), tune_perm(dot = "smmla", mr = 8, kstep = 4, nrsplit = 2, requires = "i8mm"),
 tune_perm(dot = "smmla", mr = 8, kstep = 2, nrsplit = 2, gkstep = 2, requires = "i8mm"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 1, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, gkstep = 2, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, pipe = 1, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, latch = 1, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, bias = 128, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, pipe = 1, latch = 1, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, pipe = 1, bias = 128, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, latch = 1, bias = 128, requires = "amx-int8"),
 tune_perm(dot = "amx_int8", width = 512, mr = 16, kstep = 1, nrsplit = 2, pipe = 1, latch = 1, bias = 128, requires = "amx-int8"),
 tune_perm(suffix = "mr8_budget", mr = 8), tune_perm(suffix = "mr2_lanes", mr = 2), tune_perm(dot = "vpdpbusd"),
 tune_companion(fn = "q8q8_layout_gen", gen = "dasllama_gemm_gen::q8q8_layout"),
 tune_companion(fn = "q8q8_wbias_gen", gen = "dasllama_gemm_gen::q8q8_wbias"),
 tune_companion(fn = "q8q8_kgroup_gen", gen = "dasllama_gemm_gen::q8q8_kgroup"),
 tune_companion(fn = "q8q8_tokstep_gen", gen = "dasllama_gemm_gen::q8q8_tokstep"),
 tune_companion(fn = "q8q8_amx_cfg_gen", gen = "dasllama_gemm_gen::q8q8_amx_cfg"),
 tune_companion(fn = "q8q8_gemv_gen", gen = "dasllama_gemm_gen::q8q8_gemv"),
 tune_companion(fn = "q8q8_tile_s16_gen", gen = "dasllama_gemm_gen::q8q8_tile_s16"),
 tune_companion(fn = "q8q8_gemv_s16_gen", gen = "dasllama_gemm_gen::q8q8_gemv_s16"),
 tune_companion(fn = "mx4q8_gemv_gen", gen = "dasllama_gemm_gen::mx4_gemv"),
 tune_companion(fn = "mx4q8_tile_gen", gen = "dasllama_gemm_gen::mx4_tile"),
 tune_companion(fn = "q8q8_family_live", gen = "dasllama_gemm_gen::q8q8_witness"),
 tune(gen = "dasllama_gemm_gen::q8q8_tile",
      fallback = "dot_vpdpbusd_width512_mr16_kstep2_gkstep2_bias128;dot_vpdpbusd_width256_mr8_kstep2_gkstep2_bias128;dot_maddubs_width256_mr8_kstep2;kstep2"), unused_argument(xbsp),
 hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q8q8_tile_gen(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    let nb = n / 32l
    unsafe {
        for (t in range64(4l)) {
            let f = dot_q8q8_laneq4(wg, sg, xqp + (t0 + t) * n, xsp + (t0 + t) * nb, n)
            let r = g * 4l
            yp[(t0 + t) * d + r + 0l] = f.x
            yp[(t0 + t) * d + r + 1l] = f.y
            yp[(t0 + t) * d + r + 2l] = f.z
            yp[(t0 + t) * d + r + 3l] = f.w
        }
    }
}

//! mr-generalized Q8 interleave repack — the q8q8_repack_type(mr) layout for any mr (mr=4
//! reproduces the old hand arm64-laneq interleave byte-for-byte). Group bytes biased by wbias
//! (bias128 stamps bake w^0x80); tail rows stay row-major AND unbiased. Runs once per tensor at load.
def repack_q8q8_grp(var wp : int8?; var sp : float?; n, d, mr : int64; wbias : int64 = 0l; kgroup : int64 = 4l) {
    let nb = n / 32l
    let ng = d / mr
    let ncg = n / kgroup
    var tw : array<int8>
    var ts : array<float>
    tw |> resize(int(d * n))
    ts |> resize(int(d * nb))
    unsafe {
        var twp = addr(tw[0])
        var tsp = addr(ts[0])
        for (i in range64(d * n)) {
            twp[i] = wp[i]
        }
        for (i in range64(d * nb)) {
            tsp[i] = sp[i]
        }
        for (g in range64(ng)) {
            let gbase = g * mr * n
            for (kg in range64(ncg)) {
                let dst = gbase + kg * kgroup * mr
                for (r in range64(mr)) {
                    let src = (g * mr + r) * n + kg * kgroup
                    for (j in range64(kgroup)) {
                        wp[dst + r * kgroup + j] = twp[src + j]
                    }
                }
            }
            let sgbase = g * mr * nb
            for (bi in range64(nb)) {
                for (r in range64(mr)) {
                    sp[sgbase + bi * mr + r] = tsp[(g * mr + r) * nb + bi]
                }
            }
        }
        if (wbias != 0l) {
            // bias128: one pass over the GROUP region only (tails stay plain s8);
            // int8 truncation makes +128 the byte xor 0x80
            for (i in range64(ng * mr * n)) {
                wp[i] = int8(int(wp[i]) + int(wbias))
            }
        }
    }
    delete tw
    delete ts
}

// the backend repack slot: layout/wbias/kgroup companions decide interleave, plane bias, and
// k-byte grouping — so a bias128-mr16 stamp gets biased grp16 weights and an smmla stamp gets
// the kg8 row-pair plane from the same load path that gives grp4 its plain weights
def private repack_q8q8_gen(var wp : int8?; var sp : float?; n, d : int64) {
    repack_q8q8_grp(wp, sp, n, d, int64(q8q8_layout_gen()), int64(q8q8_wbias_gen()), int64(q8q8_kgroup_gen()))
}

//! mr-generalized MXFP4 plane repack (mr=4 reproduces the old hand arm64-laneq mx interleave
//! byte-for-byte). Nibble layout maps byte-for-byte onto the grp<mr> Q8 order (see
//! expand_mx4_region_q8); tail rows (d % mr) stay row-major.
def repack_mx4_grp(var np : uint8?; var ep : uint8?; n, d, mr : int64) {
    let nrow = n / 2l    // nibble bytes per row
    let nbb = n / 32l    // blocks per row
    let ng = d / mr
    var tn : array<uint8>
    var te : array<uint8>
    tn |> resize(int(d * nrow))
    te |> resize(int(d * nbb))
    unsafe {
        var tnp = addr(tn[0])
        var tep = addr(te[0])
        for (i in range64(d * nrow)) {
            tnp[i] = np[i]
        }
        for (i in range64(d * nbb)) {
            tep[i] = ep[i]
        }
        for (g in range64(ng)) {
            let gnb = g * mr * nrow
            for (bi in range64(nbb)) {
                let dst = gnb + bi * 16l * mr
                for (j in range64(4l)) {
                    for (r in range64(mr)) {
                        let src = (g * mr + r) * nrow + bi * 16l + j * 4l
                        let dj = dst + j * 4l * mr + r * 4l
                        np[dj + 0l] = tnp[src + 0l]
                        np[dj + 1l] = tnp[src + 1l]
                        np[dj + 2l] = tnp[src + 2l]
                        np[dj + 3l] = tnp[src + 3l]
                    }
                }
            }
            let geb = g * mr * nbb
            for (bi in range64(nbb)) {
                for (r in range64(mr)) {
                    ep[geb + bi * mr + r] = tep[(g * mr + r) * nbb + bi]
                }
            }
        }
    }
    delete tn
    delete te
}

// the backend mx4 repack slot: same layout companion as the Q8 repack — planes and weights
// interleave at the ONE stamped mr
def private repack_mx4_gen(var np : uint8?; var ep : uint8?; n, d : int64) {
    repack_mx4_grp(np, ep, n, d, int64(q8q8_layout_gen()))
}

// ===== K-quant grp<mr> repacks (kq stage 4): the layouts the stamped k4/k5/k6 rows cores read,
// per superblock per mr-row group (tail rows d % mr stay disk-order, served by the portable
// dots). Quant/high/scale byte layouts differ per format — see repack_k4/k5/k6_grp bodies. =====

// low 4 bits of weight k off a disk-order k4/k5 nibble row (128 bytes per superblock)
def private k45_nib(rowq : uint8 const?; k : int64) : uint {
    let js = k / 64l
    let rem = k % 64l
    let b = uint(unsafe(rowq[js * 32l + rem % 32l]))
    return rem < 32l ? b & 15u : b >> 4u
}

// the 5th bit of weight k off a disk-order k5 qh row (32 bytes per superblock)
def private k5_hbit(rowh : uint8 const?; k : int64) : uint {
    let js = k / 64l
    let rem = k % 64l
    return (uint(unsafe(rowh[rem % 32l])) >> uint(2l * js + (rem < 32l ? 0l : 1l))) & 1u
}

// low 4 bits of weight k off a disk-order k6 ql row (128 bytes per superblock)
def private k6_nib(rowq : uint8 const?; k : int64) : uint {
    let h = k / 128l
    let g = (k % 128l) / 32l
    let b = uint(unsafe(rowq[h * 64l + (g % 2l) * 32l + k % 32l]))
    return g < 2l ? b & 15u : b >> 4u
}

//! mr-generalized K-quant plane repack, one function per format. Runs once per kq weight tensor
//! at load. k4/k5 (kq v2) also DECODE the 6-bit scale packing into the u8 sc/mn planes — the
//! padded-disk 20B rows and the decoded 20B/row grp regions share one stride.
def repack_k4_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 128l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(int(d * qrow))
    ts |> resize(int(d * srow))
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        for (i in range64(d * qrow)) {
            tqp[i] = kq[i]
        }
        for (i in range64(d * srow)) {
            tsp[i] = ks[i]
        }
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 128l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 128l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k45_nib(tqp + sq, k)
                                let hi = k45_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                            }
                        }
                        let sm = k4_sc_mn(tsp + ss + 4l, blk)
                        ks[ds + 4l * mr + blk * mr + r] = uint8(sm.x)
                        ks[ds + 12l * mr + blk * mr + r] = uint8(sm.y)
                    }
                    ks[ds + 2l * r] = tsp[ss]              // f16 d
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]       // f16 dmin
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                }
            }
        }
    }
    delete tq
    delete ts
}

def repack_k5_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 160l
    let srow = nsb * 20l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(int(d * qrow))
    ts |> resize(int(d * srow))
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        for (i in range64(d * qrow)) {
            tqp[i] = kq[i]
        }
        for (i in range64(d * srow)) {
            tsp[i] = ks[i]
        }
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 160l * mr
                let ds = g * mr * srow + sbi * 20l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 160l
                    let ss = (g * mr + r) * srow + sbi * 20l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            var hb = 0u
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k45_nib(tqp + sq, k)
                                let hi = k45_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                                hb |= k5_hbit(tqp + sq + 128l, k) << uint(t)
                                hb |= k5_hbit(tqp + sq + 128l, k + 16l) << uint(4l + t)
                            }
                            kq[dq + 128l * mr + (blk * 4l + j) * mr + r] = uint8(hb)
                        }
                        let sm = k4_sc_mn(tsp + ss + 4l, blk)
                        ks[ds + 4l * mr + blk * mr + r] = uint8(sm.x)
                        ks[ds + 12l * mr + blk * mr + r] = uint8(sm.y)
                    }
                    ks[ds + 2l * r] = tsp[ss]              // f16 d
                    ks[ds + 2l * r + 1l] = tsp[ss + 1l]
                    ks[ds + 2l * mr + 2l * r] = tsp[ss + 2l]       // f16 dmin
                    ks[ds + 2l * mr + 2l * r + 1l] = tsp[ss + 3l]
                }
            }
        }
    }
    delete tq
    delete ts
}

def repack_k6_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    let nsb = n / 256l
    let qrow = nsb * 192l
    let srow = nsb * 18l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(int(d * qrow))
    ts |> resize(int(d * srow))
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        for (i in range64(d * qrow)) {
            tqp[i] = kq[i]
        }
        for (i in range64(d * srow)) {
            tsp[i] = ks[i]
        }
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 192l * mr
                let ds = g * mr * srow + sbi * 18l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 192l
                    let ss = (g * mr + r) * srow + sbi * 18l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                let k = blk * 32l + j * 4l + t
                                let lo = k6_nib(tqp + sq, k)
                                let hi = k6_nib(tqp + sq, k + 16l)
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = uint8(lo | (hi << 4u))
                            }
                        }
                    }
                    for (h in range64(2l)) {   // qh bytes verbatim, row-interleaved by 4-byte columns
                        for (jj in range64(8l)) {
                            for (t in range64(4l)) {
                                kq[dq + 128l * mr + ((h * 8l + jj) * mr + r) * 4l + t] = tqp[sq + 128l + h * 32l + jj * 4l + t]
                            }
                        }
                    }
                    for (idx in range64(16l)) {
                        ks[ds + idx * mr + r] = tsp[ss + idx]
                    }
                    ks[ds + 16l * mr + 2l * r] = tsp[ss + 16l]
                    ks[ds + 16l * mr + 2l * r + 1l] = tsp[ss + 17l]
                }
            }
        }
    }
    delete tq
    delete ts
}

def repack_q40_grp(var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    // q40 grp layout: quants land byte-for-byte in the k4 tiling — the disk bytes already carry
    // the k / k+16 nibble pairing, so byte t of (blk, j) is disk byte blk*16 + j*4 + t (a pure
    // gather); scales interleave per block: [blk 0..7][mr x f16 d]
    let nsb = n / 256l
    let qrow = nsb * 128l
    let srow = nsb * 16l
    let ng = d / mr
    var tq : array<uint8>
    var ts : array<uint8>
    tq |> resize(int(d * qrow))
    ts |> resize(int(d * srow))
    unsafe {
        var tqp = addr(tq[0])
        var tsp = addr(ts[0])
        for (i in range64(d * qrow)) {
            tqp[i] = kq[i]
        }
        for (i in range64(d * srow)) {
            tsp[i] = ks[i]
        }
        for (g in range64(ng)) {
            for (sbi in range64(nsb)) {
                let dq = g * mr * qrow + sbi * 128l * mr
                let ds = g * mr * srow + sbi * 16l * mr
                for (r in range64(mr)) {
                    let sq = (g * mr + r) * qrow + sbi * 128l
                    let ss = (g * mr + r) * srow + sbi * 16l
                    for (blk in range64(8l)) {
                        for (j in range64(4l)) {
                            for (t in range64(4l)) {
                                kq[dq + ((blk * 4l + j) * mr + r) * 4l + t] = tqp[sq + blk * 16l + j * 4l + t]
                            }
                        }
                        ks[ds + blk * 2l * mr + 2l * r] = tsp[ss + blk * 2l]        // f16 d
                        ks[ds + blk * 2l * mr + 2l * r + 1l] = tsp[ss + blk * 2l + 1l]
                    }
                }
            }
        }
    }
    delete tq
    delete ts
}

//! Unpack ONE group's packed grp<mr> k5/k6 quant panel into the BYTE-EXPANDED tile form (wlo/whi
//! kqBytes stamps load verbatim). SWAR over uint64 lanes: k5 deposit uses an add-carry test
//! ((x+0x7F)&0x80) to fold the high bit; k6 tops sit at a uniform shift per block.
[hint(unsafe_range_check, noalias = kqg, noalias = dst)]
def unpack_kq_panel_grp(fmt : int64; kqg : uint8 const?; var dst : uint8?; mr, nsb : int64) {
    let LO4 = 0x0F0F0F0F0F0F0F0Ful
    let SEL = 0x0804020108040201ul   // LE lane t (0..3, twice) selects high-bit t
    let C7F = 0x7F7F7F7F7F7F7F7Ful
    let C80 = 0x8080808080808080ul
    let TOP2 = 0x0303030303030303ul
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * (fmt == 5l ? 160l : 192l) * mr
            let ob = sbi * 256l * mr
            for (blk in range64(8l)) {
                let g2 = uint64((blk % 4l) * 2l)
                let hbase = qb + 128l * mr + (blk / 4l) * 32l * mr   // k6 high plane (k5 ignores)
                for (j in range64(4l)) {
                    let gcol = ((blk * 4l + j) * mr) * 4l
                    var r4 = 0l
                    while (r4 < mr) {
                        let np = kqg + (qb + gcol + r4 * 4l)
                        let nw = reinterpret<uint64 const?>(np)
                        var lop = dst + (ob + gcol + r4 * 4l)
                        var hip = dst + (ob + 128l * mr + gcol + r4 * 4l)
                        var low = reinterpret<uint64?>(lop)
                        var hiw = reinterpret<uint64?>(hip)
                        if (fmt == 5l) {
                            let hp = kqg + (qb + 128l * mr + (blk * 4l + j) * mr + r4)
                            let hb = reinterpret<uint const?>(hp)[0]   // 4 rows' high-bit bytes
                            for (w in range64(2l)) {
                                let n = nw[w]
                                let b0 = uint64((hb >> uint(16l * w)) & 0xFFu) * 0x01010101ul
                                let b1 = uint64((hb >> uint(16l * w + 8l)) & 0xFFu) * 0x01010101ul
                                let hbw = b0 | (b1 << 32ul)
                                let dl = (((hbw & SEL) + C7F) & C80) >> 3ul
                                let dh = ((((hbw >> 4ul) & SEL) + C7F) & C80) >> 3ul
                                low[w] = (n & LO4) | dl
                                hiw[w] = ((n >> 4ul) & LO4) | dh
                            }
                        } else {
                            let h0w = reinterpret<uint64 const?>(kqg + (hbase + (j * mr + r4) * 4l))
                            let h1w = reinterpret<uint64 const?>(kqg + (hbase + ((j + 4l) * mr + r4) * 4l))
                            for (w in range64(2l)) {
                                let n = nw[w]
                                low[w] = (n & LO4) | (((h0w[w] >> g2) & TOP2) << 4ul)
                                hiw[w] = ((n >> 4ul) & LO4) | (((h1w[w] >> g2) & TOP2) << 4ul)
                            }
                        }
                        r4 += 4l
                    }
                }
            }
        }
    }
}

//! kq_grp_row_dot's twin over a BYTE-EXPANDED panel (unpack_kq_panel_grp's output) — the k5/k6
//! tile wrappers' reference body, bit-exact against the packed-plane dot. fmt picks the scale
//! form only (5 = u8 sc/mn + d/dmin, 6 = signed per-16 sub-scales + d).
def kq_grp_row_dot_b(fmt : int64; kqg : uint8 const?; ksg : uint8 const?; r, mr : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64) : float {
    var acc = 0.0
    let nsb = n / 256l
    let ssb = fmt == 6l ? 18l : 20l
    unsafe {
        for (sbi in range64(nsb)) {
            let qb = sbi * 256l * mr
            let sb = sbi * ssb * mr
            var isum = 0
            var bsum = 0
            for (blk in range64(8l)) {
                let b = sbi * 8l + blk
                var ilo = 0
                var ihi = 0
                for (j in range64(4l)) {
                    for (t in range64(4l)) {
                        let qlo = int(kqg[qb + ((blk * 4l + j) * mr + r) * 4l + t])
                        let qhi = int(kqg[qb + 128l * mr + ((blk * 4l + j) * mr + r) * 4l + t])
                        ilo += qlo * int(xqp[b * 32l + j * 4l + t])
                        ihi += qhi * int(xqp[b * 32l + 16l + j * 4l + t])
                    }
                }
                if (fmt == 6l) {
                    let s0 = int(int8(ksg[sb + 2l * blk * mr + r]))
                    let s1 = int(int8(ksg[sb + (2l * blk + 1l) * mr + r]))
                    isum += s0 * ilo + s1 * ihi
                    bsum += s0 * xbsp[b * 2l] + s1 * xbsp[b * 2l + 1l]
                } else {
                    let sc = int(ksg[sb + 4l * mr + blk * mr + r])
                    let mn = int(ksg[sb + 12l * mr + blk * mr + r])
                    isum += sc * (ilo + ihi)
                    bsum += mn * (xbsp[b * 2l] + xbsp[b * 2l + 1l])
                }
            }
            if (fmt == 6l) {
                let dk6 = f16_to_f32(uint(ksg[sb + 16l * mr + 2l * r]) | (uint(ksg[sb + 16l * mr + 2l * r + 1l]) << 8u))
                acc += float(isum - 32 * bsum) * (dk6 * xsp[sbi])
            } else {
                let dd = f16_to_f32(uint(ksg[sb + 2l * r]) | (uint(ksg[sb + 2l * r + 1l]) << 8u))
                let dm = f16_to_f32(uint(ksg[sb + 2l * mr + 2l * r]) | (uint(ksg[sb + 2l * mr + 2l * r + 1l]) << 8u))
                let d8 = xsp[sbi]
                acc += float(isum) * (dd * d8)
                acc -= float(bsum) * (dm * d8)
            }
        }
    }
    return acc
}

// the backend kq repack slot: same layout companion as the Q8/mx4 repacks — one stamped mr
def private repack_kq_gen(fmt : int; var kq : uint8?; var ks : uint8?; n, d : int64) {
    if (fmt == 4) {
        repack_k4_grp(kq, ks, n, d, int64(k4q8_layout_gen()))
    } elif (fmt == 5) {
        repack_k5_grp(kq, ks, n, d, int64(k5q8_layout_gen()))
    } elif (fmt == 40) {
        repack_q40_grp(kq, ks, n, d, int64(q40q8_layout_gen()))
    } else {
        repack_k6_grp(kq, ks, n, d, int64(k6q8_layout_gen()))
    }
}

//! Generic grp<mr> single-token dot for mr != 4 slots and tails: scalar, correct anywhere, same
//! per-block fold order as the tile. wbias un-biases bias128 planes byte-wise (identity at
//! wbias=0); kgroup selects the plane's k-byte grouping (4 = dword, 8 = smmla row-pair).
def q8q8_token_grp_generic(var yp : float?; wg : int8 const?; sg : float const?; xq : int8 const?; xs : float const?; n, d, col, trow, mr : int64; wbias : int64 = 0l; kgroup : int64 = 4l) {
    let nb = n / 32l
    let wb0 = int(wbias)
    var f : float[32]   // scratch cap = the grid's max stampable mr (busd512 mr32 row)
    for (r in range(int(mr))) {
        f[r] = 0.0
    }
    unsafe {
        for (bi in range64(nb)) {
            let wb = bi * mr * 32l
            let xb = bi * 32l
            let xsb = xs[bi]
            for (r in range64(mr)) {
                var acc = 0
                for (kg in range64(8l)) {
                    let wo = (kgroup == 8l
                        ? wb + (kg / 2l) * 8l * mr + r * 8l + (kg % 2l) * 4l
                        : wb + kg * 4l * mr + r * 4l)
                    let xo = xb + kg * 4l
                    acc += int(int8(int(wg[wo + 0l]) + wb0)) * int(xq[xo + 0l])
                    acc += int(int8(int(wg[wo + 1l]) + wb0)) * int(xq[xo + 1l])
                    acc += int(int8(int(wg[wo + 2l]) + wb0)) * int(xq[xo + 2l])
                    acc += int(int8(int(wg[wo + 3l]) + wb0)) * int(xq[xo + 3l])
                }
                f[int(r)] += float(acc) * sg[bi * mr + r] * xsb
            }
        }
        for (r in range64(mr)) {
            yp[trow * d + col + r] = f[int(r)]
        }
    }
}

// One (unit range x token range) cell of the interleaved batch walk (TB blocks outer, groups
// inner): the 1-D arm calls it once with the full token range, the 2-D grid arm once per cell,
// so both arms share one walk and stay byte-identical.
def private q8q8_batch_cell_gen(var myp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, nb, mr, TB, kg, wbias : int64; ub, ue : int; tb0, te : int64) {
    unsafe {
        var tb = tb0
        while (tb < te) {
            let tbe = min(tb + TB, te)
            let tb4 = tb + ((tbe - tb) / 4l) * 4l
            for (gi in range(ub, ue)) {
                let g = int64(gi)
                let wgp = wp + g * mr * n
                let sgp = sp + g * mr * nb
                var tk = tb
                while (tk < tb4) {
                    q8q8_tile_gen(myp, wgp, sgp, xqp, xsp, xbsp, n, d, g, tk)
                    tk += 4l
                }
                if (mr == 4l && kg == 4l) {   // an smmla mr4 stamp's plane is kg8 — generic tail below
                    while (tk < tbe) {
                        let f = dot_q8q8_laneq4(wgp, sgp, xqp + tk * n, xsp + tk * nb, n)
                        myp[tk * d + g * 4l + 0l] = f.x
                        myp[tk * d + g * 4l + 1l] = f.y
                        myp[tk * d + g * 4l + 2l] = f.z
                        myp[tk * d + g * 4l + 3l] = f.w
                        tk++
                    }
                } else {
                    while (tk < tbe) {
                        q8q8_token_grp_generic(myp, wgp, sgp, xqp + tk * n, xsp + tk * nb, n, d, g * mr, tk, mr, wbias, kg)
                        tk++
                    }
                }
            }
            tb += TB
        }
    }
}

// One (unit range x token range) cell of the amx-shaped walk (slice K I2 — the ts!=4 twin of
// q8q8_batch_cell_gen). Interior token cells are whole-macro; a ragged tail rides ONE overlapped
// macro when it stays inside this cell's range (recompute is idempotent), else vector rows-core.
def private q8q8_batch_amx_cell_gen(var myp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, nb, mr, TBa, ts, gs : int64; ub, ue : int; tb0, te : int64) {
    unsafe {
        var tb = tb0
        while (tb < te) {
            let tbe = min(tb + TBa, te)
            let tbS = tb + ((tbe - tb) / ts) * ts
            for (ui in range(ub, ue)) {
                let gg = int64(ui) * gs
                let wgp = wp + gg * mr * n
                let sgp = sp + gg * mr * nb
                var tk = tb
                while (tk < tbS) {
                    q8q8_tile_gen(myp, wgp, sgp, xqp, xsp, xbsp, n, d, gg, tk)
                    tk += ts
                }
                if (tk < tbe) {
                    if (tbe - ts >= tb0) {
                        q8q8_tile_gen(myp, wgp, sgp, xqp, xsp, xbsp, n, d, gg, tbe - ts)
                    } else {
                        while (tk < tbe) {
                            q8q8_gemv_gen(myp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, gg * mr, (gg + gs) * mr)
                            tk++
                        }
                    }
                }
            }
            tb += TBa
        }
    }
}

// The interleaved batch GEMM. Token-blocked: TB tokens outer, groups inner, so the TB×n
// activation slice stays L2-resident (effective_token_block clamps TB to the per-box L2 budget —
// above ~L2/n it spills and prefill cliffs). Strides come off the layout/tokstep companions.
def private q8q8_batch_kernel_neon_laneq_gen(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let wbias = int64(q8q8_wbias_gen())
    let kg = int64(q8q8_kgroup_gen())
    let ng = d / mr
    var myp = yp
    let TB = effective_token_block(g_q8_token_block, n)
    var bs : array<int>
    if (wbias != 0l) {
        bs |> resize(int(ntok * nb))
        unsafe {
            var bsp = addr(bs[0])
            for (t in range64(ntok)) {
                let xrow = xqp + t * n
                for (bi in range64(nb)) {
                    var s = 0
                    let xb = bi * 32l
                    for (j in range64(32l)) {
                        s += int(xrow[xb + j])
                    }
                    bsp[t * nb + bi] = -128 * s
                }
            }
        }
    }
    let xbsp : int const? = wbias != 0l ? unsafe(addr(bs[0])) : null
    let ts = int64(q8q8_tokstep_gen())   // tokens per tile call (4; 16/32 on amx stamps)
    if (ts != 4l) {
        // amx-shaped walk (slice I): one tile call covers ts tokens x gs row-groups; a ragged tail rides one overlapped macro (idempotent) or falls back to the vector rows-core.
        let gs = ts / 16l                // row-groups per call — amx macros are square in tile units
        let ngu = ng / gs
        let TBa = max(TB, ts)
        let grid = matmul_grid(int(ngu), gs * mr, get_q8_batch_chunks_per_job(), n * d * ntok, ntok, int(ts))
        if (grid.y <= 1) {
            maybe_parallel_for(0, int(ngu), grid.x) $(ub, ue) {
                q8q8_amx_cfg_gen()      // latch stamps: load the TMUL palette on this worker
                q8q8_batch_amx_cell_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TBa, ts, gs, ub, ue, 0l, ntok)
            }
        } else {
            // 2-D grid on the amx walk (slice K I2 — 18 macro units for 48 lanes was 2x starved vs 1-D): one claim per (unit x token) cell, rows-fastest decode.
            let ntile = ntok / ts
            let cells = grid.x * grid.y
            maybe_parallel_for(0, cells, cells) $(cb, ce) {
                q8q8_amx_cfg_gen()      // latch stamps: load the TMUL palette on this worker
                for (ci in range(cb, ce)) {
                    let ri = ci % grid.x
                    let ti = ci / grid.x
                    let ub = int(int64(ri) * ngu / int64(grid.x))
                    let ue = int(int64(ri + 1) * ngu / int64(grid.x))
                    let ctb = int64(ti) * ntile / int64(grid.y) * ts
                    let cte = ti == grid.y - 1 ? ntok : int64(ti + 1) * ntile / int64(grid.y) * ts
                    q8q8_batch_amx_cell_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TBa, ts, gs, ub, ue, ctb, cte)
                }
            }
        }
        if (ngu * gs < ng) {
            // leftover row-groups (ng % gs, 16- but not 32-multiple dims): vector rows-core per token
            unsafe {
                for (tk in range64(ntok)) {
                    q8q8_gemv_gen(myp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, ngu * gs * mr, ng * mr)
                }
            }
        }
    } else {
        let grid = matmul_grid(int(ng), mr, get_q8_batch_chunks_per_job(), n * d * ntok, ntok, 4)
        if (grid.y <= 1) {
            maybe_parallel_for(0, int(ng), grid.x) $(rb, re) {
                q8q8_batch_cell_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TB, kg, wbias, rb, re, 0l, ntok)
            }
        } else {
            // 2-D grid (matmul_grid engaged): rows-fastest decode so concurrent workers share a token block's activation slice in LLC while streaming disjoint weight rows.
            let ntile = ntok / 4l
            let cells = grid.x * grid.y
            maybe_parallel_for(0, cells, cells) $(cb, ce) {
                for (ci in range(cb, ce)) {
                    let ri = ci % grid.x
                    let ti = ci / grid.x
                    let ub = int(int64(ri) * ng / int64(grid.x))
                    let ue = int(int64(ri + 1) * ng / int64(grid.x))
                    let ctb = int64(ti) * ntile / int64(grid.y) * 4l
                    let cte = ti == grid.y - 1 ? ntok : int64(ti + 1) * ntile / int64(grid.y) * 4l
                    q8q8_batch_cell_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TB, kg, wbias, ub, ue, ctb, cte)
                }
            }
        }
    }
    unsafe {
        for (i in range64(ng * mr, d)) {
            let wrow = wp + i * n
            let srow = sp + i * nb
            for (tk in range64(ntok)) {
                myp[tk * d + i] = dot_q8q8_sdot4x4(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
            }
        }
    }
    delete bs
}

// mm slot: the hand laneq dispatch shape with the group walk handed to the generated rows core —
// ONE core call per chunk. Strides come off the layout companion, so the same wrapper serves
// both stamped layouts; the row tail (d % mr) stays row-major and rides sdot4x4.
def private q8q8_kernel_gen(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    maybe_parallel_for(0, int(ng), matmul_chunks_gemv(int(ng * mr), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
        q8q8_gemv_gen(myp, wp, sp, xqp, xsp, n, int64(rb) * mr, int64(re) * mr)
    }
    unsafe {
        for (i in range64(ng * mr, d)) {
            myp[i] = dot_q8q8_sdot4x4(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

// group3: the hand laneq fused walk (one dispatch over the three regions' combined group
// space), each worker chunk split at region boundaries into at most three rows-core calls
def private q8q8_group3_gen(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?;
                            woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng0 = d0 / mr
    let ng1 = d1 / mr
    let ng2 = d2 / mr
    let ngtot = ng0 + ng1 + ng2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    // oversplit only under team dispatch — same trade as the hand laneq group3
    maybe_parallel_for(0, int(ngtot), matmul_chunks_gemv(int(ngtot * mr), get_jobque_team_mode() ? get_q8_batch_chunks_per_job() : 1, n * (d0 + d1 + d2))) $(rb, re) {
        unsafe {
            let b0 = int64(rb)
            let e0 = min(int64(re), ng0)
            if (b0 < e0) {
                q8q8_gemv_gen(w0, wp + woff0, sp + woff0 / 32l, xqp, xsp, n, b0 * mr, e0 * mr)
            }
            let b1 = max(int64(rb) - ng0, 0l)
            let e1 = min(int64(re) - ng0, ng1)
            if (b1 < e1) {
                q8q8_gemv_gen(w1, wp + woff1, sp + woff1 / 32l, xqp, xsp, n, b1 * mr, e1 * mr)
            }
            let b2 = max(int64(rb) - ng0 - ng1, 0l)
            let e2 = min(int64(re) - ng0 - ng1, ng2)
            if (b2 < e2) {
                q8q8_gemv_gen(w2, wp + woff2, sp + woff2 / 32l, xqp, xsp, n, b2 * mr, e2 * mr)
            }
        }
    }
    unsafe {
        for (i in range64(ng0 * mr, d0)) {
            w0[i] = dot_q8q8_sdot4x4(wp + woff0 + i * n, sp + woff0 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng1 * mr, d1)) {
            w1[i] = dot_q8q8_sdot4x4(wp + woff1 + i * n, sp + woff1 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng2 * mr, d2)) {
            w2[i] = dot_q8q8_sdot4x4(wp + woff2 + i * n, sp + woff2 / 32l + i * nb, xqp, xsp, n)
        }
    }
}

// groupn: the hand laneq fused walk over the (region x group) space; each worker chunk runs
// region-contiguous rows-core spans + the bias post-add over the same span (an extra y pass,
// trivial next to the dots)
def private q8q8_groupn_gen(var yp : float?; wp : int8 const?; sp : float const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    maybe_parallel_for(0, int(nregions * ng), matmul_chunks_gemv(int(nregions * ng * mr), get_q8_batch_chunks_per_job(), n * d * nregions)) $(gb, ge) {
        unsafe {
            var gg = int64(gb)
            while (gg < int64(ge)) {
                let r = gg / ng
                let gLocal = gg % ng
                let gEnd = min(int64(ge) - r * ng, ng)
                let woff = offs[r * 2l]
                let xoff = offs[r * 2l + 1l]
                q8q8_gemv_gen(myp + r * d, wp + woff, sp + woff / 32l, xqp + xoff, xsp + xoff / 32l, n, gLocal * mr, gEnd * mr)
                if (bp != null) {
                    for (i in range64(gLocal * mr, gEnd * mr)) {
                        myp[r * d + i] += bp[boffs[r] + i]
                    }
                }
                gg = r * ng + gEnd
            }
        }
    }
    unsafe {
        for (r in range64(nregions)) {   // row tails (d % mr): row-major sdot4x4 per region
            let woff = offs[r * 2l]
            let xoff = offs[r * 2l + 1l]
            for (i in range64(ng * mr, d)) {
                var v = dot_q8q8_sdot4x4(wp + woff + i * n, sp + woff / 32l + i * nb, xqp + xoff, xsp + xoff / 32l, n)
                if (bp != null) {
                    v += bp[boffs[r] + i]
                }
                myp[r * d + i] = v
            }
        }
    }
}

// mm_mx4 slot: the mx4 twin of q8q8_kernel_gen — group chunks handed to the generated mx4
// rows core, row tail (d % mr) row-major via dot_mx4q8 (tail planes stay row-major in the
// repack)
def private mx4q8_kernel_gen(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nrow = n / 2l
    let nbb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(ng), matmul_chunks_gemv(int(ng * mr), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
            mx4q8_gemv_gen(myp, wn, we, xqp, xsp, n, int64(rb) * mr, int64(re) * mr)
        }
        for (i in range64(ng * mr, d)) {
            myp[i] = dot_mx4q8(lutp, wn + i * nrow, we + i * nbb, xqp, xsp, n)
        }
    }
}

// groupn_mx4: the mx4 twin of q8q8_groupn_gen — region-contiguous rows-core spans + bias
// post-add, row tails row-major per region
def private mx4q8_groupn_gen(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nrow = n / 2l
    let nbb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(nregions * ng), matmul_chunks_gemv(int(nregions * ng * mr), get_q8_batch_chunks_per_job(), n * d * nregions)) $(gb, ge) {
            unsafe {
                var gg = int64(gb)
                while (gg < int64(ge)) {
                    let r = gg / ng
                    let gLocal = gg % ng
                    let gEnd = min(int64(ge) - r * ng, ng)
                    let woff = offs[r * 2l]
                    let xoff = offs[r * 2l + 1l]
                    mx4q8_gemv_gen(myp + r * d, wn + woff / 2l, we + woff / 32l, xqp + xoff, xsp + xoff / 32l, n, gLocal * mr, gEnd * mr)
                    if (bp != null) {
                        for (i in range64(gLocal * mr, gEnd * mr)) {
                            myp[r * d + i] += bp[boffs[r] + i]
                        }
                    }
                    gg = r * ng + gEnd
                }
            }
        }
        for (r in range64(nregions)) {   // row tails (d % mr): row-major dot per region
            let woff = offs[r * 2l]
            let xoff = offs[r * 2l + 1l]
            for (i in range64(ng * mr, d)) {
                var v = dot_mx4q8(lutp, wn + woff / 2l + i * nrow, we + woff / 32l + i * nbb, xqp + xoff, xsp + xoff / 32l, n)
                if (bp != null) {
                    v += bp[boffs[r] + i]
                }
                myp[r * d + i] = v
            }
        }
    }
}

// One (group range x token range) cell of the mx4 batch walk: TB token blocks outer, groups
// inner, 4-token tile calls + per-token rows-core tails (ts pinned 4 — no amx tokstep here).
// The tile dequants each block-group once per 4 tokens straight off the planes.
def private mx4q8_batch_cell_gen(var myp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d, nb, mr, TB : int64; ub, ue : int; tb0, tend : int64) {
    unsafe {
        var tb = tb0
        while (tb < tend) {
            let tbe = min(tb + TB, tend)
            let tb4 = tb + ((tbe - tb) / 4l) * 4l
            for (gi in range(ub, ue)) {
                let g = int64(gi)
                let wng = wn + g * mr * (n / 2l)
                let weg = we + g * mr * nb
                var tk = tb
                while (tk < tb4) {
                    mx4q8_tile_gen(myp, wng, weg, xqp, xsp, null, n, d, g, tk)
                    tk += 4l
                }
                while (tk < tbe) {   // token tail (< 4): the rows core, one position
                    mx4q8_gemv_gen(myp + tk * d, wn, we, xqp + tk * n, xsp + tk * nb, n, g * mr, (g + 1l) * mr)
                    tk++
                }
            }
            tb += TB
        }
    }
}

// mx4_batch slot: batched MXFP4·Q8 GEMM straight off the interleaved planes — group chunks
// parallel, cell walk above per chunk. No expansion scratch: retires expand_mx4_region_q8 (the
// 3·d·n Q8 DRAM round-trip was 87% of gpt-oss x64 prefill wall).
def private mx4q8_batch_kernel_gen(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    let nrow = n / 2l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    let TB = effective_token_block(g_q8_token_block, n)
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(ng), matmul_chunks(int(ng * mr), get_q8_batch_chunks_per_job(), n * d * ntok)) $(ub, ue) {
            mx4q8_batch_cell_gen(myp, wn, we, xqp, xsp, n, d, nb, mr, TB, ub, ue, 0l, ntok)
        }
        for (tk in range64(ntok)) {   // row tails (d % mr): row-major per token
            for (i in range64(ng * mr, d)) {
                myp[tk * d + i] = dot_mx4q8(lutp, wn + i * nrow, we + i * nb, xqp + tk * n, xsp + tk * nb, n)
            }
        }
    }
}

// mx4_batch_groupn slot: the fused MoE prefill form (llama.cpp mul_mat_id shape) — ONE dispatch
// runs every touched expert's batched GEMM through the SAME batch cell walk; bias post-adds over
// the span by the worker that owns it (bit-identical to add-at-store).
def private mx4q8_batch_groupn_gen(var yp : float?; wn : uint8 const?; we : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    let nrow = n / 2l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    let TB = effective_token_block(g_q8_token_block, n)
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        maybe_parallel_for(0, int(nregions * ng), matmul_chunks_gemv(int(nregions * ng * mr), get_q8_batch_chunks_per_job(), n * d * nregions)) $(gb, ge) {
            unsafe {
                var gg = int64(gb)
                while (gg < int64(ge)) {
                    let r = gg / ng
                    let gLocal = gg % ng
                    let gEnd = min(int64(ge) - r * ng, ng)
                    let woff = offs[r * 3l]
                    let row0 = offs[r * 3l + 1l]
                    let cnt = offs[r * 3l + 2l]
                    mx4q8_batch_cell_gen(myp, wn + woff / 2l, we + woff / 32l, xqp, xsp, n, d, nb, mr, TB, int(gLocal), int(gEnd), row0, row0 + cnt)
                    if (bp != null) {
                        for (tk in range64(row0, row0 + cnt)) {
                            for (i in range64(gLocal * mr, gEnd * mr)) {
                                myp[tk * d + i] += bp[boffs[r] + i]
                            }
                        }
                    }
                    gg = r * ng + gEnd
                }
            }
        }
        for (r in range64(nregions)) {   // row tails (d % mr): row-major per (region, token)
            let woff = offs[r * 3l]
            let row0 = offs[r * 3l + 1l]
            let cnt = offs[r * 3l + 2l]
            for (tk in range64(row0, row0 + cnt)) {
                for (i in range64(ng * mr, d)) {
                    var v = dot_mx4q8(lutp, wn + woff / 2l + i * nrow, we + woff / 32l + i * nb, xqp + tk * n, xsp + tk * nb, n)
                    if (bp != null) {
                        v += bp[boffs[r] + i]
                    }
                    myp[tk * d + i] = v
                }
            }
        }
    }
}

// mm_kq slot: the K-quant twin of q8q8_kernel_gen/mx4q8_kernel_gen — group chunks handed to
// the stamped kq rows core for the format, row tail (d % mr) row-major via the portable dots
// (tail rows stay disk-order in the repack)
def private kq_kernel_gen(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    let nsb = n / 256l
    let mr = kq_layout_of(fmt)
    let ng = d / mr
    var myp = yp
    maybe_parallel_for(0, int(ng), matmul_chunks_gemv(int(ng * mr), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
        if (fmt == 4) {
            k4q8_gemv_gen(myp, kqp, ksp, xqp, xsp, xbsp, n, int64(rb) * mr, int64(re) * mr)
        } elif (fmt == 5) {
            k5q8_gemv_gen(myp, kqp, ksp, xqp, xsp, xbsp, n, int64(rb) * mr, int64(re) * mr)
        } elif (fmt == 40) {
            q40q8_gemv_gen(myp, kqp, ksp, xqp, xsp, xbsp, n, int64(rb) * mr, int64(re) * mr)
        } else {
            k6q8_gemv_gen(myp, kqp, ksp, xqp, xsp, xbsp, n, int64(rb) * mr, int64(re) * mr)
        }
    }
    unsafe {
        for (i in range64(ng * mr, d)) {
            if (fmt == 4) {
                myp[i] = dot_k4q8(kqp + i * nsb * 128l, ksp + i * nsb * 20l, xqp, xsp, xbsp, n)
            } elif (fmt == 5) {
                myp[i] = dot_k5q8(kqp + i * nsb * 160l, ksp + i * nsb * 20l, xqp, xsp, xbsp, n)
            } elif (fmt == 40) {
                myp[i] = dot_q40q8(kqp + i * nsb * 128l, ksp + i * nsb * 16l, xqp, xsp, xbsp, n)
            } else {
                myp[i] = dot_k6q8(kqp + i * nsb * 192l, ksp + i * nsb * 18l, xqp, xsp, xbsp, n)
            }
        }
    }
}

// One (group range x token range) cell of the kq batch walk: TB token blocks outer, groups
// inner, 4-token tile calls + per-token gemv tails (ts pinned 4 — no TMUL kq tile exists). k5/k6
// tiles unpack a BYTE-EXPANDED scratch panel once per (group, token-block), amortized over TB.
def private kq_batch_cell_gen(fmt : int; var myp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, mr, TB : int64; ub, ue : int; tb0, tend : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let nb16 = n / 16l
    let packed = fmt == 4 || fmt == 40   // pure-nibble planes: the tile reads them directly
    var scratch : array<uint8>
    if (!packed) {
        scratch |> resize(int(mr * nsb * 256l))
    }
    unsafe {
        var scp = !packed ? addr(scratch[0]) : reinterpret<uint8?>(null)
        var tb = tb0
        while (tb < tend) {
            let tbe = min(tb + TB, tend)
            let tb4 = tb + ((tbe - tb) / 4l) * 4l
            for (gi in range(ub, ue)) {
                let g = int64(gi)
                let kqg = kqp + g * mr * nsb * qsb
                let ksg = ksp + g * mr * nsb * ssb
                var tk = tb
                if (tk < tb4 && !packed) {
                    unpack_kq_panel_grp(int64(fmt), kqg, scp, mr, nsb)
                }
                while (tk < tb4) {
                    if (fmt == 4) {
                        k4q8_tile_gen(myp, kqg, ksg, xqp, xsp, xbsp, n, d, g, tk)
                    } elif (fmt == 5) {
                        k5q8_tile_gen(myp, scp, ksg, xqp, xsp, xbsp, n, d, g, tk)
                    } elif (fmt == 40) {
                        q40q8_tile_gen(myp, kqg, ksg, xqp, xsp, xbsp, n, d, g, tk)
                    } else {
                        k6q8_tile_gen(myp, scp, ksg, xqp, xsp, xbsp, n, d, g, tk)
                    }
                    tk += 4l
                }
                while (tk < tbe) {   // token tail (< 4): the rows core, one position
                    if (fmt == 4) {
                        k4q8_gemv_gen(myp + tk * d, kqp, ksp, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n, g * mr, (g + 1l) * mr)
                    } elif (fmt == 5) {
                        k5q8_gemv_gen(myp + tk * d, kqp, ksp, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n, g * mr, (g + 1l) * mr)
                    } elif (fmt == 40) {
                        q40q8_gemv_gen(myp + tk * d, kqp, ksp, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n, g * mr, (g + 1l) * mr)
                    } else {
                        k6q8_gemv_gen(myp + tk * d, kqp, ksp, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n, g * mr, (g + 1l) * mr)
                    }
                    tk++
                }
            }
            tb += TB
        }
    }
    delete scratch
}

// kq_batch slot: batched K-quant GEMM over the repacked planes — group chunks parallel, the
// cell walk above per chunk, row tails (d % mr) disk-order per token via the portable dots
def private kq_batch_kernel_gen(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, ntok : int64) {
    let nsb = n / 256l
    let mr = kq_layout_of(fmt)
    let ng = d / mr
    var myp = yp
    let TB = effective_token_block(g_q8_token_block, n)
    // GEMM shaper, not GEMV: a batch row-unit is ntok dots, so the GEMV wave-cancel formula doesn't apply.
    maybe_parallel_for(0, int(ng), matmul_chunks(int(ng * mr), get_q8_batch_chunks_per_job(), n * d * ntok)) $(ub, ue) {
        kq_batch_cell_gen(fmt, myp, kqp, ksp, xqp, xsp, xbsp, n, d, mr, TB, ub, ue, 0l, ntok)
    }
    unsafe {
        for (tk in range64(ntok)) {
            for (i in range64(ng * mr, d)) {
                if (fmt == 4) {
                    myp[tk * d + i] = dot_k4q8(kqp + i * nsb * 128l, ksp + i * nsb * 20l, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * (n / 16l), n)
                } elif (fmt == 5) {
                    myp[tk * d + i] = dot_k5q8(kqp + i * nsb * 160l, ksp + i * nsb * 20l, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * (n / 16l), n)
                } elif (fmt == 40) {
                    myp[tk * d + i] = dot_q40q8(kqp + i * nsb * 128l, ksp + i * nsb * 16l, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * (n / 16l), n)
                } else {
                    myp[tk * d + i] = dot_k6q8(kqp + i * nsb * 192l, ksp + i * nsb * 18l, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * (n / 16l), n)
                }
            }
        }
    }
}

// kq_batch_groupn slot: fused MoE prefill form — ONE dispatch runs every touched expert's
// batched GEMM through the SAME batch cell walk the per-expert tile uses, so it is bit-exact
// vs a per-region loop of kq_batch_kernel_gen.
def private kq_batch_groupn_gen(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let mr = kq_layout_of(fmt)
    let ng = d / mr
    let nb16 = n / 16l
    var myp = yp
    let TB = effective_token_block(g_q8_token_block, n)
    maybe_parallel_for(0, int(nregions * ng), matmul_chunks_gemv(int(nregions * ng * mr), get_q8_batch_chunks_per_job(), n * d * nregions)) $(gb, ge) {
        unsafe {
            var gg = int64(gb)
            while (gg < int64(ge)) {
                let r = gg / ng
                let gLocal = gg % ng
                let gEnd = min(int64(ge) - r * ng, ng)
                let sb = offs[r * 3l] / 256l
                let row0 = offs[r * 3l + 1l]
                let cnt = offs[r * 3l + 2l]
                kq_batch_cell_gen(fmt, myp, kqp + sb * qsb, ksp + sb * ssb, xqp, xsp, xbsp, n, d, mr, TB, int(gLocal), int(gEnd), row0, row0 + cnt)
                gg = r * ng + gEnd
            }
        }
    }
    unsafe {
        for (r in range64(nregions)) {   // row tails (d % mr): disk-order per (region, token)
            let sb = offs[r * 3l] / 256l
            let row0 = offs[r * 3l + 1l]
            let cnt = offs[r * 3l + 2l]
            for (tk in range64(row0, row0 + cnt)) {
                for (i in range64(ng * mr, d)) {
                    if (fmt == 4) {
                        myp[tk * d + i] = dot_k4q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n)
                    } elif (fmt == 5) {
                        myp[tk * d + i] = dot_k5q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n)
                    } elif (fmt == 40) {
                        myp[tk * d + i] = dot_q40q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n)
                    } else {
                        myp[tk * d + i] = dot_k6q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + tk * n, xsp + tk * (n / 256l), xbsp + tk * nb16, n)
                    }
                }
            }
        }
    }
}

// kq_groupn slot: region-list kq GEMV, the kq twin of q8q8_groupn_gen — region-contiguous group
// spans through the format's rows core, row tails disk-order per region. Not `private`: the
// groupn bit-exactness gate in test_kquant drives it directly.
def kq_groupn_gen(fmt : int; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    let mr = kq_layout_of(fmt)
    let ng = d / mr
    var myp = yp
    maybe_parallel_for(0, int(nregions * ng), matmul_chunks_gemv(int(nregions * ng * mr), get_q8_batch_chunks_per_job(), n * d * nregions)) $(gb, ge) {
        unsafe {
            var gg = int64(gb)
            while (gg < int64(ge)) {
                let r = gg / ng
                let gLocal = gg % ng
                let gEnd = min(int64(ge) - r * ng, ng)
                let sb = offs[r * 2l] / 256l
                let xoff = offs[r * 2l + 1l]
                if (fmt == 4) {
                    k4q8_gemv_gen(myp + r * d, kqp + sb * qsb, ksp + sb * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n, gLocal * mr, gEnd * mr)
                } elif (fmt == 5) {
                    k5q8_gemv_gen(myp + r * d, kqp + sb * qsb, ksp + sb * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n, gLocal * mr, gEnd * mr)
                } elif (fmt == 40) {
                    q40q8_gemv_gen(myp + r * d, kqp + sb * qsb, ksp + sb * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n, gLocal * mr, gEnd * mr)
                } else {
                    k6q8_gemv_gen(myp + r * d, kqp + sb * qsb, ksp + sb * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n, gLocal * mr, gEnd * mr)
                }
                gg = r * ng + gEnd
            }
        }
    }
    unsafe {
        for (r in range64(nregions)) {   // row tails (d % mr): disk-order per region
            let sb = offs[r * 2l] / 256l
            let xoff = offs[r * 2l + 1l]
            for (i in range64(ng * mr, d)) {
                if (fmt == 4) {
                    myp[r * d + i] = dot_k4q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n)
                } elif (fmt == 5) {
                    myp[r * d + i] = dot_k5q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n)
                } elif (fmt == 40) {
                    myp[r * d + i] = dot_q40q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n)
                } else {
                    myp[r * d + i] = dot_k6q8(kqp + (sb + i * nsb) * qsb, ksp + (sb + i * nsb) * ssb, xqp + xoff, xsp + xoff / 256l, xbsp + xoff / 16l, n)
                }
            }
        }
    }
}

// ===== f16-scale twins (wscale_f16 rail): same slot traversals over the binary16 group-scale
// plane — generated cores via s16 companions, tails via dot_q8q8_f16s / the s16 rows core =====

def private q8q8_batch_cell_s16_gen(var myp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, nb, mr, TB : int64; ub, ue : int; tb0, te : int64) {
    unsafe {
        var tb = tb0
        while (tb < te) {
            let tbe = min(tb + TB, te)
            let tb4 = tb + ((tbe - tb) / 4l) * 4l
            for (gi in range(ub, ue)) {
                let g = int64(gi)
                let wgp = wp + g * mr * n
                let sgp = sp + g * mr * nb
                var tk = tb
                while (tk < tb4) {
                    q8q8_tile_s16_gen(myp, wgp, sgp, xqp, xsp, xbsp, n, d, g, tk)
                    tk += 4l
                }
                while (tk < tbe) {
                    q8q8_gemv_s16_gen(myp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, g * mr, (g + 1l) * mr)
                    tk++
                }
            }
            tb += TB
        }
    }
}

def private q8q8_batch_amx_cell_s16_gen(var myp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, nb, mr, TBa, ts, gs : int64; ub, ue : int; tb0, te : int64) {
    unsafe {
        var tb = tb0
        while (tb < te) {
            let tbe = min(tb + TBa, te)
            let tbS = tb + ((tbe - tb) / ts) * ts
            for (ui in range(ub, ue)) {
                let gg = int64(ui) * gs
                let wgp = wp + gg * mr * n
                let sgp = sp + gg * mr * nb
                var tk = tb
                while (tk < tbS) {
                    q8q8_tile_s16_gen(myp, wgp, sgp, xqp, xsp, xbsp, n, d, gg, tk)
                    tk += ts
                }
                if (tk < tbe) {
                    if (tbe - ts >= tb0) {
                        q8q8_tile_s16_gen(myp, wgp, sgp, xqp, xsp, xbsp, n, d, gg, tbe - ts)
                    } else {
                        while (tk < tbe) {
                            q8q8_gemv_s16_gen(myp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, gg * mr, (gg + gs) * mr)
                            tk++
                        }
                    }
                }
            }
            tb += TBa
        }
    }
}

def private q8q8_batch_kernel_s16_gen(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let wbias = int64(q8q8_wbias_gen())
    let ng = d / mr
    var myp = yp
    let TB = effective_token_block(g_q8_token_block, n)
    var bs : array<int>
    if (wbias != 0l) {
        bs |> resize(int(ntok * nb))
        unsafe {
            var bsp = addr(bs[0])
            for (t in range64(ntok)) {
                let xrow = xqp + t * n
                for (bi in range64(nb)) {
                    var s = 0
                    let xb = bi * 32l
                    for (j in range64(32l)) {
                        s += int(xrow[xb + j])
                    }
                    bsp[t * nb + bi] = -128 * s
                }
            }
        }
    }
    let xbsp : int const? = wbias != 0l ? unsafe(addr(bs[0])) : null
    let ts = int64(q8q8_tokstep_gen())   // tokens per tile call (4; 16/32 on amx stamps)
    if (ts != 4l) {
        let gs = ts / 16l                // row-groups per call — amx macros are square in tile units
        let ngu = ng / gs
        let TBa = max(TB, ts)
        let grid = matmul_grid(int(ngu), gs * mr, get_q8_batch_chunks_per_job(), n * d * ntok, ntok, int(ts))
        if (grid.y <= 1) {
            maybe_parallel_for(0, int(ngu), grid.x) $(ub, ue) {
                q8q8_amx_cfg_gen()      // latch stamps: load the TMUL palette on this worker
                q8q8_batch_amx_cell_s16_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TBa, ts, gs, ub, ue, 0l, ntok)
            }
        } else {
            let ntile = ntok / ts
            let cells = grid.x * grid.y
            maybe_parallel_for(0, cells, cells) $(cb, ce) {
                q8q8_amx_cfg_gen()      // latch stamps: load the TMUL palette on this worker
                for (ci in range(cb, ce)) {
                    let ri = ci % grid.x
                    let ti = ci / grid.x
                    let ub = int(int64(ri) * ngu / int64(grid.x))
                    let ue = int(int64(ri + 1) * ngu / int64(grid.x))
                    let ctb = int64(ti) * ntile / int64(grid.y) * ts
                    let cte = ti == grid.y - 1 ? ntok : int64(ti + 1) * ntile / int64(grid.y) * ts
                    q8q8_batch_amx_cell_s16_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TBa, ts, gs, ub, ue, ctb, cte)
                }
            }
        }
        if (ngu * gs < ng) {
            unsafe {
                for (tk in range64(ntok)) {
                    q8q8_gemv_s16_gen(myp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, ngu * gs * mr, ng * mr)
                }
            }
        }
    } else {
        let grid = matmul_grid(int(ng), mr, get_q8_batch_chunks_per_job(), n * d * ntok, ntok, 4)
        if (grid.y <= 1) {
            maybe_parallel_for(0, int(ng), grid.x) $(rb, re) {
                q8q8_batch_cell_s16_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TB, rb, re, 0l, ntok)
            }
        } else {
            let ntile = ntok / 4l
            let cells = grid.x * grid.y
            maybe_parallel_for(0, cells, cells) $(cb, ce) {
                for (ci in range(cb, ce)) {
                    let ri = ci % grid.x
                    let ti = ci / grid.x
                    let ub = int(int64(ri) * ng / int64(grid.x))
                    let ue = int(int64(ri + 1) * ng / int64(grid.x))
                    let ctb = int64(ti) * ntile / int64(grid.y) * 4l
                    let cte = ti == grid.y - 1 ? ntok : int64(ti + 1) * ntile / int64(grid.y) * 4l
                    q8q8_batch_cell_s16_gen(myp, wp, sp, xqp, xsp, xbsp, n, d, nb, mr, TB, ub, ue, ctb, cte)
                }
            }
        }
    }
    unsafe {
        for (i in range64(ng * mr, d)) {   // row tails (d % mr) stay row-major in the repack
            let wrow = wp + i * n
            let srow = sp + i * nb
            for (tk in range64(ntok)) {
                myp[tk * d + i] = dot_q8q8_f16s(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
            }
        }
    }
    delete bs
}

def private q8q8_kernel_s16_gen(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    maybe_parallel_for(0, int(ng), matmul_chunks_gemv(int(ng * mr), get_q8_batch_chunks_per_job(), n * d)) $(rb, re) {
        q8q8_gemv_s16_gen(myp, wp, sp, xqp, xsp, n, int64(rb) * mr, int64(re) * mr)
    }
    unsafe {
        for (i in range64(ng * mr, d)) {
            myp[i] = dot_q8q8_f16s(wp + i * n, sp + i * nb, xqp, xsp, n)
        }
    }
}

def private q8q8_group3_s16_gen(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : uint16 const?;
                                woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng0 = d0 / mr
    let ng1 = d1 / mr
    let ng2 = d2 / mr
    let ngtot = ng0 + ng1 + ng2
    var w0 = y0p
    var w1 = y1p
    var w2 = y2p
    maybe_parallel_for(0, int(ngtot), matmul_chunks_gemv(int(ngtot * mr), get_jobque_team_mode() ? get_q8_batch_chunks_per_job() : 1, n * (d0 + d1 + d2))) $(rb, re) {
        unsafe {
            let b0 = int64(rb)
            let e0 = min(int64(re), ng0)
            if (b0 < e0) {
                q8q8_gemv_s16_gen(w0, wp + woff0, sp + woff0 / 32l, xqp, xsp, n, b0 * mr, e0 * mr)
            }
            let b1 = max(int64(rb) - ng0, 0l)
            let e1 = min(int64(re) - ng0, ng1)
            if (b1 < e1) {
                q8q8_gemv_s16_gen(w1, wp + woff1, sp + woff1 / 32l, xqp, xsp, n, b1 * mr, e1 * mr)
            }
            let b2 = max(int64(rb) - ng0 - ng1, 0l)
            let e2 = min(int64(re) - ng0 - ng1, ng2)
            if (b2 < e2) {
                q8q8_gemv_s16_gen(w2, wp + woff2, sp + woff2 / 32l, xqp, xsp, n, b2 * mr, e2 * mr)
            }
        }
    }
    unsafe {
        for (i in range64(ng0 * mr, d0)) {
            w0[i] = dot_q8q8_f16s(wp + woff0 + i * n, sp + woff0 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng1 * mr, d1)) {
            w1[i] = dot_q8q8_f16s(wp + woff1 + i * n, sp + woff1 / 32l + i * nb, xqp, xsp, n)
        }
        for (i in range64(ng2 * mr, d2)) {
            w2[i] = dot_q8q8_f16s(wp + woff2 + i * n, sp + woff2 / 32l + i * nb, xqp, xsp, n)
        }
    }
}

def private q8q8_groupn_s16_gen(var yp : float?; wp : int8 const?; sp : uint16 const?; offs : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; bp : float const?; boffs : int64 const?; n, d : int64) {
    let nb = n / 32l
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr
    var myp = yp
    maybe_parallel_for(0, int(nregions * ng), matmul_chunks_gemv(int(nregions * ng * mr), get_q8_batch_chunks_per_job(), n * d * nregions)) $(gb, ge) {
        unsafe {
            var gg = int64(gb)
            while (gg < int64(ge)) {
                let r = gg / ng
                let gLocal = gg % ng
                let gEnd = min(int64(ge) - r * ng, ng)
                let woff = offs[r * 2l]
                let xoff = offs[r * 2l + 1l]
                q8q8_gemv_s16_gen(myp + r * d, wp + woff, sp + woff / 32l, xqp + xoff, xsp + xoff / 32l, n, gLocal * mr, gEnd * mr)
                if (bp != null) {
                    for (i in range64(gLocal * mr, gEnd * mr)) {
                        myp[r * d + i] += bp[boffs[r] + i]
                    }
                }
                gg = r * ng + gEnd
            }
        }
    }
    unsafe {
        for (r in range64(nregions)) {   // row tails (d % mr): row-major per region
            let woff = offs[r * 2l]
            let xoff = offs[r * 2l + 1l]
            for (i in range64(ng * mr, d)) {
                var v = dot_q8q8_f16s(wp + woff + i * n, sp + woff / 32l + i * nb, xqp + xoff, xsp + xoff / 32l, n)
                if (bp != null) {
                    v += bp[boffs[r] + i]
                }
                myp[r * d + i] = v
            }
        }
    }
}

[init]
def dasllama_math_gen_register() {
    // needs_repack keeps this out of auto-select (direct callers stay on arm64-sdot). q8_layout
    // must be evaluated at selection time, not [init] — [init]s run before the JIT installs
    // generated code (the slice C mr8 bug); arm64 needs no availability predicate.
    if (get_architecture_name() == "arm64" && jit_enabled()) {
        register_kernel_backend(KernelBackend(name = "arm64-gen", mm = @@q8q8_kernel_gen,
            batch = @@q8q8_batch_kernel_neon_laneq_gen, group3 = @@q8q8_group3_gen,
            repack = @@repack_q8q8_gen,
            mm_mx4 = @@mx4q8_kernel_gen, repack_mx4 = @@repack_mx4_gen,
            mx4_batch = @@mx4q8_batch_kernel_gen, mx4_batch_groupn = @@mx4q8_batch_groupn_gen,
            mx4_native_batch = true,
            groupn = @@q8q8_groupn_gen, groupn_mx4 = @@mx4q8_groupn_gen,
            mm_rows = @@q8q8_gemv_gen,
            mm_s16 = @@q8q8_kernel_s16_gen, batch_s16 = @@q8q8_batch_kernel_s16_gen,
            group3_s16 = @@q8q8_group3_s16_gen, groupn_s16 = @@q8q8_groupn_s16_gen,
            mm_rows_s16 = @@q8q8_gemv_s16_gen,
            // kq family: the emitters are generic-IR unpacks over kq_dot_lane, which rides the
            // NEON sdot lattice here — decline lockstep with the q8 family (same perm rails), so
            // the no-witness stance above covers kq too
            mm_kq = @@kq_kernel_gen, repack_kq = @@repack_kq_gen,
            kq_rows_k4 = @@k4q8_gemv_gen, kq_rows_k5 = @@k5q8_gemv_gen,
            kq_rows_k6 = @@k6q8_gemv_gen, kq_rows_q40 = @@q40q8_gemv_gen,
            kq_batch = @@kq_batch_kernel_gen,
            kq_groupn = @@kq_groupn_gen, kq_batch_groupn = @@kq_batch_groupn_gen,
            needs_repack = true, mx4_expand_interleaved = true, priority = 25,
            q8_layout = @@q8q8_layout_gen, kq_layout = @@kq_layout_fmt_gen, q8_wbias = @@q8q8_wbias_gen,
            q8_kgroup = @@q8q8_kgroup_gen))
    }
    // x64 twin: the ONLY x64-specific wiring is the availability predicate — the witness must
    // be true only if the family actually emitted (a declined stamp falls back to slow NEON
    // scalar bodies here). avx2 is the family's minimum tier.
    if (get_architecture_name() == "x86_64" && jit_enabled() && cpu_supports("avx2")) {
        register_kernel_backend(KernelBackend(name = "x64-gen", mm = @@q8q8_kernel_gen,
            batch = @@q8q8_batch_kernel_neon_laneq_gen, group3 = @@q8q8_group3_gen,
            repack = @@repack_q8q8_gen,
            mm_mx4 = @@mx4q8_kernel_gen, repack_mx4 = @@repack_mx4_gen,
            mx4_batch = @@mx4q8_batch_kernel_gen, mx4_batch_groupn = @@mx4q8_batch_groupn_gen,
            mx4_native_batch = true,
            groupn = @@q8q8_groupn_gen, groupn_mx4 = @@mx4q8_groupn_gen,
            mm_rows = @@q8q8_gemv_gen,
            mm_s16 = @@q8q8_kernel_s16_gen, batch_s16 = @@q8q8_batch_kernel_s16_gen,
            group3_s16 = @@q8q8_group3_s16_gen, groupn_s16 = @@q8q8_groupn_s16_gen,
            mm_rows_s16 = @@q8q8_gemv_s16_gen,
            mm_kq = @@kq_kernel_gen, repack_kq = @@repack_kq_gen,
            kq_rows_k4 = @@k4q8_gemv_gen, kq_rows_k5 = @@k5q8_gemv_gen,
            kq_rows_k6 = @@k6q8_gemv_gen, kq_rows_q40 = @@q40q8_gemv_gen,
            kq_batch = @@kq_batch_kernel_gen,
            kq_groupn = @@kq_groupn_gen, kq_batch_groupn = @@kq_batch_groupn_gen,
            needs_repack = true, mx4_expand_interleaved = true, priority = 25,
            q8_layout = @@q8q8_layout_gen, kq_layout = @@kq_layout_fmt_gen, q8_wbias = @@q8q8_wbias_gen,
            q8_kgroup = @@q8q8_kgroup_gen,
            available = @@q8q8_family_live))
    }
}
