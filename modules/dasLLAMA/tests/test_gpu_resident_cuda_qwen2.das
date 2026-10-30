options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require _cuda_feed   // cuda_forced_feed: the CUDA home's forced feed against the CPU chain
require _resident_feed   // PF_WINDOW_ROWS
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin: the file's MMQ route

// The qwen2 family on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the Vulkan qwen2 file's forced-feed
// cells: Qwen2.5-0.5B Q8_0 carries the q/k/v projection bias (the rope-and-store kernels' bias arm), and the IQ4_NL `-local` mint of
// it (`../performance/model_specs.das`) the per-32 iq4nl32 planes on the dense sites. One window and two, eight fed steps each,
// every load under the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without the file
// under models_dir(), or where the prompt window is not PF_WINDOW_ROWS rows.

[init]
def private cuda_qwen2_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

let private FILE_Q2 = "Qwen2.5-0.5B-Instruct-Q8_0.gguf"
let private FILE_Q2_IQ4NL32 = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf"
let private QWEN2_BAR_REL = 0.06   //! the Vulkan qwen2 file's bar
let private IQ4NL32_BAR_REL = 0.08   //! ... and its per-32 carrier's

[test]
def test_gpu_resident_cuda_qwen2_bias(t0 : T?) {
    t0 |> run("{FILE_Q2}: one window (40 tokens) and eight steps on the CUDA driver - the q/k/v bias on both commands") @(t : T?) {
        cuda_forced_feed(t, FILE_Q2, "qwen2", 40l, 1l, QWEN2_BAR_REL)
    }
    t0 |> run("{FILE_Q2}: a two-window prompt and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_Q2, "qwen2", PF_WINDOW_ROWS + 88l, 2l, QWEN2_BAR_REL)
    }
}

[test]
def test_gpu_resident_cuda_qwen2_iq4nl32(t0 : T?) {
    t0 |> run("{FILE_Q2_IQ4NL32}: one window (40 tokens) and eight steps on the CUDA driver - the iq4nl32 dense planes") @(t : T?) {
        cuda_forced_feed(t, FILE_Q2_IQ4NL32, "qwen2", 40l, 1l, IQ4NL32_BAR_REL)
    }
}
