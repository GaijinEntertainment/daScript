options gen2
options persistent_heap   // six fresh sessions below — their deletes must really free (tutorial 04 explains)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/strings_boost
require daslib/fio
require strings
require math

// Tutorial dasLLAMA-03: Sampling
//
// This tutorial covers:
//   - SamplingParams: temp, top_k, penalty, penalty_last_n (defaults are greedy)
//   - Greedy generation is deterministic — and why tiny models loop under it
//   - Breaking loops with the repetition penalty
//   - Temperature + top-k sampling, and reproducibility with set_seed
//
// Prerequisites: tutorial 01.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/03_sampling.das -- <path/to/model.gguf>

let PROMPT = "Once upon a time"
let N_GEN = 40l

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

// Generate one continuation on a FRESH session, so runs compare cleanly. Each session has its
// own sampling RNG — set_seed makes a sampled run reproducible.
def run_once(m : Model; prompt : array<int64>; params : SamplingParams; seed : int) : string {
    var s = create_session(m)
    set_seed(s, seed)
    var parts : array<string>
    generate(m, s, prompt, params, N_GEN) $(_id, piece) {
        parts |> push(piece)
        return true
    }
    let out = join(parts, "")
    delete s
    return out
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/03_sampling.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, 1024l)   // bound each session's KV cache (tutorial 04)
    let prompt <- encode(m, PROMPT)

    with_job_que() {
        set_jobque_fork_pool(true, true)

        // ──────────────────────────────────────────────────────────────────────
        // Section 1 — Greedy: deterministic, and loop-prone
        // ──────────────────────────────────────────────────────────────────────
        //
        // SamplingParams() defaults to greedy argmax (temp = 0, penalty = 1): the single
        // most likely token every step. Deterministic — two runs are identical — but on
        // small models the most likely continuation of a repetitive context is more
        // repetition, so greedy text tends to loop.

        let g1 = run_once(m, prompt, SamplingParams(), 0)
        let g2 = run_once(m, prompt, SamplingParams(), 0)
        print("greedy:{g1}\n")
        print("two greedy runs identical: {g1 == g2}\n\n")

        // ──────────────────────────────────────────────────────────────────────
        // Section 2 — The repetition penalty
        // ──────────────────────────────────────────────────────────────────────
        //
        // penalty > 1 scales down the logits of the last penalty_last_n generated
        // tokens (64 by default) before picking, so the argmax can't keep choosing
        // the same phrase. Still deterministic — no randomness involved.

        let p = run_once(m, prompt, SamplingParams(penalty = 1.3), 0)
        print("greedy + penalty 1.3:{p}\n\n")

        // ──────────────────────────────────────────────────────────────────────
        // Section 3 — Temperature, top-k, and seeds
        // ──────────────────────────────────────────────────────────────────────
        //
        // temp > 0 samples from the softmax distribution (higher = more adventurous);
        // top_k > 0 first cuts it to the k most likely tokens. Sampling draws from the
        // session's RNG, so variety comes from the seed — and the same seed reproduces
        // the same text exactly.

        let params = SamplingParams(temp = 0.8, top_k = 40l, penalty = 1.1)
        let s7 = run_once(m, prompt, params, 7)
        let s8 = run_once(m, prompt, params, 8)
        let s7again = run_once(m, prompt, params, 7)
        print("seed 7:{s7}\n")
        print("seed 8:{s8}\n")
        print("seed 7 reproduces: {s7 == s7again}\n")
    }
    delete m
}
