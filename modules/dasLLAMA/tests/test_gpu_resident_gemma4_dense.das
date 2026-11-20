options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require _resident_regions

// gemma-4-12B Q8_0 (the dense carrier on the Q8_0 planes: the weightless v-norm, the V-from-K global layers beside
// the sliding ones, the per-layer output scale, the final softcap and the suppressed ids), large tier: arm `g4q8`.
// Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under
// models_dir(), or when the residency plan does not fit the card - a 16 GB card arms it under DASLLAMA_GPU_MIN_CTX=1024.
let private GEMMA4_Q8_FILE = GemmaFile(file = "gemma-4-12B-it-Q8_0.gguf", arm = "g4q8")
//! the two-window cell's ratio: at 600 positions three implementations of this model read three perplexities over the same
//! 80 targets - the CPU chain 1.97 (72 hits), the resident 2.37 (72), llama.cpp b10660 2.51 (71) - so the resident sits
//! between the references and a 1.05 bar against the CPU chain measures the reference, not the driver
let private GEMMA4_Q8_TWO_WINDOW_PPL_RATIO = 1.3

[test(description = "On the whole-model driver gemma-4-12B Q8_0 keeps one-window forced-feed logits within 16 percent of the CPU chain, its 150-position perplexity within 1.05x of the CPU chain's, and its two-window perplexity within 1.3x")]
def test_gpu_resident_gemma4_dense(t : T?) {
    return if (!family_on(t, "gemma4"))
    t |> run("gemma-4-12B Q8_0: resident prefill + decode - v-norm, V-from-K 512-heads, the layer scale, the capped and pinned logits - vs the CPU chain, one window") @(t : T?) {
        gemma_forced_feed(t, 40l, GEMMA4_Q8_FILE)
    }
    t |> run("gemma-4-12B Q8_0: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_Q8_FILE)
    }
    t |> run("gemma-4-12B Q8_0: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them, the 1024 window not yet engaged") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4_Q8_FILE, 3l, GEMMA4_Q8_TWO_WINDOW_PPL_RATIO)
    }
}

// gemma-4-12B Q4_K_M (the dense carrier on the K-quant planes): arms `g4k` (eight tokens) and `g4x` (300). The
// agreement form alone: on a K-quant file the CPU chain's own prefill and decode kernels land far apart, so a
// CPU-referenced bar would measure the model's sensitivity to the activation rounding, not the driver. Skips off the
// JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or when
// the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_FILE = GemmaFile(file = "gemma-4-12B-it-Q4_K_M.gguf", arm = "g4k")
let private GEMMA4_FILE_X = GemmaFile(file = GEMMA4_FILE.file, arm = "g4x")

[test(description = "On the whole-model driver gemma-4-12B Q4_K_M's wide-head window chain and token command agree over 8 and 300 tokens within 6x the CPU chain's own prefill-versus-decode gap, the agreement form standing in for a CPU-referenced bar")]
def test_gpu_resident_gemma4_prefill_vs_decode(t : T?) {
    return if (!family_on(t, "gemma4"))
    t |> run("gemma-4-12B: eight tokens through the window chain against the token command, both arms - the wide heads' prefill kernel against their decode kernel") @(t : T?) {
        prefill_vs_decode(t, 8l, GEMMA4_FILE)
    }
    t |> run("gemma-4-12B: the same at 300 tokens - the wide heads over many keys on both paths") @(t : T?) {
        prefill_vs_decode(t, 300l, GEMMA4_FILE_X)
    }
}

// gemma-4-12B-it Q8_0 (the `gemma4-dense` category): the batched-step cells of the rig on a carrier whose
// classifier epilogue caps the logits and pins the suppressed ids, whose global layers take V from K at a head of 512
// and whose layers carry an output scale, so the N-row token command serves two rows bit for bit against the sessions
// alone - at one window, past the unsplit position, and a batched step before any one-row step. Skips off the JIT,
// without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
let private REGIONS_FILE = "gemma-4-12B-it-Q8_0.gguf"

[test(description = "On gemma-4-12B with the capped, suppressing classifier, V-from-K global layers and per-layer output scale, over one two-region load: a batched step over two sessions reads bit for bit what each session reads alone; with both prompts past the unsplit position the N-row command's split attention form does too; and a batched step before any one-row step, one-row steps, then a batched step again all read what each session reads alone")]
def test_regions_batched_g4(t : T?) {
    return if (!family_on(t, "gemma4"))
    cell_regions_batched_once(t, REGIONS_FILE)
}
