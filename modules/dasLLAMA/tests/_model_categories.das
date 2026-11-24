options gen2
options _dasllama_internal = true

module _model_categories shared public

require daslib/fio
require daslib/strings_boost
require strings
require ../../../utils/common/text_lines.das   // split_trimmed: the comma lists

//! the `--only` tokens of the CUDA home's pass: under the CUDA ask a
//! file outside them runs the Vulkan home's tests on a home that
//! serves the dense slice alone and reds on its declines. The fleet's
//! CUDA pod and `tests/CLAUDE.md`'s command read this list, and
//! `test_run_suites.das` holds every CUDA-reaching file to a token;
//! the last line is the media towers' files, whose twins seat on the
//! CUDA tower under the ask
let CUDA_PASS_ONLY = ("cuda,regions,resident_moe,embeddings,gptoss,gemma4_dense,"
    + "gemma4_moe,gemma2_3,gemma4_e2b,mtp_gemma_drafter,resident_hybrid,resident_qwen2,"
    + "resident_qwen3,resident_qwen_moe,resident_format_census,resident_llama,"
    + "audio,whisper,gigaam,rnnt,vad,gemma4uv,gemma4v,gemma3v,qwen3v,qwen25v,vision_chat,"
    + "dflash_drafter")

//! the `--only` tokens of the model-free suite's files that reach the Vulkan home: on a two-pod fleet the CUDA home runs
//! the suite without the Vulkan module (`DASLLAMA_DISABLE_MODULES=dasVulkan`), so the Vulkan home runs these files.
//! `test_run_suites.das` holds every model-free file reaching Vulkan to a token
let VULKAN_FREE_ONLY = ("vulkan,fat_start,gen_records_args,gpu_serving_declines,gpu_tier,"
    + "kernel_backend,kqformat,rnnt,whisper_chain")

//! the `--exclude` tokens of the CUDA-only test files: each requires a CUDA kernel-class module
//! (`dasllama_cuda_classes*`) with no `?cuda` guard, itself or through a fixture of this folder, so
//! its cells are the CUDA home's alone. A two-pod fleet's Vulkan home, which leaves the CUDA module
//! out, runs none of them; `test_run_suites.das` holds the tokens to that set
let CUDA_ONLY_EXCLUDE = "test_cuda_kernels,test_cuda_tower"

//! A model category: model files no run opens unless the run names the category. `stems` is a comma
//! list - a file is the category's when its name starts with one of them; `opens` a comma list of
//! stems of files no category holds - the towers its decoders pair with, the model whose test its
//! cells nest in - that a run naming the category opens too, as every run does; `topics` is what
//! only these files reach. Every model file past `MODEL_TIER_CEILING_BYTES` is a category's
typedef ModelCategory = tuple<name : string; stems : string; opens : string; topics : string>

