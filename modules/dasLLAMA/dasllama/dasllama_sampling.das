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
require math

// Token sampling and the generation drivers on top of forward()/eval_batch. The engine reaches
// this file through one registry only - the accept walk's row sampler installed at [init]; the
// umbrella re-exports it so every consumer of dasllama/dasllama keeps generate_/sample_/set_seed_/stats_.

// ===== Sampling =====

//! Seed the session's sampling RNG for reproducible generation.
def set_seed_(var s : Session; seed : int) {
    s.rng = random_seed(seed)
}

//! penalties over the last `penalty_last_n` of `s.recent` onto the row at `off` in `row`: each unique token once, multiplicative then presence + frequency*count
def private apply_penalties_(s : Session; params : SamplingParams; var row : array<float>; off : int64) {
    if (penalties_off(params)) {
        return
    }
    let rep = params.penalty > 0.0 && params.penalty != 1.0
    let n = length(s.recent)
    let win = int(params.penalty_last_n)
    let pstart = n > win ? n - win : 0
    for (k in range(pstart, n)) {
        let id = s.recent[k]
        var seen = false
        for (j in range(pstart, k)) {
            if (s.recent[j] == id) {
                seen = true
                break
            }
        }
        continue if (seen)
        var cnt = 1
        for (j in range(k + 1, n)) {
            if (s.recent[j] == id) {
                cnt++
            }
        }
        var v = row[off + id]
        if (rep) {
            v = v > 0.0 ? v / params.penalty : v * params.penalty
        }
        v -= params.presence_penalty + params.frequency_penalty * float(cnt)
        row[off + id] = v
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
    apply_penalties_(s, params, row, off)
    if (s.constrain != null && params.temp > 0.0 && off == 0l) {
        // a constrained draw over the k largest logits (a top-k wider than the cap, or none, reads as the cap): the
        // draw goes first, unconstrained, so a pick the constraint admits costs one probe; a refused pick refilters
        // to the admitted candidates and draws over them alone; no admitted survivor falls to the row's best admitted
        let k = params.top_k > 0l ? min(params.top_k, SAMPLE_TOPK_FAST_CAP) : SAMPLE_TOPK_FAST_CAP
        select_top_k_(s, row, off, vocab, min(k, vocab))
        s.cand_full |> reserve(length(s.cand))
        s.cand_full |> resize(length(s.cand))
        for (c, i in s.cand, count()) {
            s.cand_full[i] = c
        }
        let first = sample_candidates_(s, params)
        if (s.constrain->admits(first)) {
            s.constrain->commit(first)
            return first
        }
        var kept = 0
        for (c in s.cand_full) {
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
        for (c in s.cand) {
            if (c.v < cut) {
                c.v = 0.0
            }
        }
        sort(s.cand) $(x, y : SampleCand) : bool { return x.id < y.id }
    }
    if (params.min_p > 0.0) {
        var maxp = 0.0
        for (c in s.cand) {
            maxp = max(maxp, c.v)
        }
        let thr = min(params.min_p, 1.0) * maxp
        for (c in s.cand) {
            if (c.v < thr) {
                c.v = 0.0
            }
        }
    }
    var total = 0.0
    for (c in s.cand) {
        total += c.v
    }
    let r = random_float(s.rng) * total
    var cdf = 0.0
    for (c in s.cand) {
        cdf += c.v
        if (r < cdf) {
            return c.id
        }
    }
    return s.cand[length(s.cand) - 1].id
}

//! The vocab-wide sampler: temperature over the row, top-k as k max passes, softmax over the row,
//! the nucleus cut over a sorted copy, min-p, and a CDF walk over the row. Serves a top-k the
//! candidate list does not (off, or past SAMPLE_TOPK_FAST_CAP) and is the fast path's oracle.
def sample_logits_row_reference(var s : Session; params : SamplingParams; var row : array<float>; off, vocab : int64) : int64 { // nolint:STYLE037,STYLE038 — a flat dispatch over the sampling modes; each arm is independent and splitting it just hides the menu
    if (params.temp <= 0.0) {
        var mi = 0l
        for (i in range64(1l, vocab)) {
            if (row[off + i] > row[off + mi]) {
                mi = i
            }
        }
        return mi
    }
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
        // nucleus cut: sort a prob copy descending-by-walk, find the smallest prefix whose mass
        // reaches top_p, zero everything below that prefix's boundary probability
        for (i in range64(vocab)) {
            s.sample_scratch[i] = row[off + i]
        }
        sort(s.sample_scratch)
        let target = max(params.top_p, 0.0)
        var mass = 0.0
        var cut = 0.0
        var idx = length(s.sample_scratch) - 1
        while (idx >= 0) {
            cut = s.sample_scratch[idx]
            mass += cut
            break if (mass >= target)
            idx--
        }
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
    // the cuts above zero probabilities without renormalizing — draw against the surviving mass
    // (cdf accumulates in the same order as total, so the walk always lands on a surviving token)
    var total = 0.0
    for (i in range64(vocab)) {
        total += row[off + i]
    }
    let r = random_float(s.rng) * total
    var cdf = 0.0
    for (i in range64(vocab)) {
        cdf += row[off + i]
        if (r < cdf) {
            return i
        }
    }
    return vocab - 1l
}

[hot_path]   // the accept walk's row sampler, reached through the registry once per verify row
def private mtp_sample_row(var s : Session; var rows : array<float>; off, vocab : int64) : int64 =>
    sample_logits_row(s, deref(s.spec_params), rows, off, vocab)

[init]
def private sampling_init {
    register_mtp_sample_row(@@mtp_sample_row)
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

