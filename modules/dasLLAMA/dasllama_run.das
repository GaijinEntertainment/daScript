options gen2

require dasllama_transformer
require dasllama_tokenizer

// Local model + tokenizer (60MB / 433KB, gitignored).
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"

// llama2.c stories15M greedy (-t 0.0): empty-prompt token IDs after BOS.
let ORACLE <- [9038l, 2501l, 263l, 931l, 29892l, 727l, 471l, 263l, 2217l, 7826l, 4257l, 365l]
// llama2.c encode("Once upon a time", bos): the prompt token IDs.
let PROMPT_ORACLE <- [1l, 9038l, 2501l, 263l, 931l]

def matches(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

def show(prefix : string; ids : array<int64>) {
    print("{prefix}")
    for (id in ids) {
        print("{id} ")
    }
    print("\n")
}

[export]
def main {
    let t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)
    print("=== dasLLAMA loaded (dim={c.dim}, {c.n_layers} layers, vocab={c.vocab_size}) ===\n")

    // 1. encoder check: tokenize a real prompt, compare to llama2.c's tokenization
    let pt <- encode(tk, "Once upon a time", true, false)
    show("encode 'Once upon a time' -> ", pt)
    print("  encode {matches(pt, PROMPT_ORACLE) ? "MATCH" : "MISMATCH"}\n")

    // 2. empty-prompt regression: greedy decode token-for-token vs the oracle
    var s <- make_run_state(c)
    let bos_only <- encode(tk, "", true, false)
    let ids <- generate(t, s, bos_only, length(ORACLE))
    print("  empty-prompt greedy {matches(ids, ORACLE) ? "MATCH" : "MISMATCH"}\n")

    // 3. prompt-driven generation: continue the prompt, decode to text
    var s2 <- make_run_state(c)
    let story <- generate(t, s2, pt, 64)
    print("story: \"")
    var prev = 1l
    for (id in story) {
        print(decode(tk, prev, id))
        prev = id
    }
    print("\"\n")
}
