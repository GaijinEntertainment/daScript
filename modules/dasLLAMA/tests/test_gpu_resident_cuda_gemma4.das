options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident   // GemmaFile, ppl_compare, GEMMA_MOE_PPL_RATIO: the gemma files' perplexity form, the rig's context
require _resident_feed   // cuda_home_asked: the CUDA home's gate
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cell's mirror ask
require dasllama/dasllama_common   // forward, forward_prefill
require dasllama/dasllama   // create_session
require dasllama/dasllama_ple   // ple_cpu_pre_rows, ple_host_gather_rows - the CPU pre-step's and the host gather's witnesses
require dasllama/dasllama_gpu_resident   // with_gpu_resident_embed, moe_gpu_resident_prefills
require dasllama/dasllama_cuda_decode   // cud_rdec_side_gather_on, cud_rdec_side_gathered, cud_rdec_ple_side_row - the device pre-step's witnesses; set_cuda_mmq_pin: the file's MMQ route
require dasllama/dasllama_core   // set_resident_prefill_allowed / resident_prefill_allowed: the device prefill's off leg

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
// under models_dir(), or without DASLLAMA_PARITY_FULL=1; the block-codec cells also skip under DASLLAMA_VK_KV32=1, which arms
// the f32 mirror over the codec asked.

[init]
def private cuda_gemma4_intent() {
    set_cuda_mmq_pin(CUDA_MMQ_WINNERS)   // the MMQ route every load of the file measures
}

let private FILE_12B = GemmaFile(file = "gemma-4-12B-it-Q4_K_M.gguf", arm = "cuda-g4k")   //! the dense gemma-4: the global layers' heads of 512 take V off the raw k row, the sliding layers' V its own rms
let private FILE_26B = GemmaFile(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", arm = "cuda-g4m")
let private FILE_26B_K = GemmaFile(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", arm = "cuda-g4mk", host_experts = true)   //! the rig's context asked whole: the plan moves routed layers' experts to the host
let private FILE_E2B = GemmaFile(file = "gemma-4-E2B-it-Q8_0.gguf", arm = "cuda-g4e")   //! the E-series: the per-layer-embedding branch on every layer, the upper layers over the K/V of the ones below
let private FILE_E4B = GemmaFile(file = "gemma-4-E4B-it-Q8_0.gguf", arm = "cuda-g4f")   //! the larger E-series carrier: 42 layers at dim 2560, the bf16 projection rows twice the E2B's
let private FILE_12B_Q8 = GemmaFile(file = "gemma-4-12B-it-Q8_0.gguf", arm = "cuda-g4q8")   //! the dense carrier on the Q8_0 planes (the Vulkan 12B Q8_0 file's cells)
let private FILE_31B = GemmaFile(file = "gemma-4-31B-it-Q4_K_M.gguf", arm = "cuda-g31")   //! the largest dense carrier, a 32 GB card
let private FILE_26B_MX4 = GemmaFile(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", arm = "cuda-g4mx")   //! the native-MXFP4 expert stacks under gemma-4's GeGLU and per-expert down scale (`../performance/model_specs.das`)
//! the Vulkan 12B Q8_0 file's two-window ratio: three implementations read three perplexities over the same 80 targets there, so
//! the bar measures the reference, not the driver
let private GEMMA4_Q8_TWO_WINDOW_PPL_RATIO = 1.3
let private HITS_SLACK = 12l   //! the Vulkan 26B files' slack: a router near-tie flips whole positions between the arms

[test]
def test_gpu_resident_cuda_gemma4_12b(t : T?) {
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
[test]
def test_gpu_resident_cuda_gemma4_12b_q8(t : T?) {
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

//! the largest dense carrier (a 32 GB card): the Vulkan 31B file's one cell on the CUDA driver
[test]
def test_gpu_resident_cuda_gemma4_31b(t : T?) {
    t |> run("gemma-4-31B Q4_K_M: eight tokens through the window chain against the token command on the CUDA driver, both arms") @(t : T?) {
        return if (!cuda_home_asked(t))
        prefill_vs_decode(t, 8l, FILE_31B)
    }
}

//! the native-MXFP4 routed rail on the CUDA driver: the Vulkan MXFP4_MOE file's two cells - every site in its own format, the
//! expert stacks on the mx4 stamps, and the perplexity form at the routed model's slack
[test]
def test_gpu_resident_cuda_gemma4_26b_mx4(t : T?) {
    t |> run("gemma-4-26B-A4B MXFP4_MOE: every site in its own format - the native-MXFP4 expert stacks among them - and the CUDA driver serving every call") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_format_cell(t, FILE_26B_MX4, true)
    }
    t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_26B_MX4, HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}

//! the E-series on the CUDA home: the per-layer-embedding branch (the pre-step's side rows gathered off the ids and finished on the
//! device, the gate and projection GEMVs around the strided act on the token command and the window) and the shared-K/V layers (q
//! alone, the donor's mirror rows), the Vulkan E2B file's cells
[test]
def test_gpu_resident_cuda_gemma4_e2b(t : T?) {
    t |> run("gemma-4-E2B Q8_0: resident prefill + decode on the CUDA driver - the per-layer-embedding branch, the shared-KV layers - vs the CPU chain, forty tokens") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, 40l, FILE_E2B)
    }
    t |> run("gemma-4-E2B Q8_0: 150 prose ids prefilled, 150 teacher-forced on the CUDA driver, scored as a perplexity against the CPU chain") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_E2B)
    }
    t |> run("gemma-4-E2B Q8_0: eight tokens through the window chain against the token command on the CUDA driver, both arms") @(t : T?) {
        return if (!cuda_home_asked(t))
        prefill_vs_decode(t, 8l, FILE_E2B)
    }
}

