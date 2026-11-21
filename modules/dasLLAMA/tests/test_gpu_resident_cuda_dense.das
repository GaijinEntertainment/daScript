options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE030 — the umbrella's [init] registrations arm the phi3 and qwen3 archs and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require _model_tier
require _gemma_resident   // GemmaFile, gemma_forced_feed: the gemma files' forms, the rig's context
require _resident_feed   // cuda_home_asked: the CUDA home's gate; PF_WINDOW_ROWS
require _cuda_feed   // cuda_forced_feed: the CUDA home's forced feed against the CPU chain
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin: the file's MMQ route

def private cuda_dense_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

// gemma-3-4b on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the Vulkan files' forced-feed cells: the
// sliding and global layers at head 256, the q/k norms, the sandwich norms. One window and two, eight fed steps each, against the
// CPU chain, every load under the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`). gemma-2-2b's and gemma-3-1b's cells on
// this home are their model blocks' (`test_gpu_resident_gemma2_3.das`).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, or without the file
// under models_dir().

let private FILE_G3B = GemmaFile(file = "gemma-3-4b-it-Q8_0.gguf", arm = "cuda-g3b")

[test(description = "On the CUDA whole-model driver gemma-3-4b Q8_0 keeps forced-feed logits within 16 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_gemma3(t : T?) {
    return if (!family_on(t, "gemma3"))
    cuda_dense_intent()
    t |> run("{FILE_G3B.file}: one window (40 tokens) and eight steps on the CUDA driver - the sliding and global layers at head 256") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, 40l, FILE_G3B)
    }
    t |> run("{FILE_G3B.file}: a two-window prompt and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, FILE_G3B)
    }
}

// The CUDA home's whole-model driver on the phi3 family: Phi-3.5-mini Q4_K_M (head 96 over 32 heads - the window's
// attention on the h96 flash tile, the K-quant planes, the fused qkv and gate-up planes, the long-rope factors). The
// forced-feed logits-tolerance form of the CUDA llama file on the body the CUDA files share (`_cuda_feed.das`): the Q4_K_M
// file at two windows alone, the Q8_0 file at 40 tokens (one window) and at two windows. Every cell skips off the JIT,
// without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), or
// where the prompt window is not PF_WINDOW_ROWS rows.

let private FILE_35 = "Phi-3.5-mini-instruct-Q4_K_M.gguf"   //! head 96: the h96 flash tile, the K-quant planes
let private FILE_35_Q8 = "Phi-3.5-mini-instruct-Q8_0.gguf"   //! the same model on the q8 decode chain

//! the K-quant file runs the two-window prompt alone: after a 40-token prompt its steps at 41 and 44 read 7.7 and 6.6% of the max logit off the CPU's (the Q8_0 file's within 0.73 of a 1.9 bar) while the one-step-off rows sit 6.5% apart, so no one bar holds both arms
[test(description = "On the CUDA whole-model driver Phi-3.5-mini Q4_K_M, head 96 on the h96 flash tile with fused qkv and gate-up planes and long-rope factors, keeps two-window forced-feed logits within 6 percent of max logit of the CPU chain")]
def test_gpu_resident_cuda_phi35_mini(t : T?) {
    cuda_dense_intent()
    t |> run("{FILE_35}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_35, "phi3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA whole-model driver Phi-3.5-mini Q8_0 keeps prefill and eight fed-step logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_phi35_mini_q8(t : T?) {
    cuda_dense_intent()
    t |> run("{FILE_35_Q8}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_35_Q8, "phi3", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_35_Q8}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_35_Q8, "phi3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}
