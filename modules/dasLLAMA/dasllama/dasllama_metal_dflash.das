options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_dflash shared private

require dasllama/dasllama_lint public
require math
require daslib/typemacro_boost
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math   // reserve_resize - the landing's sizing
require dasllama/dasllama_dflash
require dasllama/dasllama_batch   // verify_batch_groups: the joint round's verify
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_lens   // release_handles, compile_stamp
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // the rows8 form and the eight-row tensor stamps the pass's rows ride
require dasllama/dasllama_metal_decode   // enc_block_cls_rows - the trunk's classifier over the draft rows
require dasllama/dasllama_layout   // metal_kq_scale_form - a K-quant plane's device scale form
require dasllama/dasllama_env   // g_env_metal.rows_mm8 - the eight-row stamps' A/B rail

//! the dynamic two-tap grouped convolution's arguments; `side` 0 is the sublayer's input, 1 its output
struct DflashConvArgs {
    dim : uint
    g : uint       //! channels a dynamic coefficient
    ng : uint      //! coefficients a tap
    projw : uint   //! a row's projection width, [side][tap][group]
    side : uint
    convk : uint   //! taps
    rows : uint
    blk : uint     //! rows a block where the panel holds several blocks (0 = one): a block's first row has no predecessor
}

//! row t of `y` = (base[side][0] + dyn[t][side][0]) * x[t] + (base[side][1] + dyn[t][side][1]) * x[t - 1], a dynamic coefficient a
//! group of `g` channels; out of place, since row t reads row t - 1 as it was
[metal_dispatch(name = "enc_dflash_conv", pso = "g_df_pso_conv", tg = 256, grid = "total/256", params = "total : int64")]
class MetalDflashConv {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @ssbo @binding = 2 @role = "read" dyn : array<float>
    @ssbo @binding = 3 @role = "weight" @off = "boff" base : array<float>
    @uniform @binding = 4 ka : DflashConvArgs

    [metal_kernel(name = "metal_dflash_conv_msl")]
    def metal_dflash_conv {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.rows * ka.dim) {
            return
        }
        let t = gid / ka.dim
        let c = gid % ka.dim
        let gi = c / ka.g
        let tap0 = ka.side * ka.convk
        var o = (base[tap0 * ka.dim + c] + dyn[t * ka.projw + tap0 * ka.ng + gi]) * x[gid]
        if ((ka.blk == 0u ? t : t % ka.blk) > 0u) {
            o += (base[(tap0 + 1u) * ka.dim + c] + dyn[t * ka.projw + (tap0 + 1u) * ka.ng + gi]) * x[gid - ka.dim]
        }
        y[gid] = o
    }
}

//! the head norm and rope's arguments: rows `xs` floats apart in, `ys` apart out, row 0 at position `pos0`
struct DflashQknArgs {
    hs : uint
    xs : uint
    ys : uint
    pos0 : uint
    eps : float
}

//! a head of every row rms-normed under `w` and rotated (the NEOX pairs i, i + hs/2) at the row's position; `y` may be `x`
//! itself, and the ring where the commit's K rows land
[metal_dispatch(name = "enc_dflash_qkn", pso = "g_df_pso_qkn", tgmem = "metal_dflash_qkn_msl_tgmem", tg = "hs", grid = "nheads, nrows",
    params = "nheads : int64, nrows : int64, hs : int64")]
class MetalDflashQkn {
    @ssbo @binding = 0 @role = "read" @off = "xoff" x : array<float>
    @ssbo @binding = 1 @role = "weight" @off = "woff" w : array<float>
    @ssbo @binding = 2 @role = "weight" invf : array<float>   //! [hs/2] the rope's inverse frequencies
    @ssbo @binding = 3 @role = "write" @off = "yoff" y : array<float>   //! x itself in place, or the ring
    @uniform @binding = 4 ka : DflashQknArgs
    @workgroup sh : float[256]
    @workgroup red : float[16]

    [metal_kernel(name = "metal_dflash_qkn_msl")]
    def metal_dflash_qkn {
        let h = gl_WorkGroupID.x
        let t = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x
        let hs = ka.hs
        let v = x[t * ka.xs + h * hs + i]
        let inv = rsqrt(tg_sum_all(v * v, red) / float(hs) + ka.eps)
        sh[i] = v * inv * w[i]
        barrier()
        let half = hs / 2u
        if (i < half) {
            let a = sh[i]
            let b = sh[i + half]
            let ang = float(ka.pos0 + t) * invf[i]
            let c = cos(ang)
            let s = sin(ang)
            let d = t * ka.ys + h * hs
            y[d + i] = a * c - b * s
            y[d + half + i] = a * s + b * c
        }
    }
}

//! the block attention's arguments: the ring's positions `lo .. lo + nctx - 1` then the block's `rows` at `pos0 ..`
struct DflashAttnArgs {
    rows : uint
    pos0 : uint
    lo : uint
    nctx : uint
    w : uint       //! the window: a key |q - k| >= w is masked
    hs : uint
    nheads : uint
    grp : uint     //! q heads a kv head
    kvd : uint
    qd : uint
    scale : float
    rbase : uint   //! the block's first row of the q, K, V and output panels (several blocks a panel)
}

let private DFLASH_ATTN_KEYS = 2304   //! the scores a threadgroup stages: a window of 2048 and the block's rows

//! the block's attention, a threadgroup a (head, row) of `hs` lanes: the scores over the ring's positions in view then the block's
//! own rows (both ways, as the block is bidirectional), the softmax, the weighted V sum - a lane a key stripe, then a channel
[metal_dispatch(name = "enc_dflash_attn", pso = "g_df_pso_attn", tgmem = "metal_dflash_attn_msl_tgmem", tg = "hs", grid = "nheads, nrows",
    params = "nheads : int64, nrows : int64, hs : int64")]
class MetalDflashAttn {
    @ssbo @binding = 0 @role = "read" q : array<float>    //! [rows][qd]
    @ssbo @binding = 1 @role = "read" kb : array<float>   //! the block's K, [rows][kvd]
    @ssbo @binding = 2 @role = "read" vb : array<float>
    @ssbo @binding = 3 @role = "read" @off = "koff" rk : array<float>   //! the layer's ring K, [w][kvd]
    @ssbo @binding = 4 @role = "read" @off = "voff" rv : array<float>
    @ssbo @binding = 5 @role = "write" o : array<float>   //! [rows][qd]
    @uniform @binding = 6 ka : DflashAttnArgs
    @workgroup qs : float[256]
    @workgroup sc : float[2304]
    @workgroup red : float[16]

    [metal_kernel(name = "metal_dflash_attn_msl")]
    def metal_dflash_attn {
        let h = gl_WorkGroupID.x
        let t = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x
        let hs = ka.hs
        let kh = (h / ka.grp) * hs
        let qpos = int(ka.pos0 + t)
        qs[i] = q[(ka.rbase + t) * ka.qd + h * hs + i]
        barrier()
        let nkeys = ka.nctx + ka.rows
        var mx = -3.0e38
        var j = i
        while (j < nkeys) {
            var s = -3.0e38
            var dot = 0.0
            if (j < ka.nctx) {
                let kpos = int(ka.lo + j)
                if (qpos - kpos < int(ka.w) && kpos - qpos < int(ka.w)) {
                    let kb0 = (uint(kpos) % ka.w) * ka.kvd + kh
                    var e = 0u
                    while (e < hs) {
                        dot += qs[e] * rk[kb0 + e]
                        e++
                    }
                    s = dot * ka.scale
                }
            } else {
                let kpos = int(ka.pos0 + (j - ka.nctx))
                if (qpos - kpos < int(ka.w) && kpos - qpos < int(ka.w)) {
                    let kb0 = (ka.rbase + j - ka.nctx) * ka.kvd + kh
                    var e = 0u
                    while (e < hs) {
                        dot += qs[e] * kb[kb0 + e]
                        e++
                    }
                    s = dot * ka.scale
                }
            }
            sc[j] = s
            mx = max(mx, s)
            j += hs
        }
        let mall = tg_max_all(mx, red)
        barrier()
        var sum = 0.0
        j = i
        while (j < nkeys) {
            let p = sc[j] <= -1.0e38 ? 0.0 : exp(sc[j] - mall)
            sc[j] = p
            sum += p
            j += hs
        }
        let tot = tg_sum_all(sum, red)
        var acc = 0.0
        j = 0u
        while (j < ka.nctx) {
            let p = sc[j]
            if (p != 0.0) {
                acc += p * rv[((ka.lo + j) % ka.w) * ka.kvd + kh + i]
            }
            j++
        }
        while (j < nkeys) {
            let p = sc[j]
            if (p != 0.0) {
                acc += p * vb[(ka.rbase + j - ka.nctx) * ka.kvd + kh + i]
            }
            j++
        }
        o[(ka.rbase + t) * ka.qd + h * hs + i] = acc / tot
    }
}

let private DFLASH_SEL_K = 32         //! the candidates a position the device selector keeps at most
let private DFLASH_SEL_RANK = 1024    //! the codebook rank the walk stages at most
let private DFLASH_SEL_PRODS = 4096   //! the (candidate, i) products the walk stages a position, top-k x rank at most
let private DFLASH_SEL_SPAN = 4096    //! the logits a slice threadgroup of the top-k reads
let private DFLASH_SEL_NONE = 0xFFFFFFFFu   //! a candidate slot no id filled (a row narrower than the top-k)
let private DFLASH_Q8_BLOCK = 32l           //! a Q8_0 block's elements

//! the top-k's arguments: rows `vocab` entries wide, `k` candidates a list; the slice stage's threadgroup takes `span` entries
//! of a row and lands its list at slot `nsl` * row + slice
struct public DflashTopkArgs {
    vocab : uint
    k : uint
    span : uint
    nsl : uint
}

//! The `k` best entries of a run under (value desc, id asc), best first: a lane keeps its stride's best `k` sorted, then `k`
//! rounds pick the threadgroup's best head and its lane steps past it; NaN never enters. The slice stage reads a row's logits
//! (the id the index), the merge stage the slices' lists (the id read beside the value)
[ |> template_struct_instance]
class template public MetalDflashTopkT {
    @ssbo @binding = 0 @role = "read" lg : array<float>     //! the values
    @ssbo @binding = 1 @role = "read" sid : array<uint>     //! the merge's ids beside them
    @ssbo @binding = 2 @role = "write" oid : array<uint>    //! the lists' ids
    @ssbo @binding = 3 @role = "write" oval : array<float>  //! ... and values
    @uniform @binding = 4 ka : DflashTopkArgs
    @workgroup redv : float[32]
    @workgroup redi : uint[32]
    @workgroup win : uint[1]

    //! the best `k` of entries `lo .. hi` past `base` into the list at `obase`; `ids` reads each entry's id from `sid`
    def top_k_list(base, lo, hi, obase : uint; ids : bool) {
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let last = ka.k - 1u
        var lv : float[32]
        var li : uint[32]
        var q = 0u
        while (q < ka.k) {
            lv[q] = -3.4e38
            li[q] = 0xFFFFFFFFu
            q++
        }
        var j = lo + lid
        while (j < hi) {
            let v = lg[base + j]
            let id = ids ? sid[base + j] : j
            if (v > lv[last] || (v == lv[last] && id < li[last])) {
                var p = last
                while (p > 0u && (v > lv[p - 1u] || (v == lv[p - 1u] && id < li[p - 1u]))) {
                    lv[p] = lv[p - 1u]
                    li[p] = li[p - 1u]
                    p--
                }
                lv[p] = v
                li[p] = id
            }
            j += nt
        }
        var h = 0u
        var r = 0u
        while (r < ka.k) {
            var bv = -3.4e38
            var bi = 0xFFFFFFFFu
            if (h < ka.k) {
                bv = lv[h]
                bi = li[h]
            }
            var m = 16u
            while (m >= 1u) {
                let ov = simd_shuffle_xor(bv, m)
                let oi = simd_shuffle_xor(bi, m)
                if (ov > bv || (ov == bv && oi < bi)) {
                    bv = ov
                    bi = oi
                }
                m /= 2u
            }
            if (gl_SubgroupInvocationID == 0u) {
                redv[gl_SubgroupID] = bv
                redi[gl_SubgroupID] = bi
            }
            barrier()
            if (lid == 0u) {
                var wv = redv[0]
                var wi = redi[0]
                var sgi = 1u
                while (sgi < gl_NumSubgroups) {
                    if (redv[sgi] > wv || (redv[sgi] == wv && redi[sgi] < wi)) {
                        wv = redv[sgi]
                        wi = redi[sgi]
                    }
                    sgi++
                }
                win[0] = wi
                oid[obase + r] = wi
                oval[obase + r] = wv
            }
            barrier()
            if (h < ka.k && li[h] == win[0]) {
                h++
            }
            r++
        }
    }
}

//! the top-k's first stage, a threadgroup a `span`-long slice of a row's logits: the slice's list into `oid` / `oval`
[metal_dispatch(name = "enc_dflash_topk_slices", pso = "g_df_pso_topk_slices", tgmem = "metal_dflash_topk_slices_msl_tgmem", tg = 256,
    grid = "nslices, nrows", params = "nslices : int64, nrows : int64")]
