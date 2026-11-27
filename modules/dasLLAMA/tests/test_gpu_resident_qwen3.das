options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen3)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require dasllama/dasllama_kv_dtype   // KVDtype: the tq4 forms' mirror ask
require dasllama/dasllama_math   // with_dasllama_jobque_: the compares after the block decode off the queue
require ?cuda _cuda_feed   // LOGIT_BAR_REL: the CUDA home's dense bar
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows, the CUDA feed collector, the gather cell's halves
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the files' model_specs parity rows on their blocks
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX, the fuse bisect's bits
require _resident_feed   // Held, held_within, held_pf_window
require _kv_codec_cells   // the tq4 distance and stored-rows halves, the per-op cells
require _dflash_cells   // the block drafter's walks: lossless, the planes' tags, the discrete legs
require _chat_cells   // chat_qwen3_cells: the uncached CPU form's chat cells
require _kv_codec_cpu_cells   // kv_codec_keeps_tq4_cell: the uncached CPU form's step-down control
require _embedding_cells   // the Qwen3-Embedding file's cells on its forms
require ?das_metal dasllama/dasllama_metal_prefill   // the prefill mirror's pin: the Metal cell's
require dasllama/dasllama_gpu_tier   // reset_gpu_cpu_passes: the passes the by-design cells leave
require ?vulkan dasllama/dasllama_vulkan_dflash   // nolint:STYLE030,LINT019 - the Vulkan arm registers at init
require ?cuda dasllama/dasllama_cuda_dflash   // nolint:STYLE030,LINT019 - the CUDA arm registers at init
require ?vulkan dasllama/dasllama_vulkan_common   // the box's coopmat mode: the sweep's forms
require dasllama/dasllama_convert   // quantize_q8_k_into_ptr: the layout cell's GEMV
require _kquant_cells   // dequant_plane_prefix, read_prefix: the K-quant layout cell's file truth
require _compares   // check, the bars: the K-quant layout cell's compares
require _metal_matrix_cells   // the support matrix's qwen3 rows: the Metal forms' cells
require _metal_prefill_cells   // the 4B Q4_K_M file's prefill parity on its Metal form
require _fused_decode_cells   // the fused decode's K-quant rows on the 4B files' CPU forms
require _kernel_coverage_cells   // the coverage zoo's qwen3 row, the coopmat mode sweep's

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // each block's CPU form, and the by-design passes, prefill on the CPU
}

// The dense qwen3 family on the whole-model driver under DASLLAMA_GPU=1, on the Vulkan home or under DASLLAMA_CUDA=1 on the
// CUDA home. Each model file is ONE block (`with_cpu_and_gpu_model`): its forms load once each and the CPU form once a mirror
// codec, the cells collect what the compares read and the compares follow the block.
// - Qwen3-0.6B Q8_0 (`test_gpu_resident_qwen3`, the q/k-norm carrier, no attention bias): at two regions the rig's step
//   cells - the interleaved single steps, the single steps past the wide position on the wide twin, a batched step before
//   any one-row step, the batched steps at one window and past the unsplit position (the N-row token command's q/k norm in
//   the form the one-row command takes, bit for bit against the sessions alone), and the window's fused q/k norm, rope and
//   store pass against its split pair; DASLLAMA_VK_FUSE_BISECT one load a bit within the split bar of the unsplit load;
//   under a tq4 K/V mirror (a Q/K/V-biased Qwen2.5 serves a tq4 ask at q8_0, so it cannot carry these) the distance form at
//   one window and two and with the prompt on the CPU rails, layer 0's stored rows against the CPU cache's, and every cell of
//   the rig on the rotated 4-bit codec (the staged stores and the rotate-and-quantize pass at one row and at N rows, the
//   decode attention's tq4 stamps, the prefill's attention over the f16 shadow, the rows a pass brings down as the CPU
//   cache's own bytes; the tolerance cells hold the rig's tq4 bars); with the whole-model route off the per-op rails under
//   tq4 and q8_0 sessions, and the load's decline reason; on the CUDA home the forced feed over one window and two, the
//   fused CuQknRopeKv pass, with the driver's counters and their control. On its uncached CPU form, on every box, the
//   file's own CPU cells: the chat renders (`_chat_cells.das`) and the codec step-down rule's control
//   (`_kv_codec_cpu_cells.das`); on its parity row's CPU form the row's frozen greedy ids (`parity_spec_cell`).
// - Qwen3-Embedding-0.6B Q8_0 (`test_gpu_resident_qwen3_embedding`, `_embedding_cells.das`): on the driver's default form
//   the whole-model driver serving the embeddings from the device and every region held by a live session, on a region
//   capped at EMB_REGION_ROWS the batch windows the region bounds; on its q8, uncached and q4_0 CPU forms, on every box, the
//   card's matrix, the batch against its singles, the lone forwards, and the device cells' CPU halves; on its Metal form -
//   the file's blob twin, a skipped cell on a build without das_metal - the Metal cell holds the window's forms, the
//   split-K leg and the card's matrix, and its one-row vector meets the f32 CPU form's after the block.
// - Qwen3-4B-Instruct-2507: the Q8_0 file's CUDA forced feed at its 2560-wide rows over 36 layers, and on every box its
//   parity row's frozen greedy ids on the row's CPU form (`test_gpu_resident_qwen3_4b`, `--category qwen3-4b`,
//   `../TEST_CATEGORIES.md`).
// - Qwen3-4B Q4_K_M (the base model the Instruct-2507 files fine-tune; a format carrier, in every run,
//   `test_gpu_resident_qwen3_4b_kq`): on the Vulkan home its tied K-quant table gathered on the device beside its per-op rails
//   under tq4 and q8_0 sessions, the K-quant format of a codec's parity; with its regular DFlash drafter's Q8_0 and Q4_K_M
//   files (`drafters/Qwen3-4B-DFlash-b16-GGUF/`) the CPU form's lossless walks over a chat turn and each file's planes tagged by
//   disk type, and on either discrete home the drafter form (an f16 mirror of DISCRETE_MIRROR_CTX positions asked whole, the
//   sessions cut to DISCRETE_CTX) each file's walk on the device arm against the plain walk. A drafter cell skips without its
//   file, the discrete legs on a build with no discrete DFlash arm.
// Every test skips off the JIT or without the file under models_dir(); each device form skips as a cell of its own without
// DASLLAMA_GPU=1 on a device the driver serves (the 4B Q4_K_M file's CPU form, the drafter's CPU cells, runs on every box), or
// where the plan does not fit the card. The tq4 forms skip where DASLLAMA_VK_KV32=1 arms an f32 mirror over the codec;
// the two-window feeds where the prompt window is not PF_WINDOW_ROWS rows. Under DASLLAMA_CUDA=1 the fuse bisect, the fused
// q/k norm, the tq4 distance and stored-rows cells, the per-op cells and the 4B Q4_K_M file - the Vulkan home's - give way
// to the CUDA feeds. A parity row's form (the row's load mode and K-quant knob under DASLLAMA_PIN_BACKEND's pin) loads on
// every box and skips where the box cannot take that pin; the 4B Q8_0 file's CUDA feed skips as a cell of its own off the
// CUDA home.

