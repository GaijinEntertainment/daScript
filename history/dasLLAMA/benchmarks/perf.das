options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require daslib/fio

// End-to-end throughput harness — the real metric for "reasonable performance".
// Run under JIT: bin/daslang -jit modules/dasLLAMA/dasllama_perf.das
// override the machine-local llama2.c checkout with DASLLAMA_LLAMA2C_DIR (same contract as modules/dasLLAMA/tests)
def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")
let N_TOKENS = 256  // == seq_len; greedy decode from BOS

// matmul FLOPs per generated token: per layer Q/K/V/O + SwiGLU w1/w2/w3, plus the classifier.
def flops_per_token(c : Config) : int64 {
    let per_layer = 4l * c.dim * c.dim + 4l * c.dim * c.kv_dim + 6l * c.dim * c.hidden_dim
    return c.n_layers * per_layer + 2l * c.vocab_size * c.dim
}

def run_once(t : Model; c : Config; tag : string) : double {
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
    if (!jit_enabled()) {
        print("This benchmark is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var t <- load_checkpoint(MODEL_PATH)
    let c = t.config

    // job queue persists across the whole run (created once, before timing)
    with_job_que() {
        let fp32 = run_once(t, c, "fp32 ")  // quant defaults to fp32
        quantize_weights(t)                 // sets t.quant = q8
        let q8 = run_once(t, c, "q8   ")
        quantize_weights_q4(t)              // sets t.quant = q4_0
        let q4 = run_once(t, c, "q4_0 ")
        print("  q8 speedup: {q8 / fp32}x, q4_0 speedup: {q4 / fp32}x\n")
    }
}
