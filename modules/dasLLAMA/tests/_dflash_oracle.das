// The block drafter's device selector cells' synthetic fixture and CPU oracles, shared by the Metal and the discrete homes' kernel
// cells (`GkDflashTopk`, `GkDflashWalk`): logits on a tie-filling grid, the top-k oracle, selector codebooks in every format the
// walk dequantizes, the oracle's walk, and a codebook's planes as a cell binds them.

options gen2
options _dasllama_internal = true

module _dflash_oracle shared public

require dasllama/dasllama_lint public
require daslib/f16_cvt   // f32_to_f16, f16_to_f32: the codebooks' half scales
require dasllama/dasllama_kqformat   // kq_qsb, kq_ssb: a superblock's quant and scale bytes
require dasllama/dasllama_convert   // dequant_kq_plane_sb: the oracle's codebook rows
require dasllama/dasllama_dflash public   // DflashDrafter, DflashW: the codebooks' home
require dastest/testing_boost public
require math   // min: a list's live slots
require _compares   // check_exact: the lists' live slots

let DF_NONE = 0xFFFFFFFFu   //! the top-k's empty slot
let DF_OUT_SENTINEL = 0xFFFFFFFEu   //! an id no run writes

def df_hash(i : int) : int => ((i * 1103515245 + 12345) >> 8) & 0x7fffffff

//! `rows` logits rows of `vocab`: values on a 0.25 grid, so ties fill the k-th boundary, the ids `top_a` < `top_b` tied at the
//! row's maximum
def df_logits(rows, vocab, top_a, top_b : int) : array<float> {
    var lg <- [for (i in range(rows * vocab)); float(df_hash(i) % 64) * 0.25 - 8.0]
    for (r in range(rows)) {
        if (top_b < vocab) {
            lg[r * vocab + top_a] = 9.0
            lg[r * vocab + top_b] = 9.0
        }
    }
    return <- lg
}

//! the oracle's list of the run lg[base + lo .. base + hi): its ids sorted by (logit desc, id asc), the first `k` appended,
//! the empty slot (DF_NONE / -3.4e38) past the run's ids
def df_topk_append(lg : array<float>; base, lo, hi, k : int; ids_desc : bool; var ids : array<uint>; var vals : array<float>) {
    var order <- [for (i in range(lo, hi)); i]
    sort(order) $(a, b) {
        let va = lg[base + a]
        let vb = lg[base + b]
        return va > vb || (va == vb && (ids_desc ? a > b : a < b))
    }
    ids |> reserve(length(ids) + k)
    vals |> reserve(length(vals) + k)
    for (j in range(k)) {
        ids |> push(j < length(order) ? uint(order[j]) : DF_NONE)
        vals |> push(j < length(order) ? lg[base + order[j]] : -3.4e38)
    }
    delete order
}

//! the oracle's top-k of every row: a list a row
def df_topk_oracle(lg : array<float>; rows, vocab, k : int; ids_desc : bool) : tuple<ids : array<uint>; vals : array<float>> {
    var ids : array<uint>
    ids |> reserve(rows * k)
    var vals : array<float>
    vals |> reserve(rows * k)
    for (r in range(rows)) {
        df_topk_append(lg, r * vocab, 0, vocab, k, ids_desc, ids, vals)
    }
    return (ids <- ids, vals <- vals)
}

