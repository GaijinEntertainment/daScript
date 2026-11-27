options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require dasllama/dasllama_math   // with_dasllama_jobque_: the ring compares after the block read argmax off the queue
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX
require _resident_feed   // Held, held_within: the regions cells' readings
require dasllama/dasllama_common   // Model
require _chat_cells   // chat_gemma2_cells: the uncached CPU form's chat cells
require _sampling_cells   // the gemma-2-2b sampling cells on its q8 and uncached CPU forms
require _embedding_cells   // embeddings_ring_layers_cell: gemma-3-1b's default device form
require _cpu_gpu_model   // ModelForm: the parity rows' forms beside the blocks' own
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the files' model_specs parity rows on their blocks
require _metal_matrix_cells   // the Metal family matrix's rows on the files' Metal forms
require _metal_prefill_cells   // prefill_gemma3_1b: gemma-3-1b's Metal prefill cells
require _fused_decode_cells   // fused_decode_row_cell: the portable CPU forms' fused chains
require _kernel_coverage_cells   // the kernel census's zoo and Vulkan rows

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the gemma-2-2b chat cells' end-to-end respond prefills on the CPU
}

// gemma-2-2b Q8_0 (the attention logit softcap, a window on the even layers, no qk-norm): arm `g2`. Skips off the
// JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or
// when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror). The two-region form, at
// REGIONS_RIG_CTX, carries the rig's batched cells: the N-row token command serves two rows bit for bit against the sessions
// alone - at one window, past the unsplit position so the split attention form records, and a batched step before any one-row
// step - and, on the Vulkan home with a kernel census, the stamp cell. On its q8 and uncached CPU forms, on every box, the file's
// own CPU cells: the greedy oracle (`_sampling_cells.das`) on the q8 form, the chat renders and the greedy respond
// (`_chat_cells.das`) and the seeded sampling (`_sampling_cells.das`) on the uncached form. On its parity row's CPU form (the
// row's load mode and K-quant knob under DASLLAMA_PIN_BACKEND's pin), on every box, the row's frozen greedy ids
// (`parity_spec_cell`); that form skips where the box cannot take the pin, and so do gemma-3-1b's and gemma-3-4b's.
// The Metal form (`metal_form`, the blob twin; arm `fam-gemma2`)
// carries the family matrix's row, its CPU truth on the
// portable-pinned CPU form, which on every box also carries the fused
// decode chains (`_fused_decode_cells.das`); on the Vulkan home the
// q8_0 and tq4 mirrors carry the kernel census's codec rows (arm
// `coverage-vk`).
let private GEMMA2_FILE = GemmaFile(file = "gemma-2-2b-it-Q8_0.gguf", arm = "g2")
let private COVERAGE_VK_ARM = "coverage-vk"   //! the kernel census's Vulkan rows

//! why the box leaves the kernel census's Vulkan rows out: a build
//! with no Vulkan home, or the CUDA home asked; "" where they run
def private coverage_vk_box_why() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_home_why("the census rows are the Vulkan driver's")
    } else {
        return "no Vulkan home in this build: the census rows are the Vulkan driver's"
    }
}

//! the kernel census's Vulkan rows: on a build with the Vulkan home,
//! off the CUDA home, under their arm
def private coverage_vk_on() : bool
    => empty(coverage_vk_box_why()) && empty(arm_why(COVERAGE_VK_ARM))

//! gemma-2-2b Q8_0's forms: the block's own under its arm (`own`),
//! the Metal form under the matrix row's, the codec forms under the
//! census's, the portable CPU form for every one of its cells
def private gemma2_forms(t : T?; own, metal : bool) : array<ModelForm> {
    var forms : array<ModelForm>
    if (own) {
        forms |> push_from([device_form(), device_form(regions = 2l, ctx = REGIONS_RIG_CTX),
            cpu_cells_form(), cpu_cells_form(uncached = true)])
    }
    var inscope codecs <- [device_form() |> asking(KVDtype.q8_0),
        device_form() |> asking(KVDtype.tq4)]
    if (empty(arm_why(COVERAGE_VK_ARM))) {
        forms_or_skips(t, forms, GEMMA2_FILE.file, codecs, coverage_vk_box_why())
    }
    if (metal) {
        forms |> push(metal_form(ctx = MATRIX_CTX))
    }
    forms |> push(matrix_cpu_form())
    return <- forms
}

