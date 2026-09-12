options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads the 12B Q4_K_M alone

//! the gemma-4 dense carrier on the K-quant planes: the agreement form alone - on this file the CPU chain's own prefill and
//! decode kernels land 13% to 44% of the max logit apart, so a CPU-referenced bar would measure the model's sensitivity to
//! the K-quant activation rounding, not the driver
let private GEMMA4_FILE = "gemma-4-12B-it-Q4_K_M.gguf"

[test]
def test_gpu_resident_gemma4_prefill_vs_decode(t : T?) {
    t |> run("gemma-4-12B: eight tokens through the window chain against the token command, both arms - the wide heads' prefill kernel against their decode kernel") @(t : T?) {
        if (!arm_on(t, "g4k")) return
        prefill_vs_decode(t, 8l, GEMMA4_FILE)
    }
    t |> run("gemma-4-12B: the same at 300 tokens - the wide heads over many keys on both paths") @(t : T?) {
        if (!arm_on(t, "g4x")) return
        prefill_vs_decode(t, 300l, GEMMA4_FILE)
    }
}