class public MetalDflashTopkSlices : MetalDflashTopkT {
    [metal_kernel(name = "metal_dflash_topk_slices_msl")]
    def metal_dflash_topk_slices {
        let row = gl_WorkGroupID.y
        let sl = gl_WorkGroupID.x
        let lo = sl * ka.span
        top_k_list(row * ka.vocab, lo, min(lo + ka.span, ka.vocab), (row * ka.nsl + sl) * ka.k, false)
    }
}

//! the top-k's second stage, a threadgroup a row: its slices' lists (`vocab` = slices x k entries) merged into the row's list -
//! the host's `dflash_top_k_row` bit for bit
[metal_dispatch(name = "enc_dflash_topk_merge", pso = "g_df_pso_topk_merge", tgmem = "metal_dflash_topk_merge_msl_tgmem", tg = 256,
    grid = "nrows", params = "nrows : int64")]
class public MetalDflashTopkMerge : MetalDflashTopkT {
    [metal_kernel(name = "metal_dflash_topk_merge_msl")]
    def metal_dflash_topk_merge {
        let row = gl_WorkGroupID.x
        top_k_list(row * ka.vocab, 0u, ka.vocab, row * ka.k, true)
    }
}

//! a signed byte as a float
def private dflash_sbyte(b : uint) : float => float(int(b) - (b >= 128u ? 256 : 0))

//! element `i` of a selector codebook's row `row` (`rank` wide) into `v`, in the host's dequant arithmetic: a Q8_0 block's q * d
//! (`fmt` 0), a Q6_K's (d * sc) * q (6), a Q4_K / Q5_K's (d * sc) * q - dmin * m (4, 5); `q` the quant bytes from `q0` and `qh`
//! their halves, `s` / `sh` a K-quant codebook's own scale rows in the CPU form; every half on an even byte
def private dflash_cb_into(q : array<uint8>; qh : array<float16>; s : array<uint8>; sh : array<float16>; fmt, rank, q0, row, i : uint;
                           var v : float&) {
    let e = row * rank + i
    let sb = e / 256u
    let w = e % 256u
    if (fmt == 0u) {
        let bo = q0 + row * (rank / 32u) * 34u + (i / 32u) * 34u
        let d = float(qh[bo >> 1u])
        let qb = uint(q[bo + 2u + i % 32u])
        v = dflash_sbyte(qb) * d
    } elif (fmt == 6u) {
        let hf = w / 128u
        let qd = (w % 128u) / 32u
        let l = w % 32u
        let lb = uint(q[q0 + sb * 192u + hf * 64u + (qd & 1u) * 32u + l])
        let hl = uint(q[q0 + sb * 192u + 128u + hf * 32u + l])
        let lo = qd >= 2u ? (lb >> 4u) : (lb & 15u)
        let qv = int(lo | (((hl >> (qd * 2u)) & 3u) << 4u)) - 32
        let d = float(sh[(sb * 18u + 16u) >> 1u])
        let scb = uint(s[sb * 18u + hf * 8u + l / 16u + qd * 2u])
        let dsc = d * dflash_sbyte(scb)
        v = dsc * float(qv)
    } else {
        let qsb = fmt == 5u ? 160u : 128u
        let js = w / 64u
        let hi = (w % 64u) / 32u
        let l = w % 32u
        let b = uint(q[q0 + sb * qsb + js * 32u + l])
        var qv = hi == 1u ? (b >> 4u) : (b & 15u)
        if (fmt == 5u) {
            let u = (hi == 1u ? 2u : 1u) << (js * 2u)
            if ((uint(q[q0 + sb * 160u + 128u + l]) & u) != 0u) {
                qv += 16u
            }
        }
        let so = sb * 20u
        let j = js * 2u + hi
        var sc = 0u
        var mn = 0u
        if (j < 4u) {
            sc = uint(s[so + 4u + j]) & 63u
            mn = uint(s[so + 8u + j]) & 63u
        } else {
            let sh8 = uint(s[so + 8u + j])
            sc = (sh8 & 15u) | ((uint(s[so + j]) >> 6u) << 4u)
            mn = (sh8 >> 4u) | ((uint(s[so + 4u + j]) >> 6u) << 4u)
        }
        let dj = float(sh[so >> 1u]) * float(sc)
        let mj = float(sh[(so + 2u) >> 1u]) * float(mn)
        let dq = dj * float(qv)
        v = dq - mj
    }
}

//! the walk's arguments: a codebook's format is 0 (Q8_0 rows), 4, 5 or 6 (Q4_K, Q5_K, Q6_K superblocks); its rows start `pq0` /
//! `nq0` bytes into its quant binding, a K-quant codebook's scale rows at its scale binding's start in the CPU form
struct public DflashWalkArgs {
    rank : uint
    k : uint
    nd : uint
    pfmt : uint
    nfmt : uint
    pq0 : uint
    nq0 : uint
    temp : float   //! the sampled stamp's drafter temperature (above 0)
    tbase : uint   //! the anchor's index in `tok` (a stream's block in a joint pass)
    rbase : uint   //! the walk's first row of `hsel` / `cid` / `cval`: stream b's b nd, its anchor at b (nd + 1) of `ids`, the picks past it
}

//! The selector's walk in one threadgroup: A[pred] staged, every (A[pred][i] * h_r[i]) * B[cand][i] staged, a lane a candidate
//! summing logit + its products in ascending i over codebooks in the host's dequant arithmetic; the pick the first best slot
//! (`dflash_walk`'s), or under `SOFT` one LCG draw from the candidates' softmax at `temp`, q landed (`dflash_soft_pick`'s)
[ |> template_struct_instance]
class template public MetalDflashWalkT {
    @template_constant SOFT : bool = false
    @ssbo @binding = 0 @role = "read" tok : array<uint>       //! the anchor at 0
    @ssbo @binding = 1 @role = "read" hsel : array<float>     //! [nd][rank]
    @ssbo @binding = 2 @role = "read" cid : array<uint>       //! [nd][k]
    @ssbo @binding = 3 @role = "read" cval : array<float>
    @ssbo @binding = 4 @role = "weight" @off = "pqoff" pq : array<uint8>    //! the predecessor codebook's quant bytes
    @ssbo @binding = 5 @role = "weight" @off = "pqoff" pqh : array<float16> //! ... the same bytes as halves (a Q8_0 block's scale)
    @ssbo @binding = 6 @role = "weight" ps : array<uint8>                   //! ... its scale rows (a K-quant codebook)
    @ssbo @binding = 7 @role = "weight" psh : array<float16>
    @ssbo @binding = 8 @role = "weight" @off = "nqoff" nq : array<uint8>    //! the successor codebook's
    @ssbo @binding = 9 @role = "weight" @off = "nqoff" nqh : array<float16>
    @ssbo @binding = 10 @role = "weight" ns : array<uint8>
    @ssbo @binding = 11 @role = "weight" nsh : array<float16>
    @ssbo @binding = 12 @role = "write" ids : array<uint>     //! [nd + 1] the anchor, then the picks
    @uniform @binding = 13 ka : DflashWalkArgs
    @ssbo @binding = 14 @role = "readwrite" @template_gate = SOFT qo : array<float>   //! [nd][k] each position's q, an empty slot at 0
    @ssbo @binding = 15 @role = "readwrite" @template_gate = SOFT rng : array<int>   //! the stream's LCG state; a draw steps the first
    @workgroup arow : float[1024]
    @workgroup prod : float[4096]
    @workgroup scs : float[32]
    @workgroup pick : uint[1]

    //! position `r`'s q over its scored slots - their softmax at `temp` off the first best `best`, or a point mass on it where
    //! every slot is empty - and its draw: one LCG step, the cdf walked to the slot it crosses (the last live slot past the end)
    def soft_pick(r, best : uint; var pk : uint&) {
        pk = best
        static_if (SOFT) {
            let qb = r * ka.k
            var j = 0u
            while (j < ka.k) {
                qo[qb + j] = 0.0
                j++
            }
            if (cid[qb + best] == 0xFFFFFFFFu) {
                qo[qb + best] = 1.0
            } else {
                let inv = 1.0 / ka.temp
                var sum = 0.0
                j = 0u
                while (j < ka.k) {
                    if (cid[qb + j] != 0xFFFFFFFFu) {
                        let e = exp(max((scs[j] - scs[best]) * inv, -80.0))
                        qo[qb + j] = e
                        sum += e
                    }
                    j++
                }
                let inv_sum = 1.0 / sum
                j = 0u
                while (j < ka.k) {
                    qo[qb + j] = qo[qb + j] * inv_sum
                    j++
                }
                var x = rng[0]
                x = int(uint(x) * 214013u + 2531011u)
                rng[0] = x
                let u = float((x >> 16) & 32767) * (1.0 / 32767.0)
                var cdf = 0.0
                var hit = false
                j = 0u
                while (j < ka.k && !hit) {
                    let qj = qo[qb + j]
                    if (qj > 0.0) {
                        pk = j
                        cdf += qj
                        hit = u < cdf
                    }
                    j++
                }
            }
        }
    }

    def walk_body {
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        if (lid == 0u) {
            pick[0] = tok[ka.tbase]
            ids[ka.rbase + ka.rbase / ka.nd] = tok[ka.tbase]
        }
        barrier()
        var r = 0u
        while (r < ka.nd) {
            let pred = pick[0]
            var i = lid
            while (i < ka.rank) {
                var a = 0.0
                dflash_cb_into(pq, pqh, ps, psh, ka.pfmt, ka.rank, ka.pq0, pred, i, a)
                arow[i] = a
                i += nt
            }
            barrier()
            var x = lid
            while (x < ka.k * ka.rank) {
                let j = x / ka.rank
                let e = x % ka.rank
                let cand = cid[(ka.rbase + r) * ka.k + j]
                if (cand != 0xFFFFFFFFu) {
                    var b = 0.0
                    dflash_cb_into(nq, nqh, ns, nsh, ka.nfmt, ka.rank, ka.nq0, cand, e, b)
                    let ah = arow[e] * hsel[(ka.rbase + r) * ka.rank + e]
                    prod[x] = ah * b
                }
                x += nt
            }
            barrier()
            if (lid < ka.k) {
                var s = -3.4e38
                if (cid[(ka.rbase + r) * ka.k + lid] != 0xFFFFFFFFu) {
                    s = cval[(ka.rbase + r) * ka.k + lid]
                    var e = 0u
                    while (e < ka.rank) {
                        s = s + prod[lid * ka.rank + e]
                        e++
                    }
                }
                scs[lid] = s
            }
            barrier()
            if (lid == 0u) {
                var best = 0u
                var bests = -3.0e38
                var j = 0u
                while (j < ka.k) {
                    if (cid[(ka.rbase + r) * ka.k + j] != 0xFFFFFFFFu && scs[j] > bests) {
                        bests = scs[j]
                        best = j
                    }
                    j++
                }
                var pk = best
                static_if (SOFT) {
                    soft_pick(r, best, pk)
                }
                let w = cid[(ka.rbase + r) * ka.k + pk]
                pick[0] = w
                ids[ka.rbase + ka.rbase / ka.nd + r + 1u] = w
            }
            barrier()
            r++
        }
    }
}

//! the greedy walk: each position's first best slot
[metal_dispatch(name = "enc_dflash_walk", pso = "g_df_pso_walk", tgmem = "metal_dflash_walk_msl_tgmem", tg = 256, grid = "1")]
class public MetalDflashWalk : MetalDflashWalkT {
    [metal_kernel(name = "metal_dflash_walk_msl")]
    def metal_dflash_walk {
        walk_body()
    }
}

//! the sampled walk: each position's draw from its candidates' softmax at `temp`, its q landed in `qo`
[metal_dispatch(name = "enc_dflash_walk_soft", pso = "g_df_pso_walk_soft", tgmem = "metal_dflash_walk_soft_msl_tgmem", tg = 256, grid = "1")]
class public MetalDflashWalkSoft : MetalDflashWalkT {
    override SOFT = true

    [metal_kernel(name = "metal_dflash_walk_soft_msl", fastmath = false)]
    def metal_dflash_walk_soft {
        walk_body()
    }
}

//! the accept's arguments: the verify's `nr` rows, `vocab` logits a row
struct public DflashAcceptArgs {
    nr : uint
    vocab : uint
}

//! The greedy accept over the verify's rows: every lane reads n = the leading run where row r's pick equals the id row r + 1
//! verified (r < nr - 1) and copies its element of row n's logits into `row`; the first lane lands n, row n's pick and every
//! row's pick into `words`
[metal_dispatch(name = "enc_dflash_accept", pso = "g_df_pso_accept", tg = 256, grid = "vocab/256", params = "vocab : int64")]
class MetalDflashAccept {
    @ssbo @binding = 0 @role = "read" lg : array<float>      //! [nr][vocab] the verify's logits
    @ssbo @binding = 1 @role = "read" pick : array<uint>     //! [nr] each row's argmax
    @ssbo @binding = 2 @role = "read" ids : array<uint>      //! [nr] the rows' ids: the anchor, then the drafts
    @ssbo @binding = 3 @role = "write" row : array<float>    //! [vocab] row n's logits
    @ssbo @binding = 4 @role = "write" words : array<uint>   //! [2 + nr] n, row n's pick, every row's pick
    @uniform @binding = 5 ka : DflashAcceptArgs