//! the gemma-2-2b Q8_0 cells the block's own arm does not hold, each
//! on its form; false where the form is the block's own
def private gemma2_other_cells(t : T?; var m : Model; form : ModelForm;
                               var fam : MatrixFamilyKept) : bool {
    if (form.on_metal) {
        matrix_family_metal(t, m, form, matrix_row_gemma2_2b_q8(), fam)
    } elif (alone_pass_of(form, matrix_cpu_form())) {
        matrix_family_cpu(t, m, form, matrix_row_gemma2_2b_q8(), fam)
        fused_decode_row_cell(t, m, form, fused_row_gemma2_2b())
    } elif (form.kv_asked) {
        coverage_vk_codec(t, m, form, cov_vk_codec_gemma2(form.kv))
    } else {
        return false
    }
    return true
}

[test(description = "On the whole-model driver gemma-2-2b Q8_0 with the attention softcap keeps forty-token forced-feed logits within 16 percent of max logit of the CPU chain, perplexity within 1.05x, and window chain against token command within 6x of the CPU chain's own two paths; over two mirror regions, with the attention softcap and the final logit softcap, batched steps over two sessions - at one window, past the unsplit position, and before any one-row step - read what each session reads alone, and its two q heads per kv head make a step dispatch the two-head slab decode attention stamp of the armed mirror codec; on the q8 CPU form, on every box, greedy generate reproduces the frozen counting oracle; on the uncached CPU form the folded first-turn prefill renders, a greedy respond runs end to end, and two sessions seeded alike sample alike; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the portable-pinned CPU form, on every box, the fused decode chains match the per-op path's greedy ids and final logits bit for bit; on the Metal home's blob twin, one load on a das_metal build, metal_needs derives the family's bits, all three Metal paths engage, and a metal-overrides generate emits the CPU truth's ids exactly; on the Vulkan home's q8_0 and tq4 mirrors a prefill and two steps feed the kernel census; each form loads once")]
def test_gpu_resident_gemma2(t : T?) {
    return if (!family_on(t, "gemma2")
        || !arms_on(t, [GEMMA2_FILE.arm, "fam-gemma2", COVERAGE_VK_ARM]))
    let own = empty(arm_why(GEMMA2_FILE.arm))
    let gaps <- [8l]
    let walks <- [(n = 150l, steps = 150l)]
    var feeds <- [40l]
    if (gpu_want_cuda()) {   //! the CUDA home's own cell: a two-window prompt and eight steps
        feeds |> push(PF_WINDOW_ROWS + 88l)
    }
    var rows : GemmaRows[2]
    var regions : Held
    var fam : MatrixFamilyKept
    var forms <- gemma2_forms(t, own, empty(arm_why("fam-gemma2")))
    var dev_ran = false   //! a device form collected: the CPU form's rows are its references
    //! the default form's armed codec: the CPU pass its rows answer
    var main_kv = KVDtype.f16
    with_cuda_mmq_winners() {
        let ran = with_cpu_and_gpu_model(t, "llm", "gemma-2-2b-it-Q8_0.gguf", GEMMA_RIG_CTX,
                forms, own, parity = own) $(var m, form) {
            return if (gemma2_other_cells(t, m, form, fam))
            dev_ran ||= form.on_gpu
            if (device_default(form)) {
                main_kv = form.kv
            }
            if (form.uncached) {
                chat_gemma2_cells(t, m)
                sampling_deterministic_cell(t, m)
            } elif (form.on_gpu && form.regions == 2l) {
                if (gpu_want_cuda()) {
                    regions |> held_skip("the CUDA home is asked: the slab pick and the census names are the Vulkan driver's")
                } else {   //! first on the load: the census counts a token command's dispatches as it records, and a later step replays
                    regions_stamp_rows(regions, m, 2)
                }
                regions_batched_once_rows(regions, m)
            } else {
                if (dev_ran && (form.on_gpu || form.kv == main_kv)) {
                    rows[form.on_gpu ? 1 : 0] <- gemma_rows(m, form, feeds, gaps, walks)
                }
                if (!form.on_gpu && form.alone) {
                    sampling_greedy_oracle_cell(t, m)
                }
            }
        }
        if (ran && own) {
            gemma_rows_within(t, "gemma-2-2b", rows[1], rows[0], gaps, walks)
            held_within(t, regions, "gemma-2-2b Q8_0, two regions: ", false)
        }
    }
    matrix_family_verdict(t, matrix_row_gemma2_2b_q8(), fam)
    delete rows
    delete regions
    delete feeds
    delete forms
}

