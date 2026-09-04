options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common     // Session, SamplingParams, mtp_walk_sampled / mtp_walk_samples
require dasllama/dasllama_sampling   // sample_, and the [init] that installs the walk's row sampler
require daslib/random

// The sampled accept walk is host arithmetic over the verify rows, so its reference is plain
// sampled decode over the same rows: sample_ per row, the same seed, one draw per emitted token.
// A bare session over an 8-token vocabulary carries everything the walk touches.

let VOCAB = 8l
let K = 3l

def private bare() : Session {
    var s : Session
    s.logits |> resize(VOCAB)
    s.sample_scratch |> resize(VOCAB)
    s.mtp_vbatch |> resize(K + 1l)
    s.mtp_logits_b |> resize((K + 1l) * VOCAB)
    return <- s
}

def private set_row(var s : Session; row : int64; vals : array<float>) {
    for (i in range64(VOCAB)) {
        s.mtp_logits_b[row * VOCAB + i] = vals[i]
    }
}

def private same_rng(a, b : int4) : bool {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w
}

// plain sampled decode over the same rows: the emitted tokens, and where the drafts stop matching
def private reference_walk(var r : Session; params : SamplingParams; drafts : array<int64>; var a : int64&) : int64 {
    a = 0l
    for (i in range64(K + 1l)) {
        for (j in range64(VOCAB)) {
            r.logits[j] = r.mtp_logits_b[i * VOCAB + j]
        }
        let d = sample_(r, params)
        if (i < K && d == drafts[i + 1l]) {
            r.recent |> push(d)
            a++
            continue
        }
        return d
    }
    return -1l
}

[test]
def test_sampled_walk_matches_plain_sampled_decode(t : T?) {
    // every draft is the only token with mass on its row: the walk must accept all three and draw
    // the bonus row - four RNG draws, the same four the plain decode makes
    var s <- bare()
    var r <- bare()
    let drafts <- [0l, 3l, 5l, 1l]
    var rows <- [[-1.0e30, -1.0e30, -1.0e30, 4.0, -1.0e30, -1.0e30, -1.0e30, -1.0e30],
                 [-1.0e30, -1.0e30, -1.0e30, -1.0e30, -1.0e30, 4.0, -1.0e30, -1.0e30],
                 [-1.0e30, 4.0, -1.0e30, -1.0e30, -1.0e30, -1.0e30, -1.0e30, -1.0e30],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    for (i in range64(K + 1l)) {
        set_row(s, i, rows[i])
        set_row(r, i, rows[i])
        s.mtp_vbatch[i] = drafts[i]
    }
    var sp = SamplingParams(temp = 0.8)
    s.rng = random_seed(77)
    r.rng = random_seed(77)
    t |> success(!mtp_walk_samples(s), "no sampler pointed at: the walk stays greedy")
    s.spec_params = unsafe(addr(sp))
    t |> success(mtp_walk_samples(s), "a sampler above zero temperature arms the sampled walk")
    var a_ref = 0l
    let next_ref = reference_walk(r, sp, drafts, a_ref)
    let a = mtp_walk_sampled(s, K, VOCAB)
    t |> equal(a, 3l, "every draft accepted")
    t |> equal(a, a_ref, "the accepted run equals the plain decode's")
    t |> success(s.mtp_pre_drawn, "the bonus row's draw is parked for the caller")
    t |> equal(s.mtp_pre_tok, next_ref, "the parked token is the plain decode's fourth draw")
    t |> success(same_rng(s.rng, r.rng), "one RNG draw per emitted token: the states agree after the round")
    t |> equal(length(s.recent), 0, "the recent window is put back for the caller to commit")
    t |> equal(sample_(s, sp), next_ref, "the caller's sample_ returns the parked token")
    t |> success(!s.mtp_pre_drawn, "and clears it")
    delete rows
    delete s
    delete r
}

[test]
def test_sampled_walk_stops_at_the_first_miss(t : T?) {
    // row 1's draft has no mass: the walk accepts draft 1, misses on row 1, and the miss is the next
    // token - two draws, and rows 2 and 3 are never touched
    var s <- bare()
    var r <- bare()
    let drafts <- [0l, 3l, 5l, 1l]
    var rows <- [[-1.0e30, -1.0e30, -1.0e30, 4.0, -1.0e30, -1.0e30, -1.0e30, -1.0e30],
                 [1.0, 2.0, 3.0, 1.0, 2.0, -1.0e30, 1.5, 0.5],
                 [-1.0e30, 4.0, -1.0e30, -1.0e30, -1.0e30, -1.0e30, -1.0e30, -1.0e30],
                 [0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0]]
    for (i in range64(K + 1l)) {
        set_row(s, i, rows[i])
        set_row(r, i, rows[i])
        s.mtp_vbatch[i] = drafts[i]
    }
    var sp = SamplingParams(temp = 0.8, top_k = 4l)
    s.rng = random_seed(5)
    r.rng = random_seed(5)
    s.spec_params = unsafe(addr(sp))
    var a_ref = 0l
    let next_ref = reference_walk(r, sp, drafts, a_ref)
    let a = mtp_walk_sampled(s, K, VOCAB)
    t |> equal(a, 1l, "the first draft is accepted, the second is a miss")
    t |> equal(a, a_ref, "the accepted run equals the plain decode's")
    t |> equal(s.mtp_pre_tok, next_ref, "the miss is the plain decode's second draw")
    t |> success(s.mtp_pre_tok != drafts[2], "and it is not the rejected draft")
    t |> success(same_rng(s.rng, r.rng), "two RNG draws on both sides")
    delete rows
    delete s
    delete r
}

[test]
def test_sampled_walk_penalizes_with_the_accepted_drafts(t : T?) {
    // top-k 1 makes the draw the penalized argmax. Row 1's top token IS the accepted draft 1 and
    // the second draft repeats it; with the window advanced the repetition penalty halves it and
    // the runner-up wins - a walk that forgot the window would accept two drafts here
    var s <- bare()
    let drafts <- [0l, 3l, 3l, 1l]
    var rows <- [[0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0],
                 [0.0, 0.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0],
                 [0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                 [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0]]
    for (i in range64(K + 1l)) {
        set_row(s, i, rows[i])
        s.mtp_vbatch[i] = drafts[i]
    }
    var sp = SamplingParams(temp = 0.7, top_k = 1l, penalty = 2.0)
    s.rng = random_seed(1)
    s.spec_params = unsafe(addr(sp))
    let a = mtp_walk_sampled(s, K, VOCAB)
    t |> equal(a, 1l, "the repeated draft is rejected once the first one sits in the window")
    t |> equal(s.mtp_pre_tok, 2l, "the runner-up is the next token")
    t |> equal(length(s.recent), 0, "the window is put back")
    delete rows
    delete s
}
