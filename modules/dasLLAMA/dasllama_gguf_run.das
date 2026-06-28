options gen2

require dasllama_transformer
require daslib/jobque_boost

// Step 7.1 loader check: load TinyLlama from GGUF (F16 -> fp32), run the existing fp32
// forward greedy from a known prompt, and compare our continuation token IDs against the
// oracle (instrumented llama-simple, greedy, same prompt). Tokenizer-independent: the prompt
// IDs are the oracle's own tokenization, so only the loader + forward are under test.
//
// Run with JIT (the F16 decode + 1.1B-param forward are too slow interpreted):
//   bin/daslang -jit modules/dasLLAMA/dasllama_gguf_run.das

let MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf"

[export]
def main {
    // "Once upon a time" tokenized by llama.cpp (add_bos): [1, 9038, 2501, 263, 931]
    let prompt = [1l, 9038l, 2501l, 263l, 931l]
    // Oracle greedy continuation (40 tokens) from instrumented llama-simple on the same F16 file.
    let oracle = [29892l, 727l, 471l, 263l, 4123l, 7826l, 4257l, 365l, 2354l, 29889l,
        2296l, 471l, 263l, 1407l, 15040l, 322l, 12758l, 7826l, 29892l, 2337l,
        3063l, 363l, 716l, 322l, 5566l, 11407l, 2712l, 304l, 5110l, 29889l,
        3118l, 2462l, 29892l, 1183l, 380l, 25443l, 2501l, 263l, 3143l, 393l]

    print("loading {MODEL}\n")
    let t <- load_gguf(MODEL)
    let c = t.config
    print("config: dim={c.dim} hidden={c.hidden_dim} layers={c.n_layers} ")
    print("heads={c.n_heads}/{c.n_kv_heads} (kv_mul={c.n_heads / c.n_kv_heads}) ")
    print("vocab={c.vocab_size} seq_len={c.seq_len} shared_cls={c.shared_weights}\n")

    var s = make_run_state(c)
    let n_gen = length(oracle)
    with_job_que() {
        let ids <- generate(t, s, prompt, length(prompt) + n_gen)
        // ids = [prompt echo minus BOS ..., generated ...]; generated start after the echo
        let echo = length(prompt) - 1
        var matched = 0
        print("ours vs oracle (40 generated):\n")
        var line = ""
        for (i in range(n_gen)) {
            let ours = ids[echo + i]
            let want = oracle[i]
            if (ours == want) {
                matched++
                line += "{ours} "
            } else {
                line += "{ours}(!={want}) "
            }
        }
        print("{line}\n")
        print("MATCH: {matched}/{n_gen}\n")
        if (matched == n_gen) {
            print("PASS: token-for-token with llama.cpp greedy\n")
        } else {
            print("FAIL: diverged ({matched}/{n_gen})\n")
        }
    }
}
