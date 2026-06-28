options gen2

require dasllama_transformer
require dasllama_tokenizer
require daslib/jobque_boost
require strings

// Step 7.2 end-to-end: load TinyLlama from GGUF (F16 -> fp32) + the SentencePiece tokenizer
// from the same file's metadata, then go text-in -> text-out and compare to llama.cpp.
//   - encode the prompt string and check it matches llama-tokenize's IDs
//   - greedy-generate and check the continuation token IDs match the oracle
//   - decode the whole thing back to text
//
// Run with JIT (F16 decode + 1.1B-param forward are too slow interpreted):
//   bin/daslang -jit modules/dasLLAMA/dasllama_gguf_run.das

let MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf"

[export]
def main {
    let prompt_text = "Once upon a time"
    // llama-tokenize "Once upon a time" (add_bos): [1, 9038, 2501, 263, 931]
    let prompt_oracle = [1l, 9038l, 2501l, 263l, 931l]
    // instrumented llama-simple greedy continuation (40 tokens) on the same F16 file
    let oracle = [29892l, 727l, 471l, 263l, 4123l, 7826l, 4257l, 365l, 2354l, 29889l,
        2296l, 471l, 263l, 1407l, 15040l, 322l, 12758l, 7826l, 29892l, 2337l,
        3063l, 363l, 716l, 322l, 5566l, 11407l, 2712l, 304l, 5110l, 29889l,
        3118l, 2462l, 29892l, 1183l, 380l, 25443l, 2501l, 263l, 3143l, 393l]

    print("loading {MODEL}\n")
    let t <- load_gguf(MODEL)
    let tok <- load_tokenizer_gguf(MODEL)
    let c = t.config
    print("config: dim={c.dim} hidden={c.hidden_dim} layers={c.n_layers} ")
    print("heads={c.n_heads}/{c.n_kv_heads} (kv_mul={c.n_heads / c.n_kv_heads}) ")
    print("vocab={c.vocab_size} seq_len={c.seq_len}\n")

    // --- encode check ---
    let prompt <- encode(tok, prompt_text, true, false)
    var enc_ok = length(prompt) == length(prompt_oracle)
    if (enc_ok) {
        for (i in range(length(prompt))) {
            enc_ok = enc_ok && prompt[i] == prompt_oracle[i]
        }
    }
    let pstr = build_string() $(var w) {
        for (p in prompt) {
            w |> write(p)
            w |> write(" ")
        }
    }
    let enc_status = enc_ok ? "OK" : "MISMATCH"
    print("encode('{prompt_text}') = [{pstr}] -> {enc_status}\n")

    // --- generate ---
    var s = make_run_state(c)
    let n_gen = length(oracle)
    with_job_que() {
        set_jobque_fork_pool(true, true)  // pool per-job fork contexts + skip their init (pure-data matmul jobs)
        let ids <- generate(t, s, prompt, length(prompt) + n_gen)
        // decode the whole emitted sequence back to text
        let text = build_string() $(var w) {
            var prev = 1l
            for (id in ids) {
                w |> write(decode(tok, prev, id))
                prev = id
            }
        }
        print("OUTPUT: {text}\n")
        // continuation begins after the prompt echo (prompt minus BOS)
        let echo = length(prompt) - 1
        var matched = 0
        for (i in range(n_gen)) {
            if (ids[echo + i] == oracle[i]) {
                matched++
            }
        }
        print("continuation MATCH: {matched}/{n_gen} vs llama.cpp greedy\n")
        if (enc_ok && matched == n_gen) {
            print("PASS: encode + forward + decode all match llama.cpp\n")
        } else {
            print("FAIL\n")
        }
    }
}
