options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _gemma_resident
require dasllama/dasllama_kqformat   // KqFmt - the PLE table's tag
require dasllama/dasllama_gpu_resident   // resident_plan - the table's seat
require _format_census   // census_cell - the per-layer planes in their own formats
require daslib/fio
require dasllama/dasllama_ple   // ple_cpu_pre_rows - the CPU pre-step's witness
require dasllama/dasllama_common   // Model, embed_row, forward, forward_prefill
require dasllama/dasllama   // create_session
require dasllama/dasllama_math   // with_dasllama_jobque_: the compares after a two-form block read argmax off the queue
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_ple_side_row, vk_rdec_token_gathered - the device pre-step's witnesses
require ?vulkan dasllama/dasllama_vulkan_prefill   // ple_f32_tile_ok - whether the window's f32 GEMMs take the staged tile
require ?vulkan dasllama/dasllama_env   // g_env_vulkan.vk_ple_f32_tile - the knob the pick reads
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu, vk_kernel_coverage - the device the feed pick reads, the census table
require ?cuda dasllama/dasllama_cuda_decode   // the CUDA home's pre-step witnesses
require dasllama/dasllama_core   // set_resident_prefill_allowed: the device prefill's off leg
require dasllama/dasllama_gguf   // parse_gguf_meta, gguf_read_tensor_f32: the PLE table's file rows
require dasllama/dasllama_convert   // dequant_k5_plane_superblock: the K-quant table's oracle
require dasllama/dasllama_gpu_tier   // reset_gpu_cpu_passes: the passes the by-design cells leave
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX, LogitsBar
require _segment_cells   // ple_segments_cells, SEG_WINDOW: the segment form's cells
require _dflash_cells   // the block drafter's walks: lossless, sampled, the planes' tags, the discrete legs
require dasllama/dasllama_dflash   // attach_dflash_drafter, set_dflash_device: the trimmed trunk's refusal
require dasllama/dasllama_mtp_gemma   // load_gemma_drafter, gemma_drafter_target_why: the assistant head's refusal
require ?vulkan dasllama/dasllama_vulkan_dflash   // nolint:STYLE030,LINT019 - the Vulkan arm registers at init
require ?cuda dasllama/dasllama_cuda_dflash   // nolint:STYLE030,LINT019 - the CUDA arm registers at init
require strings   // find: the refusals' words
require _chat_cells   // the gemma-4-E2B Q4_K_M chat cells on its q8 and uncached CPU forms
require _embedding_cells   // embeddings_ple_no_scratch_cell: the Q8_0 file's default device form
require _kquant_cells   // kq_ple_table_cell: the Q4_K_M file's K-quant-native portable form
require _cpu_gpu_model   // ModelForm: the parity rows' forms beside the blocks' own
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the files' model_specs parity rows on their blocks

// gemma-4-E2B Q8_0 (the E-series carrier: per-layer embeddings, the upper layers sharing the K/V of the ones below, two
// dense FFN widths, a sliding window the two-window cell engages): arm `g4e`. Skips off the JIT or without the file under
// models_dir(); each device form skips as a cell of its own without DASLLAMA_GPU=1 on a device the whole-model driver serves
// (the CPU cells forms - q8, fp32 and q4_0 - and the Q4_K_M file's fp32 form run on every box), or when the residency plan
// does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror); the block-codec forms also skip where the environment arms
// another mirror codec over the one they ask (DASLLAMA_VK_KV32=1). The regions forms load at REGIONS_RIG_CTX: two regions for
// the rig's two-session cells, nine for the groups cell; the split-forms form (the Vulkan fuse knob off) loads on the Vulkan home
// alone. Under DASLLAMA_CUDA=1 the default form also carries the CUDA home's own cells, every load under the MMQ route
// pinned `CUDA_MMQ_WINNERS`. The DFlash2 drafter form (an f16 mirror of DISCRETE_MIRROR_CTX positions asked whole, the
// sessions cut to DISCRETE_CTX) carries the drafter's discrete-home legs and the CPU form its CPU cells; a drafter cell skips
// without its drafter file under models_dir() (`drafters/gemma-4-E2B-it-dflash2-GGUF/`), the discrete legs on a build with
// no discrete DFlash arm, and the assistant head's refusal without the 26B's head (`mtp-gemma-4-26B-A4B-it-Q8_0.gguf`, the
// gemma4-moe category's). The segment form (two regions, the planes on the host, the SEG_WINDOW-row prompt window) carries
// the segmented window's cells, and the default form the embeddings' no-scratch cell (`_embedding_cells.das`). The Q4_K_M file's
// q8 and uncached CPU forms, on every box, carry its chat cells (`_chat_cells.das`), and its K-quant-native CPU form under the
// portable backend's pin its per-layer token table's plane (`_kquant_cells.das`). The Q8_0, Q4_K_M and Q4_0 files' parity rows'
// CPU forms (the row's load mode and K-quant knob under DASLLAMA_PIN_BACKEND's pin), on every box, carry each row's frozen
// greedy ids (`parity_spec_cell`); such a form skips where the box cannot take the pin.
let private GEMMA4E_FILE = GemmaFile(file = "gemma-4-E2B-it-Q8_0.gguf", arm = "g4e")

let private PRE_STEP_TOK = 1234l   //! the token the pre-step cell's decode step takes

//! the decode step's pre-step on one device form of the serving home: the 40-id prompt's window chain and one step, the
//! home's counters held in `h`, the window's last side row where the home reads one back, and the side row the one-row command
//! built (none on the Vulkan home with the gather off)
struct PreStep {
    ran : bool
    cuda : bool
    h : Held
    window_row : array<float>
    step_row : array<float>
}

//! the serving home's count of side rows gathered off a token id
def private side_rows_gathered() : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_side_gathered() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_token_gathered()
    }
    return 0l
}

//! the CUDA home's own readings around the pre-step's window chain: the gather's seat, and the window's last side row
def private cuda_window_readings(var p : PreStep; gather : bool) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        p.h |> held_fact(cud_rdec_side_gather_on() == gather, gather
            ? "the driver gathers the side rows off the ids (the table and the projection on the device)"
            : "with the gather off the driver takes the host's gathered rows")
        p.window_row <- cud_rdec_pf_side_row(39l)
    }
}

