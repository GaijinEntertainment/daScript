options gen2

require dasllama_transformer

// Step-1 checkpoint runner: load stories15M.bin and print the parsed Config.
// Model path is local (the file is 60MB, gitignored).
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"

// llama2.c stories15M, empty prompt, greedy (-t 0.0): the token IDs after BOS.
// "Once upon a time, there was a little girl named L..."
let ORACLE <- [9038, 2501, 263, 931, 29892, 727, 471, 263, 2217, 7826, 4257, 365]

[export]
def main {
    let t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    print("=== dasLLAMA loaded {MODEL_PATH} (dim={c.dim}, {c.n_layers} layers, vocab={c.vocab_size}) ===\n")

    var s <- make_run_state(c)
    let ids <- generate(t, s, length(ORACLE))

    var ok = length(ids) == length(ORACLE)
    for (i in range(length(ids))) {
        let want = i < length(ORACLE) ? ORACLE[i] : -1
        let hit = ids[i] == want
        ok = ok && hit
        print("step {i}: got {ids[i]}  oracle {want}  {hit ? "ok" : "MISMATCH"}\n")
    }
    print(ok ? "\nMATCH — forward pass reproduces llama2.c token-for-token\n" : "\nMISMATCH\n")
}
