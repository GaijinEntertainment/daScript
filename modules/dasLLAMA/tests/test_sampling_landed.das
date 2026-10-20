options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_sampling
require dasllama/dasllama_gpu_tier
require daslib/random
require daslib/math_bits
require math
require _sel_oracle   // select_rows_oracle: the landing a device step would leave, read off the cell's own rows
require _mtp_walk_fixture   // bare_session: the session the walks run on

// The landed sampler is host arithmetic over a row's survivors, max and denominator in place of the row, so its reference is
// the row sampler over the whole row with the same seed. The cell builds the landing the device would, off its own rows,
// through the selection's oracle, and installs seams that answer it and land the rows late where the sampler asks.

let VOCAB = 4096l
let DRAFTS = 3l
let CAP = 2048l
let WALK_SEEDS = 15

var g_full_rows : array<float>   //! the rows behind the landing: the late landing copies them
var g_landing : SelLanding
var g_hdr, g_ids : array<uint>
var g_vals, g_rec : array<float>
var g_lands = 0

[unused_argument(nrows, words)]
def private test_sel_ask(nrows : int64; words : array<uint>) : bool => true

def private test_sel_landing() : SelLanding => g_landing

def private test_sel_land_rows(row0, nrows : int64; var dst : array<float>) {
    g_lands++
    for (i in range64(nrows * VOCAB)) {
        dst[i] = g_full_rows[row0 * VOCAB + i]
    }
}

//! the landing the selection kernel would leave for `nrows` rows of `rows` at `temp`, the recent ids `g_sel_ask_ids` lists, packed
//! off the selection's oracle as the device packs it
def private build_landing(rows : array<float>; nrows : int64; temp : float) {
    g_full_rows := rows
    g_hdr |> resize(SEL_HDR * nrows)
    g_ids |> resize(CAP * nrows)
    g_vals |> resize(CAP * nrows)
    g_rec |> resize(SEL_RECENT * nrows)
    var temps <- [for (_r in range64(nrows)); temp]
    var recent : array<array<int64>>
    recent |> reserve(nrows)
    for (_r in range64(nrows)) {
        recent |> push_clone(g_sel_ask_ids)
    }
    var oracle <- select_rows_oracle(rows, temps, recent, VOCAB, nrows, SEL_K, CAP)
    for (r in range64(nrows)) {
        let n = long_length(oracle[r].ids)
        for (j in range64(min(n, CAP))) {
            g_ids[r * CAP + j] = uint(oracle[r].ids[j])
            g_vals[r * CAP + j] = oracle[r].vals[j]
        }
        g_hdr[r * SEL_HDR + SEL_HDR_COUNT] = uint(min(n, CAP))
        g_hdr[r * SEL_HDR + SEL_HDR_OVERFLOW] = oracle[r].overflow ? 1u : 0u
        g_hdr[r * SEL_HDR + SEL_HDR_MAX] = float_bits_to_uint(oracle[r].rmax)
        g_hdr[r * SEL_HDR + SEL_HDR_DEN] = float_bits_to_uint(oracle[r].den)
        for (v, j in oracle[r].recent, count()) {
            g_rec[r * SEL_RECENT + int64(j)] = v
        }
    }
    delete oracle
    delete recent
    delete temps
    unsafe {
        g_landing.rows = nrows
        g_landing.cap = CAP
        g_landing.recent_cap = SEL_RECENT
        g_landing.hdr = addr<uint const?>(g_hdr[0])
        g_landing.ids = addr<uint const?>(g_ids[0])
        g_landing.vals = addr<float const?>(g_vals[0])
        g_landing.rec = addr<float const?>(g_rec[0])
    }
}

//! the ids the last landing holds on its row 0
def private g_landing_ids() : array<int64> => <- [for (j in range64(int64(g_hdr[SEL_HDR_COUNT]))); int64(g_ids[j])]