//! the side row the serving home's one-row command built for the step: the CUDA home's with the gather on or off, the
//! Vulkan home's with the gather on
def private step_side_row(var p : PreStep; gather : bool) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (p.cuda) {
            p.step_row <- cud_rdec_ple_side_row()
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (gather) {
            vk_rdec_ple_side_row(p.step_row)
        }
    }
}

def private pre_step_rows(m : Model; form : ModelForm) : PreStep {
    var p = PreStep(ran = true, cuda = gpu_want_cuda())
    let what = form.gather ? "gathered off the id" : "off the host's gathered row"
    var prompt <- resident_prompt(40l)
    var sg <- create_session(m, form.kv)
    let served0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    forward_prefill(m, sg, prompt, 40l, 0l)
    p.h |> held_equal(gpu_resident_prefills_() - served0, 1l, "{what}: the resident window chain served the prompt")
    if (p.cuda) {
        cuda_window_readings(p, form.gather)
    }
    let g0 = side_rows_gathered()
    let cpu0 = ple_cpu_pre_rows()
    let host0 = ple_host_gather_rows()
    forward(m, sg, PRE_STEP_TOK, 40l)
    let gathered = side_rows_gathered() - g0
    if (p.cuda) {
        let want = form.gather ? 1l : 0l
        p.h |> held_equal(gathered, want, "{what}: {want} side row gathered off the step's token id")
        p.h |> held_equal(ple_host_gather_rows() - host0, 1l - want, "{what}: {1l - want} table row gathered on the host")
        p.h |> held_equal(ple_cpu_pre_rows() - cpu0, 0l, "{what}: the step built no CPU pre-step row")
        p.h |> held_served()
        p.h |> held_cuda_served(c0, 41l, "{what}: the prompt and the step")
    } elif (form.gather) {
        p.h |> held_fact(gathered > 0l, "{what}: the step's embed row gathered off its token id")
        p.h |> held_equal(ple_cpu_pre_rows() - cpu0, 0l, "{what}: the step built no CPU pre-step row")
    } else {
        p.h |> held_equal(gathered, 0l, "{what}: the step gathers no embed row off its id - the counter's control")
    }
    step_side_row(p, form.gather)
    delete sg
    delete prompt
    return <- p
}

//! one form's pre-step on the device: its counters, and each side row the home read back - the window's last (CUDA) and
//! the step's, projected under the CPU's 1/sqrt(dim) and finished - within the home's bar of the CPU pre-step's row of the
//! same token, another token's CPU row the control. The Vulkan row holds 1e-4 (the projection without the 1/sqrt(dim) reads
//! the eps alone apart, 1e-3); the CUDA rows hold 1e-3
def private pre_step_within(t : T?; tag : string; p : PreStep; r : E2bRows) {
    held_within(t, p.h, tag, false)
    let rel = p.cuda ? 1e-3 : 1e-4
    if (!empty(p.window_row)) {
        side_rows_within(t, p.window_row, r.window_cpu, r.window_other, "{tag}the window's last row", rel)
    }
    if (!empty(p.step_row)) {
        side_rows_within(t, p.step_row, r.side_cpu, r.side_other, "{tag}the step's row", rel)
    }
}

//! what the Q8_0 file's forms collect: the default device form's cells and their CPU side on its codec (`main`), the
//! block codecs' cells (the q8_0 mirror's forced feed, the tq4 mirror's walk; each a CPU side, a device side), the rings
//! (the default codec ringed and flat, the tq4 mirror ringed and flat), the pre-step (the gather on, the gather off) and the
//! CPU pre-step's rows of its token and of the control token; each `_ran` says a form collected
struct E2bRows {
    main : MainPair
    rows : GemmaRows[2]
    ple : PleFeed[2]
    peels : array<PeelTail>
    codec : CodecRows
    pre : PreStep[2]
    side_cpu, side_other : array<float>
    chain : Held   //! the greedy chain's walks on the default form's one region
    regions : Held   //! the rig's two-session cells on the two-region form
    gather : GatherPair   //! the embed-gather cell's batched rows, both halves
    host_rows : Held   //! the gather-off form's host half of the embed-gather cell, the pin cell, the CUDA prefill pin's control
    groups : Held   //! the groups cell on the nine-region form
    split_forms : Held   //! the split one-row forms on the Vulkan home
    cuda : bool   //! the default form ran on the CUDA home: its own cells collected
    window_cpu, window_other : array<float>   //! the CPU pre-step's rows of the window's last token and of the control token
    turns : string[3]   //! the PLE serving modes' greedy turns: q8, fp32, q4_0
    turn_ran : bool[3]
    rail : PleRail   //! the fp32 load's table seat
}

let private E2B_GAPS = fixed_array(8l)
let private E2B_TAILS = fixed_array(1l, 2l, 5l)
let private E2B_CODEC = CodecCells(q8_feed = 40l, tq4_walk = (n = 150l, steps = 150l), ring_kv = KVDtype.tq4,
    q8_note = "the shared-KV layers store no row of their own", tq4_note = "a shared-KV layer rotates its query and stores nothing")

