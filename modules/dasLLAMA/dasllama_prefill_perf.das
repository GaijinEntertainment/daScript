options gen2

require dasllama_transformer
require daslib/jobque_boost
require strings
require math

// Prompt-processing (TTFT) profile for a Llama-3 GGUF (Q8): the OLD per-token path (N× forward,
// decode-style gemv per token) vs ONE batched forward_prefill. Token content doesn't affect speed,
// so synthetic ids are fine. One run state is reused — each run overwrites the cache positions it
// reads, so no reset is needed between runs.
//   bin/daslang -jit modules/dasLLAMA/dasllama_prefill_perf.das -- <model.gguf>
let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf"

def build_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + (i % 5000l)]
}

[export]
def main {
    var path = DEFAULT_MODEL
    let args = get_command_line_arguments()
    if (!empty(args) && ends_with(args[length(args) - 1], ".gguf")) {
        path = args[length(args) - 1]
    }
    print("loading {path}\n")
    var t <- load_gguf(path, QuantMode.q8)
    t.config.seq_len = min(t.config.seq_len, 8192l)   // cap KV cache (Llama-3 context length is 131072)
    let c = t.config
    print("config: dim={c.dim} hidden={c.hidden_dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size}\n")
    print("prompt processing: per-token forward (decode-style) vs one batched forward_prefill\n\n")

    var s <- make_run_state(c)
    with_job_que() {
        set_jobque_fork_pool(true, true)
        // warmup — JIT both paths
        let warm <- build_prompt(8l)
        for (p in range64(8l)) {
            forward(t, s, warm[p], p)
        }
        forward_prefill(t, s, warm, 8l, 0l)

        let sizes <- [64l, 256l, 512l]
        for (n in sizes) {
            let prompt <- build_prompt(n)
            let t0 = ref_time_ticks()
            for (p in range64(n)) {
                forward(t, s, prompt[p], p)
            }
            let us_old = get_time_usec(t0)
            let t1 = ref_time_ticks()
            forward_prefill(t, s, prompt, n, 0l)
            let us_new = get_time_usec(t1)
            let tps_old = double(n) / (double(us_old) * 1.0e-6lf)
            let tps_new = double(n) / (double(us_new) * 1.0e-6lf)
            let speedup = double(us_old) / double(us_new)
            print("N={n}: per-token {us_old}us {tps_old} tok/s  |  prefill {us_new}us {tps_new} tok/s  |  {speedup}x\n")
        }
    }
}
