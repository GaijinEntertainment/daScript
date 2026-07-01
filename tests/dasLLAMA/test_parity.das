options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer
require daslib/jobque_boost
require daslib/fio

// Frozen token-for-token parity fixtures for the GGUF arches that stories15M (test_forward.das)
// doesn't cover. Each fixture is a prompt-ID list + the reference greedy continuation that
// llama.cpp's simple_ids oracle produced (regenerate with harness/parity.sh). The GGUF models are
// gitignored multi-GB files, so each case skips cleanly when its model is absent (a real local
// regression, a no-op in CI) — same contract as test_forward.das.
//
// Prompts are deliberately the *confident* ones: high-entropy prompts flip on genuine near-ties
// (sub-ULP libm cos/sin/exp/tanh vs llama.cpp), which are not bugs. See dasllama_newmodels_arc.

let MODELS_DIR = "/Users/borisbatkin/Work/llama.cpp/models/"

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

// Load `path` at `quant`, greedily continue `prompt`, and return whether the continuation matches
// `want` token-for-token. Kept free of T? so the [test]'s run()-lambda captures no array by move.
def private parity_ok(path : string; quant : QuantMode; prompt, want : array<int64>) : bool {
    var tr <- load_gguf(path, quant)
    // Cap the KV context: only ~(prompt + want) positions are used, and the KV stride doesn't change
    // the computed tokens — but Llama-3's native 131072 makes the KV array exceed the 4GB uint32 cap.
    if (tr.config.seq_len > 2048l) {
        tr.config.seq_len = 2048l
    }
    let n_gen = length(want)
    var ok = false
    // Token-exact oracle gate => run the BIT-EXACT classic prefill attention, not the (default) flash:
    // flash's online-softmax FP reorder is not bit-exact and can flip a greedy token under the
    // interpreter (whose FP differs from JIT, where the fixtures were frozen). Flash staying within
    // tolerance is what test_flash covers. Restore the prior mode so we don't leak global state.
    let prev_attn = get_prefill_attn_mode()
    set_prefill_attn_mode(AttnPrefillMode.classic)
    // ...and the scalar libm silu/gelu, not the default exp4 gate (~2 ulp) — same token-exact-oracle
    // reasoning as the attention pin above. exp4 staying within tolerance is covered by test_silu.
    let prev_act = get_act_vec()
    set_act_vec(false)
    with_job_que() {
        set_jobque_fork_pool(true, true)
        var s = make_run_state(tr.config)
        let ids <- generate(tr, s, prompt, length(prompt) + n_gen)
        // generate echoes prompt[1..] then the continuation, so the first generated token is at
        // index length(prompt)-1 (the same slice harness/parity.das compares)
        let echo = length(prompt) - 1
        let got <- [for (i in range(n_gen)); ids[echo + i]; where echo + i < length(ids)]
        ok = same(got, want)
    }
    set_prefill_attn_mode(prev_attn)
    set_act_vec(prev_act)
    return ok
}

// gemma-2-2b-it Q8_0, prompt "Once upon a time" (BOS=2). Confident — clean 40/40 vs oracle.
let GEMMA2_PROMPT <- [2l, 14326l, 3054l, 476l, 1069l]
let GEMMA2_GEN <- [235269l, 575l, 476l, 2840l, 10545l, 675l, 166962l, 27836l, 578l, 121048l,
                   17200l, 235269l, 11012l, 476l, 3486l, 4602l, 8602l, 2810l, 1610l, 235265l,
                   2810l, 1610l, 729l, 3836l]

[test]
def test_parity_gemma2(t : T?) {
    t |> run("gemma2 forward (gemma-2-2b-it Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, GEMMA2_PROMPT, GEMMA2_GEN), "gemma2 token-for-token")
    }
}

// TinyLlama-1.1B-v0.3 F16, "Once upon a time" (BOS=1) — the old gguf_run.das oracle (llama.cpp greedy
// on the same F16 file). Loaded fp32 (F16 -> fp32) to match how that oracle was produced.
let TINYLLAMA_V03_PROMPT <- [1l, 9038l, 2501l, 263l, 931l]
let TINYLLAMA_V03_GEN <- [29892l, 727l, 471l, 263l, 4123l, 7826l, 4257l, 365l, 2354l, 29889l,
                          2296l, 471l, 263l, 1407l, 15040l, 322l, 12758l, 7826l, 29892l, 2337l,
                          3063l, 363l, 716l, 322l, 5566l, 11407l, 2712l, 304l, 5110l, 29889l,
                          3118l, 2462l, 29892l, 1183l, 380l, 25443l, 2501l, 263l, 3143l, 393l]

[test]
def test_parity_tinyllama_v03(t : T?) {
    t |> run("llama forward (TinyLlama-1.1B-v0.3 F16 -> fp32)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}tinyllama-1.1b-v0.3-f16.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.fp32, TINYLLAMA_V03_PROMPT, TINYLLAMA_V03_GEN), "tinyllama v0.3 token-for-token")
    }
}

// Llama-3.2-1B-Instruct Q8, [BOS]+"Once upon a time" (θ=500000 + llama3 freq-scaling) — the old
// llama3_run.das oracle (instrumented llama-simple / simple_ids CPU greedy).
let LLAMA32_1B_PROMPT <- [128000l, 12805l, 5304l, 264l, 892l]
let LLAMA32_1B_GEN <- [11l, 304l, 264l, 2678l, 14458l, 89777l, 304l, 279l, 20700l, 35231l,
                       315l, 350l, 26089l, 3852l, 11l, 1070l, 12439l, 264l, 3995l, 3828l,
                       7086l, 71471l, 13l, 71471l, 574l, 264l, 22999l, 323l, 68188l, 13836l,
                       11l, 449l, 264l, 4851l, 2539l, 315l, 5895l, 323l, 264l, 4059l]

[test]
def test_parity_llama3_2_1b(t : T?) {
    t |> run("llama forward (Llama-3.2-1B-Instruct Q8, θ=500000)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Llama-3.2-1B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, LLAMA32_1B_PROMPT, LLAMA32_1B_GEN), "llama-3.2-1b token-for-token")
    }
}
