// The routed block's routing kernels' CPU oracles and fixtures, shared by the Vulkan and CUDA kernel cells: the router GEMM's
// dot loop, the host select over the rows (`moe_select_core`), the combine-folded residual step's fixture rows and oracle,
// and the compares each cell runs over its home's output. Device-free: a cell binds its own planes and dispatches its home's
// stamp, then hands the rows here.

options gen2
options _dasllama_internal = true

module _moe_routing_oracles shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require math
require dasllama/dasllama_common   // MoeGate: the select oracle takes the softmax router
require dasllama/dasllama_moe   // moe_select_core: the host select
require dasllama/dasllama_math   // rmsnorm: the combine's next norm
require dasllama/dasllama_gpu_math   // sigmoid_f32: the shared expert's gate
require dasllama/dasllama_gpu_kernels_discrete   // SCHED_S_ROWS / SCHED_M_ROWS: the schedule cell's bucket shapes
require _compares
require _kernel_oracles   // half_at: the f16 normed rows' reading
require _vk_kq_fixtures   // hash_word

let MR_NPOS = 37
let MR_NE = 20
let MR_K = 3
let MR_DIM = 2 * 64   //! two of the router tile's 64-wide K steps
let MR_NER = MR_NE + 1   //! the shared expert's gate row rides past the experts

//! one 32-bit word to a float in [-2, 2)
def hash_unit(i : uint) : float => float(hash_word(i) % 4096u) / 1024.0 - 2.0

//! the router GEMM cell's planes: two layers' [ner x dim] rows (the dispatch reads the second through woff), the npos x rows
def router_gemm_planes(ner : int; var wh, xh : array<float>) {
    let nw = 2 * ner * MR_DIM
    wh |> resize(nw)
    xh |> resize(MR_NPOS * MR_DIM)
    for (i in range(nw)) {
        wh[i] = hash_unit(uint(i) + 11u) * 0.25
    }
    for (i in range(MR_NPOS * MR_DIM)) {
        xh[i] = hash_unit(uint(i) + 7_777u)
    }
}

//! the router GEMM's oracle: the second layer's rows dotted with every x row, logits at the ner stride
def router_gemm_oracle(wh, xh : array<float>; ner : int) : array<float> {
    var want : array<float>
    want |> resize(MR_NPOS * ner)
    for (p in range(MR_NPOS)) {
        for (e in range(ner)) {
            var acc = 0.0
            for (c in range(MR_DIM)) {
                acc += wh[ner * MR_DIM + e * MR_DIM + c] * xh[p * MR_DIM + c]
            }
            want[p * ner + e] = acc
        }
    }
    return <- want
}

//! the router GEMM's rows against the oracle, then one poisoned element the bar reds
def router_gemm_check(t : T?; ner : int; got : array<float>; var want : array<float>) {
    check_rows(t, "router GEMM over {ner} rows against the CPU oracle at the second layer's offset", got, want, Bar(rel = 1e-4), ner)
    want[5 * ner + 3] += 0.5
    check_exceeds(t, "router GEMM control ({ner} rows): one poisoned element", got, want, Bar(rel = 1e-4))
}

//! the per-row select cell's planes: logits rows at the ner stride spread past any near-tie; two layers' router bias rows (the dispatch reads the second at rboff MR_NE, the first a decoy), wide enough to move picks, or zero
def topk_rows_planes(rbias : bool; var lh, rb : array<float>) {
    lh |> resize(MR_NPOS * MR_NER)
    rb |> resize(2 * MR_NE)
    for (i in range(MR_NPOS * MR_NER)) {
        lh[i] = hash_unit(uint(i) * 3u + 101u) * 2.0 + float(i % 7) * 0.0625
    }
    for (e in range(MR_NE)) {
        rb[e] = 7.0
        rb[MR_NE + e] = rbias ? hash_unit(uint(e) * 5u + 7u) * 3.0 - 1.5 : 0.0
    }
}

//! the host select's picks and weights over the rows, `k` a row
struct TopkSelect {
    idx : array<int64>
    w : array<float>
}