// gemma-3-1b Q8_0 (the dense base: sandwich norms, sliding layers beside the global ones with their own rope class,
// the GeGLU FFN, the sqrt(dim) embed scale): arm `g3`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the
// whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card; the
// block-codec forms also skip where the environment arms another mirror codec over the one they ask (DASLLAMA_VK_KV32=1). The
// two-region form, at REGIONS_RIG_CTX, carries the rig's batched cells and, on its ringed load, the session's ring floor through
// every rail that moves its rows (a continuation, a prompt from 0, a park, an export, an import). The default device form also
// carries the embeddings' packed-seat cell (`_embedding_cells.das`). On its parity row's CPU form, on every box, the row's
// frozen greedy ids. The Metal form (`metal_form`, the blob twin of
// MATRIX_RING_CTX positions) carries the family matrix's row and its
// ring row (arm `fam-gemma3`) and the last-row prefill cell
// (`lastrow-g`), the portable-pinned CPU form the row's CPU truth
// and, on every box, the fused decode chains; on the Vulkan home the
// census's host-planes form carries its row (arm `coverage-vk`).
let private GEMMA3_FILE = GemmaFile(file = "gemma-3-1b-it-Q8_0.gguf", arm = "g3")
let private GEMMA3_METAL_ARMS = fixed_array("fam-gemma3", "lastrow-g")

//! what gemma-3-1b's Metal form keeps for its portable CPU form and
//! the compares after the block
struct G3MetalKept {
    family : MatrixFamilyKept
    prefill : PrefillKept
}

//! gemma-3-1b's forms: the block's own under its arm (`own`), the
//! census's host-planes form where its Vulkan row runs (`vk`),
//! the Metal form under its cells' arms, the portable CPU form
def private g3_forms(t : T?; own, vk : bool) : array<ModelForm> {
    var forms : array<ModelForm>
    if (!vk && empty(arm_why(COVERAGE_VK_ARM))) {
        forms_or_skips(t, forms, GEMMA3_FILE.file, [vk_census_form()], coverage_vk_box_why())
    }
    if (own) {
        forms |> push_from([device_form(), device_form() |> asking(KVDtype.q8_0),
            device_form() |> asking(KVDtype.tq4), device_form(ring = false),
            device_form(ring = false) |> asking(KVDtype.q8_0),
            device_form(regions = 2l, ctx = REGIONS_RIG_CTX)])
    }
    if (vk) {
        forms |> push(vk_census_form())
    }
    if (any_arm_on(GEMMA3_METAL_ARMS)) {
        forms |> push(metal_form(ctx = MATRIX_RING_CTX))
    }
    forms |> push(matrix_cpu_form())
    return <- forms
}

//! gemma-3-1b's cells off the block's own arm, each on its form:
//! the Metal rows, their CPU truths and the fused chains, the
//! census's Vulkan row; false where the form is the block's own
def private g3_other_cells(t : T?; var k : G3MetalKept; var m : Model; form : ModelForm) : bool {
    if (form.on_metal) {
        matrix_family_metal(t, m, form, matrix_row_gemma3_1b(), k.family)
        matrix_ring_metal(t, m, form, matrix_ring_gemma3_1b())
        prefill_parity_metal(t, m, form, prefill_gemma3_1b(), k.prefill)
        return true
    }
    if (alone_pass_of(form, matrix_cpu_form())) {
        matrix_family_cpu(t, m, form, matrix_row_gemma3_1b(), k.family)
        prefill_parity_cpu(t, m, form, k.prefill)
        fused_decode_row_cell(t, m, form, fused_row_gemma3_1b())
        return true
    }
    if (vk_census_form_is(form)) {
        coverage_vk_model(t, m, form, cov_vk_c37_gemma3())
        return true
    }
    return false
}

let private PAST_SLIDING_WINDOW = 2l * PF_WINDOW_ROWS + 76l   //! a third prefill window: it starts a window's rows past the 512-key sliding window