//! no logit bar holds a 4-bit mirror on a 0.6B, so the codec bars hold ratios of the rows' summed largest differences: the
//! device's distance from the f16 chain, of the CPU codec chain's own, reads 0.95-1.01 (the tq4 rows 1.17 on the RTX PRO 4500,
//! driver 595), the f16 rows one step off 1.6-3.7
let private TQ4_DIST_REL = 1.25
//! the two codec chains part further on Qwen3-0.6B than on the biased qwen2 carrier: 0.82 of the CPU codec chain's distance from
//! f16 against its 0.61 (the llama files read 1.0-1.1)
let private TQ4_PAIR_REL = 1.0
//! layer 0's stored bytes against the CPU cache's: the q/k norm before the rope lands the device's normed k on the 4-bit quant's
//! edge on 0.049 of layer 0's K bytes; the steps' V rows at 0.002-0.01 (one Q8_0 feed on both chains); the prefill's V rows at
//! 0.12 - its GEMM reads f16 rows where the CPU reads Q8_0 blocks - against 0.97 for a row held to the next one (the control)
let private TQ4_ROWS_BARS = StoredRowsBars(k = 0.08, v_step = 0.05, v_prefill_of_ctrl = 0.6)
//! the per-op rails: layer 0's cache bytes, the device arm's against the CPU chain's - one input, so they agree but for a quant
//! on a rounding edge (0 of the bytes under tq4, 2e-5 under q8_0). The device's summed distance from the f16 chain reads 2.3
//! against the CPU codec chain's 48.7 where the device prefilled and 38.4 against 42.6 over rows the CPU quantized; the f16
//! rows one step off sum to 80-95
let private QWEN3_06B_PEROP_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)
//! the 4B Q4_K_M's per-op rails: the stored bytes differ from the CPU chain's at 0 to 4e-5 of them; the device's summed distance
//! from the f16 chain reads 0.28 of the CPU codec chain's own under tq4 (3.3 against 12.0) and 0.80 under q8_0 (3.3 against 4.1),
//! the f16 rows one step off 5.7 and 16 times it
let private QWEN3_4B_PEROP_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)
//! the Qwen3-4B Q4_K_M chain's device-vs-CPU distance: its K-quant sites read 0.38-0.62 of a 7.3-11.3 max logit (0.073 at its
//! widest step, the argmax the CPU's at every row) - the chain's rounding: the gathered rows are the class lane decode
//! (`test_vkd_emb_gather_family`); the one-step-off control reads 5.4 and past
let private QWEN_KQ_BAR_REL = 0.10


//! the CUDA feeds' compares after the block at the CUDA home's dense bar
[unused_argument(file, c)]   // without the cuda module no feed ran
def private qwen3_cuda_within(t : T?; file : string; c : DenseFeeds) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        dense_feeds_within(t, file, c, LOGIT_BAR_REL)
    }
}

//! the tq4 mirror's distance form and stored rows on Qwen3-0.6B: the prompts, the device's rows at each (the plain tq4 form)
//! and with the prompt on the CPU rails (the tq4 form with the planes on the host), layer 0's stored rows on both sides, and the
//! CPU form's rows at tq4 and at f16 for each prompt; each `_ran` says its device form collected
struct Qwen3CodecRows {
    feeds : array<int64>
    gpu : array<array<array<float>>>
    ran : bool
    sides : CodecSides
    cpu : array<FeedRows>
    f16 : array<FeedRows>
    h : Held
}

//! what the Qwen3-0.6B file's forms collect: the CUDA feeds, the two-region form's step cells, the fuse bisect's base rows and
//! its bits' readings, the tq4 codec cells, and the rig's cells on the tq4 two-region forms
struct Qwen3Rows {
    feeds : DenseFeeds
    regions : Held
    bisect_base : array<array<float>>
    bisect_ran : bool
    bisect : Held
    codec : Qwen3CodecRows
    tq4_regions : Held
    perop : PeropRun
    matrix : MatrixFamilyKept   //! the support matrix's row, its twin's side
    sweep_legs : int   //! the coopmat sweep's forms the body has seen
}

