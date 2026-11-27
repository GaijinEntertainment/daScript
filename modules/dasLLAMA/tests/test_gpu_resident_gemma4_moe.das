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
require dasllama/dasllama_env   // g_env_engine.pin_backend: the parity form's environment pin
require _parity_cells   // parity_gemma4_26b_cut_cell: the Q4_K_M cut's pinned CPU form's parity stream
require _metal_matrix_cells   // the Metal family matrix's gemma4moe rows on the files' Metal forms
require _metal_mtp_cells   // mtp_verify_metal: the Q4_K_M cut's served image's same-slab verify
require _kernel_coverage_cells   // cov_zoo_c15_gemma4moe: the kernel census's zoo row

// gemma-4-26B-A4B UD-IQ3_XXS (the routed MoE with its parallel dense shared expert and the routed branch's own norms):
// arm `g4m`. Each file's format census runs on its cut, the first six blocks (one global layer, the cap lifted); the
// perplexity cells read real text on the whole files (the `gemma4-moe` category) and ride each whole file's block in
// `test_mtp_gemma_drafter.das`, beside the drafter's served cells on the same trunk. Skips off the JIT, without
// DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or when the residency plan
// does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_CUT = GemmaFile(file = "gemma-4-26B-A4B-6L-UD-IQ3_XXS-local.gguf", arm = "g4m")
let private MOE_ROW_ARM = "fam-gemma4moe"   //! the Metal family matrix's gemma4moe rows

// Every 26B file's Metal form (`metal_form`, the blob twin of
// MATRIX_CTX positions; arm `fam-gemma4moe`) carries the family
// matrix's gemma4moe row - its three legs (the fused tail, the
// unfused tail, a q8_0 mirror) within the row's bars of the CPU
// truths its portable-pinned CPU form reads - and the Q4_0 cut's the
// kernel census's zoo row (arm `coverage`), on a form of
// KERNEL_COVERAGE_CTX positions.

//! the gemma4moe row `p` on the blob twin, its CPU truths on the
//! portable CPU form; false on any other form
def private moe_row_cells(t : T?; var m : Model; form : ModelForm; p : MatrixMoeRow;
                          var kept : MatrixMoeKept) : bool {
    if (form.on_metal && !form.metal_image) {
        matrix_moe_metal(t, m, form, p, kept)
    } elif (alone_pass_of(form, matrix_cpu_form())) {
        matrix_moe_cpu(t, m, form, p, kept)
    } else {
        return false
    }
    return true
}

//! a routed cut's weight-format census (`gemma_format_census`) on the
//! device form `m`, which keeps the planes on the host, as the
//! subtest `what`
def private format_census_on(t : T?; var m : Model; what, file : string; mx4 : bool) {
    unsafe {   //! the cell captures the loaded trunk by reference
        t |> run(what) @capture(& m)(t : T ?) {
            t |> success(gemma_carrier_arms(m), GEMMA_CARRIER_ARMS)
            gemma_format_census(t, m, file, mx4)
        }
    }
}

[test(description = "On the whole-model driver the routed gemma-4-26B-A4B-6L UD-IQ3_XXS serves every site in its own format with every call on the device; on the Metal home's blob twin, one load on a das_metal build, the gemma4moe row's geometry, needs and path cells hold and its fused, unfused-tail and q8_0-mirror legs read within the row's bars of the portable CPU truths, the routed stamps counted; each form loads once")]
def test_gpu_resident_gemma4_26b_format(t : T?) {
    return if (!family_on(t, "gemma4moe") || !arms_on(t, [GEMMA4_26B_CUT.arm, MOE_ROW_ARM]))
    let row = matrix_moe_iq3xxs_cut()
    var kept : MatrixMoeKept
    var forms : array<ModelForm>
    if (empty(arm_why(GEMMA4_26B_CUT.arm))) {
        forms |> push(device_form(host = true))
    }
    metal_row_forms(forms, empty(arm_why(MOE_ROW_ARM)), MATRIX_CTX, matrix_cpu_form())
    with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-6L-UD-IQ3_XXS-local.gguf", GEMMA_RIG_CTX,
            forms, false) $(var m, form) {
        return if (moe_row_cells(t, m, form, row, kept))
        format_census_on(t, m, ("gemma-4-26B-A4B-6L IQ3_XXS: every site in its own format - " +
            "the expert stacks and the token table among them - and the driver serving every call"),
            GEMMA4_26B_CUT.file, false)
    }
    matrix_moe_verdict(t, row, kept)
    delete forms
}