//! one form's collection of the Q8_0 file's cells into `r` (`walks` the default form's teacher-forced walks)
def private e2b_rows(var r : E2bRows; var m : Model; form : ModelForm; feeds : array<int64>; walks : array<GemmaWalk>) {
    var inscope gaps <- [for (g in E2B_GAPS); g]
    let side = main_side(r.main, form)
    if (side == MainSide.device) {
        r.ple[1] <- ple_feed(m, form, GEMMA4E_FILE.file)
        r.rows[1] <- gemma_rows(m, form, feeds, gaps, walks)
        delete r.peels
        r.peels <- e_series_peel_rows(m, [for (n in E2B_TAILS); n])
        codec_rows(r.codec, m, form, E2B_CODEC)
        r.pre[0] <- pre_step_rows(m, form)
        r.cuda = gpu_want_cuda()
        greedy_chain_rows(r.chain, m, GEMMA4E_FILE.file)
    } elif (form.on_gpu && form.regions == 2l && form.gather && !form.split_forms) {
        e2b_regions_rows(r, m)
    } elif (form.on_gpu && form.regions == 2l && form.split_forms) {
        regions_batched_rows(r.split_forms, m, LogitsBar.bit_exact, DownFold.split_forms)
    } elif (form.on_gpu && form.regions == 9l) {
        regions_groups_rows(r.groups, m, 9l, LogitsBar.bit_exact)
    } elif (form.on_gpu && form.regions == 2l) {
        r.pre[1] <- pre_step_rows(m, form)
        e2b_host_rows(r, m)
    } elif (form.on_gpu) {
        codec_rows(r.codec, m, form, E2B_CODEC)
    } elif (form.quant == QuantMode.fp32) {
        r.turns[1] = ple_greedy_turn(m)
        r.turn_ran[1] = true
        r.rail <- ple_rail(m, path_join(models_dir(), GEMMA4E_FILE.file))
    } elif (form.quant == QuantMode.q4_0) {
        r.turns[2] = ple_greedy_turn(m)
        r.turn_ran[2] = true
    } else {
        if (side == MainSide.cpu) {
            r.ple[0] <- ple_feed(m, form, GEMMA4E_FILE.file)
            r.rows[0] <- gemma_rows(m, form, feeds, gaps, walks)
            delete r.side_cpu
            delete r.side_other
            r.side_cpu <- cpu_side_row(m, PRE_STEP_TOK)
            r.side_other <- cpu_side_row(m, PRE_STEP_TOK + 1l)
            var inscope prompt <- resident_prompt(40l)
            delete r.window_cpu
            delete r.window_other
            r.window_cpu <- cpu_side_row(m, prompt[39])
            r.window_other <- cpu_side_row(m, prompt[39] + 1l)
        }
        if (!r.turn_ran[0]) {
            r.turns[0] = ple_greedy_turn(m)
            r.turn_ran[0] = true
        }
        codec_rows(r.codec, m, form, E2B_CODEC)
    }
}

//! the Q8_0 file's CPU cells' asserts after the block, on every box: the PLE serving modes' greedy turns, the fp32 load's
//! table seat
def private e2b_cpu_within(t : T?; r : E2bRows) {
    for (i, mode in range(3), fixed_array("q8", "fp32", "q4")) {
        if (r.turn_ran[i]) {
            t |> success(letters(r.turns[i]) >= 3, "gemma-4-E2B Q8_0, the PLE {mode} serving mode: the greedy continuation is text: '{r.turns[i]}'")
        }
    }
    if (r.rail.ran) {
        ple_rail_within(t, r.rail)
    }
}

//! the Q8_0 file's compares on what its forms collected, each cell where its forms ran
def private e2b_rows_within(t : T?; r : E2bRows; walks : array<GemmaWalk>) {
    var inscope gaps <- [for (g in E2B_GAPS); g]
    if (r.main.ran) {
        gemma_rows_within(t, "gemma-4-E2B Q8_0", r.rows[1], r.rows[0], gaps, walks)
    }
    held_within(t, r.chain, "gemma-4-E2B Q8_0, the greedy chain: ", false)
    held_within(t, r.regions, "gemma-4-E2B Q8_0, two regions: ", false)
    held_within(t, r.host_rows, "gemma-4-E2B Q8_0, two regions, the gather off: ", false)
    held_within(t, r.groups, "gemma-4-E2B Q8_0, nine regions: ", false)
    held_within(t, r.split_forms, "gemma-4-E2B Q8_0, two regions, the split one-row forms: ", false)
    gather_pair_within(t, "gemma-4-E2B Q8_0, the embed gather", r.gather)
    for (i, what in range(2), fixed_array("the gather on", "the gather off")) {
        if (r.pre[i].ran) {
            pre_step_within(t, "gemma-4-E2B Q8_0, the pre-step, {what}: ", r.pre[i], r)
        }
    }
    codec_rows_within(t, "gemma-4-E2B Q8_0", r.codec, E2B_CODEC)
    with_dasllama_jobque_() {
        if (r.main.ran) {
            ple_feed_within(t, "gemma-4-E2B Q8_0", r.ple[1], r.ple[0], GEMMA4E_FILE.file, "")
            e_series_peels_within(t, r.peels, SPLIT_BAR_REL)
        }
        rings_within(t, "gemma-4-E2B Q8_0", GEMMA_RING_IDS, r.codec.rings, r.main.kv,
            E2B_CODEC.ring_kv)
    }
}