//! `nrows` peaked rows: a few logits stand well above a spread of small ones, so a nucleus at 0.7 sits inside the first hundred
def private peaked_rows(nrows : int64; seed : int) : array<float> {
    var rng = random_seed(seed)
    var rows : array<float>
    rows |> resize(nrows * VOCAB)
    for (i in range64(nrows * VOCAB)) {
        let u = random_float(rng)
        rows[i] = 12.0 * u * u * u * u - 2.0
    }
    return <- rows
}

//! one bare session over `nrows` rows of `rows` under `sp`, seeded at `seed`, with `recent` as its window
def private twin(rows : array<float>; nrows : int64; var sp : SamplingParams; seed : int; recent : array<int64>) : Session {
    var s <- bare_session(VOCAB, DRAFTS)
    s.spec_params = unsafe(addr(sp))
    s.rng = random_seed(seed)
    s.recent |> clear()
    s.recent |> push_from(recent)
    for (i in range64(nrows * VOCAB)) {
        s.mtp_logits_b[i] = rows[i]
    }
    for (i in range64(VOCAB)) {
        s.logits[i] = rows[i]
    }
    return <- s
}

//! the landing of `rows` for `a`'s step, built with the ask the driver's glue would make and landed as the glue lands it
def private land_for(var a : Session; rows : array<float>; nrows : int64; sp : SamplingParams) {
    var words : array<uint>
    sel_ask_words(a, nrows, a.mtp_vbatch, DRAFTS + 1l, words)
    build_landing(rows, nrows, sp.temp)
    g_sel_landing = rdec_sel_landing()
    a.sel_row1 = 1l
    delete words
}

//! one draw off the landing and one off the rows: `lands` the late landings the first took, `served` the rows it drew off the landing
def private draw_pair(rows : array<float>; var sp : SamplingParams; seed : int; recent : array<int64>; var lands, served : int&) : tuple<landed : int64; row : int64> {
    var a <- twin(rows, 1l, sp, seed, recent)
    var b <- twin(rows, 1l, sp, seed, recent)
    land_for(a, rows, 1l, sp)
    let l0 = g_lands
    let s0 = int(sel_served())
    let x = sample_logits_row(a, sp, a.mtp_logits_b, 0l, VOCAB)
    lands = g_lands - l0
    served = int(sel_served()) - s0
    let y = sample_logits_row(b, sp, b.mtp_logits_b, 0l, VOCAB)
    delete a
    delete b
    to_log(LOG_INFO, "seed {seed}: landed {x}, row {y}\n")
    return (landed = x, row = y)
}

var g_norecent : array<int64>

def private cell_nucleus(t0 : T?) {
    t0 |> run("the nucleus alone: every draw off the landing is the row's, no late landing") <| @(t : T?) {
        var sp = SamplingParams(temp = 0.7, top_p = 0.9)
        var agree = 0
        var lands = 0
        var served = 0
        for (seed in range(1, 41)) {
            var rows <- peaked_rows(1l, seed)
            let p = draw_pair(rows, sp, seed, g_norecent, lands, served)
            agree += p.landed == p.row ? 1 : 0
            t |> equal(lands, 0, "seed {seed}: the nucleus is reached inside the landing")
            t |> equal(served, 1, "seed {seed}: the draw came off the landing")
            delete rows
        }
        t |> equal(agree, 40, "every draw agrees ({agree} of 40)")
    }
}

def private cell_topk_minp(t0 : T?) {
    t0 |> run("a top-k within the cap with a nucleus, and a min-p alone: the candidate-list and the floor paths") <| @(t : T?) {
        var spk = SamplingParams(temp = 0.8, top_k = 20l, top_p = 0.95)
        var spm = SamplingParams(temp = 1.0, min_p = 0.05)
        var agree = 0
        var lands = 0
        var served = 0
        for (seed in range(1, 21)) {
            var rows <- peaked_rows(1l, seed + 100)
            let pk = draw_pair(rows, spk, seed, g_norecent, lands, served)
            t |> success(lands == 0 && served == 1, "top-k seed {seed}: served off the landing")
            let pm = draw_pair(rows, spm, seed, g_norecent, lands, served)
            t |> success(lands == 0 && served == 1, "min-p seed {seed}: served off the landing")
            agree += (pk.landed == pk.row ? 1 : 0) + (pm.landed == pm.row ? 1 : 0)
            delete rows
        }
        t |> equal(agree, 40, "every draw agrees ({agree} of 40)")
    }
}

