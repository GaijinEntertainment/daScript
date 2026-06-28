options gen2

require dastest/testing_boost public
require dasllama_transformer
require dasllama_tokenizer
require daslib/jobque_boost
require daslib/fio

// forward_prefill must be BIT-FOR-BIT identical to calling forward() once per prompt position:
// same last-position logits AND same KV cache. Needs llama2.c's stories15M.bin (gitignored, 60MB);
// skips cleanly when absent.
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"

def feq(a, b : array<float>) : bool {
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

def check_prefill(t : T?; tr : Transformer; prompt : array<int64>) {
    let c = tr.config
    let n = int64(length(prompt))
    with_job_que() {
        set_jobque_fork_pool(true, true)
        // reference: per-token forward for positions 0..n-1
        var sr <- make_run_state(c)
        for (p in range64(n)) {
            forward(tr, sr, prompt[p], p)
        }
        // prefill in one batched pass
        var sp <- make_run_state(c)
        forward_prefill(tr, sp, prompt, n)
        t |> success(feq(sr.logits, sp.logits), "last-position logits bit-exact")
        t |> success(feq(sr.key_cache, sp.key_cache), "key cache bit-exact")
        t |> success(feq(sr.value_cache, sp.value_cache), "value cache bit-exact")
    }
}

[test]
def test_prefill_fp32(t : T?) {
    t |> run("prefill == N× forward, fp32 (needs stories15M.bin)") @(t : T?) {
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin not present")
            return
        }
        let tr <- load_checkpoint(MODEL_PATH)
        let tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        check_prefill(t, tr, prompt)
    }
}

[test]
def test_prefill_q8(t : T?) {
    t |> run("prefill == N× forward, Q8 (needs stories15M.bin)") @(t : T?) {
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        let tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        check_prefill(t, tr, prompt)
    }
}