[test(description = "On gemma-4-E2B Q8_0 the resident driver holds the CPU chain's forty-token forced feed, its perplexity over a two-window prefill and 80 teacher-forced positions, and its window chain against token command gap within 6x of the CPU chain's own; tails of 1, 2 and 5 rows peel within the split bar of the row-at-a-time tail; the all-Q8_0 per-layer planes dispatch no format GEMV and no f32 class; a q8_0 K/V mirror holds the CPU chain's forty-token logits and a tq4 mirror its perplexity; the sliding layers' rings match a flat mirror bit for bit over 1300 ids and eight steps on the default and the tq4 mirror; the decode step builds its side row on the device off the gathered id within 1e-4 of the CPU pre-step's, and with the gather off no row gathers; the greedy chain walks bit for bit the plain greedy walk; over two mirror regions interleaved single steps, batched steps (past the unsplit position too, and on the split one-row forms on Vulkan) and nine sessions batched in command groups read what each session reads alone, every side row and token row gathered on the device bit for bit the host-embedded rows, two device-home sessions step bit for bit a host-cached session while a live region is never handed to an outsider, a device-mode scheduler emits the host-cached scheduler's tokens and hands two streams to one segmented window, and the window's fused q/k norm reads within the wide bar of the split pair; on the CUDA driver forty-token forced-feed logits, a 150-position perplexity and the device-built side rows hold the CPU chain's, a prefill-pinned prompt passing to the CPU as pinned_off; the PLE table of an fp32 load sits on an exactly-sized q8 plane bit-identical to the file and refuses both in-place requants, and greedy chat turns in the q8, fp32 and q4_0 serving modes are text; with the E2B's DFlash2 drafter on the CPU arm a planes-trimmed trunk refuses the attach and the untrimmed one attaches, the greedy speculative walk emits the plain walk's ids with drafts accepted for the Q8_0, Q4_K_M and NVFP4 drafter files (each file's planes tagged with their disk type), and the exact sampled walk equals the plain sampled walk with drafts accepted and honors a stop id while both rejection walks accept some drafts and reject some; on the discrete home's drafter form each drafter file's greedy walk on the device arm is the plain walk's, every pass on the device's top-k and walk, the host's walk and selector drafting the same ids, and with the Q8_0 file a stop id ends the drafted stream on its first occurrence and an embedding call commits no drafter context; the 26B's assistant head refuses the E-series trunk naming its per-layer embeddings; on the segment form each segment's side rows gather off its ids in one segmented window within 16% of solo windows, and with the token-table gate unseated host-embedded rows decline the segmented form; on the default form, with per-layer embeddings and sliding layers, no scratch window is promised or run and the mean-pooled batch matches its singles at cosine above 0.9999; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token on its encoded text prompt; each form loads once")]
def test_gpu_resident_gemma4_e2b(t : T?) {
    return if (!family_on(t, "gemma4e") || !arm_on(t, GEMMA4E_FILE.arm))
    var walks : array<GemmaWalk>
    if (assert_pf_window(t)) {   //! two windows, the sliding window engaged on the last eight, where the prompt window is the cells' own
        walks |> push((n = PF_WINDOW_ROWS + 8l, steps = 80l))
    }
    var feeds : array<int64>
    if (gpu_want_cuda()) {   //! the CUDA home's own cells: forty tokens fed, 150 prefilled and 150 teacher-forced
        feeds |> push(40l)
        walks |> push((n = 150l, steps = 150l))
    }
    var r : E2bRows
    var forms <- [device_form(), device_form() |> asking(KVDtype.q8_0), device_form() |> asking(KVDtype.tq4), device_form(ring = false),
        device_form(ring = false) |> asking(KVDtype.tq4), device_form(regions = 2l, ctx = REGIONS_RIG_CTX),
        device_form(gather = false, regions = 2l, host = true, ctx = REGIONS_RIG_CTX), device_form(regions = 9l, ctx = REGIONS_RIG_CTX),
        cpu_cells_form(), cpu_cells_form(QuantMode.fp32), cpu_cells_form(QuantMode.q4_0)]
    if (!gpu_want_cuda()) {   //! the fuse knob is the Vulkan driver's
        forms |> push(device_form(regions = 2l, split_forms = true, ctx = REGIONS_RIG_CTX))
    }
    forms |> push(discrete_drafter_form())
    forms |> push(device_form(regions = 2l, host = true, ctx = REGIONS_RIG_CTX, pf_window_rows = SEG_WINDOW))
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-E2B-it-Q8_0.gguf", GEMMA_RIG_CTX, forms, parity = true) $(var m, form) {
            if (form.strict_ctx) {
                e2b_drafter_discrete_cells(t, m)
                return
            }
            if (form.pf_window_rows == SEG_WINDOW) {
                e2b_segments_on(t, m)
                return
            }
            e2b_rows(r, m, form, feeds, walks)
            if (device_default(form)) {
                embeddings_ple_no_scratch_cell(t, m)
            }
            if (!form.on_gpu && form.quant == QuantMode.q8 && form.alone) {
                e2b_drafter_cpu_cells(t, m)
            }
        }
    }
    if (ran) {
        e2b_rows_within(t, r, walks)
    }
    e2b_cpu_within(t, r)
    delete r
    delete forms
    delete feeds
    delete walks
}

let private PLE_GEMVS = fixed_array("kq_gemv_q40", "kq_gemv_k6", "q5032_gemv")   //! the format GEMVs a per-layer plane can ride

//! the census counts a per-layer plane's route moves: the f32 act, the window's scalar f32 tile and staged tile, each of `PLE_GEMVS`
struct PleCounts {
    census : bool   //! the coverage census reads on this home: its witnesses stand down where not
    act : int64
    gemm : int64
    staged : int64
    gemvs : array<int64>
}

def private ple_counts() : PleCounts {
    return <- PleCounts(census = census("ple_act_f32_cls_spv") != NO_CENSUS, act = census("ple_act_f32_cls_spv"),
        gemm = census("dn_ba_cls_spv"), staged = census("f32_batch_khr_cls_spv"), gemvs <- [for (g in PLE_GEMVS); format_gemv_count(g)])
}

//! what the counts moved since `c0`
def private ple_counts_since(c0 : PleCounts) : PleCounts {
    var inscope c1 <- ple_counts()
    return <- PleCounts(census = c0.census, act = c1.act - c0.act, gemm = c1.gemm - c0.gemm, staged = c1.staged - c0.staged,
        gemvs <- [for (i in range(length(PLE_GEMVS))); c1.gemvs[i] - c0.gemvs[i]])
}

//! the counts `n` a forced feed moved, against the planes' route: `f32` planes on the f32 act and the window's f32 GEMMs (the staged
//! KHR tile where the device has it, else the scalar f32 tile); quantized planes on their format's decode GEMV (`gemv`: its count
//! rises) with the f32 classes and the staged tile undispatched (the control); a file with no `gemv` dispatches none of
//! `PLE_GEMVS` - their counters' control
def private ple_counts_within(t : T?; tag : string; n : PleCounts; f32 : bool; gemv : string) {
    return if (!n.census)
    if (f32) {
        t |> success(n.act > 0l, "{tag}the token command's f32 act ({n.act}) served the f32 rows")
        if (staged_window_tile()) {
            t |> success(n.staged > 0l && n.gemm == 0l, "{tag}the window's f32 GEMMs ran on the staged tile ({n.staged}), the scalar f32 tile undispatched ({n.gemm})")
        } else {
            t |> success(n.gemm > 0l && n.staged == 0l, "{tag}the window's f32 GEMMs ran on the scalar f32 tile ({n.gemm}) on a device without the staged tile")
        }
        return
    }
    t |> equal(n.act + n.gemm + n.staged, 0l, "{tag}no f32 per-layer plane, no f32 act, scalar tile or staged tile dispatched")
    for (g, gv_n in PLE_GEMVS, n.gemvs) {
        if (g == gemv) {
            t |> success(gv_n > 0l, "{tag}the fed steps read the planes on their format's GEMV ({g}: {gv_n})")
        } elif (empty(gemv)) {
            t |> equal(gv_n, 0l, "{tag}an all-Q8_0 file dispatches no {g} - the format GEMVs' control")
        }
    }
}

