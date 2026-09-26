options gen2
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require daslib/jobque_boost
require daslib/fio
require daslib/random
require math
require _model_tier

// Phase-3 sampling API. Greedy (SamplingParams default: temp=0, penalty=1.0) must reproduce the
// frozen greedy oracle bit-for-bit; temp>0 sampling must be deterministic under a fixed seed.
// GGUF models are gitignored, so each case skips cleanly when its model is absent.

// gemma-2-2b-it Q8_0, "1, 2, 3, 4, 5, 6, 7," (BOS=2) — same frozen oracle as test_parity.das
// (re-frozen to the counting prompt with the cls_q8 tied classifier; see the note there).
let GEMMA2_PROMPT <- [2l, 235274l, 235269l, 235248l, 235284l, 235269l, 235248l, 235304l, 235269l, 235248l,
                      235310l, 235269l, 235248l, 235308l, 235269l, 235248l, 235318l, 235269l, 235248l, 235324l,
                      235269l]
let GEMMA2_GEN <- [235248l, 235321l, 235269l, 235248l, 235315l, 235269l, 235248l, 235274l, 235276l, 235269l,
                   235248l, 235274l, 235274l, 235269l, 235248l, 235274l, 235284l, 235269l, 235248l, 235274l,
                   235304l, 235269l, 235248l, 235274l, 235310l, 235269l, 235248l, 235274l, 235308l, 235269l,
                   235248l, 235274l, 235318l, 235269l, 235248l, 235274l, 235324l, 235269l, 235248l, 235274l]

[test]
def test_sampling_greedy_matches_oracle(t : T?) {
    t |> run("greedy sampling generate reproduces the gemma2 oracle") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_model_(path, QuantMode.q8)
        // Token-exact oracle gate: pin bit-exact classic attention + scalar activations (same reason
        // as test_parity — the flash / exp4 defaults are within tolerance but not bit-exact).
        let pa = get_prefill_attn_mode()
        set_prefill_attn_mode(AttnPrefillMode.classic)
        let pv = get_act_vec()
        set_act_vec(false)
        var got : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            var s = create_session(m, KVDtype.f32)   // frozen f32 oracle — the facade default is f16
            generate(m, s, GEMMA2_PROMPT, SamplingParams(), long_length(GEMMA2_GEN)) $(id, piece) {
                got |> push(id)
                return true
            }
            delete s
        }
        delete m
        set_prefill_attn_mode(pa)
        set_act_vec(pv)
        t |> success(ids_equal(got, GEMMA2_GEN), "greedy generate token-for-token vs oracle")
    }
}

// ===== Model-free sampler units =====

// sample() only touches logits / sample_scratch / recent / rng, so a bare Session with hand-set
// logits exercises every cutoff and penalty without a model (and without JIT gating).
def private mk_sampler(n : int) : Session {
    var s = Session()
    s.logits |> resize(n)
    s.sample_scratch |> resize(n)
    set_seed(s, 12345)
    return <- s
}

def private set_logits(var s : Session; vals : array<float>) {
    for (i in range(length(vals))) {
        s.logits[i] = vals[i]
    }
}

