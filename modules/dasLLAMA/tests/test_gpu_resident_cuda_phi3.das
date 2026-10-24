options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella's [init] registrations arm the phi3 arch and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_gpu_resident
require _model_tier
require _resident_feed
require _cuda_feed

// The CUDA home's whole-model driver on the phi3 family: Phi-3.5-mini Q4_K_M (head 96 over 32 heads - the window's
// attention on the h96 flash tile, the K-quant planes, the fused qkv and gate-up planes, the long-rope factors). The
// forced-feed logits-tolerance form of the CUDA llama file at 40 tokens (one window) and at a prompt of two windows, on
// the body the CUDA files share (`_cuda_feed.das`). Every cell skips off the JIT, without DASLLAMA_CUDA=1, without
// DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), or where the prompt window is not
// PF_WINDOW_ROWS rows.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model off the device) prefills on the CPU
    if (pf_window_rows_pin() <= 0l) {   // the cells size their prompts against PF_WINDOW_ROWS; the plan's own pick would run the two-window prompt as one
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
}

let private LOGIT_BAR_REL = 0.06   //! of the step's max |logit|: the CUDA llama file's bar
let private FILE_35 = "Phi-3.5-mini-instruct-Q4_K_M.gguf"   //! head 96: the h96 flash tile, the K-quant planes

//! the two-window prompt alone: after a 40-token prompt the one-row steps at positions 41 and 44 round 7.7 and 6.6 percent of the
//! max logit apart from the CPU's on both attention paths, and the fixture's one-step-off rows sit 6.5 percent apart at one
//! step, so no one bar holds both arms (`../followup_general.md`, the row on the phi one-row steps)
[test]
def test_gpu_resident_cuda_phi35_mini(t : T?) {
    t |> run("{FILE_35}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_35, "phi3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}
