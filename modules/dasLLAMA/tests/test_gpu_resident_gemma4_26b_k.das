options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-26B-A4B Q4_K_M (the routed MoE in the model table's official quant: k4 gate and up expert stacks beside native
// Q5_1 down stacks - the q51 expert rail - with the dense triple and the attention head on the K-quant rails), large
// tier: arm `g4mk`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the
// file under models_dir(), or when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_K_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"
//! the routed model's argmax slack: a router near-tie flips whole positions between the arms, and a kernel recompile alone
//! moves the hits - 122 to 128 against the CPU chain's 134 over the 150-position cell (the readings: CLAUDE.md)
let private GEMMA4_26B_K_HITS_SLACK = 16l
//! the two-window cell's band: a k4 fold's rounding pattern alone moved it 1.70 -> 2.44 against the CPU chain's 1.03 (a router near-tie flips whole positions), the 150 + 150 cell 5.51 -> 5.91 against 3.36
let private GEMMA4_26B_K_PPL_RATIO_TWO_WINDOWS = 2.5

[test]
def test_gpu_resident_gemma4_26b_k(t : T?) {
    t |> run("gemma-4-26B-A4B Q4_K_M: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the q51 down experts on their own rail") @(t : T?) {
        if (!arm_on(t, "g4mk")) return
        ppl_compare(t, 150l, 150l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B Q4_K_M: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4mk")) return
        assert_pf_window(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK, GEMMA4_26B_K_PPL_RATIO_TWO_WINDOWS)
    }
}
