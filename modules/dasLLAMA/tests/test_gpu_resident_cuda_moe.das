options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE030 — the umbrella's [init] registrations arm the MoE archs and the CUDA home
require dasllama/dasllama_common
require daslib/jobque_boost
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_cuda_decode   // CUDA_MMQ_WINNERS: the pinned MMQ route
require _model_tier
require _resident_feed
require _cuda_feed
require _moe_resident   // MOE_LOGIT_BAR_REL: the routed chain's CPU-vs-device noise class

// The CUDA home's whole-model driver on the routed block (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the window chain's routed
// block (the router GEMM, the per-row select, the bucket schedule, the f16 gather, the expert tiles on the e and s columns
// or the schedule-fed FP4 leaf, the combine on the residual step) and the token step's (the router GEMV, the top-k writing
// the expert GEMVs' slot regions, the combine), in the forced-feed form the CUDA files share (`_cuda_feed.das`) at the
// Vulkan MoE file's bar (`test_gpu_resident_moe.das`). Four fixtures: the Qwen1.5-MoE Q4_K_M mint - a gated shared expert
// beside K-quant stacks - and the Q8_0 carrier it is minted from (the Q8_0 expert path, one window, the MMQ route pinned
// `CUDA_MMQ_WINNERS`), the Qwen3-30B UD-IQ2_XXS, the MoE with no shared expert (the residual step's add partner off, the
// experts' feed the FFN norm's own), and gpt-oss-20b, the biased softmax router over native-MXFP4 stacks on the
// schedule-fed FP4 leaf (the MMQ route pinned `CUDA_MMQ_WINNERS`). Every cell skips off the JIT, without DASLLAMA_CUDA=1,
// without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(), without
// DASLLAMA_PARITY_FULL=1 (every carrier sits past the large tier's bytes), or where the prompt window is not
// PF_WINDOW_ROWS rows; the gpt-oss cells also skip on a card below the FP4 rung (`FP4_RUNG_SM`, compute capability 120).

let private FILE_TWIN = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"   //! 60 experts, 4 routed, a gated shared expert, K-quant stacks (the Vulkan MoE file's twin)
let private FILE_Q8 = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"   //! the carrier the twin is minted from: the same block over Q8_0 expert stacks (the Q8_0 act + requant, the q8 region GEMV, the q8 e column)
let private FILE_NO_SHEXP = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf"   //! 128 experts, 8 routed, no shared expert, the renormalized softmax, iq2xxs stacks on their own e column
let private FILE_GPTOSS = "gpt-oss-20b-mxfp4.gguf"   //! 32 experts, 4 routed, the biased softmax router, native-MXFP4 stacks
let private GPTOSS_BAR_REL = MOE_LOGIT_BAR_REL   //! held at `MOE_LOGIT_BAR_REL`, not the Vulkan gpt-oss file's 0.05 (the bar sits near 1.6 absolute on this file's rows): the rows' max distance reads 0.06-0.20 absolute, and one step in twelve a router near-tie flips under this home's summation order to 0.47-0.50, argmax intact; the one-step-off control reads 4.4-19 (RTX 5060 Ti, `-jit`, the MMQ pin on; the 2026-10-26 PERF_LEDGER entry)

[test]
def test_gpu_resident_cuda_moe(t : T?) {
    t |> run("{FILE_TWIN}: one window (40 tokens) and eight steps on the CUDA driver - the routed block beside the gated shared expert") @(t : T?) {
        cuda_forced_feed(t, FILE_TWIN, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL)
    }
    t |> run("{FILE_TWIN}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_TWIN, "qwen2moe", PF_WINDOW_ROWS + 88l, 2l, MOE_LOGIT_BAR_REL)
    }
}

//! the Q8_0 expert path on this home against the CPU chain, at the Vulkan MoE file's bar
[test]
def test_gpu_resident_cuda_moe_q8(t : T?) {
    t |> run("{FILE_Q8}: one window (40 tokens) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}) - the routed block over Q8_0 expert stacks beside the gated shared expert") @(t : T?) {
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_Q8, "qwen2moe", 40l, 1l, MOE_LOGIT_BAR_REL)
        }
    }
}

[test]
def test_gpu_resident_cuda_moe_no_shexp(t : T?) {
    t |> run("{FILE_NO_SHEXP}: one window (40 tokens) and eight steps on the CUDA driver - the routed block alone at the FFN site") @(t : T?) {
        cuda_forced_feed(t, FILE_NO_SHEXP, "qwen3moe", 40l, 1l, MOE_LOGIT_BAR_REL)
    }
    t |> run("{FILE_NO_SHEXP}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver") @(t : T?) {
        cuda_forced_feed(t, FILE_NO_SHEXP, "qwen3moe", PF_WINDOW_ROWS + 88l, 2l, MOE_LOGIT_BAR_REL)
    }
}

//! the window runs the schedule-fed FP4 leaf: the cells pin the MMQ route on and skip below the FP4 rung, where the window
//! keeps the s and e tiles
[test]
def test_gpu_resident_cuda_gptoss(t : T?) {
    t |> run("{FILE_GPTOSS}: one window (40 tokens) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}) - the mx4 routed block on the schedule-fed FP4 leaf, the biases, the sinks") @(t : T?) {
        return if (!fp4_rung_on(t))
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_GPTOSS, "gpt-oss", 40l, 1l, GPTOSS_BAR_REL)
        }
    }
    t |> run("{FILE_GPTOSS}: a {PF_WINDOW_ROWS + 88l}-token prompt (two windows) and eight steps on the CUDA driver under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}) - the sliding layers past their window, the FP4 leaf") @(t : T?) {
        return if (!fp4_rung_on(t))
        with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
            cuda_forced_feed(t, FILE_GPTOSS, "gpt-oss", PF_WINDOW_ROWS + 88l, 2l, GPTOSS_BAR_REL)
        }
    }
}