[test]
def test_sampler_units(t : T?) {   // nolint:STYLE038 — flat unit list, one t |> run cell per sampler knob
    t |> run("defaults are greedy argmax") @(t : T?) {
        var s = mk_sampler(4)
        set_logits(s, [0.5, 3.0, 1.0, 2.0])
        t |> equal(sample(s, SamplingParams()), 1l)
        delete s
    }
    t |> run("top_p cuts the tail to the dominant token") @(t : T?) {
        var s = mk_sampler(4)
        let params = SamplingParams(temp = 1.0, top_p = 0.5)
        for (_i in range(64)) {
            set_logits(s, [10.0, 1.0, 1.0, 1.0])
            t |> equal(sample(s, params), 0l)
        }
        delete s
    }
    t |> run("top_p keeps the top pair, drops the tail") @(t : T?) {
        var s = mk_sampler(4)
        let params = SamplingParams(temp = 1.0, top_p = 0.9)
        var seen0 = 0
        var seen1 = 0
        for (_i in range(256)) {
            set_logits(s, [5.0, 5.0, 0.0, -1.0])
            let id = sample(s, params)
            t |> success(id == 0l || id == 1l, "sampled inside the nucleus")
            if (id == 0l) {
                seen0++
            } elif (id == 1l) {
                seen1++
            }
        }
        t |> success(seen0 > 0 && seen1 > 0, "both nucleus tokens are reachable")
        delete s
    }
    t |> run("min_p drops tokens below the relative floor") @(t : T?) {
        var s = mk_sampler(3)
        let params = SamplingParams(temp = 1.0, min_p = 0.5)
        for (_i in range(128)) {
            set_logits(s, [5.0, 5.0, 0.0])
            t |> success(sample(s, params) != 2l, "below-floor token never sampled")
        }
        delete s
    }
    t |> run("presence penalty moves greedy off a recent token") @(t : T?) {
        var s = mk_sampler(8)
        s.recent |> push(3l)
        set_logits(s, [0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, SamplingParams(presence_penalty = 1.0)), 7l)
        set_logits(s, [0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, SamplingParams()), 3l)   // no penalty: greedy stays on the recent token
        delete s
    }
    t |> run("frequency penalty scales with occurrence count") @(t : T?) {
        var s = mk_sampler(8)
        let params = SamplingParams(frequency_penalty = 0.3)
        s.recent <- [3l, 3l, 3l]
        // 3 occurrences: 2.0 - 3 * 0.3 = 1.1 < 1.5 => argmax moves to 7
        set_logits(s, [0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, params), 7l)
        // 1 occurrence: 2.0 - 0.3 = 1.7 > 1.5 => argmax stays on 3
        s.recent |> clear()
        s.recent |> push(3l)
        set_logits(s, [0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, params), 3l)
        delete s
    }
    t |> run("repeat penalty <= 0 is treated as off, not a divide-by-zero") @(t : T?) {
        var s = mk_sampler(8)
        s.recent |> push(3l)
        // additive-penalty clients send 0 meaning "off" — greedy must stay on the recent argmax
        set_logits(s, [0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, SamplingParams(penalty = 0.0)), 3l)
        set_logits(s, [0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, SamplingParams(penalty = -1.0)), 3l)
        delete s
    }
    t |> run("repetition penalty applies once per unique token (llama.cpp semantics)") @(t : T?) {
        var s = mk_sampler(8)
        s.recent <- [3l, 3l, 3l]
        // once: 3.0 / 1.5 = 2.0 > 1.5 => argmax stays on 3; per-occurrence (the old behavior)
        // would give 3.0 / 1.5^3 = 0.89 and move to 7
        set_logits(s, [0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 1.5])
        t |> equal(sample(s, SamplingParams(penalty = 1.5)), 3l)
        delete s
    }
    t |> run("seeded sampled draws reproduce") @(t : T?) {
        let params = SamplingParams(temp = 1.0, top_p = 0.95, min_p = 0.05, presence_penalty = 0.5)
        var a : array<int64>
        var b : array<int64>
        for (pass_idx in range(2)) {
            var s = mk_sampler(16)
            set_seed(s, 777)
            s.recent |> reserve(32)
            for (_i in range(32)) {
                for (i in range(16)) {
                    s.logits[i] = float(i % 5)
                }
                let id = sample(s, params)
                if (pass_idx == 0) {
                    a |> push(id)
                } else {
                    b |> push(id)
                }
                s.recent |> push(id)
            }
            delete s
        }
        t |> success(ids_equal(a, b) && length(a) == 32, "same seed => identical draws")
    }
}

// ===== The candidate-list fast path against the vocab-wide reference =====

let PARITY_VOCAB = 4096
let PARITY_ROWS = 512
let REF_MASKED_MASS = 1.0e-30   // above exp(-80) * inv_sum, below any survivor at this vocab

