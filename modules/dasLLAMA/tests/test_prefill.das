options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama
require _model_tier

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require dasllama/dasllama_spm
require daslib/jobque_boost
require daslib/fio
require daslib/linq   // sequence_equal: the logits rows and the codec-encoded KV byte blobs compare element for element (byte equality IS bit-exactness for a same-dtype pair)
require dasllama/dasllama_math  // with_dasllama_jobque_ + the runtime knob setters (engine-level, not re-exported)
require math
require _compares   // mismatch_rel, logits_maxdiff: the tile arm's bar

// forward_prefill / forward_batch must be BIT-FOR-BIT identical to calling forward() once per
// position: same last-position logits AND same KV cache — both for a from-scratch prompt and for a
// prefill that continues an already-populated cache (start_pos > 0, the multi-turn-chat path). The
// fp32 model's batched GEMMs ride the tile walk, which rounds in its lanes
// (ARCHITECTURE_CPU_KERNELS.md#f32-batch-tile-lattice), so its bit-exact cells pin the row nest - the
// same dot on both paths, as the Q8 cell's portable pin is - and one tile cell holds the shipped walk's
// last-position logits within PREFILL_TILE_REL of Nx forward.
// Needs llama2.c's stories15M.bin (gitignored, 60MB); skips cleanly when absent.
// override the machine-local llama2.c checkout with DASLLAMA_LLAMA2C_DIR (e.g. on the x64 handoff box)
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")
let TOKENIZER_PATH = path_join(llama2c_dir(), "tokenizer.bin")
let PREFILL_TILE_REL = 1.0e-4   // ten times the 1e-5 six layers of the tile's lane rounding put on the 15M model's logits

// the shipped tile walk: forward_batch lands other bits than Nx forward (the control that the arm left
// the nest), the last-position logits within the tile's rounding
def check_prefill_tile(t : T?; tr : Model; prompt : array<int64>) {
    let c = tr.config
    let n = long_length(prompt)
    with_dasllama_jobque_() {
        set_prefill_attn_mode(AttnPrefillMode.classic)
        var sr <- make_run_state(c)
        for (p in range64(n)) {
            forward(tr, sr, prompt[p], p)
        }
        var sp <- make_run_state(c)
        forward_batch(tr, sp, prompt, n, 0l)
        to_log(LOG_INFO, "tile prefill: last-position logits off Nx forward by {logits_maxdiff(sp.logits, sr.logits)} over logits up to {logits_maxabs(sr.logits)}\n")
        t |> equal(mismatch_rel("tile prefill logits", sp.logits, sr.logits, PREFILL_TILE_REL), 0, "tile walk: last-position logits within {PREFILL_TILE_REL} of Nx forward")
        t |> success(!sequence_equal(sr.logits, sp.logits), "control: the tile walk lands other bits than Nx forward")
        delete sr
        delete sp
    }
}

// forward_batch over the whole prompt (one batched pass from position 0) == N× forward.
def check_prefill(t : T?; tr : Model; prompt : array<int64>; mode : AttnPrefillMode) {
    let c = tr.config
    let n = long_length(prompt)
    with_dasllama_jobque_() {
        set_prefill_attn_mode(mode)
        var sr <- make_run_state(c)
        for (p in range64(n)) {
            forward(tr, sr, prompt[p], p)
        }
        var sp <- make_run_state(c)
        forward_batch(tr, sp, prompt, n, 0l)
        t |> success(sequence_equal(sr.logits, sp.logits), "{mode} last-position logits bit-exact")
        t |> success(sequence_equal(sr.key_cache, sp.key_cache), "{mode} key cache bit-exact")
        t |> success(sequence_equal(sr.value_cache, sp.value_cache), "{mode} value cache bit-exact")
        delete sr
        delete sp
    }
}

// Prefill that CONTINUES a populated cache: warm positions 0..split-1 one token at a time, then
// prefill the tail at start_pos=split. Must match running all positions per-token.
def check_continuation(t : T?; tr : Model; prompt : array<int64>; split : int64; mode : AttnPrefillMode) {
    let c = tr.config
    let n = long_length(prompt)
    with_dasllama_jobque_() {
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
        t |> success(sequence_equal(sr.logits, sp.logits), "{mode} continuation last-position logits bit-exact")
        t |> success(sequence_equal(sr.key_cache, sp.key_cache), "{mode} continuation key cache bit-exact")
        t |> success(sequence_equal(sr.value_cache, sp.value_cache), "{mode} continuation value cache bit-exact")
        delete sr
        delete sp
    }
}

