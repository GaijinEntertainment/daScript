options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require daslib/fio

// Phase-3 sampling API. Greedy (SamplingParams default: temp=0, penalty=1.0) must reproduce the
// frozen greedy oracle bit-for-bit; temp>0 sampling must be deterministic under a fixed seed.
// GGUF models are gitignored, so each case skips cleanly when its model is absent.

let MODELS_DIR = "/Users/borisbatkin/Work/llama.cpp/models/"

// gemma-2-2b-it Q8_0, "Once upon a time" (BOS=2) — same frozen oracle as test_parity.das.
let GEMMA2_PROMPT <- [2l, 14326l, 3054l, 476l, 1069l]
let GEMMA2_GEN <- [235269l, 575l, 476l, 2840l, 10545l, 675l, 166962l, 27836l, 578l, 121048l,
                   17200l, 235269l, 11012l, 476l, 3486l, 4602l, 8602l, 2810l, 1610l, 235265l,
                   2810l, 1610l, 729l, 3836l]

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
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_model(path, QuantMode.q8)
        // Token-exact oracle gate: pin bit-exact classic attention + scalar activations (same reason
        // as test_parity — the flash / exp4 defaults are within tolerance but not bit-exact).
        let pa = get_prefill_attn_mode()
        set_prefill_attn_mode(AttnPrefillMode.classic)
        let pv = get_act_vec()
        set_act_vec(false)
        var got : array<int64>
        with_job_que() {
            set_jobque_fork_pool(true, true)
            var s = create_session(m)
            generate(m, s, GEMMA2_PROMPT, SamplingParams(), int64(length(GEMMA2_GEN))) $(id, piece) {
                got |> push(id)
                return true
            }
        }
        set_prefill_attn_mode(pa)
        set_act_vec(pv)
        t |> success(same(got, GEMMA2_GEN), "greedy generate token-for-token vs oracle")
    }
}

[test]
def test_sampling_deterministic(t : T?) {
    t |> run("same seed => identical sampled output") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_model(path, QuantMode.q8)
        let params = SamplingParams(temp = 0.8, top_k = 40l)
        let n = 16l
        var a : array<int64>
        var b : array<int64>
        with_job_que() {
            set_jobque_fork_pool(true, true)
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
        }
        t |> success(same(a, b) && int64(length(a)) == n, "reproducible under a fixed seed")
    }
}