//! A seeded logits row; every other row is quantized to a 0.5 grid so ties at the k-th value and
//! at the nucleus cut occur by construction.
def private parity_row(trial : int) : array<float> {
    var rng = random_seed(9000 + trial)
    var row : array<float>
    row |> resize(PARITY_VOCAB)
    for (i in range(PARITY_VOCAB)) {
        let v = random_float(rng) * 20.0 - 10.0
        row[i] = (trial % 2 == 0) ? v : float(int(v * 2.0)) * 0.5
    }
    return <- row
}

//! The draw the reference makes on `row` under `seed`: `r` on the reference's own CDF, and its
//! distance to the nearest CDF boundary as a fraction of the surviving mass.
def private ref_draw_margin(seed : int; probs : array<float>; picked : int64) : float {
    var rng = random_seed(seed)
    var total = 0.0
    for (p in probs) {
        total += p
    }
    let r = random_float(rng) * total
    var cdf_lo = 0.0
    for (i in range64(picked)) {
        cdf_lo += probs[i]
    }
    let cdf_hi = cdf_lo + probs[picked]
    return min(r - cdf_lo, cdf_hi - r) / total
}

struct ParityCount {
    rows : int
    clear_rows : int         // draws further than PARITY_MARGIN from a CDF boundary
    wide_clear_rows : int    // the same count at a margin no draw can clear: the floor's control
    mismatches : int
}

let PARITY_MARGIN = 1.0e-5
let PARITY_WIDE_MARGIN = 0.5   // half the mass: no draw sits that far from both boundaries
let PROB_ABS = 1.0e-6
let PROB_REL = 1.0e-5

def private prob_within(got, want : float) : bool => abs(got - want) <= PROB_ABS + PROB_REL * want

[cold_path]   // a test driver over the sampler's entry
def private parity_sweep(t : T?; params : SamplingParams) : ParityCount {
    var pc : ParityCount
    // the tolerance's control: a probability poisoned by more than the bound must miss it
    t |> success(!prob_within(0.25 + 1.0e-4, 0.25), "a 1e-4 poison on a 0.25 probability misses the bound")
    for (trial in range(PARITY_ROWS)) {
        let seed = 100 + trial
        let row <- parity_row(trial)
        var sf = mk_sampler(PARITY_VOCAB)
        var sr = mk_sampler(PARITY_VOCAB)
        set_logits(sf, row)
        set_logits(sr, row)
        set_seed(sf, seed)
        set_seed(sr, seed)
        let id_fast = sample(sf, params)
        let id_ref = sample_logits_row_reference(sr, params, sr.logits, 0l, int64(PARITY_VOCAB))
        // the survivor sets: the reference leaves a probability on every survivor; a token its
        // top-k masked keeps exp(-80) of the mass (the softmax clamp), a nucleus or min-p cut leaves 0
        var n_ref = 0
        for (i in range(PARITY_VOCAB)) {
            if (sr.logits[i] > REF_MASKED_MASS) {
                n_ref++
            }
        }
        var n_fast = 0
        var probs_ok = true
        var worst_gap = 0.0
        var worst_id = -1l
        for (c in sf.cand) {
            if (c.v > 0.0) {
                n_fast++
                let want = sr.logits[c.id]
                let gap = want <= REF_MASKED_MASS ? 1.0 : abs(c.v - want)
                if (gap > worst_gap) {
                    worst_gap = gap
                    worst_id = c.id
                }
                if (want <= REF_MASKED_MASS || !prob_within(c.v, want)) {
                    probs_ok = false
                    to_log(LOG_ERROR, "trial {trial} id {c.id}: fast {c.v} reference {want}\n")
                }
            }
        }
        t |> equal(n_fast, n_ref, "trial {trial}: survivor count")
        t |> success(probs_ok, "trial {trial}: survivor probabilities match the reference (worst gap {worst_gap} at id {worst_id})")
        t |> success(sf.rng == sr.rng, "trial {trial}: one draw on each path leaves the same RNG state")
        pc.rows++
        let margin = ref_draw_margin(seed, sr.logits, id_ref)
        if (margin > PARITY_WIDE_MARGIN) {
            pc.wide_clear_rows++
        }
        if (margin > PARITY_MARGIN) {
            pc.clear_rows++
            if (id_fast != id_ref) {
                pc.mismatches++
                to_log(LOG_ERROR, "trial {trial}: fast path drew {id_fast}, the reference {id_ref} (margin {margin})\n")
            }
        }
        delete sf
        delete sr
    }
    return pc
}

