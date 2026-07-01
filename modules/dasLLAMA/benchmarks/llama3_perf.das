options gen2
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama_transformer
require daslib/jobque_boost
require strings

// Generation-speed profile for a Llama-3 GGUF (Q8). Prefills a short prompt (untimed),
// then times N greedy generation steps — the batch-1 gemv path, matching llama.cpp's `tg`.
// Tokenizer-independent (forward speed doesn't depend on content), so no BPE vocab needed.
//   bin/daslang -jit modules/dasLLAMA/dasllama_llama3_perf.das -- <model.gguf>

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/Meta-Llama-3.1-8B-Instruct-Q8_0.gguf"
let N_TOKENS = 64

// matmul FLOPs per generated token: per layer Q/K/V/O + SwiGLU w1/w2/w3, plus the classifier.
def flops_per_token(c : Config) : int64 {
    let per_layer = 4l * c.dim * c.dim + 4l * c.dim * c.kv_dim + 6l * c.dim * c.hidden_dim
    return c.n_layers * per_layer + 2l * c.vocab_size * c.dim
}

def argmax_logits(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (i in range64(vocab)) {
        if (s.logits[i] > bv) {
            bv = s.logits[i]
            best = i
        }
    }
    return best
}

[export]
def main {
    if (!jit_enabled()) {
        print("This benchmark is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var path = DEFAULT_MODEL
    let args = get_command_line_arguments()
    if (!empty(args) && ends_with(args[length(args) - 1], ".gguf")) {
        path = args[length(args) - 1]
    }
    print("loading {path}\n")
    let t <- load_gguf(path, QuantMode.q8)
    let c = t.config
    print("config: dim={c.dim} hidden={c.hidden_dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size}\n")

    var s <- make_run_state(c)
    let prompt = [128000l, 12805l, 5304l, 264l, 892l]
    with_job_que() {
        set_jobque_fork_pool(true, true)
        // prefill (untimed)
        var pos = 0l
        for (tk in prompt) {
            forward(t, s, tk, pos)
            pos++
        }
        var token = argmax_logits(s, c.vocab_size)
        // timed generation
        let t0 = ref_time_ticks()
        for (_i in range(N_TOKENS)) {
            forward(t, s, token, pos)
            pos++
            token = argmax_logits(s, c.vocab_size)
        }
        let us = get_time_usec(t0)
        let tps = double(N_TOKENS) / (double(us) * 1.0e-6lf)
        let gf = double(int64(N_TOKENS) * flops_per_token(c)) / (double(us) * 1000.0lf)
        print("gen Q8: {N_TOKENS} tok in {us} us -> {tps} tok/s ({gf} GFLOP/s)\n")
    }
}
