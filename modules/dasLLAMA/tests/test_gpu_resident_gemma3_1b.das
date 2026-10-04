options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident

// gemma-3-1b Q8_0 (the dense base: sandwich norms, sliding layers beside the global ones with their own rope class,
// the GeGLU FFN, the sqrt(dim) embed scale): arm `g3`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the
// whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card.
let private GEMMA3_FILE = GemmaFile(file = "gemma-3-1b-it-Q8_0.gguf", arm = "g3")

[test]
def test_gpu_resident_gemma3_one_window(t : T?) {
    t |> run("gemma-3-1b: resident prefill + decode with sandwich norms and sliding layers on device vs the CPU chain, one window") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: an eight-token prompt - the shortest window, where this model's logits move most with the rounding") @(t : T?) {
        gemma_forced_feed(t, 8l, GEMMA3_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_past_window(t : T?) {
    t |> run("gemma-3-1b: a 520-token prompt - two prefill windows, the sliding window engaged on the last eight positions") @(t : T?) {
        assert_pf_window(t)
        gemma_forced_feed(t, PF_WINDOW_ROWS + 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: a 600-token prompt - every sliding layer masks below its window on the prefill tiles, then the decode chunks past it") @(t : T?) {
        assert_pf_window(t)
        gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, GEMMA3_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_prefill_vs_decode(t : T?) {
    t |> run("gemma-3-1b: eight tokens through the window chain against the same eight through the token command, both arms") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: the same at forty tokens") @(t : T?) {
        prefill_vs_decode(t, 40l, GEMMA3_FILE)
    }
}

let private PAST_SLIDING_WINDOW = 2l * PF_WINDOW_ROWS + 76l   //! a third prefill window: it starts a window's rows past the 512-key sliding window

[test]
def test_gpu_resident_gemma3_block_codecs(t : T?) {
    t |> run("gemma-3-1b: a q8_0 K/V mirror at 1100 tokens - the third window's f16 shadow starts past the sliding layers' window") @(t : T?) {
        assert_pf_window(t)
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            gemma_forced_feed(t, PAST_SLIDING_WINDOW, GEMMA3_FILE)
        }
    }
    t |> run("gemma-3-1b: the same prompt over a tq4 mirror, 80 ids teacher-forced after it and scored as a perplexity against the CPU chain") @(t : T?) {
        assert_pf_window(t)
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, PAST_SLIDING_WINDOW, 80l, GEMMA3_FILE)   //! a 4-bit mirror reads one step 5.0 of a 33.7 max logit from the CPU's: no logit bar holds it with room
        }
    }
}