//! the E2B on the block-codec mirrors (the Vulkan file's cells): a q8_0 mirror in the forced-feed form - the shared-KV layers store
//! no row of their own - and a tq4 mirror scored as a perplexity - a shared-KV layer rotates its query and stores nothing
[test]
def test_gpu_resident_cuda_gemma4_e2b_block_codecs(t : T?) {
    t |> run("gemma-4-E2B Q8_0: a q8_0 K/V mirror on the CUDA driver - the shared-KV layers store no row of their own - vs the CPU chain, forty tokens") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            gemma_forced_feed(t, 40l, FILE_E2B)
        }
    }
    t |> run("gemma-4-E2B Q8_0: a tq4 K/V mirror on the CUDA driver - a shared-KV layer rotates its query and stores nothing - scored as a perplexity against the CPU chain") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, 150l, 150l, FILE_E2B)
        }
    }
}

//! the E4B on the CUDA home (the Vulkan `g4f` file's cells): the E-series arms on the larger carrier, and its tq4 mirror - a
//! shared-K/V layer rotates its query and stores nothing - scored as a perplexity
[test]
def test_gpu_resident_cuda_gemma4_e4b(t : T?) {
    t |> run("gemma-4-E4B Q8_0: resident prefill + decode on the CUDA driver vs the CPU chain, forty tokens - the E-series arms on the larger carrier") @(t : T?) {
        return if (!cuda_home_asked(t))
        gemma_forced_feed(t, 40l, FILE_E4B)
    }
    t |> run("gemma-4-E4B Q8_0: 150 prose ids prefilled, 150 teacher-forced on the CUDA driver, scored as a perplexity against the CPU chain") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_E4B)
    }
    t |> run("gemma-4-E4B Q8_0: a tq4 K/V mirror on the CUDA driver - a shared-KV layer rotates its query and stores nothing - scored as a perplexity against the CPU chain") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            ppl_compare(t, 150l, 150l, FILE_E4B)
        }
    }
}