//! the per-op cells on Qwen3-0.6B, a form each: a tq4 session above the device prefill's floor and under it, a q8_0 session
def private qwen3_06b_perop_cells() : array<PeropCell> => <- [PeropCell(n = 40l, kv = KVDtype.tq4, bars = QWEN3_06B_PEROP_BARS),
    PeropCell(n = 20l, kv = KVDtype.tq4, bars = QWEN3_06B_PEROP_BARS), PeropCell(n = 40l, kv = KVDtype.q8_0, bars = QWEN3_06B_PEROP_BARS)]

//! the Qwen3-0.6B file's forms: two regions (the step cells), on the CUDA home the default (the feeds); on the Vulkan home the
//! fuse bisect's load a bit (no bit first, the base) and the per-op form; under a tq4 mirror (a codec the environment keeps off
//! held as a skip) on the Vulkan home the plain form (the distance form and the stored rows) and the planes on the host (the
//! prompt on the CPU rails), on both homes two regions (the rig), the same with the planes on the host (the pin and the gap
//! pass), at the capped context (the cap pass), and the per-op form asking the codec (the decline reason); and the uncached
//! CPU form whose cells stand alone
def private qwen3_forms(var r : Qwen3Rows) : array<ModelForm> {
    let cuda = gpu_want_cuda()
    var forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    if (cuda) {
        forms |> push(device_form())
    } else {
        forms |> reserve(length(forms) + length(FUSE_BISECT_BITS) + 2)
        forms |> push(device_form(regions = 2l, ctx = REGIONS_RIG_CTX, fuse_bisect = 0l))
        for (bit in FUSE_BISECT_BITS) {
            forms |> push(device_form(regions = 2l, ctx = REGIONS_RIG_CTX, fuse_bisect = bit))
        }
        r.perop.cells <- qwen3_06b_perop_cells()
        forms |> push_from(perop_forms(r.perop))
    }
    forms |> push(cpu_cells_form(uncached = true))
    if (any_arm_on(["fam-qwen3", "coverage"])) {
        forms |> push(metal_form(ctx = KERNEL_COVERAGE_CTX))
    }
    if (any_arm_on(["fam-qwen3"]) && empty(metal_build_why())) {
        forms |> push(cpu_cells_form(backend = "portable"))
    }
    if (!cuda) {
        sweep_forms(forms, true)
    }
    let why = codec_env_why(KVDtype.tq4)
    if (!empty(why)) {
        r.codec.h |> held_skip("the tq4 mirror's cells: {why}")
        return <- forms
    }
    if (!cuda) {
        forms |> push(device_form() |> asking(KVDtype.tq4))
        forms |> push(device_form(host = true) |> asking(KVDtype.tq4))
    }
    forms |> push_from(regions_rig_forms(KVDtype.tq4))
    return <- forms
}

//! The coverage census's coopmat mode sweep forms of a vehicle, on the
//! Vulkan home where the arm reaches them and the device opens: one
//! census form (`vk_census_form`) a mode the driver's prepare reads at
//! load - the box's, mm and sdot4, then where `twin` the f16 and int8
//! twin legs
def private sweep_forms(var forms : array<ModelForm>; twin : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (!any_arm_on(["coverage-vk"]) || !vk_moe_init())
        var inscope modes <- [g_gpu.coopmat_mode, COOPMAT_MM, COOPMAT_SDOT4]
        if (twin) {
            modes |> push(COOPMAT_F16)
            modes |> push(COOPMAT_INT8)
        }
        forms |> reserve(length(forms) + length(modes))
        for (mode in modes) {
            forms |> push(vk_census_form(mode))
        }
    }
}

//! the sweep vehicle `i`'s row on its next mode form, the first three
//! legs on the common modes, the rest the twin legs
def private sweep_cell(t : T?; var legs : int&; var m : Model; form : ModelForm; i : int) {
    coverage_vk_model(t, m, form, cov_vk_sweep_row(i, legs >= SWEEP_COMMON_LEGS))
    legs++
}

let private SWEEP_COMMON_LEGS = 3   //! the box's mode, mm and sdot4

//! one form's share of the tq4 codec cells: the device's rows on the tq4 forms, the CPU form's at tq4 and f16 on its tq4 run
def private qwen3_codec_rows(t : T?; var c : Qwen3CodecRows; m : Model; form : ModelForm) {
    if (form.on_gpu && form.host) {
        codec_up_rows(t, c.sides, m, KVDtype.tq4)
    } elif (form.on_gpu) {
        c.gpu |> reserve(length(c.feeds))
        for (n in c.feeds) {
            c.gpu |> emplace <| codec_device_rows(t, m, n, KVDtype.tq4, false)
        }
        codec_stored_device_rows(t, c.sides, m, KVDtype.tq4)
        c.ran = true
    } elif (form.kv == KVDtype.tq4 && (c.ran || c.sides.up_ran)) {
        var f16 = form
        f16.kv = KVDtype.f16
        c.cpu |> reserve(length(c.feeds))
        c.f16 |> reserve(length(c.feeds))
        for (n in c.feeds) {
            c.cpu |> emplace <| resident_feed_rows(m, form, n, CODEC_FED_STEPS)
            c.f16 |> emplace <| resident_feed_rows(m, f16, n, CODEC_FED_STEPS)
        }
        if (c.ran) {
            c.sides.stored_host <- codec_stored_host(m, KVDtype.tq4)
        }
    }
}

