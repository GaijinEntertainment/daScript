options gen2
options persistent_heap   // Mistral-7B's Q8 blob is a single >4GB array — over LinearChunkAllocator's per-chunk uint32 cap

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require daslib/fio
require strings

// Frozen token-for-token parity fixtures for the GGUF arches that stories15M (test_forward.das)
// doesn't cover. Each fixture is a prompt-ID list + the reference greedy continuation that
// llama.cpp's simple_ids oracle produced (regenerate with harness/parity.sh). The GGUF models are
// gitignored multi-GB files, so each case skips cleanly when its model is absent (a real local
// regression, a no-op in CI) — same contract as test_forward.das.
//
// Prompts are deliberately the *confident* ones: high-entropy prompts flip on genuine near-ties
// (sub-ULP libm cos/sin/exp/tanh vs llama.cpp), which are not bugs. See dasllama_newmodels_arc.

let MODELS_DIR = "/Users/borisbatkin/Work/llama.cpp/models/"

def private same(a, b : array<int64>) : bool {
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

// The pinned-kernel generate-and-compare core: greedily continue `prompt` on a loaded model and
// return whether the continuation matches `want` token-for-token. Kept free of T? so the [test]'s
// run()-lambda captures no array by move.
def private parity_gen_ok(tr : Model; prompt, want : array<int64>) : bool {
    let n_gen = length(want)
    var ok = false
    // Token-exact oracle gate => run the BIT-EXACT classic prefill attention, not the (default) flash:
    // flash's online-softmax FP reorder is not bit-exact and can flip a greedy token under the
    // interpreter (whose FP differs from JIT, where the fixtures were frozen). Flash staying within
    // tolerance is what test_flash covers. Restore the prior mode so we don't leak global state.
    let prev_attn = get_prefill_attn_mode()
    set_prefill_attn_mode(AttnPrefillMode.classic)
    // ...and the scalar libm silu/gelu, not the default exp4 gate (~2 ulp) — same token-exact-oracle
    // reasoning as the attention pin above. exp4 staying within tolerance is covered by test_silu.
    let prev_act = get_act_vec()
    set_act_vec(false)
    with_job_que() {
        set_jobque_fork_pool(true, true)
        var s = make_run_state(tr.config)
        let ids <- generate(tr, s, prompt, length(prompt) + n_gen)
        // generate echoes prompt[1..] then the continuation, so the first generated token is at
        // index length(prompt)-1 (the same slice harness/parity.das compares)
        let echo = length(prompt) - 1
        let got <- [for (i in range(n_gen)); ids[echo + i]; where echo + i < length(ids)]
        ok = same(got, want)
    }
    set_prefill_attn_mode(prev_attn)
    set_act_vec(prev_act)
    return ok
}

// Load `path` at `quant` and gate the pre-tokenized `prompt` fixture against `want`.
def private parity_ok(path : string; quant : QuantMode; prompt, want : array<int64>) : bool {
    var tr <- load_gguf(path, quant)
    // Cap the KV context: only ~(prompt + want) positions are used, and the KV stride doesn't change
    // the computed tokens — but Llama-3's native 131072 makes the KV array exceed the 4GB uint32 cap.
    if (tr.config.seq_len > 2048l) {
        tr.config.seq_len = 2048l
    }
    return parity_gen_ok(tr, prompt, want)
}

// gemma-2-2b-it Q8_0, prompt "Once upon a time" (BOS=2). Confident — clean 40/40 vs oracle.
let GEMMA2_PROMPT <- [2l, 14326l, 3054l, 476l, 1069l]
let GEMMA2_GEN <- [235269l, 575l, 476l, 2840l, 10545l, 675l, 166962l, 27836l, 578l, 121048l,
                   17200l, 235269l, 11012l, 476l, 3486l, 4602l, 8602l, 2810l, 1610l, 235265l,
                   2810l, 1610l, 729l, 3836l]

[test]
def test_parity_gemma2(t : T?) {
    t |> run("gemma2 forward (gemma-2-2b-it Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, GEMMA2_PROMPT, GEMMA2_GEN), "gemma2 token-for-token")
    }
}

// TinyLlama-1.1B-v0.3 F16, "Once upon a time" (BOS=1) — the old gguf_run.das oracle (llama.cpp greedy
// on the same F16 file). Loaded fp32 (F16 -> fp32) to match how that oracle was produced.
let TINYLLAMA_V03_PROMPT <- [1l, 9038l, 2501l, 263l, 931l]
let TINYLLAMA_V03_GEN <- [29892l, 727l, 471l, 263l, 4123l, 7826l, 4257l, 365l, 2354l, 29889l,
                          2296l, 471l, 263l, 1407l, 15040l, 322l, 12758l, 7826l, 29892l, 2337l,
                          3063l, 363l, 716l, 322l, 5566l, 11407l, 2712l, 304l, 5110l, 29889l,
                          3118l, 2462l, 29892l, 1183l, 380l, 25443l, 2501l, 263l, 3143l, 393l]

[test]
def test_parity_tinyllama_v03(t : T?) {
    t |> run("llama forward (TinyLlama-1.1B-v0.3 F16 -> fp32)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}tinyllama-1.1b-v0.3-f16.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.fp32, TINYLLAMA_V03_PROMPT, TINYLLAMA_V03_GEN), "tinyllama v0.3 token-for-token")
    }
}

// Llama-3.2-1B-Instruct Q8, [BOS]+"Once upon a time" (θ=500000 + llama3 freq-scaling) — the old
// llama3_run.das oracle (instrumented llama-simple / simple_ids CPU greedy).
let LLAMA32_1B_PROMPT <- [128000l, 12805l, 5304l, 264l, 892l]
let LLAMA32_1B_GEN <- [11l, 304l, 264l, 2678l, 14458l, 89777l, 304l, 279l, 20700l, 35231l,
                       315l, 350l, 26089l, 3852l, 11l, 1070l, 12439l, 264l, 3995l, 3828l,
                       7086l, 71471l, 13l, 71471l, 574l, 264l, 22999l, 323l, 68188l, 13836l,
                       11l, 449l, 264l, 4851l, 2539l, 315l, 5895l, 323l, 264l, 4059l]

[test]
def test_parity_llama3_2_1b(t : T?) {
    t |> run("llama forward (Llama-3.2-1B-Instruct Q8, θ=500000)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Llama-3.2-1B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, LLAMA32_1B_PROMPT, LLAMA32_1B_GEN), "llama-3.2-1b token-for-token")
    }
}

// Qwen2.5-1.5B-Instruct Q8, prompt "1, 2, 3, 4, 5, 6, 7," (no BOS). The counting prompt is the
// confident one — prose prompts flip on genuine near-ties vs the oracle's libm/kernels (~0.02
// logit gaps; documented in the README oracle notes), which a frozen fixture must not depend on.
let QWEN25_15B_PROMPT <- [16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l,
                          11l, 220l, 20l, 11l, 220l, 21l, 11l, 220l, 22l, 11l]
let QWEN25_15B_GEN <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 16l, 15l, 11l,
                       220l, 16l, 16l, 11l, 220l, 16l, 17l, 11l, 220l, 16l,
                       18l, 11l, 220l, 16l, 19l, 11l, 220l, 16l, 20l, 11l,
                       220l, 16l, 21l, 11l, 220l, 16l, 22l, 11l, 220l, 16l]