//! the host select (`moe_select_core`) a row at a time over `npos` rows of `logits` at `lstride`, each row under `rbias`'s row at `rboff` when `bias_on`
def topk_rows_select(gate : MoeGate; logits, rbias : array<float>; npos, ne, k, lstride, rboff : int; bias_on, norm : bool; wscale : float) : TopkSelect {
    var sel : TopkSelect
    sel.idx |> resize(npos * k)
    sel.w |> resize(npos * k)
    var row : array<float>
    row |> resize(ne)
    for (p in range(npos)) {
        for (e in range(ne)) {
            row[e] = logits[p * lstride + e] + (bias_on ? rbias[rboff + e] : 0.0)
        }
        unsafe {
            moe_select_core(gate, int64(ne), int64(k), norm, wscale, addr(row[0]), addr(sel.idx[0]), addr(sel.w[0]), int64(p * k))
        }
    }
    delete row
    return <- sel
}

//! the device's picks and weights against the host select's: the picks exact (the count and the first differing pick logged, a
//! pick moved by one the compare's control), the weights within 1e-4
def topk_rows_compare(t : T?; tag : string; gidx : array<uint>; gw : array<float>; sel : TopkSelect) {
    var picks <- [for (g in gidx); int64(g)]
    check_exact(t, "{tag}: the picks against the host select", picks, sel.idx)
    check_rows(t, "{tag}: the weights against the host select", gw, sel.w, Bar(rel = 1e-4, poison_of_max = true, poison_abs = 0.5), FLAT_ROWS)
    delete picks
}

//! the wide select cells' planes over `ne` experts: `npos` logits rows at `lstride` spread past any near-tie, past 256 experts each
//! row's largest logit past the first 256 (a select that reaches 256 misses it); two layers' router bias rows (the dispatch reads the
//! second at `ne`, the first a decoy), wide enough to move picks under `rbias`, else zero
def topk_wide_planes(ne, npos, lstride : int; rbias : bool; var lh, rb : array<float>) {
    lh |> resize(npos * lstride)
    rb |> resize(2 * ne)
    for (i in range(npos * lstride)) {
        lh[i] = hash_unit(uint(i) * 3u + 211u) * 2.0 + float(i % 7) * 0.0625
    }
    if (ne > 256) {
        for (p in range(npos)) {
            lh[p * lstride + 300 + 37 * p] += 8.0
        }
    }
    for (e in range(ne)) {
        rb[e] = 7.0
        rb[ne + e] = rbias ? hash_unit(uint(e) * 5u + 7u) * 3.0 - 1.5 : 0.0
    }
}

//! the routed block's gather on the CPU: slot `r`'s position row (`r / k`, `n` wide) at its bucket row `slot_idx[r]` less `r0`
//! wherever that row sits in the window [r0, r1), rounded to half under `f16`; rows the window holds no slot of stay NaN
def moe_gather_oracle(x : array<float>; slot_idx : array<uint>; n, k, r0, r1 : int; f16 : bool) : array<float> {
    var want <- nan_plane((r1 - r0) * n)
    for (r in range(length(slot_idx))) {
        let b = int(slot_idx[r])
        continue if (b < r0 || b >= r1)
        for (i in range(n)) {
            let v = x[(r / k) * n + i]
            want[(b - r0) * n + i] = f16 ? f16r(v) : v
        }
    }
    return <- want
}

//! a per-row select arm's verdict: the device's picks and weights against the host select's, one poisoned weight the bar reds, and under `rbias` the control that the bias moved some pick off the unbiased select
def topk_rows_verdict(t : T?; gidx : array<uint>; gw : array<float>; lh, rb : array<float>; norm : bool; wscale : float; gate : MoeGate; rbias : bool) {
    var sel <- topk_rows_select(gate, lh, rb, MR_NPOS, MR_NE, MR_K, MR_NER, MR_NE, true, norm, wscale)
    let arm = (gate == MoeGate.softmax_weight ? "softmax over the picks" : (norm ? "renormalized" : "scaled by {wscale}")) + (rbias ? ", router bias" : "")
    topk_rows_compare(t, "per-row top-k ({arm})", gidx, gw, sel)
    sel.w[4] += 0.5
    check_exceeds(t, "per-row top-k control: one poisoned weight", gw, sel.w, Bar(rel = 1e-4))
    if (rbias) {
        var sel0 <- topk_rows_select(gate, lh, rb, MR_NPOS, MR_NE, MR_K, MR_NER, MR_NE, false, norm, wscale)
        var moved = 0
        for (i0, i in sel0.idx, sel.idx) {
            if (i0 != i) {
                moved++
            }
        }
        t |> success(moved > 0, "router bias control: the bias moves {moved} of {MR_NPOS * MR_K} picks off the unbiased select")
        delete sel0
    }
    delete sel
}

