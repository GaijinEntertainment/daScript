options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident

// gemma-4-E2B Q8_0 (the E-series carrier: per-layer embeddings, the upper layers sharing the K/V of the ones below, two
// dense FFN widths, a sliding window the two-window cell engages): arm `g4e`. Skips off the JIT, without DASLLAMA_GPU=1
// on a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4E_FILE = GemmaFile(file = "gemma-4-E2B-it-Q8_0.gguf", arm = "g4e")

[test]
def test_gpu_resident_gemma4_e2b(t : T?) {
    t |> run("gemma-4-E2B Q8_0: resident prefill + decode - the per-layer-embedding branch, the shared-KV layers, two FFN widths - vs the CPU chain, forty tokens") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 520 ids prefilled - two windows, the sliding window engaged on the last eight - then 80 teacher-forced") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA4E_FILE)
    }
}

[test]
def test_gpu_resident_gemma4_e2b_block_codecs(t : T?) {
    t |> run("gemma-4-E2B Q8_0: a q8_0 K/V mirror - the shared-KV layers store no row of their own - vs the CPU chain, forty tokens") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            gemma_forced_feed(t, 40l, GEMMA4E_FILE)
        }
    }
    t |> run("gemma-4-E2B Q8_0: a tq4 K/V mirror - a shared-KV layer rotates its query and stores nothing - scored as a perplexity against the CPU chain") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
        }
    }
}