// gemma-4-26B-A4B Q4_K_M (the routed MoE in the model table's official quant: k4 gate and up expert stacks beside native
// Q5_1 down stacks - the q51 expert rail - with the dense triple and the attention head on the K-quant rails): arm `g4mk`.
// Its cut's cells load each form once (`test_gpu_resident_gemma4_26b_k_cut`, below); the whole file's perplexity cells ride its
// block in `test_mtp_gemma_drafter.das`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves,
// without the file under models_dir(), or when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the
// mirror).
let private GEMMA4_26B_K_CUT = GemmaFile(file = "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", arm = "g4mk")
//! the cut's repeat prompt: long enough that an expert's bucket passes one m column, so its rows split between the m and s tiles
let private G4MOE_REPEAT_IDS = 400l

// gemma-4-26B-A4B MXFP4_MOE (the Q4_K_M file requantized, `../performance/model_specs.das`: the fused gate_up and the down
// expert stacks MXFP4 on disk, every other plane the source's): arm `mx26`. The expert stacks load as the native-MXFP4
// planes under gemma-4's GeGLU and per-expert down scale. The format census runs on the cut of the M5's mint, off the Q8_0
// file (`gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf`), the perplexity cell on the pod's whole mint. Skips off the JIT, without DASLLAMA_GPU=1 on a device
// the whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card.
let private GEMMA4_26B_MX4_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", arm = "mx26")
let private GEMMA4_26B_MX4_CUT = GemmaFile(file = "gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf", arm = "mx26")
//! the routed model's argmax slack: the IQ3_XXS file's (a router near-tie flips whole positions between the arms)
let private GEMMA4_26B_MX4_HITS_SLACK = 12l

[test(description = "On the whole-model driver gemma-4-26B-A4B-6L MXFP4_MOE serves every site in its own format, the native-MXFP4 expert stacks dispatched on the mx4 stamps (the Vulkan home's summed down, the CUDA home's per-slot down), with the driver serving every call - on the CUDA driver under the MMQ route pinned too; on the Metal home's blob twin, one load on a das_metal build, the gemma4moe row's geometry, needs and path cells hold, the stacks load as the native-MXFP4 planes, and its three legs read within the row's bars of the portable CPU truths; each form loads once")]
def test_gpu_resident_gemma4_26b_mx4_format(t : T?) {
    //! the CUDA home's cells measure the MMQ route under their own
    //! arm
    let arm = gpu_want_cuda() ? "cuda-g4mx" : GEMMA4_26B_MX4_CUT.arm
    return if (!family_on(t, "gemma4moe") || !arms_on(t, [arm, MOE_ROW_ARM]))
    let row = matrix_moe_mx4_cut()
    var kept : MatrixMoeKept
    var forms : array<ModelForm>
    if (empty(arm_why(arm))) {
        forms |> push(device_form(host = true))
    }
    metal_row_forms(forms, empty(arm_why(MOE_ROW_ARM)), MATRIX_CTX, matrix_cpu_form())
    with_cuda_mmq_winners() {
        with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf", GEMMA_RIG_CTX,
                forms, false) $(var m, form) {
            return if (moe_row_cells(t, m, form, row, kept))
            format_census_on(t, m, ("gemma-4-26B-A4B-6L MXFP4_MOE: every site in its own " +
                "format - the native-MXFP4 expert stacks among them, on the mx4 stamps - and the driver " +
                "serving every call"),
                GEMMA4_26B_MX4_CUT.file, true)
        }
    }
    matrix_moe_verdict(t, row, kept)
    delete forms
}

//! the whole MXFP4_MOE file's walk: 150 prose ids prefilled and 150 teacher-forced
let private GEMMA4_26B_MX4_WALKS = fixed_array((n = 150l, steps = 150l))
let private GEMMA4_26B_MX4_CUDA_ARM = "cuda-g4mx"   //! the CUDA home's arm of the whole file's walk

//! the walk's compare under `tag`: the device served it, its perplexity within 2x of the CPU chain's, its hits within 12
def private mx4_ppl_within(t : T?; tag : string; r : GemmaWalkRows) {
    gemma_walk_twin_within(t, tag, r, GEMMA4_26B_MX4_WALKS, 0, GEMMA_MOE_PPL_RATIO, GEMMA4_26B_MX4_HITS_SLACK)
}