//! the tq4 codec cells' compares after the block: each prompt's distance form, the distance form with the prompt on the CPU
//! rails (its prompt's row out of the sums), and layer 0's stored rows
def private qwen3_codec_within(t : T?; c : Qwen3CodecRows) {
    held_within(t, c.h, "Qwen3-0.6B tq4: ", false)
    return if (empty(c.cpu))
    for (n, i in c.feeds, count()) {
        if (c.ran) {
            unsafe {
                t |> run("Qwen3-0.6B: the tq4 K/V mirror - resident prefill + decode over {n} tokens against the CPU chain's tq4 and f16 rows") @capture(& c)(t : T ?) {
                    codec_distances_within(t, "tq4, {n} tokens", c.gpu[i], c.cpu[i].rows, c.f16[i].rows, 0, TQ4_DIST_REL,
                            TQ4_PAIR_REL) $(id) {
                        return feed_piece(c.cpu[i], c.f16[i], id)
                    }
                }
            }
        }
        if (c.sides.up_ran && n == 40l) {
            unsafe {
                t |> run("Qwen3-0.6B: the device steps over tq4 rows the CPU prefill wrote") @capture(& c)(t : T ?) {
                    codec_distances_within(t, "tq4, 40 tokens, the prompt on the CPU rails", c.sides.up, c.cpu[i].rows, c.f16[i].rows, 1,
                            TQ4_DIST_REL, TQ4_PAIR_REL) $(id) {
                        return feed_piece(c.cpu[i], c.f16[i], id)
                    }
                }
            }
        }
    }
    if (c.ran) {
        codec_stored_cell(t, "Qwen3-0.6B: the tq4 mirror's stored rows against the CPU cache's, layer 0", KVDtype.tq4, c.sides,
            TQ4_ROWS_BARS)
    }
}

//! one form's collection of the Qwen3-0.6B file's cells into `r`; the per-op forms', the regions forms' and the fuse bisect's
//! own cells run on their load
def private qwen3_rows(t : T?; var r : Qwen3Rows; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        matrix_family_metal(t, m, form, matrix_row_qwen3_0_6b(), r.matrix)
        coverage_zoo_metal(t, m, form, cov_zoo_c7_qwen3())
    } elif (alone_pass_of(form, cpu_cells_form(backend = "portable"))) {
        matrix_family_cpu(t, m, form, matrix_row_qwen3_0_6b(), r.matrix)
    } elif (form.coopmat >= 0) {
        sweep_cell(t, r.sweep_legs, m, form, 0)
    } elif (form.uncached) {
        chat_qwen3_cells(t, m)
        kv_codec_keeps_tq4_cell(t, m)
    } elif (form.perop) {
        if (form.kv_asked) {
            regions_route_off_cell(t, m)
        } else {
            perop_next(t, r.perop, m)
        }
    } elif (form.on_gpu && form.fuse_bisect == 0l) {
        r.bisect_base <- regions_bisect_base(m)
        r.bisect_ran = true
    } elif (form.on_gpu && form.fuse_bisect > 0l) {
        if (r.bisect_ran) {
            regions_bisect_rows(r.bisect, m, form.fuse_bisect, r.bisect_base)
        } else {
            r.bisect |> held_skip("fuse bisect bit {form.fuse_bisect}: the base form with no bit split did not run")
        }
    } elif (form.on_gpu && form.regions == 2l && form.kv_asked) {
        regions_rig_form_rows(t, r.tq4_regions, m, form, SPLIT_BAR_REL)
    } elif (form.on_gpu && form.regions == 2l) {
        regions_interleaved_rows(r.regions, m)
        regions_wide_rows(r.regions, m)
        regions_batched_first_rows(r.regions, m)
        regions_batched_rows(r.regions, m)
        regions_split_rows(r.regions, m)
        let qkn_why = vulkan_home_why("the window's fused q/k norm is the Vulkan driver's lever")
        if (!empty(qkn_why)) {
            r.regions |> held_skip(qkn_why)
        } else {
            regions_window_qkn_rows(r.regions, m)
        }
    } elif (form.on_gpu && form.kv_asked) {
        qwen3_codec_rows(t, r.codec, m, form)
    } else {
        dense_feeds_rows(r.feeds, m, form, "qwen3")
        if (!form.on_gpu) {
            qwen3_codec_rows(t, r.codec, m, form)
        }
    }
}