def private cell_penalties(t0 : T?) {
    t0 |> run("penalties over a recent window that spans landed and unlanded ids: the survivors penalized and the denominator corrected") <| @(t : T?) {
        var sp = SamplingParams(temp = 0.7, top_p = 0.9, penalty = 1.3, presence_penalty = 0.4, frequency_penalty = 0.2)
        var agree = 0
        var lands = 0
        var served = 0
        for (seed in range(1, 21)) {
            var rows <- peaked_rows(1l, seed + 200)
            // the peak and a mid token sit among the survivors, 4000 well under the floor
            var mx = 0l
            for (i in range64(VOCAB)) {
                if (rows[i] > rows[mx]) {
                    mx = i
                }
            }
            var win <- [mx, mx, 300l, 4000l, 12l]
            let p = draw_pair(rows, sp, seed, win, lands, served)
            agree += p.landed == p.row ? 1 : 0
            t |> success(lands == 0 && served == 1, "seed {seed}: served off the landing")
            delete win
            delete rows
        }
        t |> equal(agree, 20, "every draw agrees ({agree} of 20)")
    }
}

def private cell_late_landing(t0 : T?) {
    t0 |> run("a sampler the landing cannot serve lands the rows late and draws the row's draw: no cut at all, and a nucleus a flat row does not reach") <| @(t : T?) {
        var spn = SamplingParams(temp = 0.7)
        var lands = 0
        var served = 0
        var rows <- peaked_rows(1l, 7)
        let p = draw_pair(rows, spn, 3, g_norecent, lands, served)
        t |> success(lands == 1 && served == 0, "no cut: the rows landed late ({lands} landings, {served} served)")
        t |> equal(p.landed, p.row, "... and the draw is the row's")
        var flat : array<float>
        flat |> resize(VOCAB)
        for (i in range64(VOCAB)) {
            flat[i] = 1.0 + 0.001 * float(i % 7l)   // seven values spread over the row: the landed quarter holds a quarter of the mass
        }
        var spp = SamplingParams(temp = 1.0, top_p = 0.9)
        let flat_pair = draw_pair(flat, spp, 5, g_norecent, lands, served)
        t |> success(lands == 1 && served == 0, "a flat row's nucleus reaches past the landing: the rows landed late ({lands} landings, {served} served)")
        t |> equal(flat_pair.landed, flat_pair.row, "... and the draw is the row's")
        delete flat
        delete rows
    }
}

//! one draw a side as `draw_pair`, over twins the caller shaped (`shape` runs on both after the landing): `lands` and `served` as there
def private draw_pair_shaped(rows : array<float>; var sp : SamplingParams; seed : int; recent : array<int64>; var lands, served : int&; shape : block<(var s : Session) : void>) : tuple<landed : int64; row : int64> {
    var a <- twin(rows, 1l, sp, seed, recent)
    var b <- twin(rows, 1l, sp, seed, recent)
    land_for(a, rows, 1l, sp)
    invoke(shape, a)
    invoke(shape, b)
    let l0 = g_lands
    let s0 = int(sel_served())
    let x = sample_logits_row(a, sp, a.mtp_logits_b, 0l, VOCAB)
    lands = g_lands - l0
    served = int(sel_served()) - s0
    let y = sample_logits_row(b, sp, b.mtp_logits_b, 0l, VOCAB)
    delete a
    delete b
    to_log(LOG_INFO, "seed {seed}: landed {x}, row {y}\n")
    return (landed = x, row = y)
}

//! a row whose logit rises with the id, `rising` ids at 3 + id / 1000 and the rest at -2
def private rising_row(rising : int64) : array<float> => <- [for (i in range64(VOCAB)); i < rising ? 3.0 + 0.001 * float(i) : -2.0]

