options gen2
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options stack = 65536   // dastest harness + generate_ sampler + forward chain sits near the default 16KB; 4x headroom (dasPEG tests do the same)

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require daslib/jobque_boost
require daslib/fio

// Phase-3 sampling API. Greedy (SamplingParams default: temp=0, penalty=1.0) must reproduce the
// frozen greedy oracle bit-for-bit; temp>0 sampling must be deterministic under a fixed seed.
// GGUF models are gitignored, so each case skips cleanly when its model is absent.

// override the machine-local model root with DASLLAMA_MODELS_DIR (e.g. on the x64 handoff box)
def private models_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}

// gemma-2-2b-it Q8_0, "1, 2, 3, 4, 5, 6, 7," (BOS=2) — same frozen oracle as test_parity.das
// (re-frozen to the counting prompt with the cls_q8 tied classifier; see the note there).
let GEMMA2_PROMPT <- [2l, 235274l, 235269l, 235248l, 235284l, 235269l, 235248l, 235304l, 235269l, 235248l,
                      235310l, 235269l, 235248l, 235308l, 235269l, 235248l, 235318l, 235269l, 235248l, 235324l,
                      235269l]
let GEMMA2_GEN <- [235248l, 235321l, 235269l, 235248l, 235315l, 235269l, 235248l, 235274l, 235276l, 235269l,
                   235248l, 235274l, 235274l, 235269l, 235248l, 235274l, 235284l, 235269l, 235248l, 235274l,
                   235304l, 235269l, 235248l, 235274l, 235310l, 235269l, 235248l, 235274l, 235308l, 235269l,
                   235248l, 235274l, 235318l, 235269l, 235248l, 235274l, 235324l, 235269l, 235248l, 235274l]

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

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
        var m <- load_model(path, QuantMode.q8)
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
            generate(m, s, GEMMA2_PROMPT, SamplingParams(), int64(length(GEMMA2_GEN))) $(id, piece) {
                got |> push(id)
                return true
            }
            delete s
        }
        delete m
        set_prefill_attn_mode(pa)
        set_act_vec(pv)
        t |> success(same(got, GEMMA2_GEN), "greedy generate token-for-token vs oracle")
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
def test_sampler_units(t : T?) {
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
        s.recent |> push(3l)
        s.recent |> push(3l)
        s.recent |> push(3l)
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
        s.recent |> push(3l)
        s.recent |> push(3l)
        s.recent |> push(3l)
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
        t |> success(same(a, b) && length(a) == 32, "same seed => identical draws")
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
        var m <- load_model(path, QuantMode.q8)
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
        t |> success(same(a, b) && int64(length(a)) == n, "reproducible under a fixed seed")
    }
}
