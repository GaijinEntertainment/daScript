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
require _gemma_resident   // GemmaFile, gemma_forced_feed, prefill_vs_decode: the gemma files' forms, the rig's context
require _resident_feed   // cuda_home_asked: the CUDA home's gate; PF_WINDOW_ROWS
require _cuda_feed   // cuda_forced_feed: the CUDA home's forced feed against the CPU chain
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin: the file's MMQ route

def private cuda_dense_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

// The gemma-2 and gemma-3 families on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the Vulkan files'
// forced-feed cells: gemma-2-2b (the attention softcap on every layer's scores - the window's h256 flash tile under its cap stamp,
// the token step's attention under its cap; the final logit softcap), gemma-3-1b and gemma-3-4b (the sliding and global layers at
// head 256, the q/k norms, the sandwich norms). One window and two, eight fed steps each, against the CPU chain, every load under
// the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, or without the file
// under models_dir().

let private FILE_G2 = GemmaFile(file = "gemma-2-2b-it-Q8_0.gguf", arm = "cuda-g2")   //! the attention softcap: the h256 cap stamps on the window
let private FILE_G3 = GemmaFile(file = "gemma-3-1b-it-Q8_0.gguf", arm = "cuda-g3")
let private FILE_G3B = GemmaFile(file = "gemma-3-4b-it-Q8_0.gguf", arm = "cuda-g3b")

[test(description = "On the CUDA whole-model driver gemma-2-2b Q8_0 with softcapped attention keeps prefill and eight fed-step logits within 16 percent of max logit of the CPU chain over one and two windows, one-step-off controls past the bar")]
def test_gpu_resident_cuda_gemma2(t : T?) {
    cuda_dense_intent()
    t |> run("gemma-2-2b Q8_0: one window (40 tokens) and eight steps on the CUDA driver - the softcapped attention on both commands") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, 40l, FILE_G2)
    }
    t |> run("gemma-2-2b Q8_0: a two-window prompt and eight steps on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, FILE_G2)
    }
}

[test(description = "On the CUDA whole-model driver gemma-3-1b and 4b Q8_0 keep forced-feed logits within 16 percent of max logit of the CPU chain over one and two windows, and the window chain against the token command stays within its ratio of the CPU's own two paths")]
def test_gpu_resident_cuda_gemma3(t : T?) {
    cuda_dense_intent()
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

// The qwen2 family on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the Vulkan qwen2 file's forced-feed
// cells: Qwen2.5-0.5B Q8_0 carries the q/k/v projection bias (the rope-and-store kernels' bias arm), and the IQ4_NL `-local` mint of
// it (`../performance/model_specs.das`) the per-32 iq4nl32 planes on the dense sites. One window and two, eight fed steps each,
// every load under the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without the file
// under models_dir(), or where the prompt window is not PF_WINDOW_ROWS rows.

let private FILE_Q2 = "Qwen2.5-0.5B-Instruct-Q8_0.gguf"
let private FILE_Q2_IQ4NL32 = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf"
let private QWEN2_BAR_REL = 0.06   //! the Vulkan qwen2 file's bar
let private IQ4NL32_BAR_REL = 0.08   //! ... and its per-32 carrier's

[test(description = "On the CUDA whole-model driver Qwen2.5-0.5B Q8_0, carrying the q/k/v projection bias on both commands, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_qwen2_bias(t0 : T?) {
    cuda_dense_intent()
    t0 |> run("{FILE_Q2}: one window (40 tokens) and eight steps on the CUDA driver - the q/k/v bias on both commands") @(t : T?) {
        cuda_forced_feed(t, FILE_Q2, "qwen2", 40l, 1l, QWEN2_BAR_REL)
    }
    t0 |> run("{FILE_Q2}: a two-window prompt and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_Q2, "qwen2", PF_WINDOW_ROWS + 88l, 2l, QWEN2_BAR_REL)
    }
}

[test(description = "On the CUDA whole-model driver an IQ4_NL Qwen2.5-0.5B on per-32 iq4nl32 dense planes keeps one-window prefill and eight fed-step logits within 8 percent of max logit of the CPU chain")]
def test_gpu_resident_cuda_qwen2_iq4nl32(t0 : T?) {
    cuda_dense_intent()
    t0 |> run("{FILE_Q2_IQ4NL32}: one window (40 tokens) and eight steps on the CUDA driver - the iq4nl32 dense planes") @(t : T?) {
        cuda_forced_feed(t, FILE_Q2_IQ4NL32, "qwen2", 40l, 1l, IQ4NL32_BAR_REL)
    }
}

// The CUDA home's whole-model driver on Qwen3-0.6B and Qwen3-4B Q8_0 (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the q/k-normed
// carriers, whose norm, rope and K/V store run as the fused pass `CuQknRopeKv` in the token command and in the window
// chain, through the forced-feed form the CUDA files share (`_cuda_feed.das`). Every cell skips off the JIT, without
// DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), or where
// the prompt window is not PF_WINDOW_ROWS rows.

let private FILE_06B = "Qwen3-0.6B-Q8_0.gguf"   //! head 128 over 16 q heads and 8 k heads, the per-head q/k norm, no attention bias
let private FILE_4B = "Qwen3-4B-Instruct-2507-Q8_0.gguf"   //! the larger dense qwen3: 36 layers over 2560, the same head geometry at 32 q heads

[test(description = "On the CUDA whole-model driver Qwen3-0.6B Q8_0, its q/k norm, rope and K/V store as the fused CuQknRopeKv pass, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_qwen3_06b(t : T?) {
    cuda_dense_intent()
    t |> run("{FILE_06B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_06B, "qwen3", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_06B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_06B, "qwen3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}

[test(description = "On the CUDA whole-model driver Qwen3-4B Q8_0 with the fused q/k-norm rope pass keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows")]
def test_gpu_resident_cuda_qwen3_4b(t : T?) {
    cuda_dense_intent()
    t |> run("{FILE_4B}: one window (40 tokens) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_4B, "qwen3", 40l, 1l, LOGIT_BAR_REL)
    }
    t |> run("{FILE_4B}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_4B, "qwen3", PF_WINDOW_ROWS + 88l, 2l, LOGIT_BAR_REL)
    }
}