//! a row whose landing holds more survivors than MTP_Q_CAP: `ties` ids tied at the K-th largest logit under 1000 distinct ones above
//! them, the rest at -2 (the landing keeps SEL_K survivors and every tie at the K-th)
def private tied_row(ties : int64) : array<float> => <- [for (i in range64(VOCAB)); i < ties ? 3.4 : (i < ties + 1000l ? 3.5 + 0.001 * float(i - ties) : -2.0)]

def private cell_row_paths(t0 : T?) {
    t0 |> run("the landing's four refusals take the row path and draw the row's draw: a row whose ties overflow the slots, a penalized id the ask never listed, a top-k whose k-th falls under the floor once penalized, and a min-p floor under the landing's") <| @(t : T?) {
        var lands = 0
        var served = 0
        // 3000 ties at the max over 4096 ids: past the 2048 slots, the landing flags the overflow and the row sampler takes the row
        var ties <- [for (i in range64(VOCAB)); i < 3000l ? 5.0 : -2.0]
        var spn = SamplingParams(temp = 0.7, top_p = 0.9)
        let over = draw_pair(ties, spn, 21, g_norecent, lands, served)
        t |> success(lands == 1 && served == 0, "an overflowing row lands late ({lands} landings, {served} served)")
        t |> equal(over.landed, over.row, "... and the draw is the row's")
        // the ask listed id 12 alone; a window grown by 300 after the ask penalizes an id with no landed logit
        var rows <- peaked_rows(1l, 31)
        var spp = SamplingParams(temp = 0.7, top_p = 0.9, penalty = 1.3)
        var win <- [12l]
        let unlisted = draw_pair_shaped(rows, spp, 22, win, lands, served) $(var s : Session) {
            s.recent |> push(300l)
        }
        t |> success(lands == 1 && served == 0, "a penalized id the ask never listed lands the rows late ({lands} landings, {served} served)")
        t |> equal(unlisted.landed, unlisted.row, "... and the draw is the row's")
        // a top-k of the landing's whole width with the argmax penalized tenfold: the k-th largest penalized logit is the argmax's,
        // under the floor, so an unlanded logit could outrank it and the row sampler takes the row
        var rise <- rising_row(VOCAB)
        var spk = SamplingParams(temp = 0.7, top_k = SEL_K, penalty = 10.0)
        var top <- [VOCAB - 1l]
        let kth = draw_pair(rise, spk, 23, top, lands, served)
        t |> success(lands == 1 && served == 0, "a top-k whose k-th falls under the floor lands the rows late ({lands} landings, {served} served)")
        t |> equal(kth.landed, kth.row, "... and the draw is the row's")
        // a min-p floor at a ten-thousandth of the peak sits under the landing's floor: an unlanded logit could survive the cut
        var spm = SamplingParams(temp = 1.0, min_p = 1.0e-4)
        let low = draw_pair(rows, spm, 24, g_norecent, lands, served)
        t |> success(lands == 1 && served == 0, "a min-p floor under the landing's lands the rows late ({lands} landings, {served} served)")
        t |> equal(low.landed, low.row, "... and the draw is the row's")
        // the control: the same min-p sampler at a twentieth of the peak is served inside the landing
        var spm_in = SamplingParams(temp = 1.0, min_p = 0.05)
        let inside = draw_pair(rows, spm_in, 24, g_norecent, lands, served)
        t |> success(lands == 0 && served == 1, "control: a min-p floor inside the landing is served off it ({lands} landings, {served} served)")
        t |> equal(inside.landed, inside.row, "... and the draw is the row's")
        // a negative presence penalty lifts the window's lowest id (unlanded on a rising row) past the landing's floor: only the row holds it
        var spb = SamplingParams(temp = 0.7, top_p = 0.9, presence_penalty = -100.0)
        var low_id <- [0l]
        let boosted = draw_pair(rise, spb, 25, low_id, lands, served)
        t |> success(lands == 1 && served == 0, "a boosting penalty on an unlanded id lands the rows late ({lands} landings, {served} served)")
        t |> equal(boosted.landed, boosted.row, "... and the draw is the row's")
        var gate <- twin(rise, 1l, spb, 25, low_id)
        t |> success(!sel_wanted(gate), "the ask refuses a boosting presence penalty outright")
        spb.presence_penalty = 1.0
        t |> success(sel_wanted(gate), "control: a lowering presence penalty asks the selection")
        delete gate
        delete low_id
        delete top
        delete rise
        delete win
        delete rows
        delete ties
    }
}

