options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // the gemma cells; one model a file - this process loads gemma-3-1b alone

//! the gemma dense base: the sandwich norms, the sliding-window layers beside the global ones with their own
//! rope class, the GeGLU FFN and the sqrt(dim) embed scale
let private GEMMA3_FILE = "gemma-3-1b-it-Q8_0.gguf"

[test]
def test_gpu_resident_gemma3_one_window(t : T?) {
    t |> run("gemma-3-1b: resident prefill + decode with sandwich norms and sliding layers on device vs the CPU chain, one window") @(t : T?) {
        if (!arm_on(t, "g3")) return
        gemma_forced_feed(t, 40l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: an eight-token prompt - the shortest window, where this model's logits move most with the rounding") @(t : T?) {
        if (!arm_on(t, "g3")) return
        gemma_forced_feed(t, 8l, GEMMA3_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_past_window(t : T?) {
    t |> run("gemma-3-1b: a 520-token prompt - two prefill windows, the sliding window engaged on the last eight positions") @(t : T?) {
        if (!arm_on(t, "g3")) return
        window_pin(t)
        gemma_forced_feed(t, PF_WINDOW_ROWS + 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: a 600-token prompt - every sliding layer masks below its window on the prefill tiles, then the decode chunks past it") @(t : T?) {
        if (!arm_on(t, "g3")) return
        window_pin(t)
        gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, GEMMA3_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_prefill_vs_decode(t : T?) {
    t |> run("gemma-3-1b: eight tokens through the window chain against the same eight through the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g3")) return
        prefill_vs_decode(t, 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: the same at forty tokens") @(t : T?) {
        if (!arm_on(t, "g3")) return
        prefill_vs_decode(t, 40l, GEMMA3_FILE)
    }
}
