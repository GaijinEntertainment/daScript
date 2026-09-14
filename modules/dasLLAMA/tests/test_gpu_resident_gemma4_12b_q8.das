options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-12B Q8_0 (the dense carrier on the Q8_0 planes: the weightless v-norm, the V-from-K global layers beside
// the sliding ones, the per-layer output scale, the final softcap and the suppressed ids), large tier: arm `g4q8`.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under
// models_dir(), or when the residency plan does not fit the card - a 16 GB card arms it under DASLLAMA_GPU_MIN_CTX=1024.
let private GEMMA4_Q8_FILE = "gemma-4-12B-it-Q8_0.gguf"
//! the two-window cell's ratio: at 600 positions three implementations of this model read three perplexities over the same
//! 80 targets - the CPU chain 1.97 (72 hits), the resident 2.37 (72), llama.cpp b10660 2.51 (71) - so the resident sits
//! between the references and a 1.05 bar against the CPU chain measures the reference, not the driver
let private GEMMA4_Q8_TWO_WINDOW_PPL_RATIO = 1.3

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
        assert_pf_window(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4_Q8_FILE, 3l, GEMMA4_Q8_TWO_WINDOW_PPL_RATIO)
    }
}