[test(description = "On the whole-model driver gemma-3-1b Q8_0 with sandwich norms and sliding layers on the device keeps forced-feed logits within 16 percent of max logit of the CPU chain at prompts of 40, 8, 520 and 600 tokens, and its window chain and token command agree over 8 and 40 tokens within 6x of the CPU chain's own two paths; over a q8_0 K/V mirror it keeps the forced-feed logits within 16 percent past the sliding window, over a tq4 mirror the teacher-forced perplexity within 1.05x; its ringed K/V mirrors match a flat every-row mirror bit for bit over 1300 prose ids and eight steps on the default and the q8_0 mirror; over two mirror regions, with sandwich norms, sliding and global layers and GeGLU, batched steps over two sessions - at one window, past the unsplit position, and before any one-row step - read what each session reads alone, and on the ringed load the ring floor sits at the prompt's rows less the slack: a call under it panics, a prompt from 0 clears it, and a park, export or import under it refuses while every row moves with it; on the default device form the packed seat declines a sliding layer's ring, so an embedding batch takes a window a text, prefills every text on the device and matches its singles at cosine above 0.9999; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the portable-pinned CPU form, on every box, the fused decode chains match the per-op path's greedy ids and final logits bit for bit; on the Metal home's blob twin, one load on a das_metal build, metal_needs derives the family's bits, all three Metal paths engage on f16 and q8_0 sessions, a metal-overrides generate emits the CPU truth's ids exactly and a continuation prefill holds the CPU's logits, the ringed Metal mirror emits the flat mirror's tokens past its wrap, and the narrowed last-row prefill tail reads within 0.05 of every row's with the same argmax; on the Vulkan home's default form a prefill and two steps feed the kernel census; each form loads once")]
def test_gpu_resident_gemma3(t : T?) {
    let vk = coverage_vk_on()
    var arms <- [GEMMA3_FILE.arm, COVERAGE_VK_ARM]
    arms |> push_from(GEMMA3_METAL_ARMS)
    return if (!family_on(t, "gemma3") || !arms_on(t, arms))
    let own = empty(arm_why(GEMMA3_FILE.arm))
    var r : G3Rows
    var k : G3MetalKept
    //! forty and eight tokens in one window; 520 and 600 across two, the sliding window engaged, where the prompt window is the cells' own
    r.feeds <- assert_pf_window(t) ? [40l, 8l, PF_WINDOW_ROWS + 8l, PF_WINDOW_ROWS + 88l] : [40l, 8l]
    var forms <- g3_forms(t, own, vk)
    with_cuda_mmq_winners() {
        let ran = with_cpu_and_gpu_model(t, "llm", "gemma-3-1b-it-Q8_0.gguf", GEMMA_RIG_CTX,
                forms, own, parity = own) $(var m, form) {
            return if (g3_other_cells(t, k, m, form) || !own)
            g3_rows(r, m, form)
            if (device_default(form)) {
                embeddings_ring_layers_cell(t, m)
            }
        }
        if (ran && own) {
            g3_rows_within(t, r)
        }
    }
    matrix_family_verdict(t, matrix_row_gemma3_1b(), k.family)
    prefill_parity_verdict(t, k.prefill)
    delete r
    delete k
    delete arms
    delete forms
}

//! what the gemma-3-1b file's forms collect: the default device form's feeds and path gaps with their CPU side on its codec
//! (`main`), the q8_0 mirror's forced feed past the sliding window and the tq4 mirror's teacher-forced walk (each a CPU side,
//! a device side), the rings (the default codec ringed and flat, the q8_0 mirror ringed and flat), and the rig's cells on the
//! two-region form; each `_ran` says a form collected
struct G3Rows {
    feeds : array<int64>
    main : MainPair
    rows : GemmaRows[2]
    codec : CodecRows
    regions : Held
}

let private G3_GAPS = fixed_array(8l, 40l)
//! the q8_0 feed runs past the sliding window; a 4-bit mirror reads one step 5.0 of a 33.7 max logit from the CPU's, so no
//! logit bar holds its walk with room
let private G3_CODEC = CodecCells(q8_feed = PAST_SLIDING_WINDOW, tq4_walk = (n = PAST_SLIDING_WINDOW, steps = 80l),
    ring_kv = KVDtype.q8_0, q8_note = "the third window's f16 shadow starts past the sliding layers' window",
    tq4_note = "its teacher-forced walk")

