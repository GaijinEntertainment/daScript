options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-31B Q4_K_M (the largest dense carrier, a 32 GB card), large tier: arm `g31`. The agreement form the 12B
// Q4_K_M takes, one eight-token cell: on a K-quant file the CPU chain's own prefill and decode kernels land far apart,
// and the CPU control of a dense 31B runs the reference kernel bodies for half an hour on a box with no tune sidecar.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under
// models_dir(), or when the residency plan does not fit the card.
let private GEMMA4_31B_FILE = "gemma-4-31B-it-Q4_K_M.gguf"

[test]
def test_gpu_resident_gemma4_31b(t : T?) {
    t |> run("gemma-4-31B Q4_K_M: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g31")) return
        prefill_vs_decode(t, 8l, GEMMA4_31B_FILE)
    }
}