//! one pass's per-layer-plane cell on `file` (`ple_feed`): the forty-token forced feed and the counts it moved, and on the device
//! pass what the load alone witnesses - the weight-format census, whether the planes ship F32, the carrier's arms
struct PleFeed {
    feed : FeedRows
    counts : PleCounts
    sites : array<SiteCensus>
    f32 : bool
    arms : bool
}

def private ple_feed(m : Model; form : ModelForm; file : string) : PleFeed {
    var p : PleFeed
    if (form.on_gpu) {
        p.sites <- census_sites(m, path_join(models_dir(), file))
        p.f32 = m.ple_gate_f32 && m.ple_proj_f32
        p.arms = gemma_carrier_arms(m)
    }
    var inscope c0 <- ple_counts()
    p.feed <- prose_feed_rows(m, form, 40l)
    p.counts <- ple_counts_since(c0)
    return <- p
}

//! the device pass's per-layer-plane cell against the CPU pass's on a file whose planes are quantized (`gemv` their format's
//! decode GEMV, empty for an all-Q8_0 file): every plane in its own format, none F32, the feed within the gemma logit bar of the
//! CPU chain's, the counts as `ple_counts_within` says
def private ple_feed_within(t : T?; tag : string; gpu, cpu : PleFeed; file, gemv : string) {
    t |> success(gpu.arms, "{tag}: {GEMMA_CARRIER_ARMS}")
    var inscope every : array<string>
    census_sites_within(t, gpu.sites, path_join(models_dir(), file), "vulkan load", ["per-layer embeddings"], every)
    t |> equal(gpu.f32, false, "{tag}: the planes ship F32 exactly where the file carries F32")
    feed_rows_within(t, "{tag}, {gpu.feed.nprompt} tokens: ", gpu.feed, cpu.feed, GEMMA_LOGIT_BAR_REL)
    ple_counts_within(t, "{tag}: ", gpu.counts, false, gemv)
}

//! the Q4_K_M file's per-layer-plane cell against the file's f32 chain (`f32`) where the CPU q8 chain (`cpu`) is no reference: the
//! Q8_K activations make the feed chaotic, so step 4's top logit reads 17.85 (arm64), 16.70 (x64) and 18.50 (x64 on the reference
//! kernel bodies) where the f32 chain reads 19.14, and the reference build's quantized CPU path reads 17.8 to 20.1 by its attention
//! form and batching, each chain summing as far from the f32 rows. Every plane in its own format, the gate and projection planes
//! F32, the device's feed within the gemma logit bar of the f32 chain's, the device no further from that chain than the CPU q8
//! chain (`f32_chain_witness`, within the ratio of the route `staged` names), the counts on the f32 route (`ple_counts_within`)
[arch(at="../ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed")]
def private ple_f32_feed_within(t : T?; tag : string; gpu, cpu, f32 : PleFeed; file : string; staged : bool) {
    t |> success(gpu.arms, "{tag}: {GEMMA_CARRIER_ARMS}")
    var inscope every : array<string>
    census_sites_within(t, gpu.sites, path_join(models_dir(), file), "vulkan load", ["per-layer embeddings"], every)
    t |> equal(gpu.f32, true, "{tag}: the planes ship F32 exactly where the file carries F32")
    feed_rows_within(t, "{tag}, {gpu.feed.nprompt} tokens: ", gpu.feed, f32.feed, GEMMA_LOGIT_BAR_REL, "the f32 chain")
    let route = staged ? " [the staged tile, DASLLAMA_VK_PLE_F32_TILE on]" : " [the scalar f32 tile]"
    f32_chain_witness(t, "{file}: {gpu.feed.nprompt} tokens against the f32 chain{route}", gpu.feed.rows, cpu.feed.rows, f32.feed.rows,
        staged ? F32_DIST_REL_STAGED : F32_DIST_REL) $(_id) => ""
    ple_counts_within(t, "{tag}: ", gpu.counts, true, "")
}

//! the short window's peel on the CUDA home (`e_series_peel_cell`): tails of 2, 9 and 16 rows - one column tile, then past it -
//! within the gemma logits bar of the row-at-a-time tail (the carrier's two token-command forms round up to 0.11 of the max logit
//! apart, its window chain as far off the CPU chain; the one-step-off control reads 0.38 and up, a tail past the tile with its router
//! columns untiled 1.6 and up)
let private CUDA_PEEL_TAILS = fixed_array(2l, 9l, 16l)