def private cell_q_cap(t0 : T?) {
    t0 |> run("the rejection drafter's q off a landing with more survivors than MTP_Q_CAP (600 ties at the K-th under 1000 distinct): exactly the cap's worth of entries, id ascending, renormalized to one, the draft served off the landing") <| @(t : T?) {
        let ties = 600l
        var head <- tied_row(ties)
        var sp = SamplingParams(temp = 1.0, top_p = 0.99)
        var a <- twin(head, 1l, sp, 1, g_norecent)
        land_for(a, head, 1l, sp)
        t |> success(long_length(g_landing_ids()) > MTP_Q_CAP, "the landing holds more survivors than the cap ({length(g_landing_ids())})")
        var row := head
        let l0 = g_lands
        let s0 = sel_served()
        let d = mtp_draft_sample(a, row, VOCAB, 0l)
        t |> success(g_lands == l0 && sel_served() - s0 == 1l, "the draft came off the landing ({g_lands - l0} late landings, {sel_served() - s0} served)")
        t |> equal(a.mtp_qn[0], MTP_Q_CAP, "q holds exactly the cap's worth of entries")
        var sum = 0.0
        for (j in range64(a.mtp_qn[0])) {
            sum += a.mtp_q[j].v
        }
        t |> success(abs(sum - 1.0) < 1.0e-4, "... summing to one ({sum})")
        t |> success(mtp_q_of(a, 0l, 0l) > 0.0 && mtp_q_of(a, 0l, MTP_Q_CAP - 1l) > 0.0 && mtp_q_of(a, 0l, MTP_Q_CAP) == 0.0, "... the first cap's worth by id: the K-th's ties outrank nothing, so the record truncates by id")
        t |> success(d >= 0l && d < MTP_Q_CAP, "the draft is one of the recorded ids ({d})")
        delete row
        delete a
        delete head
    }
}

