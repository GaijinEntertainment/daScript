options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_classes_rows shared public

//! The rows tensor leaf's family: the decode GEMV on the tensor cores at up to sixteen token rows, its feed's writers, and the
//! format templates that stamp it. The module is the family's carveout policy too (`cud_class_fn`): every tile re-reads the
//! rows' feed, so its kernels leave the L1 / shared split to the driver

require dasllama/dasllama_lint public
require cuda/cuda_kernel public
require cuda/cuda_intrinsics
require spirv/spirv_markers
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_dispatch
require dasllama/dasllama_cuda_classes   // nv4_prmt_split: the e2m1 decode through the byte permute
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require daslib/templates
require daslib/typemacro_boost
require math
require daslib/math_bits   // uint_bits_to_float: the NVFP4 plane's factor

//! the feed's grid over a width of `n`: sixteen tokens by its superblocks (a Q8_0 row's last one may be short), a subgroup each
def public rows_mma_feed_wgs(n : int64) : int64 => 2l * ((n + 255l) / 256l)

//! the feed off rows already quantized, re-laid in the product's lane order - the quants and scales as they are, so a site
//! reads the feed its N-column leaf would: `ncols` rows of Q8_K quants (a row `n` bytes, a scale a superblock) with their
//! block sums, or under `Q8_0` rows of Q8_0 quants, a scale a 32-block and no sums
struct RowsMmaRelayArgs {
    n : uint
    ncols : uint
}

[ |> template_struct_instance]
class template RowsMmaRelayT {
    @template_constant Q8_0 : bool = false   //!< the source rows are Q8_0: a scale a 32-block at (block, slot), no block sums
    @ssbo @binding = 0 @readonly srcq : array<int8>
    @ssbo @binding = 1 @readonly srcs : array<float>
    @ssbo @binding = 2 outq : array<int8>
    @ssbo @binding = 3 outs : array<float>
    @ssbo @binding = 4 outsum : array<float>
    @kargs pa : RowsMmaRelayArgs

    [spirv_inline]
    def relay_body {
        let nsb = (pa.n + 255u) / 256u   // a Q8_0 row is whole 32-blocks: its last superblock may hold fewer than eight
        let nblk = pa.n / 32u
        let gid = gl_GlobalInvocationID.x
        let lane = gid % 32u
        let wi = gid / 32u
        let c = wi / nsb
        let sb = wi % nsb
        if (c < 16u) {
            let live = c < pa.ncols
            let slot = gk_feed_slot(c)
            var d = 0.0
            static_if (!Q8_0) {
                if (live) {
                    d = srcs[c * nsb + sb]
                }
                if (lane == 0u) {
                    outs[sb * 16u + slot] = d
                }
            }
            for [unroll] (r in range(8)) {
                let blk = sb * 8u + uint(r)
                var q = 0
                if (live && blk < nblk) {
                    q = int(srcq[c * pa.n + blk * 32u + lane])
                }
                if (blk < nblk) {
                    outq[gk_feed_byte(blk, c, lane)] = int8(q)
                }
                static_if (Q8_0) {
                    if (lane == 0u && blk < nblk) {
                        outs[blk * 16u + slot] = live ? srcs[c * nblk + blk] : 0.0
                    }
                } else {
                    let bs = gk_subgroup_add(float(q))
                    if (lane == 0u) {
                        outsum[blk * 16u + slot] = d * bs
                    }
                }
            }
        }
    }
}