[test(description = "The gemma-4-E2B Q4_K_M file's per-layer gate and projection planes, F32 on disk, run on the f32 act and the staged or scalar f32 tile against the file's f32 chain; on the CUDA driver short tails of 2, 9 and 16 rows peeled through the token commands land within 16 percent of max logit of the row-at-a-time tail; an fp32 load pins the PLE table to its native k5 plane with no q8 side plane, row 0 dequantized off it within 1e-3 relative RMS of the file; on the q8 CPU form, on every box, the instruct text turn answers and the fused media body holds its logits; on the uncached CPU form the thinking default and the instruct opt-out and the tool wire's turns and heals render; on the K-quant-native portable-pinned CPU form the per-layer token table loads on the k5 plane, not a silent q8 fallback, its first 16 rows within 1e-3 relative L2 of the file dequant; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_gemma4_e2b_ple_planes(t : T?) {
    return if (!family_on(t, "gemma4e") || !arm_on(t, "g4e-kq"))
    var ple : PleFeed[3]   //! the CPU form's, the device form's, the f32 form's
    var staged = false
    var peel_f32 = false   //! the CUDA home's peel cell ran: the gate and projection rows ship F32
    var peel_ran = false
    var peels : array<PeelTail>
    var kq : PleKqTable
    var dev_ran = false   //! a device form collected: the CPU forms' feeds are its references
    var forms <- [device_form(), cpu_cells_form(QuantMode.fp32), cpu_cells_form(), cpu_cells_form(uncached = true),
        cpu_cells_form(backend = "portable", kq_native = KqNative.on)]
    with_cuda_mmq_winners() {
        let ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-E2B-it-Q4_K_M.gguf", GEMMA_RIG_CTX, forms, parity = true) $(var m, form) {
            if (form.uncached) {
                chat_gemma4e_cells(t, m)
                return
            }
            if (form.kq_native == KqNative.on) {
                kq_ple_table_cell(t, m)
                return
            }
            dev_ran ||= form.on_gpu
            if (dev_ran) {
                ple[form.quant == QuantMode.fp32 ? 2 : (form.on_gpu ? 1 : 0)] <- ple_feed(m, form, "gemma-4-E2B-it-Q4_K_M.gguf")
            }
            if (form.on_gpu) {
                staged = staged_window_tile()   //! the tile pick reads the device: only an armed form has one to read
                if (gpu_want_cuda()) {
                    peel_ran = true
                    peel_f32 = m.ple_gate_f32 && m.ple_proj_f32
                    delete peels
                    peels <- e_series_peel_rows(m, [for (n in CUDA_PEEL_TAILS); n])
                }
            }
            if (form.quant == QuantMode.fp32) {
                kq = ple_kq_table(m, path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf"))
            } elif (!form.on_gpu && form.alone) {
                chat_gemma4e_host_cells(t, m)
            }
        }
        if (ran) {
            with_dasllama_jobque_() {
                ple_f32_feed_within(t, "gemma-4-E2B Q4_K_M", ple[1], ple[0], ple[2], "gemma-4-E2B-it-Q4_K_M.gguf", staged)
                if (peel_ran) {
                    t |> success(peel_f32, "gemma-4-E2B Q4_K_M, the CUDA peel: the per-layer gate and projection rows ship F32: the branch rides the router GEMV")
                    e_series_peels_within(t, peels, GEMMA_LOGIT_BAR_REL)
                }
            }
        }
        if (kq.ran) {
            ple_kq_table_within(t, kq)
        }
    }
    delete ple
    delete peels
    delete forms
}

[test(description = "The gemma-4-E2B Q4_0 file's per-layer gate and projection planes run on the Q4_0 decode GEMV against the CPU chain, no f32 class dispatched; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_gpu_resident_gemma4_e2b_ple_planes_q40(t : T?) {
    return if (!family_on(t, "gemma4e") || !arm_on(t, "g4e-kq-q40"))
    var ple : PleFeed[2]
    let forms <- [device_form()]
    let ran = with_cpu_and_gpu_model(t, "llm", "gemma-4-E2B-it-Q4_0.gguf", GEMMA_RIG_CTX, forms, parity = true) $(var m, form) {
        ple[form.on_gpu ? 1 : 0] <- ple_feed(m, form, "gemma-4-E2B-it-Q4_0.gguf")
    }
    if (ran) {
        with_dasllama_jobque_() {
            ple_feed_within(t, "gemma-4-E2B Q4_0", ple[1], ple[0], "gemma-4-E2B-it-Q4_0.gguf", "kq_gemv_q40")
        }
    }
    delete ple
}

//! the staged tile's f16 weights and f16 sums (`ARCHITECTURE_GPU_ASYMMETRIES.md`) read 1.16 on the RTX PRO 4500, the scalar tile 0.88
let private F32_DIST_REL_STAGED = 1.3

//! the window's f32 per-layer GEMMs ride the staged KHR tile on this device and knob (`pf_ple_f32_staged`'s answer at the E2B's
//! dim and ple)
def private staged_window_tile() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return false if (gpu_want_cuda())   //! the CUDA home's window runs its f32 GEMMs on no staged KHR tile
        return ple_f32_tile_ok(1536l, 256l, g_env_vulkan.vk_ple_f32_tile)
    } else {
        return false
    }
}

//! a format's decode GEMV dispatches: its one-row and N-column leaves summed
def private format_gemv_count(gemv : string) : int64 => census("{gemv}_cls_spv") + census("{gemv}_n_cls_spv")


//! the rig's two-session cells on the two-region form, in the order a load serves them: the scheduler's cells last, since its
//! streams leave parked claims behind
def private e2b_regions_rows(var r : E2bRows; m : Model) {
    regions_interleaved_rows(r.regions, m)
    regions_batched_rows(r.regions, m)
    regions_split_rows(r.regions, m)
    regions_side_rows(r.regions, m)
    gather_half(r.gather, r.regions, m, true, GEMMA4E_FILE.file)
    regions_device_home_rows(r.regions, m)
    if (gpu_want_cuda()) {
        r.regions |> held_skip("the CUDA home is asked: the window's fused q/k norm lever is the Vulkan driver's")
    } else {   //! the wide bar: the two norm orders put the E-series' fed steps 7.4-7.6% of the max logit apart
        regions_window_qkn_rows(r.regions, m, LogitsBar.wide_bar)
    }
    regions_scheduler_rows(r.regions, m)
    regions_scheduler_no_cache_rows(r.regions, m)
    regions_scheduler_handoff_rows(r.regions, m)
}

//! the gather-off form's cells, the planes on the host: the embed-gather cell's host half, then the cells whose calls pass to the
//! CPU by design - the pin cell's outsider, the CUDA prefill pin's prompt - after the served witness of every call before them;
//! the passes they leave are cleared, so the form's own witness reads the calls after them
def private e2b_host_rows(var r : E2bRows; m : Model) {
    gather_half(r.gather, r.host_rows, m, false, GEMMA4E_FILE.file)
    r.host_rows |> held_served("the device served every call before the by-design passes")
    regions_device_home_pin_rows(r.host_rows, m)
    if (r.cuda) {
        cuda_prefill_pinned_off(r.host_rows, m)
    }
    reset_gpu_cpu_passes()
}

//! the CUDA pre-step on one form: the counters a 40-row prompt and one step on `PRE_STEP_TOK` moved (held), the window's last side
//! row and the step's side row as the device built them
//! the prefill counter's control on the CUDA home: a prompt under the server's prefill pin (`set_resident_prefill_allowed(false)`,
//! put back) passes to the CPU rails as `pinned_off`, the window chain's count stays and the CUDA driver serves no position
def private cuda_prefill_pinned_off(var h : Held; m : Model) {
    var prompt <- resident_prompt(40l)
    var sc <- create_session(m, mirror_kv_dtype())
    let served0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    let pinned0 = pass_count("pinned_off")
    let allowed0 = resident_prefill_allowed()
    set_resident_prefill_allowed(false)
    forward_prefill(m, sc, prompt, 40l, 0l)
    set_resident_prefill_allowed(allowed0)
    h |> held_equal(gpu_resident_prefills_(), served0, "control: under the server's prefill pin the window chain served no prompt - its counter stays")
    h |> held_fact(pass_count("pinned_off") - pinned0 >= 1l, "control: the prompt passed to the CPU rails as pinned_off ({pass_count("pinned_off") - pinned0} counted)")
    if (c0 >= 0l) {
        h |> held_equal(cuda_positions_served(), c0, "control: the pinned-off prompt: the CUDA driver served no position")
    }
    delete sc
    delete prompt
}

