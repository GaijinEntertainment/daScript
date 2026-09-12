options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads the 26B-A4B alone

//! the gemma-4 MoE (large tier): 128 experts, 8 routed, a parallel dense shared expert, the routed branch's own norms
let private GEMMA4_26B_FILE = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf"
//! the routed model's argmax slack: a near-tie at the router flips whole positions between the arms, and the CPU chain
//! sits farther from llama.cpp on this model than the resident does (its own forced-feed logits are no instrument here)
let private GEMMA4_26B_HITS_SLACK = 8l

[test]
def test_gpu_resident_gemma4_26b(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the routed block's gemma-4 form") @(t : T?) {
        if (!arm_on(t, "g4m")) return
        ppl_compare(t, 150l, 150l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4m")) return
        window_pin(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK)
    }
}