//! the schedule cell's slot experts over `nk` rows, the ladder's four shapes: expert 0 a whole m column plus an s remainder, 1 and 2 one partial m column each, 3 within the s column; the rest thin, the last three empty
def sched_cell_idx(nk, ne : int) : array<uint> {
    let c0 = int(SCHED_M_ROWS) + int(SCHED_S_ROWS) - 10
    let c1 = int(SCHED_M_ROWS) - 28
    let c2 = int(SCHED_S_ROWS) + 8
    let c3 = int(SCHED_S_ROWS) - 12
    return <- [for (r in range(nk)); r < c0 ? 0u : (r < c0 + c1 ? 1u : (r < c0 + c1 + c2 ? 2u : (r < c0 + c1 + c2 + c3 ? 3u : 4u + hash_word(uint(r) + 55u) % uint(ne - 7))))]
}

//! the two maps' misses: slots whose bucket row sits outside their expert's bucket or repeats one (`inv`), rows naming another
//! expert (`bex`), each with its first slot (-1 where none misses)
struct SchedMapsOff {
    inv : int
    inv_at : int
    bex : int
    bex_at : int
}

def private sched_maps_off(idx, ginv, gbex : array<uint>; bucket_at : block<(e : int) : tuple<start : int; rows : int>>) : SchedMapsOff {
    var o = SchedMapsOff(inv_at = -1, bex_at = -1)
    var seen : array<bool>
    seen |> resize(length(gbex))   // the bucket rows: the slot rows, or past them under a padded schedule (`gbex` holds every bucket row)
    for (r in range(length(idx))) {
        let b = invoke(bucket_at, int(idx[r]))
        let row = ginv[r]
        if (row < uint(b.start) || row >= uint(b.start + b.rows) || seen[int(row)]) {
            o.inv_at = o.inv < 1 ? r : o.inv_at
            o.inv++
        } else {
            seen[int(row)] = true
            if (gbex[int(row)] != idx[r]) {
                o.bex_at = o.bex < 1 ? r : o.bex_at
                o.bex++
            }
        }
    }
    delete seen
    return o
}

//! the schedule's slot -> bucket row map as a permutation of each bucket's rows, and the bucket row -> expert map naming each row's
//! slot's expert; `bucket_at(e)` gives a bucket's (start, rows). The controls: slot 0's row moved past every bucket, and the row it
//! holds naming another expert, each miss counted
def sched_maps_check(t : T?; idx, ginv, gbex : array<uint>; bucket_at : block<(e : int) : tuple<start : int; rows : int>>) {
    let nk = length(idx)
    let o = sched_maps_off(idx, ginv, gbex, bucket_at)
    to_log(LOG_INFO, "the schedule's maps: {o.inv} of {nk} slot rows off (the first at slot {o.inv_at}), {o.bex} row experts off (the first at slot {o.bex_at})\n")
    t |> success(o.inv == 0, "the schedule's slot -> bucket row map is a permutation of each bucket's rows ({nk} slots, {o.inv} off, the first at slot {o.inv_at})")
    t |> success(o.bex == 0, "the schedule's bucket row -> expert map names each row's slot's expert ({o.bex} of {nk} off, the first at slot {o.bex_at})")
    var inv_p := ginv
    inv_p[0] = uint(length(gbex))
    t |> success(sched_maps_off(idx, inv_p, gbex, bucket_at).inv > 0, "schedule maps control: slot 0's row moved past every bucket reds the permutation compare")
    var bex_p := gbex
    let row0 = int(ginv[0])
    if (row0 < length(bex_p)) {
        bex_p[row0] = idx[0] + 1u
    }
    t |> success(sched_maps_off(idx, ginv, bex_p, bucket_at).bex > 0, "schedule maps control: slot 0's row naming another expert reds the expert-map compare")
    delete inv_p
    delete bex_p
}

//! the combine-folded residual cells' fixture rows: a permutation of the `npos` x `k` bucket rows as the slot map, the slot
//! weights, the bucket rows `n` wide, the residual rows and the shared-expert rows
struct ArCombFix {
    sidx : array<uint>
    sw : array<float>
    rows : array<float>
    xs : array<float>
    sh : array<float>
}

def ar_comb_fixture(n, k, npos : int) : ArCombFix {
    let nrows = npos * k
    return <- ArCombFix(sidx <- [for (i in range(nrows)); uint((i * 7) % nrows)],
        sw <- [for (i in range(nrows)); 0.125 + float(i % 5) * 0.25],
        rows <- row_fill(nrows * n, 7, 0, 0.13, 5.5, 93),
        xs <- row_fill(npos * n, 11, 0, 0.1, 1.7, 37),
        sh <- row_fill(npos * n, 5, 0, 0.2, 3.0, 41))
}