//! one short greedy chat turn on `m` (its context cut to 512 for the turn, put back): the continuation alone, decoded
def private ple_greedy_turn(var m : Model) : string {
    let seq = m.config.seq_len
    m.config.seq_len = min(seq, 512l)   //! KV sizing only: the turn is short
    var s <- make_run_state(m.config)
    var chat = create_chat(m, "")
    add_user(chat, "Name three colors.")
    let prompt <- render_turn(m, chat)
    let np = long_length(prompt)
    let ids <- generate(m, s, prompt, int(np + 24l))
    var gen <- [for (i in range64(np - 1l, long_length(ids))); ids[i]]
    let text = decode(m, gen)
    to_log(LOG_INFO, "PLE {m.quant} greedy: {text}\n")
    delete gen
    delete chat
    delete s
    m.config.seq_len = seq
    return text
}

def private letters(s : string) : int {
    var n = 0
    for (ch in s) {
        if (is_alpha(ch)) {
            n++
        }
    }
    return n
}

//! the file's own dequant of the PLE table's row `r` (`all` = layers * ple elements), off the gguf mapping
def private file_row(path : string; r, all : int64) : array<float> {
    var dst : array<float>
    dst |> resize(all)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope gm <- parse_gguf_meta(bytes)
        gguf_read_tensor_f32(gm, bytes, "per_layer_token_embd.weight", dst, 0l, all, r * all)
    }
    fclose(f)
    return <- dst
}

//! the fp32 load's PLE table seat: its tag and plane offset, the q8 plane's size against the table's, the fp32 blob's, the first,
//! second and last rows dequantized off the plane against the file's own, and both in-place requant forms' refusals
struct PleRail {
    ran : bool
    q8_seat : bool
    qblob : int64
    tab : int64
    wblob : int64
    eq : bool
    worst : double
    q8_tripped : bool
    q4_tripped : bool
}

def private ple_rail(var m : Model; path : string) : PleRail {
    var p = PleRail(ran = true)
    let c = m.config
    let all = c.n_layers * c.n_embd_per_layer
    p.tab = c.vocab_size * all
    p.q8_seat = m.ple_emb_fmt == KqFmt.q8 && m.ple_emb_off == 0l
    p.qblob = long_length(m.qblob)
    p.wblob = long_length(m.wblob)
    var have : array<float>
    have |> resize(all)
    p.eq = true
    for (r in [0l, 1l, c.vocab_size - 1l]) {
        dequant_q8_row(m, m.ple_emb_off + r * all, all, unsafe(addr(have[0])))
        var inscope want <- file_row(path, r, all)
        for (i in range64(all)) {
            if (have[i] != want[i]) {
                p.eq = false
                p.worst = max(p.worst, double(abs(have[i] - want[i])))
            }
        }
    }
    delete have
    try {   //! in place, a requant replaces wblob and orphans the pinned table's plane: both forms refuse an E-series model first
        quantize_weights(m)
    } recover {
        p.q8_tripped = true
    }
    try {
        quantize_weights_q4(m)
    } recover {
        p.q4_tripped = true
    }
    return p
}

def private ple_rail_within(t : T?; p : PleRail) {
    t |> success(p.q8_seat, "the fp32 load of gemma-4-E2B Q8_0: the PLE table tagged q8 at plane offset 0")
    t |> success(p.qblob == p.tab, "the q8 plane holds EXACTLY the table ({p.qblob} of {p.tab} elements)")
    t |> success(p.wblob < p.tab, "fp32 wblob ({p.wblob}) is smaller than the table it no longer carries ({p.tab})")
    t |> success(p.eq, "gather rows bit-match the file dequant (worst |diff| {p.worst})")
    t |> success(p.q8_tripped, "quantize_weights refuses a PLE model in place")
    t |> success(p.q4_tripped, "quantize_weights_q4 refuses a PLE model in place")
}

//! the fp32 load of a K-quant file's PLE table seat: tagged k5, no q8 side plane, and row 0 dequantized off the k5 plane against the
//! file's own as a relative RMS
struct PleKqTable {
    ran : bool
    k5 : bool
    no_q8 : bool
    rel_rms : float
}

def private ple_kq_table(m : Model; path : string) : PleKqTable {
    var p = PleKqTable(ran = true, k5 = m.ple_emb_fmt == KqFmt.k5, no_q8 = empty(m.qblob))
    let all = m.config.n_layers * m.config.n_embd_per_layer
    var have : array<float>
    have |> resize(all)
    for (blk in range64(all / 256l)) {
        let sb = (m.ple_emb_off + blk * 256l) / 256l
        dequant_k5_plane_superblock(m.kq[int(KqFmt.k5)].q, sb * K5_QSB, m.kq[int(KqFmt.k5)].s, sb * K5_SSB, have, blk * 256l)
    }
    var want <- file_row(path, 0l, all)
    var num = 0.0lf
    var den = 0.0lf
    for (hv, w in have, want) {
        num += double(hv - w) * double(hv - w)
        den += double(w) * double(w)
    }
    p.rel_rms = float(sqrt(num / max(den, 1e-30lf)))
    delete want
    delete have
    return p
}

def private ple_kq_table_within(t : T?; p : PleKqTable) {
    t |> success(p.k5, "the fp32 load of gemma-4-E2B Q4_K_M: per_layer_token_embd pinned to its disk k5 plane")
    t |> success(p.no_q8, "no q8 side plane (the table rides the kq plane)")
    t |> success(p.rel_rms < 1e-3, "PLE table row 0 matches the file dequant (rel rms {p.rel_rms})")
}

