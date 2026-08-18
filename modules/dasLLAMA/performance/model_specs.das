options gen2
options indenting = 4

// The model-set table — ONE source of truth for the model set; every catalog list is a view
// over it (the profiled llm_catalog, the official board, and — via profile_common, which also
// folds in asr_catalog — the models_provenance manifest). A deliberately light leaf: tests
// require it directly without paying profile_common's tool closure, and it defines no path
// helpers, so it never collides with a test harness's own models_dir.

module model_specs shared public

require strings public
require dasllama/dasllama_common public   // QuantMode

// ===== The model set (ONE table; every list below is a view over it) =====

//! One provenance-pinned artifact: local `name` under `root` ("llm" = DASLLAMA_MODELS_DIR,
//! "whisper" = WHISPER_CPP_MODELS), the exact source `url` ("" = converted on-box per
//! `recipe`), canonical `bytes` + `sha256` ("" = the conversion does real math, so bytes are
//! per-architecture — the gate is the recipe's output-parity command, never a hash).
struct ProvEntry {
    name : string
    root : string
    url : string
    bytes : int64
    sha256 : string
    recipe : string
}

//! A spec's frozen greedy parity fixture — evidence as DATA (pinned ids, never a test name).
//! Empty `gen` = no evidence. `prompt_text` (instead of `prompt`) = the prompt encodes in-test
//! through the model's own tokenizer. The bools name the extra pinned arms and load modes the
//! fixture was frozen under; the decoded-text eyeball is always derived via the tokenizer at
//! run time, never stored.
[safe_when_uninitialized]   // default init (empty ids, false arms) IS "no evidence"
struct ParityEvidence {
    prompt : array<int64>
    prompt_text : string
    gen : array<int64>
    quant : QuantMode = QuantMode.q8   // parity load quant (fp32 for F16/F32 carriers)
    kq_native : bool       // load with native K-quant planes forced on
    dn_chunked : bool      // + a chunked deltanet-prefill arm
    moe_reference : bool   // + a per-position reference MoE-prefill arm
    moe_per_expert : bool  // + a per-expert-chain (fused expert GEMMs off) prefill arm
    require_dn_kq : bool   // assert deltanet planes engaged kq-native, not silently demoted
}

//! One model of the set — the union row behind every view. A facet left empty means the model
//! does not participate in that view: `display` (the profiled catalog, [[llm_catalog]]),
//! `official` (the public board, [[official_catalog]]), `url`/`recipe` (the provenance
//! manifest, [[models_provenance]] — companions ride along), `parity.gen` (the frozen parity
//! evidence — the test loop and the board pregate).
struct ModelSpec {
    file : string          // gguf basename under [[models_dir]] — the primary key
    display : string       // scoreboard display name; "" = not profiled by gen_profile
    quant : QuantMode = QuantMode.q8
    quant_label : string   // scoreboard quant cell override ("" = quant_str) — Q4_K_M carriers load kq-native under q8 mode
    prefill_ctx : int = 512
    emission_ctx : int = 128
    official : bool        // on the public board (gen_bench_records)
    note : string          // board caveat, stamped into the model record ("" = none)
    url : string
    bytes : int64
    sha256 : string
    recipe : string
    companions : array<ProvEntry>   // artifacts fetched/verified with this model (mmproj, image fixture)
    parity : ParityEvidence
}

// ===== Parity fixture ids (frozen from the simple_ids oracle — harness/parity.sh) =====
// Prompts are deliberately the *confident* ones: high-entropy prompts flip on genuine near-ties
// (sub-ULP libm cos/sin/exp/tanh vs llama.cpp), which are not bugs. See dasllama_newmodels_arc.

// the Qwen BPE counting fixture, "1, 2, 3, 4, 5, 6, 7," (no BOS) — every Qwen-vocab carrier
// continues it identically (verified per-file against the live oracle before each reuse)
def private ids_qwen_count_prompt() : array<int64> {
    return <- [16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l,
               11l, 220l, 20l, 11l, 220l, 21l, 11l, 220l, 22l, 11l]
}

def private ids_qwen_count_gen() : array<int64> {
    return <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 16l, 15l, 11l,
               220l, 16l, 16l, 11l, 220l, 16l, 17l, 11l, 220l, 16l,
               18l, 11l, 220l, 16l, 19l, 11l, 220l, 16l, 20l, 11l,
               220l, 16l, 21l, 11l, 220l, 16l, 22l, 11l, 220l, 16l]
}

