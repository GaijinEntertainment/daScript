options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_sampling
require dasllama/dasllama_dflash   // dflash_soft_pick, dflash_record_q: the block drafter's sampled drafts and their q
require dasllama/dasllama_kqformat   // KqFmt, kq_qsb: the selector walk's NVFP4 codebook planes
require dasllama/dasllama_convert   // dequant_kq_plane_sb: the codebook rows the walk reads, unscaled
require dasllama/dasllama_gpu_kernels_common   // GK_DFLASH_HS, GK_DFLASH_ATTN_KEYS: the discrete arms' row-kernel stage the shape check reads
require daslib/random
require math
require _mtp_walk_fixture   // bare_session, same_rng
require _compares   // nan_wide_diff

// The non-exact acceptances are host arithmetic over the verify rows. Typical: a draft is accepted
// where the session's sampler admits it on its row - the token survives the cuts the sampler would
// draw from - whatever the row's draw would have been, and the first cut draft (or the row past the
// last draft) is one draw over that row's survivors. Rejection sampling: the drafter draws its draft
// from q, the sampler's chain over the head's row, and the row's p accepts it with min(1, p/q), else
// draws the residual. A bare session over an 8-token vocabulary carries everything the walks touch.

let VOCAB = 8l
let DRAFTS = 3l
let NO_MASS = -1.0e30   //! a fixture row's masked token: no sampler gives it mass

def private set_rows(var s : Session; rows : array<array<float>>; drafts : array<int64>) {
    for (i in range64(DRAFTS + 1l)) {
        for (j in range64(VOCAB)) {
            s.mtp_logits_b[i * VOCAB + j] = rows[i][j]
        }
        s.mtp_vbatch[i] = drafts[i]
    }
}

//! the walk at `seed` over `rows` and `drafts` under `sp`: the accepted count and the parked token
def private typical_walk(rows : array<array<float>>; drafts : array<int64>; var sp : SamplingParams; seed : int) : tuple<a : int64; next : int64> {
    var s <- bare_session(VOCAB, DRAFTS)
    set_rows(s, rows, drafts)
    s.rng = random_seed(seed)
    s.spec_params = unsafe(addr(sp))
    let a = mtp_walk_typical(s, DRAFTS, VOCAB)
    let next = s.mtp_pre_tok
    delete s
    return (a = a, next = next)
}

