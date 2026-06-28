options gen2

require dasllama_transformer
require dasllama_tokenizer
require daslib/jobque_boost
require strings
require math

// Step 7.3 — the quant payoff on a REAL bandwidth-bound model. Self-quantize TinyLlama's
// fp32 GGUF weights to Q8_0 / Q4_0 (all-weights) and A/B the throughput against fp32 on the
// same loaded model. On the L2-resident toy Q4 was a 0.87x regression; here every matmul is
// big, so time ~ bytes/bandwidth should finally make Q8/Q4 win. Also reports how long each
// tracks the fp32 greedy stream and decodes each for a coherence eyeball.
//
//   bin/daslang -jit modules/dasLLAMA/dasllama_gguf_perf.das

let MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf"
let N_TOKENS = 64

// matmul FLOPs per generated token: per layer Q/K/V/O + SwiGLU w1/w2/w3, plus the classifier.
def flops_per_token(c : Config) : int64 {
    let per_layer = 4l * c.dim * c.dim + 4l * c.dim * c.kv_dim + 6l * c.dim * c.hidden_dim
    return c.n_layers * per_layer + 2l * c.vocab_size * c.dim
}

def run_capture(t : Transformer; c : Config; tag : string; var out_tps : double&) : array<int64> {
    var s <- make_run_state(c)
    let prompt = [1l, 9038l, 2501l, 263l, 931l]  // "Once upon a time"
    let t0 = ref_time_ticks()
    var ids <- generate(t, s, prompt, length(prompt) + N_TOKENS)
    let us = get_time_usec(t0)
    let toks = length(ids)
    let tps = double(toks) / (double(us) * 1.0e-6lf)
    let gf = double(int64(toks) * flops_per_token(c)) / (double(us) * 1000.0lf)
    print("  {tag}: {toks} tok in {us} us -> {tps} tok/s ({gf} GFLOP/s)\n")
    out_tps = tps
    return <- ids
}

// number of leading token IDs that two streams share (how long quant tracks fp32)
def match_len(a, b : array<int64>) : int {
    let n = min(length(a), length(b))
    var i = 0
    while (i < n && a[i] == b[i]) {
        i++
    }
    return i
}

def decode_ids(tok : Tokenizer; ids : array<int64>; limit : int) : string {
    return build_string() $(var w) {
        var prev = 1l
        var n = 0
        for (id in ids) {
            break if (n >= limit)
            w |> write(decode(tok, prev, id))
            prev = id
            n++
        }
    }
}

[export]
def main {
    var path = MODEL
    let args = get_command_line_arguments()
    if (!empty(args)) {
        path = args[length(args) - 1]
    }
    print("loading {path}\n")
    var t <- load_gguf(path)
    let tok <- load_tokenizer_gguf(MODEL)
    let c = t.config
    print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size}\n")

    with_job_que() {
        var fp = 0.0lf
        var q8 = 0.0lf
        var q4 = 0.0lf
        let fp32_ids <- run_capture(t, c, "fp32", fp)
        quantize_weights(t)
        let q8_ids <- run_capture(t, c, "q8  ", q8)
        quantize_weights_q4(t)
        let q4_ids <- run_capture(t, c, "q4  ", q4)

        print("\nspeedup: q8 {q8 / fp}x, q4 {q4 / fp}x\n")
        print("tracks fp32 greedy: q8 {match_len(fp32_ids, q8_ids)}/{length(fp32_ids)}, q4 {match_len(fp32_ids, q4_ids)}/{length(fp32_ids)} tokens\n")
        print("\nfp32: {decode_ids(tok, fp32_ids, 48)}\n")
        print("q8  : {decode_ids(tok, q8_ids, 48)}\n")
        print("q4  : {decode_ids(tok, q4_ids, 48)}\n")
    }
}