def private ev_qwen_count(kq_native : bool = false; dn_chunked : bool = false;
                          moe_reference : bool = false; require_dn_kq : bool = false) : ParityEvidence {
    return <- ParityEvidence(prompt <- ids_qwen_count_prompt(), gen <- ids_qwen_count_gen(),
        kq_native = kq_native, dn_chunked = dn_chunked, moe_reference = moe_reference,
        require_dn_kq = require_dn_kq)
}

// gemma-2 counting fixture, "1, 2, 3, 4, 5, 6, 7," (BOS=2): the old "Once upon a time"
// continuation flipped on a genuine near-tie under the PINNED classic+libm kernels when the
// tied classifier went Q8 (cls_q8). Re-frozen from simple_ids 2026-07-02, verified 40/40.
def private ev_gemma2() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [2l, 235274l, 235269l, 235248l, 235284l, 235269l, 235248l, 235304l, 235269l, 235248l,
                   235310l, 235269l, 235248l, 235308l, 235269l, 235248l, 235318l, 235269l, 235248l, 235324l,
                   235269l],
        gen <- [235248l, 235321l, 235269l, 235248l, 235315l, 235269l, 235248l, 235274l, 235276l, 235269l,
                235248l, 235274l, 235274l, 235269l, 235248l, 235274l, 235284l, 235269l, 235248l, 235274l,
                235304l, 235269l, 235248l, 235274l, 235310l, 235269l, 235248l, 235274l, 235308l, 235269l,
                235248l, 235274l, 235318l, 235269l, 235248l, 235274l, 235324l, 235269l, 235248l, 235274l])
}

// TinyLlama-1.1B-v0.3 F16, "Once upon a time" (BOS=1) — the old gguf_run.das oracle. Loaded
// fp32 (F16 -> fp32) to match how that oracle was produced.
def private ev_tinyllama_v03() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [1l, 9038l, 2501l, 263l, 931l],
        gen <- [29892l, 727l, 471l, 263l, 4123l, 7826l, 4257l, 365l, 2354l, 29889l,
                2296l, 471l, 263l, 1407l, 15040l, 322l, 12758l, 7826l, 29892l, 2337l,
                3063l, 363l, 716l, 322l, 5566l, 11407l, 2712l, 304l, 5110l, 29889l,
                3118l, 2462l, 29892l, 1183l, 380l, 25443l, 2501l, 263l, 3143l, 393l],
        quant = QuantMode.fp32)
}

// Llama-3.2-1B-Instruct Q8, [BOS]+"Once upon a time" (θ=500000 + llama3 freq-scaling) — the
// old llama3_run.das oracle (instrumented llama-simple / simple_ids CPU greedy).
def private ev_llama32_1b() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [128000l, 12805l, 5304l, 264l, 892l],
        gen <- [11l, 304l, 264l, 2678l, 14458l, 89777l, 304l, 279l, 20700l, 35231l,
                315l, 350l, 26089l, 3852l, 11l, 1070l, 12439l, 264l, 3995l, 3828l,
                7086l, 71471l, 13l, 71471l, 574l, 264l, 22999l, 323l, 68188l, 13836l,
                11l, 449l, 264l, 4851l, 2539l, 315l, 5895l, 323l, 264l, 4059l])
}

// SmolLM2-1.7B-Instruct Q8, "Once upon a time" (BPE, no BOS) — the unmodified llama arch.
def private ev_smollm2() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [6403l, 1980l, 253l, 655l],
        gen <- [28l, 281l, 253l, 1165l, 6560l, 32047l, 826l, 827l, 1109l, 8718l,
                28l, 665l, 4161l, 253l, 1805l, 44586l, 3365l, 414l, 3792l, 30l,
                2306l, 436l, 253l, 10632l, 392l, 10247l, 28l, 1343l, 327l, 874l,
                28418l, 18375l, 338l, 443l, 9086l, 1283l, 702l, 260l, 5703l, 335l])
}

// Mistral-7B-Instruct-v0.3 Q8, "Once upon a time" (SPM, BOS=1) — arch "llama" (v0.3 has no SWA).
def private ev_mistral7b() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [1l, 6481l, 4482l, 1032l, 1495l],
        gen <- [29493l, 1504l, 1171l, 1032l, 3286l, 1444l, 5928l, 5067l, 29491l, 5067l,
                1171l, 1032l, 2624l, 21635l, 1072l, 28981l, 4003l, 1461l, 5767l, 1054l,
                1070l, 3260l, 1032l, 1909l, 1122l, 3490l, 1065l, 1040l, 2294l, 1070l,
                2723l, 29491l, 1418l, 1321l, 1032l, 3598l, 2913l, 1065l, 1040l, 6282l])
}