[test(description = "On Qwen3-0.6B Q8_0 over two mirror regions two sessions stepped interleaved, past the wide position on the wide twin, batched first, batched at one window and past the unsplit position each read what that session reads alone, and the window's fused q/k norm, rope and store pass reads within the split bar of the split pair; every DASLLAMA_VK_FUSE_BISECT bit serves one-row logits within the split bar of the unsplit load and the split-pair rows batch bit for bit; a tq4 K/V mirror sits within 1.25 times the CPU tq4 chain's distance from the f16 rows over one and two windows and over rows the CPU prefill wrote, its layer 0 stored rows match the CPU cache's within the byte bars, and the rig's cells - interleaved, continued, batched, device-home, parked, guarded, reported, scheduled, the pin and the gap and cap passes - hold the tq4 bars; on the per-op rails tq4 and q8_0 sessions keep layer 0's cache bytes within 5 percent of the CPU chain's and the device no further from f16 than the CPU codec chain, and the route-off load reports never attempted; on the CUDA driver the fused CuQknRopeKv pass keeps forced-feed logits within 6 percent of the CPU chain over one and two windows; on the uncached CPU form, on every box, the thinking-off generation prompt and the preserve_thinking replays render, and bias-free Qwen3-0.6B keeps a tq4 session at tq4; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; on the Metal home's blob twin, one load on a das_metal build, the support matrix's qwen3 row - the path cells on f16 and tq4 K/V, the metal-overrides generate token for token the portable CPU form's, the continuation within 4.0 - and the coverage zoo's qwen3 row; on the Vulkan home the coverage census's coopmat sweep, a load a mode; each form loads once")]
def test_gpu_resident_qwen3(t : T?) {
    return if (!family_on(t, "qwen3"))
    var r : Qwen3Rows
    if (gpu_want_cuda()) {
        dense_feeds(r.feeds, [40l, TWO_WINDOWS])
    } elif (held_pf_window(r.codec.h)) {
        r.codec.feeds <- [40l, TWO_WINDOWS]
    } else {
        r.codec.feeds <- [40l]
    }
    var forms <- qwen3_forms(r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-0.6B-Q8_0.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
            qwen3_rows(t, r, m, form)
        }
    }
    matrix_family_verdict(t, matrix_row_qwen3_0_6b(), r.matrix)
    if (ran) {
        qwen3_cuda_within(t, "Qwen3-0.6B-Q8_0.gguf", r.feeds)
        held_within(t, r.regions, "Qwen3-0.6B Q8_0, two regions: ", false)
        held_within(t, r.bisect, "Qwen3-0.6B Q8_0, the fuse bisect: ", false)
        held_within(t, r.tq4_regions, "Qwen3-0.6B Q8_0, two regions, a tq4 mirror: ", false)
        with_dasllama_jobque_() {
            qwen3_codec_within(t, r.codec)
        }
    }
    delete r
    delete forms
}

let private EMB_FILE_CTX = 32768l   //! Qwen3-Embedding's own context: its cells read at it

//! what the Qwen3-Embedding file's forms collect: the device's mean read of the long text (empty where its cell skipped) and
//! the q8 CPU form's reads it is held to (`mean_ran` where collected), and whether the window-rows cell ran past its served
//! gate
struct Qwen3EmbRows {
    dev_mean : array<float>
    cpu_mean : EmbMeanRows
    mean_ran : bool
    window_ran : bool
    metal : EmbMetalRows
}

def private qwen3_emb_rows(t : T?; var r : Qwen3EmbRows; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        qwen3_emb_metal_cell(t, r.metal, m)
    } elif (form.on_gpu && form.ctx == EMB_REGION_ROWS) {
        r.window_ran = embeddings_window_rows_cell(t, m)
        reset_gpu_cpu_passes()   //! the cell's route-off leg passes to the CPU by design
    } elif (form.on_gpu) {
        r.dev_mean <- embeddings_on_the_resident_driver_cell(t, m)
        reset_gpu_cpu_passes()   //! the cell's route-off leg passes to the CPU by design
        embeddings_every_region_held_cell(t, m)
        reset_gpu_cpu_passes()   //! the cell's host-cached sessions pass to the CPU by design
    } elif (form.quant == QuantMode.q4_0) {
        embeddings_lone_forwards_q4_0_cell(t, m)
    } elif (form.quant == QuantMode.fp32) {
        embeddings_f32_one_rows(m, r.metal)
    } elif (form.uncached) {
        embeddings_card_cell(t, m)
        embeddings_batch_matches_single_cell(t, m)
    } elif (form.alone) {
        embeddings_lone_forwards_cell(t, m)
        if (!empty(r.dev_mean)) {
            r.cpu_mean <- embeddings_resident_mean_rows(m)
            r.mean_ran = true
        }
        if (r.window_ran) {
            embeddings_unserved_window_rows(t, m)
        }
        embeddings_metal_off_cell(t, m, r.metal)
    }
}

let private EMB_METAL_CTX = 4096l   //! the Metal form's context: the Metal cell's windows

//! the Qwen3-Embedding file's Metal cell on its Metal form's blob twin `m`, the prefill mirror pinned for it and put back
//! (`embeddings_metal_window_rows`)
def private qwen3_emb_metal_cell(t : T?; var r : EmbMetalRows; var m : Model) {
    unsafe {
        t |> run("an embedding's Metal window runs no classifier and feeds no decode mirror, a chat window does both, and the card's matrix holds (0.6B Q8_0)") @capture(& r, & m)(tt : T ?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                let prior_mirror = get_metal_prefill_mirror()
                set_metal_prefill_mirror(true)   // the chat control's mirror leg reads it
                embeddings_metal_window_rows(tt, m, r)
                set_metal_prefill_mirror(prior_mirror)
            }
        }
    }
}