//! one selector codebook of `vocab` rows `rank` wide appended to `dr` in format `f`, behind a lead of foreign bytes (two, so a Q8_0
//! codebook sits two bytes off a word) or two foreign superblocks; quants hashed off `salt`, every half scale a sane magnitude
def df_codebook(var dr : DflashDrafter; f : KqFmt; vocab, rank, salt : int) : DflashW {
    if (f == KqFmt.q8) {
        let lead = length(dr.blob)
        let off = lead + 2
        let nb = vocab * rank / 32
        dr.blob |> resize(off + nb * 34)
        dr.blob[lead] = uint8(0x5a)
        dr.blob[lead + 1] = uint8(0xa5)
        for (b in range(nb)) {
            let h = f32_to_f16(0.01 + 0.0003 * float(df_hash(b + salt) % 100) * ((b & 1) == 0 ? 1.0 : -1.0))
            let bo = off + b * 34
            dr.blob[bo] = uint8(h & 0xffu)
            dr.blob[bo + 1] = uint8(h >> 8u)
            for (k in range(32)) {
                dr.blob[bo + 2 + k] = uint8(df_hash(b * 32 + k + salt * 7) & 0xff)
            }
        }
        return DflashW(fmt = f, off = int64(off))
    }
    let fi = int(f)
    let qsb = int(kq_qsb(f))
    let ssb = int(kq_ssb(f))
    let sb_lead = length(dr.kq[fi].q) / qsb
    let sb0 = sb_lead + 2
    let nsb = vocab * rank / 256
    dr.kq[fi].q |> resize((sb0 + nsb) * qsb)
    dr.kq[fi].s |> resize((sb0 + nsb) * ssb)
    for (i in range(sb_lead * qsb, (sb0 + nsb) * qsb)) {
        dr.kq[fi].q[i] = uint8(df_hash(i + salt * 13) & 0xff)
    }
    for (i in range(sb_lead * ssb, (sb0 + nsb) * ssb)) {
        dr.kq[fi].s[i] = uint8(df_hash(i + salt * 29) & 0xff)
    }
    if (f == KqFmt.nv4) {   // sixteen UE4M3 sub-block scales a superblock and no header: a narrow band of exponents
        for (i in range(sb_lead * ssb, (sb0 + nsb) * ssb)) {
            dr.kq[fi].s[i] = uint8((uint(dr.kq[fi].s[i]) & 0x0Fu) | 0x30u)
        }
        return DflashW(fmt = f, off = int64(sb0 * 256))
    }
    for (sb in range(sb_lead, sb0 + nsb)) {
        let d = f32_to_f16(0.002 + 0.00005 * float(df_hash(sb + salt) % 100))
        let dmin = f32_to_f16(0.001 + 0.00003 * float(df_hash(sb * 3 + salt) % 100))
        let at = f == KqFmt.k6 ? sb * ssb + 16 : sb * ssb
        dr.kq[fi].s[at] = uint8(d & 0xffu)
        dr.kq[fi].s[at + 1] = uint8(d >> 8u)
        if (f != KqFmt.k6) {
            dr.kq[fi].s[at + 2] = uint8(dmin & 0xffu)
            dr.kq[fi].s[at + 3] = uint8(dmin >> 8u)
        }
    }
    return DflashW(fmt = f, off = int64(sb0 * 256))
}

//! codebook row `src`'s bytes copied over row `dst`'s: two candidates whose successor rows, and so whose scores, tie exactly
def df_copy_row(var dr : DflashDrafter; w : DflashW; rank, src, dst : int) {
    if (w.fmt == KqFmt.q8) {
        let rowb = rank / 32 * 34
        for (i in range(rowb)) {
            dr.blob[int(w.off) + dst * rowb + i] = dr.blob[int(w.off) + src * rowb + i]
        }
        return
    }
    let fi = int(w.fmt)
    let qsb = int(kq_qsb(w.fmt))
    let ssb = int(kq_ssb(w.fmt))
    let sb0 = int(w.off) / 256
    let per = rank / 256
    for (i in range(per * qsb)) {
        dr.kq[fi].q[(sb0 + dst * per) * qsb + i] = dr.kq[fi].q[(sb0 + src * per) * qsb + i]
    }
    for (i in range(per * ssb)) {
        dr.kq[fi].s[(sb0 + dst * per) * ssb + i] = dr.kq[fi].s[(sb0 + src * per) * ssb + i]
    }
}

//! codebook row `row` of `w` read as the oracle reads it: a Q8_0 block's q * d, a K-quant superblock through the engine's dequant
def df_book_row(dr : DflashDrafter; w : DflashW; rank, row : int) : array<float> {
    var out : array<float>
    out |> resize(rank)
    if (w.fmt == KqFmt.q8) {
        let rowb = rank / 32 * 34
        for (b in range(rank / 32)) {
            let bo = int(w.off) + row * rowb + b * 34
            let d = f16_to_f32(uint(dr.blob[bo]) | (uint(dr.blob[bo + 1]) << 8u))
            for (k in range(32)) {
                let q = int(dr.blob[bo + 2 + k])
                out[b * 32 + k] = float(q >= 128 ? q - 256 : q) * d
            }
        }
        return <- out
    }
    let sb0 = (w.off + int64(row * rank)) / 256l
    for (b in range64(int64(rank / 256))) {
        dequant_kq_plane_sb(w.fmt, dr.kq[int(w.fmt)].q, dr.kq[int(w.fmt)].s, sb0 + b, false, out, b * 256l)
    }
    return <- out
}