//! every category; `../TEST_CATEGORIES.md` carries the same rows for a reader, and `REVIEW.das` holds the two together
def model_categories() : array<ModelCategory> {
    return <- [
        (name = "gemma4-e4b", stems = "gemma-4-E4B,mmproj-gemma-4-E4B", opens = "",
            topics = "the gemma-4 audio embedder widening 1536 to a 2560 decoder, the gemma-4 vision projector at 2560 with no std bias"),
        (name = "gemma4-dense", stems = "gemma-4-12B-it,gemma-4-12b-it,gemma-3-4b",
            opens = "mmproj-gemma-4-12B,mmproj-gemma-3-4b",
            topics = "the dense gemma-4 decoder at its forty-eight blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain, the Metal counting generates and batched decode token for token against the CPU truth on the K-quant and Q8_0 planes, the image chat on the 12B; the gemma-3 dense base at the 4B's wider geometry, and the image chat on it"),
        (name = "gemma4-31b", stems = "gemma-4-31B,mmproj-gemma-4-31B", opens = "",
            topics = "the Metal residual step past its 4096-float slab, on a decode of the 31B's 5376-wide rows"),
        (name = "gptoss-20b", stems = "gpt-oss-20b", opens = "",
            topics = "the gpt-oss decoder at its twenty-four blocks: the frozen greedy fixtures against the reference, short and with the sliding window engaged; a cold planar mint streaming the native-MXFP4 expert planes under their bytes"),
        (name = "gemma4-moe", stems = "gemma-4-26B-A4B-it,gemma-4-26B_q4_0,mtp-gemma-4-26B,mmproj-gemma-4-26B", opens = "",
            topics = "the routed gemma-4 decoder at its thirty blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain on the IQ, K-quant and MXFP4 expert stacks and with routed layers' experts on the host under a pinned plan room, the IQ4_XS stack only the last block carries, a cold planar mint streaming the native-MXFP4 expert planes, the QAT Q4_0 file's repacked expert planes, the NextN draft head, the DFlash block drafter's rounds on the routed trunk"),
        (name = "qwen35-4b", stems = "Qwen3.5-4B", opens = "",
            topics = "the deltanet hybrid at the 4B's 2560-wide rows over thirty-two layers: the CUDA driver's forced feed against the CPU chain, the Metal MTP rail's verify rows against the plain GPU step"),
        (name = "qwen35-9b", stems = "Qwen3.5-9B", opens = "",
            topics = "the deltanet hybrid on K-quant recurrent projections: the N-row recurrent form's Q8_K feed at every recurrent head, batched and past the unsplit position, its K-quant draft head through the pool's moves, the Vulkan census's gated 256-key decode attention, the Metal MTP rail's verify rows"),
        (name = "qwen35-27b", stems = "Qwen3.6-27B,Qwen3.8-27B-Q,Qwen3.8-27B-UD,Qwen3.8-27B.i1,Qwen3.8-27B-4bit,Qwen3.8-27B-DFlash2,mtp-Qwen3.6-27B,mtp-Qwen3.8-27B", opens = "",
            topics = "the deltanet hybrid at its sixty-four trunk layers: the NextN draft block at layer index 64 under self-speculative decode, the DFlash block drafter's rounds on the 27B trunk, the NextN head files' architectures, the tokenizer.json twin of the 27B's GGUF, the Metal MTP rail on the 27B"),
        (name = "qwen15-moe", stems = "Qwen1.5-MoE-A2.7B-Chat", opens = "",
            topics = "the shared-expert routed block at its twenty-four blocks: the host twin's model-level matrix - the routed experts requantized to one format a file, routed layers' experts pinned to the host under the streamed prompt, the hot pool and the expert cache on twenty host layers"),
        (name = "qwen3moe-30b", stems = ("Qwen3-30B-A3B-Instruct-2507,Qwen3-30B-A3B-Q8_0,"
                + "Qwen3-30B-A3B-Thinking-2507,Qwen3-30B-A3B-NVFP4-pure"), opens = "",
            topics = "the routed block with no shared expert at its forty-eight blocks: the host-layer split a strict 32768 context forces on a 16 GB card - the split token command and the cut window chain against the CPU chain, a 30-layer ask past the forced count, a two-token prompt on the window chain, layer 0's mirror rows the cut chain writes; the frozen greedy fixtures of the base Q8_0 and the Thinking files"),
        (name = "qwen35moe-35b", stems = ("Qwen3.6-35B-A3B-UD,Qwen3.6-35B-A3B-MTP,"
                + "Qwen3.6-35B-A3B-NVFP4,Qwen3.6-35B-A3B-Q8_0,Qwen3.5-35B-A3B-UD"), opens = "",
            topics = "the deltanet hybrid MoE at its forty layers: the host split a strict 131072 context forces on a 16 GB card against the CPU chain, the published NVFP4 file's attention and expert sites on the nv4 kernels beside four host layers, the Metal support matrix row's generate token for token against the CPU truth, the Metal MTP rail's sampled selection landed on the device against the rows-landed stream; the frozen greedy fixtures of the Q8_0 file and the Qwen3.5 i-quant files"),
        (name = "flash-next", stems = "Qwen3.8-Flash-Next,mtp-Qwen3.8-Flash-Next", opens = "",
            topics = "the hyper-connection hybrid MoE with the n-gram side input at its three shards and split NextN head: the whole-model resident driver's window chain, split token command, NextN draft and speculative round against the CPU chain, the continuations from an adopted checkpoint, the parity file's spec row, the Metal support matrix's served row and the MTP parity rows, the scheduler's cells on the carrier"),
        (name = "qwen3-4b", stems = "Qwen3-4B-Instruct-2507-Q8_0", opens = "",
            topics = "the dense qwen3 decoder at the 4B's 2560-wide rows over thirty-six layers: the CUDA driver's forced feed with the fused q/k-norm rope pass against the CPU chain"),
        (name = "qwen3-8b", stems = "Qwen3-8B-BF16", opens = "",
            topics = "the dense qwen3 decoder on bf16 planes: its frozen greedy fixture"),
        (name = "qwen3coder-30b", stems = "Qwen3-Coder-30B-A3B", opens = "",
            topics = ("the routed qwen3 Coder files at Q8_0, Q4_K_M and UD-Q3_K_XL: their frozen "
                + "greedy fixtures")),
        (name = "qwen3coder-next", stems = "Qwen3-Coder-Next", opens = "",
            topics = ("the qwen3next file layout at 512 experts: the whole-model driver's device "
                + "select past 256 experts with the first routed layers' experts on the host, one "
                + "window against the CPU chain; the loader's grouped-to-tiled permutes under its "
                + "frozen greedy fixture")),
        (name = "qwen3omni-30b", stems = "Qwen3-Omni-30B", opens = "mmproj-Qwen3-Omni-30B",
            topics = ("the Omni MoE thinker under its towers: the qwen3a AuT tower's greedy audio "
                + "transcription token for token with llama-mtmd-cli, the image chat turn and a "
                + "text turn recalling it through the qwen3v tower")),
        (name = "qwen3vl-8b", stems = "Qwen3VL-8B", opens = "",
            topics = ("no cell loads the file: its spec row and provenance pin, the board's vision "
                + "row")),
        (name = "qwen2audio-7b", stems = "qwen2audio-7b", opens = "",
            topics = ("the qwen2audio chat template's audio markers against the decoder's own "
                + "vocab")),
        (name = "gemma3-12b", stems = "gemma-3-12b-it", opens = "mmproj-gemma-3-12b",
            topics = ("the gemma3v image turn at the 3840-wide projection on the 12B pair, the "
                + "Metal family matrix's K-quant gemma3 row on k4 and k6 planes and the Q6_K token "
                + "table, the 12B's frozen greedy fixture")),
        (name = "llama31-8b", stems = "Meta-Llama-3.1-8B-Instruct-Q8_0",
            opens = "Llama-3.2-1B-Instruct-Q8_0",
            topics = ("the Metal dim ladder at the 8B's 4096-wide rows, nested in the 1B's tests: "
                + "batched decode past B=4 on the q8 GEMM tier in the decode and prefill parity "
                + "files, the support matrix's 8B matrix cell and batched dim ladder")),
        (name = "llama33-70b", stems = "Llama-3.3-70B", opens = "Llama-3.2-1B-Instruct-Q8_0",
            topics = ("the Metal dim ladder at the 70B's 8192-wide rows, nested in the 1B's tests: "
                + "the decode parity's 70B decode served on the GPU, the support matrix's 70B "
                + "K-quant matrix cell and batched dim ladder on the kq mul_mm twin")),
        (name = "mistral-7b", stems = "Mistral-7B-Instruct-v0.3-Q8_0", opens = "",
            topics = ("the Mistral v0.3 decoder's frozen greedy fixture, the v0.3 vocabulary's "
                + "byte pieces past its 768 control tokens")),
        (name = "mistral3-24b", stems = "Mistral-Small-3.1-24B", opens = "",
            topics = "the mistral3 decoder on the tekken vocabulary: its frozen greedy fixture")
    ]
}

