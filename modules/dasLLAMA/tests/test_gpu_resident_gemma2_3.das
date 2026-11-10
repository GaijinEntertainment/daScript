options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident

// gemma-2-2b Q8_0 (the attention logit softcap, a window on the even layers, no qk-norm): arm `g2`. Skips off the
// JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or
// when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA2_FILE = GemmaFile(file = "gemma-2-2b-it-Q8_0.gguf", arm = "g2")

[test(description = "On the whole-model driver gemma-2-2b Q8_0 with the attention softcap keeps forty-token forced-feed logits within 16 percent of max logit of the CPU chain, perplexity within 1.05x, and window chain against token command within 6x the CPU's own gap")]
def test_gpu_resident_gemma2(t : T?) {
    t |> run("gemma-2-2b: resident prefill + decode vs the CPU chain, forty tokens - the attention logit softcap on the 8-row tile and the token command") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA2_FILE)
    }
    t |> run("gemma-2-2b: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA2_FILE)
    }
    t |> run("gemma-2-2b: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA2_FILE)
    }
}

// gemma-3-1b Q8_0 (the dense base: sandwich norms, sliding layers beside the global ones with their own rope class,
// the GeGLU FFN, the sqrt(dim) embed scale): arm `g3`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the
// whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card.
let private GEMMA3_FILE = GemmaFile(file = "gemma-3-1b-it-Q8_0.gguf", arm = "g3")

[test(description = "On the whole-model driver gemma-3-1b Q8_0 with sandwich norms and sliding layers on the device keeps forced-feed logits within 16 percent of max logit of the CPU chain at forty-token and eight-token prompts")]
def test_gpu_resident_gemma3_one_window(t : T?) {
    t |> run("gemma-3-1b: resident prefill + decode with sandwich norms and sliding layers on device vs the CPU chain, one window") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: an eight-token prompt - the shortest window, where this model's logits move most with the rounding") @(t : T?) {
        gemma_forced_feed(t, 8l, GEMMA3_FILE)
    }
}

[test(description = "On the whole-model driver gemma-3-1b keeps forced-feed logits within 16 percent of the CPU chain over two-window prompts of 520 and 600 tokens, where the sliding layers mask below their window on the prefill tiles and the decode")]
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

[test(description = "On the whole-model driver gemma-3-1b's window chain and token command agree over 8 and 40 tokens within 6x the distance between the CPU chain's own prefill and decode paths")]
def test_gpu_resident_gemma3_prefill_vs_decode(t : T?) {
    t |> run("gemma-3-1b: eight tokens through the window chain against the same eight through the token command, both arms") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: the same at forty tokens") @(t : T?) {
        prefill_vs_decode(t, 40l, GEMMA3_FILE)
    }
}

let private PAST_SLIDING_WINDOW = 2l * PF_WINDOW_ROWS + 76l   //! a third prefill window: it starts a window's rows past the 512-key sliding window

[test(description = "On the whole-model driver gemma-3-1b over a q8_0 K/V mirror keeps 1100-token forced-feed logits within 16 percent of the CPU chain past the sliding window, and over a tq4 mirror keeps the teacher-forced perplexity within 1.05x")]
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

//! the sliding layers' rings against the flat mirror (`ring_vs_flat`): 1300 prose ids past the 512-position window and eight steps,
//! bit for bit, on the f16 mirror and the q8_0 one
[test(description = "On the whole-model driver gemma-3-1b's sliding layers on ringed K/V mirrors match a flat every-row mirror bit for bit over 1300 prose ids past the 512-position window and eight steps, on f16 and q8_0 mirrors")]
def test_gpu_resident_gemma3_ring(t : T?) {
    for (kv in fixed_array(KVDtype.f16, KVDtype.q8_0)) {
        t |> run("gemma-3-1b Q8_0: the sliding layers' rings against every row kept, a {kv} mirror, 1300 ids and eight steps") @(t : T?) {
            ring_vs_flat(t, 1300l, GEMMA3_FILE, kv)
        }
    }
}

// gemma-3-4b Q8_0 (the dense base at its wider geometry): arm `g3b`. Skips off the JIT, without DASLLAMA_GPU=1 on
// a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA3_4B_FILE = GemmaFile(file = "gemma-3-4b-it-Q8_0.gguf", arm = "g3b")

[test(description = "On the whole-model driver gemma-3-4b Q8_0 keeps forty-token forced-feed logits within 16 percent of max logit of the CPU chain and its 150-position teacher-forced perplexity within 1.05x of the CPU chain's")]
def test_gpu_resident_gemma3_4b(t : T?) {
    t |> run("gemma-3-4b: resident prefill + decode vs the CPU chain, forty tokens - the dense base at its wider geometry") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA3_4B_FILE)
    }
    t |> run("gemma-3-4b: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA3_4B_FILE)
    }
}