[test(description = "Qwen3-Embedding 0.6B Q8_0 on one load a form: on the q8 load exactly the one-row text of a host batch runs `embed_forward` alone while multi-row texts share the window; loaded uncached it reads last-token pooling and an appended EOS off the file, its last-token read lands the model card's 2 x 2 cosine matrix within 0.02 while the mean read cannot separate the passages (control), and `embed_batch` gives each text its single `embed` vector at cosine above 0.9999 in one window, in 40-row split windows and under the 256-dim Matryoshka cut, in input order; loaded as q4_0 every text runs alone; each lone text lands its single vector at cosine above 0.9999 with a shifted-text control missing; the whole-model driver serves it on the device with no CPU-rail pass: the card matrix within 0.02, a batch packed in one scratch window matching its singles, and a long mean read equal to the CPU's; `embed_window_rows` is the armed driver's capped region context, so a batch past 256 rows takes the predicted count of scratch windows and matches its singles, and is EMBED_WINDOW_ROWS for an unserved model; with every device region held by a live session `embed_batch` takes host-cached sessions and no scratch window yet reads the same vectors as with a region free, and parking the streams frees every region; on Metal its embed rides one prefill window with no classifier and no decode mirror (a chat forward does both), takes split-K on q8 sites, matches the card matrix within 0.02, and its one-row vector is the CPU f32 form's; each form loads once")]
def test_gpu_resident_qwen3_embedding(t : T?) {
    return if (!family_on(t, "qwen3"))
    var r : Qwen3EmbRows
    var forms <- [device_form(host = true), device_form(host = true, ctx = EMB_REGION_ROWS, host_ctx = EMB_FILE_CTX), cpu_cells_form(),
        cpu_cells_form(uncached = true), cpu_cells_form(QuantMode.q4_0)]
    forms |> push(metal_form(ctx = EMB_METAL_CTX))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        forms |> push(cpu_cells_form(QuantMode.fp32))   // the Metal cell's reference
    }
    with_cpu_and_gpu_model(t, "llm", "Qwen3-Embedding-0.6B-Q8_0.gguf", EMB_FILE_CTX, forms) $(m, form) {
        qwen3_emb_rows(t, r, m, form)
    }
    embeddings_metal_f32_within(t, r.metal)
    if (r.mean_ran) {
        embeddings_resident_mean_within(t, r.dev_mean, r.cpu_mean)
    } elif (!empty(r.dev_mean)) {
        t |> failure("Qwen3-Embedding: the device's mean read has no CPU half - the q8 CPU form collected none")
    }
    delete r
    delete forms
}

[test(description = "On the CUDA whole-model driver Qwen3-4B Q8_0, its 2560-wide rows over 36 layers, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows with the fused q/k-norm rope pass; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_qwen3_4b(t : T?) {
    return if (!family_on(t, "qwen3"))
    var c : DenseFeeds
    var forms : array<ModelForm>
    let why = cuda_home_why()
    if (empty(why)) {
        dense_feeds(c, [40l, TWO_WINDOWS])
        forms |> push(device_form())
    } else {
        skip_cell(t, "Qwen3-4B-Instruct-2507-Q8_0.gguf: the CUDA forced feed: {why}")
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-4B-Instruct-2507-Q8_0.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
            dense_feeds_rows(c, m, form, "qwen3")
        }
    }
    if (ran) {
        qwen3_cuda_within(t, "Qwen3-4B-Instruct-2507-Q8_0.gguf", c)
    }
    delete c
    delete forms
}

let private GATHER_CTX = 4096l   //! the gather form's context: the cells' prompts are one window

[test(description = "On the whole-model driver Qwen3-4B Q4_K_M's tied K-quant table is gathered on the device by the window and every fed step off its token id, with forced-feed steps within 10 percent of max logit of the CPU chain; on the per-op rails tq4 and q8_0 sessions keep layer 0's cache bytes within 5 percent of the CPU chain's and the device no further from the f16 chain than the CPU codec chain; with its Q8_0 and Q4_K_M DFlash drafters every plane is tagged with its disk type's format and on the CPU arm, over a chat turn, the greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted; on the serving discrete home each drafter file's greedy walk on the device arm is the plain walk's on the same device, every pass ending on the device's top-k, the host's walk and selector drafting the same ids; each form loads once")]
def test_gpu_resident_qwen3_4b_kq(t : T?) {
    return if (!family_on(t, "qwen3"))
    let file = "Qwen3-4B-Q4_K_M.gguf"
    var g : EmbGather
    var run : PeropRun
    let vk_why = vulkan_home_why("the table gather and the per-op cells are the Vulkan home's")
    var forms <- empty(vk_why) ? qwen3_4b_kq_forms(run, g, file) : array<ModelForm>()
    if (!empty(vk_why)) {
        skip_cell(t, "{file}: {vk_why}")
    }
    forms |> push(discrete_drafter_form())
    forms |> push(cpu_cells_form())
    forms |> push(metal_form(required = true, ctx = HOST_CTX))
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-4B-Q4_K_M.gguf", GATHER_CTX, forms) $(var m, form) {
            if (form.on_metal) {
                metal_drafter_cell(t, m, "On the Metal home over the dense Qwen3-4B Q4_K_M trunk with its Q8_0 DFlash drafter, over a chat turn, the greedy walk with the drafter's pass on the CPU arm over the device's captures emits the plain greedy walk's ids token for token with drafts accepted; the device arm's lossless walk and its count against the CPU arm's are a counted skip",
                    "", QWEN3_4B_DRAFTER_FILE, "Qwen3-4B metal", false, QWEN3_4B_DEVICE_GAP)
            } elif (form.strict_ctx) {
                qwen3_4b_drafter_discrete_cells(t, m)
            } elif (form.perop) {
                perop_next(t, run, m)
            } elif (form.on_gpu) {
                emb_gather_rows(g, m, form, file)
            } else {
                if (g.ran) {
                    g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
                }
                if (form.quant == QuantMode.q8 && form.alone) {
                    qwen3_4b_drafter_cpu_cells(t, m)
                }
            }
        }
    }
    if (ran) {
        if (!g.ran) {
            held_within(t, g.h, "", false)
        }
        with_dasllama_jobque_() {
            emb_gather_within(t, g, file, QWEN_KQ_BAR_REL)
        }
    }
    delete g
    delete forms
    delete run
}

