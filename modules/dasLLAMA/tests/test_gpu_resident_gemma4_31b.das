options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads the 31B alone

//! the largest gemma-4 dense carrier (large tier, a 32 GB card: 18.2 GB of Q4_K and Q6_K planes) in the agreement form the
//! 12B Q4_K_M takes - on a K-quant file the CPU chain's own prefill and decode kernels land far apart, so the two resident
//! paths are held to each other and to the CPU chain's own gap, not to a CPU-referenced bar; the one cell is eight tokens,
//! since the CPU control of a dense 31B runs the reference kernel bodies for half an hour on a box with no tune sidecar
//! (the RunPod RTX PRO 4500: the resident paths 0.18 of the max logit apart, the CPU chain's own 0.30)
let private GEMMA4_31B_FILE = "gemma-4-31B-it-Q4_K_M.gguf"

[test]
def test_gpu_resident_gemma4_31b(t : T?) {
    t |> run("gemma-4-31B Q4_K_M: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g31")) return
        prefill_vs_decode(t, 8l, GEMMA4_31B_FILE)
    }
}