    [metal_kernel(name = "metal_dflash_accept_msl")]
    def metal_dflash_accept {
        let gid = gl_GlobalInvocationID.x
        var n = 0u
        while (n + 1u < ka.nr && pick[n] == ids[n + 1u]) {
            n++
        }
        if (gid < ka.vocab) {
            row[gid] = lg[n * ka.vocab + gid]
        }
        if (gid == 0u) {
            words[0] = n
            words[1] = pick[n]
            var r = 0u
            while (r < ka.nr) {
                words[2u + r] = pick[r]
                r++
            }
        }
    }
}

var private g_df_pso_conv : MetalComputePipeline?
var private g_df_pso_qkn : MetalComputePipeline?
var private g_df_pso_attn : MetalComputePipeline?
var private g_df_pso_topk_slices : MetalComputePipeline?
var private g_df_pso_topk_merge : MetalComputePipeline?
var private g_df_pso_walk : MetalComputePipeline?
var private g_df_pso_walk_soft : MetalComputePipeline?
var private g_df_pso_accept : MetalComputePipeline?
var private g_df_tried = false
var private g_df_select = g_env_metal.dflash_select   //! the device selector serves where the drafter's shapes let it
var private g_df_soft = g_env_metal.dflash_soft_pick   //! ... and draws a sampled round's drafts
var private g_df_accept = g_env_metal.dflash_accept   //! a held pass's verify lands the accept on the device
var private g_df_ahead = g_env_metal.verify_ahead     //! 1: a sampled round whose verify reads no drafted id on the host holds too; 2: a late verify's model too

//! a session's context ring on the device: a layer's K and V rows a slot, [L][window][kvd] f32 each
struct private DflashRing {
    bk : MetalBuffer?
    bv : MetalBuffer?
}

let private DF_JOINT_PASS_STREAMS = 4l   //! the streams one joint pass carries at most

//! A session's staged commit: its captured rows of one ring run, encoded at the head of the next pass
struct private DflashPend {
    uid : uint64
    pos0 : int64             //! the run's first position
    rows : int64             //! rows staged in `bcaps` (0 = the slot is free)
    bcaps : MetalBuffer?     //! [cap][ncap][dim] the captured layer outputs
    cap : int64
}

let private DF_PEND_SLOTS = 4   //! the sessions whose commits stay staged together; one more flushes them all

//! The attached drafter on the device: its planes uploaded once, the pass's row buffers at the block's rows, the commit's at the
//! widest commit so far, the width uniforms, and a ring a session
struct private DflashGpu {
    plain : bool   //! a DFlash file's pass: no convs, no selector - each draft its row's best id, into `bcid`
    ready : bool
    bblob : MetalBuffer?    //! the q8 planes (the region cache owns it); null where the drafter carries none
    bfblob : MetalBuffer?   //! the f32 planes (the same)
    kq_q : array<MetalBuffer?>   //! [KQ_FMT_COUNT] a K-quant format's quant plane, verbatim
    kq_s : array<MetalBuffer?>   //! ... its scale plane in the device form
    kq_nsb : array<int64>        //! ... its superblocks
    binvf : MetalBuffer?    //! [hs/2] the rope's inverse frequencies
    btok : MetalBuffer?     //! [nblock] the pass's token ids
    bdx : MetalBuffer?      //! [nblock][dim] a joint pass's draft rows, a stream after a stream
    bx : MetalBuffer?       //! [nblock][dim] the residual rows
    bhn : MetalBuffer?      //! [nblock][dim] the normed rows
    bhc : MetalBuffer?      //! [nblock][dim] the normed rows after the input convolution
    bdyn : MetalBuffer?     //! [nblock][projw] a sublayer's dynamic coefficients
    bq : MetalBuffer?       //! [nblock][qd]
    bk : MetalBuffer?       //! [nblock][kvd]
    bv : MetalBuffer?
    batt : MetalBuffer?     //! [nblock][qd]
    bo : MetalBuffer?       //! [nblock][dim] a sublayer's output
    boc : MetalBuffer?      //! ... after the output convolution
    bg : MetalBuffer?       //! [nblock][hidden] the gate, then silu(gate) * up
    bu : MetalBuffer?       //! [nblock][hidden] the up (the separate gate and up sites' arm; the fused ladder needs none)
    bhsel : MetalBuffer?    //! [nblock - 1][rank]
    blog : MetalBuffer?     //! [nblock - 1][vocab]
    bcid : MetalBuffer?     //! [nblock - 1][topk] the device selector's candidates, best first
    bcval : MetalBuffer?    //! ... their logits
    bpid : MetalBuffer?     //! [nblock - 1][sel_nsl][topk] the top-k's slice lists
    bpval : MetalBuffer?
    sel_nsl : int64         //! the slices a draft row's logits split into
    bids : MetalBuffer?     //! [nblock] the walk's rows: the anchor, then the picks
    bsq : MetalBuffer?      //! [nblock - 1][topk] the sampled walk's q a position
    brng : MetalBuffer?     //! [4] the stream's LCG state the sampled walk steps
    sel_pq : MetalBuffer?   //! the predecessor codebook's quant bytes (the blob or its format's plane)
    sel_pqoff : uint64
    sel_ps : MetalBuffer?   //! ... its scale rows in the CPU form, the codebook's own (a K-quant codebook)
    sel_nq : MetalBuffer?   //! the successor codebook's
    sel_nqoff : uint64
    sel_ns : MetalBuffer?
    sel_args : DflashWalkArgs
    sel_ok : bool           //! the device selector serves this drafter's shapes
    bacc_words : MetalBuffer?   //! [nblock + 2] the device accept's landing: the count, the next anchor, every row's pick
    bacc_row : MetalBuffer?     //! [vocab] ... the committed row's logits
    pends : DflashPend[DF_PEND_SLOTS]   //! the staged commits, a session a slot
    bfc : MetalBuffer?      //! [caps_rows][dim] fc's rows of the commit being encoded
    bcr : MetalBuffer?      //! ... normed
    caps_rows : int64
    u_d : MetalBuffer?
    u_projw : MetalBuffer?
    u_qd : MetalBuffer?
    u_kvd : MetalBuffer?
    u_hidden : MetalBuffer?
    u_rank : MetalBuffer?
    u_fcn : MetalBuffer?    //! fc's input width, ncap * dim
    u_vocab : MetalBuffer?
    u_eps : MetalBuffer?
    u_act : MetalBuffer?    //! 0: the fused gate's silu
    u_escale : MetalBuffer?
    u_tot : array<MetalBuffer?>   //! rows * dim a row count, index rows
    u_toth : array<MetalBuffer?>  //! rows * hidden a row count, index rows
    rings : table<uint64; DflashRing>
    vocab : int64
    us_gpu : int64
    us_encode : int64
    passes : int64
    commits : int64
    selected : int64        //! the passes the device selector ended
    held : int64            //! ... committed unwaited, their picks the verify's rows
    soft : int64            //! ... that drew a sampled round's drafts
    accepts : int64         //! the verifies that landed the accept on the device
    ahead : int64           //! the held passes only `g_df_ahead` held: a sampled round's, or a late verify's
}

var private g_df : DflashGpu
var private g_df_held : MetalCommandBuffer?   //! a pass committed unwaited, its picks the verify's rows
var private g_df_held_submit_us = 0l          //! ... its encode and commit on the host
var private g_df_held_soft = false            //! ... drawn: its candidates, q and stepped rng land with the picks
var private g_df_sites : int64[KQ_FMT_COUNT]
var private g_df_rows8_sites : int64[KQ_FMT_COUNT]

//! the pass's device-busy and host-encode microseconds, and the passes and commits they cover, since the attach
def public metal_dflash_stats : tuple<gpu_us : int64; encode_us : int64; passes : int64; commits : int64>
    => (gpu_us = g_df.us_gpu, encode_us = g_df.us_encode, passes = g_df.passes, commits = g_df.commits)

//! the passes the device selector ended, and of them those committed unwaited with the verify reading their picks, since the attach
def public metal_dflash_selected : tuple<selected : int64; held : int64> => (selected = g_df.selected, held = g_df.held)

//! the passes whose device selector drew a sampled round's drafts, since the attach
def public metal_dflash_soft_picked : int64 => g_df.soft

//! the verifies that landed their accept on the device - the count, the picks and the committed row alone - since the attach
def public metal_dflash_accepted : int64 => g_df.accepts

//! the rounds whose pass only the verify-ahead knob held - a sampled round's, or a late verify's - since the attach
def public metal_verify_ahead_rounds : int64 => g_df.ahead

//! the verify-ahead level's announce clause
def private df_ahead_words(level : int) : string {
    return "only a greedy round of a model with no side input holds its pass" if (level <= 0)
    return "a side-input model's verify is encoded after its pass" if (level == 1)
    return "a sampled round and a side-input model's round hold their pass too"
}

//! the verify-ahead level: 0 holds the greedy round of a model whose verify takes device rows alone, 1 (default) a sampled round
//! whose verify reads no drafted id on the host too, 2 also a hyper-connection or n-gram model's round, its verify encoded ahead
//! and committed once the picks land (`metal_verify_takes_device_rows_late`)
[arch(at="../ARCHITECTURE_GPU_MTP_ROUND.md#verify-ahead")]
def public set_metal_verify_ahead(level : int) {
    if (level != g_df_ahead) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: set_metal_verify_ahead({level}) - {df_ahead_words(level)}\n")
    }
    g_df_ahead = level
}

def public get_metal_verify_ahead : int => g_df_ahead

//! the pass's and the commits' weight sites encoded over a plane of format `fmt`, since the attach
def public metal_dflash_sites(fmt : KqFmt) : int64 => g_df_sites[int(fmt)]

//! the pass's and the commits' weight sites the rows8 matrix-unit form served, since the attach
def public metal_dflash_rows8_sites : int64 {
    var total = 0l
    for (s in g_df_rows8_sites) {
        total += s
    }
    return total
}

//! the pass's and the commits' weight sites over a plane of format `fmt` the rows8 matrix-unit form served, since the attach
def public metal_dflash_rows8_sites_of(fmt : KqFmt) : int64 => g_df_rows8_sites[int(fmt)]

//! the drafter's environment overrides off their defaults, one line each as the device drafter first comes up
[cold_path]
def private df_env_announce {
    if (!g_env_metal.dflash_select) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: DASLLAMA_METAL_DFLASH_SELECT=0 - the drafter's selector walks the landed logits on the host after a waited pass\n")
    }
    if (!g_env_metal.dflash_soft_pick) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: DASLLAMA_METAL_DFLASH_SOFT_PICK=0 - a sampled round's drafts are drawn on the host off the landed logits\n")
    }
    if (!g_env_metal.dflash_accept) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: DASLLAMA_METAL_DFLASH_ACCEPT=0 - a held pass's greedy accept lands every row's logits for the host\n")
    }
    if (g_env_metal.verify_ahead != 2) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: DASLLAMA_METAL_VERIFY_AHEAD={g_env_metal.verify_ahead} - {df_ahead_words(g_env_metal.verify_ahead)}\n")
    }
}

[cold_path]
def private df_init : bool {
    return false if (!metal_common_init() || !metal_decode_init())
    if (!g_df_tried) {
        g_df_tried = true
        df_env_announce()
        var ok = true
        g_df_pso_conv = compile_stamp(metal_dflash_conv_msl, ok)
        g_df_pso_qkn = compile_stamp(metal_dflash_qkn_msl, ok)
        g_df_pso_attn = compile_stamp(metal_dflash_attn_msl, ok)
        var sel_ok = true
        g_df_pso_topk_slices = compile_stamp(metal_dflash_topk_slices_msl, sel_ok)
        g_df_pso_topk_merge = compile_stamp(metal_dflash_topk_merge_msl, sel_ok)
        g_df_pso_walk = compile_stamp(metal_dflash_walk_msl, sel_ok)
        var soft_ok = true
        g_df_pso_walk_soft = compile_stamp(metal_dflash_walk_soft_msl, soft_ok)
        var acc_ok = true
        g_df_pso_accept = compile_stamp(metal_dflash_accept_msl, acc_ok)
    }
    return g_df_pso_conv != null && g_df_pso_qkn != null && g_df_pso_attn != null
}

//! the device selector on (default) or the host's walk over the landed logits on the device pass (the A/B rail)
def public set_metal_dflash_select(on : bool) {
    if (on != g_df_select) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: set_metal_dflash_select({on}) - the drafter's selector {on ? "runs on the device" : "walks the landed logits on the host after a waited pass"}\n")
    }
    g_df_select = on
}