//! the Vulkan home's K-quant forms of `file`: the per-op forms of `run` (its cells set here), and the table gather's form, or
//! its filtered arm held as a skip in `g`
def private qwen3_4b_kq_forms(var run : PeropRun; var g : EmbGather; file : string) : array<ModelForm> {
    run.cells <- [PeropCell(n = 40l, kv = KVDtype.tq4, bars = QWEN3_4B_PEROP_BARS),
        PeropCell(n = 40l, kv = KVDtype.q8_0, bars = QWEN3_4B_PEROP_BARS)]
    var forms <- perop_forms(run)
    let arm = arm_why("vk-gather")
    if (empty(arm)) {
        forms |> push(device_form(regions = 1l))
    } else {
        g.h |> held_skip("{file}: the tied K-quant table's gather: {arm}")
    }
    return <- forms
}

//! the regular DFlash drafter's CPU cells on the CPU form `m`, its context cut to HOST_CTX for them and put back, the Metal mode
//! off (put back): each drafter file's planes tagged with their disk types and its lossless walk over a chat turn
def private qwen3_4b_drafter_cpu_cells(t : T?; var m : Model) {
    with_cpu_drafter_scope(m) {
        unsafe {   //! the cells capture the loaded trunk by reference
            t |> run("Qwen3-4B, the Q8_0 DFlash drafter: every plane tagged with its disk type's format, and on the CPU arm, over a chat turn, the greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
                cpu_drafter_lossless(t, m, QWEN3_4B_DRAFTER_FILE, "Qwen3-4B cpu", true, true)
            }
            t |> run("Qwen3-4B, the Q4_K_M DFlash drafter: every plane tagged with its disk type's format, and on the CPU arm, over a chat turn, the greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
                cpu_drafter_lossless(t, m, QWEN3_4B_DRAFTER_K4_FILE, "Qwen3-4B k4 cpu", true, true)
            }
        }
    }
}

//! the regular DFlash drafter's discrete-home cells on the drafter form `m`: the Q8_0 and Q4_K_M files' legs over a chat turn
def private qwen3_4b_drafter_discrete_cells(t : T?; m : Model) {
    discrete_drafter_cell(t, m, "Qwen3-4B's resident trunk on the serving discrete home, the Q8_0 and Q4_K_M DFlash drafters: each greedy walk on the device arm is the plain walk's on the same device, every pass on the device's top-k, the host's walk and selector drafting the same ids",
        "", [QWEN3_4B_DRAFTER_FILE, QWEN3_4B_DRAFTER_K4_FILE], "Qwen3-4B discrete", false, true, false)
}

//! what the Qwen3-4B-Instruct-2507 Q4_K_M file's Metal form keeps for
//! its portable CPU form and the compares after the block
struct Qwen3KqKept {
    matrix : MatrixFamilyKept
    prefill : PrefillKept
    sweep_legs : int
}

//! `mm_b_kq` over disk-order planes - the portable batch kernel - bit
//! for bit the per-position GEMV (`mm_at_kq_pre`): blk.0.attn_q's
//! first 64 rows over four positions
def private mm_b_kq_disk_cell(t : T?; tr : Model) {
    let n = tr.config.dim
    let d = 64l
    let npos = 4l
    let fmt = tr.wq_fmt[0]
    let woff = tr.wq_offs[0]
    var inscope x <- [for (k in range64(npos * n)); float((k * 37l) % 255l - 127l) * 0.01]
    var s <- create_session(tr, mirror_kv_dtype())
    forward_prefill_alloc(tr, s, npos, 0l)
    var inscope y <- nan_plane(int(npos * d))
    mm_b_kq(s, y, tr, fmt, woff, x, n, d, npos)
    delete s
    var inscope want <- nan_plane(int(npos * d))
    var inscope xq : array<int8>
    var inscope xs : array<float>
    var inscope xbs : array<int>
    xq |> resize(n)
    xs |> resize(n / 256l)
    xbs |> resize(n / 16l)
    for (p in range64(npos)) {
        unsafe {
            quantize_q8_k_into_ptr(addr<float const?>(x[p * n]), n, addr(xq[0]), addr(xs[0]),
                addr(xbs[0]), 0l, 0l, 0l)
        }
        mm_at_kq_pre(want, tr, fmt, woff, xq, xs, xbs, n, d, p * d)
    }
    check(t, "{fmt} attn_q rows through mm_b_kq on disk-order planes against the per-position GEMV",
        y, want, BAR_EXACT, Shape(row_width = int(d)))
}

//! The K-quant native load's cursor placement on its portable-pinned
//! CPU form `tr` (the planes disk-order): plane-dequant prefixes
//! against the file dequant - a deep k4 plane within f16-pair
//! rounding, the tied k6 classifier bit for bit - and `mm_b_kq`
//! against the per-position GEMV
def private kq_load_layout_cell(t : T?; var tr : Model; path : string) {
    unsafe {   //! the cell reads the block's model by reference
        let title = "kquant_native load places tensors at their cursor offsets (Qwen3-4B Q4_K_M)"
        t |> run(title) @capture(& tr)(t : T ?) {
            t |> success(!tr.kq_repacked, "portable-pinned load keeps the kq planes disk-order")
            t |> success(tr.kquant_native, "load detected native K-quant tensors")
            t |> success(cls_kq(tr), "tied Q6_K classifier takes the kq plane path")
            t |> success(tr.wq_fmt[0] == KqFmt.k4, "blk.0.attn_q tagged k4")
            t |> success(tr.wv_fmt[0] == KqFmt.k6, "blk.0.attn_v tagged k6")
            let n = 262144l
            var have : array<float>
            have |> resize(n)
            let l = tr.config.n_layers - 1l   //! the drift of every earlier tensor
            dequant_plane_prefix(tr, tr.wq_fmt[l], tr.wq_offs[l], n, have)
            var gt = 0
            var fdq <- read_prefix(path, "blk.{l}.attn_q.weight", n, gt)
            t |> success(rel_l2(have, fdq) < 1.0e-3,
                "deep-layer k4 plane matches the file dequant (f16-pair rounding only)")
            delete fdq
            var fdq6 <- read_prefix(path, "token_embd.weight", n, gt)
            dequant_plane_prefix(tr, tr.emb_fmt, tr.wcls_off, n, have)
            check(t, "k6 classifier plane against the file dequant", have, fdq6, BAR_EXACT,
                FLAT_SHAPE)
            delete fdq6
            delete have
            mm_b_kq_disk_cell(t, tr)
        }
    }
}