[test(description = "On the whole-model driver gemma-4-26B MXFP4_MOE's native-MXFP4 routed rail scores a 150-position teacher-forced perplexity within 2x of the CPU chain's, hits within 12; on the CUDA driver under the MMQ route pinned the same; each form loads once")]
def test_gpu_resident_gemma4_26b_mx4_ppl(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let vk = empty(arm_why(GEMMA4_26B_MX4_FILE.arm))
    let cuda = empty(cuda_home_why()) && empty(arm_why(GEMMA4_26B_MX4_CUDA_ARM))
    var r : GemmaWalkRows
    var ran = false
    if (vk || cuda) {
        with_cuda_mmq_winners() {
            ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", GEMMA_RIG_CTX, [device_form()]) $(m, form) {
                gemma_walk_rows(r, m, form, GEMMA4_26B_MX4_WALKS)
            }
        }
    }
    if (ran) {
        held_within(t, r.carrier, "gemma-4-26B-A4B MXFP4_MOE: ", false)
    }
    unsafe {   //! the cells read the block's collection by reference
        t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail") @capture(& r)(t : T ?) {
            return if (!arm_on(t, GEMMA4_26B_MX4_FILE.arm))
            if (!ran) {
                t |> skip("gemma-4-26B-A4B MXFP4_MOE: no device form ran - the block's skip names why")
                return
            }
            mx4_ppl_within(t, "gemma-4-26B-A4B MXFP4_MOE", r)
        }
        t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail on the CUDA driver") @capture(& r)(t : T ?) {
            return if (!cuda_home_asked(t) || !arm_on(t, GEMMA4_26B_MX4_CUDA_ARM) || !ran)
            mx4_ppl_within(t, "gemma-4-26B-A4B MXFP4_MOE, cuda", r)
        }
    }
    delete r
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
// cell needs the dasVulkan module. On its CPU form under the environment's backend pin (DASLLAMA_PIN_BACKEND, none where
// unset) with the K-quant planes requantized to q8, on every box, the parity reference's greedy stream after the 1492-id
// count (`_parity_cells.das`). Skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without the
// file under models_dir(); the parity form where the box cannot take the pin.

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
        static_if (typeinfo builtin_module_exists(vulkan)) {   // the discrete homes: the Metal home's repeat is its own pass
            prefill_repeats(t, m, G4MOE_REPEAT_IDS, GEMMA4_26B_K_CUT.file)
        }
        egu_form_follows_set(t, GEMMA4_26B_K_CUT.file)
    } else {
        gather_half(r.gather, r.regions, m, false, GEMMA4_26B_K_CUT.file)
    }
}

//! the cut's forms: the gathering two-region form; where the
//! dasVulkan module builds the host-embedded one and, unless the CUDA
//! home is asked, the split one-row forms on two regions; three
//! regions on the split forms; and the parity-pinned CPU form; a cell
//! the box cannot run is held as a skip
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
    forms |> push(cpu_cells_form(backend = g_env_engine.pin_backend, kq_native = KqNative.off))
    return <- forms
}

//! the parity row's context: a cut's parity stream runs at the context its full file's parity row loads at
let private PARITY_CUT_CTX = 2048l

