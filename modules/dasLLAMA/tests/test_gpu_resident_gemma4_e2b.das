options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads E2B alone

//! the gemma-4 E-series carrier: per-layer embeddings (ple 256 on 35 layers), twenty layers sharing the K/V of the fifteen
//! below, two dense FFN widths, a 512 sliding window
let private GEMMA4E_FILE = "gemma-4-E2B-it-Q8_0.gguf"

[test]
def test_gpu_resident_gemma4_e2b(t : T?) {
    t |> run("gemma-4-E2B Q8_0: resident prefill + decode - the per-layer-embedding branch, twenty shared-KV layers, two FFN widths - vs the CPU chain, forty tokens") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        gemma_forced_feed(t, 40l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 520 ids prefilled - two windows, the 512 sliding window engaged on the last eight - then 80 teacher-forced") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        window_pin(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        prefill_vs_decode(t, 8l, GEMMA4E_FILE)
    }
}
