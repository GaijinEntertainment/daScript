options gen2
options indenting = 4
options _dasllama_internal = true

// ONE source of truth for the text model set: llm_catalog / official_catalog are views below;
//   models_provenance (profile_common, folding in asr_catalog) is the third.
// A light leaf on purpose: tests require it without profile_common's tool closure.
// Defines no path helpers, so it never collides with a test harness's own models_dir.

module model_specs shared public

require dasllama/dasllama_lint public
require strings public
require daslib/fio   // base_name - spec_by_file's split-shard rows
require dasllama/dasllama_common public   // QuantMode

// ===== Row types =====

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
    quantize : QuantizeRecipe   //! a row `fetch_models --convert` mints with llama-quantize; an empty `fmt` = none
}

//! A row minted on-box by llama-quantize from a fetched row: the arguments the row's `recipe` text and the mint both read
//! through ``quantize_commands``, so the two cannot drift
struct QuantizeRecipe {
    source : string       //! the fetched row the planes come from
    imatrix : string      //! the importance matrix built once from `source` over the calibration text
    fmt : string          //! llama-quantize's type: every plane in it (`--pure`)
    token_type : string   //! the token table's and the output's type
}

//! The two commands minting `file` from `q`, every path under `root`: the importance matrix over `calib`, then the
//! quantize. The tools as given: bare names in a row's recipe, the reference build's binaries in the mint.
def quantize_commands(q : QuantizeRecipe; file, root, calib, imatrix_tool, quantize_tool : string) : tuple<imatrix : string; quantize : string> {
    let src = "{root}{q.source}"
    let imx = "{root}{q.imatrix}"
    return (imatrix = "{imatrix_tool} -m \"{src}\" -f \"{calib}\" -o \"{imx}\" -c 128 --chunks 64 -ngl 0",
        quantize = "{quantize_tool} --imatrix \"{imx}\" --pure --token-embedding-type {q.token_type} --output-tensor-type {q.token_type} \"{src}\" \"{root}{file}\" {q.fmt}")
}

//! What `fetch_models --convert` runs for an absent row: the quantize mint, the canary conversion, or nothing - a Windows box
//! and a row with no recipe of either kind refuse
enum ConvertRoute {
    refuse_platform
    quantize
    canary
    no_recipe
}

def convert_route(e : ProvEntry; platform : string) : ConvertRoute {
    return ConvertRoute.refuse_platform if (platform == "windows")
    return ConvertRoute.quantize if (!empty(e.quantize.fmt))
    return ConvertRoute.canary if (find(e.name, "canary") >= 0)
    return ConvertRoute.no_recipe
}

//! A quantize mint's steps on a box where its source and its importance matrix are or are not on disk: an absent source
//! refuses, an absent matrix is built before the quantize
enum MintSteps {
    refuse_no_source
    imatrix_then_quantize
    quantize
}

def mint_steps(source_present, imatrix_present : bool) : MintSteps {
    return MintSteps.refuse_no_source if (!source_present)
    return imatrix_present ? MintSteps.quantize : MintSteps.imatrix_then_quantize
}

//! A format-ladder row (the models dir's fmt-ladder/): `stem`'s f16 row with every plane in `fmt`, its token table and
//! output in `token_type` (`fmt`'s own by default)
def private ladder_row(stem, fmt : string; token_type : string = "") : ModelSpec {
    var row = ModelSpec(file = "fmt-ladder/{stem}-{fmt}-pure-local.gguf",
        quantize = QuantizeRecipe(source = "{stem}-f16.gguf", imatrix = "fmt-ladder/{stem}.imatrix", fmt = fmt,
            token_type = empty(token_type) ? to_lower(fmt) : token_type))
    let steps = quantize_commands(row.quantize, row.file, "", "calib.txt", "llama-imatrix", "llama-quantize")
    row.recipe = "{steps.imatrix}; {steps.quantize}"
    return <- row
}

//! A layer cut (harness/gguf_cut_layers.py): `src`'s first `blocks`
//! blocks (and its NextN block, when it has one) as `{stem}-{blocks}L-{fmt}-local.gguf` in the format repo's
//! `dir`; `lift_softcap` lifts gemma-4's final soft cap, which would
//! pin most of a cut's logits
def private cut_row(dir, src, stem, fmt : string; blocks : int; lift_softcap : bool; bytes : int64;
                    sha256 : string) : ModelSpec {
    let file = "{stem}-{blocks}L-{fmt}-local.gguf"
    let lift = lift_softcap ? " --set gemma4.final_logit_softcapping=100000" : ""
    return ModelSpec(file = file, url = "{FORMATS_HF}/{dir}/{file}", bytes = bytes,
        sha256 = sha256, recipe = "harness/gguf_cut_layers.py {src} {file} {blocks}{lift}")
}

//! A spec's frozen greedy parity fixture — evidence as DATA (pinned ids, never a test name).
//! Empty `gen` = no evidence. `prompt_text` (instead of `prompt`) = the prompt encodes in-test
//! through the model's own tokenizer; the decoded-text eyeball is derived the same way, never stored.
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
    prefix_bar : bool      // the gates hold the fixture as a matching prefix of half the span, never token for token
}

//! how many of `got`'s leading ids equal `want`'s - the one compare every fixture gate reads
def prefix_match_len(got, want : array<int64>) : int64 {
    var same = 0l
    while (same < long_length(got) && same < long_length(want) && got[same] == want[same]) {
        same++
    }
    return same
}

//! whether `same` matching opening tokens of `n_check` pass the fixture's bar: half the span as a prefix under
//! `prefix_bar` (the timed run's fixture gate asks that form of every carrier), else every token
def fixture_ok(prefix_bar : bool; same, n_check : int64) : bool => prefix_bar ? same * 2l >= n_check : same == n_check

//! A TTS model's cell on the board: the voice both engines speak in and how the reference names
//! the model. An empty `voice` = the model has no TTS cell.
struct TtsBoard {
    display : string   // the board's model label
    voice : string     // the das voice; the reference takes the same name (a kitten alias maps to the pack's voice there too)
    ref : string       // the model's name in harness/tts_ref_bench.py
    package : string   // the torch-engine reference row's `engine`: the family's own package
    torch_gpu : bool   // the package serves a torch GPU device (mps, cuda); kitten's runs the onnxruntime CPU provider, pocket's pins the CPU
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
    quantize : QuantizeRecipe       //! minted on-box by llama-quantize (``ladder_row``); an empty `fmt` = a row nothing mints
    companions : array<ProvEntry>   // artifacts fetched/verified with this model (mmproj, image fixture)
    parity : ParityEvidence
    // serving-catalog membership (dasllama-server's /catalog view): a non-empty serve_name
    // puts the row on the page. Serving rows carry COMMIT-PINNED urls - resolve/main floats.
    serve_name : string             // stable kebab id ("" = not in the serving catalog)
    serve_display : string
    serve_note : string             // one-line what-it-is on the card
    serve_ctx : int = 0             // trained context window
    serve_vram_gb : float = 0.0     // advertised working-set hint, not a fit check
    serve_default : bool = false    // the one the getting-started path recommends
    serve_vision : string           // vision-tower companion NAME ("" = text only); [[serve_companion]] resolves it
    serve_drafter : string          //! the drafter companion NAME the server serves the file with ("" = none): an assistant drafter it attaches beside the trunk, or a split NextN head it pins for the load where no adjacency rule names it; [[serve_companion]] resolves it
    serve_tts : bool = false        // a speech model of the served text-to-speech set ([[serve_tts_set]]); its packs ride the companions
    serve_mtp : string              //! the platforms ("metal", "vulkan", comma list) where the model's self-speculation measured a win at one stream (PERF_LEDGER.md); "" = the server's unset `mtp` decodes plain
    mlx_repo : string               //! the MLX checkpoint (HF repo) of the same model the board's mlx-lm reference leg runs on Apple boxes; "" = no MLX leg
    mlx_fmt : string                //! that checkpoint's precision, stamped as the MLX row's exec_fmt
    tts : TtsBoard                  //! the TTS board cell ([[tts_catalog]], profile_common); an empty `tts.voice` = none
}

// ===== Parity fixture ids (simple_ids, harness/parity.sh) =====
// Prompts are the confident ones: a high-entropy prompt flips on a
// genuine near-tie (sub-ULP libm cos/sin/exp/tanh vs the oracle),
// which is not a bug.

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

// the gemma counting fixture, "Count from 1 to 60, separated by commas: 1, 2, ..., 23," (BOS=2):
// a bare count leaves the list after 10 on the E-series (end-of-turn) and after 25 on the 26B
// files, and the instruction keeps every gemma-3/gemma-4 file counting through all 40 tokens
def private ids_gemma_count_prompt() : array<int64> {
    return <- [2l, 4377l, 699l, 236743l, 236770l, 531l, 236743l, 236825l, 236771l, 236764l, 15914l, 684l, 162760l,
               236787l, 236743l, 236770l, 236764l, 236743l, 236778l, 236764l, 236743l, 236800l, 236764l, 236743l,
               236812l, 236764l, 236743l, 236810l, 236764l, 236743l, 236825l, 236764l, 236743l, 236832l, 236764l,
               236743l, 236828l, 236764l, 236743l, 236819l, 236764l, 236743l, 236770l, 236771l, 236764l, 236743l,
               236770l, 236770l, 236764l, 236743l, 236770l, 236778l, 236764l, 236743l, 236770l, 236800l, 236764l,
               236743l, 236770l, 236812l, 236764l, 236743l, 236770l, 236810l, 236764l, 236743l, 236770l, 236825l,
               236764l, 236743l, 236770l, 236832l, 236764l, 236743l, 236770l, 236828l, 236764l, 236743l, 236770l,
               236819l, 236764l, 236743l, 236778l, 236771l, 236764l, 236743l, 236778l, 236770l, 236764l, 236743l,
               236778l, 236778l, 236764l, 236743l, 236778l, 236800l, 236764l]
}

def private ev_gemma_count(kq_native : bool = false) : ParityEvidence {
    return <- ParityEvidence(prompt <- ids_gemma_count_prompt(),
        gen <- [236743l, 236778l, 236812l, 236764l, 236743l, 236778l, 236810l, 236764l, 236743l, 236778l,
                236825l, 236764l, 236743l, 236778l, 236832l, 236764l, 236743l, 236778l, 236828l, 236764l,
                236743l, 236778l, 236819l, 236764l, 236743l, 236800l, 236771l, 236764l, 236743l, 236800l,
                236770l, 236764l, 236743l, 236800l, 236778l, 236764l, 236743l, 236800l, 236800l, 236764l],
        kq_native = kq_native)
}

// gemma-2's vocab spells the same fixture with its own ids
def private ev_gemma2_count(kq_native : bool = false) : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [2l, 3074l, 774l, 235248l, 235274l, 577l, 235248l, 235318l, 235276l, 235269l, 19597l, 731l, 150248l,
                   235292l, 235248l, 235274l, 235269l, 235248l, 235284l, 235269l, 235248l, 235304l, 235269l, 235248l,
                   235310l, 235269l, 235248l, 235308l, 235269l, 235248l, 235318l, 235269l, 235248l, 235324l, 235269l,
                   235248l, 235321l, 235269l, 235248l, 235315l, 235269l, 235248l, 235274l, 235276l, 235269l, 235248l,
                   235274l, 235274l, 235269l, 235248l, 235274l, 235284l, 235269l, 235248l, 235274l, 235304l, 235269l,
                   235248l, 235274l, 235310l, 235269l, 235248l, 235274l, 235308l, 235269l, 235248l, 235274l, 235318l,
                   235269l, 235248l, 235274l, 235324l, 235269l, 235248l, 235274l, 235321l, 235269l, 235248l, 235274l,
                   235315l, 235269l, 235248l, 235284l, 235276l, 235269l, 235248l, 235284l, 235274l, 235269l, 235248l,
                   235284l, 235284l, 235269l, 235248l, 235284l, 235304l, 235269l],
        gen <- [235248l, 235284l, 235310l, 235269l, 235248l, 235284l, 235308l, 235269l, 235248l, 235284l,
                235318l, 235269l, 235248l, 235284l, 235324l, 235269l, 235248l, 235284l, 235321l, 235269l,
                235248l, 235284l, 235315l, 235269l, 235248l, 235304l, 235276l, 235269l, 235248l, 235304l,
                235274l, 235269l, 235248l, 235304l, 235284l, 235269l, 235248l, 235304l, 235304l, 235269l],
        kq_native = kq_native)
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

// Llama-3.2-1B-Instruct, the counting fixture [BOS]+"1, 2, 3, 4, 5, 6, 7," - the Metal format
// sweep's gate over the 1B requant family
def private ids_llama_count_prompt() : array<int64> {
    return <- [128000l, 16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l, 11l, 220l, 20l, 11l,
               220l, 21l, 11l, 220l, 22l, 11l]
}

def private ev_llama_count(kq_native : bool = false) : ParityEvidence {
    return <- ParityEvidence(prompt <- ids_llama_count_prompt(),
        gen <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 605l, 11l, 220l, 806l, 11l, 220l, 717l, 11l,
                220l, 1032l, 11l, 220l, 975l, 11l, 220l, 868l, 11l, 220l, 845l, 11l, 220l, 1114l,
                11l, 220l, 972l, 11l, 220l, 777l, 11l, 220l, 508l, 11l, 220l],
        kq_native = kq_native)
}

// the IQ2_XS requant skips 19 on llama.cpp too - its own continuation
def private ev_llama_count_iq2xs() : ParityEvidence {
    return <- ParityEvidence(prompt <- ids_llama_count_prompt(),
        gen <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 605l, 11l, 220l, 806l, 11l, 220l, 717l, 11l,
                220l, 1032l, 11l, 220l, 975l, 11l, 220l, 868l, 11l, 220l, 845l, 11l, 220l, 1114l,
                11l, 220l, 972l, 11l, 220l, 508l, 11l, 220l, 1691l, 11l, 220l],
        kq_native = true)
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

//! "1, 2, ... 7," - BOS=1, SPM dummy-prefix 29473 before each number
def private ev_mistral7b() : ParityEvidence {
    return <- ParityEvidence(
        prompt <- [1l, 29473l, 29508l, 29493l, 29473l, 29518l, 29493l, 29473l, 29538l, 29493l,
                   29473l, 29549l, 29493l, 29473l, 29550l, 29493l, 29473l, 29552l, 29493l,
                   29473l, 29555l, 29493l],
        gen <- [29473l, 29551l, 29493l, 29473l, 29542l, 29493l, 29473l, 29508l, 29502l, 29493l,
                29473l, 29508l, 29508l, 29493l, 29473l, 29508l, 29518l, 29493l, 29473l, 29508l,
                29538l, 29493l, 29473l, 29508l, 29549l, 29493l, 29473l, 29508l, 29550l, 29493l,
                29473l, 29508l, 29552l, 29493l, 29473l, 29508l, 29555l, 29493l, 29473l, 29508l])
}

// gemma-3 counting fixture (5-local:1-global SWA + dual RoPE θ): the 4B counts cleanly from
// "1..7," (BOS=2); the 1B rides the instructed fixture - on a bare count to 23 the bench's
// pinned reference chain ends its list at "30." where llama.cpp writes "30,", a near-tie the
// instruction removes. The sliding MASK itself (kstart > 0) engages only past the 512-token
// window; validated separately with a ~900-token counting prompt (40/40).
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

//! "1, 2, …, N," — the counting prompt the long text-form fixtures encode in-model (byte-identical
//! to the oracle run's input).
def counting_prompt(n : int) : string => counting_prompt(1, n)

//! "start, start+1, …, last," — the kernel-robust confident prompt family the parity suites encode
def counting_prompt(start, last : int) : string {
    return build_string() $(var w) {
        for (i in range(start, last + 1)) {
            w |> write(i)
            w |> write(i < last ? ", " : ",")
        }
    }
}

// the gemma4 fixture prompt, 1..320 (~1490 tokens): the sliding window is genuinely engaged
// (kstart > 0). The short 21-token counting prompt was rejected — its oracle continuation
// drifts into the channel format on a near-tie that flips under the pinned classic+libm kernels.
def private gemma4_counting_prompt() : string => counting_prompt(320)

// the dense-gemma4 continuation (frozen on the 12B, shared by every dense gemma4 carrier —
// same vocab, all count; each reuse verified per-file against the oracle)
def private ev_gemma4_dense() : ParityEvidence {
    return <- ParityEvidence(prompt_text = gemma4_counting_prompt(),
        gen <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l,
                236743l, 236800l, 236778l, 236800l, 236764l, 236743l, 236800l, 236778l, 236812l, 236764l,
                236743l, 236800l, 236778l, 236810l, 236764l, 236743l, 236800l, 236778l, 236825l, 236764l,
                236743l, 236800l, 236778l, 236832l, 236764l, 236743l, 236800l, 236778l, 236828l, 236764l])
}

def private ev_gemma4_26b() : ParityEvidence {
    return <- ParityEvidence(prompt_text = gemma4_counting_prompt(),
        gen <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l, 236743l],
        prefix_bar = true)
}