// gemma-3 counting fixtures (5-local:1-global SWA + dual RoPE θ). The 4B counts cleanly from
// "1..7," (BOS=2); the 1B used to drop into prose after "10" — a prose tail that flipped a
// genuine near-tie when the box profile's tuned kernel perms went live — so its prompt counts
// to 23 and the continuation stays numeric for all 40 tokens (kernel-robust; oracle-refrozen
// 2026-07-02, verified pinned 40/40 both with and without a live box profile). The sliding
// MASK itself (kstart > 0) engages only past the 512-token window; validated separately with
// a ~900-token counting prompt (40/40) — see the wave-2 notes.
def private ev_gemma3_1b() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [2l, 236770l, 236764l, 236743l, 236778l, 236764l, 236743l, 236800l, 236764l, 236743l,
                   236812l, 236764l, 236743l, 236810l, 236764l, 236743l, 236825l, 236764l, 236743l, 236832l,
                   236764l, 236743l, 236828l, 236764l, 236743l, 236819l, 236764l, 236743l, 236770l, 236771l,
                   236764l, 236743l, 236770l, 236770l, 236764l, 236743l, 236770l, 236778l, 236764l, 236743l,
                   236770l, 236800l, 236764l, 236743l, 236770l, 236812l, 236764l, 236743l, 236770l, 236810l,
                   236764l, 236743l, 236770l, 236825l, 236764l, 236743l, 236770l, 236832l, 236764l, 236743l,
                   236770l, 236828l, 236764l, 236743l, 236770l, 236819l, 236764l, 236743l, 236778l, 236771l,
                   236764l, 236743l, 236778l, 236770l, 236764l, 236743l, 236778l, 236778l, 236764l, 236743l,
                   236778l, 236800l, 236764l],
        gen <- [236743l, 236778l, 236812l, 236764l, 236743l, 236778l, 236810l, 236764l, 236743l, 236778l,
                236825l, 236764l, 236743l, 236778l, 236832l, 236764l, 236743l, 236778l, 236828l, 236764l,
                236743l, 236778l, 236819l, 236764l, 236743l, 236800l, 236771l, 236764l, 236743l, 236800l,
                236770l, 236764l, 236743l, 236800l, 236778l, 236764l, 236743l, 236800l, 236800l, 236764l])
}

def private ev_gemma3_4b() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [2l, 236770l, 236764l, 236743l, 236778l, 236764l, 236743l, 236800l, 236764l, 236743l,
                   236812l, 236764l, 236743l, 236810l, 236764l, 236743l, 236825l, 236764l, 236743l, 236832l,
                   236764l],
        gen <- [236743l, 236828l, 236764l, 236743l, 236819l, 236764l, 236743l, 236770l, 236771l, 236764l,
                236743l, 236770l, 236770l, 236764l, 236743l, 236770l, 236778l, 236764l, 236743l, 236770l,
                236800l, 236764l, 236743l, 236770l, 236812l, 236764l, 236743l, 236770l, 236810l, 236764l,
                236743l, 236770l, 236825l, 236764l, 236743l, 236770l, 236832l, 236764l, 236743l, 236770l])
}

//! The gemma4 fixture prompt: "1, 2, …, 320," (~1490 tokens, byte-identical to the oracle
//! run's input) — encoded in-test through the model's tokenizer, so it gates the tokenizer AND
//! the forward, and the sliding window is genuinely engaged (kstart > 0). The short 21-token
//! counting prompt was rejected: its oracle continuation drifts into the channel format on a
//! near-tie that flips under the pinned classic+libm kernels.
def gemma4_counting_prompt() : string {
    return build_string() $(var w) {
        for (i in range(1, 321)) {
            w |> write(i)
            w |> write(i < 320 ? ", " : ",")
        }
    }
}

