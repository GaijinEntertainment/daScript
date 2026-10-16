options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-26B-A4B MXFP4_MOE (the Q4_K_M file requantized, `../performance/model_specs.das`: the fused gate_up and the down
// expert stacks MXFP4 on disk, every other plane the source's), large tier: arm `g4mx`. The expert stacks load as the
// native-MXFP4 planes under gemma-4's GeGLU and per-expert down scale. Skips off the JIT, without DASLLAMA_GPU=1 on a device
// the whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card.
let private GEMMA4_26B_MX4_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", arm = "g4mx")
//! the routed model's argmax slack: the IQ3_XXS file's (a router near-tie flips whole positions between the arms)
let private GEMMA4_26B_MX4_HITS_SLACK = 12l

[test]
def test_gpu_resident_gemma4_26b_mx4(t : T?) {
    t |> run("gemma-4-26B-A4B MXFP4_MOE: every site in its own format - the native-MXFP4 expert stacks among them - and the driver serving every call") @(t : T?) {
        gemma_format_cell(t, GEMMA4_26B_MX4_FILE, true)
    }
    t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_MX4_FILE, GEMMA4_26B_MX4_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}
