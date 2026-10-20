// The bare session the model-free accept-walk cells run on: a vocabulary-wide logits row, the sampler scratch and the
// verify rows of a round at `drafts` deep - everything a walk touches and nothing a model loads - and the RNG-state
// compare that counts a walk's draws.

options gen2
options _dasllama_internal = true

module _mtp_walk_fixture shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // Session
require daslib/random   // random_seed, random_int

//! a bare session over a `vocab`-token vocabulary carrying a round's `drafts` + 1 verify rows
def bare_session(vocab, drafts : int64) : Session {
    var s : Session
    s.logits |> resize(vocab)
    s.sample_scratch |> resize(vocab)
    s.mtp_vbatch |> resize(drafts + 1l)
    s.mtp_logits_b |> reserve((drafts + 1l) * vocab)
    s.mtp_logits_b |> resize((drafts + 1l) * vocab)
    return <- s
}

//! two RNG states word for word: a walk that drew once leaves the state one draw past the other's
def same_rng(a, b : int4) : bool => a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w

//! an RNG seeded at a spread of `seed` (times a large prime) and drawn eight times: seeded at a small seed the generator's first
//! draw grows with the seed (under 0.05 over the first few hundred) and some later draw's halves split 295 : 105 across
//! consecutive seeds, so a cell counting accepts over seeds 1..400 reads a uniform state only off a spread seed
def seeded_rng(seed : int) : int4 {
    var rng = random_seed(seed * 1000003)
    for (_i in range(8)) {
        random_int(rng)
    }
    return rng
}