//! the length of the longest stem of the comma list `stems` that opens `name`, 0 when none does
def private longest_stem_len(stems, name : string) : int {
    var best = 0
    for (stem in split(stems, ",")) {
        let st = strip(stem)
        if (length(st) > best && name |> starts_with(st)) {
            best = length(st)
        }
    }
    return best
}

//! the row of `rows` (each a `name` and its `stems`) whose stem opens the most of the model file `file` (a path or a bare
//! name): its name, "" when no row holds the file
def private stem_owner(rows; file : string) : string {
    let name = base_name(file)
    var best = 0
    var owner = ""
    for (row in rows) {
        let n = longest_stem_len(row.stems, name)
        if (n > best) {
            best = n
            owner = row.name
        }
    }
    return owner
}

//! the category that holds the model file `file` (a path or a bare name), "" when none does
def model_category(file : string) : string => stem_owner(model_categories(), file)

//! true when the comma list `named` names `name`, exactly
def list_names(named, name : string) : bool {
    for (tok in split(named, ",")) {
        return true if (strip(tok) == name)
    }
    return false
}

//! A model family: the decoder files a `--family` run opens, under the name `family_on` tags a block with. `stems` as a
//! category's; a name is the family's whose stem opens the most of it, so `Qwen3-30B-A3B` is the routed family's under `Qwen3-`
typedef ModelFamily = tuple<name : string; stems : string>

