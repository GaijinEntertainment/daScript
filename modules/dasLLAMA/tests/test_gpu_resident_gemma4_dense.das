options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require _resident_regions
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms
require _resident_feed   // with_cuda_mmq_winners, Held
require dasllama/dasllama_common   // Model
require dasllama/dasllama_gpu_tier   // gpu_want_cuda: the CUDA home's arms
require dasllama/dasllama_env   // g_env_engine.pin_backend: the parity form's environment pin
require _parity_cells   // parity_gemma4_12b_cut_cell: the Q4_K_M cut's pinned CPU form's parity stream; the Q8_0 file's parity row

// gemma-4-12B Q8_0 (the dense carrier on the Q8_0 planes: the weightless v-norm, the V-from-K global layers beside
// the sliding ones, the per-layer output scale, the final softcap and the suppressed ids). Its device cells run on
// gemma-4-12B-12L, the file's first twelve blocks (two global layers, the cap lifted), every form loaded once
// (`with_cpu_and_gpu_model`): the default form's forced feed (arm `g4q8`, `cuda-g4q8` under DASLLAMA_CUDA=1) against
// the CPU form's rows of the same prose, and two mirror regions for the batched-step cells - the N-row token command
// serving two rows bit for bit against the sessions alone at one window, past the unsplit position, and a batched step
// before any one-row step. The perplexity cells read real text and stay on the whole file (the `gemma4-dense`
// category). Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file
// under models_dir(), or when the residency plan does not fit the card - a 16 GB card arms the whole file under
// DASLLAMA_GPU_MIN_CTX=1024. The whole file's walks - the Vulkan home's and the CUDA home's cells (arm `cuda-g4q8`, under the
// MMQ route pinned `CUDA_MMQ_WINNERS`) - ride one block: its default form and the CPU form, each walk read once a form; the
// block's parity row CPU form (the row's load mode and K-quant knob under DASLLAMA_PIN_BACKEND's pin), on every box, the row's
// frozen greedy ids (`parity_spec_cell`), skipped where the box cannot take the pin.
let private GEMMA4_Q8_FILE = GemmaFile(file = "gemma-4-12B-it-Q8_0.gguf", arm = "g4q8")
let private GEMMA4_Q8_CUDA_ARM = "cuda-g4q8"   //! the CUDA home's arm of the whole file's walks
//! the two-window cell's ratio: at 600 positions three implementations of this model read three perplexities over the same
//! 80 targets - the CPU chain 1.97 (72 hits), the resident 2.37 (72), llama.cpp b10660 2.51 (71) - so the resident sits
//! between the references and a 1.05 bar against the CPU chain measures the reference, not the driver
let private GEMMA4_Q8_TWO_WINDOW_PPL_RATIO = 1.3

//! the cuts the dense cells load, each its file's first twelve blocks
//! (two global layers, the cap lifted); the load names each as a
//! literal, the file `REVIEW.das` holds to one call an area
let private GEMMA4_12B_Q8_CUT = "gemma-4-12B-12L-Q8_0-local.gguf"
let private GEMMA4_12B_K_CUT = "gemma-4-12B-12L-Q4_K_M-local.gguf"

//! what a dense cut's forms collect: the default form beside its CPU
//! form (`main`), the two forms' feeds and path gaps, the arms the
//! device forms' carrier reads (`carrier`), and the regions rig
struct G4DenseRows {
    main : MainPair
    gpu : GemmaRows
    cpu : GemmaRows
    carrier : Held
    regions : Held
}

//! one form's collection into `r`: the default form's and the CPU
//! form's feeds of `feeds` and gaps of `gaps`, the two-region form's
//! three batched-step cells
def private g4dense_rows(var r : G4DenseRows; m : Model; form : ModelForm;
                         feeds, gaps : array<int64>) {
    if (form.on_gpu) {
        r.carrier |> held_fact(gemma_carrier_arms(m), GEMMA_CARRIER_ARMS)
    }
    let side = main_side(r.main, form)
    if (side == MainSide.device) {
        r.gpu <- gemma_rows(m, form, feeds, gaps, array<GemmaWalk>())
    } elif (side == MainSide.cpu) {
        r.cpu <- gemma_rows(m, form, feeds, gaps, array<GemmaWalk>())
    } elif (form.on_gpu && form.regions == 2l) {
        regions_batched_once_rows(r.regions, m)
    }
}

