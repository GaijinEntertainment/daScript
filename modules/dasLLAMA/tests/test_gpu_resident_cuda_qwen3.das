options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella's [init] registrations arm the qwen3 arch and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_gpu_resident
require _model_tier
require _resident_feed
require _cuda_feed

// The CUDA home's whole-model driver on Qwen3-0.6B Q8_0 (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the q/k-normed carrier,
// whose norm, rope and K/V store run as the fused pass `CuQknRopeKv` in the token command and in the window chain. The
// forced-feed logits-tolerance form of the CUDA llama file: the same fixed tokens through the CUDA chain and the all-CPU
// chain, the prefill logits and every step's within the bar, the one-step-off control past it, at 40 tokens (one window)
// and at a prompt of two windows; the witnesses that the CUDA driver served every position, ran the windows the prompt
// takes and replayed the captured graph past the first step, a CPU run with the model off the device moving none of
// them (the control). Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the
// driver serves, without the file under models_dir(), or where the prompt window is not PF_WINDOW_ROWS rows.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model off the device) prefills on the CPU
    if (pf_window_rows_pin() <= 0l) {   // the cells size their prompts against PF_WINDOW_ROWS; the plan's own pick would run the two-window prompt as one
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
}

let private LOGIT_BAR_REL = 0.06   //! of the step's max |logit|: the CUDA llama file's bar
let private FILE_06B = "Qwen3-0.6B-Q8_0.gguf"   //! head 128 over 16 q heads and 8 k heads, the per-head q/k norm, no attention bias
let private FILE_4B = "Qwen3-4B-Instruct-2507-Q8_0.gguf"   //! the larger dense qwen3: 36 layers over 2560, the same head geometry at 32 q heads

[test]
def test_gpu_resident_cuda_qwen3_06b(t : T?) {
    t |> run("{FILE_06B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_06B, "qwen3", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_06B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_06B, "qwen3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_qwen3_4b(t : T?) {
    t |> run("{FILE_4B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_4B, "qwen3", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_4B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_4B, "qwen3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}