//! the combine cells' other planes: the next norm row, the logits rows (the gate logit last), the slot -> expert map over six experts, the down-bias rows (two layers' when biased, the dispatch reading the second; else the driver's one zero row of the model dim)
def ar_comb_planes(n, k, npos, ne : int; dbias : bool; var wn, lg, db : array<float>; var sex : array<uint>) {
    let nrows = npos * k
    wn |> resize(n)
    lg |> resize(npos * MR_NER)
    sex |> resize(nrows)
    let dbn = dbias ? 2 * ne * n : n
    db |> resize(dbn)
    for (i in range(nrows)) {
        sex[i] = hash_word(uint(i) + 3u) % uint(ne)
    }
    for (i in range(dbn)) {
        db[i] = dbias ? (i < ne * n ? 9.0 : float((i * 19) % 23) * 0.25 - 2.75) : 0.0
    }
    for (i in range(n)) {
        wn[i] = 0.5 + float(i % 7) * 0.125
    }
    for (i in range(npos * MR_NER)) {
        lg[i] = hash_unit(uint(i) + 9u)
    }
}

//! the combine-folded residual step's oracle: x += [g *] shared row + the k weighted expert rows through the slot map (each plus its expert's down bias under `dbias`), then the next norm into `want_y`
def ar_comb_oracle(fx : ArCombFix; wn, lg, db : array<float>; sex : array<uint>; n, k, npos, ne : int; gated, with_sh, dbias : bool; eps : float;
                   var want_x, want_y : array<float>) {
    want_x |> resize(npos * n)
    want_y |> resize(npos * n)
    for (p in range(npos)) {
        let g = with_sh && gated ? sigmoid_f32(lg[p * MR_NER + MR_NE]) : 1.0
        for (i in range(n)) {
            var v = fx.xs[p * n + i]
            if (with_sh) {
                v += g * fx.sh[p * n + i]
            }
            for (j in range(k)) {
                v += fx.sw[p * k + j] * (fx.rows[int(fx.sidx[p * k + j]) * n + i] + (dbias ? db[ne * n + int(sex[p * k + j]) * n + i] : 0.0))
            }
            want_x[p * n + i] = v
        }
        unsafe {
            rmsnorm(addr<float?>(want_y[p * n]), addr<float const?>(want_x[p * n]), addr<float const?>(wn[0]), int64(n), eps)
        }
    }
}

//! the combine step's outputs under `tag`: residual rows moved off `xs` and within the approx bar of `want_x`, f32 normed rows of
//! `want_y`, and under `has_f16` the f16 twin `got_h` within four halves' 2^-11 rounding, floored at that fraction of the largest
//! |want_y| - an f16 readback short of the rows (an empty one among them) reds it; under `ctrl` one poisoned element of each reds
//! its bar
def ar_comb_check(t : T?; tag, ctrl : string; xs, got_x, got_y : array<float>; got_h : array<uint>; has_f16 : bool; var want_x, want_y : array<float>; n : int) {
    check_exceeds(t, "{tag}: the in-place residual rows moved off their input", got_x, xs, BAR_APPROX)
    check(t, "{tag}: the residual rows match the CPU oracle", got_x, want_x, BAR_APPROX, Shape(row_width = n))
    check(t, "{tag}: the f32 normed rows match the CPU oracle", got_y, want_y, BAR_APPROX, Shape(row_width = n))
    let f16_bar = Bar(rel = 2.0e-3, abs_floor = 2.0e-3 * logits_maxabs(want_y))
    var got_yf <- [for (i in range(min(length(want_y), length(got_h) * 2))); half_at(got_h, i)]
    if (has_f16) {
        check(t, "{tag}: the f16 normed rows within the half's rounding of the oracle", got_yf, want_y, f16_bar, Shape(row_width = n))
    }
    want_x[2 * n + 9] += 0.5
    check_exceeds(t, "{ctrl}: the residual bar against one poisoned element", got_x, want_x, BAR_APPROX)
    want_y[3 * n + 5] += 0.5
    check_exceeds(t, "{ctrl}: the f32 normed-row bar against one poisoned element", got_y, want_y, BAR_APPROX)
    if (has_f16) {
        check_exceeds(t, "{ctrl}: the f16 normed-row bar against one poisoned element", got_yf, want_y, f16_bar)
    }
    delete got_yf
}
