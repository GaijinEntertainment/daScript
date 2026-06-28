options gen2

require dasllama_transformer
require dasllama_tokenizer

// Local model + tokenizer (60MB / 433KB, gitignored).
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"

// llama2.c stories15M, empty prompt, greedy (-t 0.0): the token IDs after BOS.
let ORACLE <- [9038l, 2501l, 263l, 931l, 29892l, 727l, 471l, 263l, 2217l, 7826l, 4257l, 365l]

[export]
def main {
    let t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)
    print("=== dasLLAMA loaded {MODEL_PATH} (dim={c.dim}, {c.n_layers} layers, vocab={c.vocab_size}) ===\n")

    var s <- make_run_state(c)
    let ids <- generate(t, s, length(ORACLE))

    // decode to text (llama2.c-style: prev token drives the leading-space strip after BOS)
    print("generated: \"")
    var prev = 1l  // BOS
    for (id in ids) {
        print(decode(tk, prev, id))
        prev = id
    }
    print("\"\n")

    // token-for-token check against the oracle IDs
    var ok = length(ids) == length(ORACLE)
    for (i in range(length(ids))) {
        if (i >= length(ORACLE) || ids[i] != ORACLE[i]) {
            ok = false
        }
    }
    print(ok ? "MATCH — reproduces llama2.c token-for-token\n" : "MISMATCH\n")
}