[cuda_dispatch(name = "rows_mma_relay", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuRowsMmaRelay : RowsMmaRelayT {
    [cuda_kernel(local_size_x = 256, name = "rows_mma_relay_cuda")]
    def run {
        relay_body()
    }
}

[cuda_dispatch(name = "rows_mma_relay_q8", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuRowsMmaRelayQ8 : RowsMmaRelayT {
    override Q8_0 = true

    [cuda_kernel(local_size_x = 256, name = "rows_mma_relay_q8_cuda")]
    def run {
        relay_body()
    }
}

//! A decode GEMV on the tensor cores at up to sixteen token rows, one weight pass, the body every format stamps: a block of
//! eight warps owns sixteen weight rows, warp w the 32-blocks w, w + 8, .. of the row - K split inside the block, so the
//! warps stream neighbouring lines of the same sixteen rows - and each 32-block is the int8 product m16n8k32 twice, sixteen
//! rows by sixteen tokens. The weight fragment fills from registers: lane (g, t) holds bytes 4t..4t+3 and 16+4t.. of rows
//! g and g + 8 of the block (`w_quants`, the format's own load: no staged tile). The activation fragment is one 16-byte
//! load off the feed (`RowsMmaFeedT`, `RowsMmaRelayT`). A block's s32 products scale by the row's block scale and the
//! token's feed scale (`x_scale_at`: a superblock's on a Q8_K feed, the block's on a Q8_0 one) into the f32 accumulators;
//! a format with a min term (`mins`) takes -min times the feed's block sum, a multiply-add an element. The warps'
//! accumulators meet in shared memory after one barrier and warp 0 stores the sums. A row past the plane redoes the last
//! one, never stored; a row's width is whole superblocks, or on the Q8_0 member whole 32-blocks
[ |> template_struct_instance]
class template RowsMmaT {
    @ssbo @binding = 2 @readonly regions : array<uint>                //! (rel_wblock, xblock) pairs per region; the feed is the whole dispatch's
    @ssbo @binding = 3 @readonly xq4 : array<uint4>                   //! the feed's quants: a 32-block's lane L at blk * 32 + L
    @ssbo @binding = 4 @readonly cs4 : array<float4>                  //! the feed's scales, lane t's four columns at `x_scale_at`
    @ssbo @binding = 5 y : array<float>
    @ssbo @binding = 6 @readonly xm4 : array<float4>                  //! the feed's block sums, lane t's four columns at blk * 4 + t
    @kargs pa : GemvArgs
    @workgroup red : float[2048]   //! the warps' accumulators, [warp][lane][element]

    def abstract w_row_units : uint                             //! the plane units a weight row spans (a region record counts in them)
    def abstract x_scale_at(blk, t : uint) : uint               //! where lane t's column scales of block `blk` sit in the feed
    def x_scale_paired : bool => false                          //! a turn's pair of neighbour blocks reads one feed scale (a superblock's)
    def abstract mins : bool                                    //! the format carries a min term: `w_scale_min`'s second value its factor
    def abstract halves : bool                                  //! the block's two 16-element halves carry scales of their own, `w_scale_min`'s two values: two products a block
    def abstract warp_blk(warp, i : uint) : uint                //! the i-th block warp `warp` of eight owns, ascending in i: the eight warps' blocks are the row's, each once
    def tile_begin : void {}                                    //! a format's own staging ahead of the walk (a decode table in shared memory), its barrier its own
    //! A turn's blocks come in pairs of neighbours: `w_pair_raw` loads the plane words a pair shares once (the default loads
    //! none), and a block's scales and quants read off them and whatever the block alone holds. A plane word loaded twice is
    //! a second wait on its stream: the loads a turn issues bound the leaf
    def w_pair_raw(wrow, blk, t : uint) : uint4 => uint4(0u)
    def abstract w_scale_min(raw : uint4; wrow, blk : uint) : float2      //! the block's scale and its min term's factor, or its two halves' scales
    def abstract w_quants(raw : uint4; wrow, blk, t : uint) : uint2       //! the lane's words of block `blk` of the row at `wrow`: its k 4t.., its k 16 + 4t..
    //! a format whose block scale is one word may load a turn's four in one load - a row's lane t loads block t's, shuffles
    //! hand them around the row (`turn_scales` true, `w_turn_scales` the four) - in place of a load a block
    def turn_scales : bool => false

    def w_turn_scales(wrow, b0, b1, b2, b3, t : uint) : float4 => float4(0.0)

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-rows-leaf")]
    def rows_mma_body {   // nolint:STYLE038 - one register-coupled product loop: the fragments, their scales and the accumulators ride the same lanes
        let nblk = pa.n / 32u
        let tid = gl_LocalInvocationID.x
        let warp = tid >> 5u
        let lane = tid & 31u
        let g = lane >> 2u
        let t = lane & 3u
        let total = pa.nreg * pa.d
        let m0 = gl_WorkGroupID.x * 16u
        let ra = min(m0 + g, total - 1u)
        let rb = min(m0 + g + 8u, total - 1u)
        let wa0 = regions[(ra / pa.d) * 2u] + (ra % pa.d) * w_row_units()
        let wb0 = regions[(rb / pa.d) * 2u] + (rb % pa.d) * w_row_units()
        tile_begin()
        var f : coopmatAcc_f32_16x16
        var bi = 0u
        while (warp_blk(warp, bi) < nblk) {
            // four of the warp's blocks a turn, their loads issued together ahead of the products: the memory in flight a warp
            var qa, qb : uint2[4]
            var sma, smb : float2[4]
            var xv : uint4[4]
            var xm, cs : float4[4]
            var swa, swb : uint4[2]
            for [unroll] (p in range(2)) {
                let blk = min(warp_blk(warp, bi + uint(p * 2)), nblk - 1u)
                swa[p] = w_pair_raw(wa0, blk, t)
                swb[p] = w_pair_raw(wb0, blk, t)
            }
            for [unroll] (u in range(4)) {
                let blk = min(warp_blk(warp, bi + uint(u)), nblk - 1u)   // a turn past the row loads its last block, never multiplied
                qa[u] = w_quants(swa[u / 2], wa0, blk, t)
                qb[u] = w_quants(swb[u / 2], wb0, blk, t)
                if (!turn_scales()) {
                    sma[u] = w_scale_min(swa[u / 2], wa0, blk)
                    smb[u] = w_scale_min(swb[u / 2], wb0, blk)
                }
                xv[u] = xq4[blk * 32u + lane]
                if (x_scale_paired() && (u & 1) != 0) {   // a pair's blocks share a superblock: its feed scale loaded once
                    cs[u] = cs[u - 1]
                } else {
                    cs[u] = cs4[x_scale_at(blk, t)]
                }
                if (mins()) {
                    xm[u] = xm4[blk * 4u + t]
                }
            }
            if (turn_scales()) {   // after the turn's every load is issued: the shuffles wait on their word
                let b0 = min(warp_blk(warp, bi), nblk - 1u)
                let b1 = min(warp_blk(warp, bi + 1u), nblk - 1u)
                let b2 = min(warp_blk(warp, bi + 2u), nblk - 1u)
                let b3 = min(warp_blk(warp, bi + 3u), nblk - 1u)
                let tsa = w_turn_scales(wa0, b0, b1, b2, b3, t)
                let tsb = w_turn_scales(wb0, b0, b1, b2, b3, t)
                sma[0] = float2(tsa.x, 0.0)
                sma[1] = float2(tsa.y, 0.0)
                sma[2] = float2(tsa.z, 0.0)
                sma[3] = float2(tsa.w, 0.0)
                smb[0] = float2(tsb.x, 0.0)
                smb[1] = float2(tsb.y, 0.0)
                smb[2] = float2(tsb.z, 0.0)
                smb[3] = float2(tsb.w, 0.0)
            }
            for [unroll] (u in range(4)) {
                if (warp_blk(warp, bi + uint(u)) < nblk) {
                    var a : coopmatA_s8_16x32
                    mma_frag_set(a, 0, qa[u].x)
                    mma_frag_set(a, 1, qb[u].x)
                    mma_frag_set(a, 2, qa[u].y)
                    mma_frag_set(a, 3, qb[u].y)
                    var bx : coopmatB_s8_32x16
                    mma_frag_set(bx, 0, xv[u].x)
                    mma_frag_set(bx, 1, xv[u].y)
                    mma_frag_set(bx, 2, xv[u].z)
                    mma_frag_set(bx, 3, xv[u].w)
                    if (halves()) {   // the low half's product under its scale, then the high half's: a fragment's other half zero
                        mma_frag_set(a, 2, 0u)
                        mma_frag_set(a, 3, 0u)
                        var sl : coopmatAcc_s32_16x16
                        sl = coopmatMulAdd(a, bx, sl)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
                        mma_scale_acc(f, sl, float2(sma[u].x, smb[u].x), cs[u])
                        mma_frag_set(a, 0, 0u)
                        mma_frag_set(a, 1, 0u)
                        mma_frag_set(a, 2, qa[u].y)
                        mma_frag_set(a, 3, qb[u].y)
                        var sh : coopmatAcc_s32_16x16
                        sh = coopmatMulAdd(a, bx, sh)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
                        mma_scale_acc(f, sh, float2(sma[u].y, smb[u].y), cs[u])
                    } else {
                        var sd : coopmatAcc_s32_16x16
                        sd = coopmatMulAdd(a, bx, sd)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
                        mma_scale_acc(f, sd, float2(sma[u].x, smb[u].x), cs[u])
                    }
                    if (mins()) {
                        mma_acc_set(f, 0, mma_acc_get(f, 0) - sma[u].y * xm[u].x)
                        mma_acc_set(f, 1, mma_acc_get(f, 1) - sma[u].y * xm[u].y)
                        mma_acc_set(f, 2, mma_acc_get(f, 2) - smb[u].y * xm[u].x)
                        mma_acc_set(f, 3, mma_acc_get(f, 3) - smb[u].y * xm[u].y)
                        mma_acc_set(f, 4, mma_acc_get(f, 4) - sma[u].y * xm[u].z)
                        mma_acc_set(f, 5, mma_acc_get(f, 5) - sma[u].y * xm[u].w)
                        mma_acc_set(f, 6, mma_acc_get(f, 6) - smb[u].y * xm[u].z)
                        mma_acc_set(f, 7, mma_acc_get(f, 7) - smb[u].y * xm[u].w)
                    }
                }
            }
            bi += 4u
        }
        for [unroll] (k in range(8)) {
            red[tid * 8u + uint(k)] = mma_acc_get(f, k)
        }
        barrier()
        if (warp == 0u) {
            for [unroll] (k in range(8)) {
                var v = 0.0
                for [unroll] (w in range(8)) {
                    v += red[(uint(w) * 32u + lane) * 8u + uint(k)]
                }
                let row = m0 + mma_acc_row(lane, k)
                let col = mma_acc_col(lane, k)
                if (row < total && col < pa.ncols) {
                    y[pa.ybase + col * pa.ystride + row] = v
                }
            }
        }
    }
}

//! the grid of the rows tensor leaf: a block a tile of sixteen weight rows
def public rows_mma_wgs(rows : int64) : int64 => (rows + 15l) / 16l

//! a superblock format whose 32-block is sixteen nibble bytes, over a Q8_K feed: the plane's words and scale rows, a row's
//! span, the feed's superblock scales, and the warps' blocks - a block is half a memory sector, so a warp owns a pair of
//! neighbours in every other superblock (a sector one warp's, a superblock's scale row four warps')
[ |> template_struct_instance]
class template Sb16RowsMmaT : RowsMmaT {
    @ssbo @binding = 0 @readonly @role = "weight" wq4 : array<uint4>   //! the superblocks as 16-byte words (an arena past 2^32 words stays in reach): block b's 16 nibble bytes at wsb * (sb_words / 4) + b
    @ssbo @binding = 1 @readonly @role = "weight" wsu : array<uint>   //! the scale rows, five words a superblock

    def override w_row_units : uint => pa.n / 256u

    def override x_scale_at(blk, t : uint) : uint => (blk / 8u) * 4u + t

    def override x_scale_paired : bool => true

    def override warp_blk(warp, i : uint) : uint => ((i / 2u) * 2u + (warp >> 2u)) * 8u + (warp & 3u) * 2u + (i & 1u)

    def sb_words : uint => 32u   //! a superblock's quant words: its eight blocks' nibbles, then whatever the format keeps past them

    //! the lane's word of block `blk` of the row at `wrow`: its low nibbles the block's k 4t.., its high nibbles its k 16 + 4t..
    def blk_word(wrow, blk, t : uint) : uint => wq4[(wrow + blk / 8u) * (sb_words() / 4u) + (blk & 7u)][t]

    //! the scale word a pair of neighbour blocks shares under the f16-d scale row (Q4_0, IQ4_NL): two d a word
    def d16_word(wrow, blk : uint) : uint => wsu[(wrow + blk / 8u) * 5u + (blk & 7u) / 2u]
}

//! the K formats under the d | dmin scale row (d | dmin, eight sc bytes, eight mn bytes): a block's scale d x sc and its min
//! term dmin x mn. A pair's raw words: the superblock's d | dmin, the quarter's sc and mn words
[ |> template_struct_instance]
class template KmRowsMmaT : Sb16RowsMmaT {
    def override w_pair_raw(wrow, blk, t : uint) : uint4 {
        let h = (wrow + blk / 8u) * 5u
        let q = (blk & 7u) / 4u
        return uint4(wsu[h], wsu[h + 1u + q], wsu[h + 3u + q], 0u)
    }

    def override w_scale_min(raw : uint4; wrow, blk : uint) : float2 => k4_scale_min(unpackHalf2x16(raw.x), raw.y, raw.z, blk & 7u)

    def override mins : bool => true

    def override halves : bool => false
}

//! Q4_K on the rows tensor leaf: the nibbles as they are
[ |> template_struct_instance]
class template K4RowsMmaT : KmRowsMmaT {
    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 {
        let w = blk_word(wrow, blk, t)
        return uint2(nib_word(w, 0u), nib_word(w, 4u))
    }
}

//! Q5_K on the rows tensor leaf: the nibbles with their fifth bits, a block's 32 of them one word past the superblock's
//! nibbles - byte lane t's low four the bits of the block's k 4t.., its high four of its k 16 + 4t.. (`k5_word`)
[ |> template_struct_instance]
class template K5RowsMmaT : KmRowsMmaT {
    def override sb_words : uint => 40u

    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 {
        let w = blk_word(wrow, blk, t)
        let qh = wq4[(wrow + blk / 8u) * 10u + 8u + ((blk & 7u) >> 2u)][blk & 3u] >> (t * 8u)
        return uint2(k5_word(w, qh, 0u), k5_word(w, qh >> 4u, 4u))
    }
}

//! Q6_K on the rows tensor leaf: the nibbles with their two high bits - the qh words past the superblock's nibbles, a 2-bit
//! lane a block of each byte (`k6_word`) - less the format's offset of 32 as signed bytes, so no min term; each 16-element
//! half under its own signed sub-scale and the superblock's d: two products a block. A pair's raw words: its sub-scale word,
//! the superblock's d, and the lane's two qh words (a quarter's four blocks share them)
[ |> template_struct_instance]
class template K6RowsMmaT : Sb16RowsMmaT {
    def override sb_words : uint => 48u

    //! four 6-bit quants (a byte each, 0 .. 63) less 32 as signed bytes, no carry across the lanes
    def k6_centered(q : uint) : uint => ((q | 0x80808080u) - 0x20202020u) ^ 0x80808080u

    def override w_pair_raw(wrow, blk, t : uint) : uint4 {
        let h = (wrow + blk / 8u) * 5u
        let b = blk & 7u
        let qh = (wrow + blk / 8u) * 12u + 8u + (b / 4u) * 2u
        return uint4(wsu[h + b / 2u], wsu[h + 4u], wq4[qh][t], wq4[qh + 1u][t])
    }

    def override w_scale_min(raw : uint4; wrow, blk : uint) : float2 {
        let d = unpackHalf2x16(raw.y).x
        return float2(d * float(k6_sc(raw.x, (blk & 1u) * 2u)), d * float(k6_sc(raw.x, (blk & 1u) * 2u + 1u)))
    }

    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 {
        let w = blk_word(wrow, blk, t)
        let sh = (blk & 3u) * 2u
        return uint2(k6_centered(k6_word(nib_word(w, 0u), raw.z, sh)), k6_centered(k6_word(nib_word(w, 4u), raw.w, sh)))
    }

    def override mins : bool => false

    def override halves : bool => true
}

//! NVFP4 on the rows tensor leaf: the e2m1 nibbles as signed bytes of their doubled magnitudes through the byte permute
//! (`nv4_prmt_split`), each of the block's two 16-element halves under its own UE4M3 scale - read off a table of the 256
//! codes the block stages once - and the plane's factor (the scale row's fifth word): two products a block, no min term.
//! A pair's raw words: its scale word and the plane's factor
[ |> template_struct_instance]
class template Nv4RowsMmaT : Sb16RowsMmaT {
    @workgroup ue4 : float[256]   //! the UE4M3 codes at half their value (`ue4m3_half_f32`)

    def override tile_begin : void {
        ue4[gl_LocalInvocationID.x] = ue4m3_half_f32(gl_LocalInvocationID.x)
        barrier()
    }

    def override w_pair_raw(wrow, blk, t : uint) : uint4 {
        let h = (wrow + blk / 8u) * 5u
        return uint4(wsu[h + (blk & 7u) / 2u], wsu[h + 4u], 0u, 0u)
    }

    def override w_scale_min(raw : uint4; wrow, blk : uint) : float2 {
        let sw = raw.x >> ((blk & 1u) * 16u)
        let site = uint_bits_to_float(raw.y)
        return float2(ue4[sw & 0xFFu] * site, ue4[(sw >> 8u) & 0xFFu] * site)
    }

    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 {
        var lo = 0u
        var hi = 0u
        nv4_prmt_split(blk_word(wrow, blk, t), lo, hi)
        return uint2(lo, hi)
    }

    def override mins : bool => false

    def override halves : bool => true
}

//! the codebook formats - a block one scale over signed nibble codes, no min term: the lane's word through the format's
//! code map (`code_word`), the block's scale off the pair's raw words (`blk_scale`)
[ |> template_struct_instance]
class template CodeRowsMmaT : Sb16RowsMmaT {
    def abstract code_word(n : uint) : uint                    //! four nibble codes (a byte each) as the format's signed int8 values
    def abstract blk_scale(raw : uint4; blk : uint) : float    //! block `blk`'s scale off its pair's raw words

    def override w_scale_min(raw : uint4; wrow, blk : uint) : float2 => float2(blk_scale(raw, blk & 7u), 0.0)

    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 {
        let w = blk_word(wrow, blk, t)
        return uint2(code_word(nib_word(w, 0u)), code_word(nib_word(w, 4u)))
    }

    def override mins : bool => false

    def override halves : bool => false
}

//! Q4_0 on the rows tensor leaf: the nibbles as they are under the block's f16 d, the format's offset of 8 a min term of
//! 8 d against the feed's block sum - a pair's two d one word
[ |> template_struct_instance]
class template Q40RowsMmaT : Sb16RowsMmaT {
    def override w_pair_raw(wrow, blk, t : uint) : uint4 => uint4(d16_word(wrow, blk), 0u, 0u, 0u)

    def override w_scale_min(raw : uint4; wrow, blk : uint) : float2 {
        let d = q40_d_of(raw.x, blk & 7u)
        return float2(d, 8.0 * d)
    }

    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 {
        let w = blk_word(wrow, blk, t)
        return uint2(nib_word(w, 0u), nib_word(w, 4u))
    }

    def override mins : bool => true

    def override halves : bool => false
}

//! IQ4_NL on the rows tensor leaf: Q4_0's scale row under the codebook's bytes, through the byte permute (`cud_iq4_word`)
[ |> template_struct_instance]
class template Iq4nlRowsMmaT : CodeRowsMmaT {
    def override w_pair_raw(wrow, blk, t : uint) : uint4 => uint4(d16_word(wrow, blk), 0u, 0u, 0u)

    def override blk_scale(raw : uint4; blk : uint) : float => q40_d_of(raw.x, blk)

    def override code_word(n : uint) : uint => cud_iq4_word(n)
}

//! IQ4_XS on the rows tensor leaf: the codebook's bytes under the superblock's d and the block's signed 6-bit sub-scale,
//! the scale row's two words a pair's raw words
[ |> template_struct_instance]
class template Iq4xsRowsMmaT : CodeRowsMmaT {
    def override w_pair_raw(wrow, blk, t : uint) : uint4 => uint4(wsu[(wrow + blk / 8u) * 2u], wsu[(wrow + blk / 8u) * 2u + 1u], 0u, 0u)

    def override blk_scale(raw : uint4; blk : uint) : float => unpackHalf2x16(raw.x).x * float(iq4xs_sc(raw.x, raw.y, blk))

    def override code_word(n : uint) : uint => cud_iq4_word(n)
}

//! IQ3_S4 on the rows tensor leaf: IQ4_XS's tiling under the odd codebook 2q - 15 and the grid row's (1 + 2s) sub-scale
[ |> template_struct_instance]
class template Iq3s4RowsMmaT : Iq4xsRowsMmaT {
    def override blk_scale(raw : uint4; blk : uint) : float => unpackHalf2x16(raw.x).x * float(grid_sc(raw.y, blk))

    def override code_word(n : uint) : uint => iq3s4_word(n)
}

//! Q8_0 on the rows tensor leaf, over a Q8_0 feed: a block's 32 quants eight words, the lane's two its k 4t.. and its
//! k 16 + 4t.., the block's f16 scale, no min term
[ |> template_struct_instance]
class template Q8RowsMmaT : RowsMmaT {
    @ssbo @binding = 0 @readonly @role = "weight" wq4 : array<uint4>     //! Q8_0 blocks as 16-byte words: block wb's 32 quants at (wb * 2, wb * 2 + 1)
    @ssbo @binding = 1 @readonly @role = "weight" wsh : array<float16>   //! per-block f16 weight scales

    def override w_row_units : uint => pa.n / 32u

    def override w_quants(raw : uint4; wrow, blk, t : uint) : uint2 => uint2(wq4[(wrow + blk) * 2u][t], wq4[(wrow + blk) * 2u + 1u][t])

    @template_constant COOP : bool = false   //!< the big-plane stamp: a turn's four block scales in one load, shuffled around the row

    def override w_scale_min(raw : uint4; wrow, blk : uint) : float2 => float2(float(wsh[wrow + blk]), 0.0)

    def override turn_scales : bool => COOP

    def override w_turn_scales(wrow, b0, b1, b2, b3, t : uint) : float4 {
        let own = float(wsh[wrow + (t == 0u ? b0 : (t == 1u ? b1 : (t == 2u ? b2 : b3)))])
        let row0 = gl_SubgroupInvocationID & 0xFFFFFFFCu
        return float4(subgroupShuffle(own, row0), subgroupShuffle(own, row0 + 1u), subgroupShuffle(own, row0 + 2u), subgroupShuffle(own, row0 + 3u))
    }

    def override x_scale_at(blk, t : uint) : uint => blk * 4u + t

    def override mins : bool => false

    def override halves : bool => false

    def override warp_blk(warp, i : uint) : uint => i * 8u + warp   //! a block is a memory sector: K split a block a warp
}

[cuda_dispatch(name = "kq_rows_k4_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaK4 : K4RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_k4_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_k5_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaK5 : K5RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_k5_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_k6_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaK6 : K6RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_k6_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_nv4_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaNv4 : Nv4RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_nv4_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_q40_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaQ40 : Q40RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_q40_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_iq4nl_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaIq4nl : Iq4nlRowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_iq4nl_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_iq4xs_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaIq4xs : Iq4xsRowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_iq4xs_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "kq_rows_iq3s4_mma", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuKqRowsMmaIq3s4 : Iq3s4RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "kq_rows_iq3s4_mma_cuda")]
    def run {
        rows_mma_body()
    }
}

[cuda_dispatch(name = "q8_rows_mma_n", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8RowsMmaN : Q8RowsMmaT {
    [cuda_kernel(local_size_x = 256, name = "q8_rows_mma_n_cuda")]
    def run {
        rows_mma_body()
    }
}

//! the Q8_0 leaf's big-plane stamp (`COOP`): wide rows of a plane that waits on memory, where one scale load a turn wins
[cuda_dispatch(name = "q8_rows_mma_w", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8RowsMmaW : Q8RowsMmaT {
    override COOP = true

    [cuda_kernel(local_size_x = 256, name = "q8_rows_mma_w_cuda")]
    def run {
        rows_mma_body()
    }
}

//! the plane the Q8_0 leaf's big-plane stamp takes: rows wide enough for whole turns and bytes enough to wait on memory. It
//! wins a tenth on an 85 MB plane of 5120-wide rows and loses on a 2B model's planes - its 9 MB ones, and its 400 MB
//! classifier of 1536-wide rows, a turn and a half a warp
let CUD_Q8_ROWS_COOP_N = 4096u
let CUD_Q8_ROWS_COOP_BYTES = 16l * 1024l * 1024l

//! both Q8_0 leaf stamps made ready
def ensure_q8_rows_mma : bool => ensure_q8_rows_mma_n() && ensure_q8_rows_mma_w()

//! the Q8_0 leaf's launch: the big-plane stamp from `CUD_Q8_ROWS_COOP_N` wide and `CUD_Q8_ROWS_COOP_BYTES` of quants
def enc_q8_rows_mma(var s : void?; bufs : uint64 const[7]; var pc : GemvArgs; wgs : int64) {
    if (pc.n >= CUD_Q8_ROWS_COOP_N && int64(pc.n) * int64(pc.d) * int64(pc.nreg) >= CUD_Q8_ROWS_COOP_BYTES) {
        enc_q8_rows_mma_w(s, bufs, pc, wgs)
    } else {
        enc_q8_rows_mma_n(s, bufs, pc, wgs)
    }
}

//! the formats with no rows tensor leaf: the superblock formats off the sixteen-nibble block (Q3_K, Q2_K), the grid formats
//! (a codebook the leaf binds no plane for), the per-32 formats and MXFP4 (no dense plane)
let private CUD_ROWS_MMA_SKIP = "k3 k2 iq3s iq3xxs iq2s iq2xs iq2xxs q51 q4032 q5032 iq4nl32 mx4"
let private CUD_ROWS_MMA_TAILS = "mma"

//! format `fmt` has a rows tensor leaf
def cud_rows_mma_stamped(fmt : int) : bool {
    let tail = 0
    return kq_tile_stamped(fmt, CUD_ROWS_MMA_SKIP, tail, CUD_ROWS_MMA_TAILS)
}

//! the feed a format's leaf reads is the Q8_0 one (a scale a 32-block, no block sums): else the Q8_K one
def cud_rows_mma_q8_feed(fmt : int) : bool => kq_fmt_of_slot(fmt) == KqFmt.q8

//! the leaf's stamp of `fmt` made ready, with the feed's re-lay of its class
def cud_rows_mma_ensure(fmt : int) : bool {
    return false if (!cud_rows_mma_stamped(fmt) || !(cud_rows_mma_q8_feed(fmt) ? ensure_rows_mma_relay_q8() : ensure_rows_mma_relay()))
    var r = false
    kq_tile_stamp(fmt, CUD_ROWS_MMA_SKIP, "rows") <| $() {
        r = tile_ensure_mma()
    }
    return r
}

//! the leaf's launch of `fmt` over its seven planes
def cud_rows_mma_enc(fmt : int; var s : void?; bufs : uint64 const[7]; var pc : GemvArgs; wgs : int64) {
    kq_tile_stamp(fmt, CUD_ROWS_MMA_SKIP, "rows") <| $() {
        tile_enc_mma(s, bufs, pc, wgs)
    }
}

//! the re-lay of rows already quantized into the leaf's feed, in the feed class `fmt`'s leaf reads
def cud_rows_mma_relay_enc(fmt : int; var s : void?; bufs : uint64 const[5]; n : int64; ncols : uint) {
    var pcr = RowsMmaRelayArgs(n = uint(n), ncols = ncols)
    if (cud_rows_mma_q8_feed(fmt)) {
        enc_rows_mma_relay_q8(s, bufs, pcr, rows_mma_feed_wgs(n))
    } else {
        enc_rows_mma_relay(s, bufs, pcr, rows_mma_feed_wgs(n))
    }
}

//! the requants that write the leaf's feed beside their rows (`RqPlainT` and `ActRqT` under `OUT_FEED`): the float rows into Q8_0 blocks,
//! into Q8_K superblocks, and the gate / up pair's activation into either - a producer's one launch where a requant and a
//! re-lay were two
[cuda_dispatch(name = "cls_dn_rq_feed", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDnRequantFeed : RqPlainT {
    override OUT_FEED = true

    [cuda_kernel(local_size_x = 256, name = "cls_dn_rq_feed_cuda")]
    def run {
        rq_body()
    }
}

//! `RqPlainT` writing the leaf's feed beside its Q8_K superblocks
[cuda_dispatch(name = "cls_q8k_rq_feed", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8kRequantFeed : RqPlainT {
    override KBLK = true
    override OUT_FEED = true

    [cuda_kernel(local_size_x = 256, name = "cls_q8k_rq_feed_cuda")]
    def run {
        rq_body()
    }
}

//! `ArRqBase`'s rows form writing the rows tensor leaf's feed beside its Q8_0 blocks (`GkQ8BlockStore`'s `OUT_FEED`): a row
//! a workgroup, the row its token row of the feed
[ |> template_struct_instance]
class template ArRqFeedT : ArRqBase {
    override OUT_FEED = true
    @ssbo @binding = 4 @role = "write" fq : array<int8>     //! the feed's quants in lane order
    @ssbo @binding = 5 @role = "write" fs : array<float>    //! ... the scales
    @ssbo @binding = 6 @role = "write" fm : array<float>    //! ... the block sums (a Q8_K feed's; unwritten here)

    def override feed_units : uint => pa.dim / KQ_B32_ELEMS_U
}

//! the residual step fused with its Q8_0 requant over token rows, writing the leaf's feed in the same launch (`ArRqBase` through `ArRqFeedT`)
[cuda_dispatch(name = "cls_ar_rq_feed_b", kernel = "run", grid = "nrows", params = "nrows : int64")]
class CuArRqFeedB : ArRqFeedT {
    override LANES = 1024u
    [cuda_kernel(local_size_x = 1024, name = "cls_ar_rq_feed_b_cuda")]
    def run {
        rq_b_body()
    }
}

//! `ActRqT` writing the leaf's feed beside its Q8_0 blocks
[cuda_dispatch(name = "q8_actrq_feed_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8ActRqFeed : ActRqT {
    override OUT_FEED = true

    [cuda_kernel(local_size_x = 256, name = "q8_actrq_feed_cls_cuda")]
    def run {
        rq_body()
    }
}

//! `ActRqT` writing the leaf's feed beside its Q8_K superblocks
[cuda_dispatch(name = "q8k_actrq_feed_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8kActRqFeed : ActRqT {
    override KBLK = true
    override OUT_FEED = true

    [cuda_kernel(local_size_x = 256, name = "q8k_actrq_feed_cls_cuda")]
    def run {
        rq_body()
    }
}