let private QWEN3_4B_KQ = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"

//! the 4B Q4_K_M file's portable CPU form with the native K-quant
//! planes on: no kq-capable backend repacks them, so they stay
//! disk-order
def private kq_layout_form() : ModelForm
    => cpu_cells_form(backend = "portable", kq_native = KqNative.on)

//! the 4B files' auto-backend CPU form with the native K-quant planes
//! on, the kq rows cores' repack the fused rows ask
def private kq_repack_form() : ModelForm => cpu_cells_form(kq_native = KqNative.on)

//! the 4B Q4_K_M file's forms: on a das_metal build the blob twin and
//! the portable CPU form its CPU sides read where the arm filter
//! reaches a Metal cell, the layout cell's and the fused rows' CPU
//! forms on every box, and on the Vulkan home the coverage sweep's
def private qwen3_kq_forms() : array<ModelForm> {
    var forms <- [kq_layout_form(), kq_repack_form()]
    if (any_arm_on(["fam-qwen3", "kq-tail"])) {
        forms |> push(metal_form(ctx = PREFILL_QWEN3_4B_CTX))
        if (empty(metal_build_why())) {
            forms |> push(prefill_cpu_form())
        }
    }
    if (!gpu_want_cuda()) {
        sweep_forms(forms, false)
    }
    return <- forms
}

//! one form's cells of the 4B Q4_K_M file: on the twin the matrix row,
//! then the prefill parity, whose cells shut the drivers down
def private qwen3_kq_rows(t : T?; var k : Qwen3KqKept; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        matrix_family_metal(t, m, form, matrix_row_qwen3_4b_k4(), k.matrix)
        prefill_parity_metal(t, m, form, prefill_qwen3_4b(), k.prefill)
    } elif (alone_pass_of(form, prefill_cpu_form())) {
        matrix_family_cpu(t, m, form, matrix_row_qwen3_4b_k4(), k.matrix)
        prefill_parity_cpu(t, m, form, k.prefill)
    } elif (alone_pass_of(form, kq_layout_form())) {
        kq_load_layout_cell(t, m, path_join(models_dir(), QWEN3_4B_KQ))
    } elif (alone_pass_of(form, kq_repack_form())) {
        fused_decode_row_cell(t, m, form, fused_row_qwen3_4b_q4km())
    } elif (form.coopmat >= 0) {
        sweep_cell(t, k.sweep_legs, m, form, 2)
    }
}

[test(description = "On Qwen3-4B-Instruct-2507 Q4_K_M: on its portable-pinned K-quant native CPU form, on every box, each tagged tensor sits at its cursor offset - a deep k4 plane-dequant prefix within f16-pair rounding of the file dequant, the tied k6 classifier bit for bit - and `mm_b_kq` matches the per-position GEMV; on its auto-backend K-quant native CPU form, repacked for the kq rows cores, the fused decode chains with Q8_K activations match the per-op kq path's greedy ids and final logits bit for bit; on the Metal home's blob twin, one load on a das_metal build, the support matrix's k4/k6 row - the path cell, the metal-overrides generate token for token the portable CPU form's - and the Metal prefill parity cells against their portable CPU controls; on the Vulkan home the coverage census's coopmat sweep, a load a mode; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_qwen3_4b_q4km(t : T?) {
    return if (!family_on(t, "qwen3"))
    var k : Qwen3KqKept
    var forms <- qwen3_kq_forms()
    with_cpu_and_gpu_model(t, "llm", "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", PARITY_ROW_CTX, forms,
            false, parity = true) $(var m, form) {
        qwen3_kq_rows(t, k, m, form)
    }
    matrix_family_verdict(t, matrix_row_qwen3_4b_k4(), k.matrix)
    prefill_parity_verdict(t, k.prefill)
    delete forms
}

[test(description = "On Qwen3-4B-Instruct-2507 Q5_K_M, on its auto-backend K-quant native CPU form repacked for the kq rows cores, the fused decode chains with Q8_K activations over k5 planes and the k6 ffn_down match the per-op kq path's greedy ids and final logits bit for bit; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_qwen3_4b_q5km(t : T?) {
    return if (!family_on(t, "qwen3"))
    var forms <- [kq_repack_form()]
    with_cpu_and_gpu_model(t, "llm", "Qwen3-4B-Instruct-2507-Q5_K_M.gguf", PARITY_ROW_CTX, forms,
            false, parity = true) $(var m, form) {
        if (alone_pass_of(form, kq_repack_form())) {
            fused_decode_row_cell(t, m, form, fused_row_qwen3_4b_q5km())
        }
    }
    delete forms
}
