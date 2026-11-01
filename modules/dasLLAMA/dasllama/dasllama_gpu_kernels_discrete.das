options gen2
options indenting = 4
options _dasllama_internal = true

//! The kernel bodies the discrete GPU homes share - Vulkan and CUDA, not Metal: the decode GEMV family over every
//! `KqFmt` with its decode helpers, the token step's row families, the decode attention, the K/V mirror stores and
//! codecs, the window's cm2 GEMM tiles and the flash tiles. A body here carries no kernel method and no dispatch
//! annotation; a home derives a leaf that calls the body under its own `[spirv_kernel]` / `[cuda_kernel]` and dispatch line.
module dasllama_gpu_kernels_discrete shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_kqformat public
require dasllama/dasllama_gpu_math public
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_tier   // moe_gpu_ffn_form: the routed form the act push both discrete homes build reads
require daslib/templates
require daslib/typemacro_boost
require spirv/spirv_markers   // [spirv_inline]: the markers alone, no emitter
require math

//! the subgroup primitives the shared reductions ride, in SPIR-V's spelling, which both discrete emitters lower in place at
//! every call site; a home defines a lane primitive of its own only where its spelling differs (Metal's `simd_*`)
[spirv_inline]
def gk_subgroup_add(v : float) : float => subgroupAdd(v)
[spirv_inline]
def gk_subgroup_max(v : float) : float => subgroupMax(v)
[spirv_inline]
def gk_subgroup_xor(v : float; m : uint) : float => subgroupShuffleXor(v, m)
[spirv_inline]
def gk_subgroup_xor_u(v : uint; m : uint) : uint => subgroupShuffleXor(v, m)
[spirv_inline]
def gk_subgroup_bcast0_u(v : uint) : uint => subgroupShuffle(v, 0u)
[spirv_inline]
def gk_subgroup_min_u(v : uint) : uint => subgroupMin(v)

//! a grid word's four magnitude bytes under scale `ds`, byte i negated where bit i of `sgn` is set
def grid_sv4(gw, sgn : uint; ds : float) : float4 {
    let g0 = float(gw & 0xFFu)
    let g1 = float((gw >> 8u) & 0xFFu)
    let g2 = float((gw >> 16u) & 0xFFu)
    let g3 = float(gw >> 24u)
    return float4(ds * ((sgn & 1u) != 0u ? -g0 : g0), ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1),
        ds * (((sgn >> 2u) & 1u) != 0u ? -g2 : g2), ds * (((sgn >> 3u) & 1u) != 0u ? -g3 : g3))
}

//! `grid_sv4`'s pair twin: the low two magnitude bytes of `gp` signed by bits 0 and 1 of `sgn`, the second where `e` is odd
def grid_sv1(gp, sgn, e : uint; ds : float) : float {
    let g0 = float(gp & 0xFFu)
    let g1 = float((gp >> 8u) & 0xFFu)
    let v0 = ds * ((sgn & 1u) != 0u ? -g0 : g0)
    let v1 = ds * (((sgn >> 1u) & 1u) != 0u ? -g1 : g1)
    return (e & 1u) != 0u ? v1 : v0
}

//! Q4_K / Q5_K: sub-block `blk`'s d x scale and dmin x min, off the row's unpacked (d, dmin) and the words that hold its scale and its min byte
def k4_scale_min(dm : float2; scw, mnw, blk : uint) : float2 {
    let sh = (blk & 3u) * 8u
    return float2(dm.x * float((scw >> sh) & 0xFFu), dm.y * float((mnw >> sh) & 0xFFu))
}

//! Q4_0 / IQ4_NL: block `blk`'s f16 d, off the word that packs it beside its neighbour's
def q40_d_of(w, blk : uint) : float {
    let dp = unpackHalf2x16(w)
    return (blk & 1u) == 0u ? dp.x : dp.y
}

//! Q6_K / Q3_K: the signed int8 sub-block scale `i` (mod 4) of its scale word
def k6_sc(w, i : uint) : int => int(w << ((3u - (i & 3u)) * 8u)) >> 24

def q8_amax3(m0 : float) : float {
    var m = m0
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    return m
}

[arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
def q8_quant1(v, id : float) : int8 => int8(int(round(v * id)))

def q8k_amax5(m0 : float) : float {
    var m = m0
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    m = max(m, subgroupShuffleXor(m, 8u))
    m = max(m, subgroupShuffleXor(m, 16u))
    return m
}

//! a gather row's token id that names no table row: the row is the host's, staged in the gather's `xh` plane
let EMB_ID_HOST = 0xFFFFFFFFu

//! the Q8_0 block scale of a head row's dim: the 32-lane group's amax over 127 (the group is the block on a 32- or 64-wide subgroup)
def da_q8_blk_d(o : float) : float => q8k_amax5(abs(o)) / 127.0

//! a lane's 32-lane Q8_0 block of `v`: the block scale (`da_q8_blk_d`) and its inverse, the factor `q8_quant1` takes
def q8_lane_scale(v : float) : float2 {
    let d = da_q8_blk_d(v)
    return float2(d, d != 0.0 ? 1.0 / d : 0.0)
}

//! hb's four low bits to bit 4 of each byte lane: the multiply's shifted copies never overlap, so no carry crosses a lane
def k5_dep(hb : uint) : uint => ((hb & 0xFu) * 0x02040810u) & 0x10101010u

//! Q5_K - the nibble half at `sh` (0 or 4) of four packed words with the 5th bits deposited from the block's qh byte lanes
def k5_quad(r : uint4; qh, sh : uint) : uint4 {
    return uint4(((r.x >> sh) & KQC_NIBBLE_MASK) | k5_dep(qh >> sh), ((r.y >> sh) & KQC_NIBBLE_MASK) | k5_dep(qh >> (8u + sh)),
                 ((r.z >> sh) & KQC_NIBBLE_MASK) | k5_dep(qh >> (16u + sh)), ((r.w >> sh) & KQC_NIBBLE_MASK) | k5_dep(qh >> (24u + sh)))
}

//! Q6_K - the nibble lanes with the 2-bit high lanes of `qh` at `shift` composed above them
def k6_quad(nib, qh : uint4; shift : uint) : uint4 {
    let m2 = 0x03030303u
    return uint4(nib.x | (((qh.x >> shift) & m2) << 4u), nib.y | (((qh.y >> shift) & m2) << 4u),
                 nib.z | (((qh.z >> shift) & m2) << 4u), nib.w | (((qh.w >> shift) & m2) << 4u))
}

//! Q3_K - the 2-bit qs lanes at `sh` with the hmask bit `bit` of each lane above them
def k3_quad(q, h : uint4; sh, bit : uint) : uint4 {
    let m2 = 0x03030303u
    let m1 = 0x01010101u
    return uint4(((q.x >> sh) & m2) | (((h.x >> bit) & m1) << 2u), ((q.y >> sh) & m2) | (((h.y >> bit) & m1) << 2u),
                 ((q.z >> sh) & m2) | (((h.z >> bit) & m1) << 2u), ((q.w >> sh) & m2) | (((h.w >> bit) & m1) << 2u))
}

//! Q2_K - the unsigned 2-bit qs lanes at `sh`
def k2_quad(q : uint4; sh : uint) : uint4 {
    let m2 = 0x03030303u
    return uint4((q.x >> sh) & m2, (q.y >> sh) & m2, (q.z >> sh) & m2, (q.w >> sh) & m2)
}

//! the sign nibbles of two sign bytes, low nibble first
def sgn4(sg0, sg1 : uint) : uint4 => uint4(sg0 & 15u, sg0 >> 4u, sg1 & 15u, sg1 >> 4u)

//! IQ3_S: 9-bit indices from the qs word and the block's qh byte at lane pair `l0` (0 or 2)
def iq3s_idx4(qsw, qh, l0 : uint) : uint4 {
    return uint4((qsw & 255u) | ((qh << (8u - 2u * l0)) & 256u), ((qsw >> 8u) & 255u) | ((qh << (7u - 2u * l0)) & 256u),
                 ((qsw >> 16u) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 256u), (qsw >> 24u) | ((qh << (7u - 2u * (l0 + 1u))) & 256u))
}

//! IQ3_S and IQ2_S: the two sign bytes of lane pair `l0` out of the block's sign word
def sign_word_sgn4(sgw, l0 : uint) : uint4 => sgn4((sgw >> (l0 * 8u)) & 255u, (sgw >> (l0 * 8u + 8u)) & 255u)

//! IQ3_XXS: the qs word's four byte indices; the signs from the aux word's 7-bit ksigns at lane pair `l0`, the parity bit recomputed
def iq3xxs_idx4(qsw : uint) : uint4 => uint4(qsw & 255u, (qsw >> 8u) & 255u, (qsw >> 16u) & 255u, qsw >> 24u)

def iq3xxs_sgn4(aux, l0 : uint) : uint4 => sgn4(ksign7((aux >> (7u * l0)) & 127u), ksign7((aux >> (7u * l0 + 7u)) & 127u))

//! IQ2_XXS: the grid word's two byte indices at half `hu` into the two-word grid; the aux word's ksigns at 14 hu
def iq2xxs_idx4(gw, hu : uint) : uint4 {
    let b0 = ((gw >> (16u * hu)) & 255u) * 2u
    let b1 = ((gw >> (16u * hu + 8u)) & 255u) * 2u
    return uint4(b0, b0 + 1u, b1, b1 + 1u)
}

def iq2xxs_sgn4(aux, hu : uint) : uint4 => sgn4(ksign7((aux >> (14u * hu)) & 127u), ksign7((aux >> (14u * hu + 7u)) & 127u))

//! IQ2_S: the 10-bit qs | qh indices of lane pair `l0` into the two-word grid
def iq2s_idx4(qw, qh, l0 : uint) : uint4 {
    let i0 = (((qw >> (l0 * 8u)) & 255u) | ((qh << (8u - 2u * l0)) & 0x300u)) * 2u
    let i1 = (((qw >> (l0 * 8u + 8u)) & 255u) | ((qh << (8u - 2u * (l0 + 1u))) & 0x300u)) * 2u
    return uint4(i0, i0 + 1u, i1, i1 + 1u)
}

//! IQ2_XS: each u16 of the qs word is a 9-bit index into the two-word grid and a 7-bit ksigns index
def iq2xs_idx4(qw : uint) : uint4 {
    let i0 = (qw & 511u) * 2u
    let i1 = ((qw >> 16u) & 511u) * 2u
    return uint4(i0, i0 + 1u, i1, i1 + 1u)
}

def iq2xs_sgn4(qw : uint) : uint4 => sgn4(ksign7((qw & 0xFFFFu) >> 9u), ksign7(qw >> 25u))

[grid_words(of = "iq3s_grid")]
def iq3s_grid_word(i : int) : uint {}

//! iq2xxs_grid's 256 u64 entries: word 2i lanes 0..3, 2i+1 4..7
[grid_words(of = "iq2xxs_grid2")]
def iq2xxs_grid_word(i : int) : uint {}

//! iq2xs_grid's 512 u64 entries: word 2i lanes 0..3, 2i+1 4..7
[grid_words(of = "iq2xs_grid2")]
def iq2xs_grid_word(i : int) : uint {}

//! iq2s_grid's 1024 u64 entries: word 2i lanes 0..3, 2i+1 4..7
[grid_words(of = "iq2s_grid2")]
def iq2s_grid_word(i : int) : uint {}

//! iq3xxs_grid halved a byte: the plane's d absorbs the 0.25
[grid_words(of = "iq3xxs_grid")]
def iq3xxs_grid_word(i : int) : uint {}

//! kvalues_iq4nl, the IQ4_NL codebook as four packed words, four signed bytes a word, in order
let IQ4NL_WORD0 = 3215825025u
let IQ4NL_WORD1 = 4142587343u
let IQ4NL_WORD2 = 639175937u
let IQ4NL_WORD3 = 1901675829u

//! four nibble lanes to SIGNED int8 sdot4 lanes off packed codebook words: a fixed_array table runs 3.4x slower on the GEMV
def iq4_word(n : uint) : uint {
    let tbl = uint4(IQ4NL_WORD0, IQ4NL_WORD1, IQ4NL_WORD2, IQ4NL_WORD3)
    let q0 = n & 15u
    let q1 = (n >> 8u) & 15u
    let q2 = (n >> 16u) & 15u
    let q3 = (n >> 24u) & 15u
    let b0 = (tbl[int(q0 >> 2u)] >> ((q0 & 3u) * 8u)) & 0xFFu
    let b1 = (tbl[int(q1 >> 2u)] >> ((q1 & 3u) * 8u)) & 0xFFu
    let b2 = (tbl[int(q2 >> 2u)] >> ((q2 & 3u) * 8u)) & 0xFFu
    let b3 = (tbl[int(q3 >> 2u)] >> ((q3 & 3u) * 8u)) & 0xFFu
    return b0 | (b1 << 8u) | (b2 << 16u) | (b3 << 24u)
}

//! four nibble lanes to SIGNED int8 lanes, 2q - 15 a byte by arithmetic: no carry crosses a lane
def iq3s4_word(n : uint) : uint {
    let v = (n & 0x0F0F0F0Fu) * 2u + 0x01010101u
    return (v & 0x0F0F0F0Fu) | ((0x01010101u - ((v >> 4u) & 0x01010101u)) * 0xF0u)
}

[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#kq-dev-scale-row")]
def iq4xs_sc(w0, w1, blk : uint) : int {
    let ls = ((w1 >> (4u * blk)) & 15u) | (((w0 >> (16u + 2u * blk)) & 3u) << 4u)
    return int(ls) - 32
}

[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#kq-dev-scale-row")]
def grid_sc(w1, blk : uint) : int => 1 + 2 * int((w1 >> (4u * blk)) & 15u)

//! the DOUBLED e2m1 magnitudes {0, 1, 2, 3, 4, 6, 8, 12} of four nibble codes, one per byte of `nib`, built without a table
def mx4_mag2x4(nib : uint) : uint {
    let mag = nib & 0x07070707u
    let lo2 = mag & 0x03030303u
    let hi1 = (mag >> 2u) & 0x01010101u
    return lo2 + ((lo2 + 0x04040404u + (((lo2 & (lo2 >> 1u)) & 0x01010101u) << 1u)) & (hi1 * 0xFFu))
}

//! four MXFP4 values off four nibble codes (one per byte of `nib`) under the halved scale `sc`
def mx4_dec4(nib : uint; sc : float) : float4 {
    let mag2 = mx4_mag2x4(nib)
    let sgn = (nib >> 3u) & 0x01010101u
    return float4(float(mag2 & 0xFFu) * (1.0 - 2.0 * float(sgn & 1u)) * sc,
        float((mag2 >> 8u) & 0xFFu) * (1.0 - 2.0 * float((sgn >> 8u) & 1u)) * sc,
        float((mag2 >> 16u) & 0xFFu) * (1.0 - 2.0 * float((sgn >> 16u) & 1u)) * sc,
        float(mag2 >> 24u) * (1.0 - 2.0 * float(sgn >> 24u)) * sc)
}

//! the doubled e2m1 magnitudes {0 1 2 3 4 6 8 12} as two byte words, then their negatives: the codebook a byte permute reads
let MX4_BYTES_LO = 0x03020100u
let MX4_BYTES_HI = 0x0C080604u
let MX4_NEG_BYTES_LO = 0xFDFEFF00u
let MX4_NEG_BYTES_HI = 0xF4F8FAFCu

//! four nibble codes (a byte each) to SIGNED int8 lanes of the doubled e2m1 magnitudes, each sign bit negating its byte with no carry across lanes
def mx4_word(nib : uint) : uint {
    let mag = mx4_mag2x4(nib)
    let m = ((nib >> 3u) & 0x01010101u) * 0xFFu
    let neg = (0x80808080u - mag) ^ 0x80808080u
    return (mag & ~m) | (neg & m)
}

struct GemvArgs {
    n : uint       //! reduction dim
    d : uint       //! output rows per region
    nreg : uint    //! region count
    ybase : uint   //! output base row
    lanes_per_row : uint   //! 8, 16, or 0 = the whole subgroup (the q8 GEMV ignores it)
    act : uint    //! the fused gate-up twins' activation code (`act_code`: silu, gelu, the clamped swiglu); the plain GEMVs ignore it
    bias_on : uint   //! the mx4 gate-up twin: each slot's expert (`rex`) names its gate and up bias rows, `gboff` this layer's base in both planes ([ne x d] rows a layer)
    gboff : uint
    ncols : uint     //! the N-column forms: activation rows dotted against one weight pass, 1 to 8; the one-column forms ignore it
    ystride : uint   //! ... and the distance between their y rows
    wblk0 : uint     //! the token-table gather (`KqGemvLeafT`'s `EMB` stamps): the table plane's slab-local superblock base; the GEMVs ignore it
    escale : float   //! ... and the embed scale its rows take
    col0 : uint      //! the N-column forms: the dispatch's first column - a tile of the rows past `GEMV_COL_TILE`; column c reads activation row col0 + c and lands y row col0 + c
}

//! a lane pair's Q8_0 half-blocks dotted: the four sdot4 over this lane's 16 bytes, the partner lane's sum added (xor 1)
def q8_pair_dot(w, x : uint4) : int {
    var i = sdot4(w.x, x.x)
    i += sdot4(w.y, x.y)
    i += sdot4(w.z, x.z)
    i += sdot4(w.w, x.w)
    return i + subgroupShuffleXor(i, 1u)
}

//! the q8 decode GEMV: two lanes a 32-block, the xor-1 fold
[ |> template_struct_instance]
class template Q8GemvT {
    @ssbo @binding = 0 @role = "weight" wq4 : array<uint4>      //! weight quant words, vec4 view (16B loads)
    @ssbo @binding = 1 @role = "weight" wsh : array<float16>    //! per-block f16 weight scales
    @ssbo @binding = 2 regions : array<uint>   //! (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xq4 : array<uint4>      //! activation quant words: a lane's half-block, one 16-byte load
    @ssbo @binding = 4 axs : array<float>      //! per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#gemv-region-walk")]
    def gemv_body {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg % pa.nreg   // regions interleave a row at a time: a weight row two regions share leaves DRAM once and reaches the next from cache
            let row = rg / pa.nreg
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc = 0.0
            var bi0 = 0u
            let full = (nb / bpp) * bpp   // unguarded whole passes: the loads of a pass issue ahead of the previous pass; a guarded load waits on its branch
            while (bi0 < full) {
                let b = bi0 + lane / 2u
                let idot = q8_pair_dot(wq4[(wb0 + bi0) * 2u + lane], xq4[(xb0 + bi0) * 2u + lane])
                let ws = float(wsh[wb0 + b])
                let xs = axs[xb0 + b]
                if (lane % 2u == 0u) {
                    acc = mad(float(idot) * ws, xs, acc)
                }
                bi0 += bpp
            }
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                if (b < nb) {
                    let idot = q8_pair_dot(wq4[(wb0 + bi0) * 2u + lane], xq4[(xb0 + bi0) * 2u + lane])
                    if (lane % 2u == 0u) {
                        acc = mad(float(idot) * float(wsh[wb0 + b]), axs[xb0 + b], acc)
                    }
                }
                bi0 += bpp
            }
            let total = subgroupAdd(acc)
            if (subgroupElect()) {
                y[pa.ybase + r * pa.d + row] = total
            }
        }
    }
}

//! the Q8_0 GEMV's N-column forms: `NC` activation rows (column c's blocks c x nb past the region's, `pa.ncols` of
//! them live) against a lane pair's weight word and scale read once, each row's half-block one 16-byte load, the y
//! rows `pa.ystride` apart; the `R2` stamps take two output rows a subgroup, so every activation load feeds two
//! weight rows and a long row's re-reads halve - the rows a region holds must then be even
[ |> template_struct_instance]
class template Q8GemvNT {
    @ssbo @binding = 0 @role = "weight" wq4 : array<uint4>      //! weight quant words, vec4 view (16B loads)
    @ssbo @binding = 1 @role = "weight" wsh : array<float16>    //! per-block f16 weight scales
    @ssbo @binding = 2 regions : array<uint>   //! (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xq4 : array<uint4>      //! activation quant words, vec4 view
    @ssbo @binding = 4 axs : array<float>      //! per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs
    @template_constant NC : int = 4       //!< the columns a lane dots per weight word
    @template_constant R2 : bool = false  //!< two output rows a subgroup

    [spirv_inline]
    def gemv_n_body {
        let sg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let sg_per_region = R2 ? pa.d / 2u : pa.d
        if (sg < pa.nreg * sg_per_region) {
            let r = sg / sg_per_region
            let row = R2 ? (sg % sg_per_region) * 2u : sg % sg_per_region
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc0, acc1 : float[8]
            for [unroll] (c in range(NC)) {
                acc0[c] = 0.0
                acc1[c] = 0.0
            }
            let clast = max(pa.ncols, 1u) - 1u   // a dead column redots row clast: its loads ride along, never stored
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                if (b < nb) {
                    let wv0 = wq4[(wb0 + bi0) * 2u + lane]
                    let ws0 = float(wsh[wb0 + b])
                    for [unroll] (c in range(NC)) {
                        let xb = xb0 + (pa.col0 + min(uint(c), clast)) * nb
                        let xv = xq4[(xb + bi0) * 2u + lane]
                        let i0 = q8_pair_dot(wv0, xv)
                        static_if (R2) {
                            let i1 = q8_pair_dot(wq4[(wb0 + nb + bi0) * 2u + lane], xv)
                            if (lane % 2u == 0u) {
                                let xs = axs[xb + b]
                                acc0[c] = mad(float(i0) * ws0, xs, acc0[c])
                                acc1[c] = mad(float(i1) * float(wsh[wb0 + nb + b]), xs, acc1[c])
                            }
                        } else {
                            if (lane % 2u == 0u) {
                                acc0[c] = mad(float(i0) * ws0, axs[xb + b], acc0[c])
                            }
                        }
                    }
                }
                bi0 += bpp
            }
            for [unroll] (c in range(NC)) {
                if (uint(c) < pa.ncols) {
                    let t0 = subgroupAdd(acc0[c])
                    static_if (R2) {
                        let t1 = subgroupAdd(acc1[c])
                        if (subgroupElect()) {
                            let yb = pa.ybase + (pa.col0 + uint(c)) * pa.ystride + r * pa.d + row
                            y[yb] = t0
                            y[yb + 1u] = t1
                        }
                    } else {
                        if (subgroupElect()) {
                            y[pa.ybase + (pa.col0 + uint(c)) * pa.ystride + sg] = t0
                        }
                    }
                }
            }
        }
    }
}

struct GemvArArgs {
    n : uint        //! the GEMV's reduction width
    d : uint        //! its output rows - the residual row's width
    wblk : uint     //! the plane's block base in its slab (one region: the pair rides the push)
    xblk : uint     //! the activation's block base
    add_on : uint   //! the residual step's `GkAddRmsArgs` fields (the add partner is the GEMV's own row)
    woff : uint
    pwoff : uint
    pre_on : uint
    no_norm : uint
    eps : float
    ascale : float
    abias_on : uint   //! the GEMV's row takes a bias row out of wn before the add (gpt-oss's attention output bias)
    aboff : uint      //! ... that row's base in wn
}

//! the q8 decode GEMV carrying the residual step as its last workgroup's epilogue (the shell both homes stamp, `GkResidualRow`): a
//! subgroup a row, eight rows a workgroup, the rows into a `@coherent` y, the last workgroup to arrive running `ArRq`'s step and
//! resetting the counter; a template, since the residual row's reductions resolve `gk_subgroup_*` in the home that stamps it
[ |> template_struct_instance]
class template Q8GemvArT : GkResidualRow {
    override OUT_Q8 = true
    override NORM_SWITCH = true
    @ssbo @binding = 0 wq4 : array<uint4>          //! weight quant words, vec4 view
    @ssbo @binding = 1 wsh : array<float16>        //! per-block f16 weight scales
    @ssbo @binding = 2 xqw : array<uint>           //! the GEMV's activation quant words
    @ssbo @binding = 2 @role = "alias" xq4 : array<uint4>   //! ... as 16-byte words: a lane's half-block in one load
    @ssbo @binding = 3 axs : array<float>          //! ... and their block scales
    @ssbo @binding = 4 @coherent y : array<float>  //! the rows out, read back whole by the last workgroup
    @ssbo @binding = 5 @coherent xres : array<float>   //! the residual row, updated in place and re-read by other waves of the epilogue's workgroup
    @ssbo @binding = 6 wn : array<float>           //! the norms plane
    @ssbo @binding = 7 outq : array<int8>          //! the normed row's Q8_0 quants (the next GEMV's feed)
    @ssbo @binding = 8 outs : array<float>         //! ... and its block scales
    @ssbo @binding = 9 cnt : array<uint>           //! the arrival counter at word 0, zero between dispatches
    @push_constant pa : GemvArArgs
    @workgroup arrived : uint[1]

    def override width : uint => pa.d
    def override xin(i : uint) : float => xres[i]
    def override xout(i : uint; v : float) : void {
        xres[i] = v
    }
    def override partner(i : uint) : float => y[i]
    def override stash(k : uint; v : float) : void {}   //! the finished row is re-read from `xres`
    def override stashed(k : uint) : float => xres[k]
    def override pre_partner(base, k : uint) : float => apart(base, k)

    [arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#residual-step-fma")]
    def epilogue {
        var ss = 0.0
        accum_row_into(0u, ss)
        var inv = 0.0
        row_inv_into(ss, inv)
        memoryBarrierBuffer()   // the row's writes reach the lanes that quantize them
        barrier()
        quant32(0u, inv)
        if (gl_LocalInvocationID.x == 0u) {
            cnt[0] = 0u
        }
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def gemv_ar_body {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.d) {
            let nb = pa.n / 32u
            let wb0 = pa.wblk + rg * nb
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc = 0.0
            var bi0 = 0u
            let full = (nb / bpp) * bpp   // unguarded whole passes, as `Q8GemvT`'s
            while (bi0 < full) {
                let b = bi0 + lane / 2u
                let idot = q8_pair_dot(wq4[(wb0 + bi0) * 2u + lane], xq4[(pa.xblk + bi0) * 2u + lane])
                let ws = float(wsh[wb0 + b])
                let xs = axs[pa.xblk + b]
                if (lane % 2u == 0u) {
                    acc = mad(float(idot) * ws, xs, acc)
                }
                bi0 += bpp
            }
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                if (b < nb) {
                    let xbase = (pa.xblk + bi0) * 8u + lane * 4u
                    let idot = q8_pair_dot(wq4[(wb0 + bi0) * 2u + lane], uint4(xqw[xbase], xqw[xbase + 1u], xqw[xbase + 2u], xqw[xbase + 3u]))
                    if (lane % 2u == 0u) {
                        acc = mad(float(idot) * float(wsh[wb0 + b]), axs[pa.xblk + b], acc)
                    }
                }
                bi0 += bpp
            }
            let total = subgroupAdd(acc)
            if (subgroupElect()) {
                y[rg] = total
            }
        }
        memoryBarrierBuffer()
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            arrived[0] = atomicAdd(cnt[0], 1u)
        }
        barrier()
        if (arrived[0] + 1u == gl_NumWorkGroups.x) {
            memoryBarrierBuffer()
            epilogue()
        }
    }
}

struct GuNArgs {
    n : uint      //! the reduction width (the model dim)
    d : uint      //! the output rows (the FFN width), a 32-multiple
    gblk : uint   //! the gate plane's block base in its slab
    ublk : uint   //! the up plane's block base in its slab
    act : uint    //! the activation code: ACT_SILU / ACT_GELU / ACT_OAI
    ncols : uint  //! the rows the command steps at once, 1 to GU_N_COLS: column c's activation blocks sit c x (n / 32) past the first, its quants c x d past
    col0 : uint   //! the dispatch's first column - a tile of the rows past `GU_N_COLS`; column c reads and writes row col0 + c
}

let GU_N_COLS = 8u   //! the widest gate-up N stamp's columns - the width every stamp's slabs and accumulators carry; `q8_gemv_gu_n_ensure` holds the rail's row count to it

//! the N-column leaves' width: a dispatch of more columns runs in tiles of this many, each tile's `col0` its first column
let GEMV_COL_TILE = 8u

//! the dense FFN's gate and up GEMVs, the activation and its Q8_0 requant in one dispatch: a workgroup owns 32 output rows
//! (one requant block), its subgroups dot both q8 planes; `NC` column slots a weight word, `pa.ncols` of them live and stored
//! (the one-row command's stamp at one column)
[ |> template_struct_instance]
class template Q8GemvGuNT {
    @ssbo @binding = 0 @role = "weight" gq4 : array<uint4>     //! gate quant words, vec4 view
    @ssbo @binding = 1 @role = "weight" gsh : array<float16>   //! gate per-block f16 scales
    @ssbo @binding = 2 @role = "weight" uq4 : array<uint4>     //! up quant words, vec4 view
    @ssbo @binding = 3 @role = "weight" ush : array<float16>   //! up per-block f16 scales
    @ssbo @binding = 4 xq4 : array<uint4>     //! the normed rows' Q8_0 quant words, vec4 view, a row a column
    @ssbo @binding = 5 axs : array<float>     //! ... and their per-block scales
    @ssbo @binding = 6 outq : array<int8>     //! activated hidden rows' Q8_0 quants (the down feed), d apart
    @ssbo @binding = 7 outs : array<float>    //! ... and their per-block scales, d / 32 apart
    @push_constant pa : GuNArgs
    @template_constant NC : int = 4       //!< the columns a lane dots per weight word
    @workgroup wg_g : float[256]   //! the block's gate dots, [column x 32 rows]
    @workgroup wg_u : float[256]   //! ... and up dots

    //! one output row's gate and up dots against every column's row, a subgroup wide, into the slab
    def row_dots(r : uint) {
        let nb = pa.n / 32u
        let row = gl_WorkGroupID.x * 32u + r
        let gb0 = pa.gblk + row * nb
        let ub0 = pa.ublk + row * nb
        let lane = gl_SubgroupInvocationID
        let bpp = gl_SubgroupSize / 2u
        var accg, accu : float[8]
        for [unroll] (c in range(NC)) {
            accg[c] = 0.0
            accu[c] = 0.0
        }
        let clast = max(pa.ncols, 1u) - 1u   // a dead column redots row clast: its loads ride along, never stored
        var bi0 = 0u
        let full = (nb / bpp) * bpp   // unguarded whole passes, as `Q8GemvT`'s
        while (bi0 < full) {
            let b = bi0 + lane / 2u
            let gv = gq4[(gb0 + bi0) * 2u + lane]
            let uv = uq4[(ub0 + bi0) * 2u + lane]
            let gs = float(gsh[gb0 + b])
            let us = float(ush[ub0 + b])
            for [unroll] (c in range(NC)) {
                let xb = (pa.col0 + min(uint(c), clast)) * nb
                let xv = xq4[(xb + bi0) * 2u + lane]
                let xs = axs[xb + b]
                let gd = q8_pair_dot(gv, xv)
                let ud = q8_pair_dot(uv, xv)
                if (lane % 2u == 0u) {
                    accg[c] = mad(float(gd) * gs, xs, accg[c])
                    accu[c] = mad(float(ud) * us, xs, accu[c])
                }
            }
            bi0 += bpp
        }
        while (bi0 < nb) {
            let b = bi0 + lane / 2u
            if (b < nb) {
                let gv = gq4[(gb0 + bi0) * 2u + lane]
                let uv = uq4[(ub0 + bi0) * 2u + lane]
                let gs = float(gsh[gb0 + b])
                let us = float(ush[ub0 + b])
                for [unroll] (c in range(NC)) {
                    let xb = (pa.col0 + min(uint(c), clast)) * nb
                    let xv = xq4[(xb + bi0) * 2u + lane]
                    let gd = q8_pair_dot(gv, xv)
                    let ud = q8_pair_dot(uv, xv)
                    if (lane % 2u == 0u) {
                        let xs = axs[xb + b]
                        accg[c] = mad(float(gd) * gs, xs, accg[c])
                        accu[c] = mad(float(ud) * us, xs, accu[c])
                    }
                }
            }
            bi0 += bpp
        }
        let ncols = NC == 1 ? uint(NC) : pa.ncols   // the one-column stamp has one column by construction, not by its push block
        for [unroll] (c in range(NC)) {
            if (uint(c) < ncols) {
                let g = subgroupAdd(accg[c])
                let u = subgroupAdd(accu[c])
                if (subgroupElect()) {
                    wg_g[uint(c) * 32u + r] = g
                    wg_u[uint(c) * 32u + r] = u
                }
            }
        }
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def gu_n_body {
        let row0 = gl_WorkGroupID.x * 32u
        let nsg = gl_NumSubgroups
        let rows_per = 32u / nsg
        var k = 0u
        while (k < rows_per) {
            row_dots(gl_SubgroupID + k * nsg)
            k++
        }
        barrier()
        let ncols = min(pa.ncols, uint(NC))
        var c = gl_SubgroupID
        while (c < ncols) {
            let lane = gl_SubgroupInvocationID
            let li = min(lane, 31u)
            let a = lane < 32u ? gk_act_mul(wg_g[c * 32u + li], wg_u[c * 32u + li], pa.act) : 0.0
            let s = q8_lane_scale(a)
            if (lane == 0u) {
                outs[(pa.col0 + c) * (pa.d / 32u) + gl_WorkGroupID.x] = s.x
            }
            if (lane < 32u) {
                outq[(pa.col0 + c) * pa.d + row0 + lane] = q8_quant1(a, s.y)
            }
            c += nsg
        }
    }
}

struct PleActArgs {
    n : uint      //! the side row's width (the reduction; at most 256, a 32-multiple)
    d : uint      //! the output rows (the model dim)
    wblk : uint   //! the proj plane's block base in its slab
    ubase : uint  //! this layer's row base in the side plane
    act : uint   //! the activation code: ACT_SILU / ACT_GELU / ACT_OAI
    ncols : uint    //! the rows the dispatch serves (the N-row command, at most eight): column c's gate row at `c * n`, its side row `c * sstride` past `ubase`, its proj row at `c * d`; 0 or 1 = one row
    sstride : uint  //! a row's stride in the side plane (every layer's row)
    col0 : uint     //! the dispatch's first column - a tile of the rows past eight; column c reads and writes row col0 + c
}

//! the per-layer-embedding branch's activation, its Q8_0 requant and the proj GEMV in one dispatch: every workgroup
//! rebuilds the (at most 256-wide) x in workgroup memory, then dots one output row a subgroup - the redundant build costs 256 elements a workgroup
[ |> template_struct_instance]
class template Q8GemvPleActT {
    @ssbo @binding = 0 wq4 : array<uint4>     //! proj quant words, vec4 view
    @ssbo @binding = 1 wsh : array<float16>   //! proj per-block f16 scales
    @ssbo @binding = 2 gate : array<float>    //! the gate GEMV's row
    @ssbo @binding = 3 side : array<float>    //! the side plane (every layer's row)
    @ssbo @binding = 4 y : array<float>       //! the proj row out
    @push_constant pa : PleActArgs
    @workgroup xw : uint[512]   //! the built x's Q8_0 quant words, a column's 64 at `c * 64` (eight columns, the per-row planes' extent)
    @workgroup xs : float[64]   //! ... and its block scales, a column's eight at `c * 8`

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def pleact_body {
        let tid = gl_LocalInvocationID.x
        let nc = clamp(pa.ncols, 1u, 8u)
        var k0 = tid
        while (k0 < 64u * nc) {
            xw[k0] = 0u
            k0 += 256u
        }
        barrier()
        let e = min(tid, pa.n - 1u)
        var c0 = 0u
        while (c0 < nc) {
            let v = tid < pa.n ? gk_act_mul(gate[(pa.col0 + c0) * pa.n + e], side[pa.ubase + (pa.col0 + c0) * pa.sstride + e], pa.act) : 0.0
            let s = q8_lane_scale(v)
            let q = uint(int(round(v * s.y))) & 0xFFu
            atomicOr(xw[c0 * 64u + tid / 4u], q << ((tid % 4u) * 8u))
            if ((tid & 31u) == 0u) {
                xs[c0 * 8u + tid / 32u] = s.x
            }
            c0++
        }
        barrier()
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row < pa.d) {
            let nb = pa.n / 32u
            let wb0 = pa.wblk + row * nb
            let lane = gl_SubgroupInvocationID
            let bpp = gl_SubgroupSize / 2u
            var acc : float[8]
            var c1 = 0u
            while (c1 < 8u) {
                acc[c1] = 0.0
                c1++
            }
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane / 2u
                let inb = b < nb
                let wv = inb ? wq4[(wb0 + bi0) * 2u + lane] : uint4(0u)
                let ws = inb ? float(wsh[wb0 + min(b, nb - 1u)]) : 0.0
                let xbase = inb ? bi0 * 8u + lane * 4u : 0u   // a dead lane reads words inside the slab, and its zero weight word dots them to 0
                var c2 = 0u
                while (c2 < nc) {
                    let xb = c2 * 64u + xbase
                    let idot = q8_pair_dot(wv, uint4(xw[xb], xw[xb + 1u], xw[xb + 2u], xw[xb + 3u]))
                    if (inb && lane % 2u == 0u) {
                        acc[c2] += float(idot) * ws * xs[c2 * 8u + b]
                    }
                    c2++
                }
                bi0 += bpp
            }
            var c3 = 0u
            while (c3 < nc) {
                let total = subgroupAdd(acc[c3])
                if (subgroupElect()) {
                    y[(pa.col0 + c3) * pa.d + row] = total
                }
                c3++
            }
        }
    }
}

//! a per-32 block's sixteen quants of half `sh` (0: elements 0..15, 4: 16..31) as byte lanes, off its four nibble words `w`:
//! Q5_1's nibbles with the qh word's fifth bits
[spirv_inline]
def blk32_q51_quad(w : uint4; qh, sh : uint) : uint4 {
    let hb = qh >> (sh * 4u)
    return uint4(((w.x >> sh) & KQC_NIBBLE_MASK) | k5_dep(hb), ((w.y >> sh) & KQC_NIBBLE_MASK) | k5_dep(hb >> 4u),
        ((w.z >> sh) & KQC_NIBBLE_MASK) | k5_dep(hb >> 8u), ((w.w >> sh) & KQC_NIBBLE_MASK) | k5_dep(hb >> 12u))
}

//! ... IQ4_NL's codebook bytes
[spirv_inline]
def blk32_iq4_quad(w : uint4; sh : uint) : uint4 {
    return uint4(iq4_word((w.x >> sh) & KQC_NIBBLE_MASK), iq4_word((w.y >> sh) & KQC_NIBBLE_MASK), iq4_word((w.z >> sh) & KQC_NIBBLE_MASK),
        iq4_word((w.w >> sh) & KQC_NIBBLE_MASK))
}

//! four Q4_0 nibble bytes `n` as signed int8 lanes `n - 8`: `n ^ 8` is the low bits, and a lane under 8 sets its top nibble
[spirv_inline]
def q40_signed_word(n : uint) : uint {
    let x = n ^ 0x08080808u
    return x | ((x & 0x08080808u) * 30u)
}

//! ... Q4_0's linear codes (q - 8)
[spirv_inline]
def blk32_q40_quad(w : uint4; sh : uint) : uint4 {
    return uint4(q40_signed_word((w.x >> sh) & KQC_NIBBLE_MASK), q40_signed_word((w.y >> sh) & KQC_NIBBLE_MASK), q40_signed_word((w.z >> sh) & KQC_NIBBLE_MASK),
        q40_signed_word((w.w >> sh) & KQC_NIBBLE_MASK))
}

//! four Q5_0 codes `q` (0..31 a byte) as signed int8 lanes `q - 16`: `q ^ 16` is the low bits, and a lane under 16 sets its top three
[spirv_inline]
def q50_signed_word(q : uint) : uint {
    let x = q ^ 0x10101010u
    return x | ((x & 0x10101010u) * 14u)
}

//! ... Q5_0's codes (q - 16): Q5_1's nibbles with the qh word's fifth bits, re-centred
[spirv_inline]
def blk32_q50_quad(w : uint4; qh, sh : uint) : uint4 {
    let q = blk32_q51_quad(w, qh, sh)
    return uint4(q50_signed_word(q.x), q50_signed_word(q.y), q50_signed_word(q.z), q50_signed_word(q.w))
}

//! ... the raw nibble codes (MXFP4's: `mx4_sdot` decodes them against x)
[spirv_inline]
def blk32_nib_quad(w : uint4; sh : uint) : uint4 => uint4((w.x >> sh) & KQC_NIBBLE_MASK, (w.y >> sh) & KQC_NIBBLE_MASK, (w.z >> sh) & KQC_NIBBLE_MASK, (w.w >> sh) & KQC_NIBBLE_MASK)

//! the per-32 decode `Blk32GemvT` and `Blk32BatchT` share, dispatched at compile time on the format's tag (`type<KqTag_<fmt>>`,
//! stripped by `[template]`): a block's quant words in the quant plane
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qwords(fmt : KqTag_q51) : uint => KQ_Q51_QWORDS
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qwords(fmt : KqTag_iq4nl32) : uint => KQ_B32_QWORDS
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qwords(fmt : KqTag_mx4) : uint => KQ_B32_QWORDS
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qwords(fmt : KqTag_q4032) : uint => KQ_B32_QWORDS
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qwords(fmt : KqTag_q5032) : uint => KQ_Q5032_QWORDS

//! ... the quant word a block's decode reads past its four nibble words - Q5_1's qh word, the first nibble word elsewhere (read
//! again and unused, so no load passes the block)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qh_off(fmt : KqTag_q51) : uint => KQ_B32_QWORDS
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qh_off(fmt : KqTag_iq4nl32) : uint => 0u
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qh_off(fmt : KqTag_mx4) : uint => 0u
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qh_off(fmt : KqTag_q4032) : uint => 0u
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_qh_off(fmt : KqTag_q5032) : uint => KQ_B32_QWORDS

//! ... the block's half `sh` as byte lanes off its four nibble words `w` and, on Q5_1, its qh word
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_quad(fmt : KqTag_q51; w : uint4; qh, sh : uint) : uint4 => blk32_q51_quad(w, qh, sh)
[template(fmt), spirv_inline, unused_argument(fmt, qh)]
def blk32_quad(fmt : KqTag_iq4nl32; w : uint4; qh, sh : uint) : uint4 => blk32_iq4_quad(w, sh)
[template(fmt), spirv_inline, unused_argument(fmt, qh)]
def blk32_quad(fmt : KqTag_mx4; w : uint4; qh, sh : uint) : uint4 => blk32_nib_quad(w, sh)
[template(fmt), spirv_inline, unused_argument(fmt, qh)]
def blk32_quad(fmt : KqTag_q4032; w : uint4; qh, sh : uint) : uint4 => blk32_q40_quad(w, sh)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_quad(fmt : KqTag_q5032; w : uint4; qh, sh : uint) : uint4 => blk32_q50_quad(w, qh, sh)

//! ... the scale plane's word holding block `wb`'s scale
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_sword(fmt : KqTag_q51; wb : uint) : uint => wb / (4u / KQ_Q51_SBYTES)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_sword(fmt : KqTag_iq4nl32; wb : uint) : uint => wb / (4u / KQ_IQ4NL32_SBYTES)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_sword(fmt : KqTag_mx4; wb : uint) : uint => wb / (4u / KQ_MX4_SBYTES)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_sword(fmt : KqTag_q4032; wb : uint) : uint => wb / (4u / KQ_Q4032_SBYTES)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_sword(fmt : KqTag_q5032; wb : uint) : uint => wb / (4u / KQ_Q5032_SBYTES)

//! ... the block's scale pair off the scale word `sw` holding it: Q5_1 (d, m), IQ4_NL (d, -), MXFP4 (the halved E8M0, -)
[template(fmt), spirv_inline, unused_argument(fmt, wb)]
def blk32_scale(fmt : KqTag_q51; sw, wb : uint) : float2 => unpackHalf2x16(sw)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_scale(fmt : KqTag_iq4nl32; sw, wb : uint) : float2 => float2(q40_d_of(sw, wb), 0.0)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_scale(fmt : KqTag_q4032; sw, wb : uint) : float2 => float2(q40_d_of(sw, wb), 0.0)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_scale(fmt : KqTag_q5032; sw, wb : uint) : float2 => float2(q40_d_of(sw, wb), 0.0)
[template(fmt), spirv_inline, unused_argument(fmt)]
def blk32_scale(fmt : KqTag_mx4; sw, wb : uint) : float2 {
    let per = 4u / KQ_MX4_SBYTES
    return float2(e8m0_half_f32((sw >> ((wb % per) * KQ_MX4_SBYTES * 8u)) & 0xFFu), 0.0)
}

//! the per-32 x Q8_0 decode GEMVs, `q8_gemv_cls`'s shape with ONE lane per 32-block, the block's decode the format's:
//! Q5_1's five words (four nibble words and the qh word) with (d * isum + m * asum) * xs, IQ4_NL's codebook words with
//! d * isum * xs, MXFP4's doubled magnitudes under the halved E8M0 scale - each format's scalar dot's float order, reassociated by lane
[ |> template_struct_instance]
class template Blk32GemvT {
    @template_constant Q51 : bool = false      //!< a Q5_1 plane
    @template_constant MX4 : bool = false      //!< an MXFP4 plane; neither constant: an IQ4_NL per-32 plane
    @template_constant SUM : bool = false      //!< the routed down GEMV summed under the routing weights (`sum_shell`)
    @ssbo @binding = 6 @template_gate = SUM sw : array<float>   //! the k routing weights (the per-expert down scale folded in)
    @ssbo @binding = 0 wq : array<uint>        //! weight quant words, five a block on q51, four elsewhere
    @ssbo @binding = 1 wsu : array<uint>       //! scales: q51 d|m, iq4nl32 two d a word, mx4 four E8M0 a word
    @ssbo @binding = 2 regions : array<uint>   //! (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xqw : array<uint>       //! activation quant words
    @ssbo @binding = 4 axs : array<float>      //! per-block activation scales
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    //! weight block `wb`'s quants as int8 quads against x's lo / hi words (Q5_1's nibbles with the qh bits, IQ4_NL's codebook
    //! bytes, MXFP4's raw nibbles - `blk_xdot` decodes those per column) and its scale pair: Q5_1 (d, m), IQ4_NL (d, -), MXFP4 (the halved E8M0, -)
    def blk_words(wb : uint; var wl : uint4&; var wh : uint4&) : float2 {
        wl = blk_quad(wb, 0u)
        wh = blk_quad(wb, 4u)
        return blk_scale(wb)
    }

    def iq4_word_of(n : uint) : uint {
        //! the IQ4_NL codebook's word map: the method a home overrides with its own lowering (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`)
        return iq4_word(n)
    }

    def blk_quad(wb, sh : uint) : uint4 {
        //! weight block `wb`'s half `sh` (`blk32_quad`): its words as this class binds the plane; the codebook format through `iq4_word_of`
        let b = wb * blk32_qwords(type<FMT>)
        let w = uint4(wq[b], wq[b + 1u], wq[b + 2u], wq[b + 3u])
        static_if (typeinfo typename(type<FMT>) == typeinfo typename(type<KqTag_iq4nl32>)) {
            return uint4(iq4_word_of((w.x >> sh) & KQC_NIBBLE_MASK), iq4_word_of((w.y >> sh) & KQC_NIBBLE_MASK), iq4_word_of((w.z >> sh) & KQC_NIBBLE_MASK), iq4_word_of((w.w >> sh) & KQC_NIBBLE_MASK))
        } else {
            return blk32_quad(type<FMT>, w, wq[b + blk32_qh_off(type<FMT>)], sh)
        }
    }

    //! weight block `wb`'s scale pair (`blk32_scale`)
    def blk_scale(wb : uint) : float2 => blk32_scale(type<FMT>, wsu[blk32_sword(type<FMT>, wb)], wb)

    //! one decoded block's contribution against activation block `xb` folded onto `acc`: the one-row and N-column forms and the host
    //! twin all fold through this one text, every multiply that feeds the add spelled `mad` (the GLSL `Fma`: a driver contracting a
    //! product into the sum in one module and not the other rounds the forms apart), so the modules sum bit for bit
    def blk_xdot(wl, wh : uint4; sc : float2; xb : uint; acc : float) : float {
        let xbase = xb * 8u
        let x0 = uint4(xqw[xbase], xqw[xbase + 1u], xqw[xbase + 2u], xqw[xbase + 3u])
        let x1 = uint4(xqw[xbase + 4u], xqw[xbase + 5u], xqw[xbase + 6u], xqw[xbase + 7u])
        static_if (MX4) {
            let idot = (mx4_sdot(wl.x, x0.x) + mx4_sdot(wh.x, x1.x) + mx4_sdot(wl.y, x0.y) + mx4_sdot(wh.y, x1.y)
                + mx4_sdot(wl.z, x0.z) + mx4_sdot(wh.z, x1.z) + mx4_sdot(wl.w, x0.w) + mx4_sdot(wh.w, x1.w))
            return mad(float(idot) * sc.x, axs[xb], acc)
        } else {
            let idot = (sdot4(wl.x, x0.x) + sdot4(wh.x, x1.x) + sdot4(wl.y, x0.y) + sdot4(wh.y, x1.y)
                + sdot4(wl.z, x0.z) + sdot4(wh.z, x1.z) + sdot4(wl.w, x0.w) + sdot4(wh.w, x1.w))
            static_if (Q51) {
                let asum = (sdot4(x0.x, 0x01010101u) + sdot4(x0.y, 0x01010101u) + sdot4(x0.z, 0x01010101u) + sdot4(x0.w, 0x01010101u)
                    + sdot4(x1.x, 0x01010101u) + sdot4(x1.y, 0x01010101u) + sdot4(x1.z, 0x01010101u) + sdot4(x1.w, 0x01010101u))
                return mad(mad(sc.x, float(idot), sc.y * float(asum)), axs[xb], acc)
            } else {
                return mad(sc.x * float(idot), axs[xb], acc)
            }
        }
    }

    //! one row's dot over `nb` blocks from weight block `wb0` and activation block `xb0`: a lane a block, the subgroup's sum
    def row_dot(wb0, xb0, nb : uint) : float {
        let lane = gl_SubgroupInvocationID
        var acc = 0.0
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + lane
            if (b < nb) {
                var wl = uint4(0u)
                var wh = uint4(0u)
                let sc = blk_words(wb0 + b, wl, wh)
                acc = blk_xdot(wl, wh, sc, xb0 + b, acc)
            }
            bi0 += gl_SubgroupSize
        }
        return subgroupAdd(acc)
    }

    //! the routed down GEMV summed under the routing weights over `pa.ncols` token rows (0 or 1: the one-row command's): a subgroup a
    //! (token row p, model-dim row), y[p x d + row] = sum_j w[p k + j] dot_(p k + j), token row p's k slots the regions p k .. p k + k - 1
    def sum_shell {
        static_if (SUM) {
            let rows = max(pa.ncols, 1u)
            let d = max(pa.d, 1u)
            let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
            if (rg < rows * pa.d) {
                let p = rg / d
                let row = rg % d
                let k = pa.nreg / rows
                let nb = pa.n / 32u
                var tot = 0.0
                var r = p * k
                while (r < p * k + k) {
                    tot += sw[r] * row_dot(regions[r * 2u] + row * nb, regions[r * 2u + 1u], nb)
                    r++
                }
                if (subgroupElect()) {
                    y[pa.ybase + rg] = tot
                }
            }
        }
    }
}

//! the region-interleaved one-row form: a subgroup a (region, row), the regions interleaved a row at a time; `NCOL` the
//! N-column form - a subgroup a row, a lane's blocks decoded once and dotted against every column's activation row (column
//! c's blocks sit c x nb past the region's), `pa.ncols` columns to 8, their y rows `pa.ystride` apart
[ |> template_struct_instance]
class template Blk32GemvRunT : Blk32GemvT {
    @template_constant NCOL : bool = false
    @template_constant EMB : bool = false   //!< the token-table gather (`emb_shell`) in place of either GEMV shell

    //! the per-32 token-table gather: a thread a 32-block of a row, decoded by `blk_words` - element k the lo quad's lane k (k < 16),
    //! else the hi quad's, `d * q + m` (Q5_1) or `d * q` (the codebook formats) under `pa.escale`; row r's token id is `regions[r]`,
    //! an `EMB_ID_HOST` row copies the host's staged row out of `axs`
    [arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#token-table-gather")]
    def emb_shell {
        let nb = pa.n / 32u
        var gid = gl_GlobalInvocationID.x
        while (gid < pa.nreg * nb) {   // a grid stride: the stamp's workgroup width is the pipeline's specialization
            let row = gid / max(nb, 1u)
            let b = gid - row * nb
            let o = row * pa.n + b * 32u
            let id = regions[row]
            if (id == EMB_ID_HOST) {
                for [unroll_full] (k in range(32)) {
                    y[o + uint(k)] = axs[o + uint(k)]
                }
            } else {
                var wl = uint4(0u)
                var wh = uint4(0u)
                let sc = blk_words(pa.wblk0 + id * nb + b, wl, wh)
                b32_emb_store16(o, wl, sc)
                b32_emb_store16(o + 16u, wh, sc)
            }
            gid += gl_NumWorkGroups.x * gl_WorkGroupSize.x
        }
    }

    //! the 16 int8 lanes of `w` at y[o ..], each `d x q + m` under the embed scale
    def b32_emb_store16(o : uint; w : uint4; sc : float2) {
        for [unroll_full] (c in range(4)) {
            let q = float4(int4(unpack8(int(w[c]))))
            let oc = o + uint(c) * 4u
            y[oc] = (sc.x * q.x + sc.y) * pa.escale
            y[oc + 1u] = (sc.x * q.y + sc.y) * pa.escale
            y[oc + 2u] = (sc.x * q.z + sc.y) * pa.escale
            y[oc + 3u] = (sc.x * q.w + sc.y) * pa.escale
        }
    }

    def gemv_shell_n {
        let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (rg < pa.nreg * pa.d) {
            let r = rg / pa.d
            let row = rg % pa.d
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            let lane = gl_SubgroupInvocationID
            var acc : float[8]
            for [unroll] (c in range(8)) {
                acc[c] = 0.0
            }
            var bi0 = 0u
            while (bi0 < nb) {
                let b = bi0 + lane
                if (b < nb) {
                    var wl = uint4(0u)
                    var wh = uint4(0u)
                    let sc = blk_words(wb0 + b, wl, wh)
                    for [unroll] (c in range(8)) {
                        if (uint(c) < pa.ncols) {
                            acc[c] = blk_xdot(wl, wh, sc, xb0 + b + (pa.col0 + uint(c)) * nb, acc[c])
                        }
                    }
                }
                bi0 += gl_SubgroupSize
            }
            for [unroll] (c in range(8)) {
                if (uint(c) < pa.ncols) {
                    let total = subgroupAdd(acc[c])
                    if (subgroupElect()) {
                        y[pa.ybase + (pa.col0 + uint(c)) * pa.ystride + rg] = total
                    }
                }
            }
        }
    }

    [spirv_inline]
    def b32_body {
        static_if (EMB) {
            emb_shell()
        } static_elif (NCOL) {
            gemv_shell_n()
        } else {
            let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
            if (rg < pa.nreg * pa.d) {
                let r = rg % pa.nreg
                let row = rg / pa.nreg
                let nb = pa.n / 32u
                let total = row_dot(regions[r * 2u] + row * nb, regions[r * 2u + 1u], nb)
                if (subgroupElect()) {
                    y[pa.ybase + r * pa.d + row] = total
                }
            }
        }
    }
}

//! the sdot4 of four MXFP4 nibbles (one per byte of `nib`) against four Q8_0 activation bytes: the doubled magnitudes, the sign applied by subtracting the negative bytes twice
def mx4_sdot(nib, x : uint) : int {
    let mag2 = mx4_mag2x4(nib)
    let neg = mag2 & (((nib >> 3u) & 0x01010101u) * 0xFFu)
    return sdot4(mag2, x) - 2 * sdot4(neg, x)
}

//! bindings 10 and 11 sit at the hazard rail's cap (`SetBits` holds twelve): a thirteenth plane here is a rail change, not a line
[ |> template_struct_instance]
class template Mx4GemvGuT : Blk32GemvT {
    typedef FMT = KqTag_mx4
    override MX4 = true
    @ssbo @binding = 6 regions3 : array<uint>   //! the up stack's (rel_wblock, xblock) pairs per slot
    @ssbo @binding = 7 @role = "weight" gb : array<float>   //! the gate-bias plane ([ne x d] rows a layer), read under `bias_on`
    @ssbo @binding = 8 @role = "weight" ub : array<float>   //! ... and the up-bias plane
    @ssbo @binding = 9 rex : array<uint>        //! slot -> its expert (the top-k's picks)
    @ssbo @binding = 10 outq : array<int8>      //! the activated hidden rows' Q8_0 quants (the down GEMV's feed)
    @ssbo @binding = 11 outs : array<float>     //! ... and their per-block scales
    @workgroup wg_g : float[32]   //! the block's gate dots
    @workgroup wg_u : float[32]   //! ... and up dots

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#moe-gate-up-fused")]
    def gu_body {
        let row0 = gl_WorkGroupID.x * 32u
        let nsg = gl_NumSubgroups
        let rows_per = 32u / nsg   // every block row in flight: 4 a subgroup at 32 lanes, 8 at 64
        let nb = pa.n / 32u
        var t = 0u
        while (t < rows_per) {
            let lr = gl_SubgroupID + t * nsg
            let rg = row0 + lr
            var g = 0.0
            var u = 0.0
            if (rg < pa.nreg * pa.d) {
                let r = rg / pa.d
                let row = rg % pa.d
                let xb0 = regions[r * 2u + 1u]
                g = row_dot(regions[r * 2u] + row * nb, xb0, nb)
                u = row_dot(regions3[r * 2u] + row * nb, xb0, nb)
            }
            if (subgroupElect()) {
                wg_g[lr] = g
                wg_u[lr] = u
                y[pa.ybase + rg] = g   // the gate rows land in the family's y as well: the dot half's own witness
            }
            t++
        }
        barrier()
        if (gl_SubgroupID == 0u) {
            let lane = gl_SubgroupInvocationID
            let li = min(lane, 31u)
            let rg = row0 + li
            var gv = wg_g[li]
            var uv = wg_u[li]
            if (pa.bias_on != 0u) {   // the slot's expert names the bias rows; the add before the activation is the biased requant kernel's
                let ex = rex[rg / pa.d]
                let c = rg % pa.d
                gv += gb[pa.gboff + ex * pa.d + c]
                uv += ub[pa.gboff + ex * pa.d + c]
            }
            let a = lane < 32u ? gk_act_mul(gv, uv, pa.act) : 0.0
            let s = q8_lane_scale(a)
            if (lane == 0u) {
                outs[gl_WorkGroupID.x] = s.x
            }
            if (lane < 32u) {
                outq[row0 + lane] = q8_quant1(a, s.y)
            }
        }
    }
}

let KQC_NIBBLE_MASK = 0x0F0F0F0Fu
let KQC_ONES = 0x01010101u  //! sdot4 against this sums 4 signed bytes

let KQ_GRID_IQ2S_WORDS = 2048u
let KQ_GRID_IQ2XS_WORDS = 1024u
let KQ_GRID_IQ2XXS_WORDS = 512u
let KQ_GRID_IQ3S_WORDS = 512u
let KQ_GRID_IQ3XXS_WORDS = 256u
let KQ_GRID_IQ2S = 0u
let KQ_GRID_IQ2XS = KQ_GRID_IQ2S + KQ_GRID_IQ2S_WORDS
let KQ_GRID_IQ2XXS = KQ_GRID_IQ2XS + KQ_GRID_IQ2XS_WORDS
let KQ_GRID_IQ3S = KQ_GRID_IQ2XXS + KQ_GRID_IQ2XXS_WORDS
let KQ_GRID_IQ3XXS = KQ_GRID_IQ3S + KQ_GRID_IQ3S_WORDS
let KQ_GRID_WORDS = int(KQ_GRID_IQ3XXS + KQ_GRID_IQ3XXS_WORDS)
let KQ_GRID_BYTES = int64(KQ_GRID_WORDS) * 4l

//! the grid codebooks laid end to end as the kq leaves' `gridb` binding reads them (`KQ_GRID_<FMT>` the word each starts at)
def kq_grid_words() : array<uint> {
    var words : array<uint>
    words |> resize(KQ_GRID_WORDS)
    for (i in range(int(KQ_GRID_IQ2S_WORDS))) {
        words[int(KQ_GRID_IQ2S) + i] = iq2s_grid_word(i)
    }
    for (i in range(int(KQ_GRID_IQ2XS_WORDS))) {
        words[int(KQ_GRID_IQ2XS) + i] = iq2xs_grid_word(i)
    }
    for (i in range(int(KQ_GRID_IQ2XXS_WORDS))) {
        words[int(KQ_GRID_IQ2XXS) + i] = iq2xxs_grid_word(i)
        words[int(KQ_GRID_IQ3S) + i] = iq3s_grid_word(i)
    }
    for (i in range(int(KQ_GRID_IQ3XXS_WORDS))) {
        words[int(KQ_GRID_IQ3XXS) + i] = iq3xxs_grid_word(i)
    }
    return <- words
}

//! the codebook and grid formats: a block's decode gathers, so a lane wants more blocks in flight than the k-lattice's
def gemv_grid_fmt(fmt : int) : bool =>
    fmt == int(KqFmt.iq2s) || fmt == int(KqFmt.iq2xs) || fmt == int(KqFmt.iq2xxs) || fmt == int(KqFmt.iq3xxs) || fmt == int(KqFmt.iq4xs) || fmt == int(KqFmt.iq4nl) || fmt == int(KqFmt.iq3s4) || fmt == int(KqFmt.nv4)

[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#kq-gemv-lanes")]
def gemv_lanes_per_row(fmt : int; n : int64) : uint {
    if (kq_block32(fmt)) return 0u   //! the per-32 formats (q8, q51, mx4): a whole subgroup a row, a lane a block
    let nb = n / 32l
    let grid = gemv_grid_fmt(fmt)
    if (nb <= 24l) return 8u
    if (nb <= 48l) return grid ? 8u : 16u
    if (nb <= 96l) return 16u
    return grid ? 16u : 0u
}

//! the superblock decode GEMV family's planes and the block decode a host caller reaches through a `KqGemvBase?` -
//! the CPU host twin and the kernel cells' oracle run a leaf's `blk_contrib` on the CPU; the shells and the fold are the
//! template chain's (`KqGemvShellT`), stamped into the dispatching module
class KqGemvBase {
    @ssbo @binding = 0 @role = "alias" wq : array<uint>   //! weight quant words (k5's qh rides this view)
    @ssbo @binding = 0 wq4 : array<uint4>      //! vec4 view of the same words
    @ssbo @binding = 1 wsu : array<uint>       //! scale rows: k4 d|dmin + sc + mn, q40 d pairs, k6 sc + d
    @ssbo @binding = 2 regions : array<uint>   //! (rel_wblock, xblock) pairs per region
    @ssbo @binding = 3 xq4 : array<uint4>      //! activation quant words, vec4 view
    @ssbo @binding = 4 axs : array<float>      //! activation block scales
    @ssbo @binding = 5 y : array<float>        //! output rows
    @ssbo @binding = 6 @role = "weight" gridb : array<uint>   //! the grid codebooks (kq_grid_dev); the grid formats stage from it
    @push_constant pa : GemvArgs

    def abstract blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4
    def abstract fold_reads_bsum : bool
    def abstract blk_contrib(wsb, blk, xsb : uint) : float
}

[ |> template_struct_instance]
class template KqGemvShellT : KqGemvBase {
    //! fills wl / wh with the lo / hi int8 quads (x's lo / hi words), returns (A_lo, B_lo, A_hi, B_hi): the block adds axs x (A_lo dot_lo + B_lo bsum_lo + A_hi dot_hi + B_hi bsum_hi)
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 => float4(0.0)

    //! the fold carries B terms (a min or an offset against the block sums): the k-lattice leaves; the codebook and grid leaves skip the sums
    def override fold_reads_bsum : bool => false

    def nib_split(wv : uint4; var wl : uint4&; var wh : uint4&) {
        wl = uint4(wv.x & KQC_NIBBLE_MASK, wv.y & KQC_NIBBLE_MASK, wv.z & KQC_NIBBLE_MASK, wv.w & KQC_NIBBLE_MASK)
        wh = uint4((wv.x >> 4u) & KQC_NIBBLE_MASK, (wv.y >> 4u) & KQC_NIBBLE_MASK, (wv.z >> 4u) & KQC_NIBBLE_MASK, (wv.w >> 4u) & KQC_NIBBLE_MASK)
    }

    def iq4_word_of(n : uint) : uint {
        //! the IQ4_NL codebook's word map: the method a home overrides with its own lowering (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`)
        return iq4_word(n)
    }

    def iq4_split(wv : uint4; var wl : uint4&; var wh : uint4&) {
        wl = uint4(iq4_word_of(wv.x & KQC_NIBBLE_MASK), iq4_word_of(wv.y & KQC_NIBBLE_MASK), iq4_word_of(wv.z & KQC_NIBBLE_MASK), iq4_word_of(wv.w & KQC_NIBBLE_MASK))
        wh = uint4(iq4_word_of((wv.x >> 4u) & KQC_NIBBLE_MASK), iq4_word_of((wv.y >> 4u) & KQC_NIBBLE_MASK), iq4_word_of((wv.z >> 4u) & KQC_NIBBLE_MASK), iq4_word_of((wv.w >> 4u) & KQC_NIBBLE_MASK))
    }

    def dm_terms(wsb, blk : uint) : float4 {
        let sm = k4_scale_min(unpackHalf2x16(wsu[wsb * 5u]), wsu[wsb * 5u + 1u + blk / 4u], wsu[wsb * 5u + 3u + blk / 4u], blk)
        return float4(sm.x, -sm.y, sm.x, -sm.y)
    }

    //! the per-16 signed sub-scale fold terms (Q3_K, Q6_K): each half's d x int8 sub-scale, and its block-sum term at quant offset `off`
    def k6_terms(wsb, blk : uint; off : float) : float4 {
        let s4 = int4(unpack8(int(wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
        let slo = float((blk % 2u) == 0u ? s4.x : s4.z)
        let shi = float((blk % 2u) == 0u ? s4.y : s4.w)
        let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
        return float4(dd * slo, -off * dd * slo, dd * shi, -off * dd * shi)
    }

    //! the per-16 UNSIGNED strip fold terms (IQ2_S, IQ2_XS): each half's d x strip byte, no block-sum term
    def strip_terms(wsb, blk : uint) : float4 {
        let sw = wsu[wsb * 5u + 1u + blk / 2u]
        let s0 = float((sw >> ((blk % 2u) * 16u)) & 255u)
        let s1 = float((sw >> ((blk % 2u) * 16u + 8u)) & 255u)
        let dd = unpackHalf2x16(wsu[wsb * 5u]).x
        return float4(dd * s0, 0.0, dd * s1, 0.0)
    }

    //! the per-32 grid sub-scale fold terms (the iq3 and iq2xxs grids): d x `grid_sc`, no block-sum term
    def grid_terms(wsb, blk : uint) : float4 {
        let a = unpackHalf2x16(wsu[wsb * 2u]).x * float(grid_sc(wsu[wsb * 2u + 1u], blk))
        return float4(a, 0.0, a, 0.0)
    }

    //! the decoded block's dot against activation block `xsb`; the one-column and N-column forms both add its result to
    //! their accumulator as a plain product, so the two modules compile one text and sum bit for bit - a `mad` here
    //! would be a fused multiply-add the CPU oracle's `mad` (a multiply, then an add) does not reproduce
    def blk_fold(wl, wh : uint4; terms : float4; xsb, blk : uint) : float {
        let xb4 = xsb * 16u + blk * 2u
        let xl = xq4[xb4]
        let xh = xq4[xb4 + 1u]
        let dlo = sdot4(wl.x, xl.x) + sdot4(wl.y, xl.y) + sdot4(wl.z, xl.z) + sdot4(wl.w, xl.w)
        let dhi = sdot4(wh.x, xh.x) + sdot4(wh.y, xh.y) + sdot4(wh.z, xh.z) + sdot4(wh.w, xh.w)
        var f = mad(terms.z, float(dhi), terms.x * float(dlo))
        if (fold_reads_bsum()) {
            let blo = sdot4(xl.x, KQC_ONES) + sdot4(xl.y, KQC_ONES) + sdot4(xl.z, KQC_ONES) + sdot4(xl.w, KQC_ONES)
            let bhi = sdot4(xh.x, KQC_ONES) + sdot4(xh.y, KQC_ONES) + sdot4(xh.z, KQC_ONES) + sdot4(xh.w, KQC_ONES)
            f = mad(terms.w, float(bhi), mad(terms.y, float(blo), f))
        }
        return axs[xsb] * f
    }

    def override blk_contrib(wsb, blk, xsb : uint) : float {
        var wl = uint4(0u)
        var wh = uint4(0u)
        let terms = blk_decode(wsb, blk, wl, wh)
        return blk_fold(wl, wh, terms, xsb, blk)
    }

    //! this lane's share of one row's dot: the row's `nsb` superblocks from weight superblock `wsb0` against the
    //! activation's from `xsb0`, the row's `lanes_per_row` lanes striding the 32-blocks, four a step then the tail
    def row_acc(wsb0, xsb0, nsb, lanes_per_row, row_lane : uint) : float {
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 + 4u * lanes_per_row <= nb) {
            let bb = b0 + row_lane
            acc += blk_contrib(wsb0 + bb / 8u, bb % 8u, xsb0 + bb / 8u)
            let b1 = bb + lanes_per_row
            acc += blk_contrib(wsb0 + b1 / 8u, b1 % 8u, xsb0 + b1 / 8u)
            let b2 = b1 + lanes_per_row
            acc += blk_contrib(wsb0 + b2 / 8u, b2 % 8u, xsb0 + b2 / 8u)
            let b3 = b2 + lanes_per_row
            acc += blk_contrib(wsb0 + b3 / 8u, b3 % 8u, xsb0 + b3 / 8u)
            b0 += 4u * lanes_per_row
        }
        while (b0 < nb) {
            let bb = b0 + row_lane
            if (bb < nb) {
                acc += blk_contrib(wsb0 + bb / 8u, bb % 8u, xsb0 + bb / 8u)
            }
            b0 += lanes_per_row
        }
        return acc
    }

    //! the row's fold is an xor-shuffle butterfly over its lanes (the shuffle every GEMV already requires;
    //! a clustered add would want the clustered subgroup feature the tier never checks)
    def fold_lanes(acc : float; lanes_per_row : uint) : float {
        var total = acc
        if (lanes_per_row == 8u) {
            total += subgroupShuffleXor(total, 4u)
            total += subgroupShuffleXor(total, 2u)
            total += subgroupShuffleXor(total, 1u)
        } elif (lanes_per_row == 16u) {
            total += subgroupShuffleXor(total, 8u)
            total += subgroupShuffleXor(total, 4u)
            total += subgroupShuffleXor(total, 2u)
            total += subgroupShuffleXor(total, 1u)
        } else {
            total = subgroupAdd(acc)
        }
        return total
    }

    [arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#kq-gemv-lanes")]
    def gemv_shell {
        let lanes_per_row = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
        let lane = gl_SubgroupInvocationID
        let row_lane = lane % lanes_per_row
        let rg = gemv_lane_row(gl_WorkGroupID.x, gl_SubgroupID, lane, lanes_per_row, gl_SubgroupSize, gl_NumSubgroups)
        let live = rg < pa.nreg * pa.d
        let nreg = max(pa.nreg, 1u)   // the divisor guarded: a dead lane computes it too
        let r = rg % nreg
        let row = rg / nreg
        var acc = 0.0
        if (live) {
            let nsb = pa.n / 256u
            acc = row_acc(regions[r * 2u] + row * nsb, regions[r * 2u + 1u], nsb, lanes_per_row, row_lane)
        }
        let total = fold_lanes(acc, lanes_per_row)
        if (live && row_lane == 0u) {
            y[pa.ybase + r * pa.d + row] = total
        }
    }

    //! the N-column form: a lane's blocks decode once and dot against every column's activation row (column c's
    //! superblocks sit c x nsb past the region's), `pa.ncols` columns to 8, their y rows `pa.ystride` apart
    def gemv_shell_n {
        let lanes_per_row = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
        let lane = gl_SubgroupInvocationID
        let row_lane = lane % lanes_per_row
        let rg = gemv_lane_row(gl_WorkGroupID.x, gl_SubgroupID, lane, lanes_per_row, gl_SubgroupSize, gl_NumSubgroups)
        let live = rg < pa.nreg * pa.d
        var acc : float[8]
        for [unroll] (c in range(8)) {
            acc[c] = 0.0
        }
        if (live) {
            let r = rg / pa.d
            let row = rg % pa.d
            let nsb = pa.n / 256u
            let wsb0 = regions[r * 2u] + row * nsb
            let xsb0 = regions[r * 2u + 1u]
            let nb = nsb * 8u
            var b0 = row_lane
            while (b0 < nb) {
                let wsb = wsb0 + b0 / 8u
                let blk = b0 % 8u
                let xsb = xsb0 + b0 / 8u
                var wl = uint4(0u)
                var wh = uint4(0u)
                let terms = blk_decode(wsb, blk, wl, wh)
                for [unroll] (c in range(8)) {
                    if (uint(c) < pa.ncols) {   // uniform on the push constant; the clamp form folds every dead column against a live row and costs 1.6x at four columns
                        acc[c] += blk_fold(wl, wh, terms, xsb + (pa.col0 + uint(c)) * nsb, blk)
                    }
                }
                b0 += lanes_per_row
            }
        }
        for [unroll] (c in range(8)) {
            if (uint(c) < pa.ncols) {
                let total = fold_lanes(acc[c], lanes_per_row)
                if (live && row_lane == 0u) {
                    y[pa.ybase + (pa.col0 + uint(c)) * pa.ystride + rg] = total
                }
            }
        }
    }
}

//! every decode GEMV leaf's shape: the format's decode on a `<Fmt>GemvT` template, its one-column and N-column stamps
//! two overrides of `NCOL`, and a grid format's codebook staged into workgroup memory ahead of either shell
[ |> template_struct_instance]
class template KqGemvLeafT : KqGemvShellT {
    @template_constant NCOL : bool = false      //!< the N-column shell (`gemv_shell_n`) in place of the one-column one
    @template_constant GU : bool = false        //!< the routed gate and up GEMVs with the act and its Q8_0 requant (`gu_shell`)
    @ssbo @binding = 7 @template_gate = GU regions3 : array<uint>   //! the up stack's (rel_wblock, xblock) pairs per slot
    @ssbo @binding = 8 @template_gate = GU outq : array<int8>       //! the activated hidden rows' Q8_0 quants (the down GEMV's feed)
    @ssbo @binding = 9 @template_gate = GU outs : array<float>      //! ... and their per-block scales
    @workgroup @template_gate = GU wg_g : float[256]   //! a turn's gate dots, a lane's at its slot
    @workgroup @template_gate = GU wg_u : float[256]   //! ... and up dots
    @template_constant EMB : bool = false       //!< the token-table gather (`emb_shell`) in place of either GEMV shell
    @template_constant GRID : bool = false      //!< a grid-codebook format: `GRID_WORDS` words of `gridb` from `GRID_OFF` stage into `gridw` ahead of the shell
    @template_constant GRID_WORDS : uint = 64u  //!< the codebook's words and its first word in `gridb` - literals (template constants), held equal to the format's `KQ_GRID_<FMT>_WORDS` / `KQ_GRID_<FMT>` by REVIEW.das
    @template_constant GRID_OFF : uint = 0u
    @workgroup @template_gate = GRID gridw : uint[int(GRID_WORDS)]

    def stage_grid {
        static_if (GRID) {
            let lid = gl_LocalInvocationID.x
            for (k in range(int(GRID_WORDS / 64u))) {
                let idx = lid + uint(k) * 64u
                gridw[idx] = gridb[GRID_OFF + idx]
            }
            barrier()
        }
    }

    //! the four staged grid words at `i`, sign-applied by the nibbles `sg` (a grid format's leaf; the pick helpers name the words)
    def grid4(i, sg : uint4) : uint4 {
        static_if (GRID) {
            return uint4(iq3s_signed(gridw[i.x], sg.x), iq3s_signed(gridw[i.y], sg.y), iq3s_signed(gridw[i.z], sg.z), iq3s_signed(gridw[i.w], sg.w))
        } else {
            return uint4(0u)
        }
    }

    //! the routed experts' gate and up GEMVs over the slot regions, the activation and its Q8_0 requant in one dispatch (`Q8GemvGuNT`'s
    //! form): the gate stack's regions at binding 2, the up stack's at 7, the one feed row; a workgroup of 256 lanes a 32-row block
    [arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#moe-gate-up-fused")]
    def gu_shell {
        static_if (GU) {
            static_if (GRID) {   // a grid format's codebook staged over the 256 lanes (`stage_grid` strides 64)
                var gi = gl_LocalInvocationID.x
                while (gi < GRID_WORDS) {
                    gridw[gi] = gridb[GRID_OFF + gi]
                    gi += 256u
                }
                barrier()
            }
            let lanes_per_row = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
            let lane = gl_SubgroupInvocationID
            let row_lane = lane % lanes_per_row
            let wg_rows_per_turn = gl_NumSubgroups * (gl_SubgroupSize / lanes_per_row)
            let slot = gl_SubgroupID * gl_SubgroupSize + lane
            let row0 = gl_WorkGroupID.x * 32u
            let nsb = pa.n / 256u
            var gj = 0.0   // subgroup 0's lane j < 32: row j's gate and up dots, gathered off the turn that serves it
            var uj = 0.0
            var t = 0u
            while (t * wg_rows_per_turn < 32u) {
                let lr = gemv_lane_row(t, gl_SubgroupID, lane, lanes_per_row, gl_SubgroupSize, gl_NumSubgroups)
                let rg = row0 + lr
                let live = lr < 32u && rg < pa.nreg * pa.d
                var g = 0.0
                var u = 0.0
                if (live) {
                    let r = rg / pa.d
                    let row = rg % pa.d
                    let xsb0 = regions[r * 2u + 1u]
                    g = row_acc(regions[r * 2u] + row * nsb, xsb0, nsb, lanes_per_row, row_lane)
                    u = row_acc(regions3[r * 2u] + row * nsb, xsb0, nsb, lanes_per_row, row_lane)
                }
                g = fold_lanes(g, lanes_per_row)
                u = fold_lanes(u, lanes_per_row)
                if (live && row_lane == 0u) {
                    y[pa.ybase + rg] = g   // the gate rows land in the family's y as well: the dot half's own witness
                }
                wg_g[slot] = g   // every lane of a row holds its folded dots
                wg_u[slot] = u
                barrier()
                if (gl_SubgroupID == 0u && lane < 32u && lane >= t * wg_rows_per_turn && lane < (t + 1u) * wg_rows_per_turn) {
                    let src = gemv_lane_row_slot(lane, t, lanes_per_row, gl_SubgroupSize, gl_NumSubgroups)
                    gj = wg_g[src]
                    uj = wg_u[src]
                }
                barrier()
                t++
            }
            if (gl_SubgroupID == 0u) {
                let a = lane < 32u ? gk_act_mul(gj, uj, pa.act) : 0.0
                let s = q8_lane_scale(a)
                if (lane == 0u) {
                    outs[gl_WorkGroupID.x] = s.x
                }
                if (lane < 32u) {
                    outq[row0 + lane] = q8_quant1(a, s.y)
                }
            }
        }
    }

    //! the token-table gather (`EMB`): a thread a 32-block of a row, decoded by the format's own `blk_decode` - element k of the block is
    //! lane k of the lo quad (k < 16) under the lo terms or of the hi quad under the hi ones, the term pair `blk_fold` reads - scaled by
    //! `pa.escale`; row r's token id is `regions[r]`, and an `EMB_ID_HOST` row copies the host's staged row out of `axs`
    [arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#token-table-gather")]
    def emb_shell {
        let gid = gl_GlobalInvocationID.x
        let nsb = pa.n / 256u
        let nb = nsb * 8u
        if (gid < pa.nreg * nb) {
            let row = gid / max(nb, 1u)
            let b = gid - row * nb
            let o = row * pa.n + b * 32u
            let id = regions[row]
            if (id == EMB_ID_HOST) {
                for [unroll_full] (k in range(32)) {
                    y[o + uint(k)] = axs[o + uint(k)]
                }
            } else {
                var wl = uint4(0u)
                var wh = uint4(0u)
                let terms = blk_decode(pa.wblk0 + id * nsb + b / 8u, b % 8u, wl, wh)
                let bsum = fold_reads_bsum()
                emb_store16(o, wl, terms.x, bsum ? terms.y : 0.0)
                emb_store16(o + 16u, wh, terms.z, bsum ? terms.w : 0.0)
            }
        }
    }

    //! the 16 int8 lanes of `w` at y[o ..], each `a x q + c` under the embed scale
    def emb_store16(o : uint; w : uint4; a, c : float) {
        emb_store4(o, w.x, a, c)
        emb_store4(o + 4u, w.y, a, c)
        emb_store4(o + 8u, w.z, a, c)
        emb_store4(o + 12u, w.w, a, c)
    }

    def emb_store4(o, w : uint; a, c : float) {
        let q = float4(int4(unpack8(int(w))))
        y[o] = (a * q.x + c) * pa.escale
        y[o + 1u] = (a * q.y + c) * pa.escale
        y[o + 2u] = (a * q.z + c) * pa.escale
        y[o + 3u] = (a * q.w + c) * pa.escale
    }

    [spirv_inline]
    def leaf_body {
        static_if (GRID) {
            stage_grid()
        }
        static_if (EMB) {
            emb_shell()
        } static_elif (NCOL) {
            gemv_shell_n()
        } else {
            gemv_shell()
        }
    }
}

//! Q4_K: w = d*sc*q - dmin*mn; a block axs*(d*sc*idot - dmin*mn*bsum)
[ |> template_struct_instance]
class template K4GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        nib_split(wq4[wsb * 8u + blk], wl, wh)
        return dm_terms(wsb, blk)
    }

    def override fold_reads_bsum : bool => true
}

//! Q4_0: w = d*(q - 8); the -8 folds against the block sum
[ |> template_struct_instance]
class template Q40GemvT : KqGemvLeafT {
    def q40_d(wsb, blk : uint) : float => q40_d_of(wsu[wsb * 5u + blk / 2u], blk)

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        nib_split(wq4[wsb * 8u + blk], wl, wh)
        let d = q40_d(wsb, blk)
        return float4(d, -8.0 * d, d, -8.0 * d)
    }

    def override fold_reads_bsum : bool => true
}

//! IQ4_XS: w = d*sc*LUT[q], sc = ls - 32 signed; no bsum, no min
[ |> template_struct_instance]
class template Iq4xsGemvT : KqGemvLeafT {
    @template_constant IQODD : bool = false   //!< the iq3s4 form: the odd codebook 2q - 15 and the iq3 row's (1 + 2s) sub-scale over the same nibble tiling

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let wv = wq4[wsb * 8u + blk]
        static_if (IQODD) {
            wl = uint4(iq3s4_word(wv.x & KQC_NIBBLE_MASK), iq3s4_word(wv.y & KQC_NIBBLE_MASK), iq3s4_word(wv.z & KQC_NIBBLE_MASK), iq3s4_word(wv.w & KQC_NIBBLE_MASK))
            wh = uint4(iq3s4_word((wv.x >> 4u) & KQC_NIBBLE_MASK), iq3s4_word((wv.y >> 4u) & KQC_NIBBLE_MASK), iq3s4_word((wv.z >> 4u) & KQC_NIBBLE_MASK), iq3s4_word((wv.w >> 4u) & KQC_NIBBLE_MASK))
            return grid_terms(wsb, blk)
        } else {
            let w0 = wsu[wsb * 2u]
            let w1 = wsu[wsb * 2u + 1u]
            iq4_split(wv, wl, wh)
            let a = unpackHalf2x16(w0).x * float(iq4xs_sc(w0, w1, blk))
            return float4(a, 0.0, a, 0.0)
        }
    }
}

//! IQ3_S: grid word off 8-bit index | qh bit, sign bytes, (1+2s)*d
[ |> template_struct_instance]
class template Iq3sGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 512u
    override GRID_OFF = 3584u

    def iq3s_quad(qsw, qh, sgw, l0 : uint) : uint4 => grid4(iq3s_idx4(qsw, qh, l0), sign_word_sgn4(sgw, l0))

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qb = wsb * 26u + blk * 2u
        let qh = (wq[wsb * 26u + 16u + blk / 4u] >> ((blk % 4u) * 8u)) & 255u
        let sgw = wq[wsb * 26u + 18u + blk]
        wl = iq3s_quad(wq[qb], qh, sgw, 0u)
        wh = iq3s_quad(wq[qb + 1u], qh, sgw, 2u)
        return grid_terms(wsb, blk)
    }
}

//! IQ3_XXS: halved grid, parity ksigns, (2ls + 1) x the halved d
[ |> template_struct_instance]
class template Iq3xxsGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 256u
    override GRID_OFF = 4096u

    def iq3xxs_quad(qsw, aux, l0 : uint) : uint4 => grid4(iq3xxs_idx4(qsw), iq3xxs_sgn4(aux, l0))

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qb = wsb * 24u + blk * 2u
        let aux = wq[wsb * 24u + 16u + blk]
        wl = iq3xxs_quad(wq[qb], aux, 0u)
        wh = iq3xxs_quad(wq[qb + 1u], aux, 2u)
        return grid_terms(wsb, blk)
    }

}

//! IQ4_NL: iq4xs's codebook dot under q40's per-block f16 d
[ |> template_struct_instance]
class template Iq4nlGemvT : Q40GemvT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        iq4_split(wq4[wsb * 8u + blk], wl, wh)
        let d = q40_d(wsb, blk)
        return float4(d, 0.0, d, 0.0)
    }

    def override fold_reads_bsum : bool => false
}

//! NVFP4: w = sc16 x MX4_LUT[q], sc16 a sub-block's UE4M3 scale at half under the plane's per-tensor scale (the device row's
//! fifth word) - a 32-block's two halves under two scales off the row's byte pair 2g, 2g + 1; signed codes, so no bsum term
[ |> template_struct_instance]
class template Nv4GemvT : KqGemvLeafT {
    //! one quant word's eight nibble codes to signed int8 lanes of the doubled e2m1 magnitudes, the low nibbles' four in `lo`
    //! and the high nibbles' in `hi`: the SWAR formula here, a home with a byte permute overrides it (`Nv4GemvPrmtT`)
    def e2m1_split(w : uint; var lo, hi : uint&) {
        lo = mx4_word(w & KQC_NIBBLE_MASK)
        hi = mx4_word((w >> 4u) & KQC_NIBBLE_MASK)
    }

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let wv = wq4[wsb * 8u + blk]
        var l0 = 0u
        var l1 = 0u
        var l2 = 0u
        var l3 = 0u
        var h0 = 0u
        var h1 = 0u
        var h2 = 0u
        var h3 = 0u
        e2m1_split(wv.x, l0, h0)
        e2m1_split(wv.y, l1, h1)
        e2m1_split(wv.z, l2, h2)
        e2m1_split(wv.w, l3, h3)
        wl = uint4(l0, l1, l2, l3)
        wh = uint4(h0, h1, h2, h3)
        let sw = wsu[wsb * 5u + blk / 2u] >> ((blk & 1u) * 16u)
        let site_scale = uint_bits_to_float(wsu[wsb * 5u + 4u])
        return float4(ue4m3_half_f32(sw & 0xFFu) * site_scale, 0.0, ue4m3_half_f32((sw >> 8u) & 0xFFu) * site_scale, 0.0)
    }

    def override fold_reads_bsum : bool => false
}

//! Q3_K: q = 2-bit qs | hmask bit << 2, minus 4; signed per-16 sc x d
[ |> template_struct_instance]
class template K3GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let q4 = wsb * 6u + (blk / 4u) * 2u
        let sh = (blk % 4u) * 2u
        wl = k3_quad(wq4[q4], wq4[wsb * 6u + 4u], sh, blk)
        wh = k3_quad(wq4[q4 + 1u], wq4[wsb * 6u + 5u], sh, blk)
        return k6_terms(wsb, blk, 4.0)
    }

    def override fold_reads_bsum : bool => true
}

//! Q2_K: q = the 2-bit qs lane; per-16 sc/min nibble pairs x d/dmin
[ |> template_struct_instance]
class template K2GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let q4 = wsb * 4u + (blk / 4u) * 2u
        let sh = (blk % 4u) * 2u
        wl = k2_quad(wq4[q4], sh)
        wh = k2_quad(wq4[q4 + 1u], sh)
        let w = wsu[wsb * 5u + 1u + blk / 2u]
        let b0 = (w >> ((blk % 2u) * 16u)) & 255u
        let b1 = (w >> ((blk % 2u) * 16u + 8u)) & 255u
        let dm = unpackHalf2x16(wsu[wsb * 5u])
        return float4(dm.x * float(b0 & 15u), -dm.y * float(b0 >> 4u), dm.x * float(b1 & 15u), -dm.y * float(b1 >> 4u))
    }

    def override fold_reads_bsum : bool => true
}

//! Q5_K: the k4 fold, the 5th bit deposited from the block's qh
[ |> template_struct_instance]
class template K5GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let wv = wq4[wsb * 10u + blk]
        let qh = wq[wsb * 40u + 32u + blk]
        wl = k5_quad(wv, qh, 0u)
        wh = k5_quad(wv, qh, 4u)
        return dm_terms(wsb, blk)
    }

    def override fold_reads_bsum : bool => true
}

//! Q6_K: q = (ql | qh << 4) - 32; signed per-16 int8 sub-scales
[ |> template_struct_instance]
class template K6GemvT : KqGemvLeafT {
    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qh4 = wsb * 12u + 8u + (blk / 4u) * 2u
        let shift = (blk % 4u) * 2u
        var nl = uint4(0u)
        var nh = uint4(0u)
        nib_split(wq4[wsb * 12u + blk], nl, nh)
        wl = k6_quad(nl, wq4[qh4], shift)
        wh = k6_quad(nh, wq4[qh4 + 1u], shift)
        return k6_terms(wsb, blk, 32.0)
    }

    def override fold_reads_bsum : bool => true
}

//! IQ2_S: 10-bit index, two u64 grid words, sign bytes, (1+2s) strips
[ |> template_struct_instance]
class template Iq2sGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 2048u
    override GRID_OFF = 0u

    def iq2s_quad(qw, qh, sgw, l0 : uint) : uint4 => grid4(iq2s_idx4(qw, qh, l0), sign_word_sgn4(sgw, l0))

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let qw = wq[wsb * 18u + blk]
        let sgw = wq[wsb * 18u + 8u + blk]
        let qh = (wq[wsb * 18u + 16u + blk / 4u] >> ((blk % 4u) * 8u)) & 255u
        wl = iq2s_quad(qw, qh, sgw, 0u)
        wh = iq2s_quad(qw, qh, sgw, 2u)
        return strip_terms(wsb, blk)
    }

}

//! IQ2_XS: a u16 qs word holds a 9-bit grid and a 7-bit ksigns index
[ |> template_struct_instance]
class template Iq2xsGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 1024u
    override GRID_OFF = 2048u

    def iq2xs_quad(qw : uint) : uint4 => grid4(iq2xs_idx4(qw), iq2xs_sgn4(qw))

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        wl = iq2xs_quad(wq[wsb * 16u + blk * 2u])
        wh = iq2xs_quad(wq[wsb * 16u + blk * 2u + 1u])
        return strip_terms(wsb, blk)
    }

}

//! IQ2_XXS: iq3xxs's shell over the u64 grid, parity ksigns
[ |> template_struct_instance]
class template Iq2xxsGemvT : KqGemvLeafT {
    override GRID = true
    override GRID_WORDS = 512u
    override GRID_OFF = 3072u

    def iq2xxs_quad(gw, aux, hu : uint) : uint4 => grid4(iq2xxs_idx4(gw, hu), iq2xxs_sgn4(aux, hu))

    def override blk_decode(wsb, blk : uint; var wl : uint4&; var wh : uint4&) : float4 {
        let gw = wq[wsb * 16u + blk * 2u]
        let aux = wq[wsb * 16u + blk * 2u + 1u]
        wl = iq2xxs_quad(gw, aux, 0u)
        wh = iq2xxs_quad(gw, aux, 1u)
        return grid_terms(wsb, blk)
    }

}

//! the per-token words: push constants freeze at record time
struct TokMeta {
    pos : uint      //! absolute position: mirror row = layerbase + pos * row units
    cnt : uint      //! cached positions incl. this one
    parity : uint
    mirbase : uint  //! the N-row command: this row's K/V mirror region as a unit offset; 0 in the one-row command, whose push carries the region
    dnslot : uint
    pad0 : uint
    pad1 : uint
    pad2 : uint
}

let public TOK_META_BYTES = 32l   //! one TokMeta row (the kernels index the block by row)

let KV_Q8_BLOCK_HALVES = 17u   //! a q8_0 K/V mirror block in halfwords: its f16 scale, then 32 quant bytes (the CPU cache's block)

let KV_TQ4_BLOCK_HALVES = 9u   //! a tq4 K/V mirror block in halfwords: its f16 scale, then 16 bytes of two 4-bit quants each (the CPU cache's block)

let KV_TQ4_SIGNS = 512l        //! the rotation's sign row: the widest head a kernel's row slab holds, a narrower head reading its prefix

//! A block's f16 scale: the half at byte `bb` of the mirror, in the word `w` that holds it (a block starts 0 or 2 bytes off a word).
def kv_blk_scale(w, bb : uint) : float => unpackHalf2x16(w >> ((bb & 2u) * 8u)).x

//! The four bytes at byte `at` of the mirror, `lo` the word that holds byte `at` and `hi` the next.
def kv_skew_word(lo, hi, at : uint) : uint => (at & 2u) == 0u ? lo : ((lo >> 16u) | (hi << 16u))

//! Byte `k` of `w` as a q8_0 quant.
def kv_q8_quant(w : uint; k : int) : float => float(int(w << uint(24 - 8 * k)) >> 24)

//! The low and the high nibble of byte `by` as tq4 quants.
def kv_tq4_quant_lo(by : uint) : float => float(int(by & 15u) - 8)

def kv_tq4_quant_hi(by : uint) : float => float(int(by >> 4u) - 8)

struct KvQStoreArgs {
    kvd : uint        //! a K/V row's elements
    rows : uint
    layerbase : uint  //! this layer's base in the mirror, in blocks
    pos0 : uint       //! the window stamp: staged row 0's absolute position
    ring : uint       //! a sliding layer's ring rows: a row lands at `kv_ring_row` of its position; 0 = the flat slab
}

//! The q8_0 K/V store: a lane an element, thirty-two lanes a block (a subgroup, or a 32-lane half of a wider one - the
//! max folds by xor shuffles that stay inside the half), each quant rounded under the unrounded f32 inverse scale as
//! `quantize_q8kv_row` rounds it.
[ |> template_struct_instance]
class template KvQ8StoreT {
    @ssbo @binding = 0 kst : array<float>       //! the staged K rows [rows x kvd]
    @ssbo @binding = 1 vst : array<float>
    @ssbo @binding = 2 ksc : array<float16>
    @ssbo @binding = 2 @role = "alias" kq : array<int8>
    @ssbo @binding = 3 vsc : array<float16>
    @ssbo @binding = 3 @role = "alias" vq : array<int8>
    @ssbo @binding = 4 @template_gate = TOKROW tok : array<TokMeta>
    @push_constant pa : KvQStoreArgs
    @template_constant TOKROW : bool = true     //!< each staged row lands at the mirror position its token block names (its region in blocks); off, row `r` lands at position `pos0 + r`

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs")]
    def store_body {
        let gid = gl_GlobalInvocationID.x
        let nblk = pa.kvd / KQ_B32_ELEMS_U
        if (gid < nblk * pa.rows * KQ_B32_ELEMS_U) {   // whole 32-lane groups stand together: the grid is whole blocks
            let e = gid % KQ_B32_ELEMS_U
            let b = gid / KQ_B32_ELEMS_U
            let row = b / nblk
            let blk = b % nblk
            let src = row * pa.kvd + blk * KQ_B32_ELEMS_U + e
            let kv0 = kst[src]
            let vv0 = vst[src]
            let ks = q8_lane_scale(kv0)
            let vs = q8_lane_scale(vv0)
            var mir_blk = pa.layerbase + blk
            static_if (TOKROW) {
                mir_blk += tok[row].mirbase + kv_ring_row(tok[row].pos, pa.ring) * nblk
            } else {
                mir_blk += kv_ring_row(pa.pos0 + row, pa.ring) * nblk
            }
            let sc = mir_blk * KV_Q8_BLOCK_HALVES
            if (e == 0u) {
                ksc[sc] = float16(min(ks.x, 65504.0))
                vsc[sc] = float16(min(vs.x, 65504.0))
            }
            let qb = sc * 2u + 2u
            kq[qb + e] = q8_quant1(kv0, ks.y)
            vq[qb + e] = q8_quant1(vv0, vs.y)
        }
    }
}

//! the tq4 rotation's butterfly passes over a workgroup's head: a base shell the store and the un-rotation share
class template KvTq4WhtBase : GkBlkSignedMax {
    @workgroup buf : float[512]                 //! the head: hs <= 512, a thread's second element at tid + 256

    //! the unscaled Walsh-Hadamard transform of `buf[0 .. hs)` in place, `hs` a power of two: the five passes inside a
    //! 32-lane group fold by xor shuffles (a lane's partner is its lane xor the pass's stride), the passes across groups
    //! through the buffer, barrier-ordered; a subgroup narrower than 32 lanes takes every pass through the buffer
    def wht(hs : uint) {
        let tid = gl_LocalInvocationID.x
        let t1 = tid + 256u
        var h = 1u
        if (gl_SubgroupSize >= 32u) {
            var v0 = tid < hs ? buf[tid] : 0.0
            var v1 = t1 < hs ? buf[t1] : 0.0
            for [unroll] (sh in range(5)) {
                let hh = 1u << uint(sh)
                let o0 = subgroupShuffleXor(v0, hh)
                let o1 = subgroupShuffleXor(v1, hh)
                v0 = (tid & hh) == 0u ? v0 + o0 : o0 - v0
                v1 = (tid & hh) == 0u ? v1 + o1 : o1 - v1
            }
            if (tid < hs) {
                buf[tid] = v0
            }
            if (t1 < hs) {
                buf[t1] = v1
            }
            barrier()
            h = 32u
        }
        while (h < hs) {
            let a0 = buf[tid & ~h]
            let b0 = buf[(tid & ~h) | h]
            let a1 = buf[t1 & ~h]
            let b1 = buf[(t1 & ~h) | h]
            barrier()
            if (tid < hs) {
                buf[tid] = (tid & h) == 0u ? a0 + b0 : a0 - b0
            }
            if (t1 < hs) {
                buf[t1] = (t1 & h) == 0u ? a1 + b1 : a1 - b1
            }
            barrier()
            h *= 2u
        }
    }
}

struct KvTqStoreArgs {
    kvd : uint        //! a K/V row's elements
    hs : uint         //! the head: a power of two, 32 or more
    layerbase : uint  //! this layer's base in the mirror, in blocks
    pos0 : uint       //! the window stamp: staged row 0's absolute position
    nqh : uint        //! q heads a row, rotated in place; 0 on the NextN head's prompt warm
    nkh : uint        //! k heads a row, and as many v heads; 0 on a shared-KV layer
    qrow : uint       //! a row's stride in the q plane
    qstride : uint    //! a q head's stride: hs, or 2 * hs on a gated plane ([q | gate] a head, the gate half left in place)
    inv_sqrt_hs : float       //! the transform's scale 1 / sqrt(hs), as the host computes it
    ring : uint       //! a sliding layer's ring rows: a row lands at `kv_ring_row` of its position; 0 = the flat slab
}

//! The tq4 store: a workgroup a head, a lane an element. Per 32-element block the scale is the largest-magnitude value over
//! -8 (the block's lanes fold it by shuffles) and each quant truncates value / scale + 8.5, capped at 15, as
//! `quantize_tq4kv_row` does. A home's leaf runs it under its precise option: the butterflies and the quant sums round as the CPU's.
[ |> template_struct_instance]
class template KvTq4StoreT : KvTq4WhtBase {
    @ssbo @binding = 0 q : array<float>
    @ssbo @binding = 1 kst : array<float>       //! the staged K rows [rows x kvd]
    @ssbo @binding = 2 vst : array<float>
    @ssbo @binding = 3 ksc : array<float16>     //! the K mirror's block scales
    @ssbo @binding = 3 @role = "alias" kq : array<int8>
    @ssbo @binding = 4 vsc : array<float16>
    @ssbo @binding = 4 @role = "alias" vq : array<int8>
    @ssbo @binding = 5 @role = "weight" signs : array<float>
    @ssbo @binding = 6 @template_gate = TOKROW tok : array<TokMeta>
    @push_constant pa : KvTqStoreArgs
    @template_constant TOKROW : bool = true     //!< each row lands at the mirror position its token block names (its region in blocks); off, row `r` lands at position `pos0 + r`

    def head_elem(is_q, is_v : bool; qb, sb, e : uint) : float => is_q ? q[qb + e] : (is_v ? vst[sb + e] : kst[sb + e])

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis")]
    def store_body {
        let rowwg = max(1u, pa.nqh + 2u * pa.nkh)
        let row = gl_WorkGroupID.x / rowwg
        let wid = gl_WorkGroupID.x % rowwg
        let is_q = wid < pa.nqh
        let is_v = wid >= pa.nqh + pa.nkh
        let hh = is_q ? wid : (is_v ? wid - pa.nqh - pa.nkh : wid - pa.nqh)
        let hs = pa.hs
        let tid = gl_LocalInvocationID.x
        let t1 = tid + 256u
        let two = t1 < hs
        let qb = row * pa.qrow + hh * pa.qstride
        let sb = row * pa.kvd + hh * hs
        if (tid < hs) {
            buf[tid] = head_elem(is_q, is_v, qb, sb, tid) * signs[tid]
        }
        if (two) {
            buf[t1] = head_elem(is_q, is_v, qb, sb, t1) * signs[t1]
        }
        barrier()
        wht(hs)
        if (is_q) {
            if (tid < hs) {
                q[qb + tid] = buf[tid] * pa.inv_sqrt_hs
            }
            if (two) {
                q[qb + t1] = buf[t1] * pa.inv_sqrt_hs
            }
        } else {
            let nblk = pa.kvd / KQ_B32_ELEMS_U
            var mir_blk = pa.layerbase + hh * (hs / KQ_B32_ELEMS_U)
            static_if (TOKROW) {
                mir_blk += tok[row].mirbase + kv_ring_row(tok[row].pos, pa.ring) * nblk
            } else {
                mir_blk += kv_ring_row(pa.pos0 + row, pa.ring) * nblk
            }
            var e0 = 0u   // every lane walks: the shuffle fold needs a whole subgroup
            while (e0 < hs) {
                let e = e0 + tid
                let inside = e < hs
                let blk = e / KQ_B32_ELEMS_U
                // a tie keeps the lower index, as the CPU's compare
                var mval = inside ? buf[e] * pa.inv_sqrt_hs : 0.0
                blk_signed_max(e % KQ_B32_ELEMS_U, mval)
                let d = mval / -8.0
                let id = d != 0.0 ? 1.0 / d : 0.0
                let sc = (mir_blk + blk) * KV_TQ4_BLOCK_HALVES
                let lane = e % KQ_B32_ELEMS_U
                if (inside && lane == 0u) {
                    if (is_v) {
                        vsc[sc] = half_sat(d)
                    } else {
                        ksc[sc] = half_sat(d)
                    }
                }
                if (inside && (lane & 1u) == 0u) {
                    let q0 = min(15, int(buf[e] * pa.inv_sqrt_hs * id + 8.5))
                    let q1 = min(15, int(buf[e + 1u] * pa.inv_sqrt_hs * id + 8.5))
                    let at = sc * 2u + 2u + lane / 2u
                    if (is_v) {
                        vq[at] = int8(q0 | (q1 << 4))
                    } else {
                        kq[at] = int8(q0 | (q1 << 4))
                    }
                }
                e0 += 256u
            }
        }
    }
}

struct KvTqUnrotArgs {
    hs : uint         //! the head: a power of two
    nh : uint
    orow : uint       //! a row's stride in the attention-out plane
    qrow : uint       //! a row's stride in the q plane
    qstride : uint    //! a q head's stride: hs, or 2 * hs on a gated plane, whose gate half the output then takes the sigmoid of
    inv_sqrt_hs : float
}

//! tq4's un-rotation of the attention rows, a workgroup a (row, head): the inverse transform, then the signs, then a gated
//! model's gate - what acts on the attention row acts in the model's basis
[ |> template_struct_instance]
class template KvTq4UnrotT : KvTq4WhtBase {
    @ssbo @binding = 0 y : array<float>         //! the attention out rows, in place
    @ssbo @binding = 1 q : array<float>         //! the q plane, read for its gate halves
    @ssbo @binding = 2 @role = "weight" signs : array<float>
    @push_constant pa : KvTqUnrotArgs

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis")]
    def unrot_body {
        let nh = max(1u, pa.nh)
        let row = gl_WorkGroupID.x / nh
        let hh = gl_WorkGroupID.x % nh
        let hs = pa.hs
        let tid = gl_LocalInvocationID.x
        let t1 = tid + 256u
        let two = t1 < hs
        let ob = row * pa.orow + hh * hs
        if (tid < hs) {
            buf[tid] = y[ob + tid]
        }
        if (two) {
            buf[t1] = y[ob + t1]
        }
        barrier()
        wht(hs)
        let gated = pa.qstride != hs
        let gb = row * pa.qrow + hh * pa.qstride + hs
        if (tid < hs) {
            let o = buf[tid] * pa.inv_sqrt_hs * signs[tid]
            y[ob + tid] = gated ? sig_gate(o, q[gb + tid]) : o
        }
        if (two) {
            let o = buf[t1] * pa.inv_sqrt_hs * signs[t1]
            y[ob + t1] = gated ? sig_gate(o, q[gb + t1]) : o
        }
    }
}

struct KvQShadowArgs {
    kvd : uint        //! a K/V row's elements
    pos0 : uint       //! the first position read
    rows : uint
    layerbase : uint  //! the layer's base in its region of the mirror, in blocks
    ring : uint       //! a sliding layer's ring rows: position `pos` reads its `kv_ring_row` slot; 0 = the flat slab
}

//! A block-codec mirror's f16 shadow: a thread a block stores the block's 32 dequantized K and V values as halves at their
//! position's row of the shadow planes, what the prefill's attention tiles load. tq4's rows stay in the rotated basis.
[ |> template_struct_instance]
class template KvShadowT {
    @ssbo @binding = 0 kmir : array<uint>
    @ssbo @binding = 1 vmir : array<uint>
    @ssbo @binding = 2 ksh : array<float16>     //! the K shadow: [position x kvd] halves
    @ssbo @binding = 3 vsh : array<float16>
    @push_constant pa : KvQShadowArgs
    @template_constant TQ4 : bool = false       //!< the tq4 stamp: 18-byte blocks of four-bit quants

    def shadow_half(quant, scale : float) : float16 => half_sat(quant * scale)

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs")]
    def shadow_body {
        let gid = gl_GlobalInvocationID.x
        let nblk = pa.kvd / KQ_B32_ELEMS_U
        if (gid < nblk * pa.rows) {
            let pos = pa.pos0 + gid / nblk
            let blk = gid % nblk
            var bb = pa.layerbase + kv_ring_row(pos, pa.ring) * nblk + blk
            static_if (TQ4) {
                bb *= KV_TQ4_BLOCK_BYTES
            } else {
                bb *= KV_Q8_BLOCK_BYTES
            }
            let kd = kv_blk_scale(kmir[bb >> 2u], bb)
            let vd = kv_blk_scale(vmir[bb >> 2u], bb)
            let wq = (bb + 2u) >> 2u
            let dst = pos * pa.kvd + blk * KQ_B32_ELEMS_U
            var klo = kmir[wq]
            var vlo = vmir[wq]
            for [unroll] (wi in range(TQ4 ? 4 : 8)) {
                let khi = kmir[wq + uint(wi) + 1u]
                let vhi = vmir[wq + uint(wi) + 1u]
                let kw = kv_skew_word(klo, khi, bb + 2u)
                let vw = kv_skew_word(vlo, vhi, bb + 2u)
                klo = khi
                vlo = vhi
                for [unroll] (bk in range(4)) {
                    static_if (TQ4) {
                        let kby = (kw >> uint(bk * 8)) & 255u
                        let vby = (vw >> uint(bk * 8)) & 255u
                        let at = dst + uint(wi * 8 + bk * 2)
                        ksh[at] = shadow_half(kv_tq4_quant_lo(kby), kd)
                        ksh[at + 1u] = shadow_half(kv_tq4_quant_hi(kby), kd)
                        vsh[at] = shadow_half(kv_tq4_quant_lo(vby), vd)
                        vsh[at + 1u] = shadow_half(kv_tq4_quant_hi(vby), vd)
                    } else {
                        let at = dst + uint(wi * 4 + bk)
                        ksh[at] = shadow_half(kv_q8_quant(kw, bk), kd)
                        vsh[at] = shadow_half(kv_q8_quant(vw, bk), vd)
                    }
                }
            }
        }
    }
}

//! the residual step on the template every home stamps (`GkAddRmsRows`), the discrete homes' form: 256 lanes, the row stash a slab
[ |> template_struct_instance]
class template ArBase : GkAddRmsRows {
    @workgroup row : float[8192]

    def override stash(k : uint; v : float) : void {
        row[k] = v
    }
    def override stashed(k : uint) : float => row[k]
    def override pre_partner(base, k : uint) : float => row[k]

    //! the step over the workgroup's row, `row0` past it - the f16 stamps' rows start at row 0
    [spirv_inline]
    def ar_body {
        static_if (OUT_F16) {
            add_rms_row(gl_WorkGroupID.x)
        } else {
            add_rms_row(gl_WorkGroupID.x + pa.row0)
        }
    }
}

//! add_rms + Q8_0 requant in one dispatch, bit-identical to the pair
[ |> template_struct_instance]
class template ArRqBase : ArBase {
    override OUT_Q8 = true
    override NORM_SWITCH = true

    [arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#residual-step-fma"), arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def rq_row(rbase, qb0 : uint) {
        var ss = 0.0
        accum_row_into(rbase, ss)
        var inv = 0.0
        row_inv_into(ss, inv)
        quant32(qb0, inv)
    }

    def rq_b_body {
        //! the rows form a workgroup a row takes: workgroup `w` requants row `w`, whose Q8_0 blocks start at block `w * dim / 32`
        rq_row(gl_WorkGroupID.x * pa.dim, gl_WorkGroupID.x * (pa.dim / KQ_B32_ELEMS_U))
    }
}

[ |> template_struct_instance]
class template ArRqT : ArRqBase {
    @template_constant KBLK : bool = false    //!< the quant form: Q8_K superblocks (a scale per 256) instead of Q8_0's 32-blocks
    @template_constant STOREX : bool = false  //!< the normed row stored to `ystore` (binding 11) as floats beside the quants
    @ssbo @binding = 11 @template_gate = STOREX ystore : array<float>   //! the normed row beside the quants, for the consumers that read it as floats - past `outs`, so the set order stays quants, scales, row

    //! the Q8_K superblocks of the stashed row at inv: a 32-lane block a 256-block, eight values a lane - the block by the invocation id, as the requant class maps it, so the amax fold's masks stay inside it on any subgroup width
    [arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def quant_k(inv : float) {
        let nblk = pa.dim / 256u
        let lane = gl_LocalInvocationID.x & 31u
        var b = gl_LocalInvocationID.x >> 5u
        while (b < nblk) {
            sblk_store(b, lane, b * 256u + lane, inv)
            b += 8u   // the 256-thread workgroup's eight blocks
        }
    }

    [spirv_inline]
    def ar_rq_body {
        var ss = 0.0
        accum_row_into(0u, ss)
        var inv = 0.0
        row_inv_into(ss, inv)
        static_if (STOREX) {
            var k = gl_LocalInvocationID.x
            while (k < pa.dim) {
                ystore[k] = normed_at(k, 0.0, inv)
                k += 256u
            }
        }
        static_if (KBLK) {
            quant_k(inv)
        } else {
            quant32(0u, inv)
        }
    }
}

[ |> template_struct_instance]
class template ArCombBase : ArBase {
    @ssbo @binding = 4 slot_idx : array<uint>    //! position -> its k slot rows
    @ssbo @binding = 5 slot_w : array<float>     //! ... and their routing weights
    @ssbo @binding = 6 down_rows : array<float>  //! the routed down-GEMM output rows
    @ssbo @binding = 7 logits : array<float>     //! [npos x lstride]; the gate logit is the row's last
    @ssbo @binding = 8 @role = "weight" dbias : array<float>   //! the expert down biases, [ne x dim] a layer (gpt-oss), read under `dbias_on`
    @ssbo @binding = 9 slot_ex : array<uint>     //! slot -> its expert, the down bias's row (the decode's top-k picks; the window's per-slot picks)

    //! slot `s`'s down row at column k, its expert's bias added under `dbias_on`
    def drow(s, k : uint) : float {
        let d = down_rows[slot_idx[s] * pa.dim + k]
        return pa.dbias_on != 0u ? d + dbias[pa.dboff + slot_ex[s] * pa.dim + k] : d
    }

    //! the same row off registers: `rb` the slot's row base, `bb` its bias row base
    def drow_at(rb, bb, k : uint) : float => down_rows[rb + k] + dbias[bb + k]

    [arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#moe-combine-row")]
    def override accum_row_into(base : uint; var ss : float&) {
        let p = base / pa.dim
        var g = 1.0
        if (pa.gated != 0u) {
            g = sigmoid_f32(logits[p * pa.lstride + pa.lstride - 1u])
        }
        let pk = p * pa.slots
        let nreg = min(pa.slots, 8u)
        var rb, bb : uint[8]
        var sw : float[8]
        for (j in range(8)) {
            if (uint(j) < nreg) {
                rb[j] = slot_idx[pk + uint(j)] * pa.dim
                sw[j] = slot_w[pk + uint(j)]
                bb[j] = pa.dbias_on != 0u ? pa.dboff + slot_ex[pk + uint(j)] * pa.dim : 0u
            } else {
                rb[j] = 0u
                sw[j] = 0.0
                bb[j] = 0u
            }
        }
        let last = pa.dim - 1u
        ss = 0.0
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let k1 = min(k + 256u, last)   // the clamped columns past the row read a live column and store nothing
            let k2 = min(k + 512u, last)
            let k3 = min(k + 768u, last)
            var f0 = 0.0
            var f1 = 0.0
            var f2 = 0.0
            var f3 = 0.0
            if (pa.add_on != 0u) {
                f0 = g * aa[base + k]
                f1 = g * aa[base + k1]
                f2 = g * aa[base + k2]
                f3 = g * aa[base + k3]
            }
            for (j in range(8)) {
                if (uint(j) < nreg) {
                    f0 += sw[j] * drow_at(rb[j], bb[j], k)
                    f1 += sw[j] * drow_at(rb[j], bb[j], k1)
                    f2 += sw[j] * drow_at(rb[j], bb[j], k2)
                    f3 += sw[j] * drow_at(rb[j], bb[j], k3)
                }
            }
            var j = 8u
            while (j < pa.slots) {
                let w = slot_w[pk + j]
                f0 += w * drow(pk + j, k)
                f1 += w * drow(pk + j, k1)
                f2 += w * drow(pk + j, k2)
                f3 += w * drow(pk + j, k3)
                j++
            }
            let v0 = (xs[base + k] + f0) * pa.ascale
            let v1 = (xs[base + k1] + f1) * pa.ascale
            let v2 = (xs[base + k2] + f2) * pa.ascale
            let v3 = (xs[base + k3] + f3) * pa.ascale
            keep(base, k, v0, ss)
            if (k + 256u < pa.dim) {
                keep(base, k1, v1, ss)
            }
            if (k + 512u < pa.dim) {
                keep(base, k2, v2, ss)
            }
            if (k + 768u < pa.dim) {
                keep(base, k3, v3, ss)
            }
            k += 1024u
        }
    }
}

//! the gemma-4 MoE combine on the template both homes stamp (`GkMoeCombG4`): one workgroup a position, the routed slot rows off
//! the slot map, the next norm's row out in the stamp's form
[ |> template_struct_instance]
class template ArCombG4T : GkMoeCombG4 {
    @workgroup row : float[8192]
    override OUT_X4 = true   //! the residual step's store form (`GkQ8BlockStore.store_x4`): 10.37 us against the stride loop's 12.29 on the pod, `debug-jit` (ARCHITECTURE_GPU_SHARED_KERNELS.md's provenance line)

    def override stash(k : uint; v : float) : void {
        row[k] = v
    }
    def override stashed(k : uint) : float => row[k]

    [spirv_inline]
    def comb_g4_body {
        comb_g4_row(gl_WorkGroupID.x)
    }
}

struct RqArgs {
    inbase : uint   //! element base of the source rows
    nblk : uint     //! block / superblock count
    lo : float      //! with `hi`, the range a `CLAMP` stamp clamps each source element to
    hi : float
}

//! the Q8 requant of a plane's elements - Q8_0 32-blocks, or Q8_K superblocks under `KBLK` - over the stamp's element load (`load4` / `load1`)
[ |> template_struct_instance]
class template RqT : GkQ8BlockStore {
    override OUT_Q8 = true
    @template_constant KBLK : bool = false   //!< Q8_K superblocks instead of Q8_0's 32-blocks

    def abstract load1(e : uint) : float     //! element `e` (the superblock form)
    def load4(eb : uint) : float4 => float4(load1(eb), load1(eb + 1u), load1(eb + 2u), load1(eb + 3u))   //! the four elements from `eb` (the 32-block form)
    def override normed_at(e : uint; mean, inv : float) : float => load1(e)

    [spirv_inline]
    def rq_body {
        let gid = gl_GlobalInvocationID.x
        let lane = gid % 32u
        static_if (KBLK) {
            let b = gid / 32u
            if (b < pa.nblk) {
                sblk_store(b, lane, b * 256u + lane, 0.0)
            }
        } else {
            let sg = gid / 32u
            let b = sg * 4u + lane / 8u
            if (b < pa.nblk) {
                blk_store(b, lane % 8u, load4(sg * 128u + lane * 4u))
            }
        }
    }
}

//! the straight requant of a float plane from `inbase`, its elements clamped to `lo`..`hi` under `CLAMP`
[ |> template_struct_instance]
class template RqPlainT : RqT {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outq : array<int8>
    @ssbo @binding = 2 outs : array<float>
    @ssbo @binding = 3 @template_gate = OUT_SUM outsum : array<float>   //! the blocks' scale x quant sum
    @push_constant pa : RqArgs
    @template_constant CLAMP : bool = false

    def override load1(e : uint) : float {
        static_if (CLAMP) {
            return clamp(src[pa.inbase + e], pa.lo, pa.hi)
        } else {
            return src[pa.inbase + e]
        }
    }
}

//! the workgroups a requant or act+requant of `elems` floats dispatches: a Q8_K lane quantizes eight elements, a Q8_0 lane four
def rq_wgs(kq : bool; elems : int64) : int64 => (elems / (kq ? 8l : 4l) + 255l) / 256l

struct ActArgs {
    nelem : uint   //! element count, always a 32-multiple: the 4-wide guard is exact
    act : uint    //! the activation code: ACT_SILU / ACT_GELU / ACT_OAI
    nblk : uint    //! 32-block / superblock count (the requant kernels' guard)
    elem0 : uint   //! a 32-multiple; ActF16 only - the requant classes ignore it
    ubase : uint   //! the u side's element base; with `ustride` the rows of `ulen` elements sit `ustride` apart (a side input stored position-major); 0/0/0 = u is laid out like gate
    ustride : uint
    ulen : uint
    bias_on : uint   //! the biased twins: gate and up take their expert's bias rows before the activation (gpt-oss)
    gboff : uint     //! ... this layer's row base in the gate-bias plane and the up-bias plane ([ne x brow] rows a layer each)
    brow : uint      //! ... the row width: element e sits in row e / brow, whose expert `rex` names
}

//! the act + requant of a gate / up pair on `RqT`: the element load is the activation
[ |> template_struct_instance]
class template ActRqT : RqT {
    @ssbo @binding = 0 gate : array<float>     //! gate rows (act's v side)
    @ssbo @binding = 1 up : array<float>       //! up rows (act's u side)
    @ssbo @binding = 2 outq : array<int8>      //! quantized bytes out
    @ssbo @binding = 3 outs : array<float>     //! per-block scales out
    @ssbo @binding = 4 @template_gate = OUT_SUM outsum : array<float>   //! the blocks' scale x quant sum
    @push_constant pa : ActArgs

    def act_mul(v, u : float) : float => gk_act_mul(v, u, pa.act)

    //! the up element paired with gate element `eb`: with `ustride` the rows of `ulen` elements sit `ustride` apart
    def up_at(eb : uint) : uint => gk_up_at(eb, pa.ubase, pa.ustride, pa.ulen)   //! four elements never cross a row: ulen is a 4-multiple

    def override load4(eb : uint) : float4 {
        let ue = up_at(eb)
        return float4(act_mul(gate[eb], up[ue]), act_mul(gate[eb + 1u], up[ue + 1u]), act_mul(gate[eb + 2u], up[ue + 2u]), act_mul(gate[eb + 3u], up[ue + 3u]))
    }
    def override load1(e : uint) : float => act_mul(gate[e], up[up_at(e)])
}

//! the biased twin of the act + requant (gpt-oss's stacks): element e's row (e / brow) names the expert whose gate and up bias rows
//! join before the activation; the Q8_0 and Q8_K block forms are the stamps' `KBLK`
[ |> template_struct_instance]
class template ActRqBT : ActRqT {
    @ssbo @binding = 4 @role = "weight" gb : array<float>   //! the gate-bias plane
    @ssbo @binding = 5 @role = "weight" ub : array<float>   //! the up-bias plane
    @ssbo @binding = 6 rex : array<uint>     //! row -> its expert

    def biased_gate_up(e, ue : uint) : float2 {
        let ex = rex[e / pa.brow]
        let c = e % pa.brow
        return float2(gate[e] + gb[pa.gboff + ex * pa.brow + c], up[ue] + ub[pa.gboff + ex * pa.brow + c])
    }

    def override load4(eb : uint) : float4 {
        let ue = up_at(eb)
        let p0 = biased_gate_up(eb, ue)
        let p1 = biased_gate_up(eb + 1u, ue + 1u)
        let p2 = biased_gate_up(eb + 2u, ue + 2u)
        let p3 = biased_gate_up(eb + 3u, ue + 3u)
        return float4(act_mul(p0.x, p0.y), act_mul(p1.x, p1.y), act_mul(p2.x, p2.y), act_mul(p3.x, p3.y))
    }

    //! the superblock form's element load (the `KBLK` body reads elements one at a time)
    def override load1(e : uint) : float {
        let p = biased_gate_up(e, up_at(e))
        return act_mul(p.x, p.y)
    }
}

//! act straight to the f16 hidden plane (the cm2 route, no quant)
[ |> template_struct_instance]
class template ActF16T {
    @template_constant BIASED : bool = false   //!< the window's bucket rows: row r's expert in `rex` (the schedule's bucket row -> expert map), its gate and up bias rows joining before the activation
    @ssbo @binding = 0 gate : array<float>
    @ssbo @binding = 1 up : array<float>
    @ssbo @binding = 2 outh : array<float16>
    @ssbo @binding = 0 @role = "alias" gate4 : array<float4>   //! the planes as 16-byte words: a thread's four elements in one load and one store
    @ssbo @binding = 1 @role = "alias" up4 : array<float4>
    @ssbo @binding = 2 @role = "alias" outw2 : array<uint2>
    @ssbo @binding = 3 @role = "weight" @template_gate = BIASED gb : array<float>   //! the gate-bias plane
    @ssbo @binding = 4 @role = "weight" @template_gate = BIASED ub : array<float>   //! the up-bias plane
    @ssbo @binding = 5 @template_gate = BIASED rex : array<uint>     //! bucket row -> its expert
    @push_constant pa : ActArgs

    def act_mul(v, u : float) : float => gk_act_mul(v, u, pa.act)

    //! the up element paired with gate element `e`: with `ustride` the rows of `ulen` elements sit `ustride` apart (the per-layer-embedding side rows)
    def up_at(e : uint) : uint => gk_up_at(e, pa.ubase, pa.ustride, pa.ulen)

    //! element `e`'s activation as a saturated half
    def hval(e : uint) : float16 {
        static_if (BIASED) {
            let ex = rex[e / pa.brow]
            let c = e % pa.brow
            return half_sat(act_mul(gate[e] + gb[pa.gboff + ex * pa.brow + c], up[e] + ub[pa.gboff + ex * pa.brow + c]))
        } else {
            return half_sat(act_mul(gate[e], up[up_at(e)]))
        }
    }

    def act_scalar4(e : uint) {
        //! a thread's four elements by the element, where its words sit off the 16-byte grid or the up rows are strided
        outh[e] = hval(e)
        outh[e + 1u] = hval(e + 1u)
        outh[e + 2u] = hval(e + 2u)
        outh[e + 3u] = hval(e + 3u)
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#act-row-base")]
    def actf16_body {
        let e = pa.elem0 + gl_GlobalInvocationID.x * 4u
        if (e < pa.elem0 + pa.nelem) {
            static_if (BIASED) {
                act_scalar4(e)
            } else {
                if (pa.ustride == 0u && ((pa.elem0 | pa.ubase) & 3u) == 0u) {
                    let g = gate4[e / 4u]
                    let u = up4[(pa.ubase + e) / 4u]
                    let a0 = clamp(act_mul(g.x, u.x), -65504.0, 65504.0)
                    let a1 = clamp(act_mul(g.y, u.y), -65504.0, 65504.0)
                    let a2 = clamp(act_mul(g.z, u.z), -65504.0, 65504.0)
                    let a3 = clamp(act_mul(g.w, u.w), -65504.0, 65504.0)
                    outw2[e / 4u] = uint2(packHalf2x16(float2(a0, a1)), packHalf2x16(float2(a2, a3)))
                } else {
                    act_scalar4(e)
                }
            }
        }
    }
}

struct EmbArgs {
    npos : uint
    dim : uint
    wblk0 : uint   //! the embd plane's slab-local block base
    embed_scale : float
}

//! the token-table gather, four elements a thread: the q8 table dequantized then scaled (the CPU's order), or under `WF` an
//! F32/F16/BF16 table's exact f32 rows scaled; a row whose id is `EMB_ID_HOST` copies the host's staged row instead; `ids`,
//! `xout` and `xh` sit at 2, 3 and 4 on both stamps
[ |> template_struct_instance]
class template EmbGatherT {
    @template_constant WF : bool = false   //!< the f32 table instead of the q8 one
    @ssbo @binding = 0 @role = "weight" @template_gate = "!WF" wq : array<uint>
    @ssbo @binding = 1 @role = "weight" @template_gate = "!WF" wsh : array<float16>
    @ssbo @binding = 0 @role = "weight" @template_gate = WF wf : array<float>          //! the f32 table, vocab x dim rows
    @ssbo @binding = 2 ids : array<uint>       //! one token id per row
    @ssbo @binding = 3 xout : array<float>
    @ssbo @binding = 4 xh : array<float>       //! the host's staged rows, read for an `EMB_ID_HOST` row alone
    @push_constant pa : EmbArgs

    [spirv_inline, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
    def emb_body {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.npos * pa.dim) {
            let dim = max(pa.dim, 1u)
            let row = e / dim
            let col = e % dim
            let id = ids[row]
            if (id == EMB_ID_HOST) {
                xout[e] = xh[e]
                xout[e + 1u] = xh[e + 1u]
                xout[e + 2u] = xh[e + 2u]
                xout[e + 3u] = xh[e + 3u]
            } else {
                static_if (WF) {
                    let src = id * pa.dim + col
                    xout[e] = wf[src] * pa.embed_scale
                    xout[e + 1u] = wf[src + 1u] * pa.embed_scale
                    xout[e + 2u] = wf[src + 2u] * pa.embed_scale
                    xout[e + 3u] = wf[src + 3u] * pa.embed_scale
                } else {
                    let ib = pa.wblk0 + id * (pa.dim / 32u) + col / 32u
                    let lw = (col % 32u) / 4u
                    let q = float4(int4(unpack8(int(wq[ib * 8u + lw]))))
                    let d = float(wsh[ib])
                    let v = float4(mad(q.x, d, 0.0), mad(q.y, d, 0.0), mad(q.z, d, 0.0), mad(q.w, d, 0.0))   // the host embed's order, dequant then embed scale: a plain product chain lets the driver fold the scale into d first
                    xout[e] = v.x * pa.embed_scale
                    xout[e + 1u] = v.y * pa.embed_scale
                    xout[e + 2u] = v.z * pa.embed_scale
                    xout[e + 3u] = v.w * pa.embed_scale
                }
            }
        }
    }
}

//! the discrete homes' grid of the common `GkClsEpilogue` over `rows` rows of `vocab` logits: 256 invocations a workgroup
def cls_epi_wgs(vocab, rows : int64) : int64 => (vocab * rows + 255l) / 256l

let CLS_ARGMAX_CHUNKS = 64u   //! the chunks a row's pick splits into, compiled into both passes: a workgroup each over a 4096-wide slice of a 262144 vocab

struct ArgmaxArgs {
    vocab : uint
}

//! the pick's fold, shared by the two passes: a workgroup's first maximum over its threads' candidates (`wg_argmax_into`; every
//! thread returns the same answer)
[ |> template_struct_instance]
class template ClsArgmaxBase : GkWgArgmax {
    def lane_val(v : float) : float => sel_lane_val(v)

    def fold(m : float; mi : uint) : uint {
        var bm = 0.0
        var bi = 0u
        wg_argmax_into(m, mi, bm, bi)
        return bi
    }
}

//! the classifier tail's pick, the first pass: a workgroup a (row, chunk) over its slice of the row's logits, the slice's
//! first maximum and its id into the partial planes (`part[row * CLS_ARGMAX_CHUNKS + chunk]`); a thread's first element seeds its
//! candidate and every lane reads through `lane_val`, so a row of equal or non-finite values still lands an id, and an empty slice
//! lands none (`0xFFFFFFFF`)
[ |> template_struct_instance]
class template ClsArgmaxPartT : ClsArgmaxBase {
    @ssbo @binding = 0 y : array<float>        //! the logits rows
    @ssbo @binding = 1 part_max : array<float>
    @ssbo @binding = 2 part_id : array<uint>
    @push_constant pa : ArgmaxArgs

    [spirv_inline]
    def part_body {
        let wg = gl_WorkGroupID.x
        let row = wg / CLS_ARGMAX_CHUNKS
        let len = (pa.vocab + CLS_ARGMAX_CHUNKS - 1u) / CLS_ARGMAX_CHUNKS
        let c0 = (wg % CLS_ARGMAX_CHUNKS) * len
        let c1 = min(pa.vocab, c0 + len)
        let base = row * pa.vocab
        let tid = gl_LocalInvocationID.x
        var m = SEL_LANE_LOWEST
        var mi = 0xFFFFFFFFu
        var i = c0 + tid
        while (i < c1) {
            let v = lane_val(y[base + i])
            if (mi == 0xFFFFFFFFu || v > m) {
                m = v
                mi = i
            }
            i += 256u
        }
        let bi = fold(m, mi)
        if (tid == 0u) {
            part_max[wg] = bi == 0xFFFFFFFFu ? SEL_LANE_LOWEST : lane_val(y[base + bi])
            part_id[wg] = bi
        }
    }
}

//! ... the second pass: a workgroup a row over its `CLS_ARGMAX_CHUNKS` partials, the row's first maximum's id (the lowest id on a
//! tie, as the host's `parallel_argmax` reads, so a device pick and a host pick agree) into `pick`
[ |> template_struct_instance]
class template ClsArgmaxFinT : ClsArgmaxBase {
    @ssbo @binding = 0 part_max : array<float>
    @ssbo @binding = 1 part_id : array<uint>
    @ssbo @binding = 2 pick : array<uint>
    @push_constant pa : ArgmaxArgs

    [spirv_inline]
    def fin_body {
        let row = gl_WorkGroupID.x
        let tid = gl_LocalInvocationID.x
        var m = SEL_LANE_LOWEST
        var mi = 0xFFFFFFFFu
        var c = tid
        while (c < CLS_ARGMAX_CHUNKS) {
            let pm = part_max[row * CLS_ARGMAX_CHUNKS + c]
            let pi = part_id[row * CLS_ARGMAX_CHUNKS + c]
            if (pm > m || (pm == m && pi < mi)) {
                m = pm
                mi = pi
            }
            c += 256u
        }
        let bi = fold(m, mi)
        if (tid == 0u) {
            pick[row] = bi
        }
    }
}

struct DaAttnArgs {
    kvd : uint      //! kv row width
    hs : uint       //! head size
    kv_mul : uint   //! GQA group (query heads per kv head)
    kbase : uint    //! layer base into the shared K mirror (0 for the standalone seam)
    vbase : uint
    scale : float   //! the attended KV count rides tok[0].cnt, not these args
    qstride : uint  //! q head stride: 0 = hs; 2*hs on a gated-q model ([q | gate])
    gated : uint    //! 1 = scale the head's output by its gate half's sigmoid
    window : uint   //! a sliding layer attends the last `window` positions of the cnt; 0 = every position
    cap : float     //! the attention logit softcap on the scaled scores (gemma-2: 50); 0 = none
    nsplit : uint   //! the key range split across this many workgroups a group, each leaving a (max, denom, acc) piece a head that the group's last piece combines; 0 or 1 = one workgroup a group stores the rows itself
    rqk : uint      //! the row's requant folded into the store: 0 = Q8_0 blocks of 32, 1 = Q8_K superblocks of 256 (a head a multiple of 256), 2 = none (the row alone)
    sinkoff : uint  //! the SINK stamps: this layer's row base in the sink plane ([n_heads] a layer)
    rowwg : uint    //! (kv head, slab, split) workgroups a row - `da_attn_row_wgs`: the dispatch past them is the next row, which reads its own `tok` entry; 0 = the whole dispatch is row 0 (the one-row command passes its row's count: the same reading)
    qrow : uint     //! a row's stride in q (the projection row's width)
    cntoff : uint   //! the split form's arrival counters, a word a (row, kv head, slab) group, at this word offset of the partials binding: zero between dispatches
    ring : uint     //! a sliding layer's ring rows: key `c` reads its `kv_ring_row` slot; 0 = the flat slab
}

//! a (head, split) partial's floats in the partials plane: the running max, the denom, then the hs accumulators
def da_attn_part_stride(hs : uint) : uint => hs + 2u

//! gemma-2's attention logit softcap on a scaled score; 0 = none
def attn_softcap(s, cap : float) : float => cap != 0.0 ? softcap_exp(s, cap) : s

let DA_G = 4u   //! the q heads a decode-attention workgroup serves off one K/V read: a GQA group's, in slabs of four

[arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split")]
def da_slab_is_g2(kv_mul : int64) : bool => kv_mul <= 2l

//! the slab's q heads at a group width: the kernel's `G` and the host's workgroup count derive from this one expression
def da_slab_heads(kv_mul : int64) : int64 => da_slab_is_g2(kv_mul) ? 2l : int64(DA_G)

//! the decode attention's workgroups a row: one a (kv head, slab of the group's q heads, key split)
def da_attn_row_wgs(n_heads, kv_mul, nsplit : int64) : int64 {
    let g = da_slab_heads(kv_mul)
    return n_heads / kv_mul * ((kv_mul + g - 1l) / g) * nsplit
}

let DA_RQ_Q8 = 0u    //! `DaAttnArgs.rqk`: the wo plane reads Q8_0 activations

let DA_RQ_Q8K = 1u   //! ... Q8_K activations

let DA_RQ_NONE = 2u  //! ... no fold: a separate requant serves (or nothing reads the quants)

//! none under tq4 (the requant follows the un-rotation); Q8_0 for a Q8_0 `fo`; Q8_K for a K-quant `fo` on whole superblocks
def da_rqk_pick(tq4 : bool; fo : int; hs : int64) : uint {
    return DA_RQ_NONE if (tq4)
    return DA_RQ_Q8 if (!kq_sb(fo))
    return hs % 256l == 0l ? DA_RQ_Q8K : DA_RQ_NONE
}

[ |> template_struct_instance]
class template DaAttnT {
    @template_constant KV16 : bool = false   //!< the f16-mirror stamp: a lane's eight halves of a K row load as one 16-byte word
    @template_constant KVQ8 : bool = false   //!< the q8_0-mirror stamp: the mirrors are 34-byte blocks read as words, `kbase` / `vbase` and the token's region in blocks
    @template_constant KVTQ4 : bool = false  //!< the tq4-mirror stamp: 18-byte blocks of 4-bit quants in the rotated basis - the query arrives rotated, the rows leave rotated
    @template_constant KVBLK : bool = false  //!< a block-codec stamp (q8_0 or tq4): the slab's query rows quantize to q8_0 blocks in the workgroup and every K score is the packed integer dot
    @template_constant SINK : bool = false   //! the sink stamps (gpt-oss): the head's sink logit joins the softmax's max and denominator at the unsplit store; a split piece leaves it to the combine
    @template_constant G : int = 4           //!< the slab's q heads, DA_G or two (`da_slab_heads`): every score and V loop unrolls over them; the workgroup arrays hold four, so no stamp goes wider
    @ssbo @binding = 0 q : array<float>      //! roped query rows, one per head
    @ssbo @binding = 1 kmir : array<KT>      //! the K mirror plane
    @ssbo @binding = 1 @role = "alias" @template_gate = KV16 kw4 : array<uint4>   //! the K mirror as 16-byte words
    @ssbo @binding = 2 vmir : array<KT>      //! the V mirror plane
    @ssbo @binding = 2 @role = "alias" @template_gate = KV16 vw4 : array<uint4>   //! the V mirror as 16-byte words
    @ssbo @binding = 3 y : array<float>      //! attention out rows
    @ssbo @binding = 4 tok : array<TokMeta>  //! the shared per-token block (element 0)
    @ssbo @binding = 5 @coherent part : array<float>   //! the split form's partials [head x split x (hs + 2)], the arrival counters past them; unsplit, any buffer (unread); coherent: the last-arriving piece reads the others' stores
    @ssbo @binding = 5 @role = "alias" @coherent partu : array<uint>   //! the partials binding as words: the arrival counters
    @ssbo @binding = 6 aq : array<int8>      //! the row's quants when `rqk` folds the requant (the wo plane's feed); else any buffer (unwritten)
    @ssbo @binding = 7 aqs : array<float>    //! ... and its block scales
    @ssbo @binding = 8 @role = "weight" @template_gate = SINK sinks : array<float>   //! the SINK stamps: the per-head sink logits, `pa.sinkoff` the layer's row
    @push_constant pa : DaAttnArgs
    @workgroup qrow : float[2048]  //! the slab's query rows, DA_G x 512 (hs <= 512); a dead head's row is zero

    //! four of a query row's quants - the elements `base + k * stride`, k < 4 - packed low byte first into the word `w`, their
    //! sum added to `qs` (the tq4 score's -8 offset reads it)
    def q8_pack4(base, stride : uint; id : float; var w : uint&; var qs : float&) {
        w = 0u
        for [unroll] (k in range(4)) {
            let qi = int(round(qrow[base + uint(k) * stride] * id))
            qs += float(qi)
            w = w | ((uint(qi) & 255u) << uint(k * 8))
        }
    }
    @workgroup @template_gate = KVBLK qq : uint[512]   //! the slab's query rows as packed int8 words, DA_G x (hs / 4); under tq4 the even elements' words then the odd's, DA_G x (hs / 8) each
    @workgroup @template_gate = KVBLK qsc : float[64]  //! ... and their q8_0 block scales, DA_G x (hs / 32)
    @workgroup @template_gate = KVTQ4 qsum : float[64] //! tq4: a block's sum of the query's quants, the nibble offset's term, DA_G x (hs / 32)
    @workgroup sc : float[1024]    //! the slab's chunk scores, DA_G x 256
    @workgroup sgp : float[128]    //! per-subgroup partials, DA_G x 32: a chunk's maxes in a head's lower half, its sums in the upper
    @workgroup ml : float[16]      //! a head's [running max, running softmax denom, -, -], DA_G quads
    @workgroup arrived : uint[1]   //! the split form: the pieces of this group that finished before this one

    //! one V mirror element as f32
    def vld(i : uint) : float => float(vmir[i])   // nolint:PERF020,LINT019 -- KT convert-on-load; a no-op only in the f32 stamp, which fires in the stamping module's compile alone

    //! the unsplit store's normalization: 1 / L, or on the sink stamps the phantom key folded into the max and the denominator once
    def unsplit_linv(h : uint; mrun, l : float) : float {
        static_if (SINK) {
            let sk = sinks[pa.sinkoff + h]
            let msk = max(mrun, sk)
            let os = exp(mrun - msk)
            return os / (l * os + exp(sk - msk))
        } else {
            return 1.0 / l
        }
    }

    //! eight halves of a K row word against the q row's slice at `qo`
    def kdot8(w : uint4; qo : uint) : float {
        let a = unpackHalf2x16(w.x)
        let b = unpackHalf2x16(w.y)
        let c = unpackHalf2x16(w.z)
        let d = unpackHalf2x16(w.w)
        return ((qrow[qo] * a.x + qrow[qo + 1u] * a.y) + (qrow[qo + 2u] * b.x + qrow[qo + 3u] * b.y)) + ((qrow[qo + 4u] * c.x + qrow[qo + 5u] * c.y) + (qrow[qo + 6u] * d.x + qrow[qo + 7u] * d.y))
    }

    //! the finished row (o at dim `d` = tid, o2 at tid + 256 on a wide head) stored, with the push's requant fold
    def store_row(h, hs, d : uint; o, o2 : float; wide : bool) {
        let live = d < hs
        if (live) {
            y[h * hs + d] = o
        }
        if (wide) {
            y[h * hs + d + 256u] = o2
        }
        if (pa.rqk == DA_RQ_Q8) {
            if (live) {   // a 32-aligned head keeps a block's lanes live together
                let s = q8_lane_scale(o)
                if ((d & 31u) == 0u) {
                    aqs[(h * hs + d) >> 5u] = s.x
                }
                aq[h * hs + d] = q8_quant1(o, s.y)
            }
            if (wide) {
                let s2 = q8_lane_scale(o2)
                if ((d & 31u) == 0u) {
                    aqs[(h * hs + d + 256u) >> 5u] = s2.x
                }
                aq[h * hs + d + 256u] = q8_quant1(o2, s2.y)
            }
        } elif (pa.rqk == DA_RQ_Q8K) {   // the whole workgroup takes part: the head is 256 or 512 wide
            let m1 = subgroupMax(abs(o))
            let m2 = subgroupMax(wide ? abs(o2) : 0.0)
            if (gl_SubgroupInvocationID == 0u) {
                sgp[gl_SubgroupID] = m1
                sgp[32u + gl_SubgroupID] = m2
            }
            barrier()
            var mx1 = 0.0
            var mx2 = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                mx1 = max(mx1, sgp[sgi])
                mx2 = max(mx2, sgp[32u + sgi])
                sgi++
            }
            let d1 = mx1 / 127.0
            let id1 = d1 != 0.0 ? 1.0 / d1 : 0.0
            let b1 = (h * hs) >> 8u
            if (d == 0u) {
                aqs[b1] = d1
            }
            aq[h * hs + d] = q8_quant1(o, id1)
            if (wide) {
                let d2 = mx2 / 127.0
                let id2 = d2 != 0.0 ? 1.0 / d2 : 0.0
                if (d == 0u) {
                    aqs[b1 + 1u] = d2
                }
                aq[h * hs + d + 256u] = q8_quant1(o2, id2)
            }
            barrier()   // the slab's next head reuses sgp
        }
    }

    //! the aligned max's seed: the head's sink logit on the sink stamp (the phantom key), the floor elsewhere
    def seed_m(h : uint) : float {
        static_if (SINK) {
            return sinks[pa.sinkoff + h]
        } else {
            return -3.0e38
        }
    }

    //! ... and the aligned denominator's: the phantom key's mass against the aligned max, once for the head
    def seed_l(h : uint; m : float) : float {
        static_if (SINK) {
            return exp(sinks[pa.sinkoff + h] - m)
        } else {
            return 0.0
        }
    }

    //! the split form's end, run by the group's last piece: a head's pieces aligned by their maxes, normalized, then the row
    def combine_head(h, hflat, hs, nsplit, stride, qb, qstride, tid : uint; wide : bool) {
        let hb = hflat * nsplit * stride
        var m = seed_m(h)
        var sp = 0u
        while (sp < nsplit) {
            m = max(m, part[hb + sp * stride])
            sp++
        }
        var l = seed_l(h, m)
        var o = 0.0
        var o2 = 0.0
        sp = 0u
        while (sp < nsplit) {
            let pb = hb + sp * stride
            let wgt = exp(part[pb] - m)
            l += part[pb + 1u] * wgt
            if (tid < hs) {
                o += part[pb + 2u + tid] * wgt
            }
            if (wide) {
                o2 += part[pb + 2u + tid + 256u] * wgt
            }
            sp++
        }
        let linv = 1.0 / l
        finish_head(h, hflat, hs, qb, qstride, tid, o * linv, o2 * linv, wide)
    }

    //! one head's finished value (`o` at dim tid, `o2` at tid + 256 on a wide head) gated and stored, the unsplit form's end
    def finish_head(h, hflat, hs, qb, qstride, tid : uint; o, o2 : float; wide : bool) {
        var og = o
        var og2 = o2
        if (pa.gated != 0u && (wide || tid < hs)) {
            og = sig_gate(og, q[qb + h * qstride + hs + tid])
            if (wide) {
                og2 = sig_gate(og2, q[qb + h * qstride + hs + tid + 256u])
            }
        }
        store_row(hflat, hs, tid, og, og2, wide)
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs")]
    def da_attn_body {   // nolint:STYLE037,STYLE038 — the online-softmax pass over a slab of heads, one shape for every head width and form
        let hs = pa.hs
        let nsplit = max(pa.nsplit, 1u)
        let rowwg = max(pa.rowwg, 1u)
        let row = pa.rowwg == 0u ? 0u : gl_WorkGroupID.x / rowwg
        let wg = pa.rowwg == 0u ? gl_WorkGroupID.x : gl_WorkGroupID.x % rowwg
        let cnt = tok[row].cnt
        let sp = wg % nsplit
        let kvslab = wg / nsplit
        let nslab = (pa.kv_mul + uint(G) - 1u) / uint(G)
        let kvh = kvslab / nslab
        let h0 = kvh * pa.kv_mul + (kvslab % nslab) * uint(G)
        let live = min(uint(G), pa.kv_mul - (kvslab % nslab) * uint(G))   // a group's last slab may be short
        let hflat0 = row * (pa.kvd / hs * pa.kv_mul) + h0   // the slab's first head in the attention-out and partial planes, whose row stride is the heads' width
        let qb = row * pa.qrow
        let kbase = pa.kbase + tok[row].mirbase
        let vbase = pa.vbase + tok[row].mirbase
        let tid = gl_LocalInvocationID.x
        let wide = hs > 256u
        let qstride = pa.qstride == 0u ? hs : pa.qstride
        let lane = gl_SubgroupInvocationID
        var i = tid
        while (i < hs) {
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                qrow[hj * 512u + i] = hj < live ? q[qb + (h0 + hj) * qstride + i] : 0.0
            }
            i += 256u
        }
        if (tid < uint(G)) {
            ml[tid * 4u] = -3.0e38
            ml[tid * 4u + 1u] = 0.0
        }
        barrier()
        static_if (KVBLK) {
            let hj = tid / 16u   // slots 16 apart: the 512 head's block count, the arrays' stride
            let b = tid % 16u
            if (hj < uint(G) && b < hs / 32u) {
                let qo = hj * 512u + b * 32u
                var amax = 0.0
                for [unroll] (e in range(32)) {
                    amax = max(amax, abs(qrow[qo + uint(e)]))
                }
                let d = amax / 127.0
                let id = d != 0.0 ? 1.0 / d : 0.0
                qsc[hj * 16u + b] = d
                static_if (KVTQ4) {
                    var qs = 0.0
                    for [unroll] (wi in range(4)) {   // a tq4 byte: low nibble element 2k, high 2k + 1
                        var we = 0u
                        var wo = 0u
                        q8_pack4(qo + uint(wi * 8), 2u, id, we, qs)
                        q8_pack4(qo + uint(wi * 8) + 1u, 2u, id, wo, qs)
                        qq[hj * 64u + b * 4u + uint(wi)] = we
                        qq[256u + hj * 64u + b * 4u + uint(wi)] = wo
                    }
                    qsum[hj * 16u + b] = qs
                } else {
                    var qs = 0.0
                    for [unroll] (wi in range(8)) {
                        var wv = 0u
                        q8_pack4(qo + uint(wi * 4), 1u, id, wv, qs)
                        qq[hj * 128u + b * 8u + uint(wi)] = wv
                    }
                }
            }
            barrier()
        }
        let vrow_words = hs / 8u
        let kgroups = 256u / vrow_words
        let vw_pow2 = (vrow_words & (vrow_words - 1u)) == 0u
        let kgroups_per_subgroup = vw_pow2 ? gl_SubgroupSize / vrow_words : 0u   // whole key groups a subgroup: the fold pairs lanes of one dim slice
        let folded = kgroups_per_subgroup >= 1u
        let my_kgroup = folded ? gl_SubgroupID * kgroups_per_subgroup + lane / vrow_words : tid / vrow_words   // off the subgroup's own lanes where the fold serves
        let w = folded ? lane % vrow_words : tid % vrow_words
        var acc : float[32]                    // [head * 8 + dim]
        for [unroll] (j in range(G * 8)) {
            acc[j] = 0.0
        }
        let cstart = pa.window != 0u && cnt > pa.window ? cnt - pa.window : 0u
        let per = ((cnt - cstart + nsplit - 1u) / nsplit + 31u) & ~31u
        var c0 = min(cnt, cstart + sp * per)
        let cend = min(cnt, cstart + (sp + 1u) * per)
        let vec = KV16 && (kbase & 7u) == 0u && (vbase & 7u) == 0u && (pa.kvd & 7u) == 0u   // the f16 stamp with every mirror row on a 16-byte word: rows load as words
        let vb = vbase + kvh * hs + w * 8u
        while (c0 < cend) {
            let clen = min(256u, cend - c0)
            let krow = kbase + kv_ring_row(c0 + min(tid, clen - 1u), pa.ring) * pa.kvd + kvh * hs
            var kw : uint4[8]   // nolint:LINT002,LINT019 -- the f32 stamp's static_if leaves the word batches unused; the rule fires in that stamp's compile alone
            var vw : uint4[8]   // nolint:LINT002,LINT019 -- the f32 stamp's static_if leaves the word batches unused; the rule fires in that stamp's compile alone
            static_if (KV16) {
                if (vec) {
                    for [unroll] (b in range(8)) {
                        kw[b] = kw4[krow / 8u + min(uint(b), vrow_words - 1u)]   // a head of 64 fills eight words, a wider one takes more batches
                        vw[b] = vw4[(vb + kv_ring_row(c0 + min(my_kgroup + uint(b) * kgroups, clen - 1u), pa.ring) * pa.kvd) / 8u]
                    }
                }
            }
            if (tid < clen) {
                var pv : float[4]
                for [unroll] (j in range(G)) {
                    pv[j] = 0.0
                }
                static_if (KV16) {
                    if (vec) {
                        var wi = 0u
                        while (wi < vrow_words) {
                            if (wi != 0u) {
                                for [unroll] (b in range(8)) {
                                    kw[b] = kw4[krow / 8u + min(wi + uint(b), vrow_words - 1u)]
                                }
                            }
                            for [unroll] (b in range(8)) {
                                let wb = wi + uint(b)
                                if (wb < vrow_words) {
                                    for [unroll] (j in range(G)) {
                                        pv[j] += kdot8(kw[b], uint(j) * 512u + wb * 8u)
                                    }
                                }
                            }
                            wi += 8u
                        }
                    }
                }
                static_if (KVQ8 || KVTQ4) {
                    let kblk = kbase + kv_ring_row(c0 + min(tid, clen - 1u), pa.ring) * (pa.kvd / KQ_B32_ELEMS_U) + kvh * (hs / KQ_B32_ELEMS_U)
                    var bi = 0u
                    while (bi < hs / KQ_B32_ELEMS_U) {
                        let bb = (kblk + bi) * (KVQ8 ? KV_Q8_BLOCK_BYTES : KV_TQ4_BLOCK_BYTES)
                        let kd = kv_blk_scale(kmir[bb >> 2u], bb)
                        let wq = (bb + 2u) >> 2u
                        var bs : int[4]
                        for [unroll] (j in range(G)) {
                            bs[j] = 0
                        }
                        var lo = kmir[wq]
                        for [unroll] (wi in range(KVQ8 ? 8 : 4)) {
                            let hi = kmir[wq + uint(wi) + 1u]
                            let word = kv_skew_word(lo, hi, bb + 2u)
                            lo = hi
                            static_if (KVQ8) {
                                for [unroll] (j in range(G)) {
                                    bs[j] += sdot4(word, qq[uint(j) * 128u + bi * 8u + uint(wi)])
                                }
                            } else {
                                let lo4 = word & 0x0F0F0F0Fu
                                let hi4 = (word >> 4u) & 0x0F0F0F0Fu
                                for [unroll] (j in range(G)) {
                                    bs[j] += sdot4(lo4, qq[uint(j) * 64u + bi * 4u + uint(wi)]) + sdot4(hi4, qq[256u + uint(j) * 64u + bi * 4u + uint(wi)])
                                }
                            }
                        }
                        for [unroll] (j in range(G)) {
                            let qbi = uint(j) * 16u + bi
                            static_if (KVTQ4) {
                                pv[j] += (float(bs[j]) - 8.0 * qsum[qbi]) * kd * qsc[qbi]
                            } else {
                                pv[j] += float(bs[j]) * kd * qsc[qbi]
                            }
                        }
                        bi++
                    }
                } else {
                    if (!vec) {
                        var di = 0u
                        while (di < hs) {
                            let kv0 = float(kmir[krow + di])   // nolint:PERF020,LINT019 -- KT convert-on-load; a no-op only in the f32 stamp, which fires in the stamping module's compile alone
                            for [unroll] (j in range(G)) {
                                pv[j] = mad(qrow[uint(j) * 512u + di], kv0, pv[j])
                            }
                            di++
                        }
                    }
                }
                for [unroll] (j in range(G)) {
                    sc[uint(j) * 256u + tid] = attn_softcap(pv[j] * pa.scale, pa.cap)
                }
            }
            barrier()
            var s : float[4]
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                s[j] = tid < clen && hj < live ? sc[hj * 256u + tid] : -3.0e38
                let tmax = subgroupMax(s[j])
                if (lane == 0u) {
                    sgp[hj * 32u + gl_SubgroupID] = tmax
                }
            }
            barrier()
            var alpha : float[4]
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                var mnew = ml[hj * 4u]
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    mnew = max(mnew, sgp[hj * 32u + sgi])
                    sgi++
                }
                alpha[j] = exp(ml[hj * 4u] - mnew)
                let e = tid < clen && hj < live ? exp(s[j] - mnew) : 0.0
                sc[hj * 256u + tid] = e
                let tsum = subgroupAdd(e)
                if (lane == 0u) {
                    sgp[hj * 32u + 16u + gl_SubgroupID] = tsum
                }
            }
            barrier()
            if (tid < uint(G)) {   // a thread a head carries the running max and denominator forward
                var mnew = ml[tid * 4u]
                var lsum = 0.0
                var sgi = 0u
                while (sgi < gl_NumSubgroups) {
                    mnew = max(mnew, sgp[tid * 32u + sgi])
                    lsum += sgp[tid * 32u + 16u + sgi]
                    sgi++
                }
                ml[tid * 4u + 1u] = ml[tid * 4u + 1u] * exp(ml[tid * 4u] - mnew) + lsum
                ml[tid * 4u] = mnew
            }
            for [unroll] (j in range(G)) {
                for [unroll] (e in range(8)) {
                    acc[j * 8 + e] *= alpha[j]
                }
            }
            var jj = my_kgroup < kgroups ? my_kgroup : clen
            while (jj < clen) {
                static_if (KV16) {
                    if (vec) {
                        if (jj != my_kgroup) {
                            for [unroll] (b in range(8)) {
                                vw[b] = vw4[(vb + kv_ring_row(c0 + min(jj + uint(b) * kgroups, clen - 1u), pa.ring) * pa.kvd) / 8u]
                            }
                        }
                        for [unroll] (b in range(8)) {
                            let kb = jj + uint(b) * kgroups
                            if (kb < clen) {
                                let a = unpackHalf2x16(vw[b].x)
                                let bb = unpackHalf2x16(vw[b].y)
                                let cc = unpackHalf2x16(vw[b].z)
                                let d = unpackHalf2x16(vw[b].w)
                                for [unroll] (j in range(G)) {
                                    let sj = sc[uint(j) * 256u + kb]
                                    acc[j * 8] = mad(sj, a.x, acc[j * 8])
                                    acc[j * 8 + 1] = mad(sj, a.y, acc[j * 8 + 1])
                                    acc[j * 8 + 2] = mad(sj, bb.x, acc[j * 8 + 2])
                                    acc[j * 8 + 3] = mad(sj, bb.y, acc[j * 8 + 3])
                                    acc[j * 8 + 4] = mad(sj, cc.x, acc[j * 8 + 4])
                                    acc[j * 8 + 5] = mad(sj, cc.y, acc[j * 8 + 5])
                                    acc[j * 8 + 6] = mad(sj, d.x, acc[j * 8 + 6])
                                    acc[j * 8 + 7] = mad(sj, d.y, acc[j * 8 + 7])
                                }
                            }
                        }
                    }
                }
                static_if (KVQ8 || KVTQ4) {
                    let vbb = (vbase + kv_ring_row(c0 + jj, pa.ring) * (pa.kvd / KQ_B32_ELEMS_U) + kvh * (hs / KQ_B32_ELEMS_U) + w / 4u) * (KVQ8 ? KV_Q8_BLOCK_BYTES : KV_TQ4_BLOCK_BYTES)
                    let vd = kv_blk_scale(vmir[vbb >> 2u], vbb)
                    let vq0 = vbb + 2u + (w & 3u) * (KVQ8 ? 8u : 4u)
                    let v0 = vmir[vq0 >> 2u]
                    let v1 = vmir[(vq0 >> 2u) + 1u]
                    static_if (KVQ8) {
                        let v2 = vmir[(vq0 >> 2u) + 2u]
                        let wlo = kv_skew_word(v0, v1, vq0)
                        let whi = kv_skew_word(v1, v2, vq0)
                        for [unroll] (j in range(G)) {
                            let sj = sc[uint(j) * 256u + jj] * vd
                            for [unroll] (bk in range(4)) {
                                acc[j * 8 + bk] += sj * kv_q8_quant(wlo, bk)
                                acc[j * 8 + 4 + bk] += sj * kv_q8_quant(whi, bk)
                            }
                        }
                    } else {
                        let vword = kv_skew_word(v0, v1, vq0)
                        for [unroll] (j in range(G)) {
                            let sj = sc[uint(j) * 256u + jj] * vd
                            for [unroll] (bk in range(4)) {
                                let by = (vword >> uint(bk * 8)) & 255u
                                acc[j * 8 + bk * 2] += sj * kv_tq4_quant_lo(by)
                                acc[j * 8 + bk * 2 + 1] += sj * kv_tq4_quant_hi(by)
                            }
                        }
                    }
                } else {
                    if (!vec) {
                        let vrow = vb + kv_ring_row(c0 + jj, pa.ring) * pa.kvd
                        for [unroll] (e in range(8)) {
                            let v = vld(vrow + uint(e))
                            for [unroll] (j in range(G)) {
                                acc[j * 8 + e] = mad(sc[uint(j) * 256u + jj], v, acc[j * 8 + e])
                            }
                        }
                    }
                }
                jj += vec ? 8u * kgroups : kgroups
            }
            barrier()   // sc/sgp/ml settle before the next chunk overwrites them
            c0 += 256u
        }
        let stride = da_attn_part_stride(hs)
        var r = 1u
        while (r < kgroups_per_subgroup) {
            for [unroll] (k in range(G * 8)) {
                acc[k] += subgroupShuffleXor(acc[k], r * vrow_words)
            }
            r *= 2u
        }
        let ngroups_folded = folded ? gl_NumSubgroups : kgroups
        let nsl_hs = ngroups_folded * hs
        let round_cap = min(2048u / nsl_hs, 256u / hs)   // a round's heads: what the slab holds, and what a thread a dim can reduce
        let heads_per_round = max(1u, round_cap)
        let has_sum = folded ? lane < vrow_words : my_kgroup < kgroups
        let slot = folded ? gl_SubgroupID : my_kgroup
        var hr0 = 0u
        while (hr0 < live) {
            for [unroll] (j in range(G)) {
                let hj = uint(j)
                if (hj >= hr0 && hj < hr0 + heads_per_round && hj < live && has_sum) {
                    for [unroll] (e in range(8)) {
                        qrow[(hj - hr0) * nsl_hs + slot * hs + w * 8u + uint(e)] = acc[j * 8 + e]
                    }
                }
            }
            barrier()
            let hr = wide ? 0u : tid / hs
            let dd = wide ? tid : tid % hs
            if (hr < heads_per_round && hr0 + hr < live) {
                let h = hr0 + hr
                var tot = 0.0
                var tot2 = 0.0
                var sl = 0u
                while (sl < ngroups_folded) {
                    tot += qrow[hr * nsl_hs + sl * hs + dd]
                    if (wide) {
                        tot2 += qrow[hr * nsl_hs + sl * hs + dd + 256u]
                    }
                    sl++
                }
                let mrun = ml[h * 4u]
                let l = ml[h * 4u + 1u]
                let pb = ((hflat0 + h) * nsplit + sp) * stride
                if (nsplit > 1u) {
                    if (dd == 0u) {
                        part[pb] = mrun
                        part[pb + 1u] = l
                    }
                    part[pb + 2u + dd] = tot
                    if (wide) {
                        part[pb + 2u + dd + 256u] = tot2
                    }
                } else {
                    let linv = unsplit_linv(h0 + h, mrun, l)
                    finish_head(h0 + h, hflat0 + h, hs, qb, qstride, dd, tot * linv, tot2 * linv, wide)
                }
            }
            barrier()   // the slab settles before the next round
            hr0 += heads_per_round
        }
        if (nsplit > 1u) {
            let rowgroups = pa.rowwg == 0u ? gl_NumWorkGroups.x / nsplit : pa.rowwg / nsplit
            let ci = pa.cntoff + row * rowgroups + kvslab
            memoryBarrierBuffer()
            barrier()
            if (tid == 0u) {
                arrived[0] = atomicAdd(partu[ci], 1u)
            }
            barrier()
            if (arrived[0] + 1u == nsplit) {
                memoryBarrierBuffer()
                for [unroll] (j in range(G)) {
                    let hj = uint(j)
                    if (hj < live) {
                        combine_head(h0 + hj, hflat0 + hj, hs, nsplit, stride, qb, qstride, tid, wide)
                    }
                }
                if (tid == 0u) {
                    partu[ci] = 0u
                }
            }
        }
    }
}

//! the K/V mirror stores the window's rope-store stamps share: the K plane at binding 2, the V plane at 3, KT the mirror element type;
//! the f16 stamps clamp +-65504 (the metal CLAMP16 contract - an inf in the mirror poisons attention)
[ |> template_struct_instance]
class template KvMirStoreT {
    @ssbo @binding = 2 kmir : array<KT>        //! the K mirror plane
    @ssbo @binding = 3 vmir : array<KT>        //! the V mirror plane
    @template_constant CLAMP16 : bool = true

    //! one K mirror element, clamped into f16 on the f16 stamp
    def st_k(i : uint; v : float) {
        static_if (CLAMP16) {
            kmir[i] = half_sat(v)
        } else {
            kmir[i] = v
        }
    }

    //! ... and one V mirror element
    def st_v(i : uint; v : float) {
        static_if (CLAMP16) {
            vmir[i] = half_sat(v)
        } else {
            vmir[i] = v
        }
    }
}

//! the fused rope and K/V store of the token command (one or N rows), on the template both homes stamp (`GkRopeKvStore`): the q
//! rows in `qsrc`, the [q | k | v] projection rows in `ksrc` and `vsrc` (one buffer), the cos and sin rows one plane; a row's
//! mirror element base is the layer's plus its token block's
[ |> template_struct_instance]
class template RopeKvStoreT : GkRopeKvStore {
    @ssbo @binding = 8 tok : array<TokMeta>    //! the per-row token block: the row's mirror region base and position

    [spirv_inline]
    def rope_kv_body {
        let gid = gl_GlobalInvocationID.x
        let np = max(1u, pa.npairs)
        if (gid < np * max(pa.rows, 1u)) {
            let row = gid / np
            let mir = pa.mirbase + tok[row].mirbase + kv_ring_row(tok[row].pos, pa.ring) * pa.kvd
            rope_kv_pair(row, gid % np, mir, mir)
        }
    }
}

struct RopeKvBArgs {
    qd : uint
    kvd : uint
    hs : uint
    half : uint      //! rotated pairs a head; the rest pass through, k and v unrotated
    neox : uint
    kpair0 : uint    //! first k pair's per-position index
    ppp : uint       //! pairs per position (hs / 2 per head, rotated or not)
    layerbase : uint //! this layer's K/V mirror base
    npos : uint
    pos0 : uint      //! the segment's first position (window row base under a codec)
    voff : uint
    kstride : uint
    qhs : uint       //! per-head q stride: hs, or 2 * hs on a gated [q | gate] model
    boff : uint      //! this layer's row base in the [q | k | v] bias rows
    hasb : uint
    cstride : uint   //! a position's cos/sin plane width (every rope class's row); 0 = hs, the one-class plane
    cbase : uint     //! this layer's rope class's row base inside the plane
    row0 : uint      //! the segment's first row in the q, k/v and cos planes: a segmented window dispatches a segment at its rows
    ring : uint      //! a sliding layer's ring rows: a row lands at `kv_ring_row` of its position; 0 = the flat slab
}

//! batched rope + K/V store: q roped in place, k/v into the mirrors
[ |> template_struct_instance]
class template RopeKvStoreBT : KvMirStoreT {
    @ssbo @binding = 0 qrows : array<float>    //! q rows, roped in place
    @ssbo @binding = 1 kvrows : array<float>   //! k rows at 0, v rows at voff
    @ssbo @binding = 4 cossin : array<float>   //! per-position cos row then sin row (hs floats each)
    @ssbo @binding = 5 @role = "weight" bias : array<float>   //! per-layer [q | k | v] projection biases (hasb), else a placeholder
    @push_constant pa : RopeKvBArgs

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#rope-kv-merged-reads")]
    def rope_kv_b_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.npos * pa.ppp) {
            let p = gid / pa.ppp
            let lp = gid % pa.ppp
            let is_k = lp >= pa.kpair0
            let pr = is_k ? lp - pa.kpair0 : lp
            let hpairs = pa.hs / 2u
            let h = pr / hpairs
            let j = pr % hpairs
            let rot = j < pa.half
            let window_row = pa.row0 + p
            let cb = window_row * (pa.cstride == 0u ? pa.hs : pa.cstride) + pa.cbase
            let cj = min(j, pa.half - 1u)   // pass-through pairs read a clamped entry the select then drops
            let tcr = cossin[cb + cj]
            let tci = cossin[cb + pa.half + cj]
            let fcr = rot ? tcr : 1.0
            let fci = rot ? tci : 0.0
            let e0 = rot ? (pa.neox != 0u ? j : j * 2u) : 2u * pa.half + 2u * (j - pa.half)
            let e1 = rot ? (pa.neox != 0u ? j + pa.half : j * 2u + 1u) : e0 + 1u
            let apos = pa.pos0 + p
            let bhead = h * pa.hs
            if (is_k) {
                let kb = window_row * pa.kstride + h * pa.hs
                let a0 = kvrows[kb + e0] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + bhead + e0] : 0.0)
                let a1 = kvrows[kb + e1] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + bhead + e1] : 0.0)
                let mo = pa.layerbase + kv_ring_row(apos, pa.ring) * pa.kvd + h * pa.hs
                let vb = pa.voff + window_row * pa.kstride + h * pa.hs
                let v0 = kvrows[vb + e0] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + pa.kvd + bhead + e0] : 0.0)
                let v1 = kvrows[vb + e1] + (pa.hasb != 0u ? bias[pa.boff + pa.qd + pa.kvd + bhead + e1] : 0.0)
                st_k(mo + e0, a0 * fcr - a1 * fci)
                st_k(mo + e1, a0 * fci + a1 * fcr)
                st_v(mo + e0, v0)
                st_v(mo + e1, v1)
            } elif (rot || pa.hasb != 0u) {   // a pass-through q pair is rewritten only under a bias
                let qhs = pa.qhs == 0u ? pa.hs : pa.qhs
                let qb = window_row * (pa.qd / pa.hs) * qhs + h * qhs
                let a0 = qrows[qb + e0] + (pa.hasb != 0u ? bias[pa.boff + bhead + e0] : 0.0)
                let a1 = qrows[qb + e1] + (pa.hasb != 0u ? bias[pa.boff + bhead + e1] : 0.0)
                qrows[qb + e0] = a0 * fcr - a1 * fci
                qrows[qb + e1] = a0 * fci + a1 * fcr
            }
        }
    }
}

//! the fused q/k norm, rope and K/V store on the template both homes stamp (`GkQknRopeKv`): the row's mirror base off the
//! token block (`tok`), or under `win` the window's position, the sin rows a half past the cos rows in the one table plane
[ |> template_struct_instance]
class template QknRopeKvT : GkQknRopeKv {
    @ssbo @binding = 6 tok : array<TokMeta>    //! the shared per-token block

    def override mir_base(rowi : uint) : uint2 {
        var mrow = pa.layerbase + kv_ring_row(pa.pos0 + rowi, pa.ring) * pa.kvd
        if (pa.win == 0u) {   // the token rows' block is read on that form alone: a window's rows run past its extent
            mrow = pa.layerbase + tok[rowi].mirbase + kv_ring_row(tok[rowi].pos, pa.ring) * pa.kvd
        }
        return uint2(mrow, mrow)
    }

    def override rope_sin(cb, j : uint) : float => cossin[cb + pa.half + j]

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#nrow-kernel-rows")]
    def qkn_rope_body {
        let gwid = gl_WorkGroupID.x
        let rowwg = max(pa.rowwg, 1u)
        qkn_head(pa.rowwg == 0u ? 0u : gwid / rowwg, pa.rowwg == 0u ? gwid : gwid % rowwg, gl_LocalInvocationID.x)
    }
}

struct AtAttnArgs {
    rows : uint     //! the segment's query rows (the whole window on a one-session call)
    w0 : uint       //! the segment's position base (queries attend [0, w0 + row])
    qd : uint
    kvd : uint
    hs : uint
    kv_mul : uint
    flags : uint    //! the AT_ATTN_FLAG_* bits
    dpl : uint      //! output dims per lane (hs / 32)
    scale : float
}

let AT_ATTN_FLAG_GATED = 1u   //! `AtAttnArgs.flags`: the output times the sigmoid of the head's gate half

//! a Q-tile stamp's grid: a workgroup a (head, `tq`-row q tile), `tq` the stamp's `TQ`
def at_attn_tile_wgs(n_heads, rows : int64; tq : int64 = 8l) : int64 => n_heads * ((rows + tq - 1l) / tq)

//! the flash Q-tile core (AtAttn, DaAttnB, DaAttnBWT; not the h128 coopmat twin): a leaf declares its own pa/outp, so the methods take every varying scalar as a param
[ |> template_struct_instance]
class template AtAttnTileT {
    @ssbo @binding = 0 qpanel : array<float>   //! the prepped/roped q rows
    @ssbo @binding = 1 kpanel : array<KT>      //! the K plane (a panel or the mirror, kbase offsets)
    @ssbo @binding = 2 vpanel : array<KT>      //! the V plane (raw rows or the mirror, vbase offsets)
    @template_constant TQ : uint = 8u          //!< the q tile's rows
    @template_constant KS : uint = 32u         //!< the K/V stage's positions
    @template_constant LD : uint = 257u        //!< the K/V stage's row stride (one past the widest head: bank-spread)
    @workgroup q_tile : float[2048]
    @workgroup kv_stage : float[int(KS * LD)]
    @workgroup score_tile : float[int(TQ * KS)]
    @workgroup fm : float[int(TQ)]         //! per-row online-softmax state
    @workgroup fl : float[int(TQ)]
    @workgroup fal : float[int(TQ)]

    def load_q(q0, nrows, qstride, h, hs, qhs : uint) {
        let tid = gl_LocalInvocationID.x
        var i = tid
        while (i < TQ * hs) {
            let qr = q0 + i / hs
            q_tile[i] = qr < nrows ? qpanel[qr * qstride + h * qhs + i % hs] : 0.0
            i += 256u
        }
        if (tid < TQ) {
            fm[tid] = -3.0e38
            fl[tid] = 0.0
        }
    }

    def load_k(k0, kend, kbase, kvd, kvh, hs, ring : uint) {
        var i = gl_LocalInvocationID.x
        while (i < KS * hs) {
            let kr = k0 + i / hs
            kv_stage[(i / hs) * LD + i % hs] = kr < kend ? float(kpanel[kbase + kv_ring_row(kr, ring) * kvd + kvh * hs + i % hs]) : 0.0   // nolint:PERF020,LINT019 -- KT convert-on-load; a no-op only in the f32 stamp
            i += 256u
        }
    }

    //! the V tile, reusing the K staging at vbase (call only after every K read completed)
    def load_v(k0, kend, vbase, kvd, kvh, hs, ring : uint) {
        var i = gl_LocalInvocationID.x
        while (i < KS * hs) {
            let kr = k0 + i / hs
            kv_stage[(i / hs) * LD + i % hs] = kr < kend ? float(vpanel[vbase + kv_ring_row(kr, ring) * kvd + kvh * hs + i % hs]) : 0.0   // nolint:PERF020,LINT019 -- KT convert-on-load; a no-op only in the f32 stamp
            i += 256u
        }
    }

    def qk_score(r, c, hs, k0, gq, q0, nrows, win : uint; scale, cap : float) {
        var s = 0.0
        var j = 0u
        while (j < hs) {   // 4-wide manual unroll (hs % 32 == 0)
            s += q_tile[r * hs + j] * kv_stage[c * LD + j]
            s += q_tile[r * hs + j + 1u] * kv_stage[c * LD + j + 1u]
            s += q_tile[r * hs + j + 2u] * kv_stage[c * LD + j + 2u]
            s += q_tile[r * hs + j + 3u] * kv_stage[c * LD + j + 3u]
            j += 4u
        }
        let gk = k0 + c
        let inwin = win == 0u || gk + win > gq
        score_tile[r * KS + c] = (gk <= gq && inwin && q0 + r < nrows) ? attn_softcap(s * scale, cap) : -3.0e38
    }

    //! the first K/V tile a window's queries can see: a sliding layer's tile below every query's window start skips whole
    def k_first(w0, q0, win : uint) : uint => (win != 0u && w0 + q0 > win) ? ((w0 + q0 - win) / KS) * KS : 0u

    def softmax_update {
        let tid = gl_LocalInvocationID.x
        if (tid < TQ) {
            var mt = fm[tid]
            var j2 = 0u
            while (j2 < KS) {
                mt = max(mt, score_tile[tid * KS + j2])
                j2++
            }
            let alpha = exp(fm[tid] - mt)
            var lsum = 0.0
            j2 = 0u
            while (j2 < KS) {
                let e = exp(score_tile[tid * KS + j2] - mt)
                score_tile[tid * KS + j2] = e
                lsum += e
                j2++
            }
            fl[tid] = fl[tid] * alpha + lsum
            fm[tid] = mt
            fal[tid] = alpha
        }
    }
}

//! the mirror-window pass (DaAttnB's) over the tile machinery: batched causal attention over the decode mirrors, K/V read at
//! kbase/vbase, q read head-strided (qhs), the gated epilogue under the sigmoid of the head's gate half; dpl-guarded accumulators
//! serve hs 32..256 (the one-row tile leaf over `AtAttnTileT` has its own pass and declares its own args and out plane)
struct DaAttnBArgs {
    npos : uint     //! the segment's query rows (the whole window on a one-session call)
    nh : uint
    kvd : uint
    hs : uint
    kv_mul : uint
    kbase : uint    //! word offsets into the K / V mirror planes
    vbase : uint
    w0 : uint       //! the segment's position base (queries attend [0, w0 + row])
    scale : float
    qhs : uint      //! per-head q stride: hs, or 2 * hs on a gated [q | gate] model
    gated : uint    //! multiply each head's output by the sigmoid of its gate half
    window : uint   //! a sliding layer's queries attend the `window` positions up to their own; 0 = every position
    cap : float     //! the attention logit softcap on the scaled scores (gemma-2: 50); 0 = none
    row0 : uint     //! the segment's first row in the q panel and the out plane: a segmented window dispatches a segment at its rows
    ring : uint     //! a sliding layer's ring rows: key `k` reads its `kv_ring_row` slot; 0 = the flat slab
}

[ |> template_struct_instance]
class template DaAttnBT : AtAttnTileT {
    @ssbo @binding = 3 outp : array<float>   //! the attention out rows
    @push_constant pa : DaAttnBArgs

    [spirv_inline]
    def at_attn_body {   // nolint:STYLE038 — the verbatim flash pass, parity-locked to the shipped kernel
        let qd = pa.nh * pa.hs
        let qhs = pa.qhs == 0u ? pa.hs : pa.qhs
        let qrow = pa.nh * qhs
        let dpl = pa.hs / 32u
        let qtiles = (pa.npos + (TQ - 1u)) / TQ
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * TQ
        let tid = gl_LocalInvocationID.x
        let r = tid / KS
        let c = tid % KS
        let kvh = h / pa.kv_mul
        load_q(pa.row0 + q0, pa.row0 + pa.npos, qrow, h, pa.hs, qhs)
        var o0 = 0.0
        var o1 = 0.0
        var o2 = 0.0
        var o3 = 0.0
        var o4 = 0.0
        var o5 = 0.0
        var o6 = 0.0
        var o7 = 0.0
        barrier()
        let gq = pa.w0 + q0 + r
        let kend = pa.w0 + min(q0 + TQ, pa.npos)   // causal bound (exclusive) of the tile's last row
        var k0 = k_first(pa.w0, q0, pa.window)
        while (k0 < kend) {
            load_k(k0, kend, pa.kbase, pa.kvd, kvh, pa.hs, pa.ring)
            barrier()
            qk_score(r, c, pa.hs, k0, gq, q0, pa.npos, pa.window, pa.scale, pa.cap)
            barrier()
            softmax_update()
            barrier()
            let av = fal[r]
            o0 *= av
            if (dpl > 1u) {
                o1 *= av
            }
            if (dpl > 2u) {
                o2 *= av
                o3 *= av
            }
            if (dpl > 4u) {
                o4 *= av
                o5 *= av
                o6 *= av
                o7 *= av
            }
            load_v(k0, kend, pa.vbase, pa.kvd, kvh, pa.hs, pa.ring)
            barrier()
            var c2 = 0u
            while (c2 < KS) {
                let pw = score_tile[r * KS + c2]
                let vb = c2 * LD + c
                o0 += pw * kv_stage[vb]
                if (dpl > 1u) {
                    o1 += pw * kv_stage[vb + 32u]
                }
                if (dpl > 2u) {
                    o2 += pw * kv_stage[vb + 64u]
                    o3 += pw * kv_stage[vb + 96u]
                }
                if (dpl > 4u) {
                    o4 += pw * kv_stage[vb + 128u]
                    o5 += pw * kv_stage[vb + 160u]
                    o6 += pw * kv_stage[vb + 192u]
                    o7 += pw * kv_stage[vb + 224u]
                }
                c2++
            }
            barrier()
            k0 += KS
        }
        if (q0 + r < pa.npos) {
            let inv = 1.0 / fl[r]
            let ob = (pa.row0 + q0 + r) * qd + h * pa.hs + c
            let gb = (pa.row0 + q0 + r) * qrow + h * qhs + pa.hs + c
            var jj = 0u
            while (jj < dpl) {
                var v = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                    : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
                if (pa.gated != 0u) {
                    v = sig_gate(v, qpanel[gb + jj * 32u])
                }
                outp[ob + jj * 32u] = v
                jj++
            }
        }
    }
}

[spirv_combine]
def facm2_max(a, b : float) : float => max(a, b)

[spirv_combine, unused_argument(b)]
def facm2_smear(a, b : float) : float => a

[spirv_per_element, unused_argument(r, c)]
def facm2_exp(r, c : uint; v : float) : float => exp(v)

//! the flash tiles' row-max bias, 3 ln 2: every P = e^(S - M) sits at an eighth or under, so a step's f16 P x V accumulator cannot overflow; L carries the same bias and the final divide cancels it
let FA_MAX_BIAS = 2.0794415

[spirv_per_element, unused_argument(r, c)]
def facm2_pick(r, c : uint; v, o : float) : float => max(v + FA_MAX_BIAS, o)

//! -FLT_MAX/2 keeps Mold - M NaN-free; pad rows keep a causal window
[spirv_per_element]
def facm2_mask(r, c : uint; v : float; qbase, kbase, win : uint) : float => (kbase + c > qbase + r || (win != 0u && kbase + c + win <= qbase + r)) ? -1.7e38 : v

//! the bidirectional stamps' only mask: the key columns past kvlen
[spirv_per_element, unused_argument(r)]
def facm2_tail(r, c : uint; v : float; kbase, kvlen : uint) : float => kbase + c >= kvlen ? -1.7e38 : v

[spirv_per_element, unused_argument(r, c)]
def facm2_recip(r, c : uint; v : float) : float => v == 0.0 ? 0.0 : 1.0 / v

[spirv_per_element, unused_argument(r, c)]
def facm2_sigmoid(r, c : uint; v : float) : float => sigmoid_f32(v)

struct FaArgs {
    rows : uint     //! the segment's query rows (the whole window on a one-session call)
    w0 : uint       //! the segment's position base (queries attend [0, w0 + row])
    qd : uint       //! o row stride (n_heads * hs); the q row's too when qstride is 0
    kvd : uint      //! k/v row stride (n_kv * hs)
    kv_mul : uint   //! GQA ratio (head / kv_mul = kv head)
    kvbase : uint   //! element base of this layer's plane in the K/V shadow mirrors
    scale : float
    qstride : uint  //! the q panel's row stride on a gated [q | gate] plane; 0 = qd
    qhs : uint      //! the q panel's per-head stride (2 x hs on a gated plane); 0 = hs
    window : uint   //! a sliding layer's queries attend the `window` positions up to their own; 0 = every position
    cap : float     //! the attention logit softcap on the scaled scores (gemma-2: 50), read by the CAP stamps; 0 = none
    sinkoff : uint  //! the SINK stamps: this layer's row base in the sink plane ([n_heads] a layer)
    row0 : uint     //! the segment's first row in the q panel and the out plane: a segmented window dispatches a segment at its rows
    ring : uint     //! a sliding layer's ring rows: key `j` reads its `kv_ring_row` slot, a multiple of the tile's step; 0 = the flat rows
}

//! gemma-2's attention logit softcap on a scaled score tile
[spirv_per_element, unused_argument(r, c)]
def facm2_softcap(r, c : uint; v : float; cap : float) : float => softcap_exp(v, cap)

[arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#flash-attn-tile")]
def fa_stamp_wgs(khr : bool; nheads, rows : int64) : int64 {
    let qrows_per_wg = khr ? 16l : 64l
    return nheads * ((rows + qrows_per_wg - 1l) / qrows_per_wg)
}

//! the flash pass over the f16 K/V shadows: one template, HS x KHR
[ |> template_struct_instance]
class template FaT {
    @ssbo @binding = 0 qpanel : array<float>     //! the prepped/roped window q rows (window-local)
    @ssbo @binding = 1 kh : array<float16>       //! f16 K shadow at absolute positions
    @ssbo @binding = 2 vh : array<float16>       //! f16 V shadow at absolute positions
    @ssbo @binding = 3 outp : array<OT>          //! attention out plane (rows x qd)
    @ssbo @binding = 4 @role = "weight" @template_gate = SINK sinks : array<float>   //! the SINK stamps: the per-head sink logits, `pa.sinkoff` the layer's row
    @push_constant pa : FaArgs
    @template_constant HS : uint = 64u           //! the head width
    @template_constant BC : uint = 64u           //! the cm2 arm's K/V step: 64 at hs 64, 32 above
    @template_constant OUT16 : bool = false      //! the o store as f16
    @template_constant GATED : bool = false      //! o times the sigmoid of the [q | gate] head's gate half (both arms, head 256)
    @template_constant CAP : bool = false        //! the softcap stamps: every scaled score through cap * tanh(s / cap) before the mask (gemma-2)
    @template_constant SINK : bool = false       //! the sink stamps (gpt-oss): the online softmax seeds its max with the head's sink logit and its sum with one - the phantom key with no value row
    @template_constant KHR : bool = false        //! the KHR cooperative-matrix arm; its lane map is the cited section's
    @template_constant CAUSAL : bool = true      //! the bidirectional stamps (the vision towers' encoder attention) clear it: every query row attends every key row, and the mask keeps only the tail past kvlen
    @workgroup @template_gate = KHR fq : uint[4096]     //! the 16-row q tile, scaled f16 pairs, sized for the 512 head
    @workgroup @template_gate = KHR fs : float[1024]    //! the 16 x 64 score tile, row stride 64
    @workgroup @template_gate = KHR fp : uint[512]      //! P as f16 pairs, 16 x 64: 32 words a row
    @workgroup @template_gate = KHR fpv : uint[4096]    //! the step's P x V as f16 pairs, 16 x HS: HS / 2 words a row

    //! the KHR arm's running max seed: the head's sink logit on the sink stamps (the phantom key), the floor elsewhere
    def seed_m(h : uint) : float {
        static_if (SINK) {
            return sinks[pa.sinkoff + h]
        } else {
            return -3.0e38
        }
    }

    //! ... and its running sum seed: the phantom key's own mass, one, on the sink stamps
    def seed_l : float => SINK ? 1.0 : 0.0

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#flash-attn-tile")]
    def fa_body {   // nolint:STYLE037,STYLE038 — the two arms of one flash pass, each the verbatim shipped kernel; a method call would emit a SPIR-V function the emitter never inlines
        static_if (KHR) {
            let qtiles = (pa.rows + 15u) / 16u
            let h = gl_WorkGroupID.x / qtiles
            let q0 = (gl_WorkGroupID.x % qtiles) * 16u
            let tid = gl_LocalInvocationID.x
            let sg = tid / 32u
            let kvh = h / pa.kv_mul
            let kvlen = pa.w0 + pa.rows
            let qstride = pa.qstride == 0u ? pa.qd : pa.qstride
            let qhs = pa.qhs == 0u ? HS : pa.qhs
            let hw = HS / 2u
            var i = tid
            while (i < 16u * hw) {
                let qr = q0 + i / hw
                let jp = (i % hw) * 2u
                if (qr < pa.rows) {
                    let src = (pa.row0 + qr) * qstride + h * qhs + jp
                    fq[i] = packHalf2x16(float2(qpanel[src] * pa.scale, qpanel[src + 1u] * pa.scale))
                } else {
                    fq[i] = 0u
                }
                i += 128u
            }
            let softmax_row = tid / 8u
            let key_col0 = (tid % 8u) * 8u
            let gq = pa.w0 + q0 + softmax_row
            let o_dim0 = (tid % 8u) * (HS / 8u)
            var m = seed_m(h)
            var l = seed_l()
            var o : float[64]
            for [unroll] (z in range(int(HS / 8u))) {
                o[z] = 0.0
            }
            let qbase = pa.w0 + q0
            let kend = CAUSAL ? pa.w0 + min(q0 + 16u, pa.rows) : kvlen
            var j = (pa.window != 0u && qbase > pa.window) ? ((qbase - pa.window) / 64u) * 64u : 0u
            barrier()
            while [dont_unroll] (j < kend) {
                let krow = j + sg * 16u
                if (krow < kvlen) {
                    var s : coopmatAcc_f32_16x16
                    var d = 0u
                    while (d < HS) {
                        var qa : coopmatA_f16_16x16
                        coopmatLoad(qa, fq, int(d / 2u), int(hw), 0)
                        var kb : coopmatB_f16_16x16
                        coopmatLoad(kb, kh, pa.kvbase + kv_ring_row(krow, pa.ring) * pa.kvd + kvh * HS + d, int(pa.kvd), 1)
                        s = coopmatMulAdd(qa, kb, s)
                        d += 16u
                    }
                    coopmatStore(s, fs, int(sg * 16u), 64, 0)
                }
                barrier()
                let m_old = m
                var mloc = -3.0e38
                for [unroll] (c in range(8)) {
                    let idx = softmax_row * 64u + key_col0 + uint(c)
                    let gk = j + key_col0 + uint(c)
                    let v = CAP ? softcap_exp(fs[idx], pa.cap) : fs[idx]
                    static_if (CAUSAL) {
                        let sv = (gk > gq || gk >= kvlen || (pa.window != 0u && gk + pa.window <= gq)) ? -3.0e38 : v
                        fs[idx] = sv
                        mloc = max(mloc, sv)
                    } else {
                        let sv = gk >= kvlen ? -3.0e38 : v
                        fs[idx] = sv
                        mloc = max(mloc, sv)
                    }
                }
                mloc = max(mloc, subgroupShuffleXor(mloc, 1u))
                mloc = max(mloc, subgroupShuffleXor(mloc, 2u))
                mloc = max(mloc, subgroupShuffleXor(mloc, 4u))
                let mt = max(m_old, mloc + FA_MAX_BIAS)
                var lloc = 0.0
                for [unroll] (c2 in range(4)) {
                    let idx = softmax_row * 64u + key_col0 + uint(c2) * 2u
                    let e0 = exp(fs[idx] - mt)
                    let e1 = exp(fs[idx + 1u] - mt)
                    fp[idx / 2u] = packHalf2x16(float2(e0, e1))
                    lloc += e0 + e1
                }
                lloc += subgroupShuffleXor(lloc, 1u)
                lloc += subgroupShuffleXor(lloc, 2u)
                lloc += subgroupShuffleXor(lloc, 4u)
                let alpha = exp(m_old - mt)
                l = l * alpha + lloc
                m = mt
                barrier()
                let sg_col0 = sg * (HS / 4u)
                var acc : coopmatAcc_f32_16x16[8]   //! f32: an f16 accumulator rounds toward zero on RDNA3's matrix cores, and a step's sum drifts low
                var kc = 0u
                while (kc < 64u) {
                    if (j + kc < kvlen) {
                        var pf : coopmatA_f16_16x16
                        coopmatLoad(pf, fp, int(kc / 2u), 32, 0)
                        for [unroll_full] (f in range(int(HS / 64u))) {
                            var vb : coopmatB_f16_16x16
                            coopmatLoad(vb, vh, pa.kvbase + kv_ring_row(j + kc, pa.ring) * pa.kvd + kvh * HS + sg_col0 + uint(f) * 16u, int(pa.kvd), 0)
                            acc[f] = coopmatMulAdd(pf, vb, acc[f])
                        }
                    }
                    kc += 16u
                }
                for [unroll_full] (f2 in range(int(HS / 64u))) {
                    var a16 : coopmatAcc_f16_16x16   //! the step's sum rounded once for the f16 stash
                    coopmatConvert(a16, acc[f2])
                    coopmatStore(a16, fpv, int((sg_col0 + uint(f2) * 16u) / 2u), int(hw), 0)
                }
                barrier()
                for [unroll] (z2 in range(int(HS / 16u))) {
                    let pair = unpackHalf2x16(fpv[softmax_row * hw + (o_dim0 + uint(z2) * 2u) / 2u])
                    o[z2 * 2] = o[z2 * 2] * alpha + pair.x
                    o[z2 * 2 + 1] = o[z2 * 2 + 1] * alpha + pair.y
                }
                barrier()
                j += 64u
            }
            if (q0 + softmax_row < pa.rows) {
                let inv = 1.0 / l
                let ob = (pa.row0 + q0 + softmax_row) * pa.qd + h * HS + o_dim0
                for [unroll] (z3 in range(int(HS / 8u))) {
                    static_if (GATED) {   // a gated panel is [q | gate] a head: the row stride 2 qd and the head width 2 HS follow; the gate half is read unscaled
                        let ov = o[z3] * inv * sigmoid_f32(qpanel[(pa.row0 + q0 + softmax_row) * (2u * pa.qd) + h * (2u * HS) + HS + o_dim0 + uint(z3)])
                        static_if (OUT16) {
                            outp[ob + uint(z3)] = float16(ov)
                        } else {
                            outp[ob + uint(z3)] = ov
                        }
                    } else {
                        static_if (OUT16) {
                            outp[ob + uint(z3)] = float16(o[z3] * inv)
                        } else {
                            outp[ob + uint(z3)] = o[z3] * inv
                        }
                    }
                }
            }
        } else {
            let qtiles = (pa.rows + 63u) / 64u
            let h = gl_WorkGroupID.x / qtiles
            let qt = gl_WorkGroupID.x % qtiles
            let q0 = qt * 64u
            let kvh = h / pa.kv_mul
            let kvlen = pa.w0 + pa.rows
            let qstride = pa.qstride == 0u ? pa.qd : pa.qstride
            let qhs = pa.qhs == 0u ? HS : pa.qhs
            var tlq : tensorLayout2DPad
            tensorLayoutCreate(tlq)
            tensorLayoutSetDimension(tlq, pa.row0 + pa.rows, qstride)
            tensorLayoutSetStride(tlq, qstride, 1u)
            var tlk : tensorLayout2DPad
            tensorLayoutCreate(tlk)
            tensorLayoutSetDimension(tlk, pa.ring != 0u ? pa.ring : kvlen, pa.kvd)   // a ring's rows whole: a step never straddles its wrap
            tensorLayoutSetStride(tlk, pa.kvd, 1u)
            var tlo : tensorLayout2DPad
            tensorLayoutCreate(tlo)
            tensorLayoutSetDimension(tlo, pa.row0 + pa.rows, pa.qd)
            tensorLayoutSetStride(tlo, pa.qd, 1u)
            var tv : tensorView2Dt
            tensorViewCreate(tv)
            var q : QACC
            coopmatLoadTensor(q, qpanel, 0u, tlq, pa.row0 + q0, 64u, h * qhs, HS)
            coopmatScale(q, pa.scale)
            var qf : QA
            coopmatConvert(qf, q)
            var mrow, lrow : SACC
            static_if (SINK) {
                coopmatFill(mrow, sinks[pa.sinkoff + h])
                coopmatFill(lrow, 1.0)
            } else {
                coopmatFill(mrow, -1.7e38)
            }
            var o : OF
            var one : ONE
            coopmatFill(one, 1.0)
            let qbase = pa.w0 + q0
            let kmax = CAUSAL ? pa.w0 + min(q0 + 64u, pa.rows) : kvlen   // the last row's causal bound; every key row when bidirectional
            // sliding layer: start below the first query window
            var j = (pa.window != 0u && qbase > pa.window) ? ((qbase - pa.window) / BC) * BC : 0u
            while [dont_unroll] (j < kmax) {
                var s : SACC
                var kt : KB
                coopmatLoadTensor(kt, kh, pa.kvbase, tlk, kv_ring_row(j, pa.ring), BC, kvh * HS, HS, tv)
                s = coopmatMulAdd(qf, kt, s)
                static_if (CAP) {
                    coopmatPerElement(s, s, @@facm2_softcap, pa.cap)
                }
                static_if (CAUSAL) {
                    if (j + BC > qbase + 1u || (pa.window != 0u && j + pa.window < qbase + 64u)) {
                        coopmatPerElement(s, s, @@facm2_mask, qbase, j, pa.window)
                    }
                } else {
                    if (j + BC > kvlen) {   // the padded tensor load reads the tail past kvlen as zero rows: a zero score, not a masked one
                        coopmatPerElement(s, s, @@facm2_tail, j, kvlen)
                    }
                }
                var rmax : SACC
                coopmatReduce(rmax, s, COOPMAT_REDUCE_ROW, @@facm2_max)
                var mold : SACC
                coopmatCopy(mold, mrow)
                coopmatPerElement(mrow, rmax, @@facm2_pick, mold)
                coopmatSub(s, s, mrow)
                coopmatPerElement(s, s, @@facm2_exp)                        // P =e^(S - M), in place
                var em : SACC
                coopmatSub(em, mold, mrow)
                coopmatPerElement(em, em, @@facm2_exp)                      // eM =e^(Mold - M)
                var pf : PA
                coopmatConvert(pf, s)
                var rsum : SACC
                rsum = coopmatMulAdd(pf, one, rsum)                         // nolint:STYLE011,LINT019 - rowsum by ones-matrix multiply; MulAdd seeds from its own zero-filled tile, the rule fires at the leaves' compiles
                coopmatMulElem(lrow, em, lrow)
                coopmatAdd(lrow, lrow, rsum)                                // L =eM*L + rowsum
                var emd : QACC
                coopmatReduce(emd, em, COOPMAT_REDUCE_ROW, @@facm2_smear)
                var emf : OF
                coopmatConvert(emf, emd)
                coopmatMulElem(o, emf, o)
                var vt : VB
                coopmatLoadTensor(vt, vh, pa.kvbase, tlk, kv_ring_row(j, pa.ring), BC, kvh * HS, HS)
                o = coopmatMulAdd(pf, vt, o)                                // O =eMdiag*O + P @ V
                j += BC
            }
            var o32 : QACC
            coopmatConvert(o32, o)
            var ld : QACC
            coopmatReduce(ld, lrow, COOPMAT_REDUCE_ROW, @@facm2_smear)
            coopmatPerElement(ld, ld, @@facm2_recip)
            coopmatMulElem(o32, ld, o32)                                    // O /= L (padding rows discard at store)
            static_if (GATED) {
                var g : QACC
                coopmatLoadTensor(g, qpanel, 0u, tlq, pa.row0 + q0, 64u, h * qhs + HS, HS)
                coopmatPerElement(g, g, @@facm2_sigmoid)
                coopmatMulElem(o32, g, o32)
            }
            static_if (OUT16) {
                var of : OF
                coopmatConvert(of, o32)
                coopmatStoreTensor(of, outp, 0u, tlo, pa.row0 + q0, 64u, h * HS, HS)
            } else {
                coopmatStoreTensor(o32, outp, 0u, tlo, pa.row0 + q0, 64u, h * HS, HS)
            }
        }
    }
}

//! hs=64 (tinyllama-class): the whole head is one cm2 K/V step
[ |> template_struct_instance]
class template FaH64T : FaT {
    override HS = 64u
    override BC = 64u
    typedef QACC = coopmatWgAcc_f32_64x64
    typedef QA = coopmatWgA_f16_64x64
    typedef SACC = coopmatWgAcc_f32_64x64
    typedef PA = coopmatWgA_f16_64x64
    typedef KB = coopmatWgB_f16_64x64
    typedef VB = coopmatWgB_f16_64x64
    typedef ONE = coopmatWgB_f16_64x64
    typedef OF = coopmatWgAcc_f16_64x64
}

//! hs=96 (phi-3's heads) at Bc=32: the cm2 arm alone - the KHR lane map walks the head in 64-wide halves
[ |> template_struct_instance]
class template FaH96T : FaT {
    override HS = 96u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x96
    typedef QA = coopmatWgA_f16_64x96
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_96x32
    typedef VB = coopmatWgB_f16_32x96
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x96
}

//! hs=128 (llama-3.2-3B-class) at Bc=32
[ |> template_struct_instance]
class template FaH128T : FaT {
    override HS = 128u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x128
    typedef QA = coopmatWgA_f16_64x128
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_128x32
    typedef VB = coopmatWgB_f16_32x128
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x128
}

//! hs=256 (the deltanet hybrids' heads) at Bc=32, gated on [q | gate]
[ |> template_struct_instance]
class template FaH256T : FaT {
    override HS = 256u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x256
    typedef QA = coopmatWgA_f16_64x256
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_256x32
    typedef VB = coopmatWgB_f16_32x256
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x256
}

//! hs=512 (gemma-4's global heads, V from K): h256 at twice the width
[ |> template_struct_instance]
class template FaH512T : FaT {
    override HS = 512u
    override BC = 32u
    typedef QACC = coopmatWgAcc_f32_64x512
    typedef QA = coopmatWgA_f16_64x512
    typedef SACC = coopmatWgAcc_f32_64x32
    typedef PA = coopmatWgA_f16_64x32
    typedef KB = coopmatWgB_f16_512x32
    typedef VB = coopmatWgB_f16_32x512
    typedef ONE = coopmatWgB_f16_32x32
    typedef OF = coopmatWgAcc_f16_64x512
}

//! DaAttnB's wide-head twin past 256: a 4-row q tile, 16-key stages
[ |> template_struct_instance]
class template DaAttnBWT : AtAttnTileT {
    @ssbo @binding = 3 outp : array<float>     //! attention out rows
    @push_constant pa : DaAttnBArgs
    override TQ = 4u
    override KS = 16u
    override LD = 513u

    [spirv_inline]
    def da_attn_bw_body {
        let hs = pa.hs
        let qd = pa.nh * hs
        let qhs = pa.qhs == 0u ? hs : pa.qhs
        let qrow = pa.nh * qhs
        let qtiles = (pa.npos + (TQ - 1u)) / TQ
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * TQ
        let tid = gl_LocalInvocationID.x
        let kvh = h / pa.kv_mul
        load_q(pa.row0 + q0, pa.row0 + pa.npos, qrow, h, hs, qhs)
        var o : float[8]   // row vr's dims vc + 64 * z
        for (z in range(8)) {
            o[z] = 0.0
        }
        let vr = tid / 64u
        let vc = tid % 64u
        barrier()
        let kend = pa.w0 + min(q0 + TQ, pa.npos)   // causal bound (exclusive) of the tile's last row
        var k0 = k_first(pa.w0, q0, pa.window)
        while (k0 < kend) {
            load_k(k0, kend, pa.kbase, pa.kvd, kvh, hs, pa.ring)
            barrier()
            if (tid < TQ * KS) {
                qk_score(tid / KS, tid % KS, hs, k0, pa.w0 + q0 + tid / KS, q0, pa.npos, pa.window, pa.scale, pa.cap)
            }
            barrier()
            softmax_update()
            barrier()
            let av = fal[vr]
            for (z in range(8)) {
                o[z] *= av
            }
            load_v(k0, kend, pa.vbase, pa.kvd, kvh, hs, pa.ring)
            barrier()
            var c2 = 0u
            while (c2 < KS) {
                let pw = score_tile[vr * KS + c2]
                let vb = c2 * LD + vc
                for [unroll] (z in range(8)) {
                    o[z] += pw * kv_stage[vb + 64u * uint(z)]
                }
                c2++
            }
            barrier()
            k0 += KS
        }
        if (q0 + vr < pa.npos) {
            let inv = 1.0 / fl[vr]
            let ob = (pa.row0 + q0 + vr) * qd + h * hs + vc
            let gb = (pa.row0 + q0 + vr) * qrow + h * qhs + hs + vc
            for (z in range(8)) {
                if (vc + 64u * uint(z) < hs) {
                    var v = o[z] * inv
                    if (pa.gated != 0u) {
                        v = sig_gate(v, qpanel[gb + 64u * uint(z)])
                    }
                    outp[ob + 64u * uint(z)] = v
                }
            }
        }
    }
}

struct BatchArgs {
    n : uint         //! reduction dim
    d : uint         //! output cols
    map_off : uint   //! the per-wg map's word offset in sched (region records sit at 0)
    ksplit : uint    //! cm2 split-k: 256-aligned k chunk per partial plane; 0 = no split
}

struct SkRedArgs {
    nelem : uint   //! one partial plane's float count (the GEMM record's rows x d - the rounded count a tower record carries): any count - the reduce guards its four-wide tail element by element
    k : uint       //! planes
    ybase : uint   //! the sum's element base in y (0 for a plane of its own)
}

//! the split-k reduce's grid over `nelem` floats: four a thread, 256 threads a workgroup
def splitk_reduce_wgs(nelem : int64) : int64 => (nelem + 1_023l) / 1_024l

//! the split-k reduce: k partial planes of nelem floats summed into y at ybase, four floats a thread like the elementwise family
[ |> template_struct_instance]
class template SplitKReduceT {
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 y : array<float>
    @push_constant pa : SkRedArgs

    [spirv_inline]
    def splitk_reduce_body {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            var s0 = 0.0
            var s1 = 0.0
            var s2 = 0.0
            var s3 = 0.0
            let n1 = e + 1u < pa.nelem
            let n2 = e + 2u < pa.nelem
            let n3 = e + 3u < pa.nelem
            var p = 0u
            while (p < pa.k) {
                let b = p * pa.nelem + e
                s0 += part[b]
                if (n1) {
                    s1 += part[b + 1u]
                }
                if (n2) {
                    s2 += part[b + 2u]
                }
                if (n3) {
                    s3 += part[b + 3u]
                }
                p++
            }
            let o = pa.ybase + e
            y[o] = s0
            if (n1) {
                y[o + 1u] = s1
            }
            if (n2) {
                y[o + 2u] = s2
            }
            if (n3) {
                y[o + 3u] = s3
            }
        }
    }
}

let SCHED_NONE = 0xFFFFFFFFu   //! the per-wg map's sentinel: a tile workgroup past a device-written schedule's end reads a zero-row region and returns

//! the row kernels' value form of the common scan (`GkWgScan`): the exclusive prefix over the `WG` threads' v
[ |> template_struct_instance]
class template WgScanT : GkWgScan {
    def excl_scan(v : uint) : uint {
        var cum = 0u
        excl_scan_into(v, cum)
        return cum
    }
}

//! the top-k's records on the select both homes stamp (`GkTopKRow`): row `p`'s picks land in slots `p * k + j` - the expert and
//! its weight, and under `META` the slot's GEMV records (the decode's: the (wblock, xblock) region pairs the GEMV leaves read),
//! off it the two alone (the window's per-row select, which the bucket schedule reads)
[ |> template_struct_instance]
class template TopKRecords : GkTopKRow {
    @template_constant META : bool = true
    @ssbo @binding = 0 logits : array<float>  //! [npos x lstride]
    @ssbo @binding = 1 @template_gate = META meta1 : array<uint>   //! the gate stack's (wblock, xblock) per slot row
    @ssbo @binding = 2 @template_gate = META meta3 : array<uint>
    @ssbo @binding = 3 @template_gate = META meta2 : array<uint>
    @ssbo @binding = 4 w : array<float>      //! the routing weights, a slot each
    @ssbo @binding = 5 @role = "weight" ds : array<float>     //! the per-expert down scales (gemma-4), read at dsoff when the push names a row
    @ssbo @binding = 6 @role = "weight" rb : array<float>     //! the router biases ([ne] a layer, gpt-oss), read at rboff under `rbias_on`
    @ssbo @binding = 7 eidx : array<uint>    //! the picks' experts, a slot each: the bias rows the act and the combine read
    override DSCALE = true

    def override logit_at(p, e : uint) : float => logits[p * pa.lstride + e] + (pa.rbias_on != 0u ? rb[pa.rboff + e] : 0.0)
    def override down_scale(e : uint) : float => ds[pa.dsoff + e]

    //! row `p`: its logits selected, its k slots' records written by thread 0
    def row_picks(p : uint) {
        let tid = gl_LocalInvocationID.x
        select_row(p, pa.ne, pa.k, false)
        if (tid == 0u) {
            write_picks(p)
        }
    }

    def write_picks(p : uint) {
        var ws = 0.0
        pick_wsum_into(pa.k, ws)
        var jj = 0u
        while (jj < pa.k) {
            let s = p * pa.k + jj
            let ex = min(pick[jj], pa.ne - 1u)   // a NaN row leaves the sentinel: never a wild weight block
            var wj = 0.0
            pick_weight_into(jj, ex, pa.norm, ws, pa.wscale, wj)
            w[s] = wj
            eidx[s] = ex
            static_if (META) {
                meta1[s * 2u] = pa.base1 + ex * pa.stride1
                meta1[s * 2u + 1u] = p * pa.xnb1
                meta3[s * 2u] = pa.base3 + ex * pa.stride3
                meta3[s * 2u + 1u] = p * pa.xnb1
                meta2[s * 2u] = pa.base2 + ex * pa.stride2
                meta2[s * 2u + 1u] = s * pa.xnb2
            }
            jj++
        }
    }
}

struct RouterGemmArgs {
    npos : uint      //! rows
    ne : uint        //! router rows (the experts, plus the shared expert's gate row when it rides along)
    dim : uint       //! row width
    woff : uint      //! this layer's row base in w (elements)
}

//! the window's router GEMM: npos rows of ne logits over the f32 router rows
[ |> template_struct_instance]
class template RouterGemmT {
    @ssbo @binding = 0 @role = "weight" w4 : array<float4>   //! the router rows, row-major [ne x dim] per layer, as the stage's float4
    @ssbo @binding = 1 x4 : array<float4>      //! the FFN-normed rows, as float4
    @ssbo @binding = 2 logits : array<float>   //! [npos x ne]
    @push_constant pa : RouterGemmArgs
    @workgroup xs : float4[int(ROUTER_TP * ROUTER_LD)]
    @workgroup ws : float4[int(ROUTER_TE * ROUTER_LD)]

    //! the stage's float4 of slot `s` (row s / ROUTER_K4, k4 s % ROUTER_K4) of x rows from `p0` at step `kb`; zero past the window
    def fetch_x(p0, kb, s : uint) : float4 {
        let row = s / ROUTER_K4
        if (p0 + row >= pa.npos) return float4(0.0)
        return x4[((p0 + row) * pa.dim + kb) / 4u + (s % ROUTER_K4)]
    }

    //! ... and of the router rows from `eg0`; zero past the experts
    def fetch_w(eg0, kb, s : uint) : float4 {
        let row = s / ROUTER_K4
        if (eg0 + row >= pa.ne) return float4(0.0)
        return w4[(pa.woff + (eg0 + row) * pa.dim + kb) / 4u + (s % ROUTER_K4)]
    }

    //! a fetched float4 into its stage row at k4 `s % ROUTER_K4`
    def stage_x(s : uint; v : float4) {
        xs[(s / ROUTER_K4) * ROUTER_LD + (s % ROUTER_K4)] = v
    }

    def stage_w(s : uint; v : float4) {
        ws[(s / ROUTER_K4) * ROUTER_LD + (s % ROUTER_K4)] = v
    }

    //! `a` plus the four products in k order - the scalar loop's sums to the bit
    def acc4(a : float; x, w : float4) : float {
        var s = a
        s += x.x * w.x
        s += x.y * w.y
        s += x.z * w.z
        s += x.w * w.w
        return s
    }

    [arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#moe-routing-kernels")]
    def router_gemm_body {
        let tid = gl_LocalInvocationID.x
        let egroups = (pa.ne + ROUTER_TE - 1u) / ROUTER_TE
        let p0 = (gl_WorkGroupID.x / egroups) * ROUTER_TP
        let eg0 = (gl_WorkGroupID.x % egroups) * ROUTER_TE
        let tp = tid / ROUTER_TH   //! the invocation's four positions 4tp..4tp+3
        let te = tid % ROUTER_TH   //! ... and rows te, te + ROUTER_TH
        var a00 = 0.0
        var a01 = 0.0
        var a10 = 0.0
        var a11 = 0.0
        var a20 = 0.0
        var a21 = 0.0
        var a30 = 0.0
        var a31 = 0.0
        // the stage: 64 x rows x 16 float4 = four per invocation, 32 w rows x 16 = two
        var xr0 = fetch_x(p0, 0u, tid)
        var xr1 = fetch_x(p0, 0u, tid + 256u)
        var xr2 = fetch_x(p0, 0u, tid + 512u)
        var xr3 = fetch_x(p0, 0u, tid + 768u)
        var wr0 = fetch_w(eg0, 0u, tid)
        var wr1 = fetch_w(eg0, 0u, tid + 256u)
        var kb = 0u
        while (kb < pa.dim) {
            stage_x(tid, xr0)
            stage_x(tid + 256u, xr1)
            stage_x(tid + 512u, xr2)
            stage_x(tid + 768u, xr3)
            stage_w(tid, wr0)
            stage_w(tid + 256u, wr1)
            barrier()
            let nb = kb + 64u
            if (nb < pa.dim) {
                xr0 = fetch_x(p0, nb, tid)
                xr1 = fetch_x(p0, nb, tid + 256u)
                xr2 = fetch_x(p0, nb, tid + 512u)
                xr3 = fetch_x(p0, nb, tid + 768u)
                wr0 = fetch_w(eg0, nb, tid)
                wr1 = fetch_w(eg0, nb, tid + 256u)
            }
            let xb = (4u * tp) * ROUTER_LD
            let wb = te * ROUTER_LD
            let wb1 = wb + ROUTER_TH * ROUTER_LD
            var kk = 0u
            while (kk < ROUTER_K4) {
                let x0 = xs[xb + kk]
                let x1 = xs[xb + ROUTER_LD + kk]
                let x2 = xs[xb + 2u * ROUTER_LD + kk]
                let x3 = xs[xb + 3u * ROUTER_LD + kk]
                let w0 = ws[wb + kk]
                let w1 = ws[wb1 + kk]
                a00 = acc4(a00, x0, w0)
                a01 = acc4(a01, x0, w1)
                a10 = acc4(a10, x1, w0)
                a11 = acc4(a11, x1, w1)
                a20 = acc4(a20, x2, w0)
                a21 = acc4(a21, x2, w1)
                a30 = acc4(a30, x3, w0)
                a31 = acc4(a31, x3, w1)
                kk++
            }
            barrier()
            kb = nb
        }
        let pa0 = p0 + 4u * tp
        let ea0 = eg0 + te
        store_pair(pa0, ea0, a00, a01)
        store_pair(pa0 + 1u, ea0, a10, a11)
        store_pair(pa0 + 2u, ea0, a20, a21)
        store_pair(pa0 + 3u, ea0, a30, a31)
    }

    //! position p's two logits at rows e, e + ROUTER_TH, each within the window and the experts
    def store_pair(p, e : uint; v0, v1 : float) {
        if (p >= pa.npos) return
        if (e < pa.ne) {
            logits[p * pa.ne + e] = v0
        }
        if (e + ROUTER_TH < pa.ne) {
            logits[p * pa.ne + e + ROUTER_TH] = v1
        }
    }
}

struct SchedArgs {
    nk : uint          //! slot rows (npos x k)
    ne : uint          //! experts (at most 256: one thread per expert)
    wt_gu : uint       //! the gate/up GEMMs' weight tiles (ceil(nfe / 128))
    wt_dn : uint       //! the down GEMM's (ceil(dim / 128))
    map_off : uint     //! the s dispatch's per-wg map word offset in every sched plane; the records precede it
    map_off_mgu : uint //! the m dispatch's map offset in the gate/up planes
    map_off_mdn : uint //! ... and in the down plane
    bound_gu : uint    //! the s dispatch's workgroups per gate/up plane; the map's tail past the schedule reads the sentinel
    bound_dn : uint
    bound_mgu : uint   //! the m dispatch's
    bound_mdn : uint
    base1 : uint       //! expert 0's block base in the gate plane (slab-local, in the plane's units)
    stride1 : uint     //! blocks per expert
    base3 : uint
    stride3 : uint
    base2 : uint
    stride2 : uint
    s_only : uint      //! every bucket on the s column, a 32-row tile a piece (the Q8-fed chain's sdot4 tiles); 0 = the ladder's m columns too
}

let SCHED_S_ROWS = 32u    //! the s column: the ladder's small-bucket and remainder tile
let SCHED_M_ROWS = 128u   //! the m column: whole columns over a big bucket, the last one partial

//! the m-column rows of a `c`-row bucket: 0 within the s column, else whole m columns plus a partial one unless the remainder fits the s column
def sched_ladder_m_rows(c : uint) : uint {
    if (c <= SCHED_S_ROWS) return 0u
    return (c % SCHED_M_ROWS) > SCHED_S_ROWS ? c : (c / SCHED_M_ROWS) * SCHED_M_ROWS
}

let MOE_SCHED_MAP_OFF = 8l * TOPK_EXPERTS_MAX   //! a schedule plane's first per-wg map word: 4-word records for every expert the select reaches, then the s pieces' maps, then the m pieces'

//! the expert tiles' dispatch bounds for a window of `nk` slot rows over `ne` experts of width `nfe` on a model of `dim`: the s dispatch
//! at most one tile an expert (a bucket within the s column, or the remainder past whole m columns), the m dispatch every expert's whole
//! columns plus a partial one; `s_only` (the Q8-fed chain) puts every row on 32-row pieces over 32-column tiles
def moe_sched_bounds(ne, nfe, dim, nk : int64; s_only : bool) : tuple<gu : int64; dn : int64; mgu : int64; mdn : int64> {
    if (s_only) {
        let rt = nk / int64(SCHED_S_ROWS) + ne
        return (gu = rt * ((nfe + 31l) / 32l), dn = rt * ((dim + 31l) / 32l), mgu = 0l, mdn = 0l)
    }
    let wt_gu = (nfe + 127l) / 128l
    let wt_dn = (dim + 127l) / 128l
    let rt_m = nk / int64(SCHED_M_ROWS) + ne
    return (gu = ne * wt_gu, dn = ne * wt_dn, mgu = rt_m * wt_gu, mdn = rt_m * wt_dn)
}

//! the expert schedule's args both discrete homes build (`SchedArgs`): the slot rows, the experts, the weight tiles at `wcol`
//! output columns, the s dispatch's map offset and the m dispatches' past the s bounds, the bounds, and the three stacks'
//! slab-local block bases with their per-expert strides
def gk_sched_args(nk, ne, nfe, dim : int64; wcol : uint; s_only : bool; bounds : tuple<gu : int64; dn : int64; mgu : int64; mdn : int64>;
                  base1, base3, base2 : int64; fe1, fe3, fe2 : int) : SchedArgs =>
    SchedArgs(nk = uint(nk), ne = uint(ne), wt_gu = (uint(nfe) + wcol - 1u) / wcol, wt_dn = (uint(dim) + wcol - 1u) / wcol,
        s_only = s_only ? 1u : 0u,
        map_off = uint(MOE_SCHED_MAP_OFF), map_off_mgu = uint(MOE_SCHED_MAP_OFF + bounds.gu), map_off_mdn = uint(MOE_SCHED_MAP_OFF + bounds.dn),
        bound_gu = uint(bounds.gu), bound_dn = uint(bounds.dn), bound_mgu = uint(bounds.mgu), bound_mdn = uint(bounds.mdn),
        base1 = uint(base1), stride1 = uint(nfe * dim / kq_unit(fe1)),
        base3 = uint(base3), stride3 = uint(nfe * dim / kq_unit(fe3)),
        base2 = uint(base2), stride2 = uint(nfe * dim / kq_unit(fe2)))

//! the routed form's act push for `elems` gate elements of rows `nfe` wide (`moe_gpu_ffn_form`), the one both discrete homes'
//! chains build: its activation code, and under a bias the layer's row base in the bias planes (the up rows share the gate's)
//! and the row width
def ffn_form_act(elems, nfe : int64; is_gelu : bool) : ActArgs {
    let f = moe_gpu_ffn_form()
    var pca = ActArgs(nelem = uint(elems), act = act_code(f.oai, is_gelu))
    if (f.bias_off >= 0l) {
        pca.bias_on = 1u
        pca.gboff = uint(f.bias_off)
        pca.brow = uint(nfe)
    }
    return pca
}

//! the per-layer-embedding branch's args both discrete homes build for layer `l` over `nr` rows of a model of `dim` with `ple`-wide
//! side rows, `all` of them a position: the f32 gate GEMV, the f32 act, the f32 projection GEMV, the fused act-requant-proj
//! dispatch over the proj plane's block `wblk`, and the act with its requant (the caller sets the requant's `nblk`)
def gk_ple_gate_args(ple, dim, l : int64; nr : uint) : GkRouterArgs => GkRouterArgs(ne = uint(ple), dim = uint(dim), woff = uint(l * ple * dim), ncols = nr)
def gk_ple_proj_args(ple, dim, l : int64; nr : uint) : GkRouterArgs => GkRouterArgs(ne = uint(dim), dim = uint(ple), woff = uint(l * ple * dim), ncols = nr)
def gk_ple_act_f32_args(ple, l, all : int64; nr : uint) : GkPleActArgs => GkPleActArgs(nelem = uint(ple) * nr, sbase = uint(l * ple), sstride = uint(all), slen = uint(ple))
def gk_ple_fused_args(ple, dim, wblk, l, all : int64; nr : uint) : PleActArgs =>
    PleActArgs(n = uint(ple), d = uint(dim), wblk = uint(wblk), ubase = uint(l * ple), act = ACT_GELU, ncols = nr, sstride = uint(all))
def gk_ple_act_args(ple, l, all : int64; nr : uint) : ActArgs =>
    ActArgs(nelem = uint(ple) * nr, act = ACT_GELU, ubase = uint(l * ple), ustride = uint(all), ulen = uint(ple))

//! the window's bucket schedule, a thread an expert: `WG` threads reach `WG` experts (the 512-thread stamp past 256); its scan is `WgScanT`'s
[ |> template_struct_instance]
class template MoeSchedT : WgScanT {
    @template_constant ROW_PAD : uint = 0u    //!< every bucket's rows start at a multiple of it (the MMQ tiles' 128: a tile past a bucket's rows lands in its pad); 0 = packed
    @template_constant M_ONLY : bool = false  //!< every bucket's rows on the m column (the MMQ tiles, which take a partial tile); off, the ladder
    @template_constant S_ONLY_ARG : bool = true   //!< `pa.s_only` is read; off (a stamp no site ever asks the s column alone of), every dispatch takes the ladder or `M_ONLY`
    @ssbo @binding = 0 idx : array<uint>       //! [nk] the picked expert per slot row
    @ssbo @binding = 1 inv : array<uint>       //! [nk] slot row -> bucket row
    @ssbo @binding = 2 sched1 : array<uint>    //! the gate plane's records + maps
    @ssbo @binding = 3 sched3 : array<uint>
    @ssbo @binding = 4 sched2 : array<uint>
    @ssbo @binding = 5 bex : array<uint>       //! [nk] bucket row -> its expert (the biased act's row map)
    @push_constant pa : SchedArgs
    @workgroup cnt : uint[int(WG)]    //! the experts' slot counts, then their bucket cursors

    //! one piece's record and per-wg map on the three planes: expert `tid`'s `rows` from `row0`,
    //! record `ri`, its `rt` column tiles from workgroup `wg_gu` / `wg_dn` of the dispatch whose
    //! maps sit at `map_gu` / `map_dn`
    def write_piece(ri, row0, rows, rt, wg_gu, wg_dn, map_gu, map_dn : uint) {
        let tid = gl_LocalInvocationID.x
        let rb = ri * 4u
        sched1[rb] = pa.base1 + tid * pa.stride1
        sched1[rb + 1u] = row0
        sched1[rb + 2u] = rows
        sched1[rb + 3u] = wg_gu
        sched3[rb] = pa.base3 + tid * pa.stride3
        sched3[rb + 1u] = row0
        sched3[rb + 2u] = rows
        sched3[rb + 3u] = wg_gu
        sched2[rb] = pa.base2 + tid * pa.stride2
        sched2[rb + 1u] = row0
        sched2[rb + 2u] = rows
        sched2[rb + 3u] = wg_dn
        var g = 0u
        while (g < rt * pa.wt_gu) {
            sched1[map_gu + wg_gu + g] = ri
            sched3[map_gu + wg_gu + g] = ri
            g++
        }
        g = 0u
        while (g < rt * pa.wt_dn) {
            sched2[map_dn + wg_dn + g] = ri
            g++
        }
    }

    //! the sentinel tails of the four maps, from the live workgroup counts (the s count in the low
    //! half of each total, the m count in the high) to the dispatch bounds
    def write_tails(total_gu, total_dn : uint) {
        let tid = gl_LocalInvocationID.x
        var t = (total_gu & 0xFFFFu) + tid
        while (t < pa.bound_gu) {
            sched1[pa.map_off + t] = SCHED_NONE
            sched3[pa.map_off + t] = SCHED_NONE
            t += WG
        }
        t = (total_dn & 0xFFFFu) + tid
        while (t < pa.bound_dn) {
            sched2[pa.map_off + t] = SCHED_NONE
            t += WG
        }
        t = (total_gu >> 16u) + tid
        while (t < pa.bound_mgu) {
            sched1[pa.map_off_mgu + t] = SCHED_NONE
            sched3[pa.map_off_mgu + t] = SCHED_NONE
            t += WG
        }
        t = (total_dn >> 16u) + tid
        while (t < pa.bound_mdn) {
            sched2[pa.map_off_mdn + t] = SCHED_NONE
            t += WG
        }
    }

    [arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#moe-routing-kernels")]
    def sched_body {
        let tid = gl_LocalInvocationID.x
        let mine = tid < pa.ne
        cnt[tid] = 0u
        barrier()
        var i = tid
        while (i < pa.nk) {   //! the top-k clamps every pick under ne, so the tally stays inside cnt
            let _c = atomicAdd(cnt[idx[i]], 1u)
            i += WG
        }
        barrier()
        let c = mine ? cnt[tid] : 0u
        let pad = max(1u, ROW_PAD)
        let padded_cnt = ((c + pad - 1u) / pad) * pad
        let st = excl_scan(padded_cnt)
        let mrows = mine && !(S_ONLY_ARG && pa.s_only != 0u) ? (M_ONLY ? c : sched_ladder_m_rows(c)) : 0u
        let srows = mine ? c - mrows : 0u
        let ri_s = excl_scan(srows > 0u ? 1u : 0u)
        let ri_m = pa.ne + excl_scan(mrows > 0u ? 1u : 0u)
        let rt_s = (srows + SCHED_S_ROWS - 1u) / SCHED_S_ROWS
        let rt_m = (mrows + SCHED_M_ROWS - 1u) / SCHED_M_ROWS
        let g_gu = excl_scan(rt_s * pa.wt_gu | ((rt_m * pa.wt_gu) << 16u))
        let total_gu = scan[WG - 1u]
        let g_dn = excl_scan(rt_s * pa.wt_dn | ((rt_m * pa.wt_dn) << 16u))
        let total_dn = scan[WG - 1u]
        if (srows > 0u) {
            write_piece(ri_s, st + mrows, srows, rt_s, g_gu & 0xFFFFu, g_dn & 0xFFFFu, pa.map_off, pa.map_off)
        }
        if (mrows > 0u) {
            write_piece(ri_m, st, mrows, rt_m, g_gu >> 16u, g_dn >> 16u, pa.map_off_mgu, pa.map_off_mdn)
        }
        write_tails(total_gu, total_dn)
        barrier()   //! every thread has read its count
        cnt[tid] = st
        barrier()
        i = tid
        while (i < pa.nk) {
            let b = atomicAdd(cnt[idx[i]], 1u)
            inv[i] = b
            bex[b] = idx[i]
            i += WG
        }
    }
}

struct GatherArgs {
    n : uint    // row width
    k : uint    // slots per position (grid entry r sits at position r / k)
    r0 : uint   // this dispatch's bucket-row window [r0, r1)
    r1 : uint
}

//! the routed block's gather: every slot row's activation row into its bucket row, as halves (`XG` float16, the f16-fed tiles'
//! feed) or as floats (the Q8-fed chain requantizes the gathered rows)
[ |> template_struct_instance]
class template MoeGatherT {
    @template_constant F16 : bool = true
    @template_constant BYROW : bool = false    //! a workgroup a bucket row of the window (the per-expert chain), not a grid entry
    @ssbo @binding = 0 x : array<float>        // the normed activation rows, one per position
    @ssbo @binding = 1 @template_gate = "!BYROW" slot_idx : array<uint>  // position -> its k slot rows (the combine's map)
    @ssbo @binding = 1 @template_gate = BYROW bkt_grid : array<uint>     // bucket row -> its grid entry (the map's inverse)
    @ssbo @binding = 2 xg : array<XG>          // the gathered rows, bucket order, window-relative
    @push_constant pa : GatherArgs

    def gather_body {
        static_if (BYROW) {
            let b = gl_WorkGroupID.x
            let src = (bkt_grid[pa.r0 + b] / max(1u, pa.k)) * pa.n
            let dst = b * pa.n
            gather_row(src, dst)
        } else {
            let r = gl_WorkGroupID.x
            let b = slot_idx[r]
            if (b < pa.r0 || b >= pa.r1) {
                return
            }
            let src = (r / pa.k) * pa.n
            let dst = (b - pa.r0) * pa.n
            gather_row(src, dst)
        }
    }

    def gather_row(src, dst : uint) {
        var i = gl_LocalInvocationID.x
        while (i < pa.n) {
            static_if (F16) {
                xg[dst + i] = half_sat(x[src + i])
            } else {
                xg[dst + i] = x[src + i]
            }
            i += 256u
        }
    }
}

class MoeCmBase {
    @ssbo @binding = 2 sched : array<uint>   //! 4-word records [wblk0, row0, cnt, wg0], then the map at map_off
    @ssbo @binding = 5 y : array<float>      //! the output rows, [rows x d]
    @push_constant pa : BatchArgs

    //! the 32x32 sdot4 tiles' store: this thread's four columns of its row, each guarded at the row's end
    def store_y(xrow, cnt, row0, wt, word : uint; f : float4) {
        if (xrow < cnt) {
            let yb = (row0 + xrow) * pa.d + wt * 32u + word
            if (wt * 32u + word < pa.d) {
                y[yb] = f.x
            }
            if (wt * 32u + word + 8u < pa.d) {
                y[yb + 8u] = f.y
            }
            if (wt * 32u + word + 16u < pa.d) {
                y[yb + 16u] = f.z
            }
            if (wt * 32u + word + 24u < pa.d) {
                y[yb + 24u] = f.w
            }
        }
    }

    //! the per-wg map's word offset in `sched`: the push block's, or a stamp's own where every dispatch of it passes one
    [spirv_inline]
    def map_word : uint => pa.map_off

    def region_rec : uint4 {
        let rid = sched[map_word() + gl_WorkGroupID.x]
        if (rid == SCHED_NONE) {
            return uint4(0u, 0u, 0u, 0u)
        }
        let rb = rid * 4u
        return uint4(sched[rb], sched[rb + 1u], sched[rb + 2u], gl_WorkGroupID.x - sched[rb + 3u])
    }
}

let KHR_STRIDE = 20u   //! the KHR kq tile's row stride: 16 f16 pairs + 4 pad (banks)

let KHR_KHALF_WORDS = 8u   //! the 16 k halves one thread stages per step, as f16 pairs

struct VkQ8Blk {
    qs : int16[16]   //! one q8 block's 32 int8 quants as 16-bit lanes; scale in ws
}

struct VkK4Blk {
    qs : int16[64]   //! a Q4_K superblock's 128 nibble bytes as lanes; scale row in ws
}

struct VkQ51Blk {
    qs : int16[10]   //! one Q5_1 32-block: 16 nibble bytes (element j low, j + 16 high) then the 4 qh bytes (bit e = element e's fifth bit); d | m live in ws as one word
}

struct VkMx4Blk {
    qs : int16[8]   //! one MXFP4 32-block: 16 nibble bytes (element j low, j + 16 high, the e2m1 code); the E8M0 scale byte lives in ws, four blocks a word
}

struct VkIq4nl32Blk {
    qs : int16[8]   //! one IQ4_NL 32-block: 16 nibble bytes (element j low, j + 16 high, the codebook index); the f16 d lives in ws, two blocks a word
}

struct VkK6Blk {
    ql : int16[64]   //! one Q6_K superblock's 128 low-nibble bytes as 16-bit lanes
    qh : int16[32]   //! ... its 64 high-2-bit bytes; scales (16 int8 + f16 d) in ws
}

struct VkK5Blk {
    qs : int16[64]   //! a Q5_K superblock's 128 nibble bytes (the k/k+16 pairing)
    qh : int16[16]   //! ... its 32 5th-bit bytes: byte g*4+q, elems g*32+4q.. | +16..
}

struct VkIq3sBlk {
    qs : int16[32]   //! one IQ3_S superblock's 64 grid-index bytes as 16-bit lanes
    qh : int16[4]    //! ... its 8 ninth-bit bytes
    sg : int16[16]   //! ... its 32 sign bytes; scales (f16 d + 8 int8 (1 + 2s)) in ws
}

struct VkIq3xxsBlk {
    qs : int16[32]   //! one IQ3_XXS superblock's 64 grid-index bytes as 16-bit lanes
    ax : int16[16]   //! ... its 32 aux bytes (4 x 7-bit ksigns + 4-bit ls a block)
}

struct VkK3Blk {
    qs : int16[32]   //! a Q3_K superblock's 64 qs bytes, four 2-bit lanes, disk order
    hm : int16[16]   //! ... its 32 hmask bytes; scales (16 int8 (sc-32) + f16 d) in ws
}

struct VkK2Blk {
    qs : int16[32]   //! a Q2_K superblock's 64 qs bytes, four 2-bit lanes each
}

struct VkIq2sBlk {
    qs : int16[16]   //! one IQ2_S superblock's 32 grid-index bytes as 16-bit lanes
    sg : int16[16]   //! ... its 32 sign bytes; scales (f16 d8 + 16 (1 + 2s) strips)
    qh : int16[4]    //! ... its 8 qh bytes: two index high bits a (block, l) window
}

struct VkIq2xsBlk {
    qs : int16[32]   //! 32 u16 qs words: a 9-bit grid index + a 7-bit ksigns index
}

struct VkIq2xxsBlk {
    qs : int16[32]   //! 64B qs: a block's 4 grid byte indices + aux32 (ksigns, ls)
}

let SC_STRIDE = 130u   //! the cm2 scale cache's sub-block stride: the tile's 128 rows and two of pad

//! the cm2 tile over BN, BK and the format (BLK, ST, BLKW, decode)
[ |> template_struct_instance]
class template KqCm2BatchT : MoeCmBase {
    @ssbo @binding = 0 @role = "weight" wq : array<BLK>   //! the quant plane, read as 16-bit lanes
    @ssbo @binding = 1 @role = "weight" @template_gate = SCALES ws : array<ST>    //! the scale plane (q8: f16 per block; kq: 5 words per superblock)
    @ssbo @binding = 3 @role = "alias" xf16 : array<float16>    //! f16 activation plane (the tensor loads' view)
    @ssbo @binding = 3 @role = "alias" xf16w4 : array<uint4>    //! the same plane as 16-byte words: the KHR arm's activation stage
    @ssbo @binding = 0 @role = "alias" @template_gate = KHR @readonly wq4 : array<uint4>
    @ssbo @binding = 0 @role = "alias" @template_gate = KHR @readonly wqw : array<uint>
    @workgroup wg_blk0 : uint                             //! the region's block base, staged for the decode
    @workgroup wg_m0 : uint                               //! the tile's first weight row: the scale caches index a row by its offset from it (a partial last tile starts off the 128 grid)
    @template_constant BN : uint = 256u                   //! the token column
    @template_constant STILE : bool = false               //! the 32-row column's partial-column fast path
    @template_constant BLKW : uint = 256u                 //! elements per weight block
    @template_constant BK : uint = 64u                    //!< the k step of the tile loop (its AT/BT carry the depth)
    @template_constant UNR : uint = 4u                    //!< the k loop's hand unroll: BK x UNR is one superblock (8 on a 32-deep stamp)
    @template_constant DECVEC : bool = true               //!< when DECV4 is off: the synthesized four-wide twin rides the loads; false serves the scalar callback only
    @template_constant DECV4 : bool = false               //!< the format's hand-laid `decode_v4` is the twin instead of the synthesized one
    @template_constant IQLUT : bool = false               //! stage the iq4nl codebook into workgroup memory
    @workgroup @template_gate = IQLUT iq4lut : float16[16]   //! kvalues_iq4nl as f16
    @template_constant IQODD : bool = false               //!< under IQLUT the staged codebook is iq3s4's odd values 2q - 15
    @template_constant MX4LUT : bool = false              //!< under IQLUT the staged codebook is the doubled e2m1 magnitudes (nv4)
    @template_constant UE4LUT : bool = false              //! stage the 128 UE4M3 codes at half under the plane's per-tensor scale (nv4)
    @workgroup @template_gate = UE4LUT ue4lut : float16[128]
    @template_constant Q50 : bool = false   //!< q51's block under one d: d * (q - 16), q5032
    @template_constant SCGRID : bool = false              //!< under SCIQ4 the two-word row's sub-scale is the iq3 row's 1 + 2s (`grid_sc`), not iq4xs's ls - 32
    @template_constant IQ3GRID : bool = false             //! stage the 2 KB iq3s grid into workgroup memory
    @workgroup @template_gate = IQ3GRID iq3s_gridc : uint[512]
    @template_constant IQ3XGRID : bool = false            //! stage the 1 KB halved iq3xxs grid into workgroup memory
    @workgroup @template_gate = IQ3XGRID iq3x_gridc : uint[256]
    @template_constant IQ2SGRID : bool = false            //! stage the 8 KB iq2s u64 grid into workgroup memory
    @workgroup @template_gate = IQ2SGRID iq2s_gridc : uint[2048]
    @template_constant IQ2XSGRID : bool = false           //! stage the 4 KB iq2xs u64 grid into workgroup memory
    @workgroup @template_gate = IQ2XSGRID iq2xs_gridc : uint[1024]
    @template_constant IQ2XXSGRID : bool = false          //! stage the 2 KB iq2xxs u64 grid into workgroup memory
    @workgroup @template_gate = IQ2XXSGRID iq2xxs_gridc : uint[512]
    @template_constant SCALES : bool = true               //!< the format reads a scale plane at binding 1 (a file's f32 rows carry none)
    @template_constant KHR : bool = false                 //!< the KHR cooperative-matrix arm: hand-staged f16 tiles over the format's own decode, no cm2 API (a card without NV_coopmat2)
    @workgroup @template_gate = KHR khr_ao : uint[int(128u * KHR_STRIDE)]   //! the A stage: 128 weight rows of f16 pairs; then the edge tile's f32 store bounce, 8 subgroups x 256 words
    @workgroup @template_gate = KHR khr_b : uint[int(128u * KHR_STRIDE)]    //! the B stage: BN (128) token rows of f16 pairs
    @template_constant SCACHE : bool = false              //!< the scale cache: the tile's 128 rows' eight sub-block scale pairs staged per superblock
    @template_constant SCIQ4 : bool = false               //!< the cache's fill reads the IQ4_XS two-word scale row (d | the high scale bits, the low nibbles) instead of the K-quant five
    @template_constant SCK6 : bool = false                //!< the cache's fill reads the Q6_K row (sixteen int8 sub-block scales and d) into sixteen d x sc entries instead of the K-quant five's eight pairs
    @template_constant SCPAIR : bool = false              //!< the cache is the eight-pair form (every SCACHE format but k6): the pair array exists only then - the gate takes one name, and an unused 16 KB cache costs k6 a block an SM
    @workgroup @template_gate = SCPAIR sc_cache : float2[int(8u * SC_STRIDE)]    //! sub-block-major, row-minor, two rows of pad per sub-block against bank conflicts
    @workgroup @template_gate = SCK6 sc6_cache : float[int(16u * SC_STRIDE)]     //! k6's sixteen d x sc entries a superblock, the same layout, no minuend
    @ssbo @binding = 1 @role = "alias" @template_gate = SCACHE ws_words : array<uint>   //! the scale plane as words, the cache's source (K-quant: five per superblock - d | dmin, eight scale bytes, eight min bytes; IQ4_XS: two)

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-tile-body")]
    def cm2_body {
        //! the tile over its region record; a workgroup past the schedule's end (an empty record) does nothing
        let reg = region_rec()
        if (reg.z != 0u) {
            cm2_region(reg)
        }
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-tile-body")]
    def cm2_region(reg : uint4) {
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let ttiles = (cnt + BN - 1u) / BN
        var tix = reg.w
        var ks = 0u
        var k1 = pa.n
        var ybase = 0u
        if (pa.ksplit != 0u) {
            let ptiles = ((pa.d + 127u) / 128u) * ttiles
            ks = tix / ptiles
            tix -= ks * ptiles
            k1 = min(pa.n, (ks + 1u) * pa.ksplit)
            ybase = ks * (row0 + cnt) * pa.d
        }
        let k0 = (ks * pa.ksplit) & ~(BK * UNR - 1u)
        let tok_tile = tix % ttiles
        let w_tile = tix / ttiles
        if (gl_LocalInvocationID.x == 0u) {
            wg_blk0 = wblk0
        }
        static_if (IQLUT) {
            if (gl_LocalInvocationID.x < 16u) {
                let li = gl_LocalInvocationID.x
                static_if (IQODD) {
                    iq4lut[li] = float16(float(int(li) * 2 - 15))
                } else {
                    static_if (MX4LUT) {
                        let tbl = uint4(MX4_BYTES_LO, MX4_BYTES_HI, MX4_NEG_BYTES_LO, MX4_NEG_BYTES_HI)
                        iq4lut[li] = float16(float((int((tbl[int(li >> 2u)] >> ((li & 3u) * 8u)) & 255u) ^ 128) - 128))
                    } else {
                        let tbl = uint4(3215825025u, 4142587343u, 639175937u, 1901675829u)
                        iq4lut[li] = float16(float((int((tbl[int(li >> 2u)] >> ((li & 3u) * 8u)) & 255u) ^ 128) - 128))
                    }
                }
            }
        }
        static_if (UE4LUT) {
            if (gl_LocalInvocationID.x < 128u) {
                let code = gl_LocalInvocationID.x
                ue4lut[code] = float16(ue4m3_half_f32(code) * uint_bits_to_float(ws[wblk0 * 5u + 4u]))
            }
        }
        static_if (IQ3GRID) {
            let gi = gl_LocalInvocationID.x
            iq3s_gridc[gi] = iq3s_grid_word(int(gi))
            iq3s_gridc[gi + 256u] = iq3s_grid_word(int(gi + 256u))
        }
        static_if (IQ3XGRID) {
            iq3x_gridc[gl_LocalInvocationID.x] = iq3xxs_grid_word(int(gl_LocalInvocationID.x))
        }
        static_if (IQ2SGRID) {
            let g2 = gl_LocalInvocationID.x
            for (k in range(8)) {
                iq2s_gridc[g2 + uint(k) * 256u] = iq2s_grid_word(int(g2 + uint(k) * 256u))
            }
        }
        static_if (IQ2XSGRID) {
            let g2x = gl_LocalInvocationID.x
            for (k in range(4)) {
                iq2xs_gridc[g2x + uint(k) * 256u] = iq2xs_grid_word(int(g2x + uint(k) * 256u))
            }
        }
        static_if (IQ2XXSGRID) {
            let g2xx = gl_LocalInvocationID.x
            for (k in range(2)) {
                iq2xxs_gridc[g2xx + uint(k) * 256u] = iq2xxs_grid_word(int(g2xx + uint(k) * 256u))
            }
        }
        barrier()   // wg_blk0 and the gated codebook visible before the first decode
        static_if (KHR) {
            khr_tile(wblk0, row0, cnt, k0, k1, ybase, tok_tile, w_tile)
        } else {
            cm2_tile(wblk0, row0, cnt, k0, k1, ybase, tok_tile, w_tile)
        }
    }

    //! a two-word scale row's sub-block scale: iq4xs's signed ls - 32, or the iq3 row's 1 + 2s under `SCGRID`
    def iq_sub_sc(w0, w1, blk : uint) : int {
        static_if (SCGRID) {
            return grid_sc(w1, blk)
        } else {
            return iq4xs_sc(w0, w1, blk)
        }
    }

    //! the scale cache for superblock column `sb` of the tile's rows from m0; a row past the plane reads the last row (its entries unused)
    def sc_fill(m0, wblk0, sb : uint) {
        static_if (SCACHE) {
            let t = gl_LocalInvocationID.x
            let r = t >> 1u
            let g0 = (t & 1u) * 4u
            let row = min(m0 + r, pa.d - 1u)
            static_if (SCIQ4) {
                let srow = (wblk0 + row * (pa.n >> 8u) + sb) * 2u
                let w0 = ws_words[srow]
                let w1 = ws_words[srow + 1u]
                let d = unpackHalf2x16(w0).x
                for [unroll] (i in range(4)) {
                    sc_cache[(g0 + uint(i)) * SC_STRIDE + r] = float2(d * float(iq_sub_sc(w0, w1, g0 + uint(i))), 0.0)
                }
            } static_elif (SCK6) {
                // the Q6_K row: words 0-3 the sixteen int8 sub-block scales, word 4 the f16 d; a thread fills eight sub-blocks of its row from two words
                let srow = (wblk0 + row * (pa.n >> 8u) + sb) * 5u
                let d = unpackHalf2x16(ws_words[srow + 4u]).x
                let s0 = g0 * 2u
                let w0 = ws_words[srow + (s0 >> 2u)]
                let w1 = ws_words[srow + (s0 >> 2u) + 1u]
                for [unroll] (i in range(8)) {
                    let w = i < 4 ? w0 : w1
                    sc6_cache[(s0 + uint(i)) * SC_STRIDE + r] = d * float(k6_sc(w, uint(i)))
                }
            } else {
                let srow = (wblk0 + row * (pa.n >> 8u) + sb) * 5u
                let dm = unpackHalf2x16(ws_words[srow])
                let scw = ws_words[srow + 1u + (g0 >> 2u)]
                let mnw = ws_words[srow + 3u + (g0 >> 2u)]
                for [unroll] (i in range(4)) {
                    sc_cache[(g0 + uint(i)) * SC_STRIDE + r] = k4_scale_min(dm, scw, mnw, uint(i))
                }
            }
        }
    }

    [arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def sc_step(m0, wblk0, k : uint) {
        static_if (SCACHE) {
            if ((k & (BLKW - 1u)) == 0u) {
                barrier()
                sc_fill(m0, wblk0, k / BLKW)
                barrier()
            }
        }
    }

    def cm2_tile(wblk0, row0, cnt, k0, k1, ybase, tok_tile, w_tile : uint) {   // nolint:STYLE037,STYLE038 — the fast/edge path pair, barrier- and register-coupled
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var a : AT
        var b : BT
        var acc : ACC
        let t0 = row0 + tok_tile * BN
        // a partial last weight tile takes the plane's last whole 128 rows, its overlap rewriting the same values
        let m0 = w_tile * 128u + 128u <= pa.d || pa.d < 128u ? w_tile * 128u : pa.d - 128u
        if (gl_LocalInvocationID.x == 0u) {
            wg_m0 = m0   // visible past the scale cache's first barrier, ahead of every decode
        }
        if (m0 + 128u <= pa.d && (STILE || tok_tile * BN + BN <= cnt) && (pa.n & (BK - 1u)) == 0u) {
            var fla : tensorLayout2D
            tensorLayoutCreate(fla)
            tensorLayoutSetBlockSize(fla, 1u, BLKW)
            tensorLayoutSetDimension(fla, pa.d, pa.n)
            tensorLayoutSetStride(fla, pa.n / BLKW, 1u)
            var flb : tensorLayout2D
            tensorLayoutCreate(flb)
            tensorLayoutSetDimension(flb, row0 + (STILE ? ((cnt + BN - 1u) & ~(BN - 1u)) : cnt), pa.n)
            tensorLayoutSetStride(flb, pa.n & ~7u, 1u)
            let whole = tok_tile * BN + BN <= cnt
            var flw : tensorLayout2D
            tensorLayoutCreate(flw)
            tensorLayoutSetDimension(flw, row0 + cnt, pa.d)
            tensorLayoutSetStride(flw, pa.d & ~7u, 1u)
            var flp : tensorLayout2DPad
            tensorLayoutCreate(flp)
            tensorLayoutSetDimension(flp, row0 + cnt, pa.d)
            tensorLayoutSetStride(flp, pa.d & ~7u, 1u)
            if (pa.ksplit == 0u) {
                var k = 0u
                for (_i in range(int(pa.n / (BK * UNR)))) {
                    sc_step(m0, wblk0, k)
                    for [unroll] (_j in range(int(UNR))) {
                        static_if (DECV4) {
                            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                        } else {
                            coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                        }
                        coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                        acc = coopmatMulAdd(a, b, acc)
                        k += BK
                    }
                }
                while (k < pa.n) {
                    sc_step(m0, wblk0, k)
                    static_if (DECV4) {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                    } else {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                    }
                    coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += BK
                }
                coopmatClamp(acc, -65504.0, 65504.0)
                var accw : ACCW
                coopmatConvert(accw, acc)
                if (whole) {
                    coopmatStoreTensor(accw, y, 0u, flw, t0, BN, m0, 128u, tv)
                } else {
                    coopmatStoreTensor(accw, y, 0u, flp, t0, BN, m0, 128u, tv)
                }
                return
            }
            var k = k0
            for (_i in range(int((k1 - k0) / (BK * UNR)))) {
                sc_step(m0, wblk0, k)
                for [unroll] (_j in range(int(UNR))) {
                    static_if (DECV4) {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                    } else {
                        coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                    }
                    coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += BK
                }
            }
            while (k < k1) {
                sc_step(m0, wblk0, k)
                static_if (DECV4) {
                    coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, self.decode_v4)
                } else {
                    coopmatLoadTensorDecode(a, wq, wblk0, fla, m0, 128u, k, BK, self.decode, DECVEC)
                }
                coopmatLoadTensor(b, xf16, 0u, flb, t0, BN, k, BK, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += BK
            }
            coopmatClamp(acc, -65504.0, 65504.0)
            var accw : ACCW
            coopmatConvert(accw, acc)
            if (whole) {
                coopmatStoreTensor(accw, y, ybase, flw, t0, BN, m0, 128u, tv)
            } else {
                coopmatStoreTensor(accw, y, ybase, flp, t0, BN, m0, 128u, tv)
            }
            return
        }
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetBlockSize(tla, 1u, BLKW)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n / BLKW, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, row0 + cnt, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, row0 + cnt, pa.d)
        tensorLayoutSetStride(tlo, pa.d, 1u)
        if (pa.ksplit == 0u) {
            var k = 0u
            for (_i in range(int(pa.n / (BK * UNR)))) {
                sc_step(m0, wblk0, k)
                for [unroll] (_j in range(int(UNR))) {
                    coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
                    coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
                    acc = coopmatMulAdd(a, b, acc)
                    k += BK
                }
            }
            while (k < pa.n) {
                sc_step(m0, wblk0, k)
                coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
                coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += BK
            }
            coopmatClamp(acc, -65504.0, 65504.0)
            var accw : ACCW
            coopmatConvert(accw, acc)
            coopmatStoreTensor(accw, y, 0u, tlo, t0, BN, m0, 128u, tv)
            return
        }
        var k = k0
        for (_i in range(int((k1 - k0) / (BK * UNR)))) {
            sc_step(m0, wblk0, k)
            for [unroll] (_j in range(int(UNR))) {
                coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
                coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
                acc = coopmatMulAdd(a, b, acc)
                k += BK
            }
        }
        while (k < k1) {
            sc_step(m0, wblk0, k)
            coopmatLoadTensorDecode(a, wq, wblk0, tla, m0, 128u, k, BK, self.decode)
            coopmatLoadTensor(b, xf16, 0u, tlb, t0, BN, k, BK, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += BK
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : ACCW
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, y, ybase, tlo, t0, BN, m0, 128u, tv)
    }

    //! a KHR stamp's format writes weight row `blk`'s 16 values e0 .. e0 + 16 into khr_ao[sbase .. sbase + 8) as f16 pairs, read from the plane's words; the cm2 stamps never call it
    def abstract khr_stage16(blk, e0, sbase : uint) : void

    //! four decoded weights into the KHR stage as two f16 pairs at `at`: (x, y) there, (z, w) at `at + 1`
    def khr_put4(at : uint; v : float4) {
        static_if (KHR) {
            khr_ao[at] = packHalf2x16(v.xy)
            khr_ao[at + 1u] = packHalf2x16(v.zw)
        }
    }

    [arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile")]
    def khr_tile(wblk0, row0, cnt, k0, k1, ybase, tok_tile, w_tile : uint) {   // nolint:STYLE038 — the staged k loop and its fast/edge store are one register set
        static_if (KHR) {   // the gated staging arrays exist on the KHR instantiations alone
        let t0 = row0 + tok_tile * BN
        let m0 = w_tile * 128u
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let srow = tid >> 1u
        let khalf = tid & 1u   // the low or the high 16 k of the step
        let skh = khalf * 16u
        let nbk = pa.n / BLKW
        let wrow = m0 + srow
        let trow = tok_tile * BN + srow
        let sbase = srow * KHR_STRIDE + khalf * KHR_KHALF_WORDS
        let wrow_in_d = wrow < pa.d
        let trow_in_cnt = trow < cnt
        let wrow0 = (sg & 1u) * 64u
        let trow0 = (sg >> 1u) * 32u
        var acc : coopmatAcc_f32_16x16[8]   // [r * 2 + c]: weight fragment r against token fragment c; f32, since an f16 accumulator rounds toward zero on RDNA3's matrix cores
        var k = k0
        while (k < k1) {
            let kk = k + skh
            if (wrow_in_d && kk < k1) {
                let bcol = kk / BLKW
                khr_stage16(wblk0 + wrow * nbk + bcol, kk - bcol * BLKW, sbase)
            } else {
                for (j in range(int(KHR_KHALF_WORDS))) {
                    khr_ao[sbase + uint(j)] = 0u
                }
            }
            if (trow_in_cnt && kk < k1) {
                let q = ((t0 + srow) * pa.n + kk) >> 3u   // the 16 halves as two 16-byte words
                for (h in range(2)) {
                    let w4 = xf16w4[q + uint(h)]
                    for (c in range(4)) {
                        khr_b[sbase + uint(h) * 4u + uint(c)] = w4[c]
                    }
                }
            } else {
                for (j in range(int(KHR_KHALF_WORDS))) {
                    khr_b[sbase + uint(j)] = 0u
                }
            }
            barrier()
            for [unroll_full] (ks in range(2)) {
                let ko = uint(ks) * 8u
                var a : coopmatA_f16_16x16[4]
                for [unroll_full] (r in range(4)) {
                    coopmatLoad(a[r], khr_ao, int((wrow0 + uint(r) * 16u) * KHR_STRIDE + ko), int(KHR_STRIDE), 0)
                }
                for [unroll_full] (c in range(2)) {
                    var b : coopmatB_f16_16x16
                    coopmatLoad(b, khr_b, int((trow0 + uint(c) * 16u) * KHR_STRIDE + ko), int(KHR_STRIDE), 1)
                    for [unroll_full] (r in range(4)) {
                        acc[r * 2 + c] = coopmatMulAdd(a[r], b, acc[r * 2 + c])
                    }
                }
            }
            barrier()
            k += 32u
        }
        let tile_whole = m0 + 128u <= pa.d && tok_tile * BN + BN <= cnt
        if (tile_whole) {
            let yb = ybase + (t0 + trow0) * pa.d + m0 + wrow0
            for [unroll_full] (r in range(4)) {
                for [unroll_full] (c in range(2)) {
                    coopmatStore(acc[r * 2 + c], y, int(yb + uint(c) * 16u * pa.d + uint(r) * 16u), int(pa.d), 1)
                }
            }
        } else {
            let lane = gl_SubgroupInvocationID
            let ob = sg * 256u
            for [unroll_full] (r in range(4)) {
                for [unroll_full] (c in range(2)) {
                    coopmatStore(acc[r * 2 + c], khr_ao, int(ob), 16, 0)
                    barrier()
                    for (e in range(8)) {
                        let idx = lane * 8u + uint(e)
                        let mr = m0 + wrow0 + uint(r) * 16u + idx / 16u
                        let tc = tok_tile * BN + trow0 + uint(c) * 16u + idx % 16u
                        if (mr < pa.d && tc < cnt) {
                            y[ybase + (row0 + tc) * pa.d + mr] = uint_bits_to_float(khr_ao[ob + idx])
                        }
                    }
                    barrier()
                }
            }
        }
        }
    }
}

[ |> template_struct_instance]
class template Q8Cm2T : KqCm2BatchT {
    typedef BLK = VkQ8Blk
    typedef ST = float16
    override BLKW = 32u

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkQ8Blk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]   // the byte2 select: the shift form reads 1.5% under on the served Q8_0 prefill
        return ws[wg_blk0 + bc.x * (pa.n >> 5u) + bc.y] * float16(float(int(q)))
    }

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let w4 = wq4[blk * 2u + (e0 >> 4u)]   // the block's low or high 16 weights, four words of int8
            let d = float(ws[blk])
            for [unroll_full] (c in range(4)) {
                let q = float4(int4(unpack8(int(w4[c])))) * d
                khr_put4(sbase + uint(c) * 2u, q)
            }
        }
    }
}

[ |> template_struct_instance]
class template K4Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk
    typedef ST = uint
    override DECV4 = true
    override SCACHE = true
    override SCPAIR = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let sh = ((e0 >> 4u) & 1u) * 4u   // the low or the high nibbles of sub-block g's 16 bytes
            let w4 = wq4[blk * 8u + g]
            static_if (IQLUT) {
                let srow = blk * 2u
                let w0 = ws[srow]
                let ds = unpackHalf2x16(w0).x * float(iq_sub_sc(w0, ws[srow + 1u], g))
                for [unroll_full] (c in range(4)) {
                    let w = w4[c] >> sh
                    khr_put4(sbase + uint(c) * 2u, float4(ds * float(iq4lut[int(w & 0xFu)]), ds * float(iq4lut[int((w >> 8u) & 0xFu)]),
                        ds * float(iq4lut[int((w >> 16u) & 0xFu)]), ds * float(iq4lut[int((w >> 24u) & 0xFu)])))
                }
            } else {
                let srow = blk * 5u
                let sm = k4_scale_min(unpackHalf2x16(ws[srow]), ws[srow + 1u + (g >> 2u)], ws[srow + 3u + (g >> 2u)], g)
                for [unroll_full] (c in range(4)) {
                    let w = w4[c] >> sh
                    khr_put4(sbase + uint(c) * 2u, float4(sm.x * float(w & 0xFu) - sm.y, sm.x * float((w >> 8u) & 0xFu) - sm.y,
                        sm.x * float((w >> 16u) & 0xFu) - sm.y, sm.x * float((w >> 24u) & 0xFu) - sm.y))
                }
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let q = (uint(int(blk.qs[int(bidx >> 1u)])) >> ((bidx & 1u) * 8u + (e >> 4u) * 4u)) & 0xFu
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        static_if (IQLUT) {
            return float16(sc_pair.x * float(iq4lut[int(q)]))
        } else {
            return float16(sc_pair.x * float(q) - sc_pair.y)
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        static_if (IQLUT) {
            let ds = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)].x
            return half4(float4(ds * float(iq4lut[int((l0 >> sh) & 0xFu)]), ds * float(iq4lut[int((l0 >> (sh + 8u)) & 0xFu)]),
                ds * float(iq4lut[int((l1 >> sh) & 0xFu)]), ds * float(iq4lut[int((l1 >> (sh + 8u)) & 0xFu)])))
        } else {
            let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
            return half4(float4(sc_pair.x * float((l0 >> sh) & 0xFu) - sc_pair.y, sc_pair.x * float((l0 >> (sh + 8u)) & 0xFu) - sc_pair.y,
                sc_pair.x * float((l1 >> sh) & 0xFu) - sc_pair.y, sc_pair.x * float((l1 >> (sh + 8u)) & 0xFu) - sc_pair.y))
        }
    }
}

[ |> template_struct_instance]
class template K6Cm2T : KqCm2BatchT {
    typedef BLK = VkK6Blk
    typedef ST = uint
    override DECV4 = true
    override SCACHE = true
    override SCK6 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let bu = e0 >> 5u
            let hh = (e0 >> 4u) & 1u
            let lw4 = wq4[blk * 12u + bu]                            // the low-nibble bytes bu * 16 + j
            let hw4 = wq4[blk * 12u + 8u + (bu >> 2u) * 2u + hh]   // the high-2-bit bytes (bu >> 2) * 32 + hh * 16 + j
            let ls = hh * 4u
            let hs = (bu & 3u) * 2u
            let srow = blk * 5u
            let sidx = e0 >> 4u
            let ds = unpackHalf2x16(ws[srow + 4u]).x * float(k6_sc(ws[srow + (sidx >> 2u)], sidx))
            for [unroll_full] (c in range(4)) {
                let lw = lw4[c] >> ls
                let hw = hw4[c] >> hs
                let q0 = int((lw & 0xFu) | ((hw & 3u) << 4u)) - 32
                let q1 = int(((lw >> 8u) & 0xFu) | (((hw >> 8u) & 3u) << 4u)) - 32
                let q2 = int(((lw >> 16u) & 0xFu) | (((hw >> 16u) & 3u) << 4u)) - 32
                let q3 = int(((lw >> 24u) & 0xFu) | (((hw >> 24u) & 3u) << 4u)) - 32
                khr_put4(sbase + uint(c) * 2u, float4(ds * float(q0), ds * float(q1), ds * float(q2), ds * float(q3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK6Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u   // the pair's two low bytes are one 16-bit lane, its high bits two of one byte
        let bu = pair0 >> 5u
        let hh = (pair0 >> 4u) & 1u
        let j = pair0 & 15u
        let lb = bu * 16u + j
        let lw = uint(int(blk.ql[int(lb >> 1u)])) & 0xFFFFu
        let qb = (bu >> 2u) * 32u + hh * 16u + j
        let hw = uint(int(blk.qh[int(qb >> 1u)])) & 0xFFFFu
        let q2 = ((lw >> (hh * 4u)) & 0x0F0Fu) | (((hw >> ((bu & 3u) * 2u)) & 0x0303u) << 4u)
        let q6 = int((q2 >> ((e & 1u) * 8u)) & 0xFFu) - 32
        let sidx = pair0 >> 4u
        var ds : float
        // the Q6_K scale row: byte `sidx` of words 0-3, the f16 d in word 4
        static_if (SCK6) {
            ds = sc6_cache[sidx * SC_STRIDE + (bc.x - wg_m0)]
        } else {
            let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
            let sc = k6_sc(ws[srow + (sidx >> 2u)], sidx)
            ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
        }
        return float16(ds * float(q6))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK6Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let bu = e >> 5u
        let hh = (e >> 4u) & 1u
        let j = e & 15u
        let li = int((bu * 16u + j) >> 1u)
        let l0 = uint(int(blk.ql[li])) & 0xFFFFu
        let l1 = uint(int(blk.ql[li + 1])) & 0xFFFFu
        let hi = int(((bu >> 2u) * 32u + hh * 16u + j) >> 1u)
        let h0 = uint(int(blk.qh[hi])) & 0xFFFFu
        let h1 = uint(int(blk.qh[hi + 1])) & 0xFFFFu
        let ls = hh * 4u
        let hs = (bu & 3u) * 2u
        let p0 = ((l0 >> ls) & 0x0F0Fu) | (((h0 >> hs) & 0x0303u) << 4u)
        let p1 = ((l1 >> ls) & 0x0F0Fu) | (((h1 >> hs) & 0x0303u) << 4u)
        let sidx = e >> 4u
        var ds : float
        static_if (SCK6) {
            ds = sc6_cache[sidx * SC_STRIDE + (bc.x - wg_m0)]
        } else {
            let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
            let sc = k6_sc(ws[srow + (sidx >> 2u)], sidx)
            ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
        }
        return half4(float4(ds * float(int(p0 & 0xFFu) - 32), ds * float(int(p0 >> 8u) - 32), ds * float(int(p1 & 0xFFu) - 32), ds * float(int(p1 >> 8u) - 32)))
    }
}

[ |> template_struct_instance]
class template K5Cm2T : KqCm2BatchT {
    typedef BLK = VkK5Blk
    typedef ST = uint
    override DECV4 = true
    override SCACHE = true
    override SCPAIR = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let h = (e0 >> 4u) & 1u
            let sh = h * 4u
            let w4 = wq4[blk * 10u + g]
            let hw = wq4[blk * 10u + 8u + (g >> 2u)][int(g & 3u)] >> sh   // sub-block g's four 5th-bit bytes; byte c holds j = 4c .. 4c + 3 at bit (j & 3) + h * 4
            let srow = blk * 5u
            let sm = k4_scale_min(unpackHalf2x16(ws[srow]), ws[srow + 1u + (g >> 2u)], ws[srow + 3u + (g >> 2u)], g)
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                let hb = hw >> (uint(c) * 8u)
                let q0 = (w & 0xFu) | ((hb & 1u) << 4u)
                let q1 = ((w >> 8u) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
                let q2 = ((w >> 16u) & 0xFu) | (((hb >> 2u) & 1u) << 4u)
                let q3 = ((w >> 24u) & 0xFu) | (((hb >> 3u) & 1u) << 4u)
                khr_put4(sbase + uint(c) * 2u, float4(sm.x * float(q0) - sm.y, sm.x * float(q1) - sm.y, sm.x * float(q2) - sm.y, sm.x * float(q3) - sm.y))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK5Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let pair0 = e & ~1u   // the pair's two quant bytes are one 16-bit lane, its high bits two of one byte
        let j = pair0 & 15u
        let h = pair0 >> 4u
        let bidx = g * 16u + j
        let w = uint(int(blk.qs[int(bidx >> 1u)])) & 0xFFFFu
        let hidx = g * 4u + (j >> 2u)
        let hb = ((uint(int(blk.qh[int(hidx >> 1u)])) & 0xFFFFu) >> ((hidx & 1u) * 8u)) >> ((j & 3u) + h * 4u)
        let q0 = ((w >> (h * 4u)) & 0xFu) | ((hb & 1u) << 4u)
        let q1 = ((w >> (h * 4u + 8u)) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return float16((e & 1u) != 0u ? sc_pair.x * float(q1) - sc_pair.y : sc_pair.x * float(q0) - sc_pair.y)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK5Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let j = e & 15u
        let li = int((g * 16u + j) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let hidx = g * 4u + (j >> 2u)
        let hby = ((uint(int(blk.qh[int(hidx >> 1u)])) & 0xFFFFu) >> ((hidx & 1u) * 8u)) & 0xFFu
        let sh = (e >> 4u) * 4u
        let hb = hby >> sh
        let q0 = ((l0 >> sh) & 0xFu) | ((hb & 1u) << 4u)
        let q1 = ((l0 >> (sh + 8u)) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
        let q2 = ((l1 >> sh) & 0xFu) | (((hb >> 2u) & 1u) << 4u)
        let q3 = ((l1 >> (sh + 8u)) & 0xFu) | (((hb >> 3u) & 1u) << 4u)
        let sc_pair = sc_cache[g * SC_STRIDE + (bc.x - wg_m0)]
        return half4(float4(sc_pair.x * float(q0) - sc_pair.y, sc_pair.x * float(q1) - sc_pair.y, sc_pair.x * float(q2) - sc_pair.y, sc_pair.x * float(q3) - sc_pair.y))
    }
}

[ |> template_struct_instance]
class template Q40Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk   //! the k4 128-nibble lane view: q40 rows land in the k/k+16 pairing
    typedef ST = uint
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let sh = ((e0 >> 4u) & 1u) * 4u
            let w4 = wq4[blk * 8u + g]
            let d = q40_d_of(ws[blk * 5u + (g >> 1u)], g)
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                static_if (IQLUT) {
                    khr_put4(sbase + uint(c) * 2u, float4(d * float(iq4lut[int(w & 0xFu)]), d * float(iq4lut[int((w >> 8u) & 0xFu)]),
                        d * float(iq4lut[int((w >> 16u) & 0xFu)]), d * float(iq4lut[int((w >> 24u) & 0xFu)])))
                } else {
                    khr_put4(sbase + uint(c) * 2u, float4(d * (float(w & 0xFu) - 8.0), d * (float((w >> 8u) & 0xFu) - 8.0),
                        d * (float((w >> 16u) & 0xFu) - 8.0), d * (float((w >> 24u) & 0xFu) - 8.0)))
                }
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let q = (uint(int(blk.qs[int(bidx >> 1u)])) >> ((bidx & 1u) * 8u + (e >> 4u) * 4u)) & 0xFu
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = q40_d_of(ws[srow + (g >> 1u)], g)
        static_if (IQLUT) {
            return float16(d * float(iq4lut[int(q)]))
        } else {
            return float16(d * (float(q) - 8.0))
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = q40_d_of(ws[srow + (g >> 1u)], g)
        static_if (IQLUT) {
            return half4(float4(d * float(iq4lut[int((l0 >> sh) & 0xFu)]), d * float(iq4lut[int((l0 >> (sh + 8u)) & 0xFu)]),
                d * float(iq4lut[int((l1 >> sh) & 0xFu)]), d * float(iq4lut[int((l1 >> (sh + 8u)) & 0xFu)])))
        } else {
            return half4(float4(d * (float((l0 >> sh) & 0xFu) - 8.0), d * (float((l0 >> (sh + 8u)) & 0xFu) - 8.0),
                d * (float((l1 >> sh) & 0xFu) - 8.0), d * (float((l1 >> (sh + 8u)) & 0xFu) - 8.0)))
        }
    }
}

//! Q5_1 on the cm2 tile: the per-32 plane verbatim (the CPU's `dot_q51q8_scalar` layout), w = d * q + m with
//! q = nibble | qh bit << 4; the scale word is f16 d in the low half, f16 m in the high.
//! Under `Q50` (q5032) one f16 d a block, two blocks a word: w = d * (q - 16)
[ |> template_struct_instance]
class template Q51Cm2T : KqCm2BatchT {
    typedef BLK = VkQ51Blk
    typedef ST = uint
    override BLKW = 32u
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {   // the 20-byte block off its words: four nibble words then the qh word, the low or high nibbles by e0
            let sh = (e0 >> 4u) * 4u
            let qw = Q50 ? KQ_Q5032_QWORDS : KQ_Q51_QWORDS
            let qh = wqw[blk * qw + KQ_B32_QWORDS] >> e0
            for [unroll_full] (c in range(4)) {
                let w = wqw[blk * qw + uint(c)] >> sh
                let hb = qh >> (uint(c) * 4u)
                let u0 = (w & 0xFu) | ((hb & 1u) << 4u)
                let u1 = ((w >> 8u) & 0xFu) | (((hb >> 1u) & 1u) << 4u)
                let u2 = ((w >> 16u) & 0xFu) | (((hb >> 2u) & 1u) << 4u)
                let u3 = ((w >> 24u) & 0xFu) | (((hb >> 3u) & 1u) << 4u)
                static_if (Q50) {
                    let d = q40_d_of(ws[blk >> 1u], blk)
                    khr_put4(sbase + uint(c) * 2u, float4(d * float(int(u0) - 16),
                        d * float(int(u1) - 16), d * float(int(u2) - 16), d * float(int(u3) - 16)))
                } else {
                    let dm = unpackHalf2x16(ws[blk])
                    khr_put4(sbase + uint(c) * 2u, float4(dm.x * float(u0) + dm.y,
                        dm.x * float(u1) + dm.y, dm.x * float(u2) + dm.y, dm.x * float(u3) + dm.y))
                }
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkQ51Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let j = e & 15u
        let by = ((uint(int(blk.qs[int(j >> 1u)])) & 0xFFFFu) >> ((j & 1u) * 8u)) & 0xFFu
        let qh = (uint(int(blk.qs[8])) & 0xFFFFu) | ((uint(int(blk.qs[9])) & 0xFFFFu) << 16u)
        let q = ((by >> ((e >> 4u) * 4u)) & 0xFu) | (((qh >> e) & 1u) << 4u)
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        static_if (Q50) {
            let d = q40_d_of(ws[idx >> 1u], idx)
            return float16(d * float(int(q) - 16))
        } else {
            let dm = unpackHalf2x16(ws[idx])
            return float16(dm.x * float(q) + dm.y)
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkQ51Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let li = int((e & 15u) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let qh = ((uint(int(blk.qs[8])) & 0xFFFFu) | ((uint(int(blk.qs[9])) & 0xFFFFu) << 16u)) >> e
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let u0 = ((l0 >> sh) & 0xFu) | ((qh & 1u) << 4u)
        let u1 = ((l0 >> (sh + 8u)) & 0xFu) | (((qh >> 1u) & 1u) << 4u)
        let u2 = ((l1 >> sh) & 0xFu) | (((qh >> 2u) & 1u) << 4u)
        let u3 = ((l1 >> (sh + 8u)) & 0xFu) | (((qh >> 3u) & 1u) << 4u)
        static_if (Q50) {
            let d = q40_d_of(ws[idx >> 1u], idx)
            return half4(float4(d * float(int(u0) - 16), d * float(int(u1) - 16),
                d * float(int(u2) - 16), d * float(int(u3) - 16)))
        } else {
            let dm = unpackHalf2x16(ws[idx])
            return half4(float4(dm.x * float(u0) + dm.y, dm.x * float(u1) + dm.y,
                dm.x * float(u2) + dm.y, dm.x * float(u3) + dm.y))
        }
    }
}

//! Q4_0 per-32 on the cm2 tile: the per-32 plane verbatim (one f16 d a block, two blocks a word),
//! w = d * (q - 8); under `IQLUT` (iq4nl32, the CPU's `dot_iq4nl32q8_scalar` layout) w = d * LUT[q]
//! off the staged codebook
[ |> template_struct_instance]
class template Q4032Cm2T : KqCm2BatchT {
    typedef BLK = VkIq4nl32Blk
    typedef ST = uint
    override BLKW = 32u
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {   // the block's 16 nibble bytes as one word4: the low nibbles are elements 0..15, the high 16..31
            let sh = (e0 >> 4u) * 4u
            let w4 = wq4[blk]
            let d = q40_d_of(ws[blk >> 1u], blk)
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> sh
                static_if (IQLUT) {
                    khr_put4(sbase + uint(c) * 2u, float4(d * float(iq4lut[int(w & 0xFu)]),
                        d * float(iq4lut[int((w >> 8u) & 0xFu)]),
                        d * float(iq4lut[int((w >> 16u) & 0xFu)]),
                        d * float(iq4lut[int((w >> 24u) & 0xFu)])))
                } else {
                    khr_put4(sbase + uint(c) * 2u, float4(d * float(int(w & 0xFu) - 8),
                        d * float(int((w >> 8u) & 0xFu) - 8),
                        d * float(int((w >> 16u) & 0xFu) - 8),
                        d * float(int((w >> 24u) & 0xFu) - 8)))
                }
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq4nl32Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let j = e & 15u
        let by = ((uint(int(blk.qs[int(j >> 1u)])) & 0xFFFFu) >> ((j & 1u) * 8u)) & 0xFFu
        let q = (by >> ((e >> 4u) * 4u)) & 0xFu
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let d = q40_d_of(ws[idx >> 1u], idx)
        static_if (IQLUT) {
            return float16(d * float(iq4lut[int(q)]))
        } else {
            return float16(d * float(int(q) - 8))
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq4nl32Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let li = int((e & 15u) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let d = q40_d_of(ws[idx >> 1u], idx)
        static_if (IQLUT) {
            return half4(float4(d * float(iq4lut[int((l0 >> sh) & 0xFu)]),
                d * float(iq4lut[int((l0 >> (sh + 8u)) & 0xFu)]),
                d * float(iq4lut[int((l1 >> sh) & 0xFu)]),
                d * float(iq4lut[int((l1 >> (sh + 8u)) & 0xFu)])))
        } else {
            return half4(float4(d * float(int((l0 >> sh) & 0xFu) - 8),
                d * float(int((l0 >> (sh + 8u)) & 0xFu) - 8),
                d * float(int((l1 >> sh) & 0xFu) - 8),
                d * float(int((l1 >> (sh + 8u)) & 0xFu) - 8)))
        }
    }
}

[ |> template_struct_instance]
class template Iq4nl32Cm2T : Q4032Cm2T {
    override IQLUT = true
}

[ |> template_struct_instance]
class template Q5032Cm2T : Q51Cm2T {
    override Q50 = true
}

[ |> template_struct_instance]
class template Mx4Cm2T : KqCm2BatchT {
    typedef BLK = VkMx4Blk
    typedef ST = uint
    override BLKW = 32u
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {   // the block's 16 nibble bytes as one word4: the low nibbles are elements 0..15, the high 16..31; `decode_v4`'s packed-byte arithmetic
            let sh = (e0 >> 4u) * 4u
            let w4 = wq4[blk]
            let ex = (ws[blk >> 2u] >> ((blk & 3u) * 8u)) & 0xFFu
            let sc = e8m0_half_f32(ex)
            for [unroll_full] (c in range(4)) {
                let v = mx4_dec4((w4[c] >> sh) & 0x0F0F0F0Fu, sc)
                khr_put4(sbase + uint(c) * 2u, float4(v.x, v.y, v.z, v.w))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes"), arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-mx4-decode")]
    def decode(blk : VkMx4Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let j = e & 15u
        let by = ((uint(int(blk.qs[int(j >> 1u)])) & 0xFFFFu) >> ((j & 1u) * 8u)) & 0xFFu
        let nib = (by >> ((e >> 4u) * 4u)) & 0xFu
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let ex = (ws[idx >> 2u] >> ((idx & 3u) * 8u)) & 0xFFu
        return float16(mx4_dec4(nib, e8m0_half_f32(ex)).x)
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes"), arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-mx4-decode")]
    def decode_v4(blk : VkMx4Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let li = int((e & 15u) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let nw = ((l0 >> sh) & 0xFu) | (((l0 >> (sh + 8u)) & 0xFu) << 8u) | (((l1 >> sh) & 0xFu) << 16u) | (((l1 >> (sh + 8u)) & 0xFu) << 24u)
        let idx = wg_blk0 + bc.x * (pa.n >> 5u) + bc.y
        let ex = (ws[idx >> 2u] >> ((idx & 3u) * 8u)) & 0xFFu
        return half4(mx4_dec4(nw, e8m0_half_f32(ex)))
    }
}

[ |> template_struct_instance]
class template Iq4xsCm2T : K4Cm2T {
    override IQLUT = true
    override SCIQ4 = true
}

//! iq3s4 (IQ3_S served as a nibble codebook plane): iq4xs's tile over the odd codebook and the iq3 row's sub-scale
[ |> template_struct_instance]
class template Iq3s4Cm2T : K4Cm2T {
    override IQLUT = true
    override SCIQ4 = true
    override IQODD = true
    override SCGRID = true
}

[ |> template_struct_instance]
class template K3Cm2T : KqCm2BatchT {
    typedef BLK = VkK3Blk
    typedef ST = uint
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let l0 = e0 & 31u
            let qw4 = wq4[blk * 6u + (g >> 2u) * 2u + (l0 >> 4u)]   // the qs bytes (g >> 2) * 32 + l
            let hw4 = wq4[blk * 6u + 4u + (l0 >> 4u)]               // the hmask bytes l
            let qs2 = (g & 3u) * 2u
            let srow = blk * 5u
            let sidx = e0 >> 4u
            let sc = k6_sc(ws[srow + (sidx >> 2u)], sidx)
            let ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
            for [unroll_full] (c in range(4)) {
                let qw = qw4[c] >> qs2
                let hw = hw4[c] >> g
                let q0 = int(qw & 3u) - ((hw & 1u) == 0u ? 4 : 0)
                let q1 = int((qw >> 8u) & 3u) - (((hw >> 8u) & 1u) == 0u ? 4 : 0)
                let q2 = int((qw >> 16u) & 3u) - (((hw >> 16u) & 1u) == 0u ? 4 : 0)
                let q3 = int((qw >> 24u) & 3u) - (((hw >> 24u) & 1u) == 0u ? 4 : 0)
                khr_put4(sbase + uint(c) * 2u, float4(ds * float(q0), ds * float(q1), ds * float(q2), ds * float(q3)))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK3Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 5u
        let l = e & 31u
        let qi = (g >> 2u) * 32u + l
        let qby = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let hby = ((uint(int(blk.hm[int(l >> 1u)])) & 0xFFFFu) >> ((l & 1u) * 8u)) & 0xFFu
        let q3 = int((qby >> ((g & 3u) * 2u)) & 3u) - (((hby >> g) & 1u) == 0u ? 4 : 0)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = k6_sc(ws[srow + (sidx >> 2u)], sidx)
        let d = unpackHalf2x16(ws[srow + 4u]).x
        return float16(d * float(sc) * float(q3))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK3Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let l = e & 31u
        let qi = int(((g >> 2u) * 32u + l) >> 1u)
        let q0w = uint(int(blk.qs[qi])) & 0xFFFFu
        let q1w = uint(int(blk.qs[qi + 1])) & 0xFFFFu
        let hi = int(l >> 1u)
        let h0w = uint(int(blk.hm[hi])) & 0xFFFFu
        let h1w = uint(int(blk.hm[hi + 1])) & 0xFFFFu
        let qs2 = (g & 3u) * 2u
        let q3_0 = int((q0w >> qs2) & 3u) - (((h0w >> g) & 1u) == 0u ? 4 : 0)
        let q3_1 = int((q0w >> (qs2 + 8u)) & 3u) - (((h0w >> (g + 8u)) & 1u) == 0u ? 4 : 0)
        let q3_2 = int((q1w >> qs2) & 3u) - (((h1w >> g) & 1u) == 0u ? 4 : 0)
        let q3_3 = int((q1w >> (qs2 + 8u)) & 3u) - (((h1w >> (g + 8u)) & 1u) == 0u ? 4 : 0)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let sidx = e >> 4u
        let sc = k6_sc(ws[srow + (sidx >> 2u)], sidx)
        let ds = unpackHalf2x16(ws[srow + 4u]).x * float(sc)
        return half4(float4(ds * float(q3_0), ds * float(q3_1), ds * float(q3_2), ds * float(q3_3)))
    }
}

[ |> template_struct_instance]
class template K2Cm2T : KqCm2BatchT {
    typedef BLK = VkK2Blk
    typedef ST = uint
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 4u
            let qw4 = wq4[blk * 4u + (g >> 3u) * 2u + (g & 1u)]   // the qs bytes (g >> 3) * 32 + (g & 1) * 16 + l
            let qs2 = ((g >> 1u) & 3u) * 2u
            let srow = blk * 5u
            let dm = unpackHalf2x16(ws[srow])
            let pb = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
            let ds = dm.x * float(pb & 15u)
            let dmn = dm.y * float(pb >> 4u)
            for [unroll_full] (c in range(4)) {
                let qw = qw4[c] >> qs2
                khr_put4(sbase + uint(c) * 2u, float4(ds * float(qw & 3u) - dmn, ds * float((qw >> 8u) & 3u) - dmn,
                    ds * float((qw >> 16u) & 3u) - dmn, ds * float((qw >> 24u) & 3u) - dmn))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK2Blk; bc, cib : uint2) : float16 {
        let e = cib.y
        let g = e >> 4u
        let l = e & 15u
        let qi = (g >> 3u) * 32u + (g & 1u) * 16u + l
        let qby = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let q = (qby >> (((g >> 1u) & 3u) * 2u)) & 3u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let pb = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        return float16(dm.x * float(pb & 15u) * float(q) - dm.y * float(pb >> 4u))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK2Blk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 4u
        let l = e & 15u
        let qi = int(((g >> 3u) * 32u + (g & 1u) * 16u + l) >> 1u)
        let q0w = uint(int(blk.qs[qi])) & 0xFFFFu
        let q1w = uint(int(blk.qs[qi + 1])) & 0xFFFFu
        let qs2 = ((g >> 1u) & 3u) * 2u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dm = unpackHalf2x16(ws[srow])
        let pb = (ws[srow + 1u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu
        let ds = dm.x * float(pb & 15u)
        let dmn = dm.y * float(pb >> 4u)
        return half4(float4(ds * float((q0w >> qs2) & 3u) - dmn, ds * float((q0w >> (qs2 + 8u)) & 3u) - dmn,
            ds * float((q1w >> qs2) & 3u) - dmn, ds * float((q1w >> (qs2 + 8u)) & 3u) - dmn))
    }
}

[ |> template_struct_instance]
class template Iq3sCm2T : KqCm2BatchT {
    typedef BLK = VkIq3sBlk
    typedef ST = uint
    override IQ3GRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let r0 = e0 & 31u
            let bw = blk * 26u
            let qsw = wqw[bw + g * 2u + (r0 >> 4u)]                             // the four grid-index bytes g * 8 + (r >> 2)
            let qh = (wqw[bw + 16u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu   // the ninth-bit byte g
            let sgw = wqw[bw + 18u + g] >> ((r0 >> 3u) * 8u)                   // the two sign bytes g * 4 + (r >> 3) in the low half
            let srow = blk * 2u
            let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
            for [unroll_full] (i in range(4)) {
                let wj = (r0 >> 2u) + uint(i)
                let qb = (qsw >> (uint(i) * 8u)) & 0xFFu
                let gw = iq3s_gridc[qb | ((qh << (8u - wj)) & 256u)]
                let sgn = sgw >> ((uint(i) >> 1u) * 8u + (uint(i) & 1u) * 4u)
                khr_put4(sbase + uint(i) * 2u, grid_sv4(gw, sgn, ds))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq3sBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu   // the grid-index byte, shifted out of its lane
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)   // the ninth-bit byte g, shifted out of its lane
        let si = g * 4u + (r >> 3u)
        let sgw = (uint(int(blk.sg[int(si >> 1u)])) & 0xFFFFu) >> ((si & 1u) * 8u)   // the sign byte, shifted out of its lane
        let sgn = sgw >> (((r >> 2u) & 1u) * 4u + (r & 3u))
        let gw = iq3s_gridc[qb | ((qhw << (8u - wj)) & 256u)]
        let gp = gw >> ((r & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
        return float16(grid_sv1(gp, sgn, e, ds))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq3sBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)
        let si = g * 4u + (r >> 3u)
        let sgw = (uint(int(blk.sg[int(si >> 1u)])) & 0xFFFFu) >> ((si & 1u) * 8u)
        let gw = iq3s_gridc[qb | ((qhw << (8u - wj)) & 256u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = grid_sc(ws[srow + 1u], g)
        let ds = d * float(sc)
        let sgn = sgw >> (((r >> 2u) & 1u) * 4u)
        return half4(grid_sv4(gw, sgn, ds))
    }
}

[ |> template_struct_instance]
class template Iq3xxsCm2T : KqCm2BatchT {
    typedef BLK = VkIq3xxsBlk
    typedef ST = uint
    override IQ3XGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let r0 = e0 & 31u
            let bw = blk * 24u
            let qsw = wqw[bw + g * 2u + (r0 >> 4u)]   // the four grid-index bytes g * 8 + (r >> 2)
            let aux = wqw[bw + 16u + g]               // sub-block g's aux32: four 7-bit ksigns indices and the ls nibble
            let srow = blk * 2u
            let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
            for [unroll_full] (i in range(4)) {
                let l = (r0 >> 3u) + (uint(i) >> 1u)
                let sidx = (aux >> (7u * l)) & 127u
                let sgn = ksign7(sidx) >> ((uint(i) & 1u) * 4u)
                let gw = iq3x_gridc[(qsw >> (uint(i) * 8u)) & 0xFFu]
                khr_put4(sbase + uint(i) * 2u, grid_sv4(gw, sgn, ds))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq3xxsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let l = r >> 3u
        let ai = int(g * 2u)   // sub-block g's aux32 as its two 16-bit lanes: four 7-bit ksigns indices
        let aux = (uint(int(blk.ax[ai])) & 0xFFFFu) | ((uint(int(blk.ax[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        let sgn = ksign7(sidx) >> (r & 7u)
        let gw = iq3x_gridc[qb]
        let gp = gw >> ((r & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
        return float16(grid_sv1(gp, sgn, e, ds))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq3xxsBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let wj = r >> 2u
        let qi = g * 8u + wj
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let l = r >> 3u
        let ai = int(g * 2u)
        let aux = (uint(int(blk.ax[ai])) & 0xFFFFu) | ((uint(int(blk.ax[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        let sgb = ksign7(sidx)
        let gw = iq3x_gridc[qb]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let d = unpackHalf2x16(ws[srow]).x
        let sc = grid_sc(ws[srow + 1u], g)
        let ds = d * float(sc)
        let sgn = sgb >> (r & 7u)
        return half4(grid_sv4(gw, sgn, ds))
    }
}

[ |> template_struct_instance]
class template Iq4nlCm2T : Q40Cm2T {
    override IQLUT = true
}

//! NVFP4 on the cm2 tile: the k4-tiled nibbles through the staged e2m1 table under the sixteen UE4M3 sub-block scales of
//! the device row (four words, sub-block order) read through the staged scale table, which carries the site scale -
//! 32-block g's low half reads byte 2g, its high half 2g + 1; iq4nl's decode shape, two table reads an element
[ |> template_struct_instance]
class template Nv4Cm2T : KqCm2BatchT {
    typedef BLK = VkK4Blk
    typedef ST = uint
    override DECV4 = true
    override IQLUT = true
    override MX4LUT = true
    override UE4LUT = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let hh = (e0 >> 4u) & 1u
            let w4 = wq4[blk * 8u + g]
            let si = g * 2u + hh
            let sc = float(ue4lut[int((ws[blk * 5u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0x7Fu)])
            for [unroll_full] (c in range(4)) {
                let w = w4[c] >> (hh * 4u)
                khr_put4(sbase + uint(c) * 2u, float4(sc * float(iq4lut[int(w & 0xFu)]), sc * float(iq4lut[int((w >> 8u) & 0xFu)]),
                    sc * float(iq4lut[int((w >> 16u) & 0xFu)]), sc * float(iq4lut[int((w >> 24u) & 0xFu)])))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkK4Blk; bc, cib : uint2) : float16 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let bidx = g * 16u + (e & 15u)
        let q = (uint(int(blk.qs[int(bidx >> 1u)])) >> ((bidx & 1u) * 8u + (e >> 4u) * 4u)) & 0xFu
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let si = g * 2u + (e >> 4u)
        let sc = float(ue4lut[int((ws[srow + (si >> 2u)] >> ((si & 3u) * 8u)) & 0x7Fu)])
        return float16(sc * float(iq4lut[int(q)]))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkK4Blk; bc, cib : uint2) : half4 {
        let g = cib.y >> 5u
        let e = cib.y & 31u
        let li = int((g * 16u + (e & 15u)) >> 1u)
        let l0 = uint(int(blk.qs[li])) & 0xFFFFu
        let l1 = uint(int(blk.qs[li + 1])) & 0xFFFFu
        let sh = (e >> 4u) * 4u
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let si = g * 2u + (e >> 4u)
        let sc = float(ue4lut[int((ws[srow + (si >> 2u)] >> ((si & 3u) * 8u)) & 0x7Fu)])
        return half4(float4(sc * float(iq4lut[int((l0 >> sh) & 0xFu)]), sc * float(iq4lut[int((l0 >> (sh + 8u)) & 0xFu)]),
            sc * float(iq4lut[int((l1 >> sh) & 0xFu)]), sc * float(iq4lut[int((l1 >> (sh + 8u)) & 0xFu)])))
    }
}

[ |> template_struct_instance]
class template Iq2sCm2T : KqCm2BatchT {
    typedef BLK = VkIq2sBlk
    typedef ST = uint
    override IQ2SGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let r0 = e0 & 31u
            let l0 = r0 >> 3u
            let bw = blk * 18u
            let qsw = wqw[bw + g] >> (l0 * 8u)                                // the grid-index bytes g * 4 + l, this run's two in the low half
            let sgw = wqw[bw + 8u + g] >> (l0 * 8u)                           // the sign bytes 32 + g * 4 + l
            let qh = (wqw[bw + 16u + (g >> 2u)] >> ((g & 3u) * 8u)) & 0xFFu   // the qh byte g
            let srow = blk * 5u
            let si = g * 2u + (r0 >> 4u)
            let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
            for [unroll_full] (i in range(4)) {
                let li = uint(i) >> 1u
                let l = l0 + li
                let qb = (qsw >> (li * 8u)) & 0xFFu
                let sgn = (sgw >> (li * 8u + (uint(i) & 1u) * 4u)) & 0xFu
                let gw = iq2s_gridc[(qb | ((qh << (8u - 2u * l)) & 0x300u)) * 2u + (uint(i) & 1u)]
                khr_put4(sbase + uint(i) * 2u, grid_sv4(gw, sgn, ds))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2sBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 4u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)
        let sgn = ((uint(int(blk.sg[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) >> j
        let gw = iq2s_gridc[(qb | ((qhw << (8u - 2u * l)) & 0x300u)) * 2u + (j >> 2u)]
        let gp = gw >> ((j & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let si = g * 2u + (r >> 4u)
        let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
        return float16(grid_sv1(gp, sgn, e, ds))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq2sBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 4u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let qhw = (uint(int(blk.qh[int(g >> 1u)])) & 0xFFFFu) >> ((g & 1u) * 8u)
        let sgw = (uint(int(blk.sg[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)
        let gw = iq2s_gridc[(qb | ((qhw << (8u - 2u * l)) & 0x300u)) * 2u + (j >> 2u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let d = unpackHalf2x16(ws[srow]).x
        let si = g * 2u + (r >> 4u)
        let sc = (ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu
        let ds = d * float(sc)
        let sgn = sgw >> j
        return half4(grid_sv4(gw, sgn, ds))
    }
}

class template Iq2xsCm2T : KqCm2BatchT {
    typedef BLK = VkIq2xsBlk
    typedef ST = uint
    override IQ2XSGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let qw = wqw[blk * 16u + (e0 >> 4u)]   // two u16 qs words: the run's 8-value windows
            let srow = blk * 5u
            let si = e0 >> 4u
            let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
            for [unroll_full] (i in range(4)) {
                let w16 = (qw >> ((uint(i) >> 1u) * 16u)) & 0xFFFFu
                let sidx = w16 >> 9u
                let sgn = ksign7(sidx) >> ((uint(i) & 1u) * 4u)
                let gw = iq2xs_gridc[(w16 & 511u) * 2u + (uint(i) & 1u)]
                khr_put4(sbase + uint(i) * 2u, grid_sv4(gw, sgn, ds))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2xsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let wi = pair0 >> 3u
        let j = pair0 & 7u
        let w16 = uint(int(blk.qs[int(wi)])) & 0xFFFFu
        let sidx = w16 >> 9u
        let sgn = ksign7(sidx) >> j
        let gw = iq2xs_gridc[(w16 & 511u) * 2u + (j >> 2u)]
        let gp = gw >> ((j & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let si = pair0 >> 4u
        let ds = unpackHalf2x16(ws[srow]).x * float((ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu)
        return float16(grid_sv1(gp, sgn, e, ds))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq2xsBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let wi = e >> 3u
        let j = e & 7u
        let w16 = uint(int(blk.qs[int(wi)])) & 0xFFFFu
        let sidx = w16 >> 9u
        let sgb = ksign7(sidx)
        let gw = iq2xs_gridc[(w16 & 511u) * 2u + (j >> 2u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 5u
        let dv = unpackHalf2x16(ws[srow]).x
        let si = e >> 4u
        let sc = (ws[srow + 1u + (si >> 2u)] >> ((si & 3u) * 8u)) & 0xFFu
        let ds = dv * float(sc)
        let sgn = sgb >> j
        return half4(grid_sv4(gw, sgn, ds))
    }
}

class template Iq2xxsCm2T : KqCm2BatchT {
    typedef BLK = VkIq2xxsBlk
    typedef ST = uint
    override IQ2XXSGRID = true
    override DECV4 = true

    def override khr_stage16(blk, e0, sbase : uint) : void {
        static_if (KHR) {
            let g = e0 >> 5u
            let l0 = (e0 & 31u) >> 3u
            let bw = blk * 16u
            let qsw = wqw[bw + g * 2u] >> (l0 * 8u)   // sub-block g's four grid-index bytes, this run's two in the low half
            let aux = wqw[bw + g * 2u + 1u]           // its aux32: four 7-bit ksigns indices and the ls nibble
            let srow = blk * 2u
            let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
            for [unroll_full] (i in range(4)) {
                let li = uint(i) >> 1u
                let l = l0 + li
                let qb = (qsw >> (li * 8u)) & 0xFFu
                let sidx = (aux >> (7u * l)) & 127u
                let sgn = ksign7(sidx) >> ((uint(i) & 1u) * 4u)
                let gw = iq2xxs_gridc[qb * 2u + (uint(i) & 1u)]
                khr_put4(sbase + uint(i) * 2u, grid_sv4(gw, sgn, ds))
            }
        }
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode(blk : VkIq2xxsBlk; bc, cib : uint2) : float16 {
        let e = cib.y
        let pair0 = e & ~1u
        let g = pair0 >> 5u
        let r = pair0 & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 8u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let ai = int(g * 4u + 2u)   // sub-block g's aux32 as its two 16-bit lanes: four 7-bit ksigns indices
        let aux = (uint(int(blk.qs[ai])) & 0xFFFFu) | ((uint(int(blk.qs[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        let sgn = ksign7(sidx) >> j
        let gw = iq2xxs_gridc[qb * 2u + (j >> 2u)]
        let gp = gw >> ((j & 3u) * 8u)
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let ds = unpackHalf2x16(ws[srow]).x * float(grid_sc(ws[srow + 1u], g))
        return float16(grid_sv1(gp, sgn, e, ds))
    }

    [spirv_decode, arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes")]
    def decode_v4(blk : VkIq2xxsBlk; bc, cib : uint2) : half4 {
        let e = cib.y
        let g = e >> 5u
        let r = e & 31u
        let l = r >> 3u
        let j = r & 7u
        let qi = g * 8u + l
        let qb = ((uint(int(blk.qs[int(qi >> 1u)])) & 0xFFFFu) >> ((qi & 1u) * 8u)) & 0xFFu
        let ai = int(g * 4u + 2u)
        let aux = (uint(int(blk.qs[ai])) & 0xFFFFu) | ((uint(int(blk.qs[ai + 1])) & 0xFFFFu) << 16u)
        let sidx = (aux >> (7u * l)) & 127u
        let sgb = ksign7(sidx)
        let gw = iq2xxs_gridc[qb * 2u + (j >> 2u)]
        let srow = (wg_blk0 + bc.x * (pa.n >> 8u) + bc.y) * 2u
        let dv = unpackHalf2x16(ws[srow]).x
        let sc = grid_sc(ws[srow + 1u], g)
        let ds = dv * float(sc)
        let sgn = sgb >> j
        return half4(grid_sv4(gw, sgn, ds))
    }
}

//! the router GEMV on the template every home stamps (`GkRouterGemv`): a workgroup of 256 an expert row, eight columns a dispatch,
//! the row read as its weightless rms under the norm row where `norm_on`
[ |> template_struct_instance]
class template RouterGemvT : GkRouterGemv {
    override NORM = true
    override YSCALE = true

    [spirv_inline]
    def router_body {
        router_row(gl_WorkGroupID.x, gl_LocalInvocationID.x, 256u)
    }
}

//! the conv front's bindings over the template every home stamps (`GkDnConv`): the taps and the history out of the layer's smalls
[ |> template_struct_instance]
class template DnConvT : GkDnConv {
    @ssbo @binding = 2 @role = "read" smalls : array<float>   //! taps + history + any small planes (push offsets)

    def override seg_row0 : uint => pa.row0
    def override seg_npos : uint => pa.npos
    def override tap_at(ch, j : uint) : float => smalls[pa.taps_off + ch * pa.dconv + j]
    def override hist_at(ch, taps, hidx : uint) : float {
        if (pa.hist_mode == 1u) {
            return smalls[pa.hist_off + ch * taps + hidx]
        }
        return smalls[pa.hist_off + hidx * pa.cd + ch]
    }
}

struct DnTailArgs {
    cd : uint         // conv width (channels)
    taps : uint       // history rows (dconv - 1)
    row0 : uint       // the first tail row in src (the window's last taps raw rows)
    hist_off : uint   // the ring image's float offset in smalls
    old_rows : uint   //! leading ring rows the call's entry image supplies (a lone window shorter than the taps): new row k is old row k + taps - old_rows
    old_off : uint    //! the entry image's float offset in smalls (per-channel, the decode step's layout); never the image at `hist_off`
}

//! the conv ring write both tails stamp (`DnTailRingT` the window's, `DnCapTailT` a capture's): thread `idx` of
//! `cd * taps` takes channel `idx / taps`'s ring row `k = idx % taps` - projected row `row0 + k - old_rows` where `k >= old_rows`,
//! else the entry image's row `k + taps - old_rows` at `old_off` - into the destination image at `dst0`
[ |> template_struct_instance]
class template DnTailT {
    def abstract src_at(i : uint) : float     //! projected-rows float `i`
    def abstract entry_at(i : uint) : float   //! smalls float `i`
    def abstract put(i : uint; v : float) : void

    def tail_tap(idx, cd, taps, row0, old_rows, old_off, dst0 : uint) {
        if (taps != 0u && idx < cd * taps) {
            let ch = idx / taps
            let k = idx % taps
            var v = 0.0
            if (k >= old_rows) {
                v = src_at((row0 + k - old_rows) * cd + ch)
            } else {
                v = entry_at(old_off + ch * taps + k + taps - old_rows)
            }
            put(dst0 + ch * taps + k, v)
        }
    }
}

//! the window's ring hand-off: the window's last `taps` projected rows into the layer's other ring image
[ |> template_struct_instance]
class template DnTailRingT : DnTailT {
    @ssbo @binding = 0 src : array<float>      // the projected rows (position-major)
    @ssbo @binding = 1 smalls : array<float>   // the layer's smalls (the ring image at hist_off)
    @push_constant pa : DnTailArgs

    def override src_at(i : uint) : float => src[i]
    def override entry_at(i : uint) : float => smalls[i]
    def override put(i : uint; v : float) : void {
        smalls[i] = v
    }

    [spirv_inline]
    def tail_ring_body {
        tail_tap(gl_GlobalInvocationID.x, pa.cd, pa.taps, pa.row0, pa.old_rows, pa.old_off, pa.hist_off)
    }
}

struct DnCapTailArgs {
    cd : uint
    taps : uint
    row0 : uint       //! the part's first row in the projected rows
    entry_off : uint  //! the ring image the part entered on (float offset in smalls): the rows before the part, for a capture under the taps
    cap_rec : uint    //! the part's records (two words a capture: the row the state stands after, the checkpoint row's float base)
    cap_hist_off : uint   //! the layer's ring image 0 floats into a checkpoint row ((the layer's snapshot offset + the slot's state bytes) / 4)
}

//! a capture's conv history into its checkpoint row's ring image 0 (`DnTailT`; the row's parity reads image 0): the taps
//! projected rows before the capture's row, the rows before the part off the image it entered on - the template both
//! discrete homes stamp
[ |> template_struct_instance]
class template DnCapTailT : DnTailT {
    @ssbo @binding = 0 @readonly src : array<float>      //! the projected rows (position-major)
    @ssbo @binding = 1 @readonly smalls : array<float>   //! the layer's smalls (the entry ring image at entry_off)
    @ssbo @binding = 2 @readonly caps : array<uint>      //! the window's capture records
    @ssbo @binding = 3 dst : array<float>                //! the checkpoint rows
    @push_constant pa : DnCapTailArgs

    def override src_at(i : uint) : float => src[i]
    def override entry_at(i : uint) : float => smalls[i]
    def override put(i : uint; v : float) : void {
        dst[i] = v
    }

    //! a capture at part row `crow` is a window tail ending there: the rows under `crow` come off the entry image; the dispatch is
    //! `dn_cap_tail_wgs` workgroups a capture, back to back
    [spirv_inline]
    def cap_tail_body {
        let wpc = max(1u, (pa.cd * pa.taps + 255u) / 256u)
        let cap = gl_WorkGroupID.x / wpc
        let rec = pa.cap_rec + cap * 2u
        let crow = caps[rec]
        let lead = min(crow, pa.taps)
        tail_tap((gl_WorkGroupID.x % wpc) * 256u + gl_LocalInvocationID.x, pa.cd, pa.taps, pa.row0 + crow - lead, pa.taps - lead,
            pa.entry_off, caps[rec + 1u] + pa.cap_hist_off)
    }
}

//! the capture tail's workgroups a capture: a thread a history float over `cd` x `taps`
def dn_cap_tail_wgs(cd, taps : int64) : int64 => max(1l, (cd * taps + 255l) / 256l)

struct F16GemmArgs {
    n : uint        //! K, the row width (elements), a 64-multiple
    d : uint        //! M, the weight rows
    rows : uint     //! N, the positions
    woff : uint     //! the weight rows' element base (read under `WOFF`)
    ybase : uint    //! the output's element base: the sum's, or the first partial plane's
    ystride : uint  //! the output row stride (elements) when the GEMM stores whole
    ksplit : uint   //! the k chunk per workgroup (read under `KSPLIT`); 0 = the whole K, one plane at ybase with ystride
}

let F16G_BK = 64u      //! the small f16 GEMM's k step (the MoE router, the deltanet beta/alpha rows and the E-series projection on the cm2 tile)

//! a small f16 x f16 GEMM on the cm2 128 x 128 tile, y[rows x d] = x[rows x n] w[d x n]^T, the template both discrete homes
//! stamp; a k chunk's partial plane lands at ybase + chunk * rows * d (stride d)
[ |> template_struct_instance]
class template F16GemmCm2T {
    @template_constant KSPLIT : bool = true   //!< the k split rides `pa.ksplit`; off (a stamp every site dispatches whole), the body folds the split away
    @template_constant WOFF : bool = true     //!< the weight rows start at `pa.woff`; off (a stamp every site binds at row 0), at 0
    @ssbo @binding = 0 w : array<float16>   //! the weight rows; no `weight` role: the E-series projection binds its f16 panel, which the Vulkan home rewrites every window
    @ssbo @binding = 1 xf16 : array<float16>
    @ssbo @binding = 2 y : array<float>
    @push_constant pa : F16GemmArgs
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f32_128x128

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels")]
    def f16_gemm_body {
        let xtiles = (pa.rows + 127u) / 128u
        let wtiles = (pa.d + 127u) / 128u
        let chunk = gl_WorkGroupID.x / (xtiles * wtiles)
        let rem = gl_WorkGroupID.x % (xtiles * wtiles)
        let t0 = (rem / wtiles) * 128u
        let m0 = (rem % wtiles) * 128u
        let split = KSPLIT && pa.ksplit != 0u
        let woff = WOFF ? pa.woff : 0u
        let k0 = split ? (chunk * pa.ksplit) & ~(F16G_BK - 1u) : 0u
        let k1 = split ? min(k0 + pa.ksplit, pa.n) : pa.n
        let ybase = split ? pa.ybase + chunk * pa.rows * pa.d : pa.ybase
        let ystride = split ? pa.d : pa.ystride
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, pa.rows, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, pa.rows, pa.d)
        tensorLayoutSetStride(tlo, ystride, 1u)
        var a : AT
        var b : BT
        var acc : ACC
        coopmatFill(acc, 0.0)   //! a split chunk past K (a k split rounded up to the step on a short K) stores a zero plane, never a stale accumulator
        var k = k0
        while (k < k1) {
            coopmatLoadTensor(a, w, woff, tla, m0, 128u, k, F16G_BK)
            coopmatLoadTensor(b, xf16, 0u, tlb, t0, 128u, k, F16G_BK, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += F16G_BK
        }
        coopmatStoreTensor(acc, y, ybase, tlo, t0, 128u, m0, 128u, tv)
    }
}

//! the cm2 f16 GEMM's workgroups: a 128 x 128 tile a workgroup over rows x d
def f16_gemm_cm2_wgs(rows, d : int64) : int64 => ((rows + 127l) / 128l) * ((d + 127l) / 128l)

struct DnBaArgs {
    npos : uint
    dim : uint
    nvh : uint       // beta rows (the alpha rows follow them in w)
    woff : uint      // this layer's [beta ; alpha] row base in w (elements)
    xstride : uint   // x row stride (elements)
    obase_b : uint   // beta output base in outp: row p's nvh values at obase_b + p * ostride
    obase_g : uint   // alpha output base, the same layout
    ostride : uint
}

//! the window's beta and alpha rows off the file's f32 `[beta ; alpha]` rows: a workgroup 16 positions by 16 output rows, one
//! output an invocation, the x rows in the stamp's element type (`XT`: the f32 normed rows, or the window's f16 feed)
[ |> template_struct_instance]
class template DnBaGemmT {
    @ssbo @binding = 0 @role = "weight" w : array<float>   //! the [beta ; alpha] rows per layer, the file's f32
    @ssbo @binding = 1 x : array<XT>          // the normed rows
    @ssbo @binding = 2 outp : array<float>    // the layer smalls (beta / g rows, position-major)
    @push_constant pa : DnBaArgs
    @workgroup xs : float[528]                 // 16 positions x 32 k, stride 33 (bank-spread)
    @workgroup ws : float[528]                 // 16 rows x 32 k, stride 33

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]
    def dn_ba_body {
        let tid = gl_LocalInvocationID.x
        let ne = 2u * pa.nvh
        let egroups = (ne + 15u) / 16u
        let p0 = (gl_WorkGroupID.x / egroups) * 16u
        let eg0 = (gl_WorkGroupID.x % egroups) * 16u
        let p = tid / 16u
        let el = tid % 16u
        let e = eg0 + el
        let live = p0 + p < pa.npos && e < ne
        let r = tid / 32u
        let k = tid % 32u
        var a = 0.0
        var kb = 0u
        while (kb < pa.dim) {
            if (p0 + r < pa.npos) {
                xs[r * 33u + k] = float(x[(p0 + r) * pa.xstride + kb + k])   // nolint:PERF020,LINT019 — XT is float16 on the CUDA window's stamp, float on the Vulkan one
            } else {
                xs[r * 33u + k] = 0.0
            }
            if (p0 + r + 8u < pa.npos) {
                xs[(r + 8u) * 33u + k] = float(x[(p0 + r + 8u) * pa.xstride + kb + k])   // nolint:PERF020,LINT019 — XT is float16 on the CUDA window's stamp, float on the Vulkan one
            } else {
                xs[(r + 8u) * 33u + k] = 0.0
            }
            if (eg0 + r < ne) {
                ws[r * 33u + k] = w[pa.woff + (eg0 + r) * pa.dim + kb + k]
            } else {
                ws[r * 33u + k] = 0.0
            }
            if (eg0 + r + 8u < ne) {
                ws[(r + 8u) * 33u + k] = w[pa.woff + (eg0 + r + 8u) * pa.dim + kb + k]
            } else {
                ws[(r + 8u) * 33u + k] = 0.0
            }
            barrier()
            if (live) {
                var kk = 0u
                while (kk < 32u) {
                    a += xs[p * 33u + kk] * ws[el * 33u + kk]
                    kk++
                }
            }
            barrier()
            kb += 32u
        }
        if (live) {
            outp[(e < pa.nvh ? pa.obase_b : pa.obase_g) + (p0 + p) * pa.ostride + (e % pa.nvh)] = a
        }
    }
}

//! the row kernels' reduces in value form over the shared `GkWgReduce` (one wg reduces one row), and the inverse rms
[ |> template_struct_instance]
class template WgReduceBase : GkQ8BlockStore {
    //! `slot` alternates 0 / 1 between consecutive reduces of one kernel
    def wg_rms_inv(ss0 : float; n : uint; eps : float; slot : uint) : float => 1.0 / sqrt(wg_sum(ss0, slot) / float(n) + eps)

    //! the workgroup sum every thread holds alike (the layernorm's mean and variance ride it, and `wg_rms_inv`) - the
    //! shared base's reduce in value form, which both discrete emitters take
    def wg_sum(v0 : float; slot : uint) : float {
        var tot = 0.0
        wg_sum_into(v0, slot, tot)
        return tot
    }

    //! the workgroup max every thread holds alike (the chunked attention's score max)
    def wg_max(v0 : float; slot : uint) : float {
        var m = 0.0
        wg_max_into(v0, slot, m)
        return m
    }
}

struct DnStepArgs {
    cd : uint
    kd : uint
    nkh : uint
    ds : uint
    dconv : uint
    beta_off : uint
    g_off : uint
    taps_off : uint
    wnorm_off : uint
    a_off : uint
    dt_off : uint
    hist_off : uint
    eps : float
    qscale : float
    out_f32 : uint
    beta_row_stride : uint
    row0 : uint   //! the dispatch's first row: the same-slab verify steps one stream's rows one dispatch a row in one slot, each reading the state the row before it wrote
}

//! the fused deltanet decode step; its stamps differ on the out-gate alone (`ZSIG`)
[ |> template_struct_instance]
class template DnStepFusedT : WgReduceBase {
    @template_constant ZSIG : bool = false   //!< the out-gate is sigmoid(z) (qwen4exp's `Config.dn_z_sigmoid`) instead of silu(z)
    @ssbo @binding = 0 proj : array<float>     // the fresh projection row (+ z rows at cd)
    @ssbo @binding = 1 state : array<float>    // the persistent per-head f32 state (in place)
    @ssbo @binding = 2 smalls : array<float>   // beta/g/taps/wnorm/a/dt + the conv HISTORY ring (rw)
    @ssbo @binding = 3 outs : array<float>     // the head's Q8_0 scales
    @ssbo @binding = 4 outq : array<uint>      // ... and quant words
    @ssbo @binding = 5 tok : array<TokMeta>    // the shared per-token block (element 0)
    @ssbo @binding = 6 outf : array<float>     // the head's o row as f32
    @push_constant pa : DnStepArgs
    @workgroup dq : float[128]
    @workgroup dk : float[128]
    @workgroup dv : float[128]
    @workgroup dvn : float[128]
    @workgroup dout : float[128]
    @workgroup partk : float[256]   //! each row part's k.S column partials (part-major, ds wide: the thread's own slot)
    @workgroup partq : float[256]   //! ... and q.S
    @workgroup sg_sumsq : float[16]   //! per-subgroup partial squared sums (q and k); the out-norm's reduce is `WgReduceBase`'s
    @workgroup sg_qk : float[16]      //! per-subgroup partials of the raw q.k
    @workgroup ds8 : float[8]

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]
    def dn_step_body {   // nolint:STYLE037,STYLE038 — the fused chain: conv, norms, the delta rule, out-norm and the o row in one dispatch
        let ds = pa.ds
        let nvh = max(1u, (pa.cd - 2u * pa.kd) / ds)   //! the head count the geometry pins: cd = 2 kd + nvh ds
        let row = pa.row0 + gl_WorkGroupID.x / nvh
        let h = gl_WorkGroupID.x % nvh
        let taps = pa.dconv - 1u
        let par = tok[row].parity
        let slot = tok[row].dnslot
        let prow = row * (pa.cd + nvh * ds)
        let ringb = pa.hist_off + slot * 2u * pa.cd * taps
        let rrb = ringb + par * pa.cd * taps
        let wrb = ringb + (1u - par) * pa.cd * taps
        let tid = gl_LocalInvocationID.x
        let kh = h % pa.nkh
        var e = tid
        while (e < 3u * ds) {
            let sl = e / ds
            let j = e % ds
            let ch = (sl == 0u ? kh * ds : (sl == 1u ? pa.kd + kh * ds : 2u * pa.kd + h * ds)) + j
            let fresh = proj[prow + ch]
            var acc = fresh * smalls[pa.taps_off + ch * pa.dconv + taps]
            var k = 0u
            while (k < taps) {
                acc += smalls[rrb + ch * taps + k] * smalls[pa.taps_off + ch * pa.dconv + k]
                k++
            }
            if (sl == 2u || h < pa.nkh) {   // this head owns the channel's ring advance
                k = 0u
                while (k + 1u < taps) {
                    smalls[wrb + ch * taps + k] = smalls[rrb + ch * taps + k + 1u]
                    k++
                }
                smalls[wrb + ch * taps + taps - 1u] = fresh
            }
            let sv = silu_f32(acc)
            if (sl == 0u) {
                dq[j] = sv
            } elif (sl == 1u) {
                dk[j] = sv
            } else {
                dv[j] = sv
            }
            e += 256u
        }
        barrier()
        var pn = 0.0
        var pq = 0.0
        if (tid < ds) {
            pn = dq[tid] * dq[tid]
            pq = dq[tid] * dk[tid]
        } elif (tid >= 128u && tid < 128u + ds) {
            pn = dk[tid - 128u] * dk[tid - 128u]
        }
        let sn = subgroupAdd(pn)
        let spq = subgroupAdd(pq)
        if (gl_SubgroupInvocationID == 0u) {
            sg_sumsq[gl_SubgroupID] = sn
            sg_qk[gl_SubgroupID] = spq
        }
        barrier()
        if (tid == 0u) {   //! q.k is summed raw; the q and k scales apply here
            var sumq = 0.0
            var sumk = 0.0
            var qkr = 0.0
            let half = gl_NumSubgroups / 2u
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                if (sgi < half) {
                    sumq += sg_sumsq[sgi]
                    qkr += sg_qk[sgi]
                } else {
                    sumk += sg_sumsq[sgi]
                }
                sgi++
            }
            let sq = (1.0 / max(sqrt(sumq), pa.eps)) * pa.qscale
            let sk = 1.0 / max(sqrt(sumk), pa.eps)
            ds8[0u] = sq
            ds8[1u] = sk
            let graw = smalls[pa.g_off + row * nvh + h] + smalls[pa.dt_off + h]
            ds8[2u] = dn_decay(smalls[pa.a_off + h], graw)
            ds8[3u] = sigmoid_f32(smalls[pa.beta_off + row * pa.beta_row_stride + h])
            ds8[4u] = qkr * sq * sk
        }
        barrier()
        let sq = ds8[0u]
        let sk = ds8[1u]
        let eg = ds8[2u]
        let bt = ds8[3u]
        let qk = ds8[4u]
        let rows_per_part = min(ds, 64u)
        let nparts = ds / rows_per_part
        let active = tid < ds * nparts
        let c = tid % ds
        let r0 = (tid / ds) * rows_per_part
        let sbase = slot * nvh * ds * ds + h * ds * ds
        var sreg : float[64]
        var ksp = 0.0
        var qsp = 0.0
        if (active) {
            // the loads first, straight-line (a short part re-reads its last row), so all 64 issue before a sum waits on one
            for [unroll] (j in range(64)) {
                sreg[j] = state[sbase + (r0 + min(uint(j), rows_per_part - 1u)) * ds + c]
            }
            for [unroll] (j in range(64)) {
                if (uint(j) < rows_per_part) {
                    ksp += dk[r0 + uint(j)] * sreg[j]
                    qsp += dq[r0 + uint(j)] * sreg[j]
                }
            }
        }
        if (active) {
            partk[tid] = ksp
            partq[tid] = qsp
        }
        barrier()
        if (tid < ds) {
            var ks = 0.0
            var qs = 0.0
            var p = 0u
            while (p < nparts) {
                ks += partk[p * ds + tid]
                qs += partq[p * ds + tid]
                p++
            }
            let vn = bt * (dv[tid] - eg * (sk * ks))
            dvn[tid] = vn
            dout[tid] = eg * (sq * qs) + qk * vn
        }
        barrier()
        if (active) {
            let vnc = dvn[c]
            for [unroll] (j in range(64)) {
                if (uint(j) < rows_per_part) {
                    state[sbase + (r0 + uint(j)) * ds + c] = sreg[j] * eg + (sk * dk[r0 + uint(j)]) * vnc
                }
            }
        }
        var po = 0.0
        if (tid < ds) {
            po = dout[tid] * dout[tid]
        }
        let oinv = wg_rms_inv(po, ds, pa.eps, 0u)
        e = tid
        while (e < ds) {
            let zv = proj[prow + pa.cd + h * ds + e]
            let xn = dout[e] * oinv * smalls[pa.wnorm_off + e]
            dout[e] = xn * dn_zgate(zv, ZSIG)
            e += 256u
        }
        barrier()
        let orow = (row * nvh + h) * ds
        if (pa.out_f32 != 0u) {
            e = tid
            while (e < ds) {
                outf[orow + e] = dout[e]
                e += 256u
            }
        } elif (tid < ds / 32u) {   // the head's Q8_0 blocks (ds % 32 asserted host-side)
            let gb = orow / 32u + tid
            let jb = tid * 32u
            var amax = 0.0
            for (i in range(32)) {
                amax = max(amax, abs(dout[jb + uint(i)]))
            }
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            outs[gb] = d
            for (wi in range(8)) {
                let wj = jb + uint(wi) * 4u
                var word = 0u
                for (j in range(4)) {
                    let q = int(round(dout[wj + uint(j)] * id))
                    word = word | ((uint(q) & 0xFF) << (uint(j) * 8u))
                }
                outq[gb * 8u + uint(wi)] = word
            }
        }
    }
}

struct DnScanArgs {
    npos : uint
    cd : uint
    kd : uint
    nvh : uint
    nkh : uint
    ds : uint
    beta_off : uint   // raw beta per (pos, head) in smalls; sigmoid in kernel
    g_off : uint      // raw g per (pos, head); the decay is exp(a * softplus(g + dt))
    a_off : uint
    dt_off : uint
    o_off : uint      // the raw o rows' base in orows (position-major, di wide)
    di : uint
    region_state_base : uint
    row0 : uint       //! the dispatch's first row in the conv rows, the beta / g rows and the o rows: a segmented window scans a segment's rows over its own region's state
    cap_n : uint      //! the part's captures: records of two words (the row the state stands after, the checkpoint row's float base) at `cap_rec` of `caps`
    cap_rec : uint
    cap_state_off : uint   //! the layer's state floats into a checkpoint row (`RLayer.snap_off / 4`)
}

//! the plain per-token delta rule over the window's conv rows: one column of a head's state per lane cluster, 16 state rows a lane in
//! registers, the next token's k and q read ahead of the current token's recurrence
[ |> template_struct_instance]
class template DnScanT {
    @ssbo @binding = 0 @readonly conv : array<float>     //! the conv output rows (position-major: q | k | v)
    @ssbo @binding = 1 state : array<float>              // the persistent per-head f32 state [nvh][ds][ds]
    @ssbo @binding = 2 @readonly smalls : array<float>   // raw beta/g rows, a/dt per head (push offsets)
    @ssbo @binding = 3 orows : array<float>              // the raw o rows at o_off
    @ssbo @binding = 4 @readonly caps : array<uint>      //! the window's capture records
    @ssbo @binding = 5 capst : array<float>              //! the checkpoint rows (`snap_dev`): a capture's state lands at its row's base plus the layer's
    @push_constant pa : DnScanArgs
    @workgroup egs : float[int(DN_WINDOW)]   //! the head's decay per token, exp(a * softplus(g + dt)), staged once per workgroup
    @workgroup bts : float[int(DN_WINDOW)]   //! the head's sigmoid(beta) per token, the same way

    def cluster_add(v : float; lanes_per_col : uint) : float {
        var r = v
        if (lanes_per_col > 4u) {
            r += subgroupShuffleXor(r, 4u)
        }
        if (lanes_per_col > 2u) {
            r += subgroupShuffleXor(r, 2u)
        }
        if (lanes_per_col > 1u) {
            r += subgroupShuffleXor(r, 1u)
        }
        return r
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]
    def dn_scan_body {   // nolint:STYLE037,STYLE038 — the per-token recurrence over 16 register rows of one column
        let ds = pa.ds
        let lanes_per_col = ds / 16u           // 16 state rows each; ds % 16 == 0 host-side
        let cols_per_block = 32u / lanes_per_col   //! the 128-thread workgroup's four 32-lane blocks, by the invocation id: a cluster's shuffles (masks under 8) stay inside a block on any subgroup width
        let cols_per_group = 4u * cols_per_block
        let cgroups = (ds + cols_per_group - 1u) / cols_per_group
        let h = gl_WorkGroupID.x / cgroups
        let sl = gl_LocalInvocationID.x & 31u
        let lane = sl % lanes_per_col
        let col = (gl_WorkGroupID.x % cgroups) * cols_per_group + (gl_LocalInvocationID.x >> 5u) * cols_per_block + sl / lanes_per_col
        let live = col < ds                    // a whole cluster shares col, so its shuffles stay uniform
        let kh = h % pa.nkh
        let cbase = pa.row0 * pa.cd
        let q_elem0 = cbase + kh * ds + lane
        let k_elem0 = cbase + pa.kd + kh * ds + lane
        let voff = cbase + 2u * pa.kd + h * ds + (live ? col : 0u)   // an idle lane reads column 0: past the last head its own column is past the plane
        let aa = smalls[pa.a_off + h]
        let dtb = smalls[pa.dt_off + h]
        var ti = gl_LocalInvocationID.x
        while (ti < pa.npos) {
            let graw = smalls[pa.g_off + (pa.row0 + ti) * pa.nvh + h] + dtb
            egs[ti] = dn_decay(aa, graw)
            bts[ti] = sigmoid_f32(smalls[pa.beta_off + (pa.row0 + ti) * pa.nvh + h])
            ti += 128u
        }
        barrier()
        let sb = pa.region_state_base + h * ds * ds + (live ? col : 0u) + lane * ds
        let row_stride = lanes_per_col * ds
        var s : float[16]
        for [unroll_full] (i in range(16)) {
            s[i] = state[sb + uint(i) * row_stride]
        }
        var k_next, q_next : float[16]
        var v_next = conv[voff]
        for [unroll_full] (i in range(16)) {
            k_next[i] = conv[k_elem0 + uint(i) * lanes_per_col]
        }
        for [unroll_full] (i in range(16)) {
            q_next[i] = conv[q_elem0 + uint(i) * lanes_per_col]
        }
        var t = 0u
        while (t < pa.npos) {
            let eg = egs[t]
            let bt = bts[t]
            let v = v_next
            var k, q : float[16]
            for [unroll_full] (i in range(16)) {
                k[i] = k_next[i]
            }
            for [unroll_full] (i in range(16)) {
                q[i] = q_next[i]
            }
            let crow_next = min(t + 1u, pa.npos - 1u) * pa.cd
            v_next = conv[crow_next + voff]
            let kbn = crow_next + k_elem0
            for [unroll_full] (i in range(16)) {
                k_next[i] = conv[kbn + uint(i) * lanes_per_col]
            }
            let qbn = crow_next + q_elem0
            for [unroll_full] (i in range(16)) {
                q_next[i] = conv[qbn + uint(i) * lanes_per_col]
            }
            var kv = k[0] * s[0]
            for [unroll_full] (i in range(1, 16)) {
                kv += k[i] * s[i]
            }
            kv = cluster_add(kv, lanes_per_col)
            let delta = bt * (v - eg * kv)
            for [unroll_full] (i in range(16)) {
                s[i] = s[i] * eg + k[i] * delta
            }
            var o = q[0] * s[0]
            for [unroll_full] (i in range(1, 16)) {
                o += q[i] * s[i]
            }
            o = cluster_add(o, lanes_per_col)
            if (live && lane == 0u) {
                orows[pa.o_off + (pa.row0 + t) * pa.di + h * ds + col] = o
            }
            var ci = 0u
            while (ci < pa.cap_n) {
                let rec = pa.cap_rec + ci * 2u
                if (caps[rec] == t + 1u && live) {
                    let cb = caps[rec + 1u] + pa.cap_state_off + h * ds * ds + col + lane * ds
                    for [unroll_full] (i in range(16)) {
                        capst[cb + uint(i) * row_stride] = s[i]
                    }
                }
                ci++
            }
            t++
        }
        if (live) {
            for [unroll_full] (i in range(16)) {
                state[sb + uint(i) * row_stride] = s[i]
            }
        }
    }
}

//! the gated out-norm over the scan's raw o rows on the template every home stamps (`GkDnGate`), one workgroup a row, its
//! subgroups walking the value heads
[ |> template_struct_instance]
class template DnScanP3T : GkDnGate {
    [spirv_inline]
    def dn_gate_body {
        var h = gl_SubgroupID
        while (h < pa.nvh) {
            dn_gate_head(h, gl_WorkGroupID.x)
            h += gl_NumSubgroups
        }
    }
}