[test]
def test_parity_qwen25_15b(t : T?) {
    t |> run("qwen2 forward (Qwen2.5-1.5B-Instruct Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Qwen2.5-1.5B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, QWEN25_15B_PROMPT, QWEN25_15B_GEN), "qwen2.5-1.5b token-for-token")
    }
}

// Phi-3.5-mini-instruct Q8, prompt "1, 2, 3, 4, 5, 6, 7," (no BOS) — same confident-prompt
// reasoning as the Qwen fixture (Phi matches prose to ~0.06 logits and flips only near-ties).
let PHI35_PROMPT <- [29871l, 29896l, 29892l, 29871l, 29906l, 29892l, 29871l, 29941l, 29892l, 29871l,
                     29946l, 29892l, 29871l, 29945l, 29892l, 29871l, 29953l, 29892l, 29871l, 29955l, 29892l]
let PHI35_GEN <- [29871l, 29947l, 29892l, 29871l, 29929l, 29892l, 29871l, 29896l, 29900l, 29892l,
                  29871l, 29896l, 29896l, 29892l, 29871l, 29896l, 29906l, 29892l, 29871l, 29896l,
                  29941l, 29892l, 29871l, 29896l, 29946l, 29892l, 29871l, 29896l, 29945l, 29892l,
                  29871l, 29896l, 29953l, 29892l, 29871l, 29896l, 29955l, 29892l, 29871l, 29896l]

[test]
def test_parity_phi35(t : T?) {
    t |> run("phi3 forward (Phi-3.5-mini-instruct Q8, fused QKV + LongRoPE)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Phi-3.5-mini-instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, PHI35_PROMPT, PHI35_GEN), "phi-3.5 token-for-token")
    }
}