//! every family with a row; a file no row holds - a tower, a speech model, a fixture - is open to any family run
def model_families() : array<ModelFamily> {
    return <- [
        (name = "llama", stems = "Llama-3.2,Meta-Llama-3.1,tinyllama,SmolLM2"),
        (name = "qwen2", stems = "Qwen2.5-0.5B,Qwen2.5-1.5B"),
        (name = "qwen3", stems = "Qwen3-"),
        (name = "phi3", stems = "Phi-3.5"),
        (name = "gemma2", stems = "gemma-2-"),
        (name = "gemma3", stems = "gemma-3-"),
        (name = "gemma4", stems = "gemma-4-12B,gemma-4-12b,gemma-4-31B"),
        (name = "gemma4e", stems = "gemma-4-E2B,gemma-4-E4B"),
        (name = "gemma4moe", stems = "gemma-4-26B,mtp-gemma-4-26B"),
        (name = "qwen3moe", stems = "Qwen3-30B-A3B"),
        (name = "qwen2moe", stems = "Qwen1.5-MoE"),
        (name = "gptoss", stems = "gpt-oss"),
        (name = "qwen35", stems = "Qwen3.5-,Qwen3.6-27B,Qwen3.8-,mtp-Qwen3.8-"),
        (name = "qwen35moe", stems = "Qwen3.6-35B-A3B"),
        (name = "qwen4exp", stems = "micro-qwen4exp")
    ]
}

//! the family of the model file `file` (a path or a bare name), "" when no row holds it
def model_family(file : string) : string => stem_owner(model_families(), file)

//! Why a family run (`named`, DASLLAMA_TEST_FAMILY) leaves the model `file` alone, "" when it may open it: a run that names
//! no family opens every file, a run that names some leaves the files of every other family
def family_skip_reason(file, named : string) : string {
    return "" if (empty(named))
    let fam = model_family(file)
    return "" if (empty(fam) || list_names(named, fam))
    return "{base_name(file)} is the {fam} family's, outside this run's (DASLLAMA_TEST_FAMILY={named})"
}

//! the names of `named` that are no category's
def unknown_categories(named : string) : array<string> {
    let known <- {for (cat in model_categories()); cat.name}
    return <- [for (tok in split_trimmed(named, ",")); tok; where !key_exists(known, tok)]
}

//! whether a category of the comma list `named` opens the model `file` beside its own (its row's `opens`)
def private opened_by(file, named : string) : bool {
    let name = base_name(file)
    for (cat in model_categories()) {
        return true if (list_names(named, cat.name) && longest_stem_len(cat.opens, name) > 0)
    }
    return false
}

//! Why a run leaves the model `file` alone for its category, "" when it may open it. A run that names no category
//! opens no category's file; a run that names some opens their files, the files their rows' `opens` name, and no other
//! model
def category_skip_reason(file, named : string) : string {
    let cat = model_category(file)
    if (empty(named)) {
        return "" if (empty(cat))
        return "{base_name(file)} is the {cat} category's - run.das --category {cat} runs it (DASLLAMA_TEST_CATEGORIES)"
    }
    return "" if (empty(cat) ? opened_by(file, named) : list_names(named, cat))
    return "{base_name(file)} is outside the named categories (DASLLAMA_TEST_CATEGORIES={named})"
}

//! what a run's categories say of the model `file`: `why` it is left alone ("" when it may open), and whether the
//! size tier's ceiling is lifted for it - a category the run named is run whole, past the ceiling
def category_gate(file, named : string) : tuple<why : string; lifts_tier : bool> {
    let why = category_skip_reason(file, named)
    return (why = why, lifts_tier = empty(why) && !empty(named))
}