//! the oracle's walk: position r's candidates scored logit + sum_i (A[pred][i] * h_r[i]) * B[cand][i] in ascending i, the first
//! best slot the pick (`last_best` the last one, `unary` the logit alone - the controls); the anchor at 0, then the picks
def df_walk_oracle(dr : DflashDrafter; tok, nd, k, rank : int; cid : array<uint>; cval, hsel : array<float>; last_best, unary : bool) : array<uint> {
    var ids : array<uint>
    ids |> reserve(nd + 1)
    ids |> push(uint(tok))
    var pred = tok
    for (r in range(nd)) {
        var inscope arow <- df_book_row(dr, dr.sel_prev, rank, pred)
        var best = 0
        var bests = -3.0e38
        for (j in range(k)) {
            let cand = cid[r * k + j]
            continue if (cand == DF_NONE)
            var sc = cval[r * k + j]
            if (!unary) {
                var inscope brow <- df_book_row(dr, dr.sel_next, rank, int(cand))
                for (i in range(rank)) {
                    let ah = arow[i] * hsel[r * rank + i]
                    sc += ah * brow[i]
                }
            }
            if (sc > bests || (last_best && sc == bests)) {
                bests = sc
                best = j
            }
        }
        pred = int(cid[r * k + best])
        ids |> push(uint(pred))
    }
    return <- ids
}

let DF_WALK_ND = 7    //! the walk gates' positions
let DF_WALK_TOK = 4   //! ... and anchor

//! the walk gates' fixture: a drafter holding the two codebooks (successor rows 3 and 5 equal, so they tie), DF_WALK_ND rows of
//! logits and their top-k, the selector rows (position 0's zero)
struct DfWalkFixture {
    dr : DflashDrafter
    top : tuple<ids : array<uint>; vals : array<float>>
    hsel : array<float>
    cids : array<int64>   //! the top-k's ids as the host walk reads them, an empty slot -1
}

def df_walk_fixture(pf, nf : KqFmt; rank, vocab, k : int) : DfWalkFixture {
    var fx : DfWalkFixture
    fx.dr.rank = int64(rank)
    fx.dr.topk = int64(k)
    fx.dr.vocab = int64(vocab)
    fx.dr.sel_prev = df_codebook(fx.dr, pf, vocab, rank, 11)
    fx.dr.sel_next = df_codebook(fx.dr, nf, vocab, rank, 23)
    if (vocab > 5) {
        df_copy_row(fx.dr, fx.dr.sel_next, rank, 3, 5)
    }
    var inscope lg <- df_logits(DF_WALK_ND, vocab, 3, 5)
    fx.top <- df_topk_oracle(lg, DF_WALK_ND, vocab, k, false)
    fx.hsel <- [for (i in range(DF_WALK_ND * rank)); i < rank ? 0.0 : float(df_hash(i + 77) % 400) * 0.01 - 2.0]
    fx.cids <- [for (v in fx.top.ids); v == DF_NONE ? -1l : int64(v)]
    return <- fx
}

//! codebook `w`'s planes as a cell binds them: the quants from the codebook's first byte's four-aligned floor (`q0` the bytes
//! between, two on a Q8_0 book by `df_codebook`'s lead) and the scales from its first superblock; a Q8_0 book has no scale plane
def df_book_planes(dr : DflashDrafter; w : DflashW) : tuple<q : array<uint8>; s : array<uint8>; q0 : uint> {
    if (w.fmt == KqFmt.q8) {
        return (q <- [for (i in range64(w.off & ~3l, long_length(dr.blob))); dr.blob[i]], s <- array<uint8>(), q0 = uint(w.off & 3l))
    }
    let sb0 = w.off / 256l
    let fi = int(w.fmt)
    return (q <- [for (i in range64(sb0 * kq_qsb(w.fmt), long_length(dr.kq[fi].q))); dr.kq[fi].q[i]],
        s <- [for (i in range64(sb0 * kq_ssb(w.fmt), long_length(dr.kq[fi].s))); dr.kq[fi].s[i]], q0 = 0u)
}

