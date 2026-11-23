options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_gemma3v   // the SigLIP tower's lane pin: the mmproj's form loader
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _model_once   // with_model_once: the mmproj's one block in the area
require _cpu_gpu_model   // with_cpu_and_gpu_model: the decoder's one block in the area
require _vision_forms   // the towers' forms
require _vision_chat_cells   // the image chat turn's cells

// The image chat turn: the prompt stream shape around the soft-token splice (tier 2) and the
// greedy caption (tier 3). Pinned: marker ids, media-first order, span length from the
// geometry, and that the model describes what is in the picture. NOT token-parity with
// llama-mtmd-cli — tests/CLAUDE.md carries why.
// The cells are `_vision_chat_cells.das`'s. A cell reads a tower and a decoder, each file loaded once in the area, so it
// runs in the inner of two nested blocks; a tower its family's test file loads (the gemma-3-4b, gemma-4 E2B and 12B,
// Qwen3-Omni, Qwen3-VL 4B and Qwen2.5-Omni mmprojs) carries its pairs' cells there, under its block's image form
// (test_gemma3v.das, test_gemma4v.das, test_gemma4uv.das, test_qwen3v.das, test_qwen25v.das). The gemma-3-12b pair,
// whose mmproj no other test reads, is this file's: the large-tier decoder's block first, the tower's block nested in
// it, so a default run loads neither.

//! The gemma-3-12b mmproj's form loader: `image`, the carrier's own load through the image rail on the lane the policy
//! names (`image_tower_form`).
def private gemma3v_12b_form(path, form : string) : VisionEmbedder =>
    <- image_tower_form(path, tower_form(form), @@set_gemma3v_q8, @@reset_gemma3v_q8, @@gemma3v_serves_q8)

// The gemma-3-12b pair, a block a file.
[test(description = "The gemma-3-12b decoder and its mmproj each load once, the tower under the decoder: the pair sniffs as gemma3v with the 3840-wide projection, splices the fixed 256 image rows, and greedily captions the cats in over 40 characters")]
def test_vision_chat_gemma3_12b_pair(t : T?) {
    return if (!family_on(t, "gemma3v") || !vision_chat_gate(t, "gemma3", "gemma-3-12b-it-Q4_K_M.gguf", 80))
    with_cpu_and_gpu_model(t, "vision", "gemma-3-12b-it-Q4_K_M.gguf", VISION_CHAT_CTX, [cpu_cells_form(QuantMode.fp32)]) $(var m; form) {
        return if (!form.alone)
        with_model_once(t, "vision", "mmproj-gemma-3-12b-it-F16.gguf", ["image"], @@gemma3v_12b_form) $(var e; tower_form_name) {
            tower_form_is(t, "mmproj-gemma-3-12b-it-F16.gguf", tower_form_name, e.gemma3v)
            vision_chat_cell(t, "the gemma-3-12b image turn: the widened projection, then the caption", m, form, false, e, @@test_vision_chat_gemma3_12b)
        }
    }
}
