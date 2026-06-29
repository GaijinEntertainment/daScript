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
    let tr <- load_gguf(path, quant)
    let n_gen = length(want)
    var ok = false
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
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, GEMMA2_PROMPT, GEMMA2_GEN), "gemma2 token-for-token")
    }
}