// the dense-gemma4 continuation (frozen on the 12B, shared by every dense gemma4 carrier —
// same vocab, all count; each reuse verified per-file against the oracle)
def private ev_gemma4_dense() : ParityEvidence {
    return <- ParityEvidence(prompt_text = gemma4_counting_prompt(),
        gen <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l,
                236743l, 236800l, 236778l, 236800l, 236764l, 236743l, 236800l, 236778l, 236812l, 236764l,
                236743l, 236800l, 236778l, 236810l, 236764l, 236743l, 236800l, 236778l, 236825l, 236764l,
                236743l, 236800l, 236778l, 236832l, 236764l, 236743l, 236800l, 236778l, 236828l, 236764l])
}

// the gemma4 MoE continuations, capped short: the 26B routes a DISCRETE top-8-of-128 per
// layer, and past the cap the *oracle itself* is near-tied (top-1 vs top-2 gap ~0.5-2.2) — an
// irreducible router coin-flip no float engine can pin, not a forward bug. Q8 holds 11
// oracle-confident tokens (gap >6), the Q4_K_M recipe 12. Details:
// history/dasLLAMA/model_expansion_plan.md (Wave G2).
def private ev_gemma4_26b() : ParityEvidence {
    return <- ParityEvidence(prompt_text = gemma4_counting_prompt(),
        gen <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l, 236743l])
}

def private ev_gemma4_26b_q4km() : ParityEvidence {
    return <- ParityEvidence(prompt_text = gemma4_counting_prompt(),
        gen <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l,
                236743l, 236800l],
        moe_per_expert = true)
}

// Mistral-Small-3.1-24B Q4_K_M, "1, 2, 3, 4, 5, 6, 7," (tekken BPE, BOS=1) — frozen from
// simple_ids 2026-08-18, das matched 40/40 at mint time (harness/parity.sh).
def private ev_mistral_small() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [1l, 1049l, 1044l, 1032l, 1050l, 1044l, 1032l, 1051l, 1044l, 1032l,
                   1052l, 1044l, 1032l, 1053l, 1044l, 1032l, 1054l, 1044l, 1032l, 1055l,
                   1044l],
        gen <- [1032l, 1056l, 1044l, 1032l, 1057l, 1044l, 1032l, 1049l, 1048l, 1044l,
                1032l, 1049l, 1049l, 1044l, 1032l, 1049l, 1050l, 1044l, 1032l, 1049l,
                1051l, 1044l, 1032l, 1049l, 1052l, 1044l, 1032l, 1049l, 1053l, 1044l,
                1032l, 1049l, 1054l, 1044l, 1032l, 1049l, 1055l, 1044l, 1032l, 1049l])
}

// gpt-oss-20b counting fixture, "1, 2, 3, 4, 5, 6, 7," (no BOS — gpt-4o pre adds none). The
// o200k prompt ids coincide with the Qwen fixtures' (shared low-ASCII BPE layout); the
// continuation diverges at "10" (one token here, digit-split there), so the arrays are kept
// separate. The window-engaged long fixture stays test-local (test_parity.das).
def private ev_gptoss() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l,
                   11l, 220l, 20l, 11l, 220l, 21l, 11l, 220l, 22l, 11l],
        gen <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 702l, 11l, 220l,
                994l, 11l, 220l, 899l, 11l, 220l, 1311l, 11l, 220l, 1265l,
                11l, 220l, 1055l, 11l, 220l, 1125l, 11l, 220l, 1422l, 11l,
                220l, 1157l, 11l, 220l, 858l, 11l, 220l, 455l, 11l, 220l])
}

// ===== The table =====

let HF = "https://huggingface.co"   // shared with profile_common's asr_catalog provenance entries

//! The q8 reference-carrier recipe: derived on-box by setup_asr_rig --refs (whisper.cpp
//! quantize over the fp bin beside it); size varies with the quantize tool's build, so
//! presence-gated, not sha-gated. fetch_models' --convert arm keys on this exact string.
let REF_Q8_RECIPE = "derive: setup_asr_rig.das --refs quantizes it from the fp bin beside it (whisper.cpp quantize q8_0); reference-side carrier, presence-gated"

let PARAKEET_RECIPE = "convert: <pinned whisper.cpp>/models/convert-parakeet-to-ggml.py --use-f32 over the nvidia/parakeet-tdt-0.6b .nemo (nemo venv; repack - sha canonical everywhere)"
let CANARY_ENC_RECIPE = "convert: modules/dasLLAMA/harness/convert_canary_to_ggml.py over nvidia/canary-qwen-2.5b (nemo venv; encoder is a repack - sha canonical everywhere)"
let CANARY_DEC_RECIPE = "convert: modules/dasLLAMA/harness/convert_canary_to_ggml.py over nvidia/canary-qwen-2.5b (nemo venv); bytes are per-arch (fp16 LoRA merge) - gate via asr_bench --text over jfk/jfk3/gb1 vs benchmarks/asr/canary_transcripts.expected"

