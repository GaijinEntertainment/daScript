options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-26B-A4B UD-IQ3_XXS (the routed MoE with its parallel dense shared expert and the routed branch's own norms),
// large tier: arm `g4m`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without
// the file under models_dir(), or when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_FILE = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf"
//! the routed model's argmax slack: a near-tie at the router flips whole positions between the arms, and the CPU chain
//! sits farther from the reference exe on this model than the resident does (its own forced-feed logits are no instrument
//! here); the hits read 126 to 139 against the CPU chain's 133 across kernel recompiles (the readings: CLAUDE.md)
let private GEMMA4_26B_HITS_SLACK = 12l

[test]
def test_gpu_resident_gemma4_26b(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the routed block's gemma-4 form") @(t : T?) {
        if (!arm_on(t, "g4m")) return
        ppl_compare(t, 150l, 150l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4m")) return
        assert_pf_window(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}
