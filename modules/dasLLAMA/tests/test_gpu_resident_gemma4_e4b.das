options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident

// gemma-4-E4B Q8_0 (the larger E-series carrier), large tier: arm `g4f`. Skips off the JIT, without DASLLAMA_GPU=1 on
// a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit
// the card (DASLLAMA_GPU_MIN_CTX shortens the mirror); the block-codec cells also skip where the environment arms another
// mirror codec over the one they ask (DASLLAMA_VK_KV32=1).
let private GEMMA4E4_FILE = GemmaFile(file = "gemma-4-E4B-it-Q8_0.gguf", arm = "g4f")

[test]
def test_gpu_resident_gemma4_e4b(t : T?) {
    t |> run("gemma-4-E4B Q8_0: resident prefill + decode vs the CPU chain, forty tokens - the E-series arms on the larger carrier") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA4E4_FILE)
    }
    t |> run("gemma-4-E4B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4E4_FILE)
    }
}

//! the E4B on the block-codec mirrors (arm `g4f-codec`): a q8_0 mirror in the forced-feed form, a tq4 mirror scored as a perplexity
//! (a 4-bit mirror moves the logits past any bar on the row) - the shared-KV layers store no row of their own and the side rows ride
//! every step either way
[test]
def test_gpu_resident_gemma4_e4b_block_codecs(t : T?) {
    let gf = GemmaFile(file = "gemma-4-E4B-it-Q8_0.gguf", arm = "g4f-codec")
    t |> run("gemma-4-E4B Q8_0: a q8_0 K/V mirror vs the CPU chain, forty tokens") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            gemma_forced_feed(t, 40l, gf)
        }
    }
    t |> run("gemma-4-E4B Q8_0: a tq4 K/V mirror scored as a perplexity against the CPU chain") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, 150l, 150l, gf)
        }
    }
}