// The text-model set, ascending size. Absent files skip at run time, so one table serves every
// box; each view measures/verifies the subset it actually has. Audio models live in
// [[asr_catalog]] (their provenance rides those specs into [[models_provenance]]).
def model_specs() : array<ModelSpec> {   // nolint:STYLE038 — flat model-set table, one entry per carrier
    return <- [
        ModelSpec(file = "SmolLM2-135M-Instruct-Q8_0.gguf", display = "SmolLM2-135M"),
        ModelSpec(file = "Qwen2.5-0.5B-Instruct-Q8_0.gguf", display = "Qwen2.5-0.5B"),
        ModelSpec(file = "Qwen3-0.6B-Q8_0.gguf", display = "Qwen3-0.6B",
            parity <- ev_qwen_count()),   // the QK-norm arch (per-head Q/K RMSNorm before RoPE)
        ModelSpec(file = "Qwen3.5-0.8B-Q8_0.gguf", display = "Qwen3.5-0.8B",
            // the qwen35 hybrid: 18 Gated-DeltaNet recurrent + 6 gated-attention layers
            // (2x-wide Q with sigmoid out-gate, partial NEOX rope 64/256, QK-norm)
            parity <- ev_qwen_count([dn_chunked = true])),
        // the tensor-arm twin carriers (tests/test_metal_support_matrix.das --arm tensor): small
        // on purpose - q8 drives the mulmm/attn twins, the K-quant one the kq mulmm twins. The
        // arm FAILS (not skips) when they are absent, so every box gets them provisioned here
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q5_K_M.gguf",
            url = "{HF}/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q5_K_M.gguf",
            bytes = 911503488l, sha256 = "4f22f95fb1679ef8e2fd6f1659d23f06cd1dbedb79ff291db7413a7ae509e2a9"),
        ModelSpec(file = "tinyllama-1.1b-v0.3-f16.gguf",   // parity-only carrier (F16 -> fp32 load)
            parity <- ev_tinyllama_v03()),
        ModelSpec(file = "gemma-3-1b-it-Q8_0.gguf", display = "gemma-3-1B",
            parity <- ev_gemma3_1b()),
        ModelSpec(file = "tinyllama-1.1b-chat-v1.0.Q8_0.gguf", display = "TinyLlama-1.1B"),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q8_0.gguf", display = "Llama-3.2-1B",
            url = "{HF}/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q8_0.gguf",
            bytes = 1321083008l, sha256 = "432f310a77f4650a88d0fd59ecdd7cebed8d684bafea53cbff0473542964f0c3",
            parity <- ev_llama32_1b()),
        ModelSpec(file = "Qwen2.5-1.5B-Instruct-Q8_0.gguf", display = "Qwen2.5-1.5B",
            parity <- ev_qwen_count()),
        ModelSpec(file = "SmolLM2-1.7B-Instruct-Q8_0.gguf", display = "SmolLM2-1.7B",
            parity <- ev_smollm2()),
        ModelSpec(file = "gemma-2-2b-it-Q8_0.gguf", display = "gemma-2-2B",
            parity <- ev_gemma2()),
        // the tool-format live-leg carriers (REVIEW recognition-test rule): each family's
        // smallest catalogued GGUF, exercised by utils/dasllama-server/test_openai_server_think.das
        ModelSpec(file = "Llama-3.2-3B-Instruct-Q4_K_M.gguf",
            url = "{HF}/bartowski/Llama-3.2-3B-Instruct-GGUF/resolve/main/Llama-3.2-3B-Instruct-Q4_K_M.gguf",
            bytes = 2019377696l, sha256 = "6c1a2b41161032677be168d354123594c0e6e67d2b9227c84f296ad037c728ff"),
        ModelSpec(file = "Llama-3.2-3B-Instruct-Q8_0.gguf", display = "Llama-3.2-3B"),
        ModelSpec(file = "gemma-4-E2B-it-Q4_K_M.gguf",
            url = "{HF}/unsloth/gemma-4-E2B-it-GGUF/resolve/main/gemma-4-E2B-it-Q4_K_M.gguf",
            bytes = 3106738272l, sha256 = "740185b21d22ceb83a11c3aa62ad5842ef32c70f6096d756bbee85a1e4ec34b8"),
        ModelSpec(file = "gemma-3-4b-it-Q8_0.gguf", display = "gemma-3-4B",
            parity <- ev_gemma3_4b()),
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q8_0.gguf", display = "Qwen3-4B",
            parity <- ev_qwen_count()),
        // the native K-quant rail's token-level gate: the same counting fixture over the
        // Q4_K_M / Q5_K_M / Q6_K files of the SAME model, forced through kquant_native planes
        // (grp repack + kq kernels + kq embed gather). Oracles produced per-file by simple_ids.
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", display = "Qwen3-4B-q4k", quant_label = "q4_k_m",
            parity <- ev_qwen_count([kq_native = true])),
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q5_K_M.gguf",
            parity <- ev_qwen_count([kq_native = true])),
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q6_K.gguf",
            parity <- ev_qwen_count([kq_native = true])),
        // the deltanet arm of the native-kq gate: recurrent-layer projections (attn_qkv /
        // attn_gate / ssm_out) ride kq-native planes on the CPU rail (Metal/GPU-tier loads
        // keep them q8). The trio spans k4/k5/k6 dn planes (qkv mixes Q6_K into the Q4/Q5
        // files); the engaged assert is per plane kind — a silently-demoted tag would
        // otherwise pass green on the q8 fallback rail.
        ModelSpec(file = "Qwen3.5-4B-Q4_K_M.gguf",
            parity <- ev_qwen_count([kq_native = true, dn_chunked = true, require_dn_kq = true])),
        ModelSpec(file = "Qwen3.5-4B-Q5_K_M.gguf",
            parity <- ev_qwen_count([kq_native = true, dn_chunked = true, require_dn_kq = true])),
        ModelSpec(file = "Qwen3.5-4B-Q6_K.gguf",
            parity <- ev_qwen_count([kq_native = true, dn_chunked = true, require_dn_kq = true])),
        ModelSpec(file = "Mistral-7B-Instruct-v0.3-Q4_K_M.gguf",
            url = "{HF}/bartowski/Mistral-7B-Instruct-v0.3-GGUF/resolve/main/Mistral-7B-Instruct-v0.3-Q4_K_M.gguf",
            bytes = 4372812000l, sha256 = "1270d22c0fbb3d092fb725d4d96c457b7b687a5f5a715abe1e818da303e562b6"),
        ModelSpec(file = "Mistral-7B-Instruct-v0.3-Q8_0.gguf", display = "Mistral-7B",
            parity <- ev_mistral7b()),
        // provenance for this carrier rides its asr_catalog spec (the audio-chat pairing owns
        // the mmproj companion); this row is the text-LLM profile + parity facet
        ModelSpec(file = "gemma-4-E2B-it-Q8_0.gguf", display = "gemma-4-E2B",
            // PLE + cross-layer KV sharing as the E4B, plus per-layer FFN width (the elastic
            // MatFormer sub-model); dense, so the full 40 hold
            parity <- ev_gemma4_dense()),
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf",   // parity-only carrier
            // the routed-experts arch (softmax router -> top-4 of 60 with un-renormalized
            // weights + a sigmoid-gated shared expert; QKV bias, NEOX rope); the fixture runs
            // BOTH MoE prefill paths off one load — bit-identical by construction
            parity <- ev_qwen_count([moe_reference = true])),
        ModelSpec(file = "gemma-4-12B-it-Q4_K_M.gguf", official = true,
            url = "{HF}/lmstudio-community/gemma-4-12B-it-GGUF/resolve/main/gemma-4-12B-it-Q4_K_M.gguf",
            bytes = 7381382944l, sha256 = "95d83ba36642b1f385fb906b5962a71763361be3bac930a709945f72d97473f8",
            // this file's own oracle (2026-08-18) continues the dense fixture byte-identically
            parity <- ev_gemma4_dense(),
            companions <- [
                ProvEntry(name = "mmproj-gemma-4-12B-it-BF16.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-4-12B-it-GGUF/resolve/main/mmproj-gemma-4-12B-it-BF16.gguf",
                    bytes = 175115616l, sha256 = "9b1edfa05b634728ca4bfd60b4e6b278e95166c078fa54ae4fa83e680112fd1d"),
                // path-style S3 spelling: the images.cocodataset.org website endpoint is http-only
                ProvEntry(name = "coco-val2017-000000039769.jpg", root = "llm",
                    url = "https://s3.amazonaws.com/images.cocodataset.org/val2017/000000039769.jpg",
                    bytes = 173131l, sha256 = "dea9e7ef97386345f7cff32f9055da4982da5471c48d575146c796ab4563b04e")]),
        ModelSpec(file = "gemma-4-E4B-it-Q8_0.gguf", display = "gemma-4-E4B", official = true,
            url = "{HF}/ggml-org/gemma-4-E4B-it-GGUF/resolve/main/gemma-4-E4B-it-Q8_0.gguf",
            bytes = 8031242688l, sha256 = "34be82b17b4942d389b9b527170c4b058027abdd32531fda063d3d97dd8ce80a",
            // PLE (per-layer embeddings) + cross-layer KV sharing over the last 18 layers;
            // dense, so the full 40 hold (verified 40/40 vs simple_ids on the window-engaged
            // 1..320 counting prompt)
            parity <- ev_gemma4_dense()),
        ModelSpec(file = "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf", display = "Llama-3.1-8B"),
        // gpt-oss is revision-pinned (upstream re-uploads bit-different files under the same
        // name); its parity fixtures run through test_parity's own shared-load arm — the
        // ~13GB native-mx4 resident is the suite's biggest, loaded once for both fixtures
        ModelSpec(file = "gpt-oss-20b-mxfp4.gguf", display = "gpt-oss-20b", quant_label = "mxfp4", official = true,
            url = "{HF}/ggml-org/gpt-oss-20b-GGUF/resolve/e1dc459feff949ff451ce107337a2026daa80df8/gpt-oss-20b-mxfp4.gguf",
            bytes = 12109566560l, sha256 = "be37a636aca0fc1aae0d32325f82f6b4d21495f06823b5fbc1898ae0303e9935",
            parity <- ev_gptoss()),
        ModelSpec(file = "Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf", official = true,
            url = "{HF}/unsloth/Mistral-Small-3.1-24B-Instruct-2503-GGUF/resolve/main/Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf",
            bytes = 14333910592l, sha256 = "6d670773c3908584349d41a5048d1472226b593c881fd394e8ac196c802e81e2",
            parity <- ev_mistral_small()),
        ModelSpec(file = "gemma-4-26B-A4B-it-Q8_0.gguf", display = "gemma-4-26B-A4B",
            // the gemma4 MoE member: every layer sums a DENSE shared expert and routed
            // top-8-of-128 experts under a custom-normalized router
            parity <- ev_gemma4_26b()),
        ModelSpec(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", display = "gemma-4-26B-A4B-q4k", quant_label = "q4_k_m", official = true,
            url = "{HF}/unsloth/gemma-4-26B-A4B-it-GGUF/resolve/main/gemma-4-26B-A4B-it-UD-Q4_K_M.gguf",
            bytes = 16947541728l, sha256 = "f2c28b3dc4776931ac6f879e11f203dec637ea0f14267a86ec8f6165f63f293f",
            // the q51 tier gate: this recipe's ffn_down_exps are Q5_1 on 29 layers, riding the
            // native q51 planes; the Q4_K gate_up stacks ride the kq planes
            parity <- ev_gemma4_26b_q4km()),
        // the QAT file rides the q40 KqFmt tier (native Q4_0 planes on the kq rails) — quant is
        // the loader mode (q8), the label the on-disk format, like the Q4_K_M rows
        ModelSpec(file = "gemma-4-26B_q4_0-it.gguf", display = "gemma-4-26B-A4B-q40", quant_label = "q4_0"),
        ModelSpec(file = "Qwen3.6-27B-MTP-Q4_K_M.gguf", official = true,
            url = "{HF}/unsloth/Qwen3.6-27B-MTP-GGUF/resolve/main/Qwen3.6-27B-Q4_K_M.gguf",
            bytes = 17106773120l, sha256 = "a7cbd3ecc0e3f9b333edee61ae66bc87ed713c5d49587a8355814722ed329e0f",
            // plain evidence only (verified per-file 40/40, 2026-08-18): the qwen35-dense arm
            // coverage (chunked deltanet) lives on the 0.8B/4B/27B carriers
            parity <- ev_qwen_count()),
        ModelSpec(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", display = "Qwen3-30B-A3B", quant_label = "q4_k_m", official = true,
            url = "{HF}/unsloth/Qwen3-30B-A3B-Instruct-2507-GGUF/resolve/main/Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf",
            bytes = 18556686752l, sha256 = "6c997b8af17debdfb01d890214400ccbab00db6acc0ba8da5de1cc906c4774d0",
            // the qwen3moe arch (QK-norm, softmax router -> RENORMALIZED top-8-of-128, no
            // shared expert) and the MoE arm of the native-kq gate: the grouped prefill
            // gathers per-expert token batches off the K-form batch image
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.8-27B-Q4_K_M.gguf", display = "Qwen3.8-27B", quant_label = "q4_k_m", official = true,
            url = "{HF}/ggml-org/Qwen3.8-27B-GGUF/resolve/main/Qwen3.8-27B-Q4_K_M.gguf",
            bytes = 18973870432l, sha256 = "31629f53165ab6a7dad8c9847dcfd1fdf55829dac1e6e748f4a68581b0033d34",
            // the qwen35 hybrid dense at scale: 48 deltanet + 16 gated-attention layers
            // (24Q/4KV at head 256, 48 v-heads x 128 deltanet), native K-quant planes
            parity <- ev_qwen_count([dn_chunked = true])),
        // the same UD-Q4_K_M weights under two local names: the MTP-stem file carries the MTP
        // head (the official-board carrier), the plain stem predates it (the profiled carrier)
        ModelSpec(file = "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", display = "Qwen3.6-35B-A3B", quant_label = "q4_k_m",
            // the qwen35moe hybrid: 30 deltanet + 10 gated-attention layers (16Q/2KV, v-heads
            // 32 over 16 k-heads — the modulo GQA-repeat), softmax top-8-of-256 renormalized +
            // sigmoid-gated shared expert, F32 ssm_beta/alpha, K-quant expert stacks
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", official = true,
            url = "{HF}/unsloth/Qwen3.6-35B-A3B-MTP-GGUF/resolve/main/Qwen3.6-35B-A3B-UD-Q4_K_M.gguf",
            bytes = 22663387424l, sha256 = "0b21525e972670ed59e1812e170b27c26355381f0656ecc4e25617ece7dac58b",
            // plain evidence only (verified per-file 40/40, 2026-08-18): the qwen35moe arm
            // coverage (chunked deltanet + reference MoE prefill) lives on the plain-stem twin
            parity <- ev_qwen_count()),
        ModelSpec(file = "Qwen3-Coder-30B-A3B-Instruct-Q8_0.gguf", display = "Qwen3-Coder-30B"),
        ModelSpec(file = "Qwen3-Coder-30B-A3B-Instruct-Q4_K_M.gguf", display = "Qwen3-Coder-30B-q4k", quant_label = "q4_k_m"),
        ModelSpec(file = "Qwen3-Coder-Next-Q4_K_M.gguf", display = "Qwen3-Coder-Next-q4k", quant_label = "q4_k_m",
            // arch qwen3next, the parent hybrid: the FILE layout differs from qwen35 (fused
            // Q4_K ssm_ba, HF grouped v-head order) — this fixture proves the loader's
            // grouped->tiled permutes (qkv v-rows, z gate, conv channels, a/dt, ba split,
            // ssm_out columns)
            parity <- ev_qwen_count([dn_chunked = true]))
    ]
}