//! a dense cut's compares under `tag` on what its forms collected,
//! each cell where its forms ran
def private g4dense_rows_within(t : T?; tag : string; r : G4DenseRows; gaps : array<int64>) {
    held_within(t, r.carrier, "{tag}: ", false)
    held_within(t, r.regions, "{tag}, two regions: ", false)
    if (r.main.ran) {
        gemma_rows_within(t, tag, r.gpu, r.cpu, gaps, array<GemmaWalk>())
    }
}

[test(description = "On the whole-model driver gemma-4-12B-12L Q8_0 keeps one-window forced-feed logits within 16 percent of the CPU chain (on the CUDA driver too, every position served); over two mirror regions a batched step over two sessions reads bit for bit what each session reads alone, with both prompts past the unsplit position the N-row command's split attention form does too, and a batched step before any one-row step, one-row steps, then a batched step again all read what each session reads alone; each form loads once")]
def test_gpu_resident_gemma4_dense(t : T?) {
    return if (!family_on(t, "gemma4"))
    var feeds : array<int64>
    if (arm_on(t, gpu_want_cuda() ? "cuda-g4q8" : "g4q8")) {
        feeds |> push(40l)
    }
    var r : G4DenseRows
    var forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    if (!empty(feeds)) {
        forms |> push(device_form())
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-12B-12L-Q8_0-local.gguf", GEMMA_RIG_CTX, forms) $(m, form) {
            g4dense_rows(r, m, form, feeds, array<int64>())
        }
    }
    if (ran) {
        g4dense_rows_within(t, GEMMA4_12B_Q8_CUT, r, array<int64>())
    }
    delete r
    delete forms
    delete feeds
}

//! the whole Q8_0 file's walks: 150 prose ids prefilled and 150 teacher-forced, then two windows - the second eight rows
//! deep - and 80 teacher-forced past them
let private GEMMA4_Q8_WALKS = fixed_array((n = 150l, steps = 150l), (n = PF_WINDOW_ROWS + 8l, steps = 80l))

//! walk `i`'s compare under `tag` at `ratio`, the hits within 3 of the CPU chain's
def private g4q8_walk_within(t : T?; tag : string; r : GemmaWalkRows; i : int; ratio : float) {
    gemma_walk_twin_within(t, tag, r, GEMMA4_Q8_WALKS, i, ratio)
}

[test(description = "On the whole-model driver gemma-4-12B Q8_0 keeps its 150-position perplexity within 1.05x of the CPU chain's and its two-window perplexity within 1.3x; on the CUDA driver the same file's teacher-forced perplexity stays within 1.05x of the CPU chain's over 150 positions and 1.3x past two windows; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_gemma4_dense_ppl(t : T?) {
    return if (!family_on(t, "gemma4"))
    let dense = empty(arm_why(GEMMA4_Q8_FILE.arm))
    let cuda = empty(cuda_home_why()) && empty(arm_why(GEMMA4_Q8_CUDA_ARM))
    var r : GemmaWalkRows
    var ran = false
    var forms : array<ModelForm>
    if (dense || cuda) {
        forms |> push(device_form())
    }
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-12B-it-Q8_0.gguf", GEMMA_RIG_CTX, forms, parity = true) $(var m, form) {
            gemma_walk_rows(r, m, form, GEMMA4_Q8_WALKS)
        }
    }
    if (ran) {
        held_within(t, r.carrier, "gemma-4-12B Q8_0: ", false)
    }
    unsafe {   //! the cells read the block's collection by reference
        t |> run("gemma-4-12B Q8_0: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @capture(& r)(t : T ?) {
            return if (!arm_on(t, GEMMA4_Q8_FILE.arm) || !ran)
            g4q8_walk_within(t, "gemma-4-12B Q8_0", r, 0, GEMMA_PPL_RATIO)
        }
        t |> run("gemma-4-12B Q8_0: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them, the 1024 window not yet engaged") @capture(& r)(t : T ?) {
            return if (!arm_on(t, GEMMA4_Q8_FILE.arm) || !ran)
            assert_pf_window(t)
            g4q8_walk_within(t, "gemma-4-12B Q8_0", r, 1, GEMMA4_Q8_TWO_WINDOW_PPL_RATIO)
        }
        t |> run("gemma-4-12B Q8_0 on the CUDA driver: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @capture(& r)(t : T ?) {
            return if (!cuda_home_asked(t) || !arm_on(t, GEMMA4_Q8_CUDA_ARM) || !ran)
            g4q8_walk_within(t, "gemma-4-12B Q8_0, cuda", r, 0, GEMMA_PPL_RATIO)
        }
        t |> run("gemma-4-12B Q8_0 on the CUDA driver: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them") @capture(& r)(t : T ?) {
            return if (!cuda_home_asked(t) || !arm_on(t, GEMMA4_Q8_CUDA_ARM) || !ran)
            assert_pf_window(t)
            g4q8_walk_within(t, "gemma-4-12B Q8_0, cuda", r, 1, GEMMA4_Q8_TWO_WINDOW_PPL_RATIO)
        }
        t |> run("gemma-4-12B Q8_0: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @capture(& r)(t : T ?) {
            return if (!cuda_home_asked(t) || !arm_on(t, GEMMA4_Q8_CUDA_ARM) || !ran)
            g4q8_walk_within(t, "gemma-4-12B Q8_0, cuda", r, 0, GEMMA_PPL_RATIO)
        }
        t |> run("gemma-4-12B Q8_0: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them") @capture(& r)(t : T ?) {
            return if (!cuda_home_asked(t) || !arm_on(t, GEMMA4_Q8_CUDA_ARM) || !ran)
            assert_pf_window(t)
            g4q8_walk_within(t, "gemma-4-12B Q8_0, cuda", r, 1, GEMMA4_Q8_TWO_WINDOW_PPL_RATIO)
        }
    }
    delete r
    delete forms
}