//! the pre-step on the CUDA home (the Vulkan `g4e-pre` cell's form): the window's last side row, projected on the f16 tile, and
//! the side row the one-row command gathered off the step's token id, projected on the router GEMV, each finished on the device;
//! then a load with the gather off, whose step brings the host's gathered row for the device to finish (the counter's control);
//! and on the first load a prompt under the server's prefill pin, which the window chain never serves (the prefill counter's control)
[test]
def test_gpu_resident_cuda_gemma4_e2b_ple_pre_step(t : T?) {
    t |> run("gemma-4-E2B Q8_0: the step's side row built on the CUDA device is the CPU pre-step's row, gathered off the id and off the host's row") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_gpu_resident_embed(1) {   // the step gathers its row off its token id
            with_gemma_rig(t, FILE_E2B) $(tr) {
                t |> success(cud_rdec_side_gather_on(), "the driver gathers the side rows off the ids (the table and the projection on the device)")
                e2b_pre_step_row(t, tr, 1l, 0l, "gathered off the id")
                prefill_pinned_off_serves_none(t, tr)
            }
        }
        with_gpu_resident_embed(0) {   // the host gathers the table row and stages it; the device projects and finishes
            with_gemma_rig(t, FILE_E2B) $(tr) {
                t |> success(!cud_rdec_side_gather_on(), "with the gather off the driver takes the host's gathered rows")
                e2b_pre_step_row(t, tr, 0l, 1l, "off the host's gathered row")
            }
        }
    }
}

//! a 40-row prompt, then one step on token 1234: `gathered` side rows gathered on the device and `host_rows` table rows gathered
//! on the host, no CPU pre-step row, and both device side rows within `side_row_within`'s bar
def private e2b_pre_step_row(t : T?; tr : Model; gathered, host_rows : int64; what : string) {
    var prompt <- resident_prompt(40l)
    var sg <- create_session(tr, mirror_kv_dtype())
    let served0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    forward_prefill(tr, sg, prompt, 40l, 0l)
    t |> equal(gpu_resident_prefills_(), served0 + 1l, "{what}: the resident window chain served the prompt")
    side_row_within(t, cud_rdec_pf_side_row(39l), tr, prompt[39], "{what}: the window's last row", 1e-3)
    let tok = 1234l
    let g0 = cud_rdec_side_gathered()
    let cpu0 = ple_cpu_pre_rows()
    let host0 = ple_host_gather_rows()
    forward(tr, sg, tok, 40l)
    t |> equal(cud_rdec_side_gathered() - g0, gathered, "{what}: {gathered} side row gathered off the step's token id")
    t |> equal(ple_host_gather_rows() - host0, host_rows, "{what}: {host_rows} table row gathered on the host")
    t |> equal(ple_cpu_pre_rows() - cpu0, 0l, "{what}: the step built no CPU pre-step row")
    served_every_call(t)
    cuda_served_since(t, c0, 41l, "{what}: the prompt and the step")
    side_row_within(t, cud_rdec_ple_side_row(), tr, tok, "{what}: the step's row", 1e-3)
    delete sg
    delete prompt
}

//! the prefill counter's control: a prompt under the server's prefill pin (`set_resident_prefill_allowed(false)`, put back) passes
//! to the CPU rails as `pinned_off`, and the window chain's count stays
def private prefill_pinned_off_serves_none(t : T?; tr : Model) {
    var prompt <- resident_prompt(40l)
    var sc <- create_session(tr, mirror_kv_dtype())
    let served0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    let pinned0 = pass_count("pinned_off")
    let allowed0 = resident_prefill_allowed()
    set_resident_prefill_allowed(false)
    forward_prefill(tr, sc, prompt, 40l, 0l)
    set_resident_prefill_allowed(allowed0)
    t |> equal(gpu_resident_prefills_(), served0, "control: under the server's prefill pin the window chain served no prompt - its counter stays")
    t |> success(pass_count("pinned_off") - pinned0 >= 1l, "control: the prompt passed to the CPU rails as pinned_off ({pass_count("pinned_off") - pinned0} counted)")
    cuda_served_none(t, c0, "control: the pinned-off prompt")
    delete sc
    delete prompt
}

[test]
def test_gpu_resident_cuda_gemma4_26b(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the gemma-4 routed form on the CUDA driver") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_26B, HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}

[test]
def test_gpu_resident_cuda_gemma4_26b_host(t : T?) {
    t |> run("gemma-4-26B-A4B Q4_K_M with the rig's context asked whole: the first routed layers' experts on the host, 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the split command and the cut window chain under the gemma-4 form") @(t : T?) {
        return if (!cuda_home_asked(t))
        ppl_compare(t, 150l, 150l, FILE_26B_K, HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}