//! the segmented window on the E-series (`ple_segments_cells`) on the segment form - two regions, the planes on the host, the
//! token table gathered on the device, the `SEG_WINDOW`-row prompt window - as its own cell
def private e2b_segments_on(t : T?; var m : Model) {
    unsafe {   //! the cell captures the loaded trunk by reference
        t |> run("gemma-4-E2B: each segment's side rows gather off its ids in one segmented window within 16% of solo windows, and with the token-table gate unseated host-embedded rows decline the segmented form") @capture(& m)(t : T ?) {
            ple_segments_cells(t, m)
        }
    }
}

//! the block drafter's CPU cells on the CPU form `m`, its context cut to HOST_CTX for them and put back, the Metal mode off
//! (put back): the trimmed trunk's refusal, each drafter file's lossless walk, the three sampled acceptances, and the
//! assistant head's refusal of the E-series trunk
def private e2b_drafter_cpu_cells(t : T?; var m : Model) {
    with_cpu_drafter_scope(m) {
        unsafe {   //! the cells capture the loaded trunk by reference
            t |> run("gemma-4-E2B: a planes-trimmed trunk refuses the attach where the pass has no device arm; the same trunk untrimmed attaches on the CPU arm") @capture(& m)(t : T ?) {
                trimmed_trunk_refusal(t, m)
            }
            t |> run("gemma-4-E2B, the Q8_0 DFlash2 drafter on the CPU arm: the greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
                cpu_drafter_lossless(t, m, E2B_DRAFTER_FILE, "E2B cpu", false)
            }
            t |> run("gemma-4-E2B, the Q4_K_M DFlash2 drafter: every plane tagged with its disk type's format, and on the CPU arm the greedy speculative walk emits the plain greedy walk's ids with drafts accepted") @capture(& m)(t : T ?) {
                cpu_drafter_lossless(t, m, E2B_DRAFTER_K4_FILE, "E2B k4 cpu", true)
            }
            t |> run("gemma-4-E2B, the NVFP4 DFlash2 drafter: every plane tagged nv4, and on the CPU arm the greedy speculative walk emits the plain greedy walk's ids with drafts accepted") @capture(& m)(t : T ?) {
                cpu_drafter_lossless(t, m, E2B_DRAFTER_NV4_FILE, "E2B nv4 cpu", true)
            }
            t |> run("gemma-4-E2B, the Q8_0 DFlash2 drafter on the CPU arm: the exact sampled walk equals the plain seeded sampled walk with drafts accepted and honors a stop id, and both rejection walks accept some drafts and reject some") @capture(& m)(t : T ?) {
                return if (!model_available(t, path_join(models_dir(), E2B_DRAFTER_FILE)))
                sampled_modes_walk(t, m, E2B_DRAFTER_FILE, false, "E2B sampled cpu")
            }
            t |> run("the attach refuses an E-series trunk: its verify rows need the per-layer input the draft ids index, and no sidecar exists for one") @capture(& m)(t : T ?) {
                assistant_head_refusal(t, m)
            }
        }
    }
}

//! the E2B trunk flagged planes-trimmed refuses the attach with the pass off the device; unflagged it attaches on the CPU arm
def private trimmed_trunk_refusal(t : T?; var m : Model) {
    let drafter = path_join(models_dir(), E2B_DRAFTER_FILE)
    return if (!model_available(t, drafter))
    let device0 = dflash_device_knob()
    set_dflash_device(false)
    m.planes_trimmed = true   //! the trimmed lane's flag: the CPU pass would read classifier planes such an image does not hold
    let why = attach_dflash_drafter(m, drafter)
    t |> success(find(why, "planes-trimmed") >= 0, "the attach names the trimmed image: '{why}'")
    t |> success(!dflash_drafter_attached(m), "... and leaves nothing attached")
    m.planes_trimmed = false
    t |> equal(attach_dflash_drafter(m, drafter), "", "control: the untrimmed trunk attaches")
    t |> success(!dflash_on_device(), "... its pass on the CPU arm")
    detach_dflash_drafter()
    set_dflash_device(device0)
}

//! the 26B's assistant head refuses the E2B trunk `tr` naming its per-layer embeddings, the facade attaching nothing; the same
//! config with no per-layer embeddings refuses on another clause (the control)
def private assistant_head_refusal(t : T?; tr : Model) {
    let side = path_join(models_dir(), "mtp-gemma-4-26B-A4B-it-Q8_0.gguf")
    return if (!model_available(t, side))
    var dr <- load_gemma_drafter(side)
    t |> success(dr.ok, "the drafter loads (why='{dr.why}')")
    let why = gemma_drafter_target_why(tr, dr)
    t |> success(find(why, "per-layer embeddings") >= 0, "the refusal names the per-layer embeddings: '{why}'")
    t |> success(!attach_mtp_drafter(tr, side), "the facade attaches nothing to the E2B")
    t |> success(!mtp_capable(tr), "and the E2B cannot speculate")
    var shell = Model()
    shell.config := tr.config
    shell.config.n_embd_per_layer = 0l
    let why_plain = gemma_drafter_target_why(shell, dr)
    t |> success(!empty(why_plain) && find(why_plain, "per-layer embeddings") < 0, "the control: the same trunk with no per-layer embeddings refuses on another clause: '{why_plain}'")
    delete shell
    delete dr
}

//! the block drafter's discrete-home cells on the drafter form `m`: the Q8_0 file's legs with its stop id and embedding call,
//! then the Q4_K_M and NVFP4 files' legs
def private e2b_drafter_discrete_cells(t : T?; m : Model) {
    discrete_drafter_cell(t, m, "gemma-4-E2B's resident trunk on the serving discrete home, the Q8_0 DFlash2 drafter: the greedy walk on the device arm is the plain walk's, every pass on the device's top-k and walk, the host's walk and selector drafting the same ids; a stop id ends the drafted stream on its first occurrence, and an embedding call commits no drafter context and leaves the next walk lossless",
        "", [E2B_DRAFTER_FILE], "E2B discrete", true)
    discrete_drafter_cell(t, m, "gemma-4-E2B's resident trunk on the serving discrete home, the Q4_K_M and NVFP4 DFlash2 drafters: each greedy walk on the device arm is the plain walk's, every pass on the device's top-k and walk, the host's walk and selector drafting the same ids",
        "", [E2B_DRAFTER_K4_FILE, E2B_DRAFTER_NV4_FILE], "E2B discrete")
}