[test(description = "On fp32 stories15M with the row nest pinned, `forward_batch` and a continuation prefill match per-token `forward` bit for bit in logits and KV cache, and the shipped tile walk stays within 1e-4 per-element relative")]
def test_prefill_fp32(t : T?) {
    t |> run("prefill == Nx forward, fp32 (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        let long_prompt <- [for (i in range64(200l)); i % 200l + 1l]
        let min_n_was = get_f32_batch_tile_min_n()
        set_f32_batch_tile_min_n(1l << 30)   // the row nest: the per-token dot on both paths
        check_prefill(t, tr, prompt, AttnPrefillMode.classic)
        check_continuation(t, tr, prompt, 4l, AttnPrefillMode.classic)
        check_prefill(t, tr, long_prompt, AttnPrefillMode.classic)   // 200 tok trips g_attn_par_threshold (threaded arm)
        set_f32_batch_tile_min_n(min_n_was)
        check_prefill_tile(t, tr, prompt)
        delete tk
        delete tr
    }
}

[test(description = "On Q8 stories15M under the portable backend, batched prefill of a short and a 200-token prompt and a start_pos 4 continuation match per-token `forward` bit for bit in last logits and KV cache")]
def test_prefill_q8(t : T?) {
    t |> run("prefill == Nx forward, Q8 (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
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
        moe_gpu_drop_model()   // the claim is the CPU layer loop against forward(): an armed per-op tier would serve the Q8 planes' batched GEMMs on the device
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        let prompt <- encode(tk, "Once upon a time, there was a little girl", true, false)
        let long_prompt <- [for (i in range64(200l)); i % 200l + 1l]
        check_prefill(t, tr, prompt, AttnPrefillMode.classic)
        check_continuation(t, tr, prompt, 4l, AttnPrefillMode.classic)
        check_prefill(t, tr, long_prompt, AttnPrefillMode.classic)   // 200 tok trips g_attn_par_threshold (threaded arm)
        clear_kernel_backend_pin()
        delete tk
        delete tr
    }
}

[test(description = "On q4_0 stories15M the batched prefill through the Q4_0 GEMM and a start_pos 4 continuation match per-token `forward` bit for bit in last logits and KV cache")]
def test_prefill_q4_batched(t : T?) {
    t |> run("q4_0 prefill rides the batched stack and matches (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        // the q4_0 tier's prefill rides the batched stack through the Q4_0 GEMM - the same dot per
        // (row, token) as the decode GEMV, so the prefill and the per-token loop agree
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


// Flash prefill attention is online-softmax (NOT bit-exact vs classic — the FP sums reorder), so it
// is gated on (a) the predicted next token matching classic on a confident prompt and (b) the full
// logit vector staying within a small relative tolerance of classic. The token-for-token oracle for
// flash is covered by test_parity / test_forward, which run through flash as the default mode.
// Needs llama2.c's stories15M.bin (gitignored); skips cleanly when absent.

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
    let n = long_length(prompt)
    let REL_TOL = 0.05
    with_dasllama_jobque_() {
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
        let rel = logits_maxdiff(sc.logits, sf.logits) / max(logits_maxabs(sc.logits), 1.0e-6)
        t |> success(rel < REL_TOL, "flash logits within {REL_TOL} rel of classic (sp={start_pos}, got {rel})")
        if (confident) {
            t |> success(argmax(sf.logits) == argmax(sc.logits), "flash predicts the same next token as classic")
        }
        delete sc
        delete sf
    }
}

def run_all(t : T?; tr : Model; tk : SpmTokenizer) {
    let short <- encode(tk, "Once upon a time, there was a little girl", true, false)
    check_flash(t, tr, short, 0l, true)
    check_flash(t, tr, short, 4l, true)                            // continuation
    let lng <- [for (i in range64(200l)); i % 200l + 1l]
    check_flash(t, tr, lng, 0l, false)                             // long: threaded + multi-tile
    check_flash(t, tr, lng, 37l, false)                            // unaligned continuation
}

[test(description = "On fp32 stories15M, flash prefill attention lands within 5 percent relative max-logit of classic on short and 200-token prompts from position 0 and mid-sequence continuations, predicting the same next token on the confident prompt")]
def test_flash_fp32(t : T?) {
    t |> run("flash ~= classic, fp32 (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        run_all(t, tr, tk)
        delete tk
        delete tr
    }
}

[test(description = "On Q8-quantized stories15M, flash prefill stays within 5 percent relative max-logit of classic on the same prompts and continuations, and also under a forced 8-token sliding window")]
def test_flash_q8(t : T?) {
    t |> run("flash ~= classic, Q8 + forced sliding window (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid || !stat(TOKENIZER_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        var tk <- load_tokenizer(TOKENIZER_PATH, tr.config.vocab_size)
        run_all(t, tr, tk)
        // exercise the flash sliding-window key-range bounds (Gemma2-style SWA)
        var trs <- load_checkpoint(MODEL_PATH)
        quantize_weights(trs)
        trs.config.sliding_window = 8l
        let lng <- [for (i in range64(200l)); i % 200l + 1l]
        check_flash(t, trs, lng, 0l, false)
        delete tk
        delete tr
        delete trs
    }
}
