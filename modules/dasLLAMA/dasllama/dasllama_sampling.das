options gen2
options _comment_hygiene = true

module dasllama_sampling shared public

require dasllama/dasllama_common
require dasllama/dasllama_math   // softmax/softcap on the logits row
require daslib/random
require math

// Token sampling and the generation drivers. Nothing in the engine calls back into this — it sits
// on top of forward()/eval_batch, which is why it can be a leaf module; the umbrella re-exports it
// so every consumer of dasllama/dasllama keeps generate_/sample_/set_seed_/stats_.

// ===== Sampling =====

//! Sampling parameters. Defaults are greedy (temp = 0, all cutoffs/penalties off) — bit-identical
//! to argmax. Cutoff order matches llama.cpp: top-k on logits, then top-p and min-p on the
//! softmaxed probabilities.
struct SamplingParams {
    temp : float = 0.0             // <= 0 => greedy argmax; otherwise softmax temperature
    top_k : int64 = 0l             // <= 0 or >= vocab => no top-k cutoff
    top_p : float = 1.0            // nucleus: keep the smallest prob mass >= top_p (>= 1 = off)
    min_p : float = 0.0            // drop tokens with prob < min_p * max prob (<= 0 = off)
    penalty : float = 1.0          // multiplicative repetition penalty over recent tokens (1 = none; <= 0 treated as none)
    presence_penalty : float = 0.0   // subtracted from the logit of every distinct token in the window
    frequency_penalty : float = 0.0  // subtracted per occurrence in the window (OpenAI semantics)
    penalty_last_n : int64 = 64l   // penalty window (most recent N generated tokens)
}

//! Seed the session's sampling RNG for reproducible generation.
def set_seed_(var s : Session; seed : int) {
    s.rng = random_seed(seed)
}

// Repetition/presence/frequency penalties over the last `penalty_last_n` of `s.recent` (llama.cpp
// semantics): each unique token penalized once, multiplicative then presence+frequency*count.
def private apply_penalties_(var s : Session; params : SamplingParams) {
    // <= 0 treated as off too (not just 1): 0 would inf/NaN the logits, and additive-penalty clients send it
    let rep = params.penalty > 0.0 && params.penalty != 1.0
    if (!rep && params.presence_penalty == 0.0 && params.frequency_penalty == 0.0) {
        return
    }
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
        var v = s.logits[id]
        if (rep) {
            v = v > 0.0 ? v / params.penalty : v * params.penalty
        }
        v -= params.presence_penalty + params.frequency_penalty * float(cnt)
        s.logits[id] = v
    }
}

//! Sample the next token from `s.logits` per `params`: penalties, then temp + top-k, softmax,
//! top-p/min-p, and a CDF draw (or greedy argmax when temp <= 0). Consumes `s.logits` in place.
def sample_(var s : Session; params : SamplingParams) : int64 { // nolint:STYLE037 — a flat dispatch over the sampling modes; each arm is independent and splitting it just hides the menu
    let vocab = long_length(s.logits)
    apply_penalties_(s, params)
    if (params.temp <= 0.0) {
        return argmax(s, vocab)
    }
    let inv = 1.0 / params.temp
    for (i in range64(vocab)) {
        s.logits[i] *= inv
    }
    if (params.top_k > 0l && params.top_k < vocab) {
        for (i in range64(vocab)) {
            s.sample_scratch[i] = s.logits[i]
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
            if (s.logits[i] < kth) {
                s.logits[i] = -1.0e30
            }
        }
    }
    softmax(s.logits, vocab)
    if (params.top_p < 1.0) {
        // nucleus cut: sort a prob copy descending-by-walk, find the smallest prefix whose mass
        // reaches top_p, zero everything below that prefix's boundary probability
        for (i in range64(vocab)) {
            s.sample_scratch[i] = s.logits[i]
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
            if (s.logits[i] < cut) {
                s.logits[i] = 0.0
            }
        }
    }
    if (params.min_p > 0.0) {
        var maxp = 0.0
        for (i in range64(vocab)) {
            maxp = max(maxp, s.logits[i])
        }
        let thr = min(params.min_p, 1.0) * maxp
        for (i in range64(vocab)) {
            if (s.logits[i] < thr) {
                s.logits[i] = 0.0
            }
        }
    }
    // the cuts above zero probabilities without renormalizing — draw against the surviving mass
    // (cdf accumulates in the same order as total, so the walk always lands on a surviving token)
    var total = 0.0
    for (i in range64(vocab)) {
        total += s.logits[i]
    }
    let r = random_float(s.rng) * total
    var cdf = 0.0
    for (i in range64(vocab)) {
        cdf += s.logits[i]
        if (r < cdf) {
            return i
        }
    }
    return vocab - 1l
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
        if (!invoke(blk, tok, piece_(t, tok))) {
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

//! generate_'s embedding-prefill twin: prefill `npos` pre-built embedding rows (the multimodal
//! splice — see eval_embd_), then stream-sample exactly like generate_. The chat layer's audio
//! turns run through this.
def generate_embd_(t : Model; var s : Session; embd : array<float>; npos : int64; params : SamplingParams;
                   max_tokens : int64; blk : block<(id : int64; piece : string) : bool>) : int64 {
    s.recent |> clear()
    let t_pre = ref_time_ticks()
    eval_embd_(t, s, embd, npos)
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
        if (!invoke(blk, tok, piece_(t, tok))) {
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

