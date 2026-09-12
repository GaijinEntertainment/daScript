options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads the 12B Q8_0 alone

//! the gemma-4 dense carrier on the Q8_0 planes (large tier): the weightless v-norm, the V-from-K global layers at head 512
//! beside the sliding 256s, the per-layer output scale, the final softcap and the suppressed ids; arms on a 16 GB card under
//! DASLLAMA_GPU_MIN_CTX=1024 (12.3 GB of weights)
let private GEMMA4_Q8_FILE = "gemma-4-12B-it-Q8_0.gguf"

[test]
def test_gpu_resident_gemma4_dense(t : T?) {
    t |> run("gemma-4-12B Q8_0: resident prefill + decode - v-norm, V-from-K 512-heads, the layer scale, the capped and pinned logits - vs the CPU chain, one window") @(t : T?) {
        if (!arm_on(t, "g4q8")) return
        gemma_forced_feed(t, 40l, GEMMA4_Q8_FILE)
    }
    t |> run("gemma-4-12B Q8_0: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4q8")) return
        ppl_compare(t, 150l, 150l, GEMMA4_Q8_FILE)
    }
    t |> run("gemma-4-12B Q8_0: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them, the 1024 window not yet engaged") @(t : T?) {
        if (!arm_on(t, "g4q8")) return
        window_pin(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4_Q8_FILE)
    }
}