//! one form's collection of the gemma-3-1b file's cells into `r`
def private g3_rows(var r : G3Rows; m : Model; form : ModelForm) {
    var inscope gaps <- [for (g in G3_GAPS); g]
    let side = main_side(r.main, form)
    if (form.on_gpu && form.regions == 2l) {
        regions_batched_once_rows(r.regions, m)
        regions_ring_floor_rows(r.regions, m)
        return
    }
    if (side == MainSide.device) {
        r.rows[1] <- gemma_rows(m, form, r.feeds, gaps, array<GemmaWalk>())
    } elif (side == MainSide.cpu) {
        r.rows[0] <- gemma_rows(m, form, r.feeds, gaps, array<GemmaWalk>())
    }
    codec_rows(r.codec, m, form, G3_CODEC)
}

//! the gemma-3-1b file's compares on what its forms collected, each cell where its forms ran
def private g3_rows_within(t : T?; r : G3Rows) {
    var inscope gaps <- [for (g in G3_GAPS); g]
    if (r.main.ran) {
        gemma_rows_within(t, "gemma-3-1b", r.rows[1], r.rows[0], gaps, array<GemmaWalk>())
    }
    codec_rows_within(t, "gemma-3-1b Q8_0", r.codec, G3_CODEC)
    held_within(t, r.regions, "gemma-3-1b Q8_0, two regions: ", false)
    with_dasllama_jobque_() {
        rings_within(t, "gemma-3-1b Q8_0", GEMMA_RING_IDS, r.codec.rings, r.main.kv,
            G3_CODEC.ring_kv)
    }
}

//! the driver's load-time lines, read off two loads in the test child: no in-process cell reads what to_log prints
[test(description = "The whole-model driver's load log on gemma-3-1b names its rings on a ringed load, its every-row mirror on a flat one, and the host embed only where the token gather is pinned off", timeout = 1200)]
def test_gpu_resident_gemma3_load_log(t : T?) {
    return if (!family_on(t, "gemma3"))
    t |> run("gemma-3-1b Q8_0: the resident driver's load log names the rings, the flat mirror and the host embed under their pins") @(t : T?) {
        ring_load_announces(t, GEMMA3_FILE)
    }
}

// gemma-3-4b Q8_0 (the dense base at its wider geometry): arm `g3b`, its block under the MMQ route pinned `CUDA_MMQ_WINNERS`
// on the CUDA home (`with_cuda_mmq_winners`), where it adds the home's own forced feeds - one window again and two. Skips off
// the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the file under models_dir(), or when the
// residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror). On its parity row's CPU form, on every box,
// the row's frozen greedy ids. The Metal form (the blob twin; arm
// `fam-gemma3`) carries the family matrix's row, the portable-pinned
// CPU form its CPU truth.
let private GEMMA3_4B_FILE = GemmaFile(file = "gemma-3-4b-it-Q8_0.gguf", arm = "g3b")

[test(description = "On the whole-model driver gemma-3-4b Q8_0 keeps forty-token forced-feed logits within 16 percent of max logit of the CPU chain and its 150-position teacher-forced perplexity within 1.05x of the CPU chain's; on the CUDA driver, the sliding and global layers at head 256, forced feeds over one window and two, eight steps each, hold the same bar with every position served; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Metal home's blob twin, one load on a das_metal build, metal_needs derives the family's bits, all three Metal paths engage, and a metal-overrides generate emits the portable CPU truth's ids exactly; each form loads once")]
def test_gpu_resident_gemma3_4b(t : T?) {
    return if (!family_on(t, "gemma3") || !arms_on(t, [GEMMA3_4B_FILE.arm, "fam-gemma3"]))
    let own = empty(arm_why(GEMMA3_4B_FILE.arm))
    let walks <- [(n = 150l, steps = 150l)]
    var feeds <- [40l]
    if (gpu_want_cuda()) {   //! the CUDA home's own cells: one window again and a two-window prompt
        feeds |> push(40l)
        feeds |> push(PF_WINDOW_ROWS + 88l)
    }
    var rows : GemmaRows[2]
    var fam : MatrixFamilyKept
    var forms : array<ModelForm>
    if (own) {
        forms |> push(device_form())
    }
    metal_row_forms(forms, empty(arm_why("fam-gemma3")), MATRIX_CTX, matrix_cpu_form())
    with_cuda_mmq_winners() {
        let ran = with_cpu_and_gpu_model(t, "llm", "gemma-3-4b-it-Q8_0.gguf", GEMMA_RIG_CTX,
                forms, own, parity = own) $(var m, form) {
            if (form.on_metal) {
                matrix_family_metal(t, m, form, matrix_row_gemma3_4b(), fam)
            } elif (alone_pass_of(form, matrix_cpu_form())) {
                matrix_family_cpu(t, m, form, matrix_row_gemma3_4b(), fam)
            } else {
                rows[form.on_gpu ? 1 : 0] <- gemma_rows(m, form, feeds, array<int64>(), walks)
            }
        }
        if (ran) {
            gemma_rows_within(t, "gemma-3-4b", rows[1], rows[0], array<int64>(), walks)
        }
    }
    matrix_family_verdict(t, matrix_row_gemma3_4b(), fam)
    delete rows
    delete forms
}

