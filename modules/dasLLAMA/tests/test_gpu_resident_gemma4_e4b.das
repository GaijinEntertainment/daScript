options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads E4B alone

//! the larger E-series carrier (large tier): 42 layers, eighteen sharing K/V
let private GEMMA4E4_FILE = "gemma-4-E4B-it-Q8_0.gguf"

[test]
def test_gpu_resident_gemma4_e4b(t : T?) {
    t |> run("gemma-4-E4B Q8_0: resident prefill + decode vs the CPU chain, forty tokens - the E-series arms on the larger carrier") @(t : T?) {
        if (!arm_on(t, "g4f")) return
        gemma_forced_feed(t, 40l, GEMMA4E4_FILE)
    }
    t |> run("gemma-4-E4B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4f")) return
        ppl_compare(t, 150l, 150l, GEMMA4E4_FILE)
    }
}
