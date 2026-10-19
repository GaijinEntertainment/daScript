options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_sampling shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_blocks   // eval_embd_: the embedding-input forward the generator drives
require dasllama/dasllama_math   // softmax/softcap on the logits row
require daslib/random
require daslib/sort_boost   // the top-k selection heap
require daslib/algorithm   // lower_bound: a candidate by id
require daslib/math_bits
require math
require daslib/cmp

// Token sampling and the generation drivers on top of forward()/eval_batch. The engine reaches
// this file through one registry only - the accept walk's row sampler installed at [init]; the
// umbrella re-exports it so every consumer of dasllama/dasllama keeps generate_/sample_/set_seed_/stats_.

// ===== Sampling =====

//! Seed the session's sampling RNG for reproducible generation.
def set_seed_(var s : Session; seed : int) {
    s.rng = random_seed(seed)
}

//! whether `id` occurs in `s.recent[pstart .. k)`
def private recent_has_before_(s : Session; pstart, k : int; id : int64) : bool {
    for (j in range(pstart, k)) {
        return true if (s.recent[j] == id)
    }
    return false
}

//! the penalized ids of the last `penalty_last_n` of `s.recent`, each once with its count in that window: `blk(id, count)`
def private each_penalized_(s : Session; params : SamplingParams; blk : block<(id : int64; cnt : int) : void>) {
    let n = length(s.recent)
    let win = int(params.penalty_last_n)
    let pstart = n > win ? n - win : 0
    for (k in range(pstart, n)) {
        let id = s.recent[k]
        continue if (recent_has_before_(s, pstart, k, id))
        var cnt = 1
        for (j in range(k + 1, n)) {
            if (s.recent[j] == id) {
                cnt++
            }
        }
        blk |> invoke(id, cnt)
    }
}

//! logit `v` of a token seen `cnt` times in the window under `params`: multiplicative, then presence + frequency * count
def private penalized_(params : SamplingParams; v : float; cnt : int) : float {
    var r = v
    if (params.penalty > 0.0 && params.penalty != 1.0) {
        r = r > 0.0 ? r / params.penalty : r * params.penalty
    }
    return r - (params.presence_penalty + params.frequency_penalty * float(cnt))
}

//! penalties over the last `penalty_last_n` of `s.recent` onto the row at `off` in `row`
def private apply_penalties_(s : Session; params : SamplingParams; var row : array<float>; off : int64) {
    return if (penalties_off(params))
    each_penalized_(s, params) $(id : int64; cnt : int) {
        row[off + id] = penalized_(params, row[off + id], cnt)
    }
}

//! Sample the next token from `s.logits` per `params`: penalties, temp + top-k, softmax, top-p/min-p, a CDF draw
//! (the argmax at temp <= 0); under `s.constrain` the best admitted token, -1 + `constrain_dead` at a dead end.
//! Consumes `s.logits` in place; a token the accept walk already drew (s.mtp_pre_tok) is returned instead.
[hot_path]   // once per generated token
def sample_(var s : Session; params : SamplingParams) : int64 {
    s.sampled = !sampler_is_argmax(params) || s.constrain != null   // any non-argmax draw (a temperature, or a penalty at temp 0) stands the greedy chain down: it predicts the raw argmax, the sampler picks something else
    if (s.mtp_pre_drawn) {
        s.mtp_pre_drawn = false
        return s.mtp_pre_tok
    }
    if (s.pick_ready) {   //! the device landed the argmax alone: `logits` holds no row for this step
        s.pick_ready = false
        return s.pick_tok
    }
    let vocab = long_length(s.logits)
    if (params.temp <= 0.0) {
        apply_penalties_(s, params, s.logits, 0l)
        if (s.constrain != null) {
            return constrained_pick(s, vocab)
        }
        return argmax(s, vocab)
    }
    return sample_logits_row(s, params, s.logits, 0l, vocab)
}

//! The candidates a constrained draw probes before it walks the whole vocabulary.
let CONSTRAINED_PROBE_K = 256l

