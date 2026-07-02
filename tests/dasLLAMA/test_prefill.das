options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_tokenizer
require daslib/jobque_boost
require daslib/fio

// forward_prefill / forward_batch must be BIT-FOR-BIT identical to calling forward() once per
// position: same last-position logits AND same KV cache — both for a from-scratch prompt and for a
// prefill that continues an already-populated cache (start_pos > 0, the multi-turn-chat path).
// Needs llama2.c's stories15M.bin (gitignored, 60MB); skips cleanly when absent.
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

// forward_batch over the whole prompt (one batched pass from position 0) == N× forward. Only the
// bit-exact-by-design modes (classic, blocked) are checked here; flash is online-softmax (tolerance,
// not bit-exact) and has its own oracle test.
def check_prefill(t : T?; tr : Model; prompt : array<int64>; mode : AttnPrefillMode) {
    let c = tr.config
    let n = int64(length(prompt))
    with_job_que() {
        set_jobque_fork_pool(true, true)
        set_prefill_attn_mode(mode)
        var sr <- make_run_state(c)
        for (p in range64(n)) {
            forward(tr, sr, prompt[p], p)
        }
        var sp <- make_run_state(c)
        forward_batch(tr, sp, prompt, n, 0l)
        t |> success(feq(sr.logits, sp.logits), "{mode} last-position logits bit-exact")
        t |> success(feq(sr.key_cache, sp.key_cache), "{mode} key cache bit-exact")
        t |> success(feq(sr.value_cache, sp.value_cache), "{mode} value cache bit-exact")
    }
}

// Prefill that CONTINUES a populated cache: warm positions 0..split-1 one token at a time, then
// prefill the tail at start_pos=split. Must match running all positions per-token.
def check_continuation(t : T?; tr : Model; prompt : array<int64>; split : int64; mode : AttnPrefillMode) {
    let c = tr.config
    let n = int64(length(prompt))
    with_job_que() {
        set_jobque_fork_pool(true, true)
        set_prefill_attn_mode(mode)
        var sr <- make_run_state(c)
        for (p in range64(n)) {
            forward(tr, sr, prompt[p], p)
        }
        var sp <- make_run_state(c)
        for (p in range64(split)) {
            forward(tr, sp, prompt[p], p)
        }
        let tail <- [for (p in range64(split, n)); prompt[p]]
        forward_prefill(tr, sp, tail, n - split, split)
        t |> success(feq(sr.logits, sp.logits), "{mode} continuation last-position logits bit-exact")
        t |> success(feq(sr.key_cache, sp.key_cache), "{mode} continuation key cache bit-exact")
        t |> success(feq(sr.value_cache, sp.value_cache), "{mode} continuation value cache bit-exact")
    }
}

[test]
def test_prefill_fp32(t : T?) {
    t |> run("prefill == N× forward, fp32 (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin not present")
            return
        }
        let tr <- load_checkpoint(MODEL_PATH)
        let tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        let long_prompt <- [for (i in range64(200l)); int64(i % 200l + 1l)]
        for (mode in [AttnPrefillMode.classic, AttnPrefillMode.blocked]) {
            check_prefill(t, tr, prompt, mode)
            check_continuation(t, tr, prompt, 4l, mode)
            check_prefill(t, tr, long_prompt, mode)   // 200 tok trips ATTN_PAR_THRESHOLD (threaded arm)
        }
    }
}

[test]
def test_prefill_q8(t : T?) {
    t |> run("prefill == N× forward, Q8 (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> success(true, "skipped: stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        let tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        let long_prompt <- [for (i in range64(200l)); int64(i % 200l + 1l)]
        for (mode in [AttnPrefillMode.classic, AttnPrefillMode.blocked]) {
            check_prefill(t, tr, prompt, mode)
            check_continuation(t, tr, prompt, 4l, mode)
            check_prefill(t, tr, long_prompt, mode)   // 200 tok trips ATTN_PAR_THRESHOLD (threaded arm)
        }
    }
}
