options gen2

require dasllama_transformer

// End-to-end throughput harness — the real metric for "reasonable performance".
// Run under JIT: bin/daslang -jit modules/dasLLAMA/dasllama_perf.das
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let N_TOKENS = 256  // == seq_len; greedy decode from BOS

// matmul FLOPs per generated token: per layer Q/K/V/O + SwiGLU w1/w2/w3, plus the classifier.
def flops_per_token(c : Config) : int64 {
    let per_layer = 4l * c.dim * c.dim + 4l * c.dim * c.kv_dim + 6l * c.dim * c.hidden_dim
    return c.n_layers * per_layer + 2l * c.vocab_size * c.dim
}

[export]
def main {
    let t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    var s <- make_run_state(c)

    let t0 = ref_time_ticks()
    let ids <- generate(t, s, [1l], N_TOKENS)
    let us = get_time_usec(t0)

    let toks = length(ids)
    let tok_per_s = double(toks) * 1.0lf / (double(us) * 1.0e-6lf)
    let fpt = flops_per_token(c)
    let gflops = double(int64(toks) * fpt) / (double(us) * 1000.0lf)
    print("generated {toks} tokens in {us} us\n")
    print("  {tok_per_s} tok/s\n")
    print("  {gflops} GFLOP/s (matmul)\n")
}