// the first candidate the constraint admits, in logit order, committed; -1 when it admits none
def private first_admitted(var s : Session) : int64 {
    sort(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
    for (c in s.cand) {
        if (s.constrain->admits(c.id)) {
            s.constrain->commit(c.id)
            return c.id
        }
    }
    return -1l
}

// the best admitted token of the row: the argmax when it passes, else the row's k largest in logit order,
// else every token in logit order; the winner is committed. -1 and `constrain_dead` when nothing is admitted.
def private constrained_pick(var s : Session; vocab : int64) : int64 {
    let best = argmax(s, vocab)
    if (s.constrain->admits(best)) {
        s.constrain->commit(best)
        return best
    }
    select_top_k_(s, s.logits, 0l, vocab, min(CONSTRAINED_PROBE_K, vocab))
    let near = first_admitted(s)
    if (near >= 0l) {
        return near
    }
    s.cand |> clear()
    s.cand |> reserve(vocab)
    for (i in range64(vocab)) {
        s.cand |> push(SampleCand(id = i, v = s.logits[i]))
    }
    let far = first_admitted(s)
    if (far < 0l) {
        s.constrain_dead = true
    }
    return far
}

//! The widest top-k the candidate-list sampler serves; a wider (or absent) top-k takes the
//! vocab-wide reference path - under a constraint it takes the candidate list at this cap instead,
//! since a refused pick probes every candidate.
let SAMPLE_TOPK_FAST_CAP = 1024l

//! sample_'s row form over the `vocab`-wide logits row at `off` in `row` (the sampler's scratch
//! afterwards); the accept walk draws each verify row through it. A top-k within
//! SAMPLE_TOPK_FAST_CAP runs every cut and the draw over its k survivors alone.
def sample_logits_row(var s : Session; params : SamplingParams; var row : array<float>; off, vocab : int64) : int64 {
    if (sel_landed(s, off / vocab)) {
        return draw_candidates_(s) if (landed_row_(s, params, sel_row_of(s, off / vocab), vocab))
        sel_fallback_(s, row, vocab)
    }
    apply_penalties_(s, params, row, off)
    if (s.constrain != null && params.temp > 0.0 && off == 0l) {
        //! a constrained draw over the k largest logits (a wider or absent top-k reads as the cap). With no nucleus or min-p cut the
        //! draw goes first and a refused pick redraws over the admitted candidates - rejection sampling, the filtered draw's own
        //! distribution at one probe a token; a cut must see the admitted set alone, so under one every candidate is probed first
        let k = params.top_k > 0l ? min(params.top_k, SAMPLE_TOPK_FAST_CAP) : SAMPLE_TOPK_FAST_CAP
        select_top_k_(s, row, off, vocab, min(k, vocab))
        let no_cut = params.top_p >= 1.0 && params.min_p <= 0.0
        var first = -1l
        if (no_cut) {
            s.cand_full |> reserve(length(s.cand))
            s.cand_full |> resize(length(s.cand))
            for (c, i in s.cand, count()) {
                s.cand_full[i] = c
            }
            first = sample_candidates_(s, params)
            if (s.constrain->admits(first)) {
                s.constrain->commit(first)
                return first
            }
        }
        var kept = 0
        for (c in no_cut ? s.cand_full : s.cand) {
            if (c.id != first && s.constrain->admits(c.id)) {
                s.cand[kept] = c
                kept++
            }
        }
        if (kept == 0) {
            return constrained_pick(s, vocab)
        }
        s.cand |> reserve(kept)   // the list only shrinks here; the reserve is the exact-size contract's
        s.cand |> resize(kept)
        let tok = sample_candidates_(s, params)
        s.constrain->commit(tok)
        return tok
    }
    if (params.temp > 0.0 && params.top_k > 0l && params.top_k < vocab && params.top_k <= SAMPLE_TOPK_FAST_CAP) {
        select_top_k_(s, row, off, vocab, params.top_k)
        return sample_candidates_(s, params)
    }
    return sample_logits_row_reference(s, params, row, off, vocab)
}

def private cand_precedes_(x, y : SampleCand) : bool => x.v > y.v   // the heap's root is the smallest survivor

//! the landed step's rows as they would have landed, into `rows` (the walk's row buffer, or the session's logits): a sampler the
//! selection cannot serve takes the row path from here, the landing cleared
def private sel_fallback_(s : Session; var rows : array<float>; vocab : int64) {
    let row0 = sel_row_of(s, 0l)
    sel_land_rows(row0, min(long_length(rows) / vocab, g_sel_landing.rows - row0), rows)
}

//! the landing's row `r` into `s.cand`, id ascending, the recent window's penalties applied to the survivors it holds - the row's
//! floor (its smallest landed logit, which every unlanded logit sits under), max and denominator at the temperature out, the
//! denominator corrected for every penalized id; false where the row overflowed its slots or a penalized id has no landed logit
def private landed_cands_(var s : Session; params : SamplingParams; r : int64; var floor, mx, den : float&) : bool {
    unsafe {
        return false if (g_sel_landing.hdr[r * SEL_HDR + SEL_HDR_OVERFLOW] != 0u)
        let n = int64(g_sel_landing.hdr[r * SEL_HDR + SEL_HDR_COUNT])
        mx = uint_bits_to_float(g_sel_landing.hdr[r * SEL_HDR + SEL_HDR_MAX])
        den = uint_bits_to_float(g_sel_landing.hdr[r * SEL_HDR + SEL_HDR_DEN])
        s.cand |> clear()
        s.cand |> reserve(n)
        floor = 3.0e38
        for (j in range64(n)) {
            let v = g_sel_landing.vals[r * g_sel_landing.cap + j]
            floor = min(floor, v)
            s.cand |> push(SampleCand(id = int64(g_sel_landing.ids[r * g_sel_landing.cap + j]), v = v))
        }
    }
    sort(s.cand) <| cmp_by(_.id)
    return true if (penalties_off(params))
    let inv = 1.0 / params.temp
    var every_landed = true
    each_penalized_(s, params) $(id : int64; cnt : int) {
        return if (!every_landed)
        let at = find_index(g_sel_ask_ids, id)
        if (at < 0) {
            every_landed = false
            return
        }
        let raw = unsafe(g_sel_landing.rec[r * g_sel_landing.recent_cap + int64(at)])
        let v = penalized_(params, raw, cnt)
        let ci = cand_index_(s, id)
        if (ci < 0 && v > floor) {   // a penalty lifted an unlanded token past the landing's floor: it could be drawn, and only the row holds it
            every_landed = false
            return
        }
        den += exp(max((v - mx) * inv, -80.0)) - exp(max((raw - mx) * inv, -80.0))
        if (ci >= 0) {
            s.cand[ci].v = v
        }
    }
    return every_landed
}

//! the index of `id` in `s.cand` (id ascending), -1 where it holds none
def private cand_index_(s : Session; id : int64) : int {
    let at = lower_bound(s.cand, id) $(c : SampleCand const -&; v : int64) : bool { return c.id < v }
    return at < length(s.cand) && s.cand[at].id == id ? at : -1
}

//! the `k`-th largest `v` over `s.cand` (`s.cand_full` the sorted copy afterwards)
def private cand_kth_(var s : Session; k : int) : float {
    s.cand_full |> reserve(length(s.cand))
    s.cand_full |> resize(length(s.cand))
    for (c, i in s.cand, count()) {
        s.cand_full[i] = c
    }
    sort(s.cand_full) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
    return s.cand_full[min(k, length(s.cand_full)) - 1].v
}

//! `s.cand` narrowed in place to the entries at or above `kth` (ties kept), its order kept
def private keep_cands_from_(var s : Session; kth : float) {
    var kept = 0
    for (c in s.cand) {
        if (c.v >= kth) {
            s.cand[kept] = c
            kept++
        }
    }
    s.cand |> reserve(kept)
    s.cand |> resize(kept)
}

//! every `s.cand` entry under `thr` cut to 0
def private cut_cands_under_(var s : Session; thr : float) {
    for (c in s.cand) {
        if (c.v < thr) {
            c.v = 0.0
        }
    }
}

//! the min-p floor over `s.cand` (probabilities in `v`): `min_p` of the largest
def private cand_min_p_thr_(s : Session; min_p : float) : float {
    var maxp = 0.0
    for (c in s.cand) {
        maxp = max(maxp, c.v)
    }
    return min(min_p, 1.0) * maxp
}

//! the candidate-list path over the landing: `s.cand` narrowed to the top-k largest penalized logits (ties kept), then the
//! candidate cuts; false where the k-th fell under the landing's floor - an unlanded logit could outrank it
def private landed_topk_(var s : Session; params : SamplingParams; floor : float) : bool {
    let kth = cand_kth_(s, int(params.top_k))
    return false if (kth < floor)
    keep_cands_from_(s, kth)
    cut_candidates_(s, params)
    return true
}

//! the vocab-wide path over the landing: the survivors' probabilities under the row's denominator, then the nucleus and min-p cuts,
//! every unlanded token known cut since its probability sits under the floor's; false where a cut's boundary reaches under the
//! floor's probability, the nucleus is not reached inside the landing, a top-k past the cap is armed, or no cut at all is
def private landed_cut_(var s : Session; params : SamplingParams; floor, mx, den : float) : bool {
    return false if (params.top_k > 0l || (params.top_p >= 1.0 && params.min_p <= 0.0))
    let inv = 1.0 / params.temp
    let inv_den = 1.0 / den
    for (c in s.cand) {
        c.v = exp(max((c.v - mx) * inv, -80.0)) * inv_den
    }
    let pbase = exp(max((floor - mx) * inv, -80.0)) * inv_den
    if (params.top_p < 1.0) {
        sort(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
        let target = max(params.top_p, 0.0)
        var mass = 0.0
        var cut = -1.0
        for (c in s.cand) {
            return false if (c.v < pbase)
            mass += c.v
            if (mass >= target) {
                cut = c.v
                break
            }
        }
        return false if (cut < 0.0)
        cut_cands_under_(s, cut)
        sort(s.cand) <| cmp_by(_.id)
    }
    if (params.min_p > 0.0) {
        let thr = cand_min_p_thr_(s, params.min_p)
        return false if (params.top_p >= 1.0 && thr < pbase)
        cut_cands_under_(s, thr)
    }
    return true
}

//! `s.cand` out with post-cut probabilities in `v`, id ascending; false where the rows must land
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing")]
def private landed_row_(var s : Session; params : SamplingParams; r, vocab : int64) : bool {
    var floor = 0.0
    var mx = 0.0
    var den = 0.0
    return false if (!landed_cands_(s, params, r, floor, mx, den))
    let fast = params.top_k > 0l && params.top_k < vocab && params.top_k <= SAMPLE_TOPK_FAST_CAP
    let served = fast ? landed_topk_(s, params, floor) : landed_cut_(s, params, floor, mx, den)
    if (served) {
        sel_served_note()
    }
    return served
}

//! Fill `s.cand` with every id whose logit is >= the k-th largest in the row, ascending by id
//! (ties at the k-th value all survive, as the reference's `< kth` mask keeps them).
def private select_top_k_(var s : Session; row : array<float>; off, vocab, k : int64) {
    s.cand |> clear()
    for (i in range64(vocab)) {
        let v = row[off + i]
        if (long_length(s.cand) < k) {
            s.cand |> push(SampleCand(id = i, v = v))
            push_heap(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
        } elif (v > s.cand[0].v) {
            pop_heap(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
            s.cand[length(s.cand) - 1] = SampleCand(id = i, v = v)
            push_heap(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
        }
    }
    let kth = s.cand[0].v
    s.cand |> clear()
    for (i in range64(vocab)) {
        if (row[off + i] >= kth) {
            s.cand |> push(SampleCand(id = i, v = row[off + i]))
        }
    }
}

//! Temperature, softmax, the nucleus and min-p cuts and one draw over `s.cand` (logits in `v` on
//! entry, probabilities on exit); the same arithmetic as the reference over the survivors alone.
def private sample_candidates_(var s : Session; params : SamplingParams) : int64 {
    cut_candidates_(s, params)
    return draw_candidates_(s)
}

//! one draw over `n` weights `w(i)` (a cut entry at 0), the cdf walked in index order: the index the draw crosses at, else the last positive
def private cdf_draw_(var s : Session; n : int64; w : block<(i : int64) : float>) : int64 {
    var total = 0.0
    for (i in range64(n)) {
        total += w |> invoke(i)
    }
    let r = random_float(s.rng) * total
    var cdf = 0.0
    var last = 0l
    for (i in range64(n)) {
        let wi = w |> invoke(i)
        continue if (wi <= 0.0)
        last = i
        cdf += wi
        return i if (r < cdf)
    }
    return last
}

//! one draw over `s.cand`'s surviving mass (probabilities in `v`, a cut token at 0)
def private draw_candidates_(var s : Session) : int64 {
    let at = cdf_draw_(s, long_length(s.cand)) $(i : int64) : float { return s.cand[i].v }
    return s.cand[at].id
}

//! the cuts over `s.cand` with no draw: temperature, softmax, the nucleus and min-p (logits in `v` on entry, probabilities on
//! exit, a cut token at 0)
def private cut_candidates_(var s : Session; params : SamplingParams) {
    let inv = 1.0 / params.temp
    var mx = -1.0e30
    for (c in s.cand) {
        c.v *= inv
        mx = max(mx, c.v)
    }
    var sum = 0.0
    for (c in s.cand) {
        c.v = exp(max(c.v - mx, -80.0))
        sum += c.v
    }
    let inv_sum = 1.0 / sum
    for (c in s.cand) {
        c.v *= inv_sum
    }
    if (params.top_p < 1.0) {
        // the survivors sorted by probability, largest first: the smallest prefix whose mass reaches top_p sets the cut
        sort(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
        let target = max(params.top_p, 0.0)
        var mass = 0.0
        var cut = 0.0
        for (c in s.cand) {
            cut = c.v
            mass += cut
            break if (mass >= target)
        }
        cut_cands_under_(s, cut)
        sort(s.cand) <| cmp_by(_.id)
    }
    if (params.min_p > 0.0) {
        cut_cands_under_(s, cand_min_p_thr_(s, params.min_p))
    }
}

//! The vocab-wide sampler: temperature over the row, top-k as k max passes, softmax over the row,
//! the nucleus cut over a sorted copy, min-p, and a CDF walk over the row. Serves a top-k the
//! candidate list does not (off, or past SAMPLE_TOPK_FAST_CAP) and is the fast path's oracle.
def sample_logits_row_reference(var s : Session; params : SamplingParams; var row : array<float>; off, vocab : int64) : int64 {
    if (params.temp <= 0.0) {
        var mi = 0l
        for (i in range64(1l, vocab)) {
            if (row[off + i] > row[off + mi]) {
                mi = i
            }
        }
        return mi
    }
    cut_row_reference_(s, params, row, off, vocab)
    return draw_row_reference_(s, row, off, vocab)
}

//! one draw over the row's surviving mass (probabilities in the row, a cut token at 0): the cuts zero probabilities without
//! renormalizing, and the cdf accumulates in `total`'s order, so the walk always lands on a surviving token
def private draw_row_reference_(var s : Session; row : array<float>; off, vocab : int64) : int64
    => cdf_draw_(s, vocab) $(i : int64) : float { return row[off + i] }

//! the nucleus's boundary probability: the smallest descending prefix whose mass reaches `target`, read off the row's largest
//! `k` probabilities (ties kept, sorted; `k` doubling from NUCLEUS_SELECT_K while the prefix falls short, the whole row past
//! NUCLEUS_SELECT_CAP) - every probability outside the set sits under the k-th, so the prefix found is the sorted row's own
def private nucleus_cut_(var s : Session; row : array<float>; off, vocab : int64; target : float) : float {
    var k = min(NUCLEUS_SELECT_K, vocab)
    while (true) {
        select_top_k_(s, row, off, vocab, k)
        sort(s.cand) $(x, y : SampleCand) : bool { return cand_precedes_(x, y) }
        var mass = 0.0
        for (c in s.cand) {
            mass += c.v
            return c.v if (mass >= target)
        }
        return s.cand[length(s.cand) - 1].v if (k >= vocab)
        k = min(k * 2l, k >= NUCLEUS_SELECT_CAP ? vocab : NUCLEUS_SELECT_CAP)
    }
    return 0.0
}

//! the nucleus selection's first width and the widest before the whole row is taken: a flat row past the cap pays one more pass
let NUCLEUS_SELECT_K = 64l
let NUCLEUS_SELECT_CAP = 4096l

//! the vocab-wide cuts with no draw (a temperature above zero): the row's logits become its probabilities, a cut token at 0
def private cut_row_reference_(var s : Session; params : SamplingParams; var row : array<float>; off, vocab : int64) {
    let inv = 1.0 / params.temp
    for (i in range64(vocab)) {
        row[off + i] *= inv
    }
    if (params.top_k > 0l && params.top_k < vocab) {
        for (i in range64(vocab)) {
            s.sample_scratch[i] = row[off + i]
        }
        var kth = -1.0e30
        for (_j in range64(params.top_k)) {
            var mx = -1.0e30
            var mi = 0l
            for (i in range64(vocab)) {
                if (s.sample_scratch[i] > mx) {
                    mx = s.sample_scratch[i]
                    mi = i
                }
            }
            kth = mx
            s.sample_scratch[mi] = -1.0e30
        }
        for (i in range64(vocab)) {
            if (row[off + i] < kth) {
                row[off + i] = -1.0e30
            }
        }
    }
    softmax(unsafe(addr(row[off])), vocab)
    if (params.top_p < 1.0) {
        let cut = nucleus_cut_(s, row, off, vocab, max(params.top_p, 0.0))
        for (i in range64(vocab)) {
            if (row[off + i] < cut) {
                row[off + i] = 0.0
            }
        }
    }
    if (params.min_p > 0.0) {
        var maxp = 0.0
        for (i in range64(vocab)) {
            maxp = max(maxp, row[off + i])
        }
        let thr = min(params.min_p, 1.0) * maxp
        for (i in range64(vocab)) {
            if (row[off + i] < thr) {
                row[off + i] = 0.0
            }
        }
    }
}

//! the accept walk's judge seam ([[MtpJudgeFn]]): the last row (`draft` under 0), a constrained stream, a greedy sampler and the exact
//! acceptance draw the row as the sampler does; typical keeps a draft the cuts admit, rejection keeps it with probability min(1, p / q)
//! off draft row `qi`'s recorded q, and both draw the row where the draft falls (never the cut draft; the residual under rejection)
[hot_path, arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#mtp-accept-modes")]   // reached through the registry once per verify row
def private mtp_judge(var s : Session; var rows : array<float>; off, vocab, draft, qi : int64; accept : MtpAccept) : int64 {
    let params = deref(s.spec_params)
    if (draft < 0l || s.constrain != null || params.temp <= 0.0 || accept == MtpAccept.exact) {
        return sample_logits_row(s, params, rows, off, vocab)
    }
    let rejection = accept == MtpAccept.rejection
    if (sel_landed(s, off / vocab)) {
        return judge_cands_(s, qi, draft, rejection) if (landed_row_(s, params, sel_row_of(s, off / vocab), vocab))
        sel_fallback_(s, rows, vocab)
    }
    apply_penalties_(s, params, rows, off)
    if (params.top_k > 0l && params.top_k < vocab && params.top_k <= SAMPLE_TOPK_FAST_CAP) {
        select_top_k_(s, rows, off, vocab, params.top_k)
        cut_candidates_(s, params)
        return judge_cands_(s, qi, draft, rejection)
    }
    cut_row_reference_(s, params, rows, off, vocab)
    let pd = rows[off + draft]
    if (!rejection) {
        return pd > 0.0 ? draft : draw_row_reference_(s, rows, off, vocab)
    }
    var total = 0.0
    for (i in range64(vocab)) {
        total += rows[off + i]
    }
    return draft if (rejection_keeps_(s, qi, draft, pd, total))
    let mass = residual_weights_(s, qi, total, vocab, $(i : int64) : int64 { return i; }, $(i : int64) : float { return rows[off + i]; })
    return draw_row_reference_(s, rows, off, vocab) if (mass <= 0.0)
    return cdf_draw_(s, vocab) $(i : int64) : float { return s.sample_scratch[i] }
}

//! the judge over `s.cand` (post-cut probabilities): typical keeps `draft` where the cuts kept it, rejection with probability
//! min(1, p / q); else one draw over the survivors (the residual under rejection)
def private judge_cands_(var s : Session; qi, draft : int64; rejection : bool) : int64 {
    let ci = cand_index_(s, draft)
    let pd = ci >= 0 ? s.cand[ci].v : 0.0
    if (!rejection) {
        return pd > 0.0 ? draft : draw_candidates_(s)
    }
    var total = 0.0
    for (c in s.cand) {
        total += c.v
    }
    return draft if (rejection_keeps_(s, qi, draft, pd, total))
    let n = long_length(s.cand)
    let mass = residual_weights_(s, qi, total, n, $(i : int64) : int64 { return s.cand[i].id; }, $(i : int64) : float { return s.cand[i].v; })
    return draw_candidates_(s) if (mass <= 0.0)
    let at = cdf_draw_(s, n) $(i : int64) : float { return s.sample_scratch[i] }
    return s.cand[at].id
}

//! the rejection test: `draft` with mass `pd` of `total` under the row's p kept with probability min(1, p / q), one draw of the rng
def private rejection_keeps_(var s : Session; qi, draft : int64; pd, total : float) : bool {
    let qd = mtp_q_of(s, qi, draft)
    let u = random_float(s.rng)
    return pd > 0.0 && u * qd * total < pd
}

//! the residual max(0, p - q) of draft row `qi` over `n` survivors ascending by id (`id(i)` with mass `p(i)` of `total`) into
//! `s.sample_scratch[0 .. n)`, one merge pass over both lists; returns the residual's mass (0 = the row holds q whole)
def private residual_weights_(var s : Session; qi : int64; total : float; n : int64; id : block<(i : int64) : int64>; p : block<(i : int64) : float>) : float {
    let inv = total > 0.0 ? 1.0 / total : 0.0
    let base = qi * MTP_Q_CAP
    let qn = s.mtp_qn[qi]
    var sum = 0.0
    var j = 0l
    for (i in range64(n)) {
        let pi = p |> invoke(i)
        if (pi <= 0.0) {
            s.sample_scratch[i] = 0.0
            continue
        }
        let ci = id |> invoke(i)
        while (j < qn && s.mtp_q[base + j].id < ci) {
            j++
        }
        let q = j < qn && s.mtp_q[base + j].id == ci ? s.mtp_q[base + j].v : 0.0
        let w = max(0.0, pi * inv - q)
        s.sample_scratch[i] = w
        sum += w
    }
    return sum
}

//! `s.cand` (probabilities in `v`, cut entries at 0, id ascending) recorded as draft row `qi`'s q: the first MTP_Q_CAP positive
//! entries renormalized to one over themselves (ties past the cap are dropped from q, as `draw_q_` never draws them); returns the entries recorded
def private record_q_(var s : Session; qi : int64) : int64 {
    let base = qi * MTP_Q_CAP
    s.mtp_q |> grow_resize(base + MTP_Q_CAP)
    s.mtp_qn |> grow_resize(qi + 1l)
    var total = 0.0
    var n = 0l
    for (c in s.cand) {
        if (c.v > 0.0 && n < MTP_Q_CAP) {
            total += c.v
            s.mtp_q[base + n] = SampleCand(id = c.id, v = c.v)
            n++
        }
    }
    let inv = total > 0.0 ? 1.0 / total : 0.0
    for (j in range64(n)) {
        s.mtp_q[base + j].v *= inv
    }
    s.mtp_qn[qi] = n
    return n
}

//! one draw from draft row `qi`'s recorded q
def private draw_q_(var s : Session; qi : int64) : int64 {
    let base = qi * MTP_Q_CAP
    let at = cdf_draw_(s, s.mtp_qn[qi]) $(i : int64) : float { return s.mtp_q[base + i].v }
    return s.mtp_q[base + at].id
}

[hot_path, arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#mtp-accept-modes")]   //! the rejection-sampling round's drafter, reached through the registry once per draft
def private mtp_draft_draw(var s : Session; var logits : array<float>; vocab, qi : int64) : int64 {
    let params = deref(s.spec_params)
    if (params.temp <= 0.0 || s.constrain != null) {
        let d = sample_logits_row(s, params, logits, 0l, vocab)
        s.cand |> clear()
        s.cand |> push(SampleCand(id = d, v = 1.0))
        record_q_(s, qi)
        return d
    }
    let fast = params.top_k > 0l && params.top_k < vocab && params.top_k <= SAMPLE_TOPK_FAST_CAP
    if (sel_landed(s, 0l)) {
        if (landed_row_(s, params, sel_row_of(s, 0l), vocab)) {
            if (!fast) {
                q_cands_cap_(s)
            }
            record_q_(s, qi)
            return draw_q_(s, qi)
        }
        sel_fallback_(s, logits, vocab)
    }
    apply_penalties_(s, params, logits, 0l)
    if (fast) {
        select_top_k_(s, logits, 0l, vocab, params.top_k)
        cut_candidates_(s, params)
    } else {
        cut_row_reference_(s, params, logits, 0l, vocab)
        var alive = 0l
        for (i in range64(vocab)) {
            if (logits[i] > 0.0) {
                alive++
            }
        }
        select_top_k_(s, logits, 0l, vocab, min(alive, MTP_Q_CAP))
    }
    record_q_(s, qi)
    return draw_q_(s, qi)
}

//! `s.cand` (post-cut probabilities) narrowed to its MTP_Q_CAP largest survivors, ties kept, id ascending - the vocab-wide
//! path's q over the landing, as the row path's `select_top_k_` over the cut row keeps them
def private q_cands_cap_(var s : Session) {
    var alive = 0l
    for (c in s.cand) {
        if (c.v > 0.0) {
            alive++
        }
    }
    return if (alive <= MTP_Q_CAP)
    keep_cands_from_(s, cand_kth_(s, int(MTP_Q_CAP)))
}

[init]
def private sampling_init {
    register_mtp_judge(@@mtp_judge)
    register_mtp_draft_draw(@@mtp_draft_draw)
}

//! Stream-generate up to `max_tokens` from `prompt`, invoking `blk(id, piece)` per token; return
//! false from the block to stop early. Returns the number of tokens emitted.
def generate_(t : Model; var s : Session; prompt : array<int64>; params : SamplingParams;
              max_tokens : int64; blk : block<(id : int64; piece : string) : bool>) : int64 {
    s.recent |> clear()
    let t_pre = ref_time_ticks()
    eval_(t, s, prompt)
    s.stat_prefill_us = int64(get_time_usec(t_pre))
    s.stat_n_prefill = long_length(prompt)
    var one : array<int64>
    one |> resize(1)
    var nout = 0l
    var got_first = false
    let t_gen = ref_time_ticks()
    for (_step in range64(max_tokens)) {
        let tok = sample_(s, params)
        if (!got_first) {   // time-to-first-token spans the prefill + the first forward
            s.stat_ttft_us = int64(get_time_usec(t_pre))
            got_first = true
        }
        if (tok < 0l || !invoke(blk, tok, piece_(t, tok))) {   // a constraint's dead end, or the caller's stop
            break
        }
        nout++
        s.recent |> push(tok)
        one[0] = tok
        eval_(t, s, one)
    }
    s.stat_gen_us = int64(get_time_usec(t_gen))
    s.stat_n_gen = nout
    return nout
}

//! generate_'s embedding-prefill twin: prefill `npos` pre-built rows, then stream-sample like
//! generate_. Rows [span_lo, span_hi) prefill as a NON-CAUSAL image span (span_hi <= span_lo
//! = plain causal); a non-zero merged `grid` routes the span through `eval_embd_span_mrope_`.
def generate_embd_(t : Model; var s : Session; embd : array<float>; npos : int64; params : SamplingParams;
                   max_tokens : int64; span_lo : int64 = 0l; span_hi : int64 = 0l; grid : int2 = int2(0);
                   blk : block<(id : int64; piece : string) : bool>) : int64 {
    s.recent |> clear()
    let t_pre = ref_time_ticks()
    eval_embd_body_(t, s, embd, npos, span_lo, span_hi, grid)
    s.stat_prefill_us = int64(get_time_usec(t_pre))
    s.stat_n_prefill = npos
    var one : array<int64>
    one |> resize(1)
    var nout = 0l
    var got_first = false
    let t_gen = ref_time_ticks()
    for (_step in range64(max_tokens)) {
        let tok = sample_(s, params)
        if (!got_first) {   // time-to-first-token spans the prefill + the first forward
            s.stat_ttft_us = int64(get_time_usec(t_pre))
            got_first = true
        }
        if (tok < 0l || !invoke(blk, tok, piece_(t, tok))) {   // a constraint's dead end, or the caller's stop
            break
        }
        nout++
        s.recent |> push(tok)
        one[0] = tok
        eval_(t, s, one)
    }
    s.stat_gen_us = int64(get_time_usec(t_gen))
    s.stat_n_gen = nout
    return nout
}

// ===== Generation stats =====

//! A summary of the most recent generate()/respond() call's timing. Times are seconds; rates tok/s.
struct Stats {
    n_prompt : int64 = 0l        // prompt tokens prefilled
    n_gen : int64 = 0l           // tokens generated
    ttft_s : double = 0.0lf      // time to first token (prefill + first forward)
    prefill_tps : double = 0.0lf // prompt tokens / prefill seconds
    gen_tps : double = 0.0lf     // generated tokens / generation seconds
}

//! Summarize the last generate()/respond() call from the session's timing counters.
def stats_(s : Session) : Stats {
    var r = Stats(n_prompt = s.stat_n_prefill, n_gen = s.stat_n_gen,
                  ttft_s = double(s.stat_ttft_us) / 1.0e6lf)
    if (s.stat_prefill_us > 0l) {
        r.prefill_tps = double(s.stat_n_prefill) * 1.0e6lf / double(s.stat_prefill_us)
    }
    if (s.stat_gen_us > 0l) {
        r.gen_tps = double(s.stat_n_gen) * 1.0e6lf / double(s.stat_gen_us)
    }
    return r
}

