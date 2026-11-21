options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX, LogitsBar
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms
require _resident_feed   // with_cuda_mmq_winners, Held
require dasllama/dasllama_common   // Model
require dasllama/dasllama_gpu_tier   // gpu_want_cuda: the CUDA home's arms

// gemma-4-26B-A4B UD-IQ3_XXS (the routed MoE with its parallel dense shared expert and the routed branch's own norms):
// arm `g4m`. Each file's format census runs on its cut, the first six blocks (one global layer, the cap lifted); the
// perplexity cells read real text and stay on the whole files (the `gemma4-moe` category). Skips off the JIT, without
// DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or when the residency plan
// does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", arm = "g4m")
let private GEMMA4_26B_CUT = GemmaFile(file = "gemma-4-26B-A4B-6L-UD-IQ3_XXS-local.gguf", arm = "g4m")
//! the routed model's argmax slack: a near-tie at the router flips whole positions between the arms, and the CPU chain
//! sits farther from the reference exe on this model than the resident does (its own forced-feed logits are no instrument
//! here); the hits read 126 to 139 against the CPU chain's 133 across kernel recompiles (the readings: CLAUDE.md)
let private GEMMA4_26B_HITS_SLACK = 12l

[test(description = "On the whole-model driver the routed gemma-4-26B UD-IQ3_XXS scores teacher-forced perplexity within 2x of the CPU chain's over one and two windows, hits within 12")]
def test_gpu_resident_gemma4_26b(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the routed block's gemma-4 form") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}

[test(description = "On the whole-model driver the routed gemma-4-26B-A4B-6L UD-IQ3_XXS serves every site in its own format with every call on the device")]
def test_gpu_resident_gemma4_26b_format(t : T?) {
    t |> run("gemma-4-26B-A4B-6L IQ3_XXS: every site in its own format - the expert stacks and the token table among them - and the driver serving every call") @(t : T?) {
        return if (!family_on(t, "gemma4moe"))
        gemma_format_cell(t, GEMMA4_26B_CUT, false)
    }
}

// gemma-4-26B-A4B Q4_K_M (the routed MoE in the model table's official quant: k4 gate and up expert stacks beside native
// Q5_1 down stacks - the q51 expert rail - with the dense triple and the attention head on the K-quant rails): arm `g4mk`.
// Its cut's cells load each form once (`test_gpu_resident_gemma4_26b_k_cut`, below). Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the
// file under models_dir(), or when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_K_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", arm = "g4mk")
let private GEMMA4_26B_K_CUT = GemmaFile(file = "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", arm = "g4mk")
//! the routed model's argmax slack: a router near-tie flips whole positions between the arms, and a kernel recompile alone
//! moves the hits - 122 to 128 against the CPU chain's 134 over the 150-position cell (the readings: CLAUDE.md)
let private GEMMA4_26B_K_HITS_SLACK = 16l
//! the two-window cell's band: a k4 fold's rounding pattern alone moved it 1.70 -> 2.44 against the CPU chain's 1.03 (a router near-tie flips whole positions), the 150 + 150 cell 5.51 -> 5.91 against 3.36
let private GEMMA4_26B_K_PPL_RATIO_TWO_WINDOWS = 2.5

[test(description = "On the whole-model driver gemma-4-26B Q4_K_M with q51 down experts scores teacher-forced perplexity within 2x of the CPU chain over 150 positions and 2.5x over two windows, hits within 16")]
def test_gpu_resident_gemma4_26b_k(t : T?) {
    t |> run("gemma-4-26B-A4B Q4_K_M: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the q51 down experts on their own rail") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B Q4_K_M: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK, GEMMA4_26B_K_PPL_RATIO_TWO_WINDOWS)
    }
}

// gemma-4-26B-A4B MXFP4_MOE (the Q4_K_M file requantized, `../performance/model_specs.das`: the fused gate_up and the down
// expert stacks MXFP4 on disk, every other plane the source's): arm `mx26`. The expert stacks load as the native-MXFP4
// planes under gemma-4's GeGLU and per-expert down scale. The format census runs on the cut of the M5's mint, off the Q8_0
// file (`gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf`), the perplexity cell on the pod's whole mint. Skips off the JIT, without DASLLAMA_GPU=1 on a device
// the whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card.
let private GEMMA4_26B_MX4_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", arm = "mx26")
let private GEMMA4_26B_MX4_CUT = GemmaFile(file = "gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf", arm = "mx26")
//! the routed model's argmax slack: the IQ3_XXS file's (a router near-tie flips whole positions between the arms)
let private GEMMA4_26B_MX4_HITS_SLACK = 12l

[test(description = "On the whole-model driver gemma-4-26B-A4B-6L MXFP4_MOE serves every site in its own format, the native-MXFP4 expert stacks dispatched on the mx4 stamps (the Vulkan home's summed down, the CUDA home's per-slot down), with the driver serving every call - on the CUDA driver under the MMQ route pinned too")]
def test_gpu_resident_gemma4_26b_mx4_format(t : T?) {
    t |> run("gemma-4-26B-A4B-6L MXFP4_MOE: every site in its own format - the native-MXFP4 expert stacks among them, on the mx4 stamps - and the driver serving every call") @(t : T?) {
        //! the CUDA home's cells measure the MMQ route under their own arm
        let gf = gpu_want_cuda() ? GemmaFile(file = GEMMA4_26B_MX4_CUT.file, arm = "cuda-g4mx") : GEMMA4_26B_MX4_CUT
        with_cuda_mmq_winners() {
            gemma_format_cell(t, gf, true)
        }
    }
}