// SmolLM2-1.7B-Instruct Q8, "Once upon a time" (BPE, no BOS) — runs on the unmodified llama arch.
let SMOLLM2_PROMPT <- [6403l, 1980l, 253l, 655l]
let SMOLLM2_GEN <- [28l, 281l, 253l, 1165l, 6560l, 32047l, 826l, 827l, 1109l, 8718l,
                    28l, 665l, 4161l, 253l, 1805l, 44586l, 3365l, 414l, 3792l, 30l,
                    2306l, 436l, 253l, 10632l, 392l, 10247l, 28l, 1343l, 327l, 874l,
                    28418l, 18375l, 338l, 443l, 9086l, 1283l, 702l, 260l, 5703l, 335l]

[test]
def test_parity_smollm2(t : T?) {
    t |> run("llama forward (SmolLM2-1.7B-Instruct Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}SmolLM2-1.7B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, SMOLLM2_PROMPT, SMOLLM2_GEN), "smollm2 token-for-token")
    }
}

// Mistral-7B-Instruct-v0.3 Q8, "Once upon a time" (SPM, BOS=1) — arch "llama" (v0.3 has no SWA).
let MISTRAL7B_PROMPT <- [1l, 6481l, 4482l, 1032l, 1495l]
let MISTRAL7B_GEN <- [29493l, 1504l, 1171l, 1032l, 3286l, 1444l, 5928l, 5067l, 29491l, 5067l,
                      1171l, 1032l, 2624l, 21635l, 1072l, 28981l, 4003l, 1461l, 5767l, 1054l,
                      1070l, 3260l, 1032l, 1909l, 1122l, 3490l, 1065l, 1040l, 2294l, 1070l,
                      2723l, 29491l, 1418l, 1321l, 1032l, 3598l, 2913l, 1065l, 1040l, 6282l]

[test]
def test_parity_mistral7b(t : T?) {
    t |> run("llama forward (Mistral-7B-Instruct-v0.3 Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Mistral-7B-Instruct-v0.3-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, MISTRAL7B_PROMPT, MISTRAL7B_GEN), "mistral-7b-v0.3 token-for-token")
    }
}

// Qwen3-0.6B + Qwen3-4B-Instruct-2507 Q8, "1, 2, 3, 4, 5, 6, 7," (no BOS) — the QK-norm arch
// (per-head Q/K RMSNorm before RoPE). Same confident counting prompt as the other Qwen fixtures;
// both models continue it identically, and the ids equal the Qwen2.5 fixture (same BPE vocab).
let QWEN3_PROMPT <- [16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l,
                     11l, 220l, 20l, 11l, 220l, 21l, 11l, 220l, 22l, 11l]
let QWEN3_GEN <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 16l, 15l, 11l,
                  220l, 16l, 16l, 11l, 220l, 16l, 17l, 11l, 220l, 16l,
                  18l, 11l, 220l, 16l, 19l, 11l, 220l, 16l, 20l, 11l,
                  220l, 16l, 21l, 11l, 220l, 16l, 22l, 11l, 220l, 16l]

[test]
def test_parity_qwen3_06b(t : T?) {
    t |> run("qwen3 forward (Qwen3-0.6B Q8, QK-norm)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Qwen3-0.6B-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, QWEN3_PROMPT, QWEN3_GEN), "qwen3-0.6b token-for-token")
    }
}

[test]
def test_parity_qwen3_4b(t : T?) {
    t |> run("qwen3 forward (Qwen3-4B-Instruct-2507 Q8, QK-norm)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}Qwen3-4B-Instruct-2507-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, QWEN3_PROMPT, QWEN3_GEN), "qwen3-4b token-for-token")
    }
}

// gemma-3-1b-it + gemma-3-4b-it Q8, "1, 2, 3, 4, 5, 6, 7," (BOS=2) — the per-layer attention-pattern
// arch (5-local:1-global SWA + dual RoPE θ; the 4B adds linear-8 position scaling on global layers).
// Same counting prompt, but the models diverge after "10" (the 1B drops into prose), so each keeps
// its own continuation. The sliding MASK itself (kstart > 0) engages only past the 512-token window;
// that was validated separately with a ~900-token counting prompt (40/40) — see the wave-2 notes.
let GEMMA3_PROMPT <- [2l, 236770l, 236764l, 236743l, 236778l, 236764l, 236743l, 236800l, 236764l, 236743l,
                      236812l, 236764l, 236743l, 236810l, 236764l, 236743l, 236825l, 236764l, 236743l, 236832l,
                      236764l]