// gemma-2-2b Q4_K_M (gemma2's K-quant carrier: K-quant sites beside
// the wide heads and q8_0-KV block attention). Its Metal form (the
// blob twin of KERNEL_COVERAGE_CTX positions) carries the family
// matrix's row and the sliding-window row (arm `fam-gemma2`) and the
// kernel census's zoo row (arm `coverage`), the portable-pinned CPU
// form their CPU truths; its parity row's CPU form, on every box, the
// row's frozen greedy ids. Skips off the JIT or without the file
// under models_dir().

[test(description = "On the Metal home's blob twin of gemma-2-2b Q4_K_M, one load on a das_metal build, metal_needs derives the family's bits, all three Metal paths engage on f16 and q8_0 sessions, a metal-overrides generate emits the portable CPU truth's ids exactly and a continuation prefill holds its logits; with the sliding window shrunk to 160 a generate crossing it emits the CPU truth's ids exactly; three K/V codecs' prompts, continuations and batched steps feed the kernel census; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_gemma2_k4(t : T?) {
    return if (!family_on(t, "gemma2"))
    var fam : MatrixFamilyKept
    var win : MatrixWindowKept
    var forms : array<ModelForm>
    metal_row_forms(forms, any_arm_on(["fam-gemma2", "coverage"]), KERNEL_COVERAGE_CTX,
        matrix_cpu_form())
    with_cpu_and_gpu_model(t, "llm", "gemma-2-2b-it-Q4_K_M.gguf", GEMMA_RIG_CTX,
            forms, parity = true) $(var m, form) {
        if (form.on_metal) {
            matrix_family_metal(t, m, form, matrix_row_gemma2_2b_k4(), fam)
            matrix_gemma2_window_metal(t, m, form, win)
            coverage_zoo_metal(t, m, form, cov_zoo_c5_gemma2())
        } elif (alone_pass_of(form, matrix_cpu_form())) {
            matrix_family_cpu(t, m, form, matrix_row_gemma2_2b_k4(), fam)
            matrix_gemma2_window_cpu(t, m, form, win)
        }
    }
    matrix_family_verdict(t, matrix_row_gemma2_2b_k4(), fam)
    matrix_gemma2_window_verdict(t, win)
    delete forms
}

// gemma-3-12b Q4_K_M (gemma3's K-quant carrier: k4 and k6 planes, the
// Q6_K token table), the gemma3-12b category's. Its Metal form (the
// blob twin; arm `fam-gemma3`) carries the family matrix's row, the
// portable-pinned CPU form its CPU truth; its parity row's CPU form,
// on every box, the row's frozen greedy ids. Skips off the JIT,
// without the file under models_dir(), or where the run names no
// gemma3-12b category.

[test(description = "On the Metal home's blob twin of gemma-3-12b Q4_K_M, one load on a das_metal build, metal_needs derives the family's bits, all three Metal paths engage, and a metal-overrides generate emits the portable CPU truth's ids exactly; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_gemma3_12b(t : T?) {
    return if (!family_on(t, "gemma3"))
    var fam : MatrixFamilyKept
    var forms : array<ModelForm>
    metal_row_forms(forms, empty(arm_why("fam-gemma3")), MATRIX_CTX, matrix_cpu_form())
    with_cpu_and_gpu_model(t, "llm", "gemma-3-12b-it-Q4_K_M.gguf", GEMMA_RIG_CTX,
            forms, parity = true) $(var m, form) {
        if (form.on_metal) {
            matrix_family_metal(t, m, form, matrix_row_gemma3_12b_k4(), fam)
        } elif (alone_pass_of(form, matrix_cpu_form())) {
            matrix_family_cpu(t, m, form, matrix_row_gemma3_12b_k4(), fam)
        }
    }
    matrix_family_verdict(t, matrix_row_gemma3_12b_k4(), fam)
    delete forms
}