[test]
def test_fast_path_parity(t : T?) {
    t |> run("the chat preset: temp 0.7, top-k 20, top-p 0.95") @(t : T?) {
        let pc = parity_sweep(t, SamplingParams(temp = 0.7, top_k = 20l, top_p = 0.95))
        t |> equal(pc.rows, PARITY_ROWS)
        t |> success(pc.clear_rows >= 500, "{pc.clear_rows} rows draw clear of a CDF boundary")
        t |> success(pc.wide_clear_rows < 500, "the floor's control: {pc.wide_clear_rows} rows clear a half-mass margin")
        t |> equal(pc.mismatches, 0, "the same token on every clear row")
    }
    t |> run("temp 1.0, top-k 40, top-p 0.9, min-p 0.05") @(t : T?) {
        let pc = parity_sweep(t, SamplingParams(temp = 1.0, top_k = 40l, top_p = 0.9, min_p = 0.05))
        t |> success(pc.clear_rows >= 500, "{pc.clear_rows} rows draw clear of a CDF boundary")
        t |> success(pc.wide_clear_rows < 500, "the floor's control: {pc.wide_clear_rows} rows clear a half-mass margin")
        t |> equal(pc.mismatches, 0, "the same token on every clear row")
    }
    t |> run("top-k alone keeps every survivor") @(t : T?) {
        let pc = parity_sweep(t, SamplingParams(temp = 0.8, top_k = 40l))
        t |> equal(pc.mismatches, 0, "the same token on every clear row")
    }
}

[test]
def test_fast_path_ties(t : T?) {
    t |> run("k+3 equal top logits all survive a top-k of k") @(t : T?) {
        let k = 20
        var s = mk_sampler(256)
        for (i in range(256)) {
            s.logits[i] = i < k + 3 ? 5.0 : -1.0 - float(i % 7)
        }
        let params = SamplingParams(temp = 1.0, top_k = int64(k))
        var seen : table<int64; void>
        for (draw in range(400)) {
            for (i in range(256)) {
                s.logits[i] = i < k + 3 ? 5.0 : -1.0 - float(i % 7)
            }
            let id = sample(s, params)
            t |> success(id < int64(k + 3), "draw {draw} lands inside the tie set")
            seen |> insert(id)
            if (draw == 0) {
                t |> equal(length(s.cand), k + 3, "every tied token is a candidate")
                for (c in s.cand) {
                    t |> success(c.v > 0.0, "candidate {c.id} keeps a probability")
                }
            }
        }
        t |> equal(length(seen), k + 3, "every tied token is drawn over 400 draws")
        delete s
    }
    t |> run("a top-k past the fast cap takes the reference path") @(t : T?) {
        var s = mk_sampler(2048)
        for (i in range(2048)) {
            s.logits[i] = float(i % 13)
        }
        let id = sample(s, SamplingParams(temp = 1.0, top_k = 1500l))
        t |> success(id >= 0l && id < 2048l, "a token was drawn")
        t |> equal(length(s.cand), 0, "the candidate list stays empty on the reference path")
        delete s
    }
}

[test]
def test_sampling_deterministic(t : T?) {
    t |> run("same seed => identical sampled output") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_model_(path, QuantMode.q8)
        let params = SamplingParams(temp = 0.8, top_k = 40l)
        let n = 16l
        var a : array<int64>
        var b : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            var s1 = create_session(m)
            set_seed(s1, 12345)
            generate(m, s1, GEMMA2_PROMPT, params, n) $(id, piece) {
                a |> push(id)
                return true
            }
            var s2 = create_session(m)
            set_seed(s2, 12345)
            generate(m, s2, GEMMA2_PROMPT, params, n) $(id, piece) {
                b |> push(id)
                return true
            }
            delete s1
            delete s2
        }
        delete m
        t |> success(ids_equal(a, b) && long_length(a) == n, "reproducible under a fixed seed")
    }
}