let GEMMA3_1B_GEN <- [236743l, 236828l, 236764l, 236743l, 236819l, 236764l, 236743l, 236770l, 236771l, 236761l,
                      108l, 2094l, 563l, 496l, 3606l, 7501l, 529l, 4945l, 236761l, 138l,
                      1509l, 236789l, 236751l, 3187l, 1456l, 618l, 496l, 6250l, 1523l, 573l,
                      4735l, 1003l, 17047l, 236761l, 108l, 6294l, 611l, 1461l, 786l, 531l]
let GEMMA3_4B_GEN <- [236743l, 236828l, 236764l, 236743l, 236819l, 236764l, 236743l, 236770l, 236771l, 236764l,
                      236743l, 236770l, 236770l, 236764l, 236743l, 236770l, 236778l, 236764l, 236743l, 236770l,
                      236800l, 236764l, 236743l, 236770l, 236812l, 236764l, 236743l, 236770l, 236810l, 236764l,
                      236743l, 236770l, 236825l, 236764l, 236743l, 236770l, 236832l, 236764l, 236743l, 236770l]

[test]
def test_parity_gemma3_1b(t : T?) {
    t |> run("gemma3 forward (gemma-3-1b-it Q8, SWA pattern + dual rope)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-3-1b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, GEMMA3_PROMPT, GEMMA3_1B_GEN), "gemma3-1b token-for-token")
    }
}

[test]
def test_parity_gemma3_4b(t : T?) {
    t |> run("gemma3 forward (gemma-3-4b-it Q8, SWA pattern + dual rope + linear scale)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-3-4b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_ok(path, QuantMode.q8, GEMMA3_PROMPT, GEMMA3_4B_GEN), "gemma3-4b token-for-token")
    }
}

// gemma-4-12B-it Q8 — the heterogeneous-geometry arch: sliding layers 16Q/8KV heads of 256 vs
// global 16Q/1KV of 512 (per-layer bool SWA pattern), p-RoPE factors on global layers only,
// weightless V-norm, V-from-K on global layers, unit attention scale, per-layer output scale,
// final softcap 30, suppressed <image|>/<audio|> logits. The prompt is "1, 2, …, 320," (~1490
// tokens) ENCODED IN-TEST through the model's gemma4 tokenizer (verified id-for-id against the
// oracle's tokenization), so this gates the tokenizer AND the forward, and the 1024-token sliding
// window is genuinely engaged (kstart > 0). The short 21-token counting prompt was rejected as a
// fixture: its oracle continuation drifts into the channel format on a near-tie that flips under
// the pinned classic+libm kernels — the long prompt's list momentum is kernel-robust.
let GEMMA4_12B_GEN <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l,
                       236743l, 236800l, 236778l, 236800l, 236764l, 236743l, 236800l, 236778l, 236812l, 236764l,
                       236743l, 236800l, 236778l, 236810l, 236764l, 236743l, 236800l, 236778l, 236825l, 236764l,
                       236743l, 236800l, 236778l, 236832l, 236764l, 236743l, 236800l, 236778l, 236828l, 236764l]

// The gemma4 fixture prompt: "1, 2, …, 320," — byte-identical to the oracle run's input.
def private gemma4_counting_prompt() : string {
    return build_string() $(var w) {
        for (i in range(1, 321)) {
            w |> write(i)
            w |> write(i < 320 ? ", " : ",")
        }
    }
}

// Text-prompt variant of parity_ok for fixtures that encode in-test (needs the tokenizer, so
// load_model instead of load_gguf); same seq cap + pinned bit-exact kernels via parity_gen_ok.
def private parity_text_ok(path : string; quant : QuantMode; text : string; want : array<int64>) : bool {
    var tr <- load_model(path, quant)
    if (tr.config.seq_len > 2048l) {
        tr.config.seq_len = 2048l
    }
    let prompt <- encode(tr, text, true, false)
    return parity_gen_ok(tr, prompt, want)
}

[test]
def test_parity_gemma4_12b(t : T?) {
    t |> run("gemma4 forward (gemma-4-12B-it Q8, heterogeneous SWA/global geometry, window engaged)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-4-12B-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        t |> success(parity_text_ok(path, QuantMode.q8, gemma4_counting_prompt(), GEMMA4_12B_GEN), "gemma4-12b token-for-token")
    }
}
