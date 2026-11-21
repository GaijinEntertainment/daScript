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
//! `test_run_suites.das` holds every CUDA-reaching file to a token
let CUDA_PASS_ONLY = ("cuda,segments,regions,resident_moe,embeddings,gptoss,gemma4_dense,"
    + "gemma4_moe,gemma2_3,gemma4_e2b,mtp_gemma_drafter")

//! A model category: model files no run opens unless the run names the category. `stems` is a comma list - a file is
//! the category's when its name starts with one of them; `topics` is what only these files reach
typedef ModelCategory = tuple<name : string; stems : string; topics : string>

//! every category; `../TEST_CATEGORIES.md` carries the same rows for a reader, and `REVIEW.das` holds the two together
def model_categories() : array<ModelCategory> {
    return <- [
        (name = "gemma4-e4b", stems = "gemma-4-E4B,mmproj-gemma-4-E4B",
            topics = "the gemma-4 audio embedder widening 1536 to a 2560 decoder, the gemma-4 vision projector at 2560 with no std bias"),
        (name = "gemma4-dense", stems = "gemma-4-12B-it,gemma-4-12b-it,gemma-3-4b",
            topics = "the dense gemma-4 decoder at its forty-eight blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain, the Metal counting generates and batched decode token for token against the CPU truth on the K-quant and Q8_0 planes, the image chat on the 12B; the gemma-3 dense base at the 4B's wider geometry, and the image chat on it"),
        (name = "gemma4-31b", stems = "gemma-4-31B,mmproj-gemma-4-31B",
            topics = "the Metal residual step past its 4096-float slab, on a decode of the 31B's 5376-wide rows"),
        (name = "gptoss-20b", stems = "gpt-oss-20b",
            topics = "the gpt-oss decoder at its twenty-four blocks: the frozen greedy fixtures against the reference, short and with the sliding window engaged; a cold planar mint streaming the native-MXFP4 expert planes under their bytes"),
        (name = "gemma4-moe", stems = "gemma-4-26B-A4B-it,gemma-4-26B_q4_0,mtp-gemma-4-26B,mmproj-gemma-4-26B",
            topics = "the routed gemma-4 decoder at its thirty blocks: the frozen greedy fixtures against the reference, the resident drivers' teacher-forced perplexity against the CPU chain on the IQ, K-quant and MXFP4 expert stacks and with routed layers' experts on the host under a pinned plan room, the IQ4_XS stack only the last block carries, a cold planar mint streaming the native-MXFP4 expert planes, the QAT Q4_0 file's repacked expert planes, the NextN draft head, the DFlash block drafter's rounds on the routed trunk")
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
        (name = "llama", stems = "Llama-3.2,tinyllama,SmolLM2"),
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

//! Why a run leaves the model `file` alone for its category, "" when it may open it. A run that names no category
//! opens no category's file; a run that names some opens their files and no other model
def category_skip_reason(file, named : string) : string {
    let cat = model_category(file)
    if (empty(named)) {
        return "" if (empty(cat))
        return "{base_name(file)} is the {cat} category's - run.das --category {cat} runs it (DASLLAMA_TEST_CATEGORIES)"
    }
    return "" if (!empty(cat) && list_names(named, cat))
    return "{base_name(file)} is outside the named categories (DASLLAMA_TEST_CATEGORIES={named})"
}

//! what a run's categories say of the model `file`: `why` it is left alone ("" when it may open), and whether the
//! size tier's ceiling is lifted for it - a category the run named is run whole, past the ceiling
def category_gate(file, named : string) : tuple<why : string; lifts_tier : bool> {
    let why = category_skip_reason(file, named)
    return (why = why, lifts_tier = empty(why) && !empty(named))
}