// gemma-4-12B-12L Q4_K_M, the 12B Q4_K_M file's first twelve blocks (the dense carrier on the K-quant planes), its
// default form and the CPU form loaded once: the window chain against the token command at eight tokens (arm `g4k`)
// and 300 (`g4x`; both `cuda-g4k` under DASLLAMA_CUDA=1), the CPU chain's own two paths the yardstick. The agreement form
// alone: on a K-quant file the CPU chain's own prefill and decode kernels land far apart, so a CPU-referenced bar would
// measure the model's sensitivity to the activation rounding, not the driver. On its CPU form under the environment's
// backend pin (DASLLAMA_PIN_BACKEND, none where unset) with the K-quant planes requantized to q8, on every box, the parity
// reference's greedy stream after the 1492-id count (`_parity_cells.das`). Skips off the JIT, without DASLLAMA_GPU=1
// on a device the whole-model driver serves, without the file under models_dir(), or when the residency plan does not
// fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror); the parity form where the box cannot take the pin.

//! the parity row's context: a cut's parity stream runs at the context its full file's parity row loads at
let private PARITY_CUT_CTX = 2048l

[test(description = "On the whole-model driver gemma-4-12B-12L Q4_K_M's wide-head window chain and token command agree over 8 and 300 tokens within 6x the CPU chain's own prefill-versus-decode gap, the agreement form standing in for a CPU-referenced bar - on the CUDA driver too, with every position of both paths served; on the parity-pinned CPU form, on every box, fed the 12B's reference greedy stream after the 1492-id count every reference id keeps within 3% of its row's top logit, the stream one step late outside; each form loads once")]
def test_gpu_resident_gemma4_prefill_vs_decode(t : T?) {
    return if (!family_on(t, "gemma4"))
    let cuda = gpu_want_cuda()
    var gaps : array<int64>
    if (arm_on(t, cuda ? "cuda-g4k" : "g4k")) {
        gaps |> push(8l)
    }
    if (arm_on(t, cuda ? "cuda-g4k" : "g4x")) {
        gaps |> push(300l)
    }
    var forms <- [cpu_cells_form(backend = g_env_engine.pin_backend, kq_native = KqNative.off)]
    if (!empty(gaps)) {
        forms |> push(device_form())
    }
    var r : G4DenseRows
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-12B-12L-Q4_K_M-local.gguf", GEMMA_RIG_CTX, forms) $(var m, form) {
            if (form.kq_native == KqNative.off) {
                with_ctx_cut(m, PARITY_CUT_CTX) {
                    parity_gemma4_12b_cut_cell(t, m)
                }
            } else {
                g4dense_rows(r, m, form, array<int64>(), gaps)
            }
        }
    }
    if (ran) {
        g4dense_rows_within(t, GEMMA4_12B_K_CUT, r, gaps)
    }
    delete r
    delete gaps
    delete forms
}
