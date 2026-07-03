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
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
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
            var s = create_session(m)
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

[test]
def test_sampling_deterministic(t : T?) {
    t |> run("same seed => identical sampled output") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
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