//! The profiled text-LLM view over [[model_specs]] — the rows gen_profile / the baseline
//! establisher measure (a spec with a scoreboard display name).
def llm_catalog() : array<ModelSpec> {
    var all <- model_specs()
    var out : array<ModelSpec>
    for (s in all) {
        if (!empty(s.display)) {
            out |> emplace(s)
        }
    }
    delete all
    return <- out
}

//! The public-board view over [[model_specs]] — the rows gen_bench_records sweeps: community-
//! popular GGUFs, one popular quant each, NOT the internal quant matrix (curated to what
//! people run THIS year). Absent files skip with a warning, so one list serves every box.
def official_catalog() : array<ModelSpec> {
    var all <- model_specs()
    var out : array<ModelSpec>
    for (s in all) {
        if (s.official) {
            out |> emplace(s)
        }
    }
    delete all
    return <- out
}

//! The spec row keyed `file` (empty `file` field back = no such spec) — a fresh copy off the
//! table, so a caller (a test run-lambda, the bench parity pregate) holds no reference into an
//! iterated table.
def spec_by_file(file : string) : ModelSpec {
    var all <- model_specs()
    var out <- ModelSpec()
    for (s in all) {
        if (s.file == file) {
            out <- s
            break
        }
    }
    delete all
    return <- out
}
