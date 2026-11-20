options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // GemmaFile, ppl_compare, GEMMA_MOE_PPL_RATIO: the gemma files' perplexity form, the rig's context
require _resident_feed   // cuda_home_asked: the CUDA home's gate
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin: the file's MMQ route

// The gemma-4 routed form on the CUDA home's whole-model driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`): the layer's own FFN triple as
// the dense branch, the routed feed under its own pre norm, the router over x under its scale row, the picks' weights under the
// per-expert down scales, the combine under the four norms (`../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block`), in the
// Vulkan 26B files' perplexity form (token parity is no instrument on the 26B: a router near-tie flips whole positions). Two
// carriers, large tier: the UD-IQ3_XXS file, which a 16 GB card holds whole, and the Q4_K_M file, which it does not - the plan
// moves the first routed layers' experts to the host, so the split command, the cut window chain and the pool serve the form.
// The dense gemma-4-12B opens the file in the Vulkan 12B file's agreement form (the window chain against the token command, the
// CPU chain's own pair the yardstick - on a K-quant file the CPU chain's prefill and decode land far apart): gemma-4's global
// layers take V off the raw k row at heads of 512 (the wide-head batch attention on the window) and its sliding layers scale V
// by its own rms. Every load runs under the MMQ route pinned `CUDA_MMQ_WINNERS` (the file's `[init]`): the K-quant window
// sites on the int8 leaves, the MXFP4 stacks on the FP4 rung where the card carries it.
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without the file
// under models_dir(), or without DASLLAMA_PARITY_FULL=1. The E2B's cells on this home are its model blocks'
// (`test_gpu_resident_gemma4_e2b.das`).

def private cuda_gemma4_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

let private FILE_12B = GemmaFile(file = "gemma-4-12B-it-Q4_K_M.gguf", arm = "cuda-g4k")   //! the dense gemma-4: the global layers' heads of 512 take V off the raw k row, the sliding layers' V its own rms
let private FILE_26B = GemmaFile(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", arm = "cuda-g4m")
let private FILE_26B_K = GemmaFile(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", arm = "cuda-g4mk", host_experts = true)   //! the rig's context asked whole: the plan moves routed layers' experts to the host
let private FILE_12B_Q8 = GemmaFile(file = "gemma-4-12B-it-Q8_0.gguf", arm = "cuda-g4q8")   //! the dense carrier on the Q8_0 planes (the Vulkan 12B Q8_0 file's cells)
let private FILE_26B_MX4 = GemmaFile(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", arm = "cuda-g4mx")   //! the native-MXFP4 expert stacks under gemma-4's GeGLU and per-expert down scale (`../performance/model_specs.das`)
//! the Vulkan 12B Q8_0 file's two-window ratio: three implementations read three perplexities over the same 80 targets there, so
//! the bar measures the reference, not the driver
let private GEMMA4_Q8_TWO_WINDOW_PPL_RATIO = 1.3
let private HITS_SLACK = 12l   //! the Vulkan 26B files' slack: a router near-tie flips whole positions between the arms

[test(description = "On the CUDA whole-model driver gemma-4-12B Q4_K_M's window chain and token command agree over 8 and 300 tokens within 6x the CPU chain's own prefill-versus-decode gap, covering V-from-K 512-wide global heads and v-normed sliding layers")]
def test_gpu_resident_cuda_gemma4_12b(t : T?) {
    return if (!family_on(t, "gemma4"))
    cuda_gemma4_intent()
    t |> run("gemma-4-12B Q4_K_M: eight tokens through the window chain against the token command on the CUDA driver, the CPU chain's own pair the yardstick - the V-from-K global layers on the wide-head window attention, the v-normed sliding layers") @(t : T?) {
        return if (!cuda_home_asked(t))
        prefill_vs_decode(t, 8l, FILE_12B)
    }
    t |> run("gemma-4-12B Q4_K_M: the same at 300 tokens - the wide heads over many keys on both paths") @(t : T?) {
        return if (!cuda_home_asked(t))
        prefill_vs_decode(t, 300l, FILE_12B)
    }
}

//! the dense gemma-4 on the Q8_0 planes: the Vulkan 12B Q8_0 file's three cells on the CUDA driver (a 16 GB card arms it under
//! DASLLAMA_GPU_MIN_CTX=1024)
[test(description = "On the CUDA driver gemma-4-12B Q8_0 keeps one-window forced-feed logits within 16 percent of max logit of the CPU chain, and its teacher-forced perplexity within 1.05x of the CPU chain's over 150 positions and 1.3x past two windows")]
def test_gpu_resident_cuda_gemma4_12b_q8(t : T?) {
    return if (!family_on(t, "gemma4"))
    cuda_gemma4_intent()
    t |> run("gemma-4-12B Q8_0: resident prefill + decode on the CUDA driver - v-norm, V-from-K 512-heads, the layer scale, the capped and pinned logits - vs the CPU chain, one window") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, 40l, FILE_12B_Q8)
    }
    t |> run("gemma-4-12B Q8_0: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_12B_Q8)
    }
    t |> run("gemma-4-12B Q8_0: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them") @(t : T?) {
        return if (!cuda_home_asked(t))
        assert_pf_window(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, FILE_12B_Q8, 3l, GEMMA4_Q8_TWO_WINDOW_PPL_RATIO)
    }
}

//! the native-MXFP4 routed rail on the CUDA driver: the Vulkan MXFP4_MOE file's two cells - every site in its own format, the
//! expert stacks on the mx4 stamps, and the perplexity form at the routed model's slack
[test(description = "On the CUDA driver gemma-4-26B MXFP4_MOE serves every site in its own format with the native-MXFP4 expert stacks on the mx4 stamps and every call served, and its teacher-forced perplexity stays within 2x of the CPU chain's")]
def test_gpu_resident_cuda_gemma4_26b_mx4(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    cuda_gemma4_intent()
    t |> run("gemma-4-26B-A4B MXFP4_MOE: every site in its own format - the native-MXFP4 expert stacks among them - and the CUDA driver serving every call") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_format_cell(t, FILE_26B_MX4, true)
    }
    t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_26B_MX4, HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}

[test(description = "On the CUDA driver the routed gemma-4-26B UD-IQ3_XXS, held whole on the card, scores a teacher-forced perplexity over 150 prose positions within 2x of the CPU chain's, with the routed slack on hits")]
def test_gpu_resident_cuda_gemma4_26b(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    cuda_gemma4_intent()
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the gemma-4 routed form on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_26B, HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}

[test(description = "On the CUDA driver gemma-4-26B Q4_K_M at the full rig context, with the first routed layers' experts moved to the host, scores a teacher-forced perplexity over 150 positions within 2x of the CPU chain's through the split command and cut window chain")]
def test_gpu_resident_cuda_gemma4_26b_host(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    cuda_gemma4_intent()
    t |> run("gemma-4-26B-A4B Q4_K_M with the rig's context asked whole: the first routed layers' experts on the host, 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the split command and the cut window chain under the gemma-4 form") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_26B_K, HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}