def private ev_gemma4_26b_q4km() : ParityEvidence {
    return <- ParityEvidence(prompt_text = gemma4_counting_prompt(),
        gen <- [236743l, 236800l, 236778l, 236770l, 236764l, 236743l, 236800l, 236778l, 236778l, 236764l,
                236743l, 236800l],
        moe_per_expert = true, prefix_bar = true)
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
//! presence-gated, not sha-gated. fetch_models' --convert arm skips this string,
//! CRISPASR_Q8_RECIPE and GIGAAM_KQ_MIX_RECIPE: setup_asr_rig --refs derives them.
let REF_Q8_RECIPE = "derive: setup_asr_rig.das --refs quantizes it from the fp bin beside it (whisper.cpp quantize q8_0); reference-side carrier, presence-gated"
let CRISPASR_Q8_RECIPE = "derive: setup_asr_rig.das --refs quantizes it from the f16 GGUF beside it (CrispASR crispasr-quantize q8_0: the ten block GEMMs a layer, the rest copied); reference-side carrier, presence-gated"
//! the quantize arguments of the GigaAM kq lane's mixed-member carrier: q4_k with the FF GEMMs overridden to Q6_K
let GIGAAM_KQ_MIX_ARGS = "q4_k --tensor-type \"ff[12]\\.linear[12]\\.weight=q6_k\""
let GIGAAM_KQ_MIX_RECIPE = "derive: setup_asr_rig.das --refs quantizes it from the f16 GGUF beside it (CrispASR crispasr-quantize {GIGAAM_KQ_MIX_ARGS}: Q4_K attention, Q6_K FF, Q8_0 conv pointwise) - the kq lane's mixed-member carrier (tests/test_gigaam.das, tests/test_audio.das); presence-gated"

// The parakeet conversion rail is retired: ggml-org/parakeet-GGUF hosts the v3 bin
// byte-identical to the canonical conversion (sha-verified), so v3 fetches like any
// carrier. v2 was never hosted - its bins live on the boxes that converted them.
let PARAKEET_V2_NOTE = "historic nemo-venv conversion (rail retired); present only on boxes that converted it - v3 is the fetched carrier"
//! the GigaAM v3 carriers' recipe: hosted f16 GGUFs, the top precision offered
let GIGAAM_RECIPE = "fetched: cstr/gigaam-v3-GGUF, converted by CrispASR's models/convert-gigaam-to-gguf.py from salute-developers/GigaAM v3; Russian only, the e2e heads write cased, punctuated text; THIRD_PARTY_NOTICES.md (module root) carries the attribution"
let GIGAAM_Q4K_RECIPE = "fetched: cstr/gigaam-v3-GGUF's q4_k file, CrispASR's crispasr-quantize over the f16 carrier (the attention and FF GEMMs Q4_K, the conv pointwise GEMMs Q8_0, the rest as the f16 file); serves on the kq lane as its own planes; THIRD_PARTY_NOTICES.md (module root) carries the attribution"
//! the Parakeet-TDT v3 GGUF set's pinned revision and recipe: the files the GGUF route and the kq lane's cells load
let PARAKEET_GGUF_AT = "{HF}/handy-computer/parakeet-tdt-0.6b-v3-gguf/resolve/d8d8c32cb2017c57e5307767343e97870f8a7f53"
let PARAKEET_GGUF_RECIPE = "fetched: handy-computer/parakeet-tdt-0.6b-v3-gguf, the transcribe.cpp project's conversion of nvidia/parakeet-tdt-0.6b-v3 (the attention and FF GEMMs K-quant in the Q4_K_M file, the attention outputs Q8_0, the conv pointwise GEMMs f16); the quantized file serves on the kq lane - its K-quant and Q8_0 tensors as its own planes, its f16 GEMMs quantized to Q8_0; THIRD_PARTY_NOTICES.md (module root) carries the attribution"
let CANARY_ENC_RECIPE = "convert: modules/dasLLAMA/harness/convert_canary_to_ggml.py over nvidia/canary-qwen-2.5b (nemo venv; encoder is a repack - sha canonical everywhere)"
let CANARY_DEC_RECIPE = "convert: modules/dasLLAMA/harness/convert_canary_to_ggml.py over nvidia/canary-qwen-2.5b (nemo venv), then llama-quantize Q8_0 (a Q8_0 disk embedding lets the tied classifier serve cls_q8 on the Metal rail); bytes are per-arch (fp16 LoRA merge) - gate via asr_bench --text over jfk/jfk3/gb1 vs benchmarks/asr/canary_transcripts.expected"
let TTS_HF = "{HF}/borisbat/dasllama-tts/resolve/a62130a44d6f8fef8a5113825c5f15b01e1c8f80"   //! the published set, commit-pinned
let FORMATS_HF = "{HF}/borisbat/dasllama-formats/resolve/eff177a6d0456c5d269672ccc80c68a0a4a7b122"   //! the format matrix, commit-pinned
let TTS_KITTEN_RECIPE = "convert: modules/dasLLAMA/performance/build_tts_data.das -- --root <g2p experiment root> --out <models dir> runs harness/convert_kitten.py over KittenML/kitten-tts-nano-0.8 @ 7a1db645b1f3ab9420761d87428e042b9cec3f26 and kitten-tts-mini-0.8 @ c02725660cea441db4c383af69f1f26f5cd00947 (the ONNX weights into GGUF, f32); THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
let TTS_KOKORO_RECIPE = "convert: modules/dasLLAMA/performance/build_tts_data.das -- --root <g2p experiment root> --out <models dir> runs harness/convert_kokoro.py at 807f0164c3 (a later converter also writes kokoro.voice_langs; the pinned file carries none, its voices named in the pipeline's shape) over hexgrad/Kokoro-82M @ f3ff3571791e39611d31c381e3a41a3af07b4987 (kokoro-v1_0.pth + the voice packs into GGUF, f32); THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
let TTS_KOKORO_RU_RECIPE = "convert: harness/convert_kokoro.py --checkpoint <pth> --config config.json --voices <dir of .bin packs> --name <stem> --lang ru --source 'zaakirio/kokoro-ru @ d649c57b239b18c4c384378127cbf01dba039bc1' (kokoro-ru-v2-base.pth with sveta and masha into kokoro-ru.gguf, kokoro-ru-v2-dima.pth with dima into kokoro-ru-dima.gguf, f32); THIRD_PARTY_NOTICES.md carries the attribution"
//! a phoneme family's file again as the file that stores a lane's weights (`ARCHITECTURE_TTS_FORMATS.md#tts-stored-file`): the encoders round by kernel backend, so a re-mint on another box scores alike and hashes apart
let TTS_STORED_RECIPE = "convert: utils/dasllama-convert/main.das -- --model <name>.gguf --gguf <name>-q8.gguf (the Q8_0 lane's weights), and with --recipe --gguf <name>-kq.gguf (the recipe lane's), over this catalog's f32 file of the same name on an Apple M5 Max (the arm64 kernel backend): every weight the lane mints stored as minted, the rest and every key as the source holds them; the source file's licence applies"
let TTS_RU_STRESS_SEEDED_RECIPE = "convert: harness/ru_stress_oracle.py --seed-head <ruBert-base dir> (a Linear(768, 8) stress head under seed 290 as stress_head.pt), then harness/convert_ru_stress.py --checkpoint <ruBert-base dir> --out <models dir> --catalog <the forms whose stress depends on context> --source ai-forever/ruBert-base over ai-forever/ruBert-base @ 05f37a2ca9e333fd18f30cd0c96c68d274793c69 (pytorch_model.bin, vocab.txt; f32): the pretrained encoder under a seeded head, the parity rail's carrier"
let TTS_RU_STRESS_ORACLE_RECIPE = "mint: harness/ru_stress_oracle.py --checkpoint <ruBert-base dir with its seeded head> --corpus <a text corpus the caller supplies> --catalog <the forms whose stress depends on context> --tokens modules/dasLLAMA/tests/_tts_fixtures/ru_stress_tokens.json --dumps <models dir>/tts_oracle/ru_stress: PyTorch's last hidden state, the head's logits and the masked pick over 20 sentences, one TTSO dump a sentence"
let TTS_RU_STRESS_Q4_RECIPE = "convert: harness/convert_ru_stress.py --checkpoint <the fine-tuned checkpoint (not distributed)> --out <models dir> --name <the file's stem: ru-stress-rubert-base-q4 or ru-stress-ruelectra-small-q4> --source <its base model: ai-forever/ruBert-base or ai-forever/ruElectra-small> --catalog <the forms whose stress or yo spelling depends on context> --quant Q4_K --imatrix-corpus <a text corpus the caller supplies> --emb-quant Q8_0 --vocab-keep 0 --vocab-corpus <the same corpus> --llama-cpp <a checkout with a built ggml-base library>, run under OMP_NUM_THREADS=16 (the Q4_K bytes follow the thread count of the importance pass); ru-stress-rubert-base-q4.gguf fine-tuned from ai-forever/ruBert-base, ru-stress-ruelectra-small-q4.gguf from ai-forever/ruElectra-small; tts_ctx_ru.gguf beside a Russian voice is a copy of either"
let TTS_RU_STRESS_F32_RECIPE = "convert: harness/convert_ru_stress.py --checkpoint <the checkpoint ru-stress-rubert-base-q4.gguf was converted from (not distributed)> --out <models dir> --name ru-stress-rubert-base-f32 --source ai-forever/ruBert-base --catalog <the forms whose stress or yo spelling depends on context> --llama-cpp <the same checkout>, no other flag: the same model on f32 planes and the whole vocabulary, the reference the quantized lane's cells hold"
let TTS_PACKS_RU_RECIPE = "mint: harness/build_g2p_ru_data.py --openrussian <dir of the OpenRussian csv files> --frequency <a table the caller supplies: word, ordinal, count> --extra <the Wiktionary forms, then the words Wikipedia leads mark> --amend <the dictionary rows the Wiktionary editions correct> --endings 2:8:2:0.4 --rules <a rules file> --out tts_g2p_ru.bin (every inflected form's stressed vowel, the \xD1\x91 spellings, the ending table and the rule tier into tts_g2p_ru.bin; harness/README.md names each argument's columns)"
let TTS_POCKET_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language english_2026-04 --q8 --name pocket-tts-en-q8 under pocket-tts 4.0.1 over the files its config pins: kyutai/pocket-tts languages/english_2026-04/model.safetensors @ 19f95fe2df36e79fbd9f10008595cc4c977a0fcc, kyutai/pocket-tts-without-voice-cloning languages/english_2026-04/tokenizer.json @ 00eac05ed3d16bdc3f6b5d598874019c34a89214 and the voice clips of kyutai/tts-voices @ 323332d33f997de8394f24a193e1a76df720e01a, each trimmed to end on a pause before the encoder (the served linears as Q8_0, every codec conv and the rest f16); THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
let TTS_POCKET_LANG_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language <config> [--q8 | --kq] --name pocket-tts-<code>[-q8 | -kq] under pocket-tts 4.0.1 over the files each config pins (german, italian, spanish, portuguese, french_24l): kyutai/pocket-tts languages/<config>/model.safetensors @ 3e82814a68665eec246ff649b14c71331f955c06, kyutai/pocket-tts-without-voice-cloning languages/<config>/tokenizer.json @ 4e1e0a3e611c51c0b4ed8174fc10f32a54644303 (french_24l @ 8843db76457a91db32077edf8dfcd1c0e3e755fd) and the language's default clip (kyutai/pocket-tts @ 64ab7d24c479d736a83b8cc666c4a776fca30fda; estelle from kyutai/tts-voices @ 1fc7395b7e012e2bbebfca14b942a4ef62ccc899), trimmed to end on a pause before the encoder; THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
//! the small form the browser examples fetch: K-quant planes where the ladder held (backbone and codec transformers Q4_K, the embedding table Q4_K), Q8_0 where it did not (the flow head), the codec convolutions f16, the roster as latent frames
let TTS_POCKET_KQ_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language english_2026-04 --kq --name pocket-tts-en-kq under pocket-tts 4.0.1 over the sources of pocket-tts-en-q8.gguf's recipe (the backbone and the codec transformers as Q4_K through ggml's own quantizer, the flow head as Q8_0, the codec convolutions f16, the embedding table Q4_K, the codec encoder inside, the 19 voices as the latent frames of their clips); THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
let TTS_POCKET_STUART_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language english_2026-04 --kq --voices stuart_bell --no-cloning --name pocket-tts-en-stuart-kq under pocket-tts 4.0.1 over the sources of pocket-tts-en-q8.gguf's recipe (the small form of pocket-tts-en-kq.gguf with one voice as latent frames and no codec encoder: reads text in stuart_bell, cannot clone); THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
// the f16 twin the parity rail loads on its f32 lane: the same sources, no --q8; never published
let TTS_POCKET_F16_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language english_2026-04 --name pocket-tts-en under pocket-tts 4.0.1 over the sources of pocket-tts-en-q8.gguf's recipe (every tensor f16; the parity rail's reference under tests/test_tts_pocket.das, local only)"
let TTS_POCKET_SMALL_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language <config> [--q8 | --kq] [--voices stuart_bell --no-cloning] --name pocket-tts-en-<size>[-refl][-q8 | -kq | -stuart-kq] under pocket-tts 4.0.1 over kyutai/pocket-tts languages/<config>/model.safetensors @ db04f0e653473e5c7333c69686ac92b7460b0c1d (english_18m_2026-10, english_7m_2026-10, english_2m_2026-10 and the _refl_ twins), the tokenizer of kyutai/pocket-tts-without-voice-cloning languages/<config>/tokenizer.model @ 08ffc58775e62aad9409511b9591c9af626270f3 and the voice clips of kyutai/tts-voices @ 323332d33f997de8394f24a193e1a76df720e01a, each trimmed to end on a pause before the encoder (the 2m's widths sit off the quant lattices, so its file is f16); THIRD_PARTY_NOTICES.md (repo root) carries the attribution"
let TTS_POCKET_SMALL_F32_RECIPE = "convert: modules/dasLLAMA/harness/convert_pocket.py --language <config> --keep-f32 --name pocket-tts-en-<size>[-f32] over the sources of the small files' recipe (every tensor as the f32 bundle stores it; the parity rail's reference under tests/test_tts_pocket.das, local only)"
let TTS_PACKS_RECIPE = "mint: modules/dasLLAMA/performance/build_tts_data.das -- --root <g2p experiment root> --out <models dir> runs harness/build_g2p_data.py (misaki 0.9.4 gold/silver + CMUdict 0.7a + the g2p_en 2.1.0 GRU + harness/g2p_local_additions.json into tts_g2p.bin) and harness/train_postag.py (UD English-EWT + spaCy-labelled silver prose into tts_postag.bin)"
let TTS_ORACLE_RECIPE = "mint: modules/dasLLAMA/performance/build_tts_data.das -- --root <g2p experiment root> --out <models dir> runs the PyTorch / ONNX reference per model over the bring-up sentences and dumps every stage the parity rail (tests/_tts_parity.das) compares"
//! the Bergamot translation students (tests/test_nmt.das): Firefox Translations' Marian int8 files, each with its shared
//! vocabulary beside it; the rows pin the bytes the suite was held on
//! Mozilla's translation-model bucket: each student's file and vocabulary under its training run's `exported/`, gzipped
let BERGAMOT_BUCKET = "https://storage.googleapis.com/moz-fx-translations-data--303e-prod-translations-data"
let BERGAMOT_RECIPE = "fetch: the url (Mozilla's translation-model bucket, the run Firefox serves for the pair and tier the folder names, `bergamot/<src>-<trg>-<tier>/`), gunzipped, byte for byte - the suite reads the Marian file and bakes its own `.dlim`"
let NMT_ORACLE_RECIPE = "mint: modules/dasLLAMA/benchmarks/nmt/bergamot_oracle.py --marian <marian-decoder of setup_asr_rig.das --refs> --model <models dir>/bergamot/<dir> --fixture modules/dasLLAMA/benchmarks/nmt/corpus/<src>.txt --out <models dir>/nmt_oracle/<dir>/<src>_f32 (and _int8), plus --spm-edge corpus/spm_edge.txt for spm_edge.json: marian's f32 graph over the student's dequantized weights, every stage and the tokens"
//! the translation LLM the facade's completion form serves (tests/test_nmt.das, tutorial 17)
let MILMMT_RECIPE = "fetch: the url (mradermacher's GGUF quantization of MiLMMT-46-1B-v0.1 - Gemma 3 1B trained further on translation between 46 languages - at Q4_K_M), byte for byte"

//! the storyteller example's story model: our conversion of the llama2.c checkpoint, published beside the storywish model
let STORIES15M_RECIPE = "convert: the reference build's llama-convert-llama2c-to-ggml --copy-vocab-from-model tokenizer.bin --llama2c-model stories15M.bin over karpathy/tinyllamas ({HF}/karpathy/tinyllamas: stories15M.bin + tokenizer.bin, the llama2.c checkpoint), then its llama-quantize to Q8_0 and llama.context_length set to 256; the published file is that conversion - the gate is the storyteller's smoke rail (tests/test_storyteller_restart.das), which writes and reads a story out of it: a smoke, not a parity check, the toy model pins no reference output"

//! the two story models behind the browser examples, published as GGUF and commit-pinned like the TTS set: the rows fetch like any carrier
let STORIES_HF = "{HF}/borisbat/dasllama-stories/resolve/ae5d58ca1f8830d29e4698183f55dcaca7c8dd1e"
let STORYWISH_RECIPE = "train: llama2.c's trainer over roneneldan/TinyStoriesInstruct with a 4K SentencePiece vocabulary (dim 512, 8 layers, 9500 steps of 131K tokens on one H100), export.py to the llama2.c checkpoint, llama-convert-llama2c-to-ggml + llama-quantize Q8_0; the card on borisbat/dasllama-stories carries the recipe and the hit-rate table"

// The text-model set, ascending size. Absent files skip at run time, so one table serves every
// box; each view measures/verifies the subset it actually has. Audio models live in
// [[asr_catalog]] (their provenance rides those specs into [[models_provenance]]).
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#format-matrix-files")]
def model_specs() : array<ModelSpec> {   // nolint:STYLE038 — flat model-set table, one entry per carrier
    return <- [
        ModelSpec(file = "stories15M-Q8_0.gguf", url = "{STORIES_HF}/stories15M-Q8_0.gguf", bytes = 26671392l,
            sha256 = "1a8385fbd5de38704f005ba3d9b881fc0c60431bbbf17ab36c665e0be93ffd32", recipe = STORIES15M_RECIPE),   // llama2.c's TinyStories writer behind examples/dasLLAMA/storyteller; profiled nowhere
        ModelSpec(file = "tinystories-instruct-27M-Q8_0.gguf", url = "{STORIES_HF}/tinystories-instruct-27M-Q8_0.gguf", bytes = 31467200l,
            sha256 = "92c2b775070b76ee31b9921f7f339ec9fc0e08b54be0616418414d4eec65d282", recipe = STORYWISH_RECIPE),   // the request-taking story writer behind examples/dasLLAMA/storywish; profiled nowhere
        ModelSpec(file = "SmolLM2-135M-Instruct-Q8_0.gguf", display = "SmolLM2-135M",
            url = "{HF}/bartowski/SmolLM2-135M-Instruct-GGUF/resolve/09816acd5d99df7be770d85ea30822623dab342c/SmolLM2-135M-Instruct-Q8_0.gguf",
            bytes = 144811360l, sha256 = "5a1395716f7913741cc51d98581b9b1228d80987a9f7d3664106742eb06bba83"),
        ModelSpec(file = "Qwen2.5-0.5B-Instruct-Q8_0.gguf", display = "Qwen2.5-0.5B",
            url = "{HF}/bartowski/Qwen2.5-0.5B-Instruct-GGUF/resolve/41ba88dbac95fed2528c92514c131d73eb5a174b/Qwen2.5-0.5B-Instruct-Q8_0.gguf",
            bytes = 531068480l, sha256 = "25130a98aa782284a7dabea0c23245b2fd371ed47244e79d78b8ec23245fdf96",
            parity <- ev_qwen_count()),
        // the dense iq4nl32 carrier: 896-wide rows off the superblock lattice, every dense plane on the per-32 planes
        ModelSpec(file = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf",
            url = "{FORMATS_HF}/Qwen2.5-0.5B-Instruct/Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf", bytes = 354305600l, sha256 = "06c6fad13a2c209429c4bd2b08cb42dccd16e868999909365b47c050e88ffa57",
            recipe = "llama-quantize --allow-requantize Qwen2.5-0.5B-Instruct-Q8_0.gguf Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf IQ4_NL"),
        // the per-32 table carriers, every tensor in the file's format (`--pure`): the IQ4_NL token table and
        // classifier gather and serve as iq4nl32 planes, the Q4_0 ones as q4032 (896 is off the 256 lattice), the Q5_0 ones and
        // every Q5_0 plane as q5032
        ModelSpec(file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf",
            url = "{FORMATS_HF}/Qwen2.5-0.5B-Instruct/Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", bytes = 284087872l, sha256 = "e2dc2fdd822ec3a830eb1c676493fc8360129d1248fdb605d88c8b22c6c92de2",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type iq4_nl --output-tensor-type iq4_nl Qwen2.5-0.5B-Instruct-Q8_0.gguf Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf IQ4_NL"),
        ModelSpec(file = "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf",
            url = "{FORMATS_HF}/Qwen2.5-0.5B-Instruct/Qwen2.5-0.5B-Instruct-Q4_0-local.gguf", bytes = 284087872l, sha256 = "3e965443daa431864455126645721d93bb145adee757b3a59a3fe6d00833514b",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type q4_0 --output-tensor-type q4_0 Qwen2.5-0.5B-Instruct-Q8_0.gguf Qwen2.5-0.5B-Instruct-Q4_0-local.gguf Q4_0"),
        ModelSpec(file = "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf",
            url = "{FORMATS_HF}/Qwen2.5-0.5B-Instruct/Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf", bytes = 345833024l, sha256 = "a9d3131babde9926f9513dfe110890416d3f997e9c01aa634a111f281176fe39",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type q5_0 --output-tensor-type q5_0 Qwen2.5-0.5B-Instruct-Q8_0.gguf Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf Q5_0"),
        ModelSpec(file = "Qwen3-0.6B-Q8_0.gguf", display = "Qwen3-0.6B",
            url = "{HF}/Qwen/Qwen3-0.6B-GGUF/resolve/23749fefcc72300e3a2ad315e1317431b06b590a/Qwen3-0.6B-Q8_0.gguf",
            bytes = 639446688l, sha256 = "9465e63a22add5354d9bb4b99e90117043c7124007664907259bd16d043bb031",
            parity <- ev_qwen_count()),   // the QK-norm arch (per-head Q/K RMSNorm before RoPE)
        ModelSpec(file = "Qwen3.5-0.8B-Q8_0.gguf", display = "Qwen3.5-0.8B",
            url = "{HF}/unsloth/Qwen3.5-0.8B-GGUF/resolve/6ab461498e2023f6e3c1baea90a8f0fe38ab64d0/Qwen3.5-0.8B-Q8_0.gguf",
            bytes = 811843840l, sha256 = "0ad885ffd4bb022fc4f0d33a3308fa108ef8613159d3b3a67e23abca056b7a6c",
            // the qwen35 hybrid: 18 Gated-DeltaNet recurrent + 6 gated-attention layers
            // (2x-wide Q with sigmoid out-gate, partial NEOX rope 64/256, QK-norm)
            parity <- ev_qwen_count([dn_chunked = true])),
        // the NextN-headed twin of the hybrid: the small MTP carrier the joint speculative tick,
        // the batched row's --npl-mtp arm and the scheduler's self-speculative cells drive; the
        // local name carries the MTP tag the release's file name does not
        ModelSpec(file = "Qwen3.5-0.8B-MTP-Q8_0.gguf",
            url = "{HF}/unsloth/Qwen3.5-0.8B-MTP-GGUF/resolve/main/Qwen3.5-0.8B-Q8_0.gguf",
            bytes = 833592736l, sha256 = "c54f8b67069c70085b98440de696b44da8250250ac69a961b41133def876e262"),
        // the NVFP4 hybrid, the nv4 rail's carrier (tests/test_kquant.das): ModelOpt's two-level FP4 (nv4 planes, each with its `.scale` tensor) on the FFN, attention and deltanet projections, Q4_K / Q5_K / BF16 beside them
        ModelSpec(file = "Qwen3.5-0.8B-NVFP4-MTP.gguf",
            url = "{HF}/michaelw9999/Qwen3.5-0.8B-NVFP4-MTP-GGUF/resolve/d8ac925b9b4e19781e3aa8545c69ca89884fcfe4/Qwen3.5-0.8B-NVFP4-MTP-GGUF.gguf",
            bytes = 578052704l, sha256 = "71a0a29da9481f51d8e301f8ffb67feec1627cd18e92183a1f3918b0a7c35728"),
        // the per-32 hybrid table carrier: the Q5_1 token table, classifier, NextN eh_proj and deltanet planes as q51
        ModelSpec(file = "Qwen3.5-0.8B-MTP-Q5_1-local.gguf",
            url = "{FORMATS_HF}/Qwen3.5-0.8B/Qwen3.5-0.8B-MTP-Q5_1-local.gguf", bytes = 592235936l, sha256 = "a6092ab8e0039e5e6dfae47cbb1332c0b31c183720c068bdf3290c21912cefe2",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type q5_1 --output-tensor-type q5_1 Qwen3.5-0.8B-MTP-Q8_0.gguf Qwen3.5-0.8B-MTP-Q5_1-local.gguf Q5_1"),
        // the nv4 hybrid carrier (tests/test_metal_format_census.das, `census-nv4-hybrid`): every plane NVFP4 - the deltanet planes, the NextN eh_proj, the table and the classifier - with no `.scale`
        ModelSpec(file = "Qwen3.5-0.8B-MTP-NVFP4-pure-local.gguf",
            url = "{FORMATS_HF}/Qwen3.5-0.8B/Qwen3.5-0.8B-MTP-NVFP4-pure-local.gguf", bytes = 447421856l, sha256 = "c7a61caa76f42ba392616e9750050987cb953e4ff0450558d61de1e5549d52b2",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type nvfp4 --output-tensor-type nvfp4 Qwen3.5-0.8B-MTP-Q8_0.gguf Qwen3.5-0.8B-MTP-NVFP4-pure-local.gguf NVFP4"),
        // the embedding model whose file names its read: `qwen3.pooling_type` last, `add_eos_token` on
        // (tests/test_embeddings.das holds its model card's cosine matrix as the oracle)
        ModelSpec(file = "Qwen3-Embedding-0.6B-Q8_0.gguf",
            url = "{HF}/Qwen/Qwen3-Embedding-0.6B-GGUF/resolve/370f27d7550e0def9b39c1f16d3fbaa13aa67728/Qwen3-Embedding-0.6B-Q8_0.gguf",
            bytes = 639150592l, sha256 = "06507c7b42688469c4e7298b0a1e16deff06caf291cf0a5b278c308249c3e439"),
        // the embedding model's f16 source and its format ladder: every dense plane in one format (`--pure`), the short
        // window's GEMM forms raced and held per format on one shape; `fetch_models --fetch --convert -o 0.6B-` mints the
        // ladder under the models dir's fmt-ladder/ from the f16 (each row's `quantize` is the recipe it spells)
        ModelSpec(file = "Qwen3-Embedding-0.6B-f16.gguf",
            url = "{HF}/Qwen/Qwen3-Embedding-0.6B-GGUF/resolve/370f27d7550e0def9b39c1f16d3fbaa13aa67728/Qwen3-Embedding-0.6B-f16.gguf",
            bytes = 1197629632l, sha256 = "421a27e58d165478cc7acb984a688c2aa41404968b0203e7cd743ece44c54340"),
        ladder_row("Qwen3-Embedding-0.6B", "Q4_0"),
        ladder_row("Qwen3-Embedding-0.6B", "Q5_1"),
        ladder_row("Qwen3-Embedding-0.6B", "Q2_K"),
        ladder_row("Qwen3-Embedding-0.6B", "Q3_K"),
        ladder_row("Qwen3-Embedding-0.6B", "Q4_K"),
        ladder_row("Qwen3-Embedding-0.6B", "Q5_K"),
        ladder_row("Qwen3-Embedding-0.6B", "Q6_K"),
        ladder_row("Qwen3-Embedding-0.6B", "IQ4_NL"),
        ladder_row("Qwen3-Embedding-0.6B", "IQ4_XS"),
        ladder_row("Qwen3-Embedding-0.6B", "IQ3_S"),
        ladder_row("Qwen3-Embedding-0.6B", "IQ3_XXS"),
        ladder_row("Qwen3-Embedding-0.6B", "IQ2_S"),
        // the IQ2 token tables stay q4_k: the imatrix covers GEMM inputs only, and these types refuse a plane without one
        ladder_row("Qwen3-Embedding-0.6B", "IQ2_XS", "q4_k"),
        ladder_row("Qwen3-Embedding-0.6B", "IQ2_XXS", "q4_k"),
        ladder_row("Qwen3-Embedding-0.6B", "BF16"),
        // the K-quant twin of the hybrid for the resident driver's native-plane cells
        // (tests/test_gpu_resident_hybrid.das): qkv q6_K, z q4_K, beta/alpha q4_K, the deltanet
        // out plane q4_K - the step leaves the o row f32 for the superblock out plane's requant
        ModelSpec(file = "Qwen3.5-0.8B-Q4_K_M.gguf",
            url = "{FORMATS_HF}/Qwen3.5-0.8B/Qwen3.5-0.8B-Q4_K_M.gguf", bytes = 529297664l, sha256 = "de98950ab4327338d3026fa0b9de0a218ddcb3869b8aed1091e8ba730c9ab659",
            recipe = "llama-quantize --allow-requantize Qwen3.5-0.8B-Q8_0.gguf Qwen3.5-0.8B-Q4_K_M.gguf Q4_K_M",
            parity <- ev_qwen_count([kq_native = true, dn_chunked = true, require_dn_kq = true])),
        // the mixed twin: the deltanet out plane at Q8_0 (the unsloth UD shape) - the one carrier whose x and o feeds part ways
        ModelSpec(file = "Qwen3.5-0.8B-Q4_K_M-q8out.gguf",
            url = "{FORMATS_HF}/Qwen3.5-0.8B/Qwen3.5-0.8B-Q4_K_M-q8out.gguf", bytes = 548172032l, sha256 = "fc384e2da3e605a7e9ad6030421f898e16cab693ac52c87029a4617c85d7e699",
            recipe = "llama-quantize --allow-requantize --tensor-type ssm_out=q8_0 Qwen3.5-0.8B-Q8_0.gguf Qwen3.5-0.8B-Q4_K_M-q8out.gguf Q4_K_M",
            parity <- ev_qwen_count([kq_native = true, dn_chunked = true])),
        // the 4B hybrid with its in-file MTP block - the mid-size dense carrier of the MTP series'
        // Metal rail parity (tests/test_metal_mtp_parity.das); the local name carries the MTP tag
        ModelSpec(file = "Qwen3.5-4B-MTP-Q8_0.gguf",
            url = "{HF}/unsloth/Qwen3.5-4B-MTP-GGUF/resolve/main/Qwen3.5-4B-Q8_0.gguf",
            bytes = 4610580800l, sha256 = "4f40ff26e3b6e23888e520e9c33d9a309d919b940cd1375b421710c6cb6cc8dd"),
        // the 9B hybrid with its in-file MTP block, the whole-model driver's parity carrier
        // (head 256 gated attention, partial rope, Q5_K/Q6_K deltanet planes) and the vulkan
        // serving census's head-256 row; the local name carries the MTP tag the release's does not
        ModelSpec(file = "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf",
            url = "{HF}/unsloth/Qwen3.5-9B-MTP-GGUF/resolve/main/Qwen3.5-9B-UD-Q5_K_XL.gguf",
            bytes = 6874345824l, sha256 = "bb0aa4bf2acf4b6d97eca051a7af91200729e9e97449ce98785af9e70f6d703c"),
        // the tensor-arm twin carriers (tests/test_metal_support_matrix.das --arm tensor): small
        // on purpose - q8 drives the mulmm/attn twins, the K-quant one the kq mulmm twins. The
        // arm FAILS (not skips) when they are absent, so every box gets them provisioned here
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q5_K_M.gguf",
            url = "{HF}/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q5_K_M.gguf",
            bytes = 911503488l, sha256 = "4f22f95fb1679ef8e2fd6f1659d23f06cd1dbedb79ff291db7413a7ae509e2a9",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "tinyllama-1.1b-v0.3-f16.gguf",   // parity-only carrier (F16 -> fp32 load)
            parity <- ev_tinyllama_v03()),
        ModelSpec(file = "gemma-3-1b-it-Q8_0.gguf", display = "gemma-3-1B",
            url = "{HF}/ggml-org/gemma-3-1b-it-GGUF/resolve/f9c28bcd85737ffc5aef028638d3341d49869c27/gemma-3-1b-it-Q8_0.gguf",
            bytes = 1069306368l, sha256 = "b205840c5dcef55078e37d344677869a714ffd42a4ae448c48dcfb52e4bb10d5",
            parity <- ev_gemma_count()),
        ModelSpec(file = "tinyllama-1.1b-chat-v1.0.Q8_0.gguf", display = "TinyLlama-1.1B",
            url = "{HF}/TheBloke/TinyLlama-1.1B-Chat-v1.0-GGUF/resolve/52e7645ba7c309695bec7ac98f4f005b139cf465/tinyllama-1.1b-chat-v1.0.Q8_0.gguf",
            bytes = 1170781568l, sha256 = "a4c9bb1dbaa372f6381a035fa5c02ef087aaa1ff1f843a56a22328114f03fc59"),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q8_0.gguf", display = "Llama-3.2-1B",
            url = "{HF}/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q8_0.gguf",
            bytes = 1321083008l, sha256 = "432f310a77f4650a88d0fd59ecdd7cebed8d684bafea53cbff0473542964f0c3",
            parity <- ev_llama32_1b()),
        ModelSpec(file = "Qwen2.5-1.5B-Instruct-Q8_0.gguf", display = "Qwen2.5-1.5B",
            url = "{HF}/bartowski/Qwen2.5-1.5B-Instruct-GGUF/resolve/9eadc66189c7641e1ddd226b8267a9119b2ce2d4/Qwen2.5-1.5B-Instruct-Q8_0.gguf",
            bytes = 1646573312l, sha256 = "7185d306cf45956c8c017cd0d3b05ecc6bc18b3ea8eb5c240dce40e87563db7f",
            parity <- ev_qwen_count()),
        ModelSpec(file = "SmolLM2-1.7B-Instruct-Q8_0.gguf", display = "SmolLM2-1.7B",
            url = "{HF}/bartowski/SmolLM2-1.7B-Instruct-GGUF/resolve/1f03464768bfcc0319fc50da8ff5fb20b6417ba2/SmolLM2-1.7B-Instruct-Q8_0.gguf",
            bytes = 1820414944l, sha256 = "0c6e8955788b1253f418c354a4bdc4cf507b8cfe49c48bb10c7c58ae713cfa2a",
            parity <- ev_smollm2()),
        ModelSpec(file = "Phi-3.5-mini-instruct-Q4_K_M.gguf",   // the coverage census's phi3 carrier
            url = "{HF}/bartowski/Phi-3.5-mini-instruct-GGUF/resolve/main/Phi-3.5-mini-instruct-Q4_K_M.gguf",
            bytes = 2393232672l, sha256 = "e4165e3a71af97f1b4820da61079826d8752a2088e313af0c7d346796c38eff5"),
        ModelSpec(file = "Phi-3.5-mini-instruct-Q8_0.gguf",   // the phi3 carrier on the q8 decode chain, beside the K-quant one
            url = "{HF}/bartowski/Phi-3.5-mini-instruct-GGUF/resolve/main/Phi-3.5-mini-instruct-Q8_0.gguf",
            bytes = 4061222688l, sha256 = "76fbf02f6fe92af57dbd818409bc8a0240026f1f3609bb405c3be94c973fb823"),
        ModelSpec(file = "microsoft_Phi-4-mini-instruct-Q8_0.gguf",   // the phi3 family at head 128 (24 heads over 3072, the partial rope): the flash tiles' second Phi width
            url = "{HF}/bartowski/microsoft_Phi-4-mini-instruct-GGUF/resolve/main/microsoft_Phi-4-mini-instruct-Q8_0.gguf",
            bytes = 4084611456l, sha256 = "a12f242c4ee379b9da91673e5d78b30bfacfe4fd86a0b2259d2ecad5d93cb6c7"),
        ModelSpec(file = "gemma-2-2b-it-Q8_0.gguf", display = "gemma-2-2B",
            url = "{HF}/bartowski/gemma-2-2b-it-GGUF/resolve/855f67caed130e1befc571b52bd181be2e858883/gemma-2-2b-it-Q8_0.gguf",
            bytes = 2784495456l, sha256 = "2d448a9aab894b8e8e18168cf3f490cb9f65632222f29f93514ac9ecc754debe",
            parity <- ev_gemma2()),
        // the tool-format live-leg carriers (REVIEW recognition-test rule): each family's
        // smallest catalogued GGUF, exercised by utils/dasllama-server/test_openai_server_think.das
        ModelSpec(file = "Llama-3.2-3B-Instruct-Q4_K_M.gguf",
            url = "{HF}/bartowski/Llama-3.2-3B-Instruct-GGUF/resolve/5ab33fa94d1d04e903623ae72c95d1696f09f9e8/Llama-3.2-3B-Instruct-Q4_K_M.gguf",
            bytes = 2019377696l, sha256 = "6c1a2b41161032677be168d354123594c0e6e67d2b9227c84f296ad037c728ff"),
        ModelSpec(file = "Llama-3.2-3B-Instruct-Q8_0.gguf", display = "Llama-3.2-3B",
            url = "{HF}/bartowski/Llama-3.2-3B-Instruct-GGUF/resolve/5ab33fa94d1d04e903623ae72c95d1696f09f9e8/Llama-3.2-3B-Instruct-Q8_0.gguf",
            bytes = 3421899296l, sha256 = "b5607b5090a8280063fff2d706bb3408ca6542341b06aab39c3eca0a28575921"),
        ModelSpec(file = "gemma-4-E2B-it-Q4_K_M.gguf",
            url = "{HF}/unsloth/gemma-4-E2B-it-GGUF/resolve/0314792d7f1f7e229411f620751375812bb9faf2/gemma-4-E2B-it-Q4_K_M.gguf",
            bytes = 3106738272l, sha256 = "740185b21d22ceb83a11c3aa62ad5842ef32c70f6096d756bbee85a1e4ec34b8",
            serve_name = "gemma-4-e2b", serve_display = "Gemma 4 E2B", serve_default = true,
            serve_note = "the recommended default - fast, capable, runs everywhere",
            serve_ctx = 32768, serve_vram_gb = 4.0,
            serve_vision = "mmproj-gemma-4-E2B-it-bf16.gguf",
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "gemma-4-E4B-it-Q4_K_M.gguf",
            url = "{HF}/unsloth/gemma-4-E4B-it-GGUF/resolve/bfc15c382204943c3a8fff0c750b94ae2364d7a3/gemma-4-E4B-it-Q4_K_M.gguf",
            bytes = 4977171584l, sha256 = "85a896a047553e842f25297ee5b031d64ff30147d9c4af17b1e4b394cd1fab87",
            serve_name = "gemma-4-e4b", serve_display = "Gemma 4 E4B",
            serve_note = "the bigger E-series - better answers, still laptop-class",
            serve_ctx = 32768, serve_vram_gb = 6.5,
            serve_vision = "mmproj-gemma-4-E4B-it-BF16.gguf"),
        // no 12B serving row yet: the fleet's local gemma-4-12B-it-Q4_K_M bytes match no
        // current upstream revision (the .WRONG-BYTES saga) - it joins the catalog once its
        // provenance is reconciled to a pinnable upload
        ModelSpec(file = "gemma-4-26B-A4B-it-UD-Q4_K_M.gguf",
            url = "{HF}/unsloth/gemma-4-26B-A4B-it-GGUF/resolve/c099eb48e663fd284577b04978a94ffccb261841/gemma-4-26B-A4B-it-UD-Q4_K_M.gguf",
            bytes = 16947541728l, sha256 = "f2c28b3dc4776931ac6f879e11f203dec637ea0f14267a86ec8f6165f63f293f",
            serve_name = "gemma-4-26b-a4b", serve_display = "Gemma 4 26B-A4B",
            serve_note = "the MoE - 26B quality at 4B active weights per token",
            serve_ctx = 131072, serve_vram_gb = 19.0, serve_mtp = "metal, vulkan",
            serve_drafter = "mtp-gemma-4-26B-A4B-it-Q8_0.gguf"),
        ModelSpec(file = "Qwen3.8-27B-UD-Q4_K_M.gguf",
            url = "{HF}/unsloth/Qwen3.8-27B-GGUF/resolve/27af057ecb382ddfea5d12837360a8980560e3ed/Qwen3.8-27B-UD-Q4_K_M.gguf",
            bytes = 16464440224l, sha256 = "322e194ff79741c7baa497c240f677f54b201b0efab44ca8e50f122b39123482",
            serve_name = "qwen3.8-27b", serve_display = "Qwen 3.8 27B",
            serve_note = "the newest dense Qwen - thinking model, strong at code",
            serve_ctx = 262144, serve_vram_gb = 18.5, serve_mtp = "metal, vulkan",
            parity <- ev_qwen_count([dn_chunked = true])),
        ModelSpec(file = "gemma-3-4b-it-Q8_0.gguf", display = "gemma-3-4B",
            // the vision mmproj rides as a companion: the gemma3v tower tests and the image chat
            // load it beside this decoder (the tower itself is size-invariant; 4B is the tested pair)
            companions <- [
                ProvEntry(name = "mmproj-gemma-3-4b-it-F16.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-3-4b-it-GGUF/resolve/main/mmproj-model-f16.gguf",
                    bytes = 851251104l, sha256 = "8c0fb064b019a6972856aaae2c7e4792858af3ca4561be2dbf649123ba6c40cb")],
            parity <- ev_gemma3_4b()),
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q8_0.gguf", display = "Qwen3-4B",
            url = "{HF}/bartowski/Qwen_Qwen3-4B-Instruct-2507-GGUF/resolve/ae44f08e1392f39c0e474af10c3ff8355c8b6688/Qwen_Qwen3-4B-Instruct-2507-Q8_0.gguf",
            bytes = 4280405216l, sha256 = "260b5b5b6ad73e44df81a43ea1f5c11c37007b6bac18eb3cd2016e8667c19662",
            parity <- ev_qwen_count()),
        // the native K-quant rail's token-level gate: the same counting fixture over the
        // Q4_K_M / Q5_K_M / Q6_K files of the SAME model, forced through kquant_native planes
        // (grp repack + kq kernels + kq embed gather). Oracles produced per-file by simple_ids.
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", display = "Qwen3-4B-q4k", quant_label = "q4_k_m",
            url = "{HF}/unsloth/Qwen3-4B-Instruct-2507-GGUF/resolve/a06e946bb6b655725eafa393f4a9745d460374c9/Qwen3-4B-Instruct-2507-Q4_K_M.gguf",
            bytes = 2497281120l, sha256 = "3605803b982cb64aead44f6c1b2ae36e3acdb41d8e46c8a94c6533bc4c67e597",
            parity <- ev_qwen_count([kq_native = true])),
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q5_K_M.gguf",
            url = "{HF}/unsloth/Qwen3-4B-Instruct-2507-GGUF/resolve/a06e946bb6b655725eafa393f4a9745d460374c9/Qwen3-4B-Instruct-2507-Q5_K_M.gguf",
            bytes = 2889514080l, sha256 = "5bde5e9d883622acb02bf77fe7dcbc56a8b9a9ad4be78a72ca23a532658b4ecb",
            parity <- ev_qwen_count([kq_native = true])),
        ModelSpec(file = "Qwen3-4B-Instruct-2507-Q6_K.gguf",
            url = "{HF}/unsloth/Qwen3-4B-Instruct-2507-GGUF/resolve/a06e946bb6b655725eafa393f4a9745d460374c9/Qwen3-4B-Instruct-2507-Q6_K.gguf",
            bytes = 3306261600l, sha256 = "cd7b21b38b3e71400587c184b6a9b04d3beb4d13fdae6464d4075dee4f1bc5ad",
            parity <- ev_qwen_count([kq_native = true])),
        // the base model the Instruct-2507 rows fine-tune: the regular DFlash drafter's Q8_0 and Q4_K_M files ride beside it
        // for the drafter cells of the qwen3 resident file and the drafter test's Metal arms
        ModelSpec(file = "Qwen3-4B-Q4_K_M.gguf",
            url = "{HF}/Qwen/Qwen3-4B-GGUF/resolve/bc640142c66e1fdd12af0bd68f40445458f3869b/Qwen3-4B-Q4_K_M.gguf",
            bytes = 2497280256l, sha256 = "7485fe6f11af29433bc51cab58009521f205840f5b4ae3a32fa7f92e8534fdf5",
            companions <- [
                ProvEntry(name = "drafters/Qwen3-4B-DFlash-b16-GGUF/Qwen3-4B-DFlash-b16-Q8_0.gguf", root = "llm",
                    url = "{HF}/Anbeeld/Qwen3-4B-DFlash-b16-GGUF/resolve/5cbf9d2073be26ca6ce6ecaf1ec71731e818af11/Qwen3-4B-DFlash-b16-Q8_0.gguf",
                    bytes = 577047104l, sha256 = "84b096744606212b63a33324eed795bd2c24d4acb242062c3bc696356ec6d0b3"),
                ProvEntry(name = "drafters/Qwen3-4B-DFlash-b16-GGUF/Qwen3-4B-DFlash-b16-Q4_K_M.gguf", root = "llm",
                    url = "{HF}/Anbeeld/Qwen3-4B-DFlash-b16-GGUF/resolve/5cbf9d2073be26ca6ce6ecaf1ec71731e818af11/Qwen3-4B-DFlash-b16-Q4_K_M.gguf",
                    bytes = 322542144l, sha256 = "b70a02fd40bac7ad4edcf72ac8cf19d21c4e0cf7f0f19b1a8bf5c61fd31f9c7c")]),
        // the deltanet arm of the native-kq gate: recurrent-layer projections (attn_qkv /
        // attn_gate / ssm_out) ride kq-native planes on the CPU rail; the trio spans k4/k5/k6
        // dn planes (qkv mixes Q6_K into the Q4/Q5 files)
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
        // this row owns the carrier's provenance and PINS its mmproj companion (the serving
        // Q4_K_M row consumes the same tower by name via serve_vision); asr_catalog keeps
        // only the pairing
        ModelSpec(file = "gemma-4-E2B-it-Q8_0.gguf", display = "gemma-4-E2B", official = false,
            url = "{HF}/ggml-org/gemma-4-E2B-it-GGUF/resolve/main/gemma-4-E2B-it-Q8_0.gguf",
            bytes = 4967497152l, sha256 = "996d08777aadc6bfd3c7375ef70ba25a0f55240075860754fdb18d6d860aa63a",
            companions <- [
                ProvEntry(name = "mmproj-gemma-4-E2B-it-bf16.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-4-E2B-it-GGUF/resolve/b4243c156154b6dca9324415f8c7ccc098b4aed1/mmproj-gemma-4-E2B-it-BF16.gguf",
                    bytes = 986833664l, sha256 = "711e1e8f43fa0664adbac493129be1e6c25b81af4b4cdea97c7d798b25c0a3a4"),
                // the DFlash2 block drafter the drafter test's E2B arms attach: no upstream GGUF is published; the format repo hosts the local conversion
                ProvEntry(name = "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-Q8_0.gguf", root = "llm",
                    url = "{FORMATS_HF}/gemma-4-E2B-it-dflash2/gemma-4-E2B-it-dflash2-Q8_0.gguf",
                    recipe = "convert_hf_to_gguf.py <rasyosef/gemma-4-E2B-it-dflash2 snapshot> --target-model-dir <google/gemma-4-E2B-it snapshot> --outtype q8_0 (llama.cpp)",
                    bytes = 359252960l, sha256 = "e90b2224a233be4de4374edde1737a53582048d9845600a1edf7bd03cb0112aa"),
                // the same drafter in Q4_K_M and NVFP4: the drafter test's per-format legs on the E2B pair
                ProvEntry(name = "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-Q4_K_M-local.gguf", root = "llm",
                    url = "{FORMATS_HF}/gemma-4-E2B-it-dflash2/gemma-4-E2B-it-dflash2-Q4_K_M-local.gguf",
                    recipe = "llama-quantize --allow-requantize gemma-4-E2B-it-dflash2-Q8_0.gguf gemma-4-E2B-it-dflash2-Q4_K_M-local.gguf Q4_K_M (llama.cpp with the dflash arch and NVFP4)",
                    bytes = 203043808l, sha256 = "7a14a7df225d87fb2cf5eced79dca18d7a1cab5e6ca015a7fe2ca4f899791a77"),
                ProvEntry(name = "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-NVFP4-pure-local.gguf", root = "llm",
                    url = "{FORMATS_HF}/gemma-4-E2B-it-dflash2/gemma-4-E2B-it-dflash2-NVFP4-pure-local.gguf",
                    recipe = "llama-quantize --allow-requantize --pure gemma-4-E2B-it-dflash2-Q8_0.gguf gemma-4-E2B-it-dflash2-NVFP4-pure-local.gguf NVFP4 (llama.cpp with the dflash arch and NVFP4)",
                    bytes = 197772256l, sha256 = "06ec31ef1433d78f99288c59064bcdf053bdd2fa2fe08bdb5e425d113f6f5369")],
            // PLE + cross-layer KV sharing as the E4B, plus per-layer FFN width (the elastic
            // MatFormer sub-model); dense, so the full 40 hold
            parity <- ev_gemma4_dense()),
        // the resident gates' large-tier carriers (tests/test_gpu_resident_gemma4_dense / _moe): provenance rows,
        // not board rows - resolve/main floats, so the bytes and sha256 pin the file
        ModelSpec(file = "gemma-4-12B-it-Q8_0.gguf",
            url = "{HF}/ggml-org/gemma-4-12B-it-GGUF/resolve/main/gemma-4-12B-it-Q8_0.gguf",
            bytes = 12669646976l, sha256 = "abfc3044b93795f6a446fc1a04468ac2d5fc5e9c92e237f09a5b3384c5a433e4",
            parity <- ev_gemma_count()),
        ModelSpec(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf",
            url = "{HF}/unsloth/gemma-4-26B-A4B-it-GGUF/resolve/main/gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf",
            bytes = 11416548832l, sha256 = "3b7aec683e176e1d957d007e0fb590611d82aec4db358d99b7f9714b6bc7bfc2",
            parity <- ev_gemma_count([kq_native = true])),
        // the native-MXFP4 gemma-4 expert carriers (the fused gate_up and the down stacks MXFP4, every other plane the source's):
        // the M5's off the Q8_0 file, the pod's off the Q4_K_M file the pod stocks
        ModelSpec(file = "gemma-4-26B-A4B-it-MXFP4_MOE-local.gguf", bytes = 14727181312l,
            sha256 = "37b45b5b2865a290c83377542b19c9058321b188264990866372d2e10fda69a8",
            recipe = "llama-quantize --allow-requantize gemma-4-26B-A4B-it-Q8_0.gguf gemma-4-26B-A4B-it-MXFP4_MOE-local.gguf MXFP4_MOE"),
        ModelSpec(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf",
            recipe = "llama-quantize --allow-requantize gemma-4-26B-A4B-it-Q4_K_M.gguf gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf MXFP4_MOE"),
        ModelSpec(file = "gemma-4-31B-it-Q4_K_M.gguf",
            url = "{HF}/lmstudio-community/gemma-4-31B-it-GGUF/resolve/main/gemma-4-31B-it-Q4_K_M.gguf",
            bytes = 18687063904l, sha256 = "8f3479643461601703f717931e399cc440dbd5ada598c025d6296c8165d94be9"),
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf",   // parity-only carrier
            url = "{HF}/RichardErkhov/Qwen_-_Qwen1.5-MoE-A2.7B-Chat-gguf/resolve/947580e7b3904cd4081810d0efec4520a75609c7/Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf",
            bytes = 15226010336l, sha256 = "d20e59b6180c775c648703fd63b8df7d01fc107672441fc3746c41f1dcaf904c",
            // the routed-experts arch (softmax router -> top-4 of 60 with un-renormalized
            // weights + a sigmoid-gated shared expert; QKV bias, NEOX rope); the fixture runs
            // BOTH MoE prefill paths off one load — bit-identical by construction
            parity <- ev_qwen_count([moe_reference = true])),
        // the small twin of the routed-experts arch for the resident MoE chain's cells
        // (tests/test_gpu_resident_qwen_moe.das): 9.5 GB, whole on a 16 GB card with room to spare
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf",
            url = "{FORMATS_HF}/Qwen1.5-MoE-A2.7B-Chat/Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", bytes = 9496236768l, sha256 = "d2f8376b8391888b55548566a1e06afa8250c7c9e25dbf5da2a9a2b033984964",
            recipe = "llama-quantize --allow-requantize Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf Q4_K_M",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        // the per-32 shared-expert and routed carriers (`--pure`: every tensor in the file's format): the Q5_1 file's
        // shared expert serves as q51 planes, both files' 1408-wide routed rows (off the 256 lattice) as q51 / q4032
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat-Q5_1-local.gguf",
            url = "{FORMATS_HF}/Qwen1.5-MoE-A2.7B-Chat/Qwen1.5-MoE-A2.7B-Chat-Q5_1-local.gguf", bytes = 10753342176l, sha256 = "db5715e5a2a22fde7044f4f7c73b6f42d19054fc6647654f075f99e6258a6c3f",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type q5_1 --output-tensor-type q5_1 Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf Qwen1.5-MoE-A2.7B-Chat-Q5_1-local.gguf Q5_1"),
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_0-local.gguf",
            url = "{FORMATS_HF}/Qwen1.5-MoE-A2.7B-Chat/Qwen1.5-MoE-A2.7B-Chat-Q4_0-local.gguf", bytes = 8069741280l, sha256 = "37d0028795313dee184176f93358c06f1f330277338921122ddbb03e1e2c989c",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type q4_0 --output-tensor-type q4_0 Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf Qwen1.5-MoE-A2.7B-Chat-Q4_0-local.gguf Q4_0"),
        // the q5032 expert carrier: every plane Q5_0, the routed and shared stacks among them
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat-Q5_0-pure-local.gguf",
            url = "{FORMATS_HF}/Qwen1.5-MoE-A2.7B-Chat/Qwen1.5-MoE-A2.7B-Chat-Q5_0-pure-local.gguf", bytes = 9858808544l,
            sha256 = "700d2386a8ef18bdb1e5f65df612a3014d10723e6e50f74af19251b87e4b8224",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type q5_0 --output-tensor-type q5_0 Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf Qwen1.5-MoE-A2.7B-Chat-Q5_0-pure-local.gguf Q5_0"),
        // the qwen vision pairs (the qwen3vl arc): parity-less provenance carriers —
        // the deepstack and qwen25o chat cells (test_qwen3v / test_qwen25v) and the qwen tier-1 suites are their coverage
        ModelSpec(file = "Qwen3VL-4B-Instruct-Q8_0.gguf", display = "Qwen3-VL-4B", official = true,
            url = "{HF}/Qwen/Qwen3-VL-4B-Instruct-GGUF/resolve/1cd86afb9a95c410a6038ab3b40d8b578c892266/Qwen3VL-4B-Instruct-Q8_0.gguf",
            serve_name = "qwen3-vl-4b", serve_display = "Qwen3 VL 4B",
            serve_note = "the deepstack vision Qwen - reads images through wide multi-tap rows",
            serve_ctx = 262144, serve_vram_gb = 6.0,
            serve_vision = "mmproj-Qwen3VL-4B-Instruct-F16.gguf",
            bytes = 4280406144l, sha256 = "054721f478bc5fa6beffb7f38eae575d45298f88cbb8d2f83ef675a727863eb1",
            // verified 40/40 against the live oracle 2026-08-30 (the deepstack tower's carrier)
            parity <- ev_qwen_count(),
            companions <- [
                ProvEntry(name = "mmproj-Qwen3VL-4B-Instruct-F16.gguf", root = "llm",
                    url = "{HF}/Qwen/Qwen3-VL-4B-Instruct-GGUF/resolve/1cd86afb9a95c410a6038ab3b40d8b578c892266/mmproj-Qwen3VL-4B-Instruct-F16.gguf",
                    bytes = 836180256l, sha256 = "256f3a43bd4205ffef48d6b92715e1e70b5b0e9aef06522584967513a9985331")]),
        ModelSpec(file = "Qwen3VL-8B-Instruct-Q8_0.gguf", display = "Qwen3-VL-8B",
            url = "{HF}/Qwen/Qwen3-VL-8B-Instruct-GGUF/resolve/main/Qwen3VL-8B-Instruct-Q8_0.gguf",
            bytes = 8709519456l, sha256 = "0d264b3941185d00a74f75c4245521dae088ff1efc90ab8d1754e83f5844adb0",
            companions <- [
                ProvEntry(name = "mmproj-Qwen3VL-8B-Instruct-F16.gguf", root = "llm",
                    url = "{HF}/Qwen/Qwen3-VL-8B-Instruct-GGUF/resolve/main/mmproj-Qwen3VL-8B-Instruct-F16.gguf",
                    bytes = 1159029824l, sha256 = "ca524100ebf825c9a870db1c580d03879e0da0ab2541697e2458e64891cf9d38")]),
        ModelSpec(file = "Qwen2.5-Omni-3B-Q8_0.gguf", display = "Qwen2.5-Omni-3B", official = true,
            url = "{HF}/ggml-org/Qwen2.5-Omni-3B-GGUF/resolve/75f1b73b657a50f5092502799457ccb4a4a1f9df/Qwen2.5-Omni-3B-Q8_0.gguf",
            serve_name = "qwen2.5-omni-3b", serve_display = "Qwen2.5 Omni 3B",
            serve_note = "the window-ViT omni - compact vision chat on the Metal tower",
            serve_ctx = 32768, serve_vram_gb = 5.5,
            serve_vision = "mmproj-Qwen2.5-Omni-3B-f16.gguf",
            bytes = 3616087360l, sha256 = "c7d8b07c8d8d7a9ed1de1b8df7ac821eb4d259a224bd44310baacfaa5a473d4c",
            // verified 40/40 against the live oracle 2026-08-30 (the window-ViT tower's carrier)
            parity <- ev_qwen_count(),
            companions <- [
                ProvEntry(name = "mmproj-Qwen2.5-Omni-3B-f16.gguf", root = "llm",
                    url = "{HF}/ggml-org/Qwen2.5-Omni-3B-GGUF/resolve/75f1b73b657a50f5092502799457ccb4a4a1f9df/mmproj-Qwen2.5-Omni-3B-f16.gguf",
                    bytes = 2623983328l, sha256 = "f6d9276e9fa4f060c7abdbe886786cf31a8911b62770f8a54b7581b7b99fa27e")]),
        // parity-less provenance carrier: the gemma3 vision 12B pair (the same SigLIP tower at
        // projection 3840; test_vision_chat's PARITY_FULL leg is its coverage)
        ModelSpec(file = "gemma-3-12b-it-Q4_K_M.gguf",
            url = "{HF}/ggml-org/gemma-3-12b-it-GGUF/resolve/main/gemma-3-12b-it-Q4_K_M.gguf",
            bytes = 7300574976l, sha256 = "7bb69bff3f48a7b642355d64a90e481182a7794707b3133890646b1efa778ff5",
            companions <- [
                ProvEntry(name = "mmproj-gemma-3-12b-it-F16.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-3-12b-it-GGUF/resolve/main/mmproj-model-f16.gguf",
                    bytes = 854200224l, sha256 = "30c02d056410848227001830866e0a269fcc28aaf8ca971bded494003de9f5a5")],
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "gemma-4-12B-it-Q4_K_M.gguf", official = false,
            url = "{HF}/lmstudio-community/gemma-4-12B-it-GGUF/resolve/main/gemma-4-12B-it-Q4_K_M.gguf",
            bytes = 7381382944l, sha256 = "95d83ba36642b1f385fb906b5962a71763361be3bac930a709945f72d97473f8",
            // this file's own oracle (2026-08-18) continues the dense fixture byte-identically
            parity <- ev_gemma4_dense(),
            companions <- [
                ProvEntry(name = "mmproj-gemma-4-12B-it-BF16.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-4-12B-it-GGUF/resolve/main/mmproj-gemma-4-12B-it-BF16.gguf",
                    bytes = 175115616l, sha256 = "9b1edfa05b634728ca4bfd60b4e6b278e95166c078fa54ae4fa83e680112fd1d")]),
        ModelSpec(file = "gemma-4-E4B-it-Q8_0.gguf", display = "gemma-4-E4B", official = true,
            mlx_repo = "mlx-community/gemma-4-e4b-it-8bit", mlx_fmt = "8bit",
            url = "{HF}/ggml-org/gemma-4-E4B-it-GGUF/resolve/main/gemma-4-E4B-it-Q8_0.gguf",
            bytes = 8031242688l, sha256 = "34be82b17b4942d389b9b527170c4b058027abdd32531fda063d3d97dd8ce80a",
            companions <- [
                ProvEntry(name = "mmproj-gemma-4-E4B-it-BF16.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-4-E4B-it-GGUF/resolve/b8093469224f83f5c38f691eb906c380e9e63114/mmproj-gemma-4-E4B-it-BF16.gguf",
                    bytes = 991552256l, sha256 = "f77995e4b6a569ab8f0d1bfdb7e8da4a0fa5b9e6f309b9bf3bdb76164d75e29f"),
                // path-style S3 spelling: the images.cocodataset.org website endpoint is http-only
                ProvEntry(name = "coco-val2017-000000039769.jpg", root = "llm",
                    url = "https://s3.amazonaws.com/images.cocodataset.org/val2017/000000039769.jpg",
                    bytes = 173131l, sha256 = "dea9e7ef97386345f7cff32f9055da4982da5471c48d575146c796ab4563b04e")],
            // PLE (per-layer embeddings) + cross-layer KV sharing over the last 18 layers;
            // dense, so the full 40 hold (verified 40/40 vs simple_ids on the window-engaged
            // 1..320 counting prompt)
            parity <- ev_gemma4_dense()),
        ModelSpec(file = "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf", display = "Llama-3.1-8B"),
        ModelSpec(file = "Meta-Llama-3.1-8B-Instruct-Q4_K_M.gguf", display = "Llama-3.1-8B-q4km", quant_label = "q4_k_m"),
        // gpt-oss is revision-pinned (upstream re-uploads bit-different files under the same
        // name); its parity fixtures run through test_parity's own shared-load arm — the
        // ~13GB native-mx4 resident is the suite's biggest, loaded once for both fixtures
        ModelSpec(file = "gpt-oss-20b-mxfp4.gguf", display = "gpt-oss-20b", quant_label = "mxfp4", official = true,
            mlx_repo = "mlx-community/gpt-oss-20b-MXFP4-Q8", mlx_fmt = "mxfp4/q8",
            url = "{HF}/ggml-org/gpt-oss-20b-GGUF/resolve/e1dc459feff949ff451ce107337a2026daa80df8/gpt-oss-20b-mxfp4.gguf",
            bytes = 12109566560l, sha256 = "be37a636aca0fc1aae0d32325f82f6b4d21495f06823b5fbc1898ae0303e9935",
            parity <- ev_gptoss()),
        // the 20b's first four blocks, two sliding and two full: the gpt-oss paths at a size a per-PR run loads.
        // No parity row: four blocks of twenty-four write no confident text, and test_parity holds its forced feed
        cut_row("gpt-oss-20b", "gpt-oss-20b-mxfp4.gguf", "gpt-oss",
            "mxfp4", 4, false, 3054636512l,
            "ff9885f578470ed39d56371832c1b43a2221c4428b040b933091f7f7de93ebe6"),
        // the dense and the routed gemma-4 at a size a per-PR run loads: the 12B's first twelve blocks (two global
        // layers) and the 26B's first six (one), the final soft cap lifted - a cut's logits sit far past it, and the cap
        // would pin most of every row. No parity rows, as the gpt-oss cut
        cut_row("gemma-4-12B-it", "gemma-4-12B-it-Q4_K_M.gguf", "gemma-4-12B",
            "Q4_K_M", 12, true, 2508950848l,
            "b9eee5f3ab5420f339daf2e3ba4b9948d44edcb3448926c3d2a6302539fbad96"),
        cut_row("gemma-4-26B-A4B-it", "gemma-4-26B-A4B-it-Q4_K_M.gguf", "gemma-4-26B-A4B",
            "Q4_K_M", 6, true, 4013753760l,
            "28b682d9320a0755a4775555bdcbd11e23966839f4a7e99eb88c688fca80cece"),
        // the same cuts of the formats whose own rails a device cell pins: the 12B's Q8_0 planes, the 26B's Q8_0, Q4_0,
        // IQ2_S and native-MXFP4 expert stacks
        cut_row("gemma-4-12B-it", "gemma-4-12B-it-Q8_0.gguf", "gemma-4-12B",
            "Q8_0", 12, true, 3981422240l,
            "3c4e3762dc24cc41d3de290b2eebb5b00ca32a34d9e58e0aedb4dce01598e79d"),
        cut_row("gemma-4-26B-A4B-it", "gemma-4-26B-A4B-it-Q8_0.gguf", "gemma-4-26B-A4B",
            "Q8_0", 6, true, 6012076736l,
            "b5863a75da6447914b2ed01141c63a8c8d530825f50275edac4aa75c61026d45"),
        cut_row("gemma-4-26B-A4B-it", "gemma-4-26B-A4B-it-Q4_0.gguf", "gemma-4-26B-A4B",
            "Q4_0", 6, true, 3563733696l,
            "bcb1beab971e6f3139679ce44307838e7f321ef0ec6cdacbb4e4a59a637d0963"),
        cut_row("gemma-4-26B-A4B-it", "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", "gemma-4-26B-A4B",
            "UD-IQ3_XXS", 6, true, 2740961184l,
            "21e125c5e87982edb44ad19b9861749e5f11d38dcd04f8297339b09e1213b136"),
        cut_row("gemma-4-26B-A4B-it", "gemma-4-26B-A4B-it-MXFP4_MOE-local.gguf", "gemma-4-26B-A4B",
            "MXFP4_MOE", 6, true, 3585540800l,
            "a381dc268205cfbba038571b0aacfba05666c58c2181833fdcb11c94fae1e6e3"),
        // the qwen MoE families at a size a per-PR run loads, each carrier's formats whole: Qwen1.5-MoE's first four blocks,
        // Qwen3-30B-A3B's first seven (the Q4_K_M file's first Q4_K down stack sits at block 6), Qwen3.6-35B-A3B's first
        // four (three deltanet layers and one attention layer), the MTP files' NextN block kept after them. No parity rows,
        // as the gpt-oss cut
        cut_row("Qwen1.5-MoE-A2.7B-Chat", "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf", "Qwen1.5-MoE-A2.7B",
            "Q8_0", 4, false, 3093637248l,
            "5cc2a7813af497e5eb78a5a1dd4a207528153842159ea6f24a8e363f1d5ced0a"),
        cut_row("Qwen1.5-MoE-A2.7B-Chat", "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", "Qwen1.5-MoE-A2.7B",
            "Q4_K_M", 4, false, 2015158400l,
            "8efb94f1a6b764e64cdc0535ab82b5561dc83495621f071828680d4ad82a7fcf"),
        cut_row("Qwen1.5-MoE-A2.7B-Chat", "Qwen1.5-MoE-A2.7B-Chat-Q5_1-local.gguf", "Qwen1.5-MoE-A2.7B",
            "Q5_1", 4, false, 2186127488l,
            "6b98df972d28666c661fc92b6defb8441a0e338af99f14cc3733942290848828"),
        cut_row("Qwen1.5-MoE-A2.7B-Chat", "Qwen1.5-MoE-A2.7B-Chat-Q5_0-pure-local.gguf", "Qwen1.5-MoE-A2.7B",
            "Q5_0-pure", 4, false, 2004625536l,
            "933da0a74e443efd4f219321fe815ea53e83a323cabb0652613f14c62393ce20"),
        cut_row("Qwen1.5-MoE-A2.7B-Chat", "Qwen1.5-MoE-A2.7B-Chat-Q4_0-local.gguf", "Qwen1.5-MoE-A2.7B",
            "Q4_0", 4, false, 1641621632l,
            "2a4daf36e4205b7aa8f3144a42a15763da23174dd0e9110c87e992efee404fc3"),
        cut_row("Qwen3-30B-A3B-Instruct-2507", "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", "Qwen3-30B-A3B",
            "Q4_K_M", 7, false, 3209229440l,
            "e285387655a6030b01ac4f59ca35c6f738f1b09540b998249ab60b13760dddea"),
        cut_row("Qwen3-30B-A3B-Instruct-2507", "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf", "Qwen3-30B-A3B",
            "UD-IQ2_XXS", 7, false, 1928344704l,
            "158752cc15a3976ea3652e02222cfafc67621a97ff57fbd781318bcfda6d4b88"),
        cut_row("Qwen3-30B-A3B-Instruct-2507", "Qwen3-30B-A3B-Instruct-2507-UD-Q3_K_XL.gguf", "Qwen3-30B-A3B",
            "UD-Q3_K_XL", 7, false, 2421765248l,
            "9d5c85a91e160db53d5afdad8be881de09366a1ff936d31fa5c96d92089d5a71"),
        cut_row("Qwen3-30B-A3B", "Qwen3-30B-A3B-NVFP4-pure-local.gguf", "Qwen3-30B-A3B",
            "NVFP4-pure", 7, false, 2815957248l,
            "18a1ceb15bffd26924b7daa1c9cc669477445c68e61f65b006196942c484d41e"),
        cut_row("Qwen3.6-35B-A3B", "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", "Qwen3.6-35B-A3B",
            "UD-Q4_K_M", 4, false, 3074384032l,
            "0f8a80a82fa1cf9bf661731f257c09f3e4223b9787a0761f86b9236f71757d47"),
        cut_row("Qwen3.6-35B-A3B", "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", "Qwen3.6-35B-A3B",
            "MTP-UD-Q4_K_M", 4, false, 3603242432l,
            "0ff2c681740e41750321f994c76123f18029df6f5129871ce2e276d15d9a9c39"),
        cut_row("Qwen3.6-35B-A3B", "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf", "Qwen3.6-35B-A3B",
            "MTP-IQ4_XS-pure", 4, false, 2795842752l,
            "5325c3b17d2dc01cd7cd3e34caa8613b93b7dfa2b5ae6d672b7b193c9a038f17"),
        cut_row("Qwen3.6-35B-A3B", "Qwen3.6-35B-A3B-UD-IQ3_S.gguf", "Qwen3.6-35B-A3B",
            "UD-IQ3_S", 4, false, 2120269984l,
            "08d508ad6111a5eaebacc6c561ce16fbf23ff036746b51143da248a68460c038"),
        cut_row("Qwen3.6-35B-A3B", "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", "Qwen3.6-35B-A3B",
            "UD-IQ2_XXS", 4, false, 1591623840l,
            "69482587a2cb8bab2f210849e357468a6b01a9f5a65f91cdeb151ae040999134"),
        // the dense hybrid 27B's two low-bit carriers: the first eight blocks (two attention layers, the UD file's first Q2_K
        // plane at block 7) and the NextN block after them
        cut_row("Qwen3.8-27B", "Qwen3.8-27B.i1-IQ3_S.gguf", "Qwen3.8-27B",
            "i1-IQ3_S", 8, false, 3135880960l,
            "937f834282d5a68cb4d77319fbc9e93c18119f4d566cd4bc23f2ead1a559c5e2"),
        cut_row("Qwen3.8-27B", "Qwen3.8-27B-UD-Q3_K_XL.gguf", "Qwen3.8-27B",
            "UD-Q3_K_XL", 8, false, 3184544960l,
            "290c71c0d78c0cc683576dbc27fc5670b20913f32360647a47e9554b72180355"),
        ModelSpec(file = "Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf", official = false,
            url = "{HF}/unsloth/Mistral-Small-3.1-24B-Instruct-2503-GGUF/resolve/main/Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf",
            bytes = 14333910592l, sha256 = "6d670773c3908584349d41a5048d1472226b593c881fd394e8ac196c802e81e2",
            parity <- ev_mistral_small()),
        ModelSpec(file = "gemma-4-26B-A4B-it-Q8_0.gguf", display = "gemma-4-26B-A4B",
            // the gemma4 MoE member: every layer sums a DENSE shared expert and routed
            // top-8-of-128 experts under a custom-normalized router
            parity <- ev_gemma4_26b()),
        ModelSpec(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", display = "gemma-4-26B-A4B-q4k", quant_label = "q4_k_m", official = true,
            mlx_repo = "mlx-community/gemma-4-26b-a4b-it-4bit", mlx_fmt = "4bit",
            url = "{HF}/unsloth/gemma-4-26B-A4B-it-GGUF/resolve/main/gemma-4-26B-A4B-it-UD-Q4_K_M.gguf",
            bytes = 16947541728l, sha256 = "f2c28b3dc4776931ac6f879e11f203dec637ea0f14267a86ec8f6165f63f293f",
            // the assistant drafter rides as a companion: the mtp suite and the ruler attach it beside this target; its Q4_0
            // twin is the drafter file's type-decline fixture (the drafter serves a Q8_0 sidecar alone)
            companions <- [
                ProvEntry(name = "mtp-gemma-4-26B-A4B-it-Q8_0.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-4-26B-A4B-it-GGUF/resolve/bb4531cda34d1ea09d9814959ed4d5833cf2a4c8/mtp-gemma-4-26B-A4B-it-Q8_0.gguf",
                    bytes = 461766784l, sha256 = "ea4f772ffb70e02127c5f142f9ed9af97add7ca0f2bcc7b277244a9771b436bd"),
                ProvEntry(name = "mtp-gemma-4-26B-A4B-it-Q4_0.gguf", root = "llm",
                    url = "{HF}/ggml-org/gemma-4-26B-A4B-it-GGUF/resolve/bb4531cda34d1ea09d9814959ed4d5833cf2a4c8/mtp-gemma-4-26B-A4B-it-Q4_0.gguf",
                    bytes = 251939520l, sha256 = "e00b0ce326d03d8f6f6972ad2320fe90b6ff25a3da5fc51bec15efe283052aa2"),
                // the DFlash block drafter the drafter test's 26B arms attach: local conversions the format repo hosts; the GGUFs
                // Anbeeld/gemma-4-26B-A4B-it-DFlash-GGUF publishes of the same drafter read 352 bytes apart from them
                ProvEntry(name = "drafters/gemma-4-26B-A4B-it-DFlash-GGUF/gemma-4-26B-A4B-it-DFlash-Q8_0.gguf", root = "llm",
                    url = "{FORMATS_HF}/gemma-4-26B-A4B-it-DFlash/gemma-4-26B-A4B-it-DFlash-Q8_0.gguf",
                    recipe = "llama.cpp's convert_hf_to_gguf.py over the <z-lab/gemma-4-26B-A4B-it-DFlash snapshot> with --target-model-dir <google/gemma-4-26B-A4B-it snapshot>, to Q8_0 (the command line was not kept)",
                    bytes = 472433984l, sha256 = "a416057002fa1984fbf87507fde0dbfb357130bd2201b61e8dbd72ff81a929e0"),
                ProvEntry(name = "drafters/gemma-4-26B-A4B-it-DFlash-GGUF/gemma-4-26B-A4B-it-DFlash-Q4_K_M.gguf", root = "llm",
                    url = "{FORMATS_HF}/gemma-4-26B-A4B-it-DFlash/gemma-4-26B-A4B-it-DFlash-Q4_K_M.gguf",
                    recipe = "llama-quantize of the same conversion's BF16 file to Q4_K_M (the converter writes no K-quant; the command line was not kept)",
                    bytes = 267271488l, sha256 = "9276343964d49310065129213d6b3a5819042d88dba13f59bc0b9599548a7765")],
            // the q51 tier gate: this recipe's ffn_down_exps are Q5_1 on 29 layers, riding the
            // native q51 planes; the Q4_K gate_up stacks ride the kq planes
            parity <- ev_gemma4_26b_q4km()),
        // the QAT file rides the q40 KqFmt tier (native Q4_0 planes on the kq rails) — quant is
        // the loader mode (q8), the label the on-disk format, like the Q4_K_M rows
        ModelSpec(file = "gemma-4-26B_q4_0-it.gguf", display = "gemma-4-26B-A4B-q40", quant_label = "q4_0",
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "Qwen3.6-27B-MTP-Q4_K_M.gguf", official = false,
            url = "{HF}/unsloth/Qwen3.6-27B-MTP-GGUF/resolve/main/Qwen3.6-27B-Q4_K_M.gguf",
            bytes = 17106773120l, sha256 = "a7cbd3ecc0e3f9b333edee61ae66bc87ed713c5d49587a8355814722ed329e0f",
            // plain evidence only (verified per-file 40/40, 2026-08-18): the qwen35-dense arm
            // coverage (chunked deltanet) lives on the 0.8B/4B/27B carriers
            parity <- ev_qwen_count()),
        ModelSpec(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", display = "Qwen3-30B-A3B", quant_label = "q4_k_m", official = false,
            url = "{HF}/unsloth/Qwen3-30B-A3B-Instruct-2507-GGUF/resolve/main/Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf",
            bytes = 18556686752l, sha256 = "6c997b8af17debdfb01d890214400ccbab00db6acc0ba8da5de1cc906c4774d0",
            // the MoE arm of the native-kq gate: the grouped prefill gathers per-expert token
            // batches off the K-form batch image (the seam the dense arms never touch)
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        // the resident MoE chain's fixtures on a 16 GB card (tests/test_gpu_resident_qwen_moe.das): the 30B with no
        // shared expert, and the 35B deltanet hybrid with a gated one - the whole-model fits the UD-Q4_K_M cannot make
        ModelSpec(file = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf",
            url = "{HF}/unsloth/Qwen3-30B-A3B-Instruct-2507-GGUF/resolve/eea7b2be5805a5f151f8847ede8e5f9a9284bf77/Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf",
            bytes = 10341814688l, sha256 = "aa0b04ac05f71aa94b542b074500b3de956608365e74f0e1d97ae0d6a6380269",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf",
            url = "{HF}/unsloth/Qwen3.6-35B-A3B-GGUF/resolve/a483e9e6cbd595906af30beda3187c2663a1118c/Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf",
            bytes = 10756586464l, sha256 = "2e8f5f705355c56311432d0a8a5d14a696dbb7e4b197d05c75ba805fc1857bef",
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.8-27B-Q4_K_M.gguf", display = "Qwen3.8-27B", quant_label = "q4_k_m", official = true,
            mlx_repo = "mlx-community/Qwen3.8-27B-4bit", mlx_fmt = "4bit",
            url = "{HF}/ggml-org/Qwen3.8-27B-GGUF/resolve/0669b98607d47046c7c2b3f801011d54a08cfccf/Qwen3.8-27B-Q4_K_M.gguf",
            bytes = 18973870432l, sha256 = "31629f53165ab6a7dad8c9847dcfd1fdf55829dac1e6e748f4a68581b0033d34",
            serve_drafter = "mtp-Qwen3.8-27B-Q8_0.gguf",
            // the split NextN head rides as a companion: the mtp suite's 3.8-27b fixture and the ruler load it as the trunk's last shard;
            // the DFlash2 block drafter's Q8_0, Q4_K_M and NVFP4 files ride beside it for the drafter test's 27B arms
            companions <- [
                ProvEntry(name = "mtp-Qwen3.8-27B-Q8_0.gguf", root = "llm",
                    url = "{HF}/ggml-org/Qwen3.8-27B-GGUF/resolve/0669b98607d47046c7c2b3f801011d54a08cfccf/mtp-Qwen3.8-27B-Q8_0.gguf",
                    bytes = 3164006688l, sha256 = "cbf60a0c48b431bb61f1d49b8948dc88ac29c398d6dbdbbb2e6e89ef77eacc9a"),
                ProvEntry(name = "drafters/Qwen3.8-27B-DFlash2-GGUF/Qwen3.8-27B-DFlash2-Q8_0.gguf", root = "llm",
                    url = "{HF}/incoai/Qwen3.8-27B-DFlash2-GGUF/resolve/main/Qwen3.8-27B-DFlash2-Q8_0.gguf",
                    bytes = 2056414816l, sha256 = "651a1b2930c9825080480f79cf38057e681c9b175d580a357c14f67cf77ccf49"),
                ProvEntry(name = "drafters/Qwen3.8-27B-DFlash2-GGUF/Qwen3.8-27B-DFlash2-Q4_K_M.gguf", root = "llm",
                    url = "{HF}/incoai/Qwen3.8-27B-DFlash2-GGUF/resolve/main/Qwen3.8-27B-DFlash2-Q4_K_M.gguf",
                    bytes = 1143006816l, sha256 = "d169ad335929adc0d77e10e2523f968f3e75f547637ad739b0a137ccde111b5f"),
                ProvEntry(name = "drafters/Qwen3.8-27B-DFlash2-NVFP4-GGUF/Qwen3.8-27B-DFlash2-NVFP4.gguf", root = "llm",
                    url = "{HF}/costanzopadovano/Qwen3.8-27B-DFlash2-NVFP4-GGUF/resolve/main/Qwen3.8-27B-DFlash2-NVFP4.gguf",
                    bytes = 1094346016l, sha256 = "9bd8926d7d5701cee1f673dbab9e6359a739cbbfc348cb78966e8a01df8e9863")],
            // the qwen35 hybrid dense at scale, native K-quant planes
            parity <- ev_qwen_count([dn_chunked = true])),
        // the same UD-Q4_K_M weights under two local names: the MTP-stem file carries the MTP
        // head (the official-board carrier), the plain stem predates it (the profiled carrier)
        ModelSpec(file = "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", display = "Qwen3.6-35B-A3B", quant_label = "q4_k_m",
            // the qwen35moe hybrid (deltanet + routed experts + shared expert): both MoE
            // prefill paths and the chunked deltanet arm off one load
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", official = true,
            mlx_repo = "mlx-community/Qwen3.6-35B-A3B-4bit", mlx_fmt = "4bit",
            url = "{HF}/unsloth/Qwen3.6-35B-A3B-MTP-GGUF/resolve/5bc3e238d916f48a861bac2f8a1990a0e9b7e98d/Qwen3.6-35B-A3B-UD-Q4_K_M.gguf",
            bytes = 22663387424l, sha256 = "0b21525e972670ed59e1812e170b27c26355381f0656ecc4e25617ece7dac58b",
            serve_name = "qwen3.6-35b-a3b", serve_display = "Qwen 3.6 35B-A3B",
            serve_note = "the Qwen MoE - 3B active weights, MTP-ready",
            serve_ctx = 262144, serve_vram_gb = 24.0, serve_mtp = "metal, vulkan",
            // plain evidence only (verified per-file 40/40, 2026-08-18): the qwen35moe arm
            // coverage (chunked deltanet + reference MoE prefill) lives on the plain-stem twin
            parity <- ev_qwen_count()),
        // the single-format IQ4_XS twin with the MTP head (4.27 bpw): the fewest bytes of the 4-bit
        // ladder at the K-quant files' quality (PERF_LEDGER.md's 2026-10-06 equal-bytes entry); the
        // recipe is deterministic (no imatrix) from unsloth's BF16 shards, the published file its output.
        // The file exists for the bandwidth-bound boxes (the house's M4 Pro): the routed experts, 93 % of
        // its bytes, are IQ4_XS by design; every other plane is served in the file's own format too
        // (REVIEW_KQ_FORMATS.md) - a plane the engine reads as a q8 copy is a gap, not the file's recipe
        ModelSpec(file = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf",
            url = "{HF}/borisbat/Qwen3.6-35B-A3B-MTP-IQ4_XS-pure-GGUF/resolve/6a1d560e64f8a9058773e5c1521534d9ef533bc8/Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf",
            recipe = "llama-quantize --pure Qwen3.6-35B-A3B-BF16-00001-of-00002.gguf Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf IQ4_XS",
            bytes = 18951018528l, sha256 = "8296de4b5b62724ae7eac1cf005a65a2936f227ef2cafcee8d9bad0a25c4806c",
            serve_mtp = "vulkan"),
        // the NVFP4 35B (tests/test_gpu_resident_moe_formats.das): RedHatAI's NVFP4 checkpoint through llama.cpp's converter - every Linear plane nv4 under its `.scale`, the norms, SSM parameters, convs and router gates in float
        ModelSpec(file = "Qwen3.6-35B-A3B-NVFP4.gguf",
            url = "{HF}/knoopx/Qwen3.6-35B-A3B-NVFP4-GGUF/resolve/b1bb81d83149a74fc9c7179b539a796d93f18820/Qwen3.6-35B-A3B-NVFP4.gguf",
            bytes = 22499026048l, sha256 = "c1358e8808eb7695b364de30e84c415801656838392578250524c3b7fddd1906"),
        ModelSpec(file = "Qwen3-Coder-30B-A3B-Instruct-Q8_0.gguf", display = "Qwen3-Coder-30B",
            parity <- ev_qwen_count([moe_reference = true])),
        ModelSpec(file = "Qwen3-Coder-30B-A3B-Instruct-Q4_K_M.gguf", display = "Qwen3-Coder-30B-q4k", quant_label = "q4_k_m",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3-Coder-Next-Q4_K_M.gguf", display = "Qwen3-Coder-Next-q4k", quant_label = "q4_k_m",
            // arch qwen3next: the FILE layout differs from qwen35 (fused Q4_K ssm_ba, HF
            // grouped v-head order) — this fixture proves the loader's grouped->tiled permutes
            parity <- ev_qwen_count([dn_chunked = true])),
        // arch qwen4exp (Qwen3.8-Flash-Next, 125B-A6B): the qwen35moe hybrid inside hyper-connections
        // + the n-gram PLE side input; a 3-shard file named by its first shard (the split-shard walk
        // maps the rest). No board row: Metal serves the file whole (test_metal_support_matrix.das's
        // qwen4exp row), while the Vulkan driver sums its routed experts on the host by construction
        // (ARCHITECTURE_GPU_VULKAN_HC.md), so the two homes measure different forms
        ModelSpec(file = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf",
            display = "Qwen3.8-Flash-Next-iq4xs", quant_label = "iq4_xs", serve_mtp = "vulkan",
            serve_drafter = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf",
            // the split NextN head (`shared`: its own Q8_0 routed experts inside) rides as a companion: the hc cells pin it as the trunk's head
            companions <- [
                ProvEntry(name = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf", root = "llm",
                    url = "{HF}/unsloth/Qwen3.8-Flash-Next-GGUF/resolve/main/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf",
                    bytes = 2786568256l, sha256 = "5ff54097406a905cf3a724c709124ceb0e3e10235ee862298969e91c96fa96e6")],
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        // the UD file's single-format twin, 4.1 GB a token against 5.7 - the bandwidth-bound decode floor; the pack cell opens it beside the split NextN head
        ModelSpec(file = "Qwen3.8-Flash-Next-GGUF/IQ4_XS-pure/Qwen3.8-Flash-Next-IQ4_XS-pure.gguf",
            display = "Qwen3.8-Flash-Next-iq4xs-pure", quant_label = "iq4_xs", serve_mtp = "vulkan",
            serve_drafter = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf",
            companions <- [
                ProvEntry(name = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf", root = "llm",
                    url = "{HF}/unsloth/Qwen3.8-Flash-Next-GGUF/resolve/main/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf",
                    bytes = 2786568256l, sha256 = "5ff54097406a905cf3a724c709124ceb0e3e10235ee862298969e91c96fa96e6")],
            recipe = "llama-quantize --pure --tensor-type 'hc_(attn|ffn)_(down|up)=q8_0' --tensor-type 'output_hc_(down|up)=q8_0' --tensor-type 'ple_(key|value)=q8_0' --tensor-type 'indexer\\.(k|q)_proj=bf16' --tensor-type 'ssm_(alpha|beta)=f32' Qwen3.8-Flash-Next-GGUF/BF16/Qwen3.8-Flash-Next-BF16-00001-of-00008.gguf Qwen3.8-Flash-Next-GGUF/IQ4_XS-pure/Qwen3.8-Flash-Next-IQ4_XS-pure.gguf IQ4_XS",
            bytes = 97515364448l, sha256 = "266607cefd0e10717ae4132c1ba069f39b82458b95ec476d8b2999d8af4e7086",
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        // the hyper-connection chain's micro carrier (Lynxpda's random-weight qwen4exp: hc 4, the PLE 3-gram side input,
        // 8 experts top-2, a NextN head) tests/test_gpu_resident_hc_micro.das holds whole; the published head rides as a
        // companion, and the `-hcmixer` twin the cells load - the head mixer copied off the shard's own output_hc_* -
        // is minted off it by the recipe. No board row: random weights say nothing about text
        ModelSpec(file = "micro-qwen4exp/micro-qwen4exp-realqsa.gguf",
            url = "{HF}/Lynxpda/micro-qwen4exp/resolve/main/micro-qwen4exp-realqsa.gguf",
            bytes = 416452160l, sha256 = "b5ea5c6df7b079095bd1d375d477f476a1913df9c87412ed7e9800a8cc3909d7",
            companions <- [
                ProvEntry(name = "micro-qwen4exp/micro-qwen4exp-realqsa-mtp.gguf", root = "llm",
                    url = "{HF}/Lynxpda/micro-qwen4exp/resolve/main/micro-qwen4exp-realqsa-mtp.gguf",
                    bytes = 148810048l, sha256 = "4b4373fb211fce86a6c4bd0ea02a6e46762512cd4470177f497b4e32baaed924"),
                ProvEntry(name = "micro-qwen4exp/micro-qwen4exp-realqsa-mtp-hcmixer.gguf", root = "llm",
                    recipe = "python modules/dasLLAMA/harness/micro_qwen4exp_hcmixer.py micro-qwen4exp-realqsa-mtp.gguf micro-qwen4exp-realqsa-mtp-hcmixer.gguf",
                    bytes = 150002176l, sha256 = "4fd26c949319887f45e4a7a11ae92831ff2515f5b73909f30150d2986b678ade")]),
        // the Metal sweep's Qwen quant matrix - Qwen text files the M5 box stocks beyond the rows
        // above, not every one; no board row, the counting fixture as the
        // correctness gate
        ModelSpec(file = "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf", display = "Qwen2.5-1.5B-iq3xs", quant_label = "iq3_xs",
            url = "{HF}/bartowski/Qwen2.5-1.5B-Instruct-GGUF/resolve/9eadc66189c7641e1ddd226b8267a9119b2ce2d4/Qwen2.5-1.5B-Instruct-IQ3_XS.gguf",
            bytes = 731699296l, sha256 = "9d5bd9c77d11161da7c8c4160913e1592f55c1f75604ede9b856ac7cfee896a6",
            parity <- ev_qwen_count([kq_native = true])),
        ModelSpec(file = "Qwen3-8B-BF16.gguf", display = "Qwen3-8B-bf16", quant_label = "bf16",
            parity <- ev_qwen_count()),
        ModelSpec(file = "Qwen3.5-4B-Q8_0.gguf", display = "Qwen3.5-4B",
            url = "{HF}/unsloth/Qwen3.5-4B-GGUF/resolve/e87f176479d0855a907a41277aca2f8ee7a09523/Qwen3.5-4B-Q8_0.gguf",
            bytes = 4482403488l, sha256 = "10cc391b403021dd11c614679d2fd92f611c3681d29e29651b717316965d61e1",
            parity <- ev_qwen_count([dn_chunked = true])),
        ModelSpec(file = "Qwen3.5-9B-Q8_0.gguf", display = "Qwen3.5-9B",
            parity <- ev_qwen_count([dn_chunked = true])),
        ModelSpec(file = "Qwen3.5-9B-IQ4_XS.gguf", display = "Qwen3.5-9B-iq4xs", quant_label = "iq4_xs",
            parity <- ev_qwen_count([kq_native = true, dn_chunked = true])),
        // the 27B at the size a 16 GB card holds whole: the official board's i-quant row
        ModelSpec(file = "Qwen3.8-27B-UD-IQ4_XS.gguf", display = "Qwen3.8-27B-iq4xs", quant_label = "iq4_xs", official = false,
            url = "{HF}/unsloth/Qwen3.8-27B-GGUF/resolve/27af057ecb382ddfea5d12837360a8980560e3ed/Qwen3.8-27B-UD-IQ4_XS.gguf",
            bytes = 14252845984l, sha256 = "40fac4050e940397dbf13087afd50f4734a11805bf9d65ef8ddd7483470e6199",
            parity <- ev_qwen_count([dn_chunked = true])),
        ModelSpec(file = "Qwen3.8-27B-UD-Q3_K_XL.gguf", display = "Qwen3.8-27B-q3kxl", quant_label = "q3_k_xl",
            parity <- ev_qwen_count([dn_chunked = true])),
        ModelSpec(file = "Qwen3.8-27B.i1-IQ3_S.gguf", display = "Qwen3.8-27B-iq3s", quant_label = "iq3_s",
            parity <- ev_qwen_count([dn_chunked = true])),
        ModelSpec(file = "Qwen1.5-MoE-A2.7B-Chat.Q6_K.gguf",
            url = "{FORMATS_HF}/Qwen1.5-MoE-A2.7B-Chat/Qwen1.5-MoE-A2.7B-Chat.Q6_K.gguf", bytes = 12765342432l, sha256 = "fb6e1a0807e03a1c368b3b2935541f24510d1078db4cb676aa84cb179302ff50", display = "Qwen1.5-MoE-q6k", quant_label = "q6_k",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3-30B-A3B-Q8_0.gguf", display = "Qwen3-30B-A3B-q8",
            parity <- ev_qwen_count([moe_reference = true])),
        // the nv4 MoE carrier (tests/test_metal_format_census.das, `census-moe-nv4`): the 768-wide routed stacks, the table and the classifier NVFP4 - every row on the 256 lattice; no `.scale`
        ModelSpec(file = "Qwen3-30B-A3B-NVFP4-pure-local.gguf",
            recipe = "llama-quantize --allow-requantize --pure --token-embedding-type nvfp4 --output-tensor-type nvfp4 Qwen3-30B-A3B-Q8_0.gguf Qwen3-30B-A3B-NVFP4-pure-local.gguf NVFP4"),
        ModelSpec(file = "Qwen3-30B-A3B-Instruct-2507-UD-Q3_K_XL.gguf", display = "Qwen3-30B-A3B-q3kxl", quant_label = "q3_k_xl",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3-30B-A3B-Thinking-2507-UD-Q3_K_XL.gguf", display = "Qwen3-30B-A3B-thinking-q3kxl", quant_label = "q3_k_xl",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3-Coder-30B-A3B-Instruct-UD-Q3_K_XL.gguf", display = "Qwen3-Coder-30B-q3kxl", quant_label = "q3_k_xl",
            parity <- ev_qwen_count([kq_native = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.5-35B-A3B-UD-IQ2_XXS.gguf", display = "Qwen3.5-35B-A3B-iq2xxs", quant_label = "iq2_xxs",
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.5-35B-A3B-UD-IQ3_S.gguf", display = "Qwen3.5-35B-A3B-iq3s", quant_label = "iq3_s",
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.6-35B-A3B-UD-IQ3_S.gguf", display = "Qwen3.6-35B-A3B-iq3s", quant_label = "iq3_s",
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        ModelSpec(file = "Qwen3.6-35B-A3B-Q8_0.gguf", display = "Qwen3.6-35B-A3B-q8",
            parity <- ev_qwen_count([dn_chunked = true, moe_reference = true])),
        // the Metal sweep's Llama-3.2-1B format matrix - every weight format on one small dense
        // model; the -local files are requants of the bartowski Q8_0, the rest bartowski's and
        // mradermacher's own; the counting fixture is the correctness gate, no board row
        ModelSpec(file = "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-IQ2_XS-local.gguf", bytes = 475865152l, sha256 = "9131b4dc6abd76dae44f6f3abc11c964104dd5e100763d5f462214305ae841fa", display = "Llama-3.2-1B-iq2xs", quant_label = "iq2_xs",
            recipe = "llama-quantize --allow-requantize --imatrix llama32_1b.imatrix Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-IQ2_XS-local.gguf IQ2_XS (the imatrix is bartowski's Llama-3.2-1B-Instruct.imatrix; a requant is size-exact but not byte-exact across boxes)",
            parity <- ev_llama_count_iq2xs()),
        ModelSpec(file = "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf", bytes = 447029312l, sha256 = "22e807efaeb22998e9396c80b100890f701dde4e3cb6b09488a55aa9bfb6194d", display = "Llama-3.2-1B-iq2xxs", quant_label = "iq2_xxs",
            recipe = "llama-quantize --allow-requantize --imatrix llama32_1b.imatrix Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf IQ2_XXS",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf", bytes = 567353408l, sha256 = "9348b95812e315963dce841de1cde5e34b175146b7b5cd3e5300ccfe0204e5cc", display = "Llama-3.2-1B-iq3xxs", quant_label = "iq3_xxs",
            recipe = "llama-quantize --allow-requantize --imatrix llama32_1b.imatrix --tensor-type attn_k=iq3_xxs --tensor-type attn_q=iq3_xxs Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf IQ3_XXS (llama.cpp's own ftype puts IQ2_S on attn_k/q)",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-IQ4_NL-local.gguf", bytes = 777220224l, sha256 = "1a26df14c9aa516216527c176c70b75014a7ffa5b4bf0c483384de66a77c6955", display = "Llama-3.2-1B-iq4nl", quant_label = "iq4_nl",
            recipe = "llama-quantize --allow-requantize Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-IQ4_NL-local.gguf IQ4_NL",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q2_K-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-Q2_K-local.gguf", bytes = 580874336l, sha256 = "78db9169ecb0b424ad7f6ef74d223114162ac60993b1f5ab971d064081c580b8", display = "Llama-3.2-1B-q2k", quant_label = "q2_k",
            recipe = "llama-quantize --allow-requantize Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-Q2_K-local.gguf Q2_K",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q4_0-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-Q4_0-local.gguf", bytes = 770928768l, sha256 = "7fc343ff2f052fe10802d39f38cf36c101735e6b38050fb93b71c28a917089ce", display = "Llama-3.2-1B-q40", quant_label = "q4_0",
            recipe = "llama-quantize --allow-requantize Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-Q4_0-local.gguf Q4_0",
            parity <- ev_llama_count([kq_native = true])),
        // the dense nv4 carrier, the CUDA home's vehicle and the Metal census's (tests/test_metal_format_census.das, `census-nv4`): every dense plane NVFP4 under a Q6_K table, no `.scale` (the ModelOpt files' BF16 k/v beside an nv4 q is a mixed feed the whole-model drivers decline); the NVFP4 target is the `nvfp4-quantize` branch of the local llama.cpp clone - upstream has the ftype, not its llama-quantize option row
        ModelSpec(file = "Llama-3.2-1B-Instruct-NVFP4-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-NVFP4-local.gguf", bytes = 770928768l, sha256 = "7d4ab81a16508beea3f9e9d3c0fadcf2b12631f8f0c87435bdb62942b755b6c6", display = "Llama-3.2-1B-nv4", quant_label = "nv4",
            recipe = "llama-quantize --allow-requantize Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-NVFP4-local.gguf NVFP4",
            parity <- ev_llama_count([kq_native = true])),
        // the nv4 table carrier (tests/test_metal_format_census.das, `census-nv4-table`): the same mint with the token table nv4, the tied classifier reading it
        ModelSpec(file = "Llama-3.2-1B-Instruct-emb-NVFP4-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-emb-NVFP4-local.gguf", bytes = 703209600l, sha256 = "3e2ae282f2cc61190bc2a6c3ddc8e3c8a40ea25c1677dce41cce8f2e8b4e06c1",
            recipe = "llama-quantize --allow-requantize --token-embedding-type nvfp4 Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-emb-NVFP4-local.gguf NVFP4"),
        // the 8B pair of the same mints: the FP4 leaf's larger dense carrier and its equal-bpw int8 twin
        ModelSpec(file = "Meta-Llama-3.1-8B-Instruct-NVFP4-local.gguf", display = "Llama-3.1-8B-nv4", quant_label = "nv4",
            recipe = "llama-quantize --allow-requantize Meta-Llama-3.1-8B-Instruct-Q8_0.gguf Meta-Llama-3.1-8B-Instruct-NVFP4-local.gguf NVFP4"),
        ModelSpec(file = "Meta-Llama-3.1-8B-Instruct-Q4_0-local.gguf", display = "Llama-3.1-8B-q40", quant_label = "q4_0",
            recipe = "llama-quantize --allow-requantize Meta-Llama-3.1-8B-Instruct-Q8_0.gguf Meta-Llama-3.1-8B-Instruct-Q4_0-local.gguf Q4_0"),
        // the dense q51 carrier: every dense plane on the per-32 Q5_1 planes
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-Q5_1-local.gguf", bytes = 953380992l, sha256 = "22b8a3554b495a82a5f4d4444cb7bbe17d5b115f13ad3476720935750f37eabf",
            recipe = "llama-quantize --allow-requantize Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-Q5_1-local.gguf Q5_1"),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q5_0-local.gguf",
            recipe = "llama-quantize --allow-requantize Llama-3.2-1B-Instruct-Q8_0.gguf Llama-3.2-1B-Instruct-Q5_0-local.gguf Q5_0"),
        ModelSpec(file = "Llama-3.2-1B-Instruct-IQ3_M.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-IQ3_M.gguf", bytes = 657289344l, sha256 = "c80cc062a721c267ec50fee83fe6b55d36fc7abe708392dbe22e16fbd42687e8", display = "Llama-3.2-1B-iq3m", quant_label = "iq3_m",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-IQ4_XS.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-IQ4_XS.gguf", bytes = 743141504l, sha256 = "69e85c871a13a99e4008c09916a957d170e4e5623a5b2e06be46408029a7afba", display = "Llama-3.2-1B-iq4xs", quant_label = "iq4_xs",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q3_K_L.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-Q3_K_L.gguf", bytes = 732524672l, sha256 = "4cc006968acfee52002f0d117d14cf2ed587e15c93a85c84fe8c053c5c47455d", display = "Llama-3.2-1B-q3kl", quant_label = "q3_k_l",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct-Q4_K_M.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct-Q4_K_M.gguf", bytes = 807694464l, sha256 = "a21c493830108cf938e064ef7262a0a78803e3dccb0f0626b437f334ce662279", display = "Llama-3.2-1B-q4km", quant_label = "q4_k_m",
            parity <- ev_llama_count([kq_native = true])),
        ModelSpec(file = "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf",
            url = "{FORMATS_HF}/Llama-3.2-1B-Instruct/Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf", bytes = 562111008l, sha256 = "ffd226e2233c9785fdfb7cc4e91976809f7ae8b0b128ac3da9fb36a12e8c6b93", display = "Llama-3.2-1B-i1-iq3xxs", quant_label = "iq3_xxs",
            parity <- ev_llama_count([kq_native = true])),
        // the Metal sweep's gemma format rows - the files the board does not show; the counting
        // fixture is the correctness gate, no board row (the two lowercase-b 12B files match no
        // current upload by size, so they carry no url)
        ModelSpec(file = "gemma-2-2b-it-Q4_K_M.gguf", display = "gemma-2-2B-q4k", quant_label = "q4_k_m",
            url = "{HF}/bartowski/gemma-2-2b-it-GGUF/resolve/855f67caed130e1befc571b52bd181be2e858883/gemma-2-2b-it-Q4_K_M.gguf",
            bytes = 1708582752l, sha256 = "e0aee85060f168f0f2d8473d7ea41ce2f3230c1bc1374847505ea599288a7787",
            parity <- ev_gemma2_count([kq_native = true])),
        ModelSpec(file = "gemma-4-12b-it-Q5_K_M.gguf", display = "gemma-4-12B-q5k", quant_label = "q5_k_m",
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "gemma-4-12b-it-Q6_K.gguf", display = "gemma-4-12B-q6k", quant_label = "q6_k",
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "gemma-4-E2B-it-Q4_0.gguf", display = "gemma-4-E2B-q40", quant_label = "q4_0",
            url = "{HF}/ggml-org/gemma-4-E2B-it-GGUF/resolve/main/gemma-4-E2B-it-Q4_0.gguf",
            bytes = 2841481184l, sha256 = "8e30dff3ac4c8434c49a7036fa15564bdbb6044e42bf04550bf1a096ad7e6a52",
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "gemma-4-26B-A4B-it-Q4_0.gguf", display = "gemma-4-26B-A4B-q40-ggml", quant_label = "q4_0",
            url = "{HF}/ggml-org/gemma-4-26B-A4B-it-GGUF/resolve/main/gemma-4-26B-A4B-it-Q4_0.gguf",
            bytes = 14618145824l, sha256 = "d208665ab1cd3a69f7a9a4bc59430e8448c8093d9b06334f566ac59d6d504a03",
            parity <- ev_gemma_count([kq_native = true])),
        ModelSpec(file = "gemma-4-26B-A4B-it-UD-IQ4_XS.gguf", display = "gemma-4-26B-A4B-iq4xs", quant_label = "iq4_xs",
            url = "{HF}/unsloth/gemma-4-26B-A4B-it-GGUF/resolve/main/gemma-4-26B-A4B-it-UD-IQ4_XS.gguf",
            bytes = 13597177568l, sha256 = "babd1e389d386352f71600765d37390f7dc993fbfad6725caccf996ffe34aecf",
            parity <- ev_gemma_count([kq_native = true])),
        // the text-to-speech carriers: local conversions with no board row (followup_general.md
        // row 77); the English front-end packs ride the kitten-nano row, the Russian stress pack
        // the kokoro-ru row, the per-model oracle dumps their own rows
        ModelSpec(file = "kitten-nano.gguf", recipe = TTS_KITTEN_RECIPE, serve_tts = true,
            tts = TtsBoard(display = "KittenTTS nano", voice = "Bella", ref = "kitten-nano", package = "kittentts"),
            url = "{TTS_HF}/kitten-nano.gguf", bytes = 59331456l,
            sha256 = "4556948c36a29e4be5ad521e597a20ae817059c404fea3c5d935afa73506d9da",
            companions <- [
                ProvEntry(name = "tts_g2p.bin", root = "llm", recipe = TTS_PACKS_RECIPE,
                    url = "{TTS_HF}/tts_g2p.bin", bytes = 14011554l,
                    sha256 = "d7b6afea7a0901a877d10531054d3f967845c58f415f0ea937decd8a3081ccd5"),
                ProvEntry(name = "tts_g2p_en_us.bin", root = "llm", recipe = TTS_PACKS_RECIPE,
                    url = "{TTS_HF}/tts_g2p_en_us.bin", bytes = 10257455l,
                    sha256 = "6f69d2e74565bd7d876b8d1f4042bf8c1c5b615387fa26ff45215cf447932154"),
                ProvEntry(name = "tts_postag.bin", root = "llm", recipe = TTS_PACKS_RECIPE,
                    url = "{TTS_HF}/tts_postag.bin", bytes = 12566510l,
                    sha256 = "38c2e85f7fef3e57d561d2aa0af25fccda4276376ba1993c3dbc2ae0ebfa57b4"),
                ProvEntry(name = "tts_oracle/kitten_nano", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "kitten-mini.gguf", recipe = TTS_KITTEN_RECIPE, serve_tts = true,
            tts = TtsBoard(display = "KittenTTS mini", voice = "Bella", ref = "kitten-mini", package = "kittentts"),
            url = "{TTS_HF}/kitten-mini.gguf", bytes = 295975008l,
            sha256 = "e127a95c6ffa390eccc5956e7156ff055e9f30078cf16361cbcd0eea6c21fdb5",
            companions <- [
                ProvEntry(name = "tts_oracle/kitten_mini", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "kokoro-82m.gguf", recipe = TTS_KOKORO_RECIPE, serve_tts = true,
            tts = TtsBoard(display = "Kokoro-82M", voice = "af_heart", ref = "kokoro-82m", package = "kokoro", torch_gpu = true),
            url = "{TTS_HF}/kokoro-82m.gguf", bytes = 352965024l,
            sha256 = "e0d6584b5b650730bd62e7b105a2e9502384bbe3fbebf3030113560355e6232f",
            companions <- [
                ProvEntry(name = "tts_oracle/kokoro", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        // the Russian fine-tune: the base checkpoint's two voices, its stress pack beside it; the English
        // packs its facade cell reads for a Latin word are named without their pins, which ride the kitten-nano row
        ModelSpec(file = "kokoro-ru.gguf", recipe = TTS_KOKORO_RU_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kokoro-ru.gguf", bytes = 325804992l,
            sha256 = "635a43debfcbff5cf1fd0380a4ffc55dfacff083ae16d057bae6d7e7c9a29c9f",
            companions <- [
                ProvEntry(name = "tts_g2p_ru.bin", root = "llm", recipe = TTS_PACKS_RU_RECIPE,
                    url = "{TTS_HF}/tts_g2p_ru.bin", bytes = 15387767l,
                    sha256 = "74b8f3c4d3a3e140b7c7937d15da95a37328e7e3347392088514e47e68e5265d"),
                ProvEntry(name = "tts_g2p.bin", root = "llm", recipe = TTS_PACKS_RECIPE),
                ProvEntry(name = "tts_postag.bin", root = "llm", recipe = TTS_PACKS_RECIPE),
                ProvEntry(name = "tts_ctx_ru.gguf", root = "llm", recipe = TTS_RU_STRESS_Q4_RECIPE)]),
        ModelSpec(file = "kokoro-ru-dima.gguf", recipe = TTS_KOKORO_RU_RECIPE,
            url = "{TTS_HF}/kokoro-ru-dima.gguf", bytes = 325282688l,
            sha256 = "7720e1087c0c9720577e7e0b383dd31f75b7f6ab45bcd88df1f28df3c08c56a6"),
        // the five files above as the files that store a lane's weights: -q8 the Q8_0 lane's, -kq the recipe lane's; each reads the
        // packs its f32 row names
        ModelSpec(file = "kitten-nano-q8.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kitten-nano-q8.gguf", bytes = 16646016l,
            sha256 = "ed1b412ccc10499d2f4bc4fe4f4c78c7308dfb772c311782989f41ed8aee9dbe",
            companions <- [
                ProvEntry(name = "tts_g2p.bin", root = "llm", recipe = TTS_PACKS_RECIPE),
                ProvEntry(name = "tts_postag.bin", root = "llm", recipe = TTS_PACKS_RECIPE)]),
        ModelSpec(file = "kitten-nano-kq.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kitten-nano-kq.gguf", bytes = 14785920l,
            sha256 = "d92014bdfa7cc7bafb3093f3e755c0fdc76a974e03c363b1b6c42b610860326e",
            companions <- [
                ProvEntry(name = "tts_g2p.bin", root = "llm", recipe = TTS_PACKS_RECIPE),
                ProvEntry(name = "tts_postag.bin", root = "llm", recipe = TTS_PACKS_RECIPE)]),
        ModelSpec(file = "kitten-mini-q8.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kitten-mini-q8.gguf", bytes = 80495296l,
            sha256 = "1e1d094ee91aaf28a33ec4a1617a1acdd7d3a35fba7a681d2d99e022532701ed"),
        ModelSpec(file = "kitten-mini-kq.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kitten-mini-kq.gguf", bytes = 59725504l,
            sha256 = "f90e4f5aaac96cbd887fce4b9eeb3d05bd39a34a9f80c188a32a8902e6475d46"),
        ModelSpec(file = "kokoro-82m-q8.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kokoro-82m-q8.gguf", bytes = 95792544l,
            sha256 = "cb1ade7573957ed00bffe4968f1cc0ddec4222dd28d654636b93b5ea406a6d20"),
        ModelSpec(file = "kokoro-82m-kq.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kokoro-82m-kq.gguf", bytes = 71391648l,
            sha256 = "4b5102ab80b568ed6d9dc23417d7864fb23435b6f744729146a8fd1977b5bc4f"),
        ModelSpec(file = "kokoro-ru-q8.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kokoro-ru-q8.gguf", bytes = 88575552l,
            sha256 = "1961e4ca0cfde51c6e57244e19c45163c3c7e67808d94387cee942849789e5ff"),
        ModelSpec(file = "kokoro-ru-kq.gguf", recipe = TTS_STORED_RECIPE, serve_tts = true,
            url = "{TTS_HF}/kokoro-ru-kq.gguf", bytes = 64174656l,
            sha256 = "b3a238b377514b11fa778ba7b8003f83c460f3a427a5fea54525c2064d725bdb"),
        ModelSpec(file = "kokoro-ru-dima-q8.gguf", recipe = TTS_STORED_RECIPE,
            url = "{TTS_HF}/kokoro-ru-dima-q8.gguf", bytes = 88436768l,
            sha256 = "17ae7fb6698d66011facffc7e9a3e4b7db84d17542d4b9523967417545fa5d2a"),
        ModelSpec(file = "kokoro-ru-dima-kq.gguf", recipe = TTS_STORED_RECIPE,
            url = "{TTS_HF}/kokoro-ru-dima-kq.gguf", bytes = 64035872l,
            sha256 = "b396a6c2dfe4461b7127eb89aade5007654f39606e31cb56a31fa489870d978c"),
        ModelSpec(file = "ru-stress-rubert-base.gguf", recipe = TTS_RU_STRESS_SEEDED_RECIPE, bytes = 714155040l,
            sha256 = "dd74321523a6e331ead017f891f3ee258e43fcf61af5d4c2fa05bffe5948bd2b",
            companions <- [
                ProvEntry(name = "tts_oracle/ru_stress", root = "llm", recipe = TTS_RU_STRESS_ORACLE_RECIPE)]),
        ModelSpec(file = "ru-stress-rubert-base-q4.gguf", recipe = TTS_RU_STRESS_Q4_RECIPE,
            url = "{TTS_HF}/ru-stress-rubert-base-q4.gguf", bytes = 120522080l,
            sha256 = "28692fe9fbea9cb7b06f6c39e18a78e4c1e30b37173f2040808865a1d7730624",
            companions <- [
                ProvEntry(name = "ru-stress-rubert-base-f32.gguf", root = "llm", recipe = TTS_RU_STRESS_F32_RECIPE,
                    bytes = 714181728l, sha256 = "c621439f4b661bb2dd4f776ec67dbdc5b05007401007119f262294b984937ce0")]),
        ModelSpec(file = "ru-stress-ruelectra-small-q4.gguf", recipe = TTS_RU_STRESS_Q4_RECIPE,
            url = "{TTS_HF}/ru-stress-ruelectra-small-q4.gguf", bytes = 38428800l,
            sha256 = "43475b1535fdcd3d142bb04ac845909f2557bee32b524af5e19eb1528bbc1ae3"),
        // the Pocket TTS family reads text, so it needs no pack; the published form is the Q8_0 file,
        // the f16 twin (pocket-tts-en.gguf, the parity rail's reference) stays local
        ModelSpec(file = "pocket-tts-en-q8.gguf", recipe = TTS_POCKET_RECIPE, serve_tts = true,
            tts = TtsBoard(display = "Pocket TTS en q8", voice = "alba", ref = "pocket-en", package = "pocket-tts"),
            url = "{TTS_HF}/pocket-tts-en-q8.gguf", bytes = 136812032l,
            sha256 = "ff41c88d0099375e096c1a0bb9db43c10e8b53b8bc9d3874b03f658f94a5487e",
            companions <- [
                ProvEntry(name = "pocket-tts-en.gguf", root = "llm", recipe = TTS_POCKET_F16_RECIPE),
                ProvEntry(name = "tts_oracle/pocket_english_2026-04", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-de-q8.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-de-q8.gguf", bytes = 136519648l,
            sha256 = "1762eb997ab113247a5083d55dfd22bf5ee83d8256016fce13a67ba4f3fb6fd9",
            companions <- [
                ProvEntry(name = "tts_oracle/pocket_german", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-es-q8.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-es-q8.gguf", bytes = 136518688l,
            sha256 = "65ab208f0ecc976e752889bd3a1c1f61b51a95515e1159fde8468177ba083128",
            companions <- [
                ProvEntry(name = "tts_oracle/pocket_spanish", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-it-q8.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-it-q8.gguf", bytes = 136515072l,
            sha256 = "65507c2ff6c87836bca5ea50e66e55a12ecec78390556efae75eb35b4c44f754",
            companions <- [
                ProvEntry(name = "tts_oracle/pocket_italian", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-pt-q8.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-pt-q8.gguf", bytes = 136521024l,
            sha256 = "80ae1a859658149843de6701fc4b86868c34925b55490c21429c497d333201b6",
            companions <- [
                ProvEntry(name = "tts_oracle/pocket_portuguese", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-fr-q8.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-fr-q8.gguf", bytes = 377328544l,
            sha256 = "edb510a0c441f50817b8aed8cfec961b77bd9ee12691c9db9f91427a966cb738",
            companions <- [
                ProvEntry(name = "tts_oracle/pocket_french_24l", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        // the five languages' small forms: the K-quant twin of each file above, its oracle the q8 row's
        ModelSpec(file = "pocket-tts-de-kq.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-de-kq.gguf", bytes = 77466176l,
            sha256 = "29d3088f3db5aa9958b589e60da023b6c89a80a341986eb43cbe7aa29bbd7432"),
        ModelSpec(file = "pocket-tts-es-kq.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-es-kq.gguf", bytes = 77465216l,
            sha256 = "23b3a52cc77d4efaf291f46507a5e0412896f87e1f71fd0d4afa3c4d2e9d0008"),
        ModelSpec(file = "pocket-tts-it-kq.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-it-kq.gguf", bytes = 77461600l,
            sha256 = "dfe62eccdf774c660a8c11dc7da1778a3e1f986c245f4d37b3bedd4c7f21c309"),
        ModelSpec(file = "pocket-tts-pt-kq.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-pt-kq.gguf", bytes = 77467552l,
            sha256 = "0c25e851662d09fb782f1eac11caf38e7b00f1f19f307577700c0b8c2014decd"),
        ModelSpec(file = "pocket-tts-fr-kq.gguf", recipe = TTS_POCKET_LANG_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-fr-kq.gguf", bytes = 205028864l,
            sha256 = "f56ac84a47687d517a0a4e1776532096503567d09250b090c50c9a74d0c55cb8"),
        // the small English forms behind the browser examples: the parrot page fetches the cloning one,
        // the storywish page the one-voice one (tests/test_tts_pocket.das's kq cells hold both against the q8 file);
        // their 18m twins below are published for the asking - the pages keep the 100M model's pace
        ModelSpec(file = "pocket-tts-en-kq.gguf", recipe = TTS_POCKET_KQ_RECIPE, serve_tts = true,
            tts = TtsBoard(display = "Pocket TTS en kq", voice = "alba", ref = "pocket-en", package = "pocket-tts"),
            url = "{TTS_HF}/pocket-tts-en-kq.gguf", bytes = 77758560l,
            sha256 = "9b549549e44601435ec5df3f48b5ef977cf1449bcf92e6ae49db4b07da20e757"),
        ModelSpec(file = "pocket-tts-en-stuart-kq.gguf", recipe = TTS_POCKET_STUART_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-stuart-kq.gguf", bytes = 67005056l,
            sha256 = "e9e69e52254f48e544509a0dff3b594779f737eac5b372d61adda8b696a5af46"),
        // the 2026-10 small English models: one q8 (or f16) file each and the 18m's kq form; the f32 references stay local, as companions
        ModelSpec(file = "pocket-tts-en-18m-q8.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            tts = TtsBoard(display = "Pocket TTS en 18m q8", voice = "alba", ref = "pocket-en-18m", package = "pocket-tts"),
            url = "{TTS_HF}/pocket-tts-en-18m-q8.gguf", bytes = 36712480l,
            sha256 = "d5b138be971d7907eb7a568b827510d85775d7ce933fad4dba0245d2d0536fa2",
            companions <- [
                ProvEntry(name = "pocket-tts-en-18m.gguf", root = "llm", recipe = TTS_POCKET_SMALL_F32_RECIPE),
                ProvEntry(name = "tts_oracle/pocket_english_18m_2026-10", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-en-18m-kq.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-18m-kq.gguf", bytes = 23547232l,
            sha256 = "ed8db0741c377ea21a3603101d9215b538e2c143b1916f459e1b599ce096716b"),
        ModelSpec(file = "pocket-tts-en-18m-stuart-kq.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-18m-stuart-kq.gguf", bytes = 12793728l,
            sha256 = "3678b27db407fa0715feb7c0863c63c1cb00a28f9387fa76f8e4922e30c8f124"),
        ModelSpec(file = "pocket-tts-en-7m-q8.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-7m-q8.gguf", bytes = 22991840l,
            sha256 = "b3d99bc412613c4323f023572c782b980a375fed83d88239b7e5d68de2621c4c",
            companions <- [
                ProvEntry(name = "pocket-tts-en-7m.gguf", root = "llm", recipe = TTS_POCKET_SMALL_F32_RECIPE),
                ProvEntry(name = "tts_oracle/pocket_english_7m_2026-10", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-en-2m.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-2m.gguf", bytes = 23758400l,
            sha256 = "612a583ce4811f8904200295fa9293423074cffe4eadbcc3a5232aec1be5f68d",
            companions <- [
                ProvEntry(name = "pocket-tts-en-2m-f32.gguf", root = "llm", recipe = TTS_POCKET_SMALL_F32_RECIPE),
                ProvEntry(name = "tts_oracle/pocket_english_2m_2026-10", root = "llm", recipe = TTS_ORACLE_RECIPE)]),
        ModelSpec(file = "pocket-tts-en-18m-refl-q8.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-18m-refl-q8.gguf", bytes = 36712480l,
            sha256 = "3460c9cff8102e997369ab3ee7e32e1d020a8f1cffc4a454dc1ecb059f51d4dc"),
        ModelSpec(file = "pocket-tts-en-7m-refl-q8.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-7m-refl-q8.gguf", bytes = 22991840l,
            sha256 = "80ca90b4a60716693722ed19a91945d326f4611f8573defe27f722d6f4bd50a2"),
        ModelSpec(file = "pocket-tts-en-2m-refl.gguf", recipe = TTS_POCKET_SMALL_RECIPE, serve_tts = true,
            url = "{TTS_HF}/pocket-tts-en-2m-refl.gguf", bytes = 23758432l,
            sha256 = "54dead7cb04aa87971b32f4a25288d566c95c739290581df91742155fcc61230"),
        // the translation family: the five Bergamot students the suite holds to marian's graph (tests/test_nmt.das), each with
        // its vocabulary and its oracle dumps as companions, and the translation LLM the facade's completion form serves
        ModelSpec(file = "bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin", recipe = BERGAMOT_RECIPE,
            url = "{BERGAMOT_BUCKET}/models/ru-en/next26_RSpwfHrcSUC_rKTnhOEXzw/exported/model.ruen.intgemm.alphas.bin.gz",
            bytes = 31561787l, sha256 = "aa0efa194b3dbd2da129ba0b8db8ef139126cb0914e57072f62061294746f1e2",
            companions <- [
                ProvEntry(name = "bergamot/ru-en-base-memory/vocab.ruen.spm", root = "llm", recipe = BERGAMOT_RECIPE,
                    url = "{BERGAMOT_BUCKET}/models/ru-en/next26_RSpwfHrcSUC_rKTnhOEXzw/exported/vocab.ruen.spm.gz",
                    bytes = 952371l, sha256 = "de8af5f87e231ee4016cec4bb482e6b584cb1ca34dcb624fc7b8398f25bb35df"),
                ProvEntry(name = "nmt_oracle/ru-en-base-memory", root = "llm", recipe = NMT_ORACLE_RECIPE)]),
        ModelSpec(file = "bergamot/en-ru-base-memory/model.enru.intgemm.alphas.bin", recipe = BERGAMOT_RECIPE,
            url = "{BERGAMOT_BUCKET}/models/en-ru/retrain_base-memory_KJ23-iDVTcymG1ZldWY17w/exported/model.enru.intgemm.alphas.bin.gz",
            bytes = 31561787l, sha256 = "184cb5cda528eeefc0f75f5d0035d787b71d74af135e3c5608d01ae02ecfb920",
            companions <- [
                ProvEntry(name = "bergamot/en-ru-base-memory/vocab.enru.spm", root = "llm", recipe = BERGAMOT_RECIPE,
                    url = "{BERGAMOT_BUCKET}/models/en-ru/retrain_base-memory_KJ23-iDVTcymG1ZldWY17w/exported/vocab.enru.spm.gz",
                    bytes = 904455l, sha256 = "56ee63e14e8cb926c394242adc3ed7cc602644c3d33058cff2ce2959d52a6258"),
                ProvEntry(name = "nmt_oracle/en-ru-base-memory", root = "llm", recipe = NMT_ORACLE_RECIPE)]),
        ModelSpec(file = "bergamot/en-ru-base/model.enru.intgemm.alphas.bin", recipe = BERGAMOT_RECIPE,
            url = "{BERGAMOT_BUCKET}/models/en-ru/student_base_AYqN3ysXRp2EGkEqeaA5Rg/exported/model.enru.intgemm.alphas.bin.gz",
            bytes = 42992955l, sha256 = "0ef9a209c5edc46692750e7505b3695655b1c7c3ec73058b641201ef18c481ce",
            companions <- [
                ProvEntry(name = "bergamot/en-ru-base/vocab.enru.spm", root = "llm", recipe = BERGAMOT_RECIPE,
                    url = "{BERGAMOT_BUCKET}/models/en-ru/student_base_AYqN3ysXRp2EGkEqeaA5Rg/exported/vocab.enru.spm.gz",
                    bytes = 904455l, sha256 = "56ee63e14e8cb926c394242adc3ed7cc602644c3d33058cff2ce2959d52a6258"),
                ProvEntry(name = "nmt_oracle/en-ru-base", root = "llm", recipe = NMT_ORACLE_RECIPE)]),
        ModelSpec(file = "bergamot/ru-en-tiny/model.ruen.intgemm.alphas.bin", recipe = BERGAMOT_RECIPE,
            url = "{BERGAMOT_BUCKET}/models/ru-en/spring-2024_QrcdYgbwS7e7xbhtOSdoNQ/exported/model.ruen.intgemm.alphas.bin.gz",
            bytes = 17141051l, sha256 = "b1d85c13cfbb05e1d326dd6f0fb5ef270a2011b547450260f96567a93f446c94",
            companions <- [
                ProvEntry(name = "bergamot/ru-en-tiny/vocab.ruen.spm", root = "llm", recipe = BERGAMOT_RECIPE,
                    url = "{BERGAMOT_BUCKET}/models/ru-en/spring-2024_QrcdYgbwS7e7xbhtOSdoNQ/exported/vocab.ruen.spm.gz",
                    bytes = 905257l, sha256 = "93bdc941b16e523695c319f74778bca9fd8b75a25ad75020cdc98aef74cdc0fc"),
                ProvEntry(name = "nmt_oracle/ru-en-tiny", root = "llm", recipe = NMT_ORACLE_RECIPE)]),
        ModelSpec(file = "bergamot/en-de-base-memory/model.ende.intgemm.alphas.bin", recipe = BERGAMOT_RECIPE,
            url = "{BERGAMOT_BUCKET}/models/en-de/retrain_hr_fix_names_SCgGhxUPQ2WAECHLRtzrMg/exported/model.ende.intgemm.alphas.bin.gz",
            bytes = 31561787l, sha256 = "8df29d9494d19f47fd5d97c6a73474c6f657e9f81c1a607c431d02befdf3810f",
            companions <- [
                ProvEntry(name = "bergamot/en-de-base-memory/vocab.ende.spm", root = "llm", recipe = BERGAMOT_RECIPE,
                    url = "{BERGAMOT_BUCKET}/models/en-de/retrain_hr_fix_names_SCgGhxUPQ2WAECHLRtzrMg/exported/vocab.ende.spm.gz",
                    bytes = 810073l, sha256 = "69f730becafa48e3bb2c244eab66456877c08959a02f2bd5519b5a3088b62f9c"),
                ProvEntry(name = "nmt_oracle/en-de-base-memory", root = "llm", recipe = NMT_ORACLE_RECIPE)]),
        ModelSpec(file = "milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf", recipe = MILMMT_RECIPE,
            url = "https://huggingface.co/mradermacher/MiLMMT-46-1B-v0.1-GGUF/resolve/b1df0ed3a68176ac11281077fdf74a8c17e4938f/MiLMMT-46-1B-v0.1.Q4_K_M.gguf",
            bytes = 1013675392l, sha256 = "9d5c10855eb2688d453e3069e7b6dee1756fc834d738d2dc04318511993fd54f")
    ]
}

//! The text-to-speech set the serving catalog offers: the served GGUFs (the three phoneme
//! families and the Russian fine-tune, the six Pocket languages and the small English forms) and the
//! four front-end packs (the full phoneme pack, its American-only twin, the tagger, the Russian
//! stress pack), as pinned on the borisbat/dasllama-tts repository - a view over the rows above
//! (a pack rides its carrier row's companions with a url; a companion named without one is left out).
//! Every entry carries url + bytes + sha256; `pack` tells the packs apart from the models.
def serve_tts_set() : array<tuple<entry : ProvEntry; pack : bool>> {
    var out : array<tuple<entry : ProvEntry; pack : bool>>
    var all <- model_specs()
    for (s in all) {
        continue if (empty(s.url) || !s.serve_tts)
        out |> push((entry = ProvEntry(name = s.file, root = "llm", url = s.url, bytes = s.bytes, sha256 = s.sha256, recipe = s.recipe), pack = false))
        for (c in s.companions) {
            if (!empty(c.url)) {
                out |> push((entry = c, pack = true))
            }
        }
    }
    delete all
    return <- out
}

//! The serving ASR tower: the hosted parakeet v3 bin, byte-identical to the canonical
//! conversion and commit-pinned — the one entry dasllama-server's dictation auto-offer
//! downloads. profile_common's asr_catalog provenance consumes this same row (single source).
def serve_asr_tower() : ProvEntry {
    return ProvEntry(name = "ggml-parakeet-tdt-0.6b-v3-f32.bin", root = "whisper",
        url = "{HF}/ggml-org/parakeet-GGUF/resolve/35156454d1a39de06863303dd209fd2bed6ee079/ggml-parakeet-tdt-0.6b-v3-f32.bin",
        bytes = 2508463079l, sha256 = "b17956ca991a7f084c978a8a9d4c9785864260b6dd9b72f884344759d913ae7f")
}

//! The pinned companion behind a serving row's `serve_vision` or `serve_drafter` name, resolved
//! across every row's companions (a tower is size-invariant and a drafter rides several quants of
//! its trunk, so the pinning row and the serving row may differ). Empty `name` back = no pinned
//! match — the companion reads as absent.
def serve_companion(name : string) : ProvEntry {
    var all <- model_specs()
    var out <- ProvEntry()
    if (!empty(name)) {
        for (s in all) {
            for (c in s.companions) {
                if (c.name == name && !empty(c.url) && !empty(c.sha256)) {
                    out <- c
                }
            }
        }
    }
    delete all
    return <- out
}

// the rows of [[model_specs]] the predicate keeps
def private spec_view(keep : block<(s : ModelSpec) : bool>) : array<ModelSpec> {
    var all <- model_specs()
    var out : array<ModelSpec>
    for (s in all) {
        if (invoke(keep, s)) {
            out |> emplace(s)
        }
    }
    delete all
    return <- out
}

//! The profiled text-LLM view over [[model_specs]] — the rows gen_profile / the baseline
//! establisher measure (a spec with a scoreboard display name).
def llm_catalog() : array<ModelSpec> =>
    <- spec_view() $(s : ModelSpec) : bool { return !empty(s.display); }

//! The public-board view over [[model_specs]] — the rows gen_bench_records sweeps: community-
//! popular GGUFs, one popular quant each, NOT the internal quant matrix (curated to what
//! people run THIS year). Absent files skip with a warning, so one list serves every box.
def official_catalog() : array<ModelSpec> =>
    <- spec_view() $(s : ModelSpec) : bool { return s.official; }

//! Every row of [[model_specs]] - the sweep view: gen_bench_records `--catalog all` visits each
//! stocked file, board row or not (absent files skip with a warning like the board view's).
def full_catalog() : array<ModelSpec> =>
    <- spec_view() $(s : ModelSpec) : bool { return !empty(s.file); }

//! The spec's vision mmproj companion basename ("" = no vision pairing) — the image-chat
//! cell's second file. A row with one is a vision decoder; the driver pairs it with
//! [[bench_image_fixture]] for the board's image cells.
def mmproj_companion(s : ModelSpec) : string {
    for (c in s.companions) {
        if (c.name |> starts_with("mmproj-")) {
            return c.name
        }
    }
    return ""
}

//! The one pinned picture every image-chat cell measures (provenanced as a companion of the
//! spec row that owns it) — cells across models and boxes stay comparable because the input
//! never varies.
def bench_image_fixture() : string => "coco-val2017-000000039769.jpg"

//! The spec row keyed `file` - its models-dir path or its base name (a split-shard row sits in a subfolder, the
//! pregate holds the base name); an empty `file` back = no such spec. A fresh copy, never a reference into the table.
def spec_by_file(file : string) : ModelSpec {
    var all <- model_specs()
    var out <- ModelSpec()
    for (s in all) {
        if (s.file == file || base_name(s.file) == file) {
            out <- s
            break
        }
    }
    delete all
    return <- out
}
