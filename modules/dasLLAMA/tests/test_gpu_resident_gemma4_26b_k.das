options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads the 26B-A4B UD-Q4_K_M alone

//! the gemma-4 MoE (large tier) in the model table's official quant: k4 gate and up expert stacks beside NATIVE Q5_1 down
//! stacks at 704 (the q51 expert rail - the s and e stamps, the Q8_0-activation decode GEMV), the dense triple and the
//! attention head on the K-quant rails
let private GEMMA4_26B_K_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"
//! the routed model's argmax slack (the IQ3_XXS file's reading): a router near-tie flips whole positions between the arms
let private GEMMA4_26B_K_HITS_SLACK = 8l

[test]
def test_gpu_resident_gemma4_26b_k(t : T?) {
    t |> run("gemma-4-26B-A4B Q4_K_M: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the q51 down experts on their own rail") @(t : T?) {
        if (!arm_on(t, "g4mk")) return
        ppl_compare(t, 150l, 150l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK)
    }
    t |> run("gemma-4-26B-A4B Q4_K_M: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4mk")) return
        window_pin(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK)
    }
}