//! `got` against `want` as lists of `k`, list i's first `live[i]` slots through the exact compare - its poisoned-expectation
//! control landing on a slot a run fills - and its empty slots compared apart
def df_check_lists(t : T?; tag : string; got, want : array<auto(TT)>; live : array<int>; k : int) {
    var inscope g_live, w_live : array<TT -const>
    g_live |> reserve(length(got))
    w_live |> reserve(length(got))
    var empty_bad = 0
    var first_bad = -1
    for (l, i in live, count()) {
        for (j in range(k)) {
            let x = i * k + j
            if (j < l) {
                g_live |> push(got[x])
                w_live |> push(want[x])
            } elif (got[x] != want[x]) {
                first_bad = first_bad < 0 ? x : first_bad
                empty_bad++
            }
        }
    }
    check_exact(t, tag, g_live, w_live)
    t |> equal(empty_bad, 0, "{tag}: every empty slot past a list's ids holds the empty slot ({empty_bad} off, the first at {first_bad})")
}

let DF_TOPK_SPAN = 4096   //! the logits a slice of the kernel's first stage reads

//! a top-k gate's shape: `rows` rows of `vocab` logits, the `k` best a row
struct DfTopkLeg {
    rows : int
    vocab : int
    k : int
}

//! the top-k gates' shapes on every home: the 27B drafter's (seven rows of the served vocab, top-16, a tail slice), whole
//! slices, the deepest list a lane keeps, a row narrower than the list (lanes with no id, empty slots), a tail slice of four
def df_topk_legs : array<DfTopkLeg> => <- [DfTopkLeg(rows = 7, vocab = 248320, k = 16), DfTopkLeg(rows = 3, vocab = 12288, k = 7),
    DfTopkLeg(rows = 2, vocab = 50000, k = 32), DfTopkLeg(rows = 2, vocab = 10, k = 16), DfTopkLeg(rows = 2, vocab = 4100, k = 16)]

//! a top-k leg's fixture: the logits (tied at each row's maximum and on a grid, so ties fill the k-th), the oracle's rows and
//! their ids-descending control, the oracle's list of each slice, and the live slots a list (the rest hold the empty slot)
struct DfTopkFixture {
    nsl : int
    lg : array<float>
    want : tuple<ids : array<uint>; vals : array<float>>
    flipped : tuple<ids : array<uint>; vals : array<float>>
    part_ids : array<uint>
    part_vals : array<float>
    part_live : array<int>
    row_live : array<int>
}

def df_topk_fixture(leg : DfTopkLeg) : DfTopkFixture {
    let nsl = (leg.vocab + DF_TOPK_SPAN - 1) / DF_TOPK_SPAN
    var lg <- df_logits(leg.rows, leg.vocab, 3, 5)
    var part_ids : array<uint>
    var part_vals : array<float>
    var part_live : array<int>
    part_live |> reserve(leg.rows * nsl)
    for (r in range(leg.rows)) {
        for (sl in range(nsl)) {
            let lo = sl * DF_TOPK_SPAN
            let hi = min(lo + DF_TOPK_SPAN, leg.vocab)
            df_topk_append(lg, r * leg.vocab, lo, hi, leg.k, false, part_ids, part_vals)
            part_live |> push(min(leg.k, hi - lo))
        }
    }
    var want <- df_topk_oracle(lg, leg.rows, leg.vocab, leg.k, false)
    var flipped <- df_topk_oracle(lg, leg.rows, leg.vocab, leg.k, true)
    return <- DfTopkFixture(nsl = nsl, lg <- lg, want <- want, flipped <- flipped, part_ids <- part_ids, part_vals <- part_vals,
        part_live <- part_live, row_live <- [for (_r in range(leg.rows)); min(leg.k, leg.vocab)])
}
