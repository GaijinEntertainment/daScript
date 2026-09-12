options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads the 26B-A4B alone
require dasllama/dasllama_gpu_resident   // set_gpu_ctx_max

//! the gemma-4 MoE (large tier): 128 experts, 8 routed, a parallel dense shared expert, the routed branch's own norms
let private GEMMA4_26B_FILE = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf"

[init]
def private ctx_cap_for_the_moe() {
    set_gpu_ctx_max(4096l)   //! the 26B's planes take most of a 16 GB card; the mirror keeps to what is left
}

[test]
def test_gpu_resident_gemma4_26b(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS: resident prefill + decode vs the CPU chain, forty tokens - the routed block's gemma-4 form") @(t : T?) {
        if (!arm_on(t, "g4m")) return
        gemma_forced_feed(t, 40l, GEMMA4_26B_FILE)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4m")) return
        ppl_compare(t, 150l, 150l, GEMMA4_26B_FILE)
    }
}
