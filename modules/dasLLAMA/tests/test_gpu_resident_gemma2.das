options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads gemma-2-2b alone

//! the attention logit softcap (50) on every score, a 4096 window on the even layers, no qk-norm
let private GEMMA2_FILE = "gemma-2-2b-it-Q8_0.gguf"

[test]
def test_gpu_resident_gemma2(t : T?) {
    t |> run("gemma-2-2b: resident prefill + decode vs the CPU chain, forty tokens - the attention logit softcap on the 8-row tile and the token command") @(t : T?) {
        if (!arm_on(t, "g2")) return
        gemma_forced_feed(t, 40l, GEMMA2_FILE)
    }
    t |> run("gemma-2-2b: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g2")) return
        ppl_compare(t, 150l, 150l, GEMMA2_FILE)
    }
    t |> run("gemma-2-2b: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g2")) return
        prefill_vs_decode(t, 8l, GEMMA2_FILE)
    }
}
