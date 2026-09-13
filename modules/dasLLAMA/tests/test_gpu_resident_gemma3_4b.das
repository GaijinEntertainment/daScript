options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-3-4b Q8_0 (the dense base at its wider geometry): arm `g3b`. Skips off the JIT, without DASLLAMA_GPU=1 on
// a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA3_4B_FILE = "gemma-3-4b-it-Q8_0.gguf"

[test]
def test_gpu_resident_gemma3_4b(t : T?) {
    t |> run("gemma-3-4b: resident prefill + decode vs the CPU chain, forty tokens - the dense base at its wider geometry") @(t : T?) {
        if (!arm_on(t, "g3b")) return
        gemma_forced_feed(t, 40l, GEMMA3_4B_FILE)
    }
    t |> run("gemma-3-4b: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g3b")) return
        ppl_compare(t, 150l, 150l, GEMMA3_4B_FILE)
    }
}
