options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-12B Q4_K_M (the dense carrier on the K-quant planes): arms `g4k` (eight tokens) and `g4x` (300). The
// agreement form alone: on a K-quant file the CPU chain's own prefill and decode kernels land far apart, so a
// CPU-referenced bar would measure the model's sensitivity to the activation rounding, not the driver. Skips off the
// JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or when
// the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
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
