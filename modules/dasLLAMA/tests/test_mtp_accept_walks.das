options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_sampling
require daslib/random
require math
require _mtp_walk_fixture   // bare_session, same_rng

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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