[test(description = "On the whole-model driver gemma-4-26B MXFP4_MOE's native-MXFP4 routed rail scores a 150-position teacher-forced perplexity within 2x of the CPU chain's, hits within 12")]
def test_gpu_resident_gemma4_26b_mx4_ppl(t : T?) {
    t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_MX4_FILE, GEMMA4_26B_MX4_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}

// gemma-4-26B-A4B-6L Q4_K_M, the Q4_K_M file's first six blocks, its forms loaded once (`with_cpu_and_gpu_model`): two
// mirror regions with the device gather for the format census (arm `g4mk`: every site in its own format, a 40-token
// prompt and two fed steps served) and the down-fold cell - the batched rows fold the per-32 routed down under the
// routing weights as the fused one-row command does, within the wide bar with the one-token-off control, on the summed
// down stamps with no per-slot leaf - and the gathered half of the embed-gather cell; two regions with the gather off and
// the planes on the host, its host-embedded half; two regions with the fuse knob off, the split-forms cell (the N-row
// command's routed block on the split one-row forms, no routed down folded; the Vulkan home's alone); three regions with
// the fuse knob off, the widening cell. The gemma-4 MoE form - the routed feed and the router off their own norms of x,
// the parallel dense shared expert as the layer's own FFN triple, the per-expert down scale folded into the routing
// weights, the combine norming both branches and their sum under the layer's output scale - on K-quant projections and
// experts, so the wide bar with the one-token-off control (the bar's reading `_resident_regions.das`'s). The embed-gather
// cell needs the dasVulkan module. Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without
// the file under models_dir().

//! what the 26B Q4_K_M cut's forms collect: the regions rig's
//! readings, the arms the format census's carrier reads (`carrier`),
//! and the embed-gather cell's halves
struct G4MoeKRows {
    regions : Held
    carrier : Held
    gather : GatherPair
}

//! one form's collection of the cut's cells into `r` (`format` the
//! format census's arm on); the census asserts on the cell as it
//! reads each site, the one collection that does
def private g4moe_k_rows(t : T?; var r : G4MoeKRows; m : Model; form : ModelForm; format : bool) {
    return if (!form.on_gpu)
    if (form.regions == 3l) {
        regions_widen_rows(r.regions, m, LogitsBar.ctrl_bar)
    } elif (form.split_forms) {
        regions_batched_rows(r.regions, m, LogitsBar.ctrl_bar, DownFold.split_forms)
    } elif (form.gather) {
        if (format) {
            r.carrier |> held_fact(gemma_carrier_arms(m), GEMMA_CARRIER_ARMS)
            gemma_format_census(t, m, GEMMA4_26B_K_CUT.file, false)
        }
        regions_batched_rows(r.regions, m, LogitsBar.ctrl_bar, DownFold.folded)
        gather_half(r.gather, r.regions, m, true, GEMMA4_26B_K_CUT.file)
    } else {
        gather_half(r.gather, r.regions, m, false, GEMMA4_26B_K_CUT.file)
    }
}

//! the cut's forms: the gathering two-region form; where the
//! dasVulkan module builds the host-embedded one and, unless the CUDA
//! home is asked, the split one-row forms on two regions; and three
//! regions on the split forms; a cell the box cannot run is held as
//! a skip
def private g4moe_k_forms(var r : G4MoeKRows) : array<ModelForm> {
    var forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    static_if (typeinfo builtin_module_exists(vulkan)) {
        forms |> push(device_form(gather = false, regions = 2l, host = true, ctx = REGIONS_RIG_CTX))
        if (gpu_want_cuda()) {
            r.regions |> held_skip("the CUDA home is asked: the fuse knob is the Vulkan driver's")
        } else {
            forms |> push(device_form(regions = 2l, split_forms = true, ctx = REGIONS_RIG_CTX))
        }
    } else {
        r.regions |> held_skip("no dasVulkan module on this build: the split one-row forms are " +
            "the Vulkan driver's")
    }
    forms |> push(device_form(regions = 3l, split_forms = true, ctx = REGIONS_RIG_CTX))
    return <- forms
}

[test(description = "On gemma-4-26B-A4B-6L Q4_K_M with q51 down experts every site serves in its own format; the batched rows fold the per-32 routed down under the routing weights as the fused one-row command does, within the wide bar, on the summed down stamps with no per-slot leaf; the batched rows gather their token rows on the device off their ids, logits bit for bit those of a load whose host embeds every row, the gather-off counters the control; with the fused one-row forms off the N-row command's routed block batches two sessions within the wide bar of each alone, the one-token-off control past it, with no routed down folded; a batched step over two rows, a one-row step, then batched steps over three record new row counts on the N-row command, every row within the wide bar of its session alone; each form loads once")]
def test_gpu_resident_gemma4_26b_k_cut(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let format = arm_on(t, GEMMA4_26B_K_CUT.arm)
    var r : G4MoeKRows
    var forms <- g4moe_k_forms(r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", GEMMA_RIG_CTX, forms) $(m, form) {
            g4moe_k_rows(t, r, m, form, format)
        }
    }
    if (ran) {
        held_within(t, r.carrier, "{GEMMA4_26B_K_CUT.file}: ", false)
        held_within(t, r.regions, "{GEMMA4_26B_K_CUT.file}, the regions rig: ", false)
        gather_pair_within(t, GEMMA4_26B_K_CUT.file, r.gather)
    }
    delete r
    delete forms
}
