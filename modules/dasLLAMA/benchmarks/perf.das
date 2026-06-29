options gen2

require dasllama/dasllama_transformer
require daslib/jobque_boost

// End-to-end throughput harness — the real metric for "reasonable performance".
// Run under JIT: bin/daslang -jit modules/dasLLAMA/dasllama_perf.das
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let N_TOKENS = 256  // == seq_len; greedy decode from BOS

// matmul FLOPs per generated token: per layer Q/K/V/O + SwiGLU w1/w2/w3, plus the classifier.
def flops_per_token(c : Config) : int64 {
    let per_layer = 4l * c.dim * c.dim + 4l * c.dim * c.kv_dim + 6l * c.dim * c.hidden_dim
    return c.n_layers * per_layer + 2l * c.vocab_size * c.dim
}

def run_once(t : Transformer; c : Config; tag : string) : double {
    var s <- make_run_state(c)
    let t0 = ref_time_ticks()
    let ids <- generate(t, s, [1l], N_TOKENS)
    let us = get_time_usec(t0)
    let toks = length(ids)
    let tok_per_s = double(toks) * 1.0lf / (double(us) * 1.0e-6lf)
    let gflops = double(int64(toks) * flops_per_token(c)) / (double(us) * 1000.0lf)
    print("  {tag}: {toks} tokens in {us} us -> {tok_per_s} tok/s ({gflops} GFLOP/s)\n")
    return tok_per_s
}

[export]
def main {
    var t <- load_checkpoint(MODEL_PATH)
    let c = t.config

    // job queue persists across the whole run (created once, before timing)
    with_job_que() {
        let fp32 = run_once(t, c, "fp32 ")  // quant defaults to fp32
        quantize_weights(t)                 // sets t.quant = q8
        let q8 = run_once(t, c, "q8   ")
        quantize_weights_q4(t)              // sets t.quant = q4
        let q4 = run_once(t, c, "q4   ")
        print("  q8 speedup: {q8 / fp32}x, q4 speedup: {q4 / fp32}x\n")
    }
}