def private cell_walks(t0 : T?) {
    t0 |> run("the walks over landed verify rows: the typical and rejection judges and the rejection drafter answer as over the rows") <| @(t : T?) {
        var sp = SamplingParams(temp = 0.7, top_p = 0.9)
        var agree = 0
        var lands = 0
        let accept0 = get_mtp_accept()
        for (seed in range(1, WALK_SEEDS + 1)) {
            var rows <- peaked_rows(DRAFTS + 1l, seed + 300)
            for (mode in [MtpAccept.exact, MtpAccept.typical, MtpAccept.rejection]) {
                set_mtp_accept(mode)
                var a <- twin(rows, DRAFTS + 1l, sp, seed, g_norecent)
                var b <- twin(rows, DRAFTS + 1l, sp, seed, g_norecent)
                land_for(a, rows, DRAFTS + 1l, sp)
                // the drafts: the argmax of each row but the last, so the walks accept where the draw lands on it
                for (i in range64(DRAFTS)) {
                    var mx = 0l
                    for (j in range64(VOCAB)) {
                        if (rows[i * VOCAB + j] > rows[i * VOCAB + mx]) {
                            mx = j
                        }
                    }
                    a.mtp_vbatch[i + 1l] = mx
                    b.mtp_vbatch[i + 1l] = mx
                }
                if (mode == MtpAccept.rejection) {   // the drafter's q off the head's row, recorded on both twins alike
                    for (i in range64(DRAFTS)) {
                        var head <- [for (j in range64(VOCAB)); rows[i * VOCAB + j]]
                        var head2 := head
                        let l0 = g_lands
                        let da = mtp_draft_sample(a, head, VOCAB, i)
                        a.sel_row1 = 0l   // one draft row a landing: the walk's rows land separately below
                        let db = mtp_draft_sample(b, head2, VOCAB, i)
                        to_log(LOG_INFO, "seed {seed} draft {i}: landed draft {da}, row draft {db}\n")
                        agree += da == db && g_lands == l0 ? 1 : 0
                        t |> equal(a.mtp_qn[i], b.mtp_qn[i], "seed {seed} draft {i}: q holds the same entries off the landing")
                        delete head
                        delete head2
                    }
                    build_landing(rows, DRAFTS + 1l, sp.temp)
                    a.sel_row1 = 1l
                }
                let l0 = g_lands
                let s0 = sel_served()
                let xa = mtp_walk_by_accept(a, DRAFTS, VOCAB)
                t |> success(sel_served() - s0 == xa + 1l, "seed {seed} {mode}: every judged row came off the landing ({sel_served() - s0} rows for {xa} accepted)")
                let xb = mtp_walk_by_accept(b, DRAFTS, VOCAB)
                lands += g_lands - l0
                to_log(LOG_INFO, "seed {seed} {mode}: landed accepted {xa} parked {a.mtp_pre_tok}, rows accepted {xb} parked {b.mtp_pre_tok}\n")
                agree += xa == xb && a.mtp_pre_tok == b.mtp_pre_tok ? 1 : 0
                delete a
                delete b
            }
            delete rows
        }
        set_mtp_accept(accept0)
        let want = WALK_SEEDS * (3 + int(DRAFTS))   // three walks a seed, and the rejection walk's drafts beside them
        t |> equal(lands, 0, "every walk served off the landing")
        t |> equal(agree, want, "every walk accepts the same rows and parks the same token ({agree} of {want})")
    }
}

def private cell_chain_cut(t0 : T?) {
    t0 |> run("the chain cut's probability off the landing is the row's softmax at the stream's temperature") <| @(t : T?) {
        var sp = SamplingParams(temp = 0.7, top_p = 0.9)
        var rows <- peaked_rows(1l, 9)
        var a <- twin(rows, 1l, sp, 1, g_norecent)
        land_for(a, rows, 1l, sp)
        var mx = 0l
        var den = 0.0lf
        for (i in range64(VOCAB)) {
            if (rows[i] > rows[mx]) {
                mx = i
            }
        }
        for (i in range64(VOCAB)) {
            den += exp(double(rows[i] - rows[mx]) / double(sp.temp))
        }
        let want = float(exp(double(rows[12] - rows[mx]) / double(sp.temp)) / den)
        let got = sel_token_prob(a, 0l, 12l)
        let bar = 1.0e-5 * max(want, 1.0e-6)
        t |> success(abs(got - want) <= bar || got == 0.0 && want < 1.0e-6, "token 12: {got} against {want}")
        t |> success(is_finite(got) && abs(got - want * 1.01) > bar, "control: a probability a percent off reds the bar ({got} against {want * 1.01})")
        let peak = sel_token_prob(a, 0l, mx)
        let peak_want = float(1.0lf / den)
        t |> success(abs(peak - peak_want) <= 1.0e-5, "the peak's probability ({peak} against {peak_want})")
        t |> success(is_finite(peak) && abs(peak - peak_want * 1.01) > 1.0e-5, "control: the peak's probability a percent off reds the bar")
        delete a
        delete rows
    }
}

[test]
def test_landed_sampler_matches_the_row_sampler(t0 : T?) {
    let seats = rdec_select_pin()
    let landing0 = get_select_landing()
    install_rdec_select(@@test_sel_ask, @@test_sel_landing, @@test_sel_land_rows)
    set_select_landing(true)
    cell_nucleus(t0)
    cell_topk_minp(t0)
    cell_penalties(t0)
    cell_late_landing(t0)
    cell_row_paths(t0)
    cell_q_cap(t0)
    cell_walks(t0)
    cell_chain_cut(t0)
    set_select_landing(landing0)
    restore_rdec_select(seats)
}
