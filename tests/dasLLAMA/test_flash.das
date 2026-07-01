options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama_tokenizer
require daslib/jobque_boost
require daslib/fio
require math

// Flash prefill attention is online-softmax (NOT bit-exact vs classic — the FP sums reorder), so it
// is gated on (a) the predicted next token matching classic on a confident prompt and (b) the full
// logit vector staying within a small relative tolerance of classic. The token-for-token oracle for
// flash is covered by test_parity / test_forward, which run through flash as the default mode.
// Needs llama2.c's stories15M.bin (gitignored); skips cleanly when absent.
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"

def private maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (i in range(min(length(a), length(b)))) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

def private maxabs(a : array<float>) : float {
    var m = 0.0
    for (x in a) {
        m = max(m, abs(x))
    }
    return m
}

def private argmax(a : array<float>) : int {
    var bi = 0
    var bv = a[0]
    for (i in range(length(a))) {
        if (a[i] > bv) {
            bv = a[i]
            bi = i
        }
    }
    return bi
}

// flash prefill must land within REL_TOL of the classic prefill it replaces, and (for a confident
// prompt) predict the same next token. start_pos>0 exercises the continuation path.
def check_flash(t : T?; tr : Model; prompt : array<int64>; start_pos : int64; confident : bool) {
    let c = tr.config
    let n = int64(length(prompt))
    let REL_TOL = 0.05
    with_job_que() {
        set_jobque_fork_pool(true, true)
        set_prefill_attn_mode(AttnPrefillMode.classic)
        var sc <- make_run_state(c)
        for (p in range64(start_pos)) {
            forward(tr, sc, prompt[p], p)
        }
        let tailc <- [for (p in range64(start_pos, n)); prompt[p]]
        forward_prefill(tr, sc, tailc, n - start_pos, start_pos)
        set_prefill_attn_mode(AttnPrefillMode.flash)
        var sf <- make_run_state(c)
        for (p in range64(start_pos)) {
            forward(tr, sf, prompt[p], p)
        }
        let tailf <- [for (p in range64(start_pos, n)); prompt[p]]
        forward_prefill(tr, sf, tailf, n - start_pos, start_pos)
        set_prefill_attn_mode(AttnPrefillMode.flash)
        let rel = maxdiff(sc.logits, sf.logits) / max(maxabs(sc.logits), 1.0e-6)
        t |> success(rel < REL_TOL, "flash logits within {REL_TOL} rel of classic (sp={start_pos}, got {rel})")
        if (confident) {
            t |> success(argmax(sf.logits) == argmax(sc.logits), "flash predicts the same next token as classic")
        }
    }
}

def run_all(t : T?; tr : Model; tk : Tokenizer) {
    let short <- encode(tk, "Once upon a time, there was a little girl", true, false)
    check_flash(t, tr, short, 0l, true)
    check_flash(t, tr, short, 4l, true)                            // continuation
    let lng <- [for (i in range64(200l)); i % 200l + 1l]
    check_flash(t, tr, lng, 0l, false)                             // long: threaded + multi-tile
    check_flash(t, tr, lng, 37l, false)                            // unaligned continuation
}

[test]
def test_flash_fp32(t : T?) {
    t |> run("flash ~= classic, fp32 (needs stories15M.bin)") @(t : T?) {
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
        run_all(t, tr, tk)
    }
}

[test]
def test_flash_q8(t : T?) {
    t |> run("flash ~= classic, Q8 + forced sliding window (needs stories15M.bin)") @(t : T?) {
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
        run_all(t, tr, tk)
        // exercise the flash sliding-window key-range bounds (Gemma2-style SWA)
        var trs <- load_checkpoint(MODEL_PATH)
        quantize_weights(trs)
        trs.config.sliding_window = 8l
        let lng <- [for (i in range64(200l)); i % 200l + 1l]
        check_flash(t, trs, lng, 0l, false)
    }
}