[test(description = "The typical MTP walk under a 0.9 nucleus accepts a runner-up inside the nucleus and the argmax but not a massless draft at every seed, parks one draw from the cut row's survivors using one random number, and `sample_` returns and clears it")]
def test_typical_walk_accepts_the_nucleus(t : T?) {
    // row 0's draft is the runner-up inside a 0.9 nucleus, row 1's the argmax, row 2's has no mass: two accepted at any seed, the parked token one of row 2's survivors
    var rows <- [[4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [NO_MASS, NO_MASS, NO_MASS, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [3.0, NO_MASS, 3.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let drafts <- [0l, 1l, 3l, 5l]
    var sp = SamplingParams(temp = 1.0, top_p = 0.9)
    for (seed in [1, 7, 42, 1234]) {
        let r = typical_walk(rows, drafts, sp, seed)
        t |> equal(r.a, 2l, "seed {seed}: the runner-up inside the nucleus and the argmax are accepted, the massless draft is not")
        t |> success(r.next == 0l || r.next == 2l, "seed {seed}: the parked token is one of row 2's two survivors ({r.next})")
    }
    var s <- bare_session(VOCAB, DRAFTS)
    set_rows(s, rows, drafts)
    s.rng = random_seed(3)
    s.spec_params = unsafe(addr(sp))
    let rng0 = s.rng
    let a = mtp_walk_typical(s, DRAFTS, VOCAB)
    t |> equal(a, 2l, "two drafts accepted")
    t |> success(s.mtp_pre_drawn, "the first cut row's draw is parked for the caller")
    var probe = random_seed(3)
    let _one = random_float(probe)
    t |> success(same_rng(s.rng, probe) && !same_rng(s.rng, rng0), "one random draw a round: the parked token's")
    t |> equal(length(s.recent), 0, "the recent window is put back for the caller to commit")
    let parked = s.mtp_pre_tok
    t |> equal(sample_(s, sp), parked, "the caller's sample_ returns the parked token")
    t |> success(!s.mtp_pre_drawn, "and clears it")
    delete s
    delete rows
}

[test(description = "The typical walk rejects exactly the drafts the sampler's cuts remove - the third largest under top-k 2, a token under half the max under min-p 0.5, the third under a 0.6 nucleus over two equal leaders - and parks a survivor")]
def test_typical_walk_cuts(t : T?) {
    // each cut rejects a draft the exact walk might draw: top-k 2 drops the third largest, min-p 0.5 a token under half the max, a 0.6 nucleus over two equal leaders the third
    var rows <- [[4.0, 3.5, 3.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 3.5, 3.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 3.5, 3.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    var topk = SamplingParams(temp = 1.0, top_k = 2l)
    let third <- [0l, 2l, 1l, 0l]
    let r_k = typical_walk(rows, third, topk, 9)
    t |> equal(r_k.a, 0l, "top-k 2: the third largest is cut on row 0")
    t |> success(r_k.next == 0l || r_k.next == 1l, "... and the parked draw is one of the two kept ({r_k.next})")
    let second <- [0l, 1l, 1l, 2l]
    let r_k2 = typical_walk(rows, second, topk, 9)
    t |> equal(r_k2.a, 2l, "top-k 2: the runner-up is kept on rows 0 and 1, the third largest cut on row 2")
    var minp = SamplingParams(temp = 1.0, min_p = 0.5)
    var steep <- [[4.0, 3.0, 2.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [4.0, 3.0, 2.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [4.0, 3.0, 2.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let r_m = typical_walk(steep, second, minp, 9)
    t |> equal(r_m.a, 0l, "min-p 0.5: a token at e^-1 of the maximum is cut")
    t |> equal(r_m.next, 0l, "... and the maximum is the only survivor to draw")
    var nucleus = SamplingParams(temp = 1.0, top_p = 0.6)
    var pair <- [[4.0, 4.0, 1.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 4.0, 1.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 4.0, 1.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let r_p = typical_walk(pair, second, nucleus, 9)
    t |> equal(r_p.a, 2l, "a 0.6 nucleus over two equal leaders keeps both: the runner-up rides, the third is cut")
    delete pair
    delete steep
    delete rows
}

[test(description = "In both the exact and the typical accept walk a stop-id draft ends the run with the stop as the last accepted token and no parked draw, while without stop ids every matching draft is accepted")]
def test_walk_cuts_at_a_stop(t : T?) {
    let drafts <- [0l, 1l, 3l, 5l]
    var stops <- [3l]
    var s <- bare_session(VOCAB, DRAFTS)
    for (i in range64(DRAFTS + 1l)) {
        s.mtp_vbatch[i] = drafts[i]
    }
    let a_free = mtp_accept_walk(s, DRAFTS, VOCAB, $(row : int64) : int64 => drafts[row + 1l])
    t |> equal(a_free, DRAFTS, "no stop ids: every matching draft is accepted")
    s.spec_stops = unsafe(addr(stops))
    let a_cut = mtp_accept_walk(s, DRAFTS, VOCAB, $(row : int64) : int64 => drafts[row + 1l])
    t |> equal(a_cut, 2l, "the stop draft (row 1's) ends the run: two accepted, the stop the last")
    t |> success(!s.mtp_pre_drawn, "no token parked past a stop")
    s.spec_stops = null
    delete s
    var rows <- [[NO_MASS, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [NO_MASS, NO_MASS, NO_MASS, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, 4.0, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    var sp = SamplingParams(temp = 1.0, top_p = 0.9)
    var s2 <- bare_session(VOCAB, DRAFTS)
    set_rows(s2, rows, drafts)
    s2.rng = random_seed(5)
    s2.spec_params = unsafe(addr(sp))
    let a2_free = mtp_accept_walk(s2, DRAFTS, VOCAB, $(row : int64) : int64 => drafts[row + 1l])
    t |> equal(a2_free, DRAFTS, "typical, no stop ids: every draft accepted")
    t |> success(s2.mtp_pre_drawn, "... and the row past the last draft drawn and parked")
    s2.mtp_pre_drawn = false
    s2.spec_stops = unsafe(addr(stops))
    let a2_cut = mtp_accept_walk(s2, DRAFTS, VOCAB, $(row : int64) : int64 => drafts[row + 1l])
    t |> equal(a2_cut, 2l, "typical, stop ids: the run ends on the stop draft")
    t |> success(!s2.mtp_pre_drawn, "... and the parked draw is dropped")
    s2.spec_stops = null
    s2.spec_params = null
    delete s2
    delete rows
    delete stops
}

[test(description = "A penalized greedy sampler's typical walk rejects the repeated draft and parks the penalized argmax with no random draw, and the MtpAccept knob makes a sampled walk accept every in-nucleus runner-up where the exact walk accepts fewer on some seed")]
def test_typical_walk_greedy_and_knob(t : T?) {
    // a penalized greedy sampler judges as the exact walk does (the penalized argmax, no draw); the knob routes a sampled walk through the typical judge and is put back
    var s <- bare_session(VOCAB, DRAFTS)
    var rows <- [[0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0],
                 [0.0, 0.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0],
                 [0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                 [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0]]
    let drafts <- [0l, 3l, 3l, 1l]
    set_rows(s, rows, drafts)
    var sp = SamplingParams(penalty = 2.0)
    s.spec_params = unsafe(addr(sp))
    let rng0 = s.rng
    let a = mtp_walk_typical(s, DRAFTS, VOCAB)
    t |> equal(a, 1l, "the repeated draft is rejected under the penalty")
    t |> equal(s.mtp_pre_tok, 2l, "the penalized argmax is the next token")
    t |> success(same_rng(s.rng, rng0), "a greedy sampler draws no random number")
    let knob = get_mtp_accept()
    set_mtp_accept(MtpAccept.typical)
    var s2 <- bare_session(VOCAB, DRAFTS)
    var wide <- [[4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let runners <- [0l, 1l, 1l, 1l]
    set_rows(s2, wide, runners)
    var sampled = SamplingParams(temp = 1.0, top_p = 0.95)
    s2.rng = random_seed(11)
    s2.spec_params = unsafe(addr(sampled))
    t |> equal(mtp_walk_by_accept(s2, DRAFTS, VOCAB), 3l, "under the knob the walk accepts every runner-up inside the nucleus")
    set_mtp_accept(MtpAccept.exact)
    // the knob off: the exact walk accepts a runner-up only where its draw lands on it (p 0.475 a row), so over twenty
    // seeds some walk accepts fewer than the three the typical judge accepts at every seed
    var fewer = 0
    for (seed in range(1, 21)) {
        var s3 <- bare_session(VOCAB, DRAFTS)
        set_rows(s3, wide, runners)
        s3.rng = random_seed(seed)
        s3.spec_params = unsafe(addr(sampled))
        let exact = mtp_walk_by_accept(s3, DRAFTS, VOCAB)
        to_log(LOG_INFO, "seed {seed}: the exact walk accepted {exact} of 3\n")
        fewer += exact < 3l ? 1 : 0
        delete s3
    }
    t |> success(fewer > 0, "the knob off: the exact walk accepts fewer than three on some seed ({fewer} of 20 seeds)")
    set_mtp_accept(knob)
    delete s2
    delete wide
    delete rows
    delete s
}

//! the drafter's draw off `head` under `sp` at `seed`, recorded as draft row `qi`; the draft and the q entries recorded
def private draft_draw(var s : Session; head : array<float>; var sp : SamplingParams; seed : int; qi : int64) : tuple<d : int64; qn : int64> {
    var row <- [for (x in head); x]
    s.rng = random_seed(seed)
    s.spec_params = unsafe(addr(sp))
    let d = mtp_draft_sample(s, row, VOCAB, qi)
    delete row
    return (d = d, qn = s.mtp_qn[qi])
}

[test(description = "`mtp_draft_sample` records the sampler's distribution over the head row as q - half each to two equal leaders under a 0.6 nucleus within 1e-5, a point mass for greedy, the two kept under top-k 2 - and draws the draft from it")]
def test_rejection_drafter_records_q(t : T?) {
    // the head's row through the sampler is the recorded q: two equal leaders under a 0.6 nucleus hold half each, the third none; a greedy sampler records a point mass
    var s <- bare_session(VOCAB, DRAFTS)
    var head <- [4.0, 4.0, 1.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS]
    var sp = SamplingParams(temp = 1.0, top_p = 0.6)
    let r = draft_draw(s, head, sp, 5, 0l)
    t |> equal(r.qn, 2l, "q holds the two survivors")
    let q0 = mtp_q_of(s, 0l, 0l)
    let q1 = mtp_q_of(s, 0l, 1l)
    t |> success(abs(q0 - 0.5) < 1.0e-5 && abs(q1 - 0.5) < 1.0e-5, "... at half each ({q0}, {q1})")
    t |> success(!(abs(q0 + 0.001 - 0.5) < 1.0e-5), "control: a q a thousandth off reds the bar")
    t |> equal(mtp_q_of(s, 0l, 2l), 0.0, "the cut token holds nothing")
    t |> success(r.d == 0l || r.d == 1l, "the draft is drawn from q ({r.d})")
    var greedy = SamplingParams()
    let g = draft_draw(s, head, greedy, 5, 1l)
    t |> equal(g.d, 0l, "a greedy sampler drafts the argmax")
    t |> equal(g.qn, 1l, "... recorded as a point mass")
    t |> equal(mtp_q_of(s, 1l, 0l), 1.0, "... of one")
    var topk = SamplingParams(temp = 1.0, top_k = 2l)
    let k = draft_draw(s, head, topk, 5, 2l)
    t |> equal(k.qn, 2l, "top-k 2 records the two kept")
    t |> success(k.d == 0l || k.d == 1l, "... and drafts one of them ({k.d})")
    delete head
    delete s
}

//! the rejection walk over `rows` with `drafts` whose q rows are recorded first off `heads` under the drafter's `draft_sp`, the walk
//! under `sp`: the accepted count and the parked token
def private rejection_walk(rows, heads : array<array<float>>; drafts : array<int64>; var draft_sp, sp : SamplingParams; seed : int) : tuple<a : int64; next : int64> {
    var s <- bare_session(VOCAB, DRAFTS)
    s.spec_params = unsafe(addr(draft_sp))
    s.rng = seeded_rng(seed)
    for (i in range64(DRAFTS)) {
        var row <- [for (x in heads[i]); x]
        mtp_draft_sample(s, row, VOCAB, i)
        delete row
    }
    s.spec_params = unsafe(addr(sp))
    set_rows(s, rows, drafts)
    let a = mtp_walk_rejection(s, DRAFTS, VOCAB)
    let next = s.mtp_pre_tok
    delete s
    return (a = a, next = next)
}

//! the same with one sampler drafting and walking
def private rejection_walk(rows, heads : array<array<float>>; drafts : array<int64>; var sp : SamplingParams; seed : int) : tuple<a : int64; next : int64> => rejection_walk(rows, heads, drafts, sp, sp, seed)

[test(description = "The rejection walk accepts a draft whose row gives it at least q's mass and rejects one the row gives none at every seed, its residual draw landing on a survivor of the rejecting row and never on the rejected draft")]
def test_rejection_walk_accepts_by_p_over_q(t : T?) {
    // row 0 gives draft 0 at least q's mass (always accepted), row 1 gives draft 1 none (always rejected): one accepted at any seed, the parked token a row-1 survivor
    var heads <- [[4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS]]
    var rows <- [[6.0, 1.0, 1.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [NO_MASS, NO_MASS, 3.0, 3.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let drafts <- [0l, 0l, 1l, 0l]
    var sp = SamplingParams(temp = 1.0, top_p = 0.99)
    for (seed in [1, 2, 3, 4, 5, 6, 7, 8]) {
        let r = rejection_walk(rows, heads, drafts, sp, seed)
        t |> equal(r.a, 1l, "seed {seed}: draft 0 accepted (p >= q), draft 1 rejected (p = 0)")
        t |> success(r.next == 2l || r.next == 3l, "seed {seed}: the residual draw lands on a survivor of row 1 ({r.next}), never the rejected draft")
    }
    delete heads
    delete rows
}

//! the rejection walk over `rows` with every draft 0 on `heads` at 400 seeds, the drafter under `draft_sp` and the walk under
//! `sp`: the seeds that accepted the first draft, and the rejects whose residual landed on token 1
def private accept_census(rows, heads : array<array<float>>; var draft_sp, sp : SamplingParams) : tuple<accepted : int; residual_on_1 : int> {
    let drafts <- [0l, 0l, 0l, 0l]
    var accepted = 0
    var residual_on_1 = 0
    for (seed in range(1, 401)) {
        let r = rejection_walk(rows, heads, drafts, draft_sp, sp, seed)
        if (r.a >= 1l) {
            accepted++
        } elif (r.next == 1l) {
            residual_on_1++
        }
    }
    return (accepted = accepted, residual_on_1 = residual_on_1)
}

def private accept_census(rows, heads : array<array<float>>; var sp : SamplingParams) : tuple<accepted : int; residual_on_1 : int> => accept_census(rows, heads, sp, sp)

//! the heads whose q gives draft 0 half, and the rows giving it a quarter (row 0) then a point mass (rows 1, 2)
def private half_q_heads() : array<array<float>> => <- [[4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                       [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                       [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS]]

def private quarter_p_rows() : array<array<float>> => <- [[0.0, log(3.0), NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                         [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                         [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                         [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]

//! the rows giving draft 0 the whole mass on row 0: the drafter's q and the row's p agree, so every seed accepts
def private whole_p_rows() : array<array<float>> => <- [[4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                       [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                       [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                                                       [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]

[test(description = "Under top-k 4 the rejection walk accepts a draft at p = q/2 on 160 to 240 of 400 seeds with every reject's residual on the underweighted token, while the p = q control accepts all 400")]
def test_rejection_walk_top_k_path(t : T?) {
    // under a top-k within the candidate cap the judge and the residual run over the candidate list: q gives draft 0 half and the
    // row a quarter, so about half the seeds accept and every reject's residual lands on the token q underweights, never the draft
    var heads <- half_q_heads()
    var rows <- quarter_p_rows()
    var sp = SamplingParams(temp = 1.0, top_k = 4l)
    let half = accept_census(rows, heads, sp)
    t |> success(half.accepted >= 160 && half.accepted <= 240, "top-k 4: draft 0 at p = q / 2 is accepted on about half of 400 seeds ({half.accepted})")
    t |> equal(half.residual_on_1, 400 - half.accepted, "top-k 4: every reject's residual draw lands on token 1, never the rejected draft")
    var same <- whole_p_rows()
    let full = accept_census(same, heads, sp)
    t |> equal(full.accepted, 400, "control: at p = q every one of the 400 seeds accepts draft 0 under top-k 4")
    delete same
    delete heads
    delete rows
}

[test(description = "With a greedy drafter's point-mass q the rejection walk accepts a draft at p = 1/4 on 70 to 130 of 400 seeds, every reject's residual on the token q gave nothing, while p = 1 accepts all 400")]
def test_rejection_walk_point_mass_q(t : T?) {
    // a greedy drafter records a point mass on the head's argmax, so the row's p alone accepts: a quarter of the mass on the
    // draft accepts about a quarter of the seeds, the whole mass every seed
    var heads <- whole_p_rows()
    heads |> resize(3)
    var rows <- quarter_p_rows()
    var greedy = SamplingParams()
    var sp = SamplingParams(temp = 1.0, top_p = 0.99)
    let quarter = accept_census(rows, heads, greedy, sp)
    t |> success(quarter.accepted >= 70 && quarter.accepted <= 130, "a point-mass q against p = 1/4 accepts about a quarter of 400 seeds ({quarter.accepted})")
    t |> equal(quarter.residual_on_1, 400 - quarter.accepted, "every reject's residual draw lands on token 1, the one q gave nothing")
    var same <- whole_p_rows()
    let full = accept_census(same, heads, greedy, sp)
    t |> equal(full.accepted, 400, "control: a point-mass q against p = 1 accepts every one of the 400 seeds - outside the quarter bound")
    delete same
    delete heads
    delete rows
}

[test(description = "Under a 0.99 nucleus the rejection walk accepts a draft at p = q/2 on 160 to 240 of 400 seeds with every reject's residual on the token q gave nothing, while the p = q control accepts all 400")]
def test_rejection_walk_accepts_at_half_q_half_the_time(t : T?) {
    // q gives draft 0 half, the row a quarter: accepted on about half the seeds, and a reject's residual lands on the token q underweights
    var heads <- [[4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                  [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS]]
    var rows <- [[0.0, log(3.0), NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    var sp = SamplingParams(temp = 1.0, top_p = 0.99)
    let half = accept_census(rows, heads, sp)
    t |> success(half.accepted >= 160 && half.accepted <= 240, "draft 0 at p = q / 2 is accepted on about half of 400 seeds ({half.accepted})")
    t |> equal(half.residual_on_1, 400 - half.accepted, "every reject's residual draw lands on token 1, the one q gave nothing")
    // the control: row 0 as the head's own row gives draft 0 p = q, so every seed accepts and the count lands past the bound
    var same <- [[4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let full = accept_census(same, heads, sp)
    t |> equal(full.accepted, 400, "control: at p = q every one of the 400 seeds accepts draft 0 - outside the half-the-time bound")
    delete same
    delete heads
    delete rows
}

[test(description = "Under the rejection knob a round whose drafts recorded no q walks typical, accepting every in-nucleus runner-up, and `mtp_round_begin` clears a previous round's q record so a stale one is never read")]
def test_rejection_without_q_records_walks_typical(t : T?) {
    // the assistant drafter's device picks record no q: under the rejection knob the walk judges them typical, never reading past `mtp_qn`
    let knob = get_mtp_accept()
    set_mtp_accept(MtpAccept.rejection)
    var s <- bare_session(VOCAB, DRAFTS)
    var wide <- [[4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [4.0, 3.9, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    let runners <- [0l, 1l, 1l, 1l]
    set_rows(s, wide, runners)
    var sampled = SamplingParams(temp = 1.0, top_p = 0.95)
    s.rng = random_seed(11)
    s.spec_params = unsafe(addr(sampled))
    t |> equal(long_length(s.mtp_qn), 0l, "no draft row recorded a q")
    t |> equal(mtp_walk_by_accept(s, DRAFTS, VOCAB), 3l, "the walk takes the typical acceptance: every runner-up inside the nucleus accepted")
    // the q records are a round's: a round that records none after one that did walks typical too, never off the stale record
    let _parked = sample_(s, sampled)
    var head <- [4.0, 4.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS]
    let _d = mtp_draft_sample(s, head, VOCAB, 0l)
    t |> equal(long_length(s.mtp_qn), 1l, "a draft recorded its q")
    mtp_round_begin(s)
    t |> equal(long_length(s.mtp_qn), 0l, "the next round's begin cleared the record")
    set_rows(s, wide, runners)
    s.rng = random_seed(11)
    t |> equal(mtp_walk_by_accept(s, DRAFTS, VOCAB), 3l, "... so the q-less round walks typical again")
    let _parked2 = sample_(s, sampled)
    set_mtp_accept(knob)
    delete head
    delete wide
    delete s
}

[test(description = "`mtp_drafts_sampled` is false under exact acceptance, true under rejection even at head temperature 0, and false for a greedy stream under rejection")]
def test_nextn_drafts_sampled_by_mode(t : T?) {
    let knob = get_mtp_accept()
    let temp0 = get_mtp_draft_temp()
    var s <- bare_session(VOCAB, DRAFTS)
    var sampled = SamplingParams(temp = 0.7)
    var greedy = SamplingParams()
    s.spec_params = unsafe(addr(sampled))
    set_mtp_accept(MtpAccept.exact)
    t |> success(!mtp_drafts_sampled(s), "exact: the head drafts its argmax, no q")
    set_mtp_accept(MtpAccept.rejection)
    t |> success(mtp_drafts_sampled(s), "rejection: the head drafts through the draw seam")
    set_mtp_draft_temp(0.0)
    t |> success(mtp_drafts_sampled(s), "rejection at head temperature 0: still the draw seam, so the point mass is recorded")
    s.spec_params = unsafe(addr(greedy))
    t |> success(!mtp_drafts_sampled(s), "control: a greedy stream under rejection drafts no q")
    s.spec_params = null
    set_mtp_draft_temp(temp0)
    set_mtp_accept(knob)
    delete s
}

[test(description = "The NextN head's draft temperature sets q: 0 drafts the argmax as a point mass with no draw, a negative value records the stream's softmax, and 0.5 records the head's softmax at 0.5 within 1e-6 and draws the closed-form cdf pick")]
def test_nextn_draft_temp_records_q(t : T?) {
    let temp0 = get_mtp_draft_temp()
    var s <- bare_session(VOCAB, DRAFTS)
    var head <- [2.0, 1.0, 0.0, NO_MASS, NO_MASS, NO_MASS, NO_MASS, NO_MASS]
    var sp = SamplingParams(temp = 1.0)
    set_mtp_draft_temp(0.0)
    var row <- [for (x in head); x]
    s.rng = random_seed(5)
    s.spec_params = unsafe(addr(sp))
    let rng0 = s.rng
    let d0 = mtp_draft_sample(s, row, VOCAB, 0l)
    t |> equal(d0, 0l, "temperature 0: the head's argmax")
    t |> success(s.mtp_qn[0] == 1l && mtp_q_of(s, 0l, 0l) == 1.0, "... recorded as a point mass")
    t |> success(same_rng(s.rng, rng0), "... with no draw")
    set_mtp_draft_temp(-1.0)
    let own = draft_draw(s, head, sp, 5, 1l)
    t |> success(own.qn > 1l && abs(mtp_q_of(s, 1l, 0l) - soft_ref(head, 1.0)[0]) < 1.0e-6,
        "control: below 0 the stream's temperature records its softmax, no point mass ({own.qn} entries, q(0) {mtp_q_of(s, 1l, 0l)})")
    set_mtp_draft_temp(0.5)
    var q_ref <- soft_ref(head, 0.5)
    for (seed in range(1, 51)) {
        let r = draft_draw(s, head, sp, seed, 2l)
        var q_off = 0.0
        for (k in range64(VOCAB)) {
            q_off = max(q_off, nan_wide_diff(mtp_q_of(s, 2l, k), q_ref[k]))
        }
        t |> success(r.qn > 1l && q_off < 1.0e-6, "seed {seed}: q is the head's softmax at 0.5 (max off {q_off})")
        var probe = random_seed(seed)
        t |> equal(r.d, cdf_at(q_ref, random_float(probe) * sum_of(q_ref)), "seed {seed}: the draft is the closed form's draw")
    }
    var q_one <- soft_ref(head, 1.0)
    t |> success(abs(q_one[0] - q_ref[0]) > 1.0e-3, "control: the stream's temperature gives another q ({q_one[0]} against {q_ref[0]})")
    s.spec_params = null
    set_mtp_draft_temp(temp0)
    delete q_one
    delete q_ref
    delete row
    delete head
    delete s
}

[test(description = "At head temperature 0 the rejection walk at p = 1/4 accepts 70 to 130 of 400 seeds with every residual on token 1, while the stream-temperature q control accepts 160 to 240, outside the quarter bound")]
def test_nextn_point_mass_rejection_rate(t : T?) {
    let temp0 = get_mtp_draft_temp()
    var heads <- half_q_heads()
    var rows <- quarter_p_rows()
    var sp = SamplingParams(temp = 1.0, top_p = 0.99)
    set_mtp_draft_temp(0.0)
    let point = accept_census(rows, heads, sp)
    t |> success(point.accepted >= 70 && point.accepted <= 130, "head temperature 0: p = 1/4 accepts about a quarter of 400 seeds ({point.accepted})")
    t |> equal(point.residual_on_1, 400 - point.accepted, "every reject's residual draw lands on token 1")
    set_mtp_draft_temp(-1.0)
    let own = accept_census(rows, heads, sp)
    t |> success(own.accepted >= 160 && own.accepted <= 240, "control: the stream's q at p = q / 2 accepts about half ({own.accepted}), outside the quarter bound")
    set_mtp_draft_temp(temp0)
    delete heads
    delete rows
}

let DF_EMPTY = -3.4e38   //! an empty candidate slot's score, as `dflash_top_k_row` fills a slot past the row's ids
let DF_IDS <- [5l, 2l, 7l, -1l]   //! the toy position's candidate ids, an empty slot last
let DF_SCORES <- [1.0, 2.0, 0.0, DF_EMPTY]

//! the softmax of `v` at `temp` over its entries above DF_EMPTY, the others 0
def private soft_ref(v : array<float>; temp : float) : array<float> {
    var mx = DF_EMPTY
    for (x in v) {
        mx = max(mx, x)
    }
    var q <- [for (x in v); x > DF_EMPTY ? exp((x - mx) / temp) : 0.0]
    var sum = 0.0
    for (x in q) {
        sum += x
    }
    for (x in q) {
        x /= sum
    }
    return <- q
}

//! the index a cdf walk over `w` in index order lands at for the draw `r` (already scaled to the walk's total): the first it
//! crosses, else the last positive
def private cdf_at(w : array<float>; r : float) : int64 {
    var cdf = 0.0
    var last = 0l
    for (x, i in w, count()) {
        continue if (x <= 0.0)
        last = int64(i)
        cdf += x
        return int64(i) if (r < cdf)
    }
    return last
}

def private sum_of(w : array<float>) : float {
    var total = 0.0
    for (x in w) {
        total += x
    }
    return total
}

[test(description = "`dflash_soft_pick` records the candidate scores' softmax within 1e-6 at temperatures 1 and 0.5, picks the closed-form cdf draw with one random draw and never the empty slot, and at temperature 0 picks the first best of a tie as a point mass")]
def test_dflash_soft_pick_closed_form(t : T?) {
    var q_ref <- soft_ref(DF_SCORES, 1.0)
    var q : array<float>
    q |> resize(4)
    var empty_picks = 0
    for (seed in range(1, 201)) {
        var rng = seeded_rng(seed)
        var probe = seeded_rng(seed)
        let j = dflash_soft_pick(DF_SCORES, 4l, 1.0, rng, q, 0l)
        var q_off = 0.0
        for (k in range(4)) {
            q_off = max(q_off, nan_wide_diff(q[k], q_ref[k]))
        }
        t |> success(q_off < 1.0e-6, "seed {seed}: q is the scores' softmax at 1 (max off {q_off})")
        t |> equal(j, cdf_at(q_ref, random_float(probe)), "seed {seed}: the pick is the closed form's draw")
        t |> success(same_rng(rng, probe), "seed {seed}: one draw a pick")
        var again = seeded_rng(seed)
        t |> equal(dflash_soft_pick(DF_SCORES, 4l, 1.0, again, q, 0l), j, "seed {seed}: the same seed picks again")
        empty_picks += j == 3l ? 1 : 0
    }
    t |> equal(empty_picks, 0, "the empty slot is never picked")
    var hot <- soft_ref(DF_SCORES, 0.5)
    t |> success(abs(hot[1] - q_ref[1]) > 1.0e-3, "control: the temperature moves q ({hot[1]} at 0.5 against {q_ref[1]} at 1)")
    for (seed in range(1, 21)) {
        var rng = seeded_rng(seed)
        var probe = seeded_rng(seed)
        let j = dflash_soft_pick(DF_SCORES, 4l, 0.5, rng, q, 0l)
        var q_off = 0.0
        for (k in range(4)) {
            q_off = max(q_off, nan_wide_diff(q[k], hot[k]))
        }
        t |> success(q_off < 1.0e-6, "seed {seed}: q is the scores' softmax at 0.5 (max off {q_off})")
        t |> equal(j, cdf_at(hot, random_float(probe)), "seed {seed}: the pick at 0.5 is the closed form's draw")
    }
    let tied <- [1.0, 3.0, 3.0, DF_EMPTY]
    var rng = seeded_rng(9)
    let rng0 = rng
    t |> equal(dflash_soft_pick(tied, 4l, 0.0, rng, q, 0l), 1l, "temperature 0: the first best of a tie")
    t |> success(q[1] == 1.0 && q[0] == 0.0 && q[2] == 0.0 && q[3] == 0.0, "... recorded as a point mass")
    t |> success(same_rng(rng, rng0), "... with no draw")
    delete hot
    delete q
    delete q_ref
}

[test(description = "`dflash_record_q` records the three non-empty candidates id-ascending, each at its own softmax mass within 1e-7, and a greedy draft as a point mass of one")]
def test_dflash_record_q(t : T?) {
    var s <- bare_session(VOCAB, DRAFTS)
    var q <- soft_ref(DF_SCORES, 1.0)
    dflash_record_q(s, 0l, DF_IDS, 0l, q, 0l, 4l)
    t |> equal(s.mtp_qn[0], 3l, "three candidates recorded, the empty slot not")
    t |> success(s.mtp_q[0].id == 2l && s.mtp_q[1].id == 5l && s.mtp_q[2].id == 7l, "id ascending")
    t |> success(abs(mtp_q_of(s, 0l, 5l) - q[0]) < 1.0e-7 && abs(mtp_q_of(s, 0l, 2l) - q[1]) < 1.0e-7 && abs(mtp_q_of(s, 0l, 7l) - q[2]) < 1.0e-7,
        "every id at its own mass")
    let one <- [1.0]
    dflash_record_q(s, 1l, DF_IDS, 1l, one, 0l, 1l)
    t |> success(s.mtp_qn[1] == 1l && mtp_q_of(s, 1l, 2l) == 1.0, "a greedy draft recorded as a point mass")
    delete q
    delete s
}

//! the rejection walk of one draft (id 2) against `rows`, the draft's q recorded off the toy position's scores at temperature 1,
//! the rng seeded at `seed`: the accepted count and the parked token
def private df_residual_walk(rows : array<array<float>>; seed : int) : tuple<a : int64; next : int64> {
    var s <- bare_session(VOCAB, 1l)
    var q <- soft_ref(DF_SCORES, 1.0)
    dflash_record_q(s, 0l, DF_IDS, 0l, q, 0l, 4l)
    for (i in range64(2l)) {
        for (j in range64(VOCAB)) {
            s.mtp_logits_b[i * VOCAB + j] = rows[i][j]
        }
    }
    s.mtp_vbatch[0] = 0l
    s.mtp_vbatch[1] = 2l
    var sp = SamplingParams(temp = 1.0)
    s.spec_params = unsafe(addr(sp))
    s.rng = seeded_rng(seed)
    let a = mtp_walk_rejection(s, 1l, VOCAB)
    let next = s.mtp_pre_tok
    s.spec_params = null
    delete q
    delete s
    return (a = a, next = next)
}

[test(description = "Over 400 seeds the rejection walk of a DFlash draft matches the closed form seed by seed - accept where u times q is under p, else the residual cdf draw off tokens q covers - with the rate within 0.08 of p/q, where a point-mass reading misreads")]
def test_dflash_residual_closed_form(t : T?) {
    var rows <- [[0.0, 0.0, log(2.0), 0.0, 0.0, log(3.0), 0.0, NO_MASS],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    var p0 <- [0.1, 0.1, 0.2, 0.1, 0.1, 0.3, 0.1, 0.0]
    var p1 <- soft_ref(rows[1], 1.0)
    let qs <- soft_ref(DF_SCORES, 1.0)
    var q_of_id <- [0.0, 0.0, qs[1], 0.0, 0.0, qs[0], 0.0, qs[2]]   //! the candidates' masses by id (DF_IDS' slots 1, 0, 2)
    var residual <- [for (i in range(8)); max(0.0, p0[i] - q_of_id[i])]
    let total_res = sum_of(residual)
    var accepts = 0
    var point_mass_misreads = 0
    for (seed in range(1, 401)) {
        let r = df_residual_walk(rows, seed)
        var probe = seeded_rng(seed)
        let u1 = random_float(probe)
        let u2 = random_float(probe)
        let accept = u1 * q_of_id[2] < p0[2]
        point_mass_misreads += (u1 < p0[2]) != (r.a == 1l) ? 1 : 0
        if (accept) {
            accepts++
            t |> equal(r.a, 1l, "seed {seed}: u {u1} under p / q accepts the draft")
            t |> equal(r.next, cdf_at(p1, u2 * sum_of(p1)), "seed {seed}: the row past the draft is the closed form's draw")
        } else {
            t |> equal(r.a, 0l, "seed {seed}: u {u1} at or past p / q rejects the draft")
            t |> equal(r.next, cdf_at(residual, u2 * total_res), "seed {seed}: the residual draw is the closed form's")
            t |> success(r.next != 2l && residual[r.next] > 0.0, "seed {seed}: the residual never lands on the draft or a token q covers")
        }
        let again = df_residual_walk(rows, seed)
        t |> success(again.a == r.a && again.next == r.next, "seed {seed}: the same seed walks the same verdict and token")
    }
    let rate = float(accepts) / 400.0
    let closed = p0[2] / q_of_id[2]
    t |> success(abs(rate - closed) <= 0.08, "the accept rate {rate} sits at min(1, p / q) = {closed}")
    t |> success(point_mass_misreads > 0, "control: the point-mass closed form (accept where u < p) misreads {point_mass_misreads} of 400 seeds")
    delete residual
    delete q_of_id
    delete p1
    delete p0
    delete rows
}

//! a trunk with no NextN head still carries the verify's scratch: a block or assistant drafter's host-judged round embeds
//! its verify rows into mtp_cat and reads row 0's logits and the row ids off the session (the mlx-community 27B snapshot
//! ships no head; the drawn-drafts round indexed past an empty mtp_cat there)
[test(description = "A session over a synthetic trunk with no NextN head still sizes the verify scratch - mtp_cat for MTP_MAX_ROWS rows, the row ids and row 0's logits - while the NextN handoff mtp_h stays empty")]
def test_verify_scratch_sized_on_a_headless_trunk(t : T?) {
    let c = Config(dim = 64l, hidden_dim = 128l, n_layers = 2l, n_heads = 2l, n_kv_heads = 2l, vocab_size = 100l,
        seq_len = 16l, head_size = 32l, kv_dim = 64l, rope_freq_base = 10000.0)
    t |> equal(c.n_layer_nextn, 0l, "the synthetic trunk has no NextN head")
    var s <- make_run_state(c)
    t |> success(long_length(s.mtp_cat) >= 2l * MTP_MAX_ROWS * c.dim, "mtp_cat holds {MTP_MAX_ROWS} verify rows' embeds ({length(s.mtp_cat)} floats)")
    t |> equal(long_length(s.mtp_vbatch), MTP_MAX_ROWS, "the verify's row ids are sized to the row cap")
    t |> equal(long_length(s.mtp_logits), c.vocab_size, "row 0's logits are sized to the vocabulary")
    t |> equal(length(s.mtp_h), 0, "control: the NextN handoff stays unsized without a head")
    delete s
}

//! byte `i` of a fixture plane: a multiplicative hash's top byte
def private plane_byte(i : int64; salt : uint) : uint8 => uint8(((uint(i) + salt) * 2654435761u) >> 24u)

[test(description = "The block drafter's selector walk reads an NVFP4 codebook's rows under the tensor's `.scale`: over two candidates whose logits favour the first by half of what a scale of 2 on both codebooks adds to the second, the walk picks the first with no scale and with a scale of 1, and the second with the scale of 2")]
def test_dflash_walk_codebook_scale(t : T?) {
    let rank = KQ_SB_ELEMS
    let ntok = 3l
    let nv4 = int(KqFmt.nv4)
    var dr <- DflashDrafter(rank = rank, topk = 2l, vocab = ntok)
    dr.kq[nv4].q |> resize(2l * ntok * kq_qsb(KqFmt.nv4))
    dr.kq[nv4].s |> resize(2l * ntok * kq_ssb(KqFmt.nv4))
    for (i in range64(long_length(dr.kq[nv4].q))) {
        dr.kq[nv4].q[i] = plane_byte(i, 17u)
    }
    for (i in range64(long_length(dr.kq[nv4].s))) {
        dr.kq[nv4].s[i] = uint8((uint(plane_byte(i, 91u)) & 0x0Fu) | 0x30u)   // a narrow band of UE4M3 exponents: the rows stay comparable
    }
    // the unscaled bilinear term of the anchor's predecessor row against each candidate's successor row, the selector row all ones
    var prev, next : array<float>
    prev |> resize(rank)
    next |> resize(rank)
    dequant_kq_plane_sb(KqFmt.nv4, dr.kq[nv4].q, dr.kq[nv4].s, 0l, false, prev, 0l)
    var term : float[2]
    for (j in range(2)) {
        dequant_kq_plane_sb(KqFmt.nv4, dr.kq[nv4].q, dr.kq[nv4].s, ntok + 1l + int64(j), false, next, 0l)
        for (i in range64(rank)) {
            term[j] += prev[i] * next[i]
        }
    }
    let hi = term[1] > term[0] ? 1 : 0   // the candidate the bilinear term favours sits second among the scores
    let delta = abs(term[1] - term[0])
    t |> success(delta > 1.0e-3, "the fixture's two candidates differ in their bilinear term ({term[0]} and {term[1]})")
    var cids <- [int64(2 - hi), int64(1 + hi)]
    var cvals <- [0.0, -2.0 * delta]   // the first leads by delta unscaled, trails by 2 delta where the term counts four times
    var hsel <- [for (_i in range64(rank)); 1.0]
    var picks : array<int64>
    picks |> resize(2)
    for (c in fixed_array((scale = 0.0, want = 0), (scale = 1.0, want = 0), (scale = 2.0, want = 1))) {
        dr.sel_prev = DflashW(fmt = KqFmt.nv4, off = 0l, scale = c.scale)
        dr.sel_next = DflashW(fmt = KqFmt.nv4, off = ntok * rank, scale = c.scale)
        dflash_walk(dr, 0l, 1l, cids, cvals, hsel, picks)
        t |> equal(picks[1], cids[c.want], "codebooks under a scale of {c.scale}: the walk picks candidate {c.want}")
    }
    delete dr
    delete prev
    delete next
    delete cids
    delete cvals
    delete hsel
    delete picks
}

//! a drafter header the discrete arms' row kernels take: two heads of 128 over one K/V head, 256 wide, a 512-wide FFN, a
//! block of 16 under a window of 2048, no bias rows
def private shape_drafter() : DflashDrafter =>
    DflashDrafter(dim = 256l, hidden = 512l, n_heads = 2l, n_kv = 1l, hs = 128l, nblock = 16l, window = 2048l)

[test(description = "`dflash_device_shape_why` takes a drafter the row kernels stage - a head of 256, a window and block filling the staged keys exactly - and names, by its own clause, bias rows, a head past 256 or odd, a window and block past the staged keys, a block past the planes' rows, and each row width off the Q8_0 block")]
def test_dflash_device_shape_why(t : T?) {
    let hs_max = int64(GK_DFLASH_HS)
    let keys = int64(GK_DFLASH_ATTN_KEYS)
    let rows = DFLASH_DEVICE_ROWS
    var dr <- shape_drafter()
    t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "", "the fixture's shape is taken")
    dr.hs = hs_max
    t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "", "a head of {hs_max} - the stage's width - is taken")
    dr.hs = 128l
    dr.window = keys - dr.nblock
    t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "", "a window and block filling the {keys} staged keys exactly are taken")
    dr.window = 2048l
    dr.biased = true
    t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "the drafter's attention planes carry bias rows", "bias rows are refused")
    dr.biased = false
    for (hs in fixed_array(hs_max + 2l, 127l)) {
        dr.hs = hs
        t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "a head of {hs} is past the row kernels' {hs_max}", "a head of {hs} is refused")
    }
    dr.hs = 128l
    dr.window = keys - dr.nblock + 1l
    t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows),
        "a window of {dr.window} and a block of 16 pass the attention's {keys} staged keys", "one key past the stage is refused")
    dr.window = 2048l
    dr.nblock = rows + 1l
    t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "a block of {rows + 1l} rows passes the planes' {rows}", "a block one row past the planes is refused")
    dr.nblock = 16l
    //! (dim, hidden, n_heads, n_kv, hs): the model width, the FFN width, the query width (3 x 48) and the K/V width (1 x 48) each
    //! off the 32-element block with every other width on it
    for (c in fixed_array((264l, 512l, 2l, 1l, 128l), (256l, 528l, 2l, 1l, 128l), (256l, 512l, 3l, 2l, 48l), (256l, 512l, 2l, 1l, 48l))) {
        dr.dim = c._0
        dr.hidden = c._1
        dr.n_heads = c._2
        dr.n_kv = c._3
        dr.hs = c._4
        t |> equal(dflash_device_shape_why(dr, hs_max, keys, rows), "a row width off the Q8_0 block",
            "dim {c._0}, hidden {c._1}, {c._2} x {c._4} queries, {c._3} x {c._4} keys: a width off the block is refused")
    }
    delete dr
}

//! a per-layer site list of two layers: the second carries `one` where `site` is this list's `own`, every other plane `none` - a
//! walk reading only layer 0 misses the scale
def private layer_sites(site, own : int; one, none : DflashW) : array<DflashW> => <- [for (l in range(2)); site == own && l == 1 ? one : none]

[test(description = "`dflash_scaled` is false on a drafter whose planes carry no `.scale` and true when any one site carries one - fc, the selector's three codebooks, or one layer's plane of each of the nine per-layer site lists")]
def test_dflash_scaled_sites(t : T?) {
    let none = DflashW(fmt = KqFmt.nv4)
    let one = DflashW(fmt = KqFmt.nv4, scale = 2.0)
    for (site in range(14)) {
        var dr <- shape_drafter()
        dr.wq <- layer_sites(site, 4, one, none)
        dr.wk <- layer_sites(site, 5, one, none)
        dr.wv <- layer_sites(site, 6, one, none)
        dr.wo <- layer_sites(site, 7, one, none)
        dr.w1 <- layer_sites(site, 8, one, none)
        dr.w3 <- layer_sites(site, 9, one, none)
        dr.w2 <- layer_sites(site, 10, one, none)
        dr.aconv_proj <- layer_sites(site, 11, one, none)
        dr.fconv_proj <- layer_sites(site, 12, one, none)
        dr.fc = site == 0 ? one : none
        dr.sel_h = site == 1 ? one : none
        dr.sel_prev = site == 2 ? one : none
        dr.sel_next = site == 3 ? one : none
        t |> equal(dflash_scaled(dr), site != 13, site == 13 ? "control: no plane carries a scale" : "site {site} alone carries a scale")
        delete dr
    }
}