[test(description = "On gemma-4-26B-A4B-6L Q4_K_M with q51 down experts every site serves in its own format; the batched rows fold the per-32 routed down under the routing weights as the fused one-row command does, within the wide bar, on the summed down stamps with no per-slot leaf; the batched rows gather their token rows on the device off their ids, logits bit for bit those of a load whose host embeds every row, the gather-off counters the control; with the fused one-row forms off the N-row command's routed block batches two sessions within the wide bar of each alone, the one-token-off control past it, with no routed down folded; a batched step over two rows, a one-row step, then batched steps over three record new row counts on the N-row command, every row within the wide bar of its session alone; on the parity-pinned CPU form, on every box, fed the 26B's reference greedy stream after the 1492-id count every reference id keeps within 2% of its row's top logit, the stream one step late outside; on the Metal home's blob twin, one load on a das_metal build, the gemma4moe row's geometry, needs and path cells hold and its three legs read within the row's bars of the portable CPU truths; on its served image, one load on a das_metal build, the batch rail's same-slab verify rows read within 2.0 of the single steps'; each form loads once")]
def test_gpu_resident_gemma4_26b_k_cut(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let format = arm_on(t, GEMMA4_26B_K_CUT.arm)
    let row = matrix_moe_k4_cut()
    let verify = mtp_verify_gemma4_26b_cut()
    var r : G4MoeKRows
    var kept : MatrixMoeKept
    var forms <- g4moe_k_forms(r)
    metal_row_forms(forms, empty(arm_why(MOE_ROW_ARM)), MATRIX_CTX, matrix_cpu_form())
    var verify_arms <- mtp_verify_arms(verify.tag)
    if (any_arm_on(verify_arms)) {
        forms |> push(metal_form(image = true, ctx = METAL_MTP_CELLS_CTX))
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", GEMMA_RIG_CTX, forms) $(var m, form) {
            if (form.metal_image) {
                mtp_verify_metal(t, m, form, verify)
                return
            }
            return if (moe_row_cells(t, m, form, row, kept))
            if (form.kq_native == KqNative.off) {
                with_ctx_cut(m, PARITY_CUT_CTX) {
                    parity_gemma4_26b_cut_cell(t, m)
                }
            } else {
                g4moe_k_rows(t, r, m, form, format)
            }
        }
    }
    if (ran) {
        held_within(t, r.carrier, "{GEMMA4_26B_K_CUT.file}: ", false)
        held_within(t, r.regions, "{GEMMA4_26B_K_CUT.file}, the regions rig: ", false)
        gather_pair_within(t, GEMMA4_26B_K_CUT.file, r.gather)
    }
    matrix_moe_verdict(t, row, kept)
    delete r
    delete forms
    delete verify_arms
}

[test(description = "On the Metal home's blob twin of gemma-4-26B-A4B-6L Q8_0, one load on a das_metal build, the gemma4moe row's geometry, needs and path cells hold and its fused, unfused-tail and q8_0-mirror legs read within the row's bars of the portable CPU truths, the q8 routed stamps counted; each form loads once")]
def test_gpu_resident_gemma4_26b_q8_cut(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let row = matrix_moe_q8_cut()
    var kept : MatrixMoeKept
    var forms : array<ModelForm>
    metal_row_forms(forms, empty(arm_why(MOE_ROW_ARM)), MATRIX_CTX, matrix_cpu_form())
    with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-6L-Q8_0-local.gguf", GEMMA_RIG_CTX,
            forms) $(var m, form) {
        moe_row_cells(t, m, form, row, kept)
    }
    matrix_moe_verdict(t, row, kept)
    delete forms
}

[test(description = "On the Metal home's blob twin of gemma-4-26B-A4B-6L Q4_0, one load on a das_metal build, the gemma4moe row's geometry, needs and path cells hold and its three legs read within the row's bars of the portable CPU truths, the q40 and q4032 routed stamps counted, and a prompt and a continuation feed the kernel census; each form loads once")]
def test_gpu_resident_gemma4_26b_q40_cut(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let row = matrix_moe_q40_cut()
    var kept : MatrixMoeKept
    var forms : array<ModelForm>
    metal_row_forms(forms, any_arm_on([MOE_ROW_ARM, "coverage"]), KERNEL_COVERAGE_CTX,
        matrix_cpu_form())
    with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-6L-Q4_0-local.gguf", GEMMA_RIG_CTX,
            forms) $(var m, form) {
        moe_row_cells(t, m, form, row, kept)
        if (form.on_metal) {
            coverage_zoo_metal(t, m, form, cov_zoo_c15_gemma4moe())
        }
    }
    matrix_moe_verdict(t, row, kept)
    delete forms
}

[test(description = "On the Metal home's blob twin of the whole gemma-4-26B-A4B UD-IQ4_XS, one load on a das_metal build, the gemma4moe row's geometry, needs and path cells hold and its three legs read within 8 and 24 of the portable CPU truths, the last block's IQ4_XS routed stamp counted; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_gemma4_26b_iq4xs(t : T?) {
    return if (!family_on(t, "gemma4moe"))
    let row = matrix_moe_iq4xs()
    var kept : MatrixMoeKept
    var forms : array<ModelForm>
    metal_row_forms(forms, empty(arm_why(MOE_ROW_ARM)), MATRIX_CTX, matrix_cpu_form())
    with_cpu_and_gpu_model(t, "llm", "gemma-4-26B-A4B-it-UD-IQ4_XS.gguf", GEMMA_RIG_CTX,
            forms, parity = true) $(var m, form) {
        moe_row_cells(t, m, form, row, kept)
    }
    matrix_moe_verdict(t, row, kept)
    delete forms
}
