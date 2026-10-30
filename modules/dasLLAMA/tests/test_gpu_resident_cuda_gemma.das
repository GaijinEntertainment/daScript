options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // GemmaFile, gemma_forced_feed, prefill_vs_decode: the gemma files' forms, the rig's context
require _resident_feed   // cuda_home_asked: the CUDA home's gate
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin: the file's MMQ route

// The gemma-2 and gemma-3 families on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the Vulkan files'
// forced-feed cells: gemma-2-2b (the attention softcap on every layer's scores - the window's h256 flash tile under its cap stamp,
// the token step's attention under its cap; the final logit softcap), gemma-3-1b and gemma-3-4b (the sliding and global layers at
// head 256, the q/k norms, the sandwich norms). One window and two, eight fed steps each, against the CPU chain, every load under
// the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, or without the file
// under models_dir().

[init]
def private cuda_gemma_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

let private FILE_G2 = GemmaFile(file = "gemma-2-2b-it-Q8_0.gguf", arm = "cuda-g2")   //! the attention softcap: the h256 cap stamps on the window
let private FILE_G3 = GemmaFile(file = "gemma-3-1b-it-Q8_0.gguf", arm = "cuda-g3")
let private FILE_G3B = GemmaFile(file = "gemma-3-4b-it-Q8_0.gguf", arm = "cuda-g3b")

[test]
def test_gpu_resident_cuda_gemma2(t : T?) {
    t |> run("gemma-2-2b Q8_0: one window (40 tokens) and eight steps on the CUDA driver - the softcapped attention on both commands") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, 40l, FILE_G2)
    }
    t |> run("gemma-2-2b Q8_0: a two-window prompt and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, FILE_G2)
    }
}

[test]
def test_gpu_resident_cuda_gemma3(t : T?) {
    for (gf in [FILE_G3, FILE_G3B]) {
        t |> run("{gf.file}: one window (40 tokens) and eight steps on the CUDA driver - the sliding and global layers at head 256") @(t : T?) {
            return if (!cuda_home_asked(t))
            gemma_forced_feed(t, 40l, gf)
        }
        t |> run("{gf.file}: a two-window prompt and eight steps on the CUDA driver") @(t : T?) {
            return if (!cuda_home_asked(t))
            gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, gf)
        }
    }
    t |> run("gemma-3-1b Q8_0: eight tokens through the window chain against the token command on the CUDA driver, both arms") @(t : T?) {
        return if (!cuda_home_asked(t))
        prefill_vs_decode(t, 8l, FILE_G3)
    }
}
