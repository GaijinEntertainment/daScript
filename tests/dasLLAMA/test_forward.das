options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama_tokenizer
require daslib/jobque_boost
require daslib/fio

// End-to-end oracle: needs llama2.c's stories15M.bin + tokenizer.bin (gitignored, 60MB).
// Skips cleanly when absent (e.g. in CI), so it's a real regression locally and a no-op elsewhere.
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"
// llama2.c greedy (-t 0.0): empty-prompt IDs, and encode("Once upon a time", bos).
let ORACLE <- [9038l, 2501l, 263l, 931l, 29892l, 727l, 471l, 263l, 2217l, 7826l, 4257l, 365l]
let PROMPT_ORACLE <- [1l, 9038l, 2501l, 263l, 931l]

def same(a, b : array<int64>) : bool {
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

[test]
def test_forward_end_to_end(t : T?) {
    t |> run("forward + tokenizer match llama2.c (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin / tokenizer.bin not present")
            return
        }
        let tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)
        // BPE encoder vs llama2.c tokenization
        let pt <- encode(tk, "Once upon a time", true, false)
        t |> success(same(pt, PROMPT_ORACLE), "encode prompt tokens")
        // full forward + greedy decode, token-for-token (threads the classifier)
        with_job_que() {
            var s <- make_run_state(c)
            let bos_only <- encode(tk, "", true, false)
            let ids <- generate(tr, s, bos_only, length(ORACLE))
            t |> success(same(ids, ORACLE), "empty-prompt greedy token-for-token")
        }
    }
}

[test]
def test_forward_q8(t : T?) {
    t |> run("all-weights Q8 forward matches fp32 oracle (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin / tokenizer.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)
        quantize_weights(tr)  // all 2D weights -> Q8_0, tr.quant = q8
        // Q8 is effectively lossless at greedy decode on stories15M: still token-for-token.
        with_job_que() {
            var s <- make_run_state(c)
            let bos_only <- encode(tk, "", true, false)
            let ids <- generate(tr, s, bos_only, length(ORACLE))
            t |> success(same(ids, ORACLE), "Q8 empty-prompt greedy token-for-token")
        }
    }
}

[test]
def test_forward_q8_pooled(t : T?) {
    t |> run("Q8 forward with pooled fork contexts still matches the oracle (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin / tokenizer.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)
        quantize_weights(tr)
        // pool + skip-init the per-job fork contexts: same result, exercised across many tokens'
        // classifier parallel_for so a stale/misreset pooled fork would corrupt the stream.
        with_job_que() {
            set_jobque_fork_pool(true, true)
            var s <- make_run_state(c)
            let bos_only <- encode(tk, "", true, false)
            let ids <- generate(tr, s, bos_only, length(ORACLE))
            t |> success(same(ids, ORACLE), "pooled-fork Q8 greedy token-for-token")
        }
    }
}

[test]
def test_forward_q4(t : T?) {
    t |> run("all-weights Q4 forward matches the fp32 oracle prefix (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin / tokenizer.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)
        quantize_weights_q4(tr)  // all 2D weights -> Q4_0, tr.quant = q4
        // Q4 is lossier than Q8 — greedy diverges from fp32 around token ~19 — but the per-step
        // loss hasn't flipped an argmax yet over the 12-token ORACLE prefix, so it still matches.
        with_job_que() {
            var s <- make_run_state(c)
            let bos_only <- encode(tk, "", true, false)
            let ids <- generate(tr, s, bos_only, length(ORACLE))
            t |> success(same(ids, ORACLE), "Q4 empty-prompt greedy matches oracle prefix")
        }
    }
}
