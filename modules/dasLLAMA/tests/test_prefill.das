options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require dasllama/dasllama_tokenizer
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math  // setup_dasllama_jobque_ + the runtime knob setters (engine-level, not re-exported)

// forward_prefill / forward_batch must be BIT-FOR-BIT identical to calling forward() once per
// position: same last-position logits AND same KV cache — both for a from-scratch prompt and for a
// prefill that continues an already-populated cache (start_pos > 0, the multi-turn-chat path).
// Needs llama2.c's stories15M.bin (gitignored, 60MB); skips cleanly when absent.
// override the machine-local llama2.c checkout with DASLLAMA_LLAMA2C_DIR (e.g. on the x64 handoff box)
def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")
let TOKENIZER_PATH = path_join(llama2c_dir(), "tokenizer.bin")

def feq(a, b : array<float>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}

// KV caches are codec-encoded byte blobs now — byte equality IS bit-exactness for a same-dtype pair
def feq(a, b : array<uint8>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
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
    let n = long_length(prompt)
    with_job_que() {
        setup_dasllama_jobque_()
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
        delete sr
        delete sp
    }
}

// Prefill that CONTINUES a populated cache: warm positions 0..split-1 one token at a time, then
// prefill the tail at start_pos=split. Must match running all positions per-token.
def check_continuation(t : T?; tr : Model; prompt : array<int64>; split : int64; mode : AttnPrefillMode) {
    let c = tr.config
    let n = long_length(prompt)
    with_job_que() {
        setup_dasllama_jobque_()
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
        delete sr
        delete sp
    }
}

[test]
def test_prefill_fp32(t : T?) {
    t |> run("prefill == N× forward, fp32 (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        let long_prompt <- [for (i in range64(200l)); int64(i % 200l + 1l)]
        for (mode in [AttnPrefillMode.classic, AttnPrefillMode.blocked]) {
            check_prefill(t, tr, prompt, mode)
            check_continuation(t, tr, prompt, 4l, mode)
            check_prefill(t, tr, long_prompt, mode)   // 200 tok trips g_attn_par_threshold (threaded arm)
        }
        delete tk
        delete tr
    }
}

[test]
def test_prefill_q8(t : T?) {
    t |> run("prefill == N× forward, Q8 (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        // bit-exactness across forward_batch (batch tile) and forward (GEMV) needs same-dot
        // kernels on both paths: portable's batch and GEMV share dot_q8q8, so the contract holds
        // under any tune stamp; a tuned tile (e.g. kstep4 vs the gemv's gkstep2) legally folds in
        // a different order. Tuned-tier cross-kernel numerics live in test_batch_decode_tuned.
        pin_kernel_backend("portable")
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        let long_prompt <- [for (i in range64(200l)); int64(i % 200l + 1l)]
        for (mode in [AttnPrefillMode.classic, AttnPrefillMode.blocked]) {
            check_prefill(t, tr, prompt, mode)
            check_continuation(t, tr, prompt, 4l, mode)
            check_prefill(t, tr, long_prompt, mode)   // 200 tok trips g_attn_par_threshold (threaded arm)
        }
        clear_kernel_backend_pin()
        delete tk
        delete tr
    }
}

[test]
def test_prefill_q4_fallback(t : T?) {
    t |> run("q4_0 prefill takes the per-token fallback and matches (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        // regression: the legacy per-row q4_0 tier has no batched kernels — forward_prefill must
        // serve it via the per-token forward loop (generate()/eval() route every multi-token
        // prompt through here), not panic
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights_q4(tr)
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        check_prefill(t, tr, prompt, AttnPrefillMode.classic)
        check_continuation(t, tr, prompt, 4l, AttnPrefillMode.classic)
        delete tk
        delete tr
    }
}