//! the device selector draws a sampled round's drafts (default) or stands down for the host's walk over the landed logits (the
//! A/B rail)
def public set_metal_dflash_soft_pick(on : bool) {
    if (on != g_df_soft) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: set_metal_dflash_soft_pick({on}) - a sampled round's drafts are drawn {on ? "on the device selector" : "on the host off the landed logits"}\n")
    }
    g_df_soft = on
}

//! the device selector serves the attached drafter's round, a sampled one's drafts (`soft`) included: the knobs on, its pipelines
//! built, its shapes the kernels'
[no_env, arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-sampled-drafts")]
def private df_selects(soft : bool) : bool {
    return false if (!g_df_select || !g_df.ready || !g_df.sel_ok)
    return !soft || (g_df_soft && g_df_pso_walk_soft != null && g_df.bsq != null && g_df.brng != null)
}

//! a held pass's verify lands the accept on the device (default) or every row's logits for the host's walk (the A/B rail)
def public set_metal_dflash_accept(on : bool) {
    if (on != g_df_accept) {
        to_log(LOG_WARNING, "dasLLAMA metal dflash: set_metal_dflash_accept({on}) - a held pass's greedy accept {on ? "runs on the device" : "lands every row's logits for the host"}\n")
    }
    g_df_accept = on
}

//! a held pass arms its verify's device accept: the knob on, its pipeline built, its landing made
def private df_accepts : bool => g_df_accept && g_df_pso_accept != null && g_df.bacc_words != null && g_df.bacc_row != null

//! the device accept onto the verify's encoder, after its rows' picks: `nr` rows of `blog` against the ids in `ids`
def private df_encode_accept(enc : MetalComputeEncoder?; blog, bvarg, ids : MetalBuffer?; nr : int64) {
    var g & = g_df
    var ka = DflashAcceptArgs(nr = uint(nr), vocab = uint(g.vocab))
    enc_dflash_accept(enc, blog, bvarg, ids, g.bacc_row, g.bacc_words, ka, g.vocab)
    g.accepts++
}

def private df_ring_release(var r : DflashRing) {
    release_handles(r.bk, r.bv)
}

[cold_path]
def private df_release(var g : DflashGpu) {
    if (g_df_held != null) {
        metal_wait_until_completed(g_df_held)
        metal_release(g_df_held)
        g_df_held = null
    }
    g_df_held_soft = false
    metal_verify_accept_unregister()
    metal_verify_early_land_unregister()
    release_handles(g.bacc_words, g.bacc_row)
    release_handles(g.binvf, g.btok, g.bx, g.bhn, g.bhc, g.bdyn)
    release_handles(g.bq, g.bk, g.bv, g.batt, g.bo, g.boc)
    release_handles(g.bg, g.bu, g.bhsel, g.blog, g.bfc, g.bcr, g.bdx)
    for (pd in g.pends) {
        release_handle(pd.bcaps)
        pd.rows = 0l
        pd.cap = 0l
    }
    release_handles(g.bcid, g.bcval, g.bpid, g.bpval, g.bids, g.sel_ps, g.sel_ns)
    release_handles(g.bsq, g.brng)
    g.sel_pq = null
    g.sel_nq = null
    g.sel_ok = false
    release_handles(g.u_d, g.u_projw, g.u_qd, g.u_kvd, g.u_hidden, g.u_rank)
    release_handles(g.u_fcn, g.u_vocab, g.u_eps, g.u_act, g.u_escale)
    for (u in g.u_tot) {
        metal_release(u)
    }
    g.u_tot |> clear()
    for (u in g.u_toth) {
        metal_release(u)
    }
    g.u_toth |> clear()
    for (r in values(g.rings)) {
        df_ring_release(r)
    }
    g.rings |> clear()
    for (b in g.kq_q) {
        release_handle(b)
    }
    for (b in g.kq_s) {
        release_handle(b)
    }
    g.kq_q |> clear()
    g.kq_s |> clear()
    g.kq_nsb |> clear()
    g.bblob = null
    g.bfblob = null
    g.caps_rows = 0l
    g.ready = false
}

//! the commit's scratch rows at `rows` rows at least (nothing staged reads them)
def private df_caps_rows(dr : DflashDrafter; var g : DflashGpu; rows : int64) {
    return if (g.caps_rows >= rows)
    release_handles(g.bfc, g.bcr)
    let d = dr.dim
    g.caps_rows = max(rows, 2l * g.caps_rows)
    g.bfc = metal_new_buffer(g_dev, uint64(g.caps_rows * d * 4l))
    g.bcr = metal_new_buffer(g_dev, uint64(g.caps_rows * d * 4l))
}

//! the Metal arm's attach: the planes up, the pass's buffers made; false where the trunk is not the metal-blob flavor the decode
//! serves, its token table is not gathered on the device, or a pipeline failed
[cold_path]
def private df_ready(t : Model; dr : DflashDrafter) : bool {
    return false if (!t.metal_blob || !emb_dev_gather_ok(t) || !df_init())
    df_release(g_df)
    var g & = g_df
    let d = dr.dim
    let n = dr.nblock
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let projw = dr.v2 ? 2l * dr.conv_k * (d / dr.conv_g) : 0l
    let ncap = length(dr.target_layers)
    let f4 = 4ul
    g.plain = !dr.v2
    g.vocab = t.config.vocab_size
    if (!empty(dr.blob)) {
        g.bblob = upload_region(unsafe(addr<void?>(dr.blob[0])), uint64(long_length(dr.blob)))
    }
    g.bfblob = upload_region(unsafe(addr<void?>(dr.fblob[0])), uint64(long_length(dr.fblob)) * f4)
    let kq_ok = df_upload_kq(dr, g)
    g.binvf = metal_new_buffer(g_dev, uint64(dr.hs / 2l) * f4)
    unsafe {
        memcpy(metal_buffer_contents(g.binvf), addr<void?>(dr.inv_freq[0]), uint64(dr.hs / 2l) * f4)
    }
    g.btok = metal_new_buffer(g_dev, uint64(max(n * DF_JOINT_PASS_STREAMS, DF_PASS_ROWS)) * f4)
    let jr = max(n, DF_JOINT_ROWS)   //! the rows a joint pass's tail holds: every stream's draft rows
    g.bdx = metal_new_buffer(g_dev, uint64(jr * d) * f4)
    let pr = max(n, DF_PASS_ROWS)   //! the rows a joint pass's layers hold: every stream's block
    g.bx = metal_new_buffer(g_dev, uint64(pr * d) * f4)
    g.bhn = metal_new_buffer(g_dev, uint64(max(n, DF_JOINT_ROWS) * d) * f4)
    g.bhc = metal_new_buffer(g_dev, uint64(pr * d) * f4)
    g.bdyn = metal_new_buffer(g_dev, uint64(max(pr * projw, 1l)) * f4)
    g.bq = metal_new_buffer(g_dev, uint64(pr * qd) * f4)
    g.bk = metal_new_buffer(g_dev, uint64(pr * kvd) * f4)
    g.bv = metal_new_buffer(g_dev, uint64(pr * kvd) * f4)
    g.batt = metal_new_buffer(g_dev, uint64(pr * qd) * f4)
    g.bo = metal_new_buffer(g_dev, uint64(pr * d) * f4)
    g.boc = metal_new_buffer(g_dev, uint64(pr * d) * f4)
    g.bg = metal_new_buffer(g_dev, uint64(pr * dr.hidden) * f4)
    g.bu = metal_new_buffer(g_dev, uint64(pr * dr.hidden) * f4)
    g.bhsel = metal_new_buffer(g_dev, uint64(max(jr * dr.rank, 1l)) * f4)
    g.blog = metal_new_buffer(g_dev, uint64(jr * g.vocab) * f4)
    g.sel_ok = df_ready_select(dr, g)
    g.u_d = ubuf_u32(uint(d))
    g.u_projw = ubuf_u32(uint(projw))
    g.u_qd = ubuf_u32(uint(qd))
    g.u_kvd = ubuf_u32(uint(kvd))
    g.u_hidden = ubuf_u32(uint(dr.hidden))
    g.u_rank = ubuf_u32(uint(dr.rank))
    g.u_fcn = ubuf_u32(uint(ncap) * uint(d))
    g.u_vocab = ubuf_u32(uint(g.vocab))
    g.u_eps = ubuf_f32(dr.eps)
    g.u_act = ubuf_u32(0u)
    g.u_escale = ubuf_f32(t.config.embed_scale)
    g.u_tot |> reserve(pr + 1l)
    g.u_toth |> reserve(pr + 1l)
    for (r in range64(pr + 1l)) {
        g.u_tot |> push(ubuf_u32(uint(r * d)))
        g.u_toth |> push(ubuf_u32(uint(r * dr.hidden)))
    }
    df_caps_rows(dr, g, n)
    g.us_gpu = 0l
    g.us_encode = 0l
    g.passes = 0l
    g.commits = 0l
    g.selected = 0l
    g.held = 0l
    g.soft = 0l
    g.accepts = 0l
    for (s in g_df_sites) {
        s = 0l
    }
    for (s in g_df_rows8_sites) {
        s = 0l
    }
    g.ready = kq_ok && (empty(dr.blob) || g.bblob != null) && g.bfblob != null && g.blog != null
    df_ready_accept(g, n)
    return g.ready
}

//! the device accept's landing for a ready drafter of `nblock` rows, made and registered with the verify
[cold_path]
def private df_ready_accept(var g : DflashGpu; nblock : int64) {
    return if (!g.ready)
    g.bacc_words = metal_new_buffer(g_dev, uint64((nblock + 2l) * 4l))
    g.bacc_row = metal_new_buffer(g_dev, uint64(g.vocab * 4l))
    if (g.bacc_words != null && g.bacc_row != null) {
        metal_verify_accept_register(MetalVerifyAccept(encode = @@df_encode_accept, words = g.bacc_words, row = g.bacc_row))
    }
    metal_verify_early_land_register(@@df_land_early)
}

//! the walk's code for a codebook of format `f` (the formats `MetalDflashWalk` dequantizes), -1 = none
def private sel_fmt_code(f : KqFmt) : int {
    return 0 if (f == KqFmt.q8)
    return 4 if (f == KqFmt.k4)
    return 5 if (f == KqFmt.k5)
    return 6 if (f == KqFmt.k6)
    return -1
}

//! a selector codebook's binds: its quant bytes (the blob or its format's plane, bound four-aligned, `q0` the bytes past the
//! bind) and, for a K-quant codebook, its own scale rows uploaded in the CPU form the host's dequant reads
def private sel_codebook(dr : DflashDrafter; var g : DflashGpu; w : DflashW) : tuple<q : MetalBuffer?; qoff : uint64; q0 : uint; s : MetalBuffer?; ok : bool> {
    if (w.fmt == KqFmt.q8) {
        return (q = g.bblob, qoff = uint64(w.off & ~3l), q0 = uint(w.off & 3l), s = null, ok = g.bblob != null && dr.rank % DFLASH_Q8_BLOCK == 0l)
    }
    let i = int(w.fmt)
    let e = kq_elems(w.fmt)
    if (sel_fmt_code(w.fmt) < 0 || dr.rank % e != 0l || w.off % e != 0l || g.kq_q[i] == null) {
        return (q = null, qoff = 0ul, q0 = 0u, s = null, ok = false)
    }
    let sb0 = w.off / e
    let sbytes = (dr.vocab * dr.rank / e) * kq_ssb(w.fmt)
    var bs = metal_buffer_from(g_dev, uint64(sbytes), unsafe(addr<void?>(dr.kq[i].s[sb0 * kq_ssb(w.fmt)])))
    return (q = g.kq_q[i], qoff = uint64(sb0 * kq_qsb(w.fmt)), q0 = 0u, s = bs, ok = bs != null)
}

//! the device selector's buffers and the walk's codebook binds; false where a shape is past the kernels' or a pipeline failed
[cold_path]
def private df_ready_select(dr : DflashDrafter; var g : DflashGpu) : bool {
    if (!dr.v2) {   //! a DFlash file: the rows' argmax is the whole select
        g.bcid = metal_new_buffer(g_dev, uint64(max(dr.nblock, DF_JOINT_ROWS) * 4l))
        return g.bcid != null
    }
    return false if (g_df_pso_topk_slices == null || g_df_pso_topk_merge == null || g_df_pso_walk == null
        || dr.topk > int64(DFLASH_SEL_K) || dr.rank > int64(DFLASH_SEL_RANK) || dr.topk * dr.rank > int64(DFLASH_SEL_PRODS))
    let nd = max(dr.nblock, DF_JOINT_ROWS)   //! a joint pass's draft rows over every stream
    g.sel_nsl = (g.vocab + int64(DFLASH_SEL_SPAN) - 1l) / int64(DFLASH_SEL_SPAN)
    g.bcid = metal_new_buffer(g_dev, uint64(nd * dr.topk * 4l))
    g.bcval = metal_new_buffer(g_dev, uint64(nd * dr.topk * 4l))
    g.bpid = metal_new_buffer(g_dev, uint64(nd * g.sel_nsl * dr.topk * 4l))
    g.bpval = metal_new_buffer(g_dev, uint64(nd * g.sel_nsl * dr.topk * 4l))
    g.bids = metal_new_buffer(g_dev, uint64(2l * nd * 4l))
    g.bsq = metal_new_buffer(g_dev, uint64(nd * dr.topk * 4l))
    g.brng = metal_new_buffer(g_dev, 16ul)
    var p = sel_codebook(dr, g, dr.sel_prev)
    var nx = sel_codebook(dr, g, dr.sel_next)
    g.sel_pq = p.q
    g.sel_pqoff = p.qoff
    g.sel_ps = p.s
    g.sel_nq = nx.q
    g.sel_nqoff = nx.qoff
    g.sel_ns = nx.s
    g.sel_args = DflashWalkArgs(rank = uint(dr.rank), k = uint(dr.topk), pfmt = uint(sel_fmt_code(dr.sel_prev.fmt)),
        nfmt = uint(sel_fmt_code(dr.sel_next.fmt)), pq0 = p.q0, nq0 = nx.q0)
    return p.ok && nx.ok && g.bcid != null && g.bcval != null && g.bpid != null && g.bpval != null && g.bids != null
}

//! the drafter's K-quant planes up as buffers of the pass's own: a format's quants verbatim, its scale plane in the device form
//! the trunk's blob carries (`metal_kq_scale_form`); false where a buffer failed
[cold_path]
def private df_upload_kq(dr : DflashDrafter; var g : DflashGpu) : bool {
    g.kq_q |> resize(KQ_FMT_COUNT)
    g.kq_s |> resize(KQ_FMT_COUNT)
    g.kq_nsb |> resize(KQ_FMT_COUNT)
    var ok = true
    for (i in range(KQ_FMT_COUNT)) {
        continue if (empty(dr.kq[i].q))
        let f = kq_fmt_of_slot(i)
        var sdev := dr.kq[i].s
        metal_kq_scale_form(f, sdev)
        g.kq_q[i] = metal_buffer_from(g_dev, uint64(long_length(dr.kq[i].q)), unsafe(addr<void?>(dr.kq[i].q[0])))
        g.kq_s[i] = metal_buffer_from(g_dev, uint64(long_length(sdev)), unsafe(addr<void?>(sdev[0])))
        g.kq_nsb[i] = long_length(dr.kq[i].q) / kq_qsb(f)
        ok = ok && g.kq_q[i] != null && g.kq_s[i] != null
        delete sdev
    }
    return ok
}

[cold_path]
def private df_detach {
    df_release(g_df)
    if (g_df_ws_ok) {
        delete g_df_ws
        g_df_ws_ok = false
    }
}

var private g_df_ws : BatchWorkspace   //! the joint round's verify workspace, made at the first round of an attach
var private g_df_ws_ok = false
var private @scratch g_df_gw : MtpRoundGather   //! the tick's warm streams, a verify group each
var private @scratch g_df_gw_toks : array<int64>
var private g_df_joint_rounds = 0l
var private g_df_joint_max = 0l

//! the joint rounds since load, and the most streams one verify carried
def public metal_dflash_joint_rounds() : tuple<rounds : int64; streams_max : int64> => (rounds = g_df_joint_rounds, streams_max = g_df_joint_max)

//! the joint round's verify serves `t`: a served image on the device (a hybrid's groups ride the verify seat, each with its
//! own shadow recurrent region)
def private df_joint_serves(t : Model) : bool
    => g_dev != null && t.metal_blob

//! a stream the joint round cannot carry at `pos`: no mirror, a round past the context, or a mirror short of the position or
//! of the round's rows
def private df_round_cold(t : Model; s : Session; pos, k : int64) : bool =>
    pos + k + 1l > t.config.seq_len || mtp_mirror_short(s, pos, k)

def private df_plain_step(t : Model; var s : Session; tok, pos : int64) {
    forward(t, s, tok, pos)
    s.n_past = pos + 1l
}

let private DF_ROW_BUDGET = 8l      //! a tick's verify rows over every stream on the batch rail: the rows the eight-row forms carry
let private DF_PASS_ROWS = METAL_BATCH_TILED_ROWS    //! the rows a joint pass's layers carry: every stream's block as row tiles of one weight pass
let private DF_JOINT_ROWS = METAL_BATCH_TILED_ROWS   //! the draft rows over every stream one joint pass's classifier and selector carry
let private DF_ROWS_DRAFTS = 3l            //! the drafts a round verifies on the row forms, where no matrix unit shares the weight pass over its rows (an M1 Max: gemma-4 E2B's rounds pay at three, lose at seven)
let private DF_JOINT_STREAMS = 2l          //! the streams a drafted tick carries on a trunk with no recurrent layers: a wider tick's plain batched step outruns its rounds (gemma-4 E2B)
let private DF_JOINT_STREAMS_HYBRID = 4l   //! ... on a hybrid, whose verify is one weight pass at any row count (Qwen3.8-27B)

var private g_df_split : DflashTickSplit

def public metal_dflash_tick_split : DflashTickSplit => g_df_split

//! a hybrid's gathered streams on the verify seat's joint step: every stream's rows a group of one step, each stream's accept,
//! its group's recurrent replay on a reject, its commit; where the seat declines the groups each stream verifies its own rows
//! on the seat in turn
def private df_rounds_seat(t : Model; ng : int64; var acc : array<int64>) : int64 {
    let c = t.config
    var even = true
    for (nr in g_df_gw.nr) {
        even = even && nr == g_df_gw.nr[0]
    }
    let tv = ref_time_ticks()
    let joint = even && metal_mtp_verify_groups(t, g_df_gw.sessions, g_df_gw.pos, g_df_gw.nr[0])
    g_df_split.verify_us += int64(get_time_usec(tv))
    for (gi in range64(ng)) {
        var s & = unsafe(*g_df_gw.sessions[gi])
        let pos = g_df_gw.pos[gi]
        let kd = g_df_gw.nr[gi] - 1l
        if (!joint && !mtp_block_verify_alone(t, s, kd + 1l, pos)) {
            df_plain_step(t, s, s.mtp_vbatch[0], pos)
            continue
        }
        let a = mtp_accept_walk(s, kd, c.vocab_size) $(vr) => s.mtp_picks[vr]
        mtp_round_take(s, a, kd, mtp_h_dim(c))
        if (a < kd) {
            let tr = ref_time_ticks()
            let rolled = joint ? metal_mtp_rollback_group(t, s, pos, a) : mtp_block_rollback_alone(t, s, pos, a)
            g_df_split.rollback_us += int64(get_time_usec(tr))
            verify(rolled, "dasLLAMA metal dflash: a joint round's reject found no parked verify to roll back - engine bug")
        }
        mtp_round_commit(s, pos, kd, a)
        let tc = ref_time_ticks()
        mtp_block_commit(t, s, pos, a + 1l)
        g_df_split.commit_us += int64(get_time_usec(tc))
        acc[g_df_gw.idx[gi]] = a
    }
    if (joint) {
        g_df_joint_rounds++
        g_df_joint_max = max(g_df_joint_max, ng)
    }
    g_df_gw.sessions |> clear()   // borrowed handles
    return joint ? ng : 0l
}

//! The block drafter's joint round over a tick's streams at depth `k`: every warm stream drafts its block from its own anchor,
//! one verify carries each stream's [token, drafts] as a group, then each stream's accept walk, commit and context commit; a
//! stream the round cannot carry takes one plain step. `acc[i]` is stream i's accepted count; returns the streams carried
[hot_path, arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-rounds")]
def private df_rounds(t : Model; var sessions : array<Session?>; toks : array<int64>; k : int64; var acc : array<int64>) : int64 {
    let t0 = ref_time_ticks()
    let joined = df_rounds_run(t, sessions, toks, k, acc)
    g_df_split.round_us += int64(get_time_usec(t0))
    g_df_split.rounds++
    return joined
}

[arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-tick-rounds")]
def private df_rounds_run(t : Model; var sessions : array<Session?>; toks : array<int64>; k : int64; var acc : array<int64>) : int64 {
    let c = t.config
    if (!g_df_ws_ok) {
        df_ws_make(t)
    }
    let n = long_length(sessions)
    let form = metal_rows8_on()
    if (n > df_tick_joins(c, form)) {   //! past the streams a drafted tick pays for, the tick is one plain batched step
        acc |> resize(n)   // nolint:PERF026 — grows to the stream count once, the callers pass reused scratch
        for (a in acc) {
            a = 0l
        }
        eval_batch_(t, g_df_ws, sessions, toks)   // nolint:PERF026 — the workspace's rows grow to the stream count once
        return 0l
    }
    let kb = df_tick_depth(n, k, form)
    let together = n > 1l   //! several streams draft in one joint pass, after the gather
    let td = ref_time_ticks()
    let ng = mtp_round_gather(g_df_gw, sessions, toks, kb, acc) $(var s : Session; i, pos, ki : int64) : int64 {
        if (!df_round_cold(t, s, pos, ki)) {
            mtp_block_prepare(s, c)
            if (together) {
                return 0l if (dflash_ctx_reaches(s, pos))   // nolint:PERF026 — a stream's sequence slot is made at its first commit
            } else {
                return 0l if (mtp_block_draft_landed(t, s, toks[i], pos, ki) == ki)
            }
        }
        df_plain_step(t, s, toks[i], pos)
        return -1l
    }
    return 0l if (ng == 0l)
    if (together) {
        df_tick_drafts(t, ng)
    }
    g_df_split.draft_us += int64(get_time_usec(td))
    return c.recr_mask != 0ul ? df_rounds_seat(t, ng, acc) : df_rounds_rail(t, ng, acc)
}

//! the joint round's verify workspace, made at the first round of an attach
[cold_path]
def private df_ws_make(t : Model) {
    g_df_ws <- create_batch_workspace_(t)   // nolint:PERF030 — fresh: the detach deleted the attach before's
    g_df_ws_ok = true
}

//! the streams a drafted tick carries: off the matrix-unit form (`form`) a verify's rows each cost near a plain step, so a
//! round pays for one stream alone; a hybrid of routed experts on one residual stream joins two, its plain batched step of
//! three rows and up riding the eight-row forms and outrunning its rounds (Qwen3.6-35B-A3B)
def private df_tick_joins(c : Config; form : bool) : int64 {
    let wide = c.recr_mask != 0ul && (c.n_expert == 0l || c.hyper_conn)
    return !form ? 1l : wide ? DF_JOINT_STREAMS_HYBRID : DF_JOINT_STREAMS
}

//! the drafts a stream of a tick of `n` takes at the asked depth `k`: the tick's verify rows within one weight pass, a row
//! tile for every two streams - about four rows a stream, past which a draft lands too seldom for the tile it opens; a pinned
//! depth takes the pass whole
def private df_tick_depth(n, k : int64; form : bool) : int64 {
    let pinned = mtp_depth_pin().pinned
    let pass_rows = form ? METAL_VERIFY_TILED_ROWS : DF_ROW_BUDGET
    let budget = pinned ? pass_rows : min(DF_ROW_BUDGET * ((n + 1l) / 2l), pass_rows)
    return clamp(budget / n - 1l, 1l, min(k, form || pinned ? 7l : DF_ROWS_DRAFTS))
}

//! the gathered streams' drafts in one joint pass at the shortest ask among them - a stream near its budget asks for fewer,
//! and one depth keeps the tick one pass and one verify - else a stream at a time
def private df_tick_drafts(t : Model; ng : int64) {
    var shortest = g_df_gw.nr[0]
    for (nr in g_df_gw.nr) {
        shortest = min(shortest, nr)
    }
    for (nr in g_df_gw.nr) {
        nr = shortest
    }
    return if (shortest >= 2l && dflash_draft_joint(t, g_df_gw.sessions, g_df_gw.pos, shortest - 1l))   // nolint:PERF026 — a stream's sequence slot is made at its first commit
    for (gi in range64(ng)) {   //! a stream whose pass fails verifies its own token again and accepts none
        var s & = unsafe(*g_df_gw.sessions[gi])
        let kd = g_df_gw.nr[gi] - 1l
        if (mtp_block_draft_landed(t, s, s.mtp_vbatch[0], g_df_gw.pos[gi], kd) != kd) {
            for (r in range64(kd)) {
                s.mtp_vbatch[r + 1l] = s.mtp_vbatch[0]
            }
        }
    }
}

//! the gathered streams' rows as one same-slab batch verify, then each stream's accept, commit and context commit; a verify
//! the rail declines leaves every stream one plain step. Returns the streams the verify carried
def private df_rounds_rail(t : Model; ng : int64; var acc : array<int64>) : int64 {
    let tv = ref_time_ticks()
    finish_pending_step()
    mtp_round_rows(g_df_gw, g_df_gw_toks)
    let vok = verify_batch_groups(t, g_df_ws, g_df_gw.sessions, g_df_gw_toks, g_df_gw.pos, g_df_gw.nr)
    finish_pending_step()
    g_df_split.verify_us += int64(get_time_usec(tv))
    var row = 0l
    for (gi in range64(ng)) {
        var s & = unsafe(*g_df_gw.sessions[gi])
        let pos = g_df_gw.pos[gi]
        if (!vok) {
            df_plain_step(t, s, s.mtp_vbatch[0], pos)
            continue
        }
        let a = mtp_round_accept(t.config, s, pos, g_df_gw.nr[gi] - 1l, row)
        let tc = ref_time_ticks()
        mtp_block_commit(t, s, pos, a + 1l)
        g_df_split.commit_us += int64(get_time_usec(tc))
        acc[g_df_gw.idx[gi]] = a
        row += g_df_gw.nr[gi]
    }
    g_df_gw.sessions |> clear()   // borrowed handles
    return 0l if (!vok)
    g_df_joint_rounds++
    g_df_joint_max = max(g_df_joint_max, ng)
    return ng
}

def private df_drop(uid : uint64) {
    if (key_exists(g_df.rings, uid)) {
        df_ring_release(g_df.rings[uid])
        g_df.rings |> erase(uid)
    }
    for (pd in g_df.pends) {
        if (pd.rows > 0l && pd.uid == uid) {
            pd.rows = 0l
        }
    }
}

//! session `uid`'s ring, made at its first commit
def private df_ring(dr : DflashDrafter; uid : uint64) : DflashRing {
    if (!key_exists(g_df.rings, uid)) {
        let bytes = uint64(dr.n_layers * dr.window * dr.n_kv * dr.hs * 4l)
        g_df.rings[uid] = DflashRing(bk = metal_new_buffer(g_dev, bytes), bv = metal_new_buffer(g_dev, bytes))
    }
    return g_df.rings[uid]
}

//! a norm weight row of the f32 planes as its own buffer (the region cache keys it on the pointer)
def private fw(dr : DflashDrafter; off, n : int64) : MetalBuffer? => upload_region(unsafe(addr<void?>(dr.fblob[off])), uint64(n * 4l))

def private conv_args(dr : DflashDrafter; side : uint; rows : int64; blk : int64 = 0l) : DflashConvArgs {
    let ng = uint(dr.dim / dr.conv_g)
    return DflashConvArgs(dim = uint(dr.dim), g = uint(dr.conv_g), ng = ng, projw = 2u * uint(dr.conv_k) * ng,
        side = side, convk = uint(dr.conv_k), rows = uint(rows), blk = uint(blk))
}

def private qkn_args(dr : DflashDrafter; xs, ys, pos0 : int64) : DflashQknArgs =>
    DflashQknArgs(hs = uint(dr.hs), xs = uint(xs), ys = uint(ys), pos0 = uint(pos0), eps = dr.eps)

//! `rows` rows of x (n wide, a panel of eight rows at least) through plane `w` into y rows `d` apart at `yoff`. Where `wide8`
//! lets a form write whole panels: whole tiles of eight past eight rows on the rows8 form over row tiles, the eight-row forms'
//! row counts (`rows8_rows`) on that form, then the eight-row tensor stamps; else, or where they decline, the row forms
def private df_mm(enc : MetalComputeEncoder?; g : DflashGpu; w : DflashW; bx, by, u_n, u_d : MetalBuffer?; n, d, rows : int64; yoff : uint64 = 0ul;
                  wide8 : bool = true) {
    g_df_sites[int(w.fmt)]++
    let eight = wide8 && rows8_rows(rows)
    if (w.fmt != KqFmt.q8) {
        df_mm_kq(enc, g, w, bx, by, u_n, u_d, n, d, rows, yoff, eight)
        return
    }
    let boff = uint64(w.off)
    if (wide8 && rows > 8l && rows % 8l == 0l) {   //! whole row tiles past eight rows: one dispatch, the plane streamed once
        if (pf_enc_rows8_q8(enc, g.bblob, boff, bx, by, yoff, u_n, u_d, n, d, d, null, null, rows / 8l)) {
            g_df_rows8_sites[int(w.fmt)]++
            return
        }
    }
    if (eight) {
        if (pf_enc_rows8_q8(enc, g.bblob, boff, bx, by, yoff, u_n, u_d, n, d, d)) {
            g_df_rows8_sites[int(w.fmt)]++
            return
        }
        return if (g_env_metal.rows_mm8 && pf_enc_q8_mm8(enc, g.bblob, boff, bx, by, yoff, u_n, u_d, u_d, n, d, d))
    }
    enc_gemv_rows(enc, rows, g.bblob, boff, bx, by, u_n, u_d, u_d, n, d, d, yoff)
}

//! `df_mm` over a K-quant plane, every form reading the plane's own binds: the rows8 form (k4, k6), the format's eight-row tensor
//! stamps, then the f32-x rows route the trunk's K-quant sites take at these row counts
def private df_mm_kq(enc : MetalComputeEncoder?; g : DflashGpu; w : DflashW; bx, by, u_n, u_d : MetalBuffer?; n, d, rows : int64; yoff : uint64;
                     eight : bool) {
    let i = int(w.fmt)
    let at = kq_bind_offs(w.fmt, g.kq_nsb[i], w.off)
    if (rows > 8l && rows % 8l == 0l) {   //! whole row tiles past eight rows: one dispatch, the plane streamed once
        if (pf_enc_rows8_kq_planes(enc, w.fmt, g.kq_q[i], at.qoff, g.kq_s[i], at.soff, at.doff, d, n, d, bx, by, yoff, u_n, u_d, null, null, null, rows / 8l)) {
            g_df_rows8_sites[i]++
            return
        }
    }
    if (eight) {
        if (pf_enc_rows8_kq_planes(enc, w.fmt, g.kq_q[i], at.qoff, g.kq_s[i], at.soff, at.doff, d, n, d, bx, by, yoff, u_n, u_d)) {
            g_df_rows8_sites[int(w.fmt)]++
            return
        }
        return if (g_env_metal.rows_mm8 && pf_enc_kq_mm8_planes(enc, w.fmt, g.kq_q[i], at.qoff, g.kq_s[i], at.soff, at.doff, d, n, d, bx, by, yoff, u_n, u_d, u_d))
    }
    enc_kq_rows_f32(enc, w.fmt, g.kq_q[i], at.qoff, g.kq_s[i], at.soff, at.doff, bx, by, yoff, u_n, u_d, d, n, n, d, rows)
}

//! one command buffer, serially encoded (its dispatches are one chain); no join on the trunk's step queued ahead: nothing here reads its writes
def private df_run_cb(blk : block<(enc : MetalComputeEncoder?) : void>) : bool {
    var err = ""
    var gpu_ms = 0.0lf
    var encode_ms = 0.0lf
    let ok = with_compute_encoder_timed(g_queue, err, gpu_ms, encode_ms) $(enc) {
        invoke(blk, enc)
    }
    g_df.us_gpu += int64(gpu_ms * 1000.0lf)
    g_df.us_encode += int64(encode_ms * 1000.0lf)
    if (!ok) {
        to_log(LOG_ERROR, "dasLLAMA metal dflash: dispatch failed ({err})\n")
    }
    return ok
}

let private DF_PEND_CAP = 512l   //! the rows one staged commit holds at most; a longer plain stretch flushes

//! the staged commit's `rows` rows of captures at `pos0 ..` (one ring run): c = norm(fc . caps), a layer's K rows normed and roped
//! in place in the ring, its V rows straight into the ring
def private df_encode_commit(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; ring : DflashRing; bcaps : MetalBuffer?; rows, pos0 : int64) {
    let d = dr.dim
    let kvd = dr.n_kv * dr.hs
    let ncap = long_length(dr.target_layers)
    df_mm(enc, g, dr.fc, bcaps, g.bfc, g.u_fcn, g.u_d, ncap * d, d, rows)
    enc_rms_b(enc, g.bfc, 0ul, fw(dr, dr.hnorm_off, d), g.bcr, g.u_d, g.u_eps, rows)
    let slot0 = pos0 % dr.window
    let fits = slot0 + 8l <= dr.window   //! the eight-row forms write eight slots: past the ring's end the row forms write `rows` alone
    var kka = qkn_args(dr, kvd, kvd, pos0)
    for (l in range64(dr.n_layers)) {
        let lb = uint64((l * dr.window + slot0) * kvd * 4l)
        df_mm(enc, g, dr.wk[l], g.bcr, ring.bk, g.u_d, g.u_kvd, d, kvd, rows, lb, fits)
        if (dr.biased) {
            df_bias(enc, dr, g, ring.bk, lb, dr.bk_offs[l], kvd, rows)
        }
        enc_dflash_qkn(enc, ring.bk, lb, g.bfblob, uint64(dr.kn_offs[l] * 4l), g.binvf, ring.bk, lb, kka, dr.n_kv, rows, dr.hs)
        df_mm(enc, g, dr.wv[l], g.bcr, ring.bv, g.u_d, g.u_kvd, d, kvd, rows, lb, fits)
        if (dr.biased) {
            df_bias(enc, dr, g, ring.bv, lb, dr.bv_offs[l], kvd, rows)
        }
    }
}

//! every staged commit onto `enc`, then nothing is staged
def private df_encode_pending(enc : MetalComputeEncoder?; dr : DflashDrafter) {
    var g & = g_df
    for (pd in g.pends) {
        continue if (pd.rows <= 0l)
        df_encode_commit(enc, dr, g, df_ring(dr, pd.uid), pd.bcaps, pd.rows, pd.pos0)
        g.commits++
        pd.rows = 0l
    }
}

//! a commit is staged
def private df_pending : bool {
    for (pd in g_df.pends) {
        return true if (pd.rows > 0l)
    }
    return false
}

//! the slot `uid`'s commit stages in: its own where it holds one, else a free one, else -1 (every slot is another session's)
def private df_pend_slot(uid : uint64) : int {
    var free = -1
    for (i in range(DF_PEND_SLOTS)) {
        return i if (g_df.pends[i].rows > 0l && g_df.pends[i].uid == uid)
        if (free < 0 && g_df.pends[i].rows == 0l) {
            free = i
        }
    }
    return free
}

//! the staged commit in a command buffer of its own: a commit that cannot join it, or a pass that must see it, comes next
def private df_flush(dr : DflashDrafter) : bool {
    return true if (!df_pending())
    return df_run_cb() $(enc) {
        df_encode_pending(enc, dr)
    }
}

//! the commit seam: the captures staged on the device behind the commits before them where the rows continue them in the same
//! session within one ring run, else after those ran; the staged run is encoded at the head of the next pass
[no_env]
def private df_commit(dr : DflashDrafter; uid : uint64; caps : array<float>; rows, pos0 : int64) : bool {
    return false if (!g_df.ready)
    var g & = g_df
    let d = dr.dim
    let w = dr.window
    let ncap = long_length(dr.target_layers)
    var done = 0l
    while (done < rows) {
        let p = pos0 + done
        let run = min(rows - done, w - p % w)
        var si = df_pend_slot(uid)
        //! a run joins its session's staged one where it continues it inside the ring's run, the slot's rows and the cap
        let joins = (si >= 0 && g.pends[si].rows > 0l && g.pends[si].pos0 + g.pends[si].rows == p
            && g.pends[si].pos0 % w + g.pends[si].rows + run <= w
            && g.pends[si].rows + run <= DF_PEND_CAP && g.pends[si].rows + run <= g.pends[si].cap)
        if (si < 0 || (g.pends[si].rows > 0l && !joins)) {
            return false if (!df_flush(dr))
            si = 0
        }
        var pd & = unsafe(g.pends[si])
        if (pd.rows == 0l && pd.cap < run) {
            release_handle(pd.bcaps)
            pd.cap = max(run, 2l * pd.cap)
            pd.bcaps = metal_new_buffer(g_dev, uint64(pd.cap * ncap * d * 4l))
        }
        df_caps_rows(dr, g, pd.rows + run)
        unsafe {
            let dst = reinterpret<float?>(metal_buffer_contents(pd.bcaps)) + pd.rows * ncap * d
            memcpy(reinterpret<void?>(dst), addr<void?>(caps[done * ncap * d]), uint64(run * ncap * d * 4l))
        }
        if (pd.rows == 0l) {
            pd.uid = uid
            pd.pos0 = p
        }
        pd.rows += run
        done += run
    }
    return true
}

//! the bias row at `boff` of the f32 planes onto `rows` rows of `w` in `by` at byte `yoff` (a file with no biases: nothing)
def private df_bias(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; by : MetalBuffer?; yoff : uint64; boff, w, rows : int64) {
    return if (!dr.biased)
    tw_bias(enc, by, g.bfblob, GkBiasActArgs(d = uint(w), nelem = uint(rows * w), boff = uint(boff), xoff = uint(yoff / 4ul), act = BIAS_ACT_NONE))
}

//! one drafter layer over the block's `rows` in `bx`, at `pos ..` against `ring`'s positions in view
def private df_encode_layer(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; ring : DflashRing; l, rows, pos : int64; var ka : DflashAttnArgs) {
    df_layer_head(enc, dr, g, l, rows, 0l)
    df_layer_attn(enc, dr, g, ring, l, rows, 0l, pos, ka)
    df_layer_tail(enc, dr, g, l, rows, 0l)
}

//! one drafter layer over `nb` streams' blocks of `n` rows each in `bx` (block b's rows b n ..): every weight site over all the
//! rows at once - the row tiles of one weight pass - the head norm and rope, and the attention, a block at a time against its
//! own ring and positions
def private df_encode_layer_blocks(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; uids : array<uint64>; l, n, nb : int64;
                                   pos : array<int64>; kas : array<DflashAttnArgs>) {
    df_layer_head(enc, dr, g, l, n * nb, n)
    for (b in range64(nb)) {
        var ka = kas[b]
        df_layer_attn(enc, dr, g, df_ring(dr, uids[b]), l, n, b, pos[b], ka)
    }
    df_layer_tail(enc, dr, g, l, n * nb, n)
}

//! a layer's attention inputs over `rows` rows of `bx` (conv blocks of `blk` rows, 0 = one block): the norm and the q, k and
//! v projections
def private df_layer_head(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; l, rows, blk : int64) {
    let d = dr.dim
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let bhc = dr.v2 ? g.bhc : g.bhn   //! a DFlash file's sites read the normed rows and add their outputs as they are
    enc_rms_b(enc, g.bx, 0ul, fw(dr, dr.attn_norm_offs[l], d), g.bhn, g.u_d, g.u_eps, rows)
    if (dr.v2) {
        var cin = conv_args(dr, 0u, rows, blk)
        df_mm(enc, g, dr.aconv_proj[l], g.bhn, g.bdyn, g.u_d, g.u_projw, d, 2l * dr.conv_k * (d / dr.conv_g), rows)
        enc_dflash_conv(enc, g.bhn, g.bhc, g.bdyn, g.bfblob, uint64(dr.aconv_base_offs[l] * 4l), cin, rows * d)
    }
    df_mm(enc, g, dr.wq[l], bhc, g.bq, g.u_d, g.u_qd, d, qd, rows)
    df_mm(enc, g, dr.wk[l], bhc, g.bk, g.u_d, g.u_kvd, d, kvd, rows)
    df_mm(enc, g, dr.wv[l], bhc, g.bv, g.u_d, g.u_kvd, d, kvd, rows)
    if (dr.biased) {
        df_bias(enc, dr, g, g.bq, 0ul, dr.bq_offs[l], qd, rows)
        df_bias(enc, dr, g, g.bk, 0ul, dr.bk_offs[l], kvd, rows)
        df_bias(enc, dr, g, g.bv, 0ul, dr.bv_offs[l], kvd, rows)
    }
}

//! block `b`'s `n` rows at `pos ..` through the head norm and rope, then its attention against `ring`'s positions in view
def private df_layer_attn(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; ring : DflashRing; l, n, b, pos : int64; var ka : DflashAttnArgs) {
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let qo = uint64(b * n * qd * 4l)
    let ko = uint64(b * n * kvd * 4l)
    var qka = qkn_args(dr, qd, qd, pos)
    var kka = qkn_args(dr, kvd, kvd, pos)
    enc_dflash_qkn(enc, g.bq, qo, g.bfblob, uint64(dr.qn_offs[l] * 4l), g.binvf, g.bq, qo, qka, dr.n_heads, n, dr.hs)
    enc_dflash_qkn(enc, g.bk, ko, g.bfblob, uint64(dr.kn_offs[l] * 4l), g.binvf, g.bk, ko, kka, dr.n_kv, n, dr.hs)
    let lb = uint64(l * dr.window * kvd * 4l)
    enc_dflash_attn(enc, g.bq, g.bk, g.bv, ring.bk, lb, ring.bv, lb, g.batt, ka, dr.n_heads, n, dr.hs)
}

//! a layer past its attention over `rows` rows (conv blocks of `blk` rows, 0 = one block): the output projection and its
//! residual, the FFN and its residual - the fused q8 ladder off the eight-row forms, to twelve rows
def private df_layer_tail(enc : MetalComputeEncoder?; dr : DflashDrafter; g : DflashGpu; l, rows, blk : int64) {
    let d = dr.dim
    let v2 = dr.v2
    let qd = dr.n_heads * dr.hs
    let total = rows * d
    let u_tot = g.u_tot[rows]
    let abase = v2 ? uint64(dr.aconv_base_offs[l] * 4l) : 0ul
    let fbase = v2 ? uint64(dr.fconv_base_offs[l] * 4l) : 0ul
    var cin = v2 ? conv_args(dr, 0u, rows, blk) : DflashConvArgs()
    var cout = v2 ? conv_args(dr, 1u, rows, blk) : DflashConvArgs()
    let bhc = v2 ? g.bhc : g.bhn
    let boc = v2 ? g.boc : g.bo
    df_mm(enc, g, dr.wo[l], g.batt, g.bo, g.u_qd, g.u_d, qd, d, rows)
    if (dr.biased) {
        df_bias(enc, dr, g, g.bo, 0ul, dr.bo_offs[l], d, rows)
    }
    if (v2) {
        enc_dflash_conv(enc, g.bo, g.boc, g.bdyn, g.bfblob, abase, cout, total)
    }
    enc_add(enc, g.bx, 0ul, boc, 0ul, u_tot, total)
    enc_rms_b(enc, g.bx, 0ul, fw(dr, dr.ffn_norm_offs[l], d), g.bhn, g.u_d, g.u_eps, rows)
    if (v2) {
        df_mm(enc, g, dr.fconv_proj[l], g.bhn, g.bdyn, g.u_d, g.u_projw, d, 2l * dr.conv_k * (d / dr.conv_g), rows)
        enc_dflash_conv(enc, g.bhn, g.bhc, g.bdyn, g.bfblob, fbase, cin, total)
    }
    if (blk == 0l && !rows8_rows(rows) && rows <= 12l && dr.w1[l].fmt == KqFmt.q8 && dr.w3[l].fmt == KqFmt.q8) {
        enc_gemv_w13sw_rows(enc, rows, g.bblob, uint64(dr.w1[l].off), uint64(dr.w3[l].off), bhc, g.bg, g.u_d, g.u_hidden, g.u_act, d, dr.hidden)
    } else {
        df_mm(enc, g, dr.w1[l], bhc, g.bg, g.u_d, g.u_hidden, d, dr.hidden, rows)
        df_mm(enc, g, dr.w3[l], bhc, g.bu, g.u_d, g.u_hidden, d, dr.hidden, rows)
        enc_swiglu(enc, g.bg, 0ul, g.bu, 0ul, g.u_toth[rows], rows * dr.hidden)
    }
    df_mm(enc, g, dr.w2[l], g.bg, g.bo, g.u_hidden, g.u_d, dr.hidden, d, rows)
    if (v2) {
        enc_dflash_conv(enc, g.bo, g.boc, g.bdyn, g.bfblob, fbase, cout, total)
    }
    enc_add(enc, g.bx, 0ul, boc, 0ul, u_tot, total)
}

//! the block's token rows poked into `btok` (the anchor, then the masks) and the block attention's arguments
def private df_pass_args(dr : DflashDrafter; g : DflashGpu; tok, pos, rows, lo, nctx : int64; tbase : int64 = 0l) : DflashAttnArgs {
    unsafe {
        var tp = reinterpret<uint?>(metal_buffer_contents(g.btok))
        tp[tbase] = uint(tok)
        for (r in range64(1l, rows)) {
            tp[tbase + r] = uint(dr.mask_id)
        }
    }
    return DflashAttnArgs(rows = uint(rows), pos0 = uint(pos), lo = uint(lo), nctx = uint(nctx), w = uint(dr.window), hs = uint(dr.hs),
        nheads = uint(dr.n_heads), grp = uint(dr.n_heads / dr.n_kv), kvd = uint(dr.n_kv * dr.hs), qd = uint(dr.n_heads * dr.hs),
        scale = dr.attn_scale)
}

//! the pass onto `enc`: the staged commit, the block's token rows off the trunk's table, the layers, the first `nd` draft rows
//! normed, the trunk's classifier and the selector's projection over them
def private df_encode_pass(enc : MetalComputeEncoder?; t : Model; dr : DflashDrafter; ring : DflashRing; pos, rows, nd : int64; var ka : DflashAttnArgs) {
    let g & = g_df
    let d = dr.dim
    df_encode_pending(enc, dr)
    enc_embed_dev(enc, t, g.btok, 0ul, g.bx, 0ul, rows, g.u_d, g.u_escale)
    for (l in range64(dr.n_layers)) {
        df_encode_layer(enc, dr, g, ring, l, rows, pos, ka)
    }
    df_encode_draft_rows(enc, t, dr, g.bx, uint64(d * 4l), nd)
}

//! `nr` draft rows at `xoff` of `bx` normed, the trunk's classifier over them (row tiles past eight) and the selector's
//! projection where the file carries one
def private df_encode_draft_rows(enc : MetalComputeEncoder?; t : Model; dr : DflashDrafter; bx : MetalBuffer?; xoff : uint64; nr : int64) {
    let g & = g_df
    enc_rms_b(enc, bx, xoff, fw(dr, dr.norm_off, dr.dim), g.bhn, g.u_d, g.u_eps, nr)
    if (nr > 8l) {
        metal_rows_tiled_scope() {
            enc_block_cls_rows(enc, t, g.bhn, g.blog, g.u_d, g.u_vocab, nr)
        }
    } else {
        enc_block_cls_rows(enc, t, g.bhn, g.blog, g.u_d, g.u_vocab, nr)
    }
    if (dr.v2) {
        df_mm(enc, g, dr.sel_h, g.bhn, g.bhsel, g.u_d, g.u_rank, dr.dim, dr.rank, nr)
    }
}

//! the device selector after the pass on `enc` over `ns` streams' `nd` draft rows each (blocks of `rows` token rows): each
//! draft row's top-k over its logits - a list a slice, the slices' lists merged - then a stream's walk into `bids`, greedy, or
//! at `temp` above 0 drawn off the LCG state in `brng` with each position's q into `bsq`
def private df_encode_select(enc : MetalComputeEncoder?; g : DflashGpu; nd : int64; temp : float; ns : int64 = 1l; rows : int64 = 0l) {
    if (g.plain) {
        enc_argmax_rows(enc, g.blog, g.bcid, g.u_vocab, ns * nd)
        return
    }
    df_encode_topk(enc, g, ns * nd)
    for (b in range64(ns)) {
        df_encode_walk(enc, g, nd, temp, uint(b * rows), uint(b * nd))
    }
}

//! each of `rows` draft rows' top-k over its logits: a list a slice, the slices' lists merged
def private df_encode_topk(enc : MetalComputeEncoder?; g : DflashGpu; rows : int64) {
    let k = g.sel_args.k
    var ska = DflashTopkArgs(vocab = uint(g.vocab), k = k, span = uint(DFLASH_SEL_SPAN), nsl = uint(g.sel_nsl))
    var mka = DflashTopkArgs(vocab = uint(g.sel_nsl) * k, k = k, span = 0u, nsl = 1u)
    enc_dflash_topk_slices(enc, g.blog, g.blog, g.bpid, g.bpval, ska, g.sel_nsl, rows)
    enc_dflash_topk_merge(enc, g.bpval, g.bpid, g.bcid, g.bcval, mka, rows)
}

//! the walk over `nd` positions from the anchor at `tbase` of `btok`, the rows from `rbase`, into its stream's slots of `bids`
def private df_encode_walk(enc : MetalComputeEncoder?; g : DflashGpu; nd : int64; temp : float; tbase, rbase : uint) {
    var wka = g.sel_args
    wka.nd = uint(nd)
    wka.temp = temp
    wka.tbase = tbase
    wka.rbase = rbase
    let ps = g.sel_ps != null ? g.sel_ps : g.sel_pq
    let ns = g.sel_ns != null ? g.sel_ns : g.sel_nq
    if (temp > 0.0) {
        enc_dflash_walk_soft(enc, g.btok, g.bhsel, g.bcid, g.bcval, g.sel_pq, g.sel_pqoff, g.sel_pq, ps, ps, g.sel_nq, g.sel_nqoff, g.sel_nq, ns, ns, g.bids, wka,
            g.bsq, g.brng)
    } else {
        enc_dflash_walk(enc, g.btok, g.bhsel, g.bcid, g.bcval, g.sel_pq, g.sel_pqoff, g.sel_pq, ps, ps, g.sel_nq, g.sel_nqoff, g.sel_nq, ns, ns, g.bids, wka)
    }
}

//! a sampled walk's landing: every position's candidates into cids[r * topk ..] (an empty slot as -1), their q into
//! qs[r * topk ..], and the LCG state the walk stepped back into `rng`
def private df_land_soft(g : DflashGpu; nd : int64; var rng : int4&; var cids : array<int64>; var qs : array<float>) {
    let k = int64(g.sel_args.k)
    cids |> reserve_resize(nd * k)
    qs |> reserve_resize(nd * k)
    unsafe {
        let cp = reinterpret<uint const?>(metal_buffer_contents(g.bcid))
        for (i in range64(nd * k)) {
            cids[i] = cp[i] == DFLASH_SEL_NONE ? -1l : int64(cp[i])
        }
        memcpy(addr<void?>(qs[0]), metal_buffer_contents(g.bsq), uint64(nd * k * 4l))
        rng.x = *reinterpret<int const?>(metal_buffer_contents(g.brng))
    }
}

//! stream `b`'s `nd` picks into out[at .. at + nd) (a slot no id filled as -1)
def private df_land_ids(g : DflashGpu; var out : array<int64>; nd : int64; at : int64 = 1l; b : int64 = 0l) {
    unsafe {
        let ip = reinterpret<uint const?>(metal_buffer_contents(g.plain ? g.bcid : g.bids))
        let first = g.plain ? b * nd : b * (nd + 1l) + 1l   //! a walk's picks stand past its anchor; a DFlash file's rows' best ids from the first
        for (r in range64(nd)) {
            let id = ip[r + first]
            out[at + r] = id == DFLASH_SEL_NONE ? -1l : int64(id)
        }
    }
}

//! the pass in a command buffer committed with no wait: the verify committed after it reads its picks, ordered by the queue and
//! the tracked `bids` (a hazard tracker starts fresh a command buffer and orders nothing across two)
def private df_submit_cb(blk : block<(enc : MetalComputeEncoder?) : void>) : MetalCommandBuffer? {
    var cb = metal_new_command_buffer(g_queue)
    var enc = metal_new_compute_encoder(cb)
    let ts = ref_time_ticks()
    invoke(blk, enc)
    metal_end_encoding(enc)
    g_df.us_encode += int64(get_time_usec(ts))
    metal_release(enc)
    metal_commit(cb)
    return cb
}

//! the select seam: the pass and the device selector in one command buffer, held unwaited as the verify's rows where `may_hold`
//! and the verify takes its rows off the device, else waited, the picks into drafts[1 ..]; at `temp` above 0 the selector draws
//! off `rng`, the candidates and q into `cids` / `qs` (by the land where held); `may_hold_ahead` holds a sampled round's pass too
[no_env, arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-sampled-drafts")]
def private df_select(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; may_hold, may_hold_ahead : bool; temp : float;
                      var rng : int4&; var drafts : array<int64>; var cids : array<int64>; var qs : array<float>) : DflashSelect {
    let soft = temp > 0.0
    return DflashSelect.failed if (!df_selects(soft) || g_df_held != null || rows > dr.nblock || nd < 1l || nd >= rows || nctx + rows > int64(DFLASH_ATTN_KEYS))
    var g & = g_df
    let ring = df_ring(dr, uid)
    var ka = df_pass_args(dr, g, tok, pos, rows, lo, nctx)
    if (soft) {
        *unsafe(reinterpret<int?>(metal_buffer_contents(g.brng))) = rng.x
    }
    let greedy_hold = may_hold && !soft && !g.plain   //! a held pass's picks are the walk's rows in `bids`
    let takes = metal_verify_takes_device_rows(t)
    let late = !takes && g_df_ahead >= 2 && metal_verify_takes_device_rows_late(t)
    if ((greedy_hold || (g_df_ahead >= 1 && may_hold_ahead)) && (takes || late)) {
        let ts = ref_time_ticks()
        g_df_held = df_submit_cb() $(enc) {
            df_encode_pass(enc, t, dr, ring, pos, rows, nd, ka)
            df_encode_select(enc, g, nd, temp)
        }
        g.passes++
        g.selected++
        g.held++
        if (!greedy_hold || late) {
            g.ahead++
        }
        if (soft) {
            g.soft++
        }
        g_df_held_soft = soft
        metal_verify_rows_on_device(g.bids, df_accepts(), late)
        g_df_held_submit_us = int64(get_time_usec(ts))
        return DflashSelect.held
    }
    let ok = df_run_cb() $(enc) {
        df_encode_pass(enc, t, dr, ring, pos, rows, nd, ka)
        df_encode_select(enc, g, nd, temp)
    }
    return DflashSelect.failed if (!ok)
    g.passes++
    g.selected++
    df_land_ids(g, drafts, nd)
    if (soft) {
        g.soft++
        df_land_soft(g, nd, rng, cids, qs)
    }
    return DflashSelect.landed
}

//! The joint select: every stream's pass in one command buffer - its block through the layers, its first `nd` draft rows
//! gathered a stream after a stream - then one norm, one classifier pass and one select over all of them, waited once; stream
//! b's picks into picks[b * nd ..]. False where the selector, the shapes or the buffer declines (a stream drafts at a time)
[no_env]
def private df_select_joint(t : Model; dr : DflashDrafter; uids : array<uint64>; toks, pos, lo, nctx : array<int64>; rows, nd : int64;
                            var picks : array<int64>) : bool {
    let ns = long_length(uids)
    return false if (!df_selects(false) || g_df_held != null || ns < 1l || ns > DF_JOINT_PASS_STREAMS || rows != dr.nblock || nd < 1l || nd >= rows
        || ns * nd > DF_JOINT_ROWS || g_df.bdx == null)
    for (b in range64(ns)) {
        return false if (nctx[b] + rows > int64(DFLASH_ATTN_KEYS))
    }
    var g & = g_df
    let d = dr.dim
    let r_all = ns * nd
    let ok = df_run_cb() $(enc) {
        df_encode_pending(enc, dr)
        if (ns > 1l && ns * rows <= DF_PASS_ROWS && (ns * rows) % 8l == 0l) {   //! every stream's block through the layers together
            g_df_kas |> resize(ns)
            for (b in range64(ns)) {
                g_df_kas[b] = df_pass_args(dr, g, toks[b], pos[b], rows, lo[b], nctx[b], b * rows)
                g_df_kas[b].rbase = uint(b * rows)
            }
            enc_embed_dev(enc, t, g.btok, 0ul, g.bx, 0ul, ns * rows, g.u_d, g.u_escale)
            for (l in range64(dr.n_layers)) {
                df_encode_layer_blocks(enc, dr, g, uids, l, rows, ns, pos, g_df_kas)
            }
            for (b in range64(ns)) {
                enc_copy_row(enc, g.bx, uint64((b * rows + 1l) * d * 4l), g.bdx, uint64(b * nd * d * 4l), g.u_tot[nd], nd * d)
            }
        } else {
            for (b in range64(ns)) {
                let ring = df_ring(dr, uids[b])
                var ka = df_pass_args(dr, g, toks[b], pos[b], rows, lo[b], nctx[b], b * rows)
                enc_embed_dev(enc, t, g.btok, uint64(b * rows * 4l), g.bx, 0ul, rows, g.u_d, g.u_escale)
                for (l in range64(dr.n_layers)) {
                    df_encode_layer(enc, dr, g, ring, l, rows, pos[b], ka)
                }
                enc_copy_row(enc, g.bx, uint64(d * 4l), g.bdx, uint64(b * nd * d * 4l), g.u_tot[nd], nd * d)
            }
        }
        df_encode_draft_rows(enc, t, dr, g.bdx, 0ul, r_all)
        df_encode_select(enc, g, nd, 0.0, ns, rows)
    }
    return false if (!ok)
    g.passes += ns
    g.selected += ns
    picks |> resize(r_all)
    for (b in range64(ns)) {
        df_land_ids(g, picks, nd, b * nd, b)
    }
    return true
}

var private @scratch g_df_kas : array<DflashAttnArgs>   //! a joint pass's blocks' attention arguments

var private g_df_trace_prev_end = 0.0lf   //! the traced round before's verify GPU end (seconds), 0 = none

//! a held round's split printed: the host's pass submit, its way to the verify seat, the seat's prep, the verify's encode, wait
//! and landing, the pass's wait at the land; the GPU's pass, its idle to the verify's start, the verify's driver window (its
//! commit's residency pass) and its queue time, and the verify (`gs` / `ge` the pass's GPU window, `ks` / `ke` its driver window)
[cold_path]
def private df_trace_split(gs, ge, ks, ke : double; land_wait_us : int) {
    let v = metal_verify_split()
    let ms = 1000.0lf
    let gap = g_df_trace_prev_end > 0.0lf ? (gs - g_df_trace_prev_end) * ms : -1.0lf
    let cgap = g_df_trace_prev_end > 0.0lf ? (ks - g_df_trace_prev_end) * ms : -1.0lf
    g_df_trace_prev_end = v.gpu_end
    print("  held round host: pass submit {double(g_df_held_submit_us) / ms} ms, to seat {double(v.to_seat_us) / ms}, seat prep {double(v.prep_us) / ms} (retire {double(v.retire_us) / ms}, recurrent mirror {double(v.dn_us) / ms}), verify encode {double(v.enc_us) / ms}, verify wait {double(v.wait_us) / ms}, verify land {double(v.land_us) / ms}, pass wait at land {double(land_wait_us) / ms} | GPU: last verify to pass {gap} ms (pass drv {(ke - ks) * ms}, queue {(gs - ke) * ms}, last verify to pass commit {cgap}), pass {(ge - gs) * ms} ms, idle to verify {(v.gpu_start - ge) * ms} ms, verify drv {(v.kernel_end - v.kernel_start) * ms} ms, verify queue {(v.gpu_start - v.kernel_end) * ms} ms, verify {(v.gpu_end - v.gpu_start) * ms} ms\n")
}

//! the land seam: the held pass waited (the verify after it already was), its picks into drafts[1 .. nd]; the pass's GPU
//! milliseconds back, negative where it failed
[no_env]
def private df_land(var drafts : array<int64>; nd : int64; var rng : int4&; var cids : array<int64>; var qs : array<float>) : double {
    metal_verify_rows_on_device(null)
    var cb = g_df_held
    let soft = g_df_held_soft
    g_df_held_soft = false
    return -1.0lf if (cb == null)
    g_df_held = null
    let ts_wait = ref_time_ticks()
    metal_wait_until_completed(cb)
    let land_wait_us = get_time_usec(ts_wait)
    let err = metal_command_buffer_error(cb)
    let gs = metal_command_buffer_gpu_start_time(cb)
    let ge = metal_command_buffer_gpu_end_time(cb)
    let gpu_ms = (ge - gs) * 1000.0lf
    let ks = metal_command_buffer_kernel_start_time(cb)
    let ke = metal_command_buffer_kernel_end_time(cb)
    metal_release(cb)
    if (dflash_trace_on()) {
        df_trace_split(gs, ge, ks, ke, land_wait_us)
    }
    g_df.us_gpu += int64(gpu_ms * 1000.0lf)
    if (!empty(err)) {
        to_log(LOG_ERROR, "dasLLAMA metal dflash: the held pass failed ({err})\n")
        return -1.0lf
    }
    df_land_ids(g_df, drafts, nd)
    if (soft) {
        df_land_soft(g_df, nd, rng, cids, qs)
    }
    return gpu_ms
}

//! a late verify's early land: the held pass waited and its `nd` picks into drafts[1 ..] before the verify commits; the pass stays
//! held, so the round's land still lands what it owes (false = no held pass, or it failed)
def private df_land_early(var drafts : array<int64>; nd : int64) : bool {
    return false if (g_df_held == null)
    metal_wait_until_completed(g_df_held)
    return false if (!empty(metal_command_buffer_error(g_df_held)))
    df_land_ids(g_df, drafts, nd)
    return true
}

//! the pass seam: the pass, its first `nd` draft rows' logits and selector projections landed for the host's walk
[no_env]
def private df_pass(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; var logits : array<float>; var hsel : array<float>) : bool {
    return false if (!g_df.ready || g_df_held != null || rows > dr.nblock || nd < 1l || nd >= rows || nctx + rows > int64(DFLASH_ATTN_KEYS))
    var g & = g_df
    let ring = df_ring(dr, uid)
    var ka = df_pass_args(dr, g, tok, pos, rows, lo, nctx)
    let ok = df_run_cb() $(enc) {
        df_encode_pass(enc, t, dr, ring, pos, rows, nd, ka)
    }
    return false if (!ok)
    g.passes++
    logits |> reserve_resize(nd * g.vocab)
    hsel |> reserve_resize(nd * dr.rank)
    unsafe {
        memcpy(addr<void?>(logits[0]), metal_buffer_contents(g.blog), uint64(nd * g.vocab * 4l))
        if (dr.v2) {
            memcpy(addr<void?>(hsel[0]), metal_buffer_contents(g.bhsel), uint64(nd * dr.rank * 4l))
        }
    }
    return true
}

[init]
def metal_dflash_init {
    register_dflash_device("metal", DflashDeviceSeams(ready = @@df_ready, commit = @@df_commit, draft_pass = @@df_pass, selects = @@df_selects,
        select_pass = @@df_select, land = @@df_land, drop = @@df_drop, detach = @@df_detach))
    register_dflash_joint(@@df_select_joint)
    register_mtp_block_rounds("metal", MtpBlockRoundSeams(serves = @@df_joint_serves, seat_serves = @@metal_mtp_seat_shape_ok, rounds = @@df_rounds))
}
