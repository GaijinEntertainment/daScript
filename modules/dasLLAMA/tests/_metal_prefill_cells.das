options gen2
options _dasllama_internal = true

module _metal_prefill_cells shared public

//! The Metal prefill's parity cells per model file, on the blob twin
//! a block's `metal_form()` loads: a session a cell prefills there is
//! kept for the CPU form to decode, the CPU controls run on the
//! block's portable CPU form, the verdict compares; no entry loads a
//! model

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 - the public facade
require dasllama/dasllama_math   // get_dispatch_gpu_served - the spin window witness
require ?das_metal dasllama/dasllama_metal_prefill   // the prefill's knobs and stats
require ?das_metal dasllama/dasllama_metal_decode   // the decode's stats and its mirror cap
require ?das_metal dasllama/dasllama_metal_common   // g_dev and the residency set
require ?das_metal das_metal   // metal_new_residency_set, metal_release
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 - g_env_metal, das_metal halves
require daslib/jobque_boost
require math
require strings
require daslib/algorithm
require _model_tier
require _model_form   // with_ctx_cut, cpu_cells_form, alone_pass_of
require _compares
require _metal_blob_twin   // with_metal_rails, metal_kernel_count, segments_window
require ../performance/model_specs.das
require _kernel_census   // nolint:STYLE029,STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves

//! the largest context each model file's cells cut their form to
let PREFILL_LLAMA_1B_Q8_CTX = 2048l
let PREFILL_LLAMA_1B_Q4KM_CTX = 2048l
let PREFILL_LLAMA_1B_IQ4XS_CTX = 2048l
let PREFILL_LLAMA_1B_Q51_CTX = 512l
let PREFILL_LLAMA_3B_CTX = 131072l
let PREFILL_LLAMA31_8B_CTX = 512l
let PREFILL_SMOL_CTX = 256l
let PREFILL_GEMMA3_1B_CTX = 256l
let PREFILL_GEMMA4_12B_12L_CTX = 256l
let PREFILL_GEMMA4_E2B_CTX = 256l
let PREFILL_QWEN15_MOE_4L_CTX = 512l
let PREFILL_QWEN3_30B_7L_CTX = 512l
let PREFILL_GPTOSS_4L_CTX = 512l
let PREFILL_QWEN15_MOE_CTX = 512l
let PREFILL_QWEN3_30B_CTX = 512l
let PREFILL_QWEN3_4B_CTX = 512l
let PREFILL_QWEN25_IQ4NL_CTX = 512l
let PREFILL_QWEN3_30B_7L_Q3KXL_CTX = 512l
let PREFILL_QWEN3_30B_7L_IQ2XXS_CTX = 512l

let private HYBRID_CTX = 512l    //! a cell whose session the CPU form decodes
let private MIRROR_CTX = 2048l   //! the mirror cells: a 128 MiB f32 slice
let private WINDOW_CTX = 512l    //! the short-window and scratch cells
let private LASTROW_CTX = 256l   //! the 48-row and 24-row windows

let private COUNT_TO = 120   //! past 300 llama-3 BPE tokens
let private ATTN_DEV_COUNT_TO = 260   //! past 684 llama-3 BPE tokens
let private ATTN_DEV_W1 = 300   //! pads to 320 query rows: off the 128-row stamps
let private ATTN_DEV_W2 = 384   //! a whole number of 128-row tiles
let private ATTN_DEV_BAR = 0.004   //! of the row's largest logit
let private MIRROR_STEP_W = 20   //! the mirror arm's short window
let private CODEC_FEED_STEPS = 12   //! the greedy steps after a codec session's windows
let private LONG_COUNT_TO = 11000   //! f16 K/V past 4096 MiB on Llama-3.2-3B
let private LONG_WINDOW = 1024l   //! the long prompt's window
let private LONG_STEPS = 6   //! the greedy steps over that context
let private MEDIA_HEAD = 20l   //! the mrope and deepstack turns' head and tail
let private MEDIA_ROWS = 24l   //! ... and their media rows
let private SCRATCH_SHORT = 200l   //! pads to 224 rows: under a crowned twin's floor
let private SCRATCH_LONG = 400l    //! pads to 416 rows: past a crowned twin's floor
let private SCRATCH_BAR = 0.08
//! every dense twin crown: a carrier's mixed planes all sit on twins
let private SCRATCH_TWIN_CROWNS = ("mulmm_q8,kq_mulmm_k4,kq_mulmm_k5,kq_mulmm_k6,kq_mulmm_iq4xs," +
    "kq_mulmm_iq4nl,kq_mulmm_q40,kq_mulmm_nv4,kq_mulmm_k3,kq_mulmm_iq3s,kq_mulmm_iq3xxs," +
    "kq_mulmm_k2,kq_mulmm_iq2s,kq_mulmm_iq2xs,kq_mulmm_iq2xxs,kq_mulmm_q51,kq_mulmm_iq4nl32," +
    "kq_mulmm_q4032,kq_mulmm_q5032")
let private SCRATCH_GRID_FLOOR = 1l << 40l   //! a grid floor no site's tile reaches
let private SCRATCH_HMM_KNEE = 4096l   //! the dev-W half tile's split-K knee, widened
let private BF16_SPLIT_ROWS = 24l   //! one 32-row tile
let private BF16_SPLIT_KNEE = 1024l   //! wide enough that the E2B projection splits
let private CROWN_MIRROR = "mulmm_q8,attn_qkmm,attn_dev"
let private CROWN_PANEL = "mulmm_q8,attn_qkmm"

//! the short-window form under test: the routed block's gathered
//! route, or the dense K-quant sites' row-batched GEMV peel
enum ShortForm {
    moe_route
    kq_tail
}

//! whether the last-row cell holds the argmax: a whole file does
enum private Carrier {
    whole_file
    layer_cut
}

//! the control the CPU form replays for one kept cell
enum PrefillReplay {
    continuation   //! one prompt
    chunked        //! two windows on one session
    span           //! a causal head, then a non-causal span
    fused          //! head, media rows and tail in one prefill
    mrope          //! the fused turn, its media roped as a grid
    deepstack      //! the fused turn on wide rows
    generate       //! the engine's generate
    codec_windows  //! two windows, greedy steps on the codec
}

//! one Metal leg of a cell: its tokens, or the session it prefilled
//! for the CPU form to decode
struct PrefillLeg {
    text : string      //!< the assert the verdict holds the leg to
    log_tag : string   //!< the CPU side's text log, "" none
    decode : bool      //!< `s` waits for the CPU form's greedy steps
    s : Session
    ids : array<int64>
}

//! one cell's inputs, its Metal legs and its CPU control
struct PrefillCell {
    replay : PrefillReplay
    kdt : KVDtype
    steps : int
    ctx : int64        //!< the context the kept sessions were made at
    want_len : int     //!< past 0: the control's length, asserted
    chunks : array<array<int64>>
    grid : int2
    legs : array<PrefillLeg>
    cpu : array<int64>
    gpu_rows : array<array<float>>   //!< deepstack: tails, then zero tails
    cpu_rows : array<array<float>>
}

//! one short window: its rows, the Metal legs' logits and pieces
struct PrefillShortWin {
    n : int64
    w2 : array<int64>
    gpu : array<float>
    ctrl : array<float>
    tiles : array<float>
    cpu : array<float>
    gpu_piece : string
    tiles_piece : string
    cpu_piece : string
}

//! the short-window cell of a file
struct PrefillShort {
    on : bool
    tag : string
    form_name : string
    bar : float
    whole : bool
    ctx : int64
    w1 : array<int64>
    wins : array<PrefillShortWin>
}

//! one scratch-panel window's Metal legs
struct PrefillScratchWin {
    tag : string
    n : int64
    sites : int64
    gpu : array<float>
    own : array<float>
    ctrl : array<float>
}

//! the scratch-panel cell of a file
struct PrefillScratch {
    on : bool
    ctx : int64
    long_w : array<int64>   //!< the long window; the short is its head
    wins : array<PrefillScratchWin>
    cpu_short : array<float>
    cpu_long : array<float>
}

//! one routed-GEMM leg of the moe-base cell: its logits and pick
struct PrefillBaseLeg {
    leg : string
    gpu : array<float>
    ctrl : array<float>
    gpu_piece : string
}

//! the moe-base cell of a file
struct PrefillBase {
    on : bool
    tag : string
    bar : float
    ctx : int64
    w1 : array<int64>
    w2 : array<int64>
    legs : array<PrefillBaseLeg>
    cpu : array<float>
    cpu_piece : string
}

//! what a file's Metal cells hand its CPU form and its verdict
struct PrefillKept {
    file : string
    metal_ran : bool
    cpu_ran : bool
    cpu_iq3s : string   //!< the IQ3_S serving pin of the CPU form awaited
    cells : array<PrefillCell>
    window : PrefillShort
    panels : PrefillScratch
    routed : PrefillBase
}

//! the parameters of one model file's cells
struct PrefillRow {
    file : string     //!< the model file under the models folder
    family : string   //!< the family every cell of the file runs under
    ctx : int64       //!< the largest context the cells cut to
    main : bool       //!< the main arm list's cells: JIT-only
    q8_set : bool     //!< the 1B Q8 file's own arms
    cont40 : string   //!< the 40-token continuation arm: kq, dim, ""
    segs : string     //!< the segs arm's format name, "" none
    smol : bool       //!< the mm-tail arm's alignment-guard leg
    mirror_long : bool   //!< the mirror-cap arm's long-context leg
    short_tag : string   //!< the short-window cell's tag, "" none
    short_bar : float
    short_form : ShortForm
    whole : bool      //!< a whole file: the argmax held
    blocks : int64    //!< past 0: the blocks a cut file claims
    scratch_tag : string   //!< the scratch-panel cell's tag, "" none
    scratch_twin_long : bool   //!< the long window under the twin floor
    lastrow_bar : float   //!< past 0: the last-row cell's bar
    bf16 : bool       //!< the bf16 projection's split cell
    base_tag : string      //!< the moe-base cell's tag, "" none
    base_kernel : string   //!< ... its base form's census name
    base_crown : string    //!< ... and the crown of its tensor twins
    base_bar : float
    cpu_iq3s : string   //!< the CPU form's IQ3_S serving pin, "" none
}

def private slice_ids(p : array<int64>; a, b : int64) : array<int64> =>
    <- [for (i in range64(a, b)); p[i]]

//! `prompt` cut at each of `ends` into consecutive chunks
def private cut_chunks(prompt : array<int64>; ends : array<int64>) : array<array<int64>> {
    var chunks : array<array<int64>>
    chunks |> resize(length(ends))
    var at = 0l
    for (e, i in ends, count()) {
        chunks[i] <- slice_ids(prompt, at, e)
        at = e
    }
    return <- chunks
}

//! the last row's logits after two f16 windows, and one step of
//! `step_tok` after them where it is a token
def private two_window_logits(trp : Model; w1, w2 : array<int64>;
                              step_tok : int64 = -1l) : array<float> {
    var out : array<float>
    with_dasllama_jobque() {
        var s = make_run_state(trp.config, KVDtype.f16, KVDtype.f16)
        eval_(trp, s, w1)
        eval_(trp, s, w2)
        if (step_tok >= 0l) {
            var one <- [step_tok]
            eval_(trp, s, one)
            delete one
        }
        out := s.logits
        delete s
    }
    return <- out
}

//! two windows then `n` greedy steps on one session of codec `kdt`
def private codec_two_window_greedy(trp : Model; w1, w2 : array<int64>; kdt : KVDtype;
                                    n : int) : array<int64> {
    var ids : array<int64>
    with_dasllama_jobque() {
        var s = make_run_state(trp.config, kdt, kdt)
        eval_(trp, s, w1)
        eval_(trp, s, w2)
        ids <- greedy_eval_steps(trp, s, n) // nolint:PERF030 - fresh; the block runs once
        delete s
    }
    return <- ids
}

//! the non-causal span of `tail`'s own embedding rows
def private span_eval(m : Model; var s : Session; tail : array<int64>) {
    let ntail = long_length(tail)
    var rows : array<float>
    rows |> reserve(ntail * m.config.dim)
    rows |> resize(ntail * m.config.dim)
    embed_text_rows(m, tail, rows, 0l)
    eval_embd(m, s, rows, ntail, true)
    delete rows
}

//! head, media and tail in one span eval, the media roped as `grid`
//! where `mrope`
def private media_eval(m : Model; var s : Session; chunks : array<array<int64>>; mrope : bool;
                       grid : int2) {
    let nh = long_length(chunks[0])
    let nm = long_length(chunks[1])
    let npos = nh + nm + long_length(chunks[2])
    var rows : array<float>
    rows |> reserve(npos * m.config.dim)
    rows |> resize(npos * m.config.dim)
    embed_text_rows(m, chunks[0], rows, 0l)
    embed_text_rows(m, chunks[1], rows, nh)
    embed_text_rows(m, chunks[2], rows, nh + nm)
    if (mrope) {
        eval_embd_span_mrope_(m, s, rows, npos, nh, nh + nm, int64(grid.x), int64(grid.y))
    } else {
        eval_embd_span_(m, s, rows, npos, nh, nh + nm)
    }
    delete rows
}

//! the deepstack turn: slice 0 the tokens' own rows, the tails small
//! pseudo-random values where `tails`, else zero
def private deepstack_eval(m : Model; var s : Session; chunks : array<array<int64>>;
                           tails : bool) {
    let dim = m.config.dim
    let nds = m.config.n_deepstack
    let rw = (1l + nds) * dim
    let nh = long_length(chunks[0])
    let nm = long_length(chunks[1])
    let npos = nh + nm + long_length(chunks[2])
    var rows : array<float>
    rows |> reserve(npos * rw)
    rows |> resize(npos * rw)
    embed_text_rows_strided(m, chunks[0], rows, 0l, rw)
    embed_text_rows_strided(m, chunks[1], rows, nh, rw)
    embed_text_rows_strided(m, chunks[2], rows, nh + nm, rw)
    if (tails) {
        var state = 0xBEEFu
        for (r in range64(npos)) {
            for (i in range64(nds * dim)) {
                state = state * 1664525u + 1013904223u
                rows[r * rw + dim + i] = (float(state >> 8u) / float(1 << 24) - 0.5) * 0.05
            }
        }
    }
    eval_embd_span_(m, s, rows, npos, nh, nh + nm)
    delete rows
}

//! a cell's prefill on `s`, the Metal leg's and the CPU control's
def private cell_prefill(m : Model; var s : Session; replay : PrefillReplay;
                         chunks : array<array<int64>>; grid : int2; tails : bool) {
    if (replay == PrefillReplay.span) {
        eval_(m, s, chunks[0])
        span_eval(m, s, chunks[1])
    } elif (replay == PrefillReplay.fused || replay == PrefillReplay.mrope) {
        media_eval(m, s, chunks, replay == PrefillReplay.mrope, grid)
    } elif (replay == PrefillReplay.deepstack) {
        deepstack_eval(m, s, chunks, tails)
    } else {
        for (c in chunks) {
            eval_(m, s, c)
        }
    }
}

//! a fresh session's last-row logits after the prefill `replay`
def private prefill_rows(m : Model; kdt : KVDtype; replay : PrefillReplay;
                         chunks : array<array<int64>>; grid : int2; tails : bool) : array<float> {
    var lg : array<float>
    with_dasllama_jobque() {
        var s = make_run_state(m.config, kdt, kdt)
        cell_prefill(m, s, replay, chunks, grid, tails)
        lg := s.logits
        delete s
    }
    return <- lg
}

def private new_cell(m : Model; replay : PrefillReplay; kdt : KVDtype; steps : int) : PrefillCell =>
    <- PrefillCell(replay = replay, kdt = kdt, steps = steps, ctx = m.config.seq_len)

//! a Metal leg: the cell's prefill on a fresh session, kept for the
//! CPU form's greedy steps
def private metal_leg(m : Model; var cell : PrefillCell; text, log_tag : string) {
    var leg <- PrefillLeg(text = text, log_tag = log_tag, decode = true)
    leg.s <- make_run_state(m.config, cell.kdt, cell.kdt)
    with_dasllama_jobque() {
        cell_prefill(m, leg.s, cell.replay, cell.chunks, cell.grid, true)
    }
    cell.legs |> emplace(leg)
}

//! a Metal leg that ran whole on the device: its tokens
def private ids_leg(var cell : PrefillCell; text, log_tag : string; ids : array<int64>) {
    var leg <- PrefillLeg(text = text, log_tag = log_tag)
    leg.ids := ids
    cell.legs |> emplace(leg)
}

struct private ConfigStamp {
    rope_sections : int4
    mrope_interleaved : bool
    n_deepstack : int64
}

//! the mrope or deepstack stamp a 1B carries for its turn, the prior
//! config returned
def private stamp_config(var m : Model; replay : PrefillReplay) : ConfigStamp {
    let prior = ConfigStamp(rope_sections = m.config.rope_sections,
        mrope_interleaved = m.config.mrope_interleaved, n_deepstack = m.config.n_deepstack)
    if (replay == PrefillReplay.mrope) {
        m.config.rope_sections = int4(12, 10, 10, 0)   //! the 1B is no mrope arch
        m.config.mrope_interleaved = true
    } elif (replay == PrefillReplay.deepstack) {
        m.config.n_deepstack = 3l   //! the 1B is no deepstack arch
    }
    return prior
}

def private restore_config(var m : Model; prior : ConfigStamp) {
    m.config.rope_sections = prior.rope_sections
    m.config.mrope_interleaved = prior.mrope_interleaved
    m.config.n_deepstack = prior.n_deepstack
}

struct private CrownSeat {
    crowns : string
    pinned : bool
}

def private crown_seat() : CrownSeat =>
    CrownSeat(crowns = get_metal_tensor_crowns(), pinned = metal_tensor_crowns_pinned())

def private restore_crowns(seat : CrownSeat) {
    if (seat.pinned) {
        pin_metal_tensor_crowns(seat.crowns)
    } else {
        unpin_metal_tensor_crowns()
        set_metal_tensor_crowns(seat.crowns)
    }
}

//! the dispatches since the census reset of every stamp whose name
//! carries both `family` and `form`
def private stamp_dispatches(family,
                             form : string) : int64 {    // nolint:LINT012,LINT019 - das_metal half
    var n = 0l
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope cov <- metal_kernel_coverage()
        for (name, c in keys(cov), values(cov)) {
            n += find(name, family) >= 0 && find(name, form) >= 0 ? c : 0l
        }
    }
    return n
}

def private split16_dispatches() : int64 => stamp_dispatches("MetalKqMulMm", "Sk16")

def private hmm_split_dispatches() : int64 => stamp_dispatches("MetalHalfMulMm", "Sk")

def private bf16_split_dispatches() : int64 => stamp_dispatches("MetalBf16MulMm", "Sk")

//! the residency witness after a served prefill: whether the API
//! serves on this OS
def private residency_witness(t : T?) : bool {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var api_probe = metal_new_residency_set(dasllama_metal_common::g_dev)
        let live = api_probe != null
        if (live) {
            metal_release(api_probe)
        }
        if (!g_env_metal.residency) {
            t |> equal(residency_pinned(), 0l, "DASLLAMA_METAL_RESIDENCY=0 pins nothing")
        } elif (!live) {
            t |> skip(("MTLResidencySet unavailable (macOS < 15) - the pin witness has no API to " +
                "observe"))
        } else {
            t |> success(residency_active(), "the residency set is live on an API-serving OS")
            t |> success(residency_pinned() > 0l,
                "the served prefill pinned {residency_pinned()} buffers in the residency set")
            if (g_env_metal.heartbeat_s > 0) {
                t |> success(metal_residency_heartbeat_live(),
                    "the residency heartbeat is armed after a served step")
            } else {
                t |> success(!metal_residency_heartbeat_live(),
                    "DASLLAMA_METAL_HEARTBEAT_S=0 leaves the heartbeat unarmed")
                t |> equal(metal_residency_heartbeat_sets(), 0l, ("DASLLAMA_METAL_HEARTBEAT_S=0 " +
                    "registers nothing (the kick is skipped, not zero-length)"))
            }
        }
        return live
    } else {
        return false
    }
}

//! the weights-epoch push contract after a served prefill
[unused_argument(t, spin_gpu_before, api_live)]
def private epoch_witness(t : T?;
                          spin_gpu_before, api_live : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(weight_regions_cached() > 0l,
            "the served prefill cached {weight_regions_cached()} weight regions")
        t |> success(!spin_gpu_before,
            "the pool ran the CPU spin window before any Metal forward served")
        t |> success(get_dispatch_gpu_served(),
            "a served Metal prefill moved the pool to the GPU-served spin window")
        let preps0 = reload_preps_run()
        bump_weights_epoch()
        t |> success(!get_dispatch_gpu_served(),
                     "a weights load returns the pool to the CPU spin window")
        t |> equal(weight_regions_cached(), 0l,
            "a weights-epoch bump alone drops the address-keyed region caches")
        t |> success(reload_preps_run() > preps0, ("the drop ran the registered reload preps " +
            "(decode's discard_pre reached through the registry)"))
        if (g_env_metal.residency && api_live) {
            var early : array<float>
            early |> resize(4096)
            let eb = plane_buffer(dasllama_metal_common::g_dev, unsafe(addr<void?>(early[0])),
                                  16384ul,
                false)
            t |> success(eb != null && weight_regions_cached() == 1l,
                "a plane cached with no residency set live")
            t |> equal(weight_regions_unpinned(), 0l,
                "a region cached before the first flush is in the residency set")
            bump_weights_epoch()
            delete early
        }
    }
}

//! the base arm: the prompt's one Metal prefill, its 40 greedy steps
//! the CPU form's; false where the prefill declined
def private base_metal(t : T?; m : Model; var kept : PrefillKept;
                       prompt : array<int64>) : bool {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        bump_weights_epoch()   //! the spin window as a load leaves it
        var served = false
        var spin_gpu_before = false
        var cell <- new_cell(m, PrefillReplay.continuation, KVDtype.f32, 40)
        cell.want_len = 40
        cell.chunks |> push_clone(prompt)
        with_metal_rails(t, "base", MetalRails(decode = false)) {
            let before = metal_prefill_stats().prefills
            spin_gpu_before = get_dispatch_gpu_served()
            metal_leg(m, cell, ("40-token continuation matches the CPU control (blob mul_mm, " +
                "f32 activations)"), "")
            served = metal_prefill_stats().prefills - before > 0l
        }
        if (served) {
            kept.cells |> emplace(cell)
            let api_live = residency_witness(t)
            epoch_witness(t, spin_gpu_before, api_live)
        } else {
            delete cell
            feint("metal prefill declined (no Metal device?); GPU parity skipped\n")
        }
        return served
    } else {
        return false
    }
}

//! the mm-tail arm's 1B leg: the GEMV-tail residues 4, 257, 258 and
//! 261 against the control, tail on and off, the engage counters held
def private mm_tail_metal(t : T?; m : Model; var kept : PrefillKept;
                          prompt : array<int64>) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "mm-tail"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_tail = get_metal_prefill_mm_tail()
        set_metal_prefill_mm_tail(true)   //! the engage claim depends on it
        kept.cells |> reserve(length(kept.cells) + 4)
        for (nres in [4l, 257l, 258l, 261l]) {
            continue if (nres > long_length(prompt))
            var cell <- new_cell(m, PrefillReplay.continuation, KVDtype.f32, 24)
            cell.chunks |> emplace(slice_ids(prompt, 0l, nres))
            var served, dsites, drows, sites1 = 0l
            with_metal_rails(t, "mm-tail {nres}", MetalRails(decode = false, min_npos = 0l)) {
                let served0 = metal_prefill_stats().prefills
                let tail0 = metal_prefill_tail_stats()
                metal_leg(m, cell, ("npos={nres}: 24-token continuation matches the all-CPU " +
                    "control (GEMV tail)"), "mm-tail/{nres}")
                served = metal_prefill_stats().prefills - served0
                dsites = metal_prefill_tail_stats().sites - tail0.sites
                drows = metal_prefill_tail_stats().rows - tail0.rows
                set_metal_prefill_mm_tail(false)
                sites1 = metal_prefill_tail_stats().sites
                metal_leg(m, cell, "npos={nres}: the tail-off twin matches the control too (A/B)",
                    "mm-tail-off/{nres}")
                set_metal_prefill_mm_tail(true)
            }
            if (served == 0l) {
                delete cell
                feint("metal prefill declined (no Metal device?); mm-tail parity skipped\n")
                continue
            }
            t |> success(dsites > 0l,
                         "npos={nres}: the GEMV tail engaged ({dsites} sites, {drows} rows)")
            t |> equal(drows, dsites * (nres % 32l),
                "npos={nres}: every peeled site carried the {nres % 32l}-row remainder")
            t |> equal(metal_prefill_tail_stats().sites - sites1, 0l,
                "npos={nres}: tail off - the padded-tile path serves")
            kept.cells |> emplace(cell)
        }
        set_metal_prefill_mm_tail(prior_tail)
    }
}

struct private Cont40 {
    arm : string
    name : string
    served : string
    match : string
}

def private cont40_of(arm : string) : Cont40 {
    if (arm == "kq") {
        return Cont40(arm = "kq", name = "kq", served = "kq prefill served on the GPU",
            match = "40-token kq continuation matches the CPU control (kq mul_mm + kq classifier)")
    }
    if (arm == "dim") {
        return Cont40(arm = "dim", name = "dim4096", served = "dim-4096 prefill served on the GPU",
            match = ("40-token dim-4096 continuation matches the CPU control (unfused prefill " +
                "set)"))
    }
    return Cont40(arm = "s16", name = "s16", served = "wscale16 prefill served on the GPU",
        match = ("40-token wscale16 continuation matches the CPU control (s16 mul_mm + s16 " +
            "classifier)"))
}

//! the s16, kq and dim arms: the prompt's Metal prefill, its 40
//! greedy steps the CPU form's
def private cont40_metal(t : T?; m : Model; var kept : PrefillKept; prompt : array<int64>;
                         c : Cont40) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, c.arm))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (c.arm == "s16" && !m.wscale_f16) {
            t |> skip("s16: the form loaded f32 scales - the arm reads the f16 plane")
            return
        }
        var cell <- new_cell(m, PrefillReplay.continuation, KVDtype.f32, 40)
        cell.chunks |> push_clone(prompt)
        with_metal_rails(t, c.name, MetalRails(decode = false)) {
            let before = metal_prefill_stats().prefills
            metal_leg(m, cell, c.match, "")
            t |> success(metal_prefill_stats().prefills - before > 0l, c.served)
        }
        kept.cells |> emplace(cell)
    }
}

//! a two-chunk codec arm: its replay, and its texts around the
//! session codec's name
struct private CodecArm {
    arm : string
    replay : PrefillReplay
    lead : string     //!< the leg's text before the codec
    match : string    //!< ... and after it
    log : string      //!< the CPU side's log tag before the codec, "" none
    served : string   //!< the served assert after the codec
}

//! the cont arm: two chunks, fresh then start_pos > 0
def private chunked_arm() : CodecArm => CodecArm(arm = "cont", replay = PrefillReplay.chunked,
    lead = "chunked ", match = "continuation matches the all-CPU control (M2.5)",
    served = "chunks served on the GPU (fresh + start_pos > 0 continuation)")

//! the span arm: a causal head, a non-causal span at start_pos > 0
def private span_arm() : CodecArm => CodecArm(arm = "span", replay = PrefillReplay.span,
    match = "span turn matches the all-CPU control", log = "span",
    served = "head + span both served on the GPU")

//! the arm `a`'s two chunks on the device on each session codec, the
//! greedy steps the CPU form's
def private codec_arm_metal(t : T?; m : Model; var kept : PrefillKept; prompt : array<int64>;
                            a : CodecArm) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, a.arm))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let nfull = long_length(prompt)
        var inscope chunks <- cut_chunks(prompt, [nfull * 3l / 5l, nfull])
        kept.cells |> reserve(length(kept.cells) + 3)
        for (kdt in [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4]) {
            var cell <- new_cell(m, a.replay, kdt, 24)
            cell.chunks := chunks
            let pf0 = metal_prefill_stats().prefills
            with_metal_rails(MetalRails(decode = false)) {
                metal_leg(m, cell, "{a.lead}{kdt} {a.match}", empty(a.log) ? "" : "{a.log}/{kdt}")
            }
            t |> success(metal_prefill_stats().prefills - pf0 == 2l, "{kdt} {a.served}")
            kept.cells |> emplace(cell)
        }
    }
}

//! the fused turn against the three-eval splice on the device, the
//! form-swap levers (split-K's knee with them) and the attention
//! crowns off for both legs
def private fused_splice(t : T?; m : Model; cell : PrefillCell;
                         bar : float) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let kdt = cell.kdt
        let prior_tail = get_metal_prefill_mm_tail()
        let prior_devw = get_metal_prefill_dev_w()
        let prior_npos = get_metal_prefill_min_npos()
        let prior_sk = get_metal_prefill_split_k()
        let prior_half_x = get_metal_prefill_half_x()
        let prior_tall = get_metal_prefill_tall()
        let prior_fuse = get_span_fuse()
        set_metal_prefill_split_k(false)
        set_metal_prefill_mm_tail(false)
        set_metal_prefill_half_x(false)
        set_metal_prefill_dev_w(false)
        set_metal_prefill_tall(false)
        let box_crowns = get_metal_tensor_crowns()
        set_metal_tensor_crowns(crowns_without(box_crowns, ["attn_qkmm", "attn_dev"]))
        metal_prefill_shutdown()
        var inscope lg_fused <- prefill_rows(m, kdt, cell.replay, cell.chunks, cell.grid, true)
        set_span_fuse(false)
        set_metal_prefill_min_npos(1l)
        let pfsp0 = metal_prefill_stats().prefills
        var inscope lg_splice <- prefill_rows(m, kdt, cell.replay, cell.chunks, cell.grid, true)
        set_metal_tensor_crowns(box_crowns)
        metal_prefill_shutdown()
        set_metal_prefill_mm_tail(prior_tail)
        set_metal_prefill_half_x(prior_half_x)
        set_metal_prefill_dev_w(prior_devw)
        set_metal_prefill_tall(prior_tall)
        set_metal_prefill_split_k(prior_sk)
        t |> success(metal_prefill_stats().prefills - pfsp0 == 3l,
            "{kdt} splice leg served as THREE GPU prefills")
        set_metal_prefill_min_npos(prior_npos)
        set_span_fuse(prior_fuse)
        let lmax = logits_maxdiff(lg_fused, lg_splice)
        let amax_f = argmax_of(lg_fused)
        let amax_s = argmax_of(lg_splice)
        to_log(LOG_INFO, ("span-fused/{kdt} GPU fused-vs-splice prefill-logits maxdiff = {lmax} " +
            "(argmax fused {amax_f} @{lg_fused[amax_f]}, splice {amax_s} @{lg_splice[amax_s]})\n"))
        var inscope pick_f <- [amax_f]
        var inscope pick_s <- [amax_s]
        log_gen_texts(m, "span-fused/{kdt} pick", pick_f, pick_s)
        t |> success(lmax < bar, "{kdt} fused == splice on the GPU ({lmax})")
    }
}

//! the span-fused arm: head, media and tail in ONE device prefill per
//! codec, npos % 64 == 32, and fused == splice on the device
def private fused_metal(t : T?; m : Model; var kept : PrefillKept;
                        prompt : array<int64>) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "span-fused"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let nfull = long_length(prompt)
        var ntarget = (nfull / 64l) * 64l + 32l
        if (ntarget > nfull) {
            ntarget -= 64l
        }
        t |> success(ntarget % 64l == 32l && ntarget >= 96l,
            "fused-turn shape pins the ceil32 tail block (npos = {ntarget})")
        var inscope chunks <- cut_chunks(prompt, [ntarget * 2l / 5l, ntarget * 4l / 5l, ntarget])
        //! clean f16 = 0, q8 0.073, tq4 0.708; a poisoned mask ~5.1
        var inscope fbars <- { KVDtype.f16 => 1e-4, KVDtype.q8_0 => 0.5, KVDtype.tq4 => 2.0 }
        kept.cells |> reserve(length(kept.cells) + 3)
        for (kdt in [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4]) {
            var cell <- new_cell(m, PrefillReplay.fused, kdt, 24)
            cell.chunks := chunks
            with_metal_rails(MetalRails(decode = false, min_npos = 0l)) {
                let pff0 = metal_prefill_stats().prefills
                metal_leg(m, cell, "{kdt} fused turn matches the all-CPU control",
                    "span-fused/{kdt}")
                t |> success(metal_prefill_stats().prefills - pff0 == 1l,
                    "{kdt} fused turn served as ONE GPU prefill")
                fused_splice(t, m, cell, fbars?[kdt] ?? 0.0)
            }
            kept.cells |> emplace(cell)
        }
    }
}

//! a media-turn arm on a 1B stamped as its arch: the replay, the
//! turn's name in the texts, its grid
struct private MediaArm {
    arm : string
    replay : PrefillReplay
    what : string
    grid : int2
}

//! the span-mrope arm: the grid-roped turn, the grid moving the
//! logits off the sequential span's
def private mrope_arm() : MediaArm => MediaArm(arm = "span-mrope", replay = PrefillReplay.mrope,
    what = "mrope", grid = int2(6, 4))   //! a 6x4 merged grid: advance 6, delta -18

//! the span-ds arm: the deepstack turn, the add's rows with and
//! without tails kept
def private ds_arm() : MediaArm => MediaArm(arm = "span-ds", replay = PrefillReplay.deepstack,
    what = "deepstack")

//! the grid-roped turn's logits against the sequential span's
def private mrope_grid_moves(t : T?; m : Model; cell : PrefillCell) {
    let kdt = cell.kdt
    var inscope lg_m <- prefill_rows(m, kdt, PrefillReplay.mrope, cell.chunks, cell.grid, true)
    var inscope lg_s <- prefill_rows(m, kdt, PrefillReplay.fused, cell.chunks, cell.grid, true)
    let gdiff = logits_maxdiff(lg_m, lg_s)
    to_log(LOG_INFO,
           "span-mrope/{kdt} grid-vs-sequential GPU prefill-logits maxdiff = {gdiff}\n")
    t |> success(gdiff > 1e-3, "{kdt} the grid moves the GPU prefill logits ({gdiff})")
}

//! the arm `a`'s turn as ONE device prefill per codec on the stamped
//! 1B, the mrope grid's move asserted or the deepstack rows kept
def private media_arm_metal(t : T?; var m : Model; var kept : PrefillKept; prompt : array<int64>;
                            a : MediaArm) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, a.arm))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior = stamp_config(m, a.replay)
        let mh = MEDIA_HEAD + MEDIA_ROWS
        var inscope chunks <- cut_chunks(prompt, [MEDIA_HEAD, mh, mh + MEDIA_HEAD])
        kept.cells |> reserve(length(kept.cells) + 2)
        for (kdt in [KVDtype.f16, KVDtype.q8_0]) {
            var cell <- new_cell(m, a.replay, kdt, 24)
            cell.chunks := chunks
            cell.grid = a.grid
            with_metal_rails(MetalRails(decode = false, min_npos = 0l)) {
                let pf0 = metal_prefill_stats().prefills
                metal_leg(m, cell, "{kdt} {a.what} turn matches the all-CPU control",
                    "{a.arm}/{kdt}")
                t |> success(metal_prefill_stats().prefills - pf0 == 1l,
                    "{kdt} {a.what} turn served as ONE GPU prefill (no decline)")
                if (a.replay == PrefillReplay.mrope) {
                    mrope_grid_moves(t, m, cell)
                } else {
                    cell.gpu_rows <- [
                        prefill_rows(m, kdt, a.replay, chunks, cell.grid, true),
                        prefill_rows(m, kdt, a.replay, chunks, cell.grid, false)]
                }
            }
            kept.cells |> emplace(cell)
        }
        restore_config(m, prior)
    }
}

//! the qkv arm: device prefill and decode on q8_0 and tq4 sessions
def private qkv_metal(t : T?; m : Model; var kept : PrefillKept;
                      prompt : array<int64>) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "qkv"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        kept.cells |> reserve(length(kept.cells) + 2)
        for (kdt in [KVDtype.q8_0, KVDtype.tq4]) {
            var cell <- new_cell(m, PrefillReplay.generate, kdt, 24)
            cell.chunks |> push_clone(prompt)
            var gpuq : array<int64>
            with_dasllama_jobque() {
                with_metal_rails(MetalRails()) {
                    var sb = make_run_state(m.config, kdt, kdt)
                    var inscope ids2 <- generate(m, sb, prompt, length(prompt) + 24)
                    gpuq := ids2
                    delete sb
                }
            }
            ids_leg(cell,
                    "GPU prefill x {kdt} KV: 24-token continuation matches the all-CPU control",
                "", gpuq)
            delete gpuq
            kept.cells |> emplace(cell)
        }
    }
}

//! the 1B Q8 file's arms on its kept-session context
def private q8_hybrid_metal(t : T?; var m : Model; var kept : PrefillKept; prompt : array<int64>) {
    mm_tail_metal(t, m, kept, prompt)
    cont40_metal(t, m, kept, prompt, cont40_of("s16"))
    codec_arm_metal(t, m, kept, prompt, chunked_arm())
    codec_arm_metal(t, m, kept, prompt, span_arm())
    media_arm_metal(t, m, kept, prompt, mrope_arm())
    media_arm_metal(t, m, kept, prompt, ds_arm())
    qkv_metal(t, m, kept, prompt)
    fused_metal(t, m, kept, prompt)
}

//! the attn-dev arm's stamp census over the device leg
def private attn_dev_stamps(t : T?; n_attn, pexp_staged : int64) {
    t |> equal(pexp_staged, 0l, "the held leg ran the staged trio")
    t |> equal(metal_kernel_count("metal_attn_pexp"), n_attn * 2l,
        "the device leg wrote every attention layer's rows in both windows")
    t |> equal(metal_kernel_count("MetalAttnQKDev_metal_attn_qk_dev"), n_attn,
        "the 32-row QK stamp served the first window")
    t |> equal(metal_kernel_count("MetalAttnQKDev128_metal_attn_qk_dev"), n_attn,
        "the 128-row QK stamp served the continuation")
    t |> equal(metal_kernel_count("MetalAttnAVDev_metal_attn_av_dev"), n_attn,
        "the 32-row AV stamp served the first window")
    t |> equal(metal_kernel_count("MetalAttnAVDev128_metal_attn_av_dev"), n_attn,
        "the 128-row AV stamp served the continuation")
}

//! the attn-dev arm's bar: device pair against staged trio
def private attn_dev_bar(t : T?; m : Model; staged, device, device_off : array<float>) {
    let mag = logits_maxabs(staged)
    let diff = logits_maxdiff(device, staged)
    let diff_off = logits_maxdiff(device_off, staged)
    var pick_device = 0l
    var pick_staged = 0l
    with_dasllama_jobque() {
        pick_device = argmax_of(device)
        pick_staged = argmax_of(staged)
    }
    to_log(LOG_INFO, ("attn-dev: device pair vs staged trio, last-row logits max difference " +
        "{diff} of max {mag} (bar {ATTN_DEV_BAR * mag}); one token off {diff_off}; next token " +
        "'{eyeball_text(m, [pick_device])}' vs '{eyeball_text(m, [pick_staged])}'\n"))
    t |> equal(pick_device, pick_staged,
               "both forms pick the same next token on the counting prompt")
    t |> success(diff <= ATTN_DEV_BAR * mag,
        "the device pair's logits sit within the bar of the staged trio's ({diff} of {mag})")
    t |> success(diff_off > ATTN_DEV_BAR * mag,
        "the prompt one token off lands outside the bar ({diff_off})")
}

//! the attn-dev arm: device pair against staged trio on the twin, a
//! 300-row window then a 384-row continuation; the drivers rebuilt
//! under the arm's crowns and again under the form's after
def private attn_dev_metal(t : T?; m : Model) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "attn-dev"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope longp <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
        t |> success(length(longp) > ATTN_DEV_W1 + ATTN_DEV_W2,
            "the prompt covers both windows ({length(longp)} tokens)")
        let w1e = int64(ATTN_DEV_W1)
        let w2e = int64(ATTN_DEV_W1 + ATTN_DEV_W2)
        var inscope w1 <- slice_ids(longp, 0l, w1e)
        var inscope w2 <- slice_ids(longp, w1e, w2e)
        var inscope w2_off <- slice_ids(longp, w1e + 1l, w2e + 1l)
        let crowns = crown_seat()
        pin_metal_tensor_crowns(CROWN_MIRROR)
        metal_prefill_shutdown()
        var inscope staged, device, device_off : array<float>
        with_metal_rails(MetalRails(decode = false)) {
            let was_held = set_metal_prefill_held("attn_dev", true)
            kernel_census_reset_metal()
            staged <- two_window_logits(m, w1, w2) // nolint:PERF030 - fresh; the block runs once
            let pexp_staged = metal_kernel_count("metal_attn_pexp")
            set_metal_prefill_held("attn_dev", false)
            t |> success(pf_attn_dev_ready(), "the device pair's stamps compiled under its crown")
            kernel_census_reset_metal()
            device <- two_window_logits(m, w1, w2) // nolint:PERF030 - fresh; the block runs once
            attn_dev_stamps(t, m.config.n_layers, pexp_staged)
            device_off <- two_window_logits(m, w1, w2_off) // nolint:PERF030 - fresh; runs once
            set_metal_prefill_held("attn_dev", was_held)
        }
        attn_dev_bar(t, m, staged, device, device_off)
        restore_crowns(crowns)
        metal_prefill_shutdown()
    }
}

struct private MirrorWins {
    w1 : array<int64>
    w2 : array<int64>
    w2_off : array<int64>
    whole : array<int64>
    step_tok : int64
}

def private mirror_wins(m : Model) : MirrorWins {
    var mw : MirrorWins
    var inscope longp <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
    let w1e = int64(ATTN_DEV_W1)
    let w2e = int64(ATTN_DEV_W1 + MIRROR_STEP_W)
    mw.w1 <- slice_ids(longp, 0l, w1e)
    mw.w2 <- slice_ids(longp, w1e, w2e)
    mw.w2_off <- slice_ids(longp, w1e + 1l, w2e + 1l)
    mw.whole <- slice_ids(longp, 0l, w2e)
    mw.step_tok = longp[w2e]
    return <- mw
}

struct private MirrorRows {
    panels : array<float>
    mirror : array<float>
    mag : float
}

//! the panels form against the mirror-fed form over the two windows
[unused_argument(t, m, mw)]
def private mirror_forms(t : T?; m : Model;
                         mw : MirrorWins) : MirrorRows {
    var r : MirrorRows
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let n_attn = m.config.n_layers
        let slab_reach = get_metal_attn_slab_reach_probe_()
        set_metal_attn_slab_reach_probe_(0ul)   //! the whole-window legs under the slab's own reach
        set_metal_prefill_mirror(false)
        kernel_census_reset_metal()
        let served0 = metal_prefill_mirror_served()
        r.panels <- two_window_logits(m, mw.w1, mw.w2)
        t |> equal(metal_prefill_mirror_served() - served0, 0l,
            "the knob off serves no window on the mirror-fed form")
        let half_panels = metal_kernel_count("metal_half_rows")
        t |> equal(metal_kernel_count("MetalAttnQKDev_metal_attn_qk_dev"), n_attn,
            "the device pair serves the 300-row window alone - 20 rows sit under its floor")
        set_metal_prefill_mirror(true)
        kernel_census_reset_metal()
        r.mirror <- two_window_logits(m, mw.w1, mw.w2)
        t |> equal(metal_prefill_mirror_served() - served0, 2l,
                   "both windows ran the mirror-fed form")
        t |> equal(metal_kernel_count("metal_half_rows") - half_panels, n_attn * 2l, ("the " +
            "continuation's K and V rows convert too: a layer's two passes more than the panels " +
            "form's"))
        t |> equal(metal_kernel_count("MetalAttnQKDev_metal_attn_qk_dev"), n_attn * 2l,
            "the device pair serves the 20-row continuation too")
        t |> equal(metal_kernel_count("MetalAttnQKDev128_metal_attn_qk_dev"), 0l,
            "the whole 300-row window takes no 128-row stamp (the pieced legs' control)")
        var inscope mirror_off <- two_window_logits(m, mw.w1, mw.w2_off)
        r.mag = logits_maxabs(r.panels)
        let diff = logits_maxdiff(r.mirror, r.panels)
        let diff_off = logits_maxdiff(mirror_off, r.panels)
        to_log(LOG_INFO,
               ("mirror: mirror-fed vs panels, last-row logits max difference {diff} of " +
            "max {r.mag} (bar {ATTN_DEV_BAR * r.mag}); one token off {diff_off}\n"))
        t |> success(diff <= ATTN_DEV_BAR * r.mag, ("the mirror-fed form's logits sit within the " +
            "bar of the panels form's ({diff} of {r.mag})"))
        t |> success(diff_off > ATTN_DEV_BAR * r.mag,
            "the prompt one token off lands outside the bar ({diff_off})")
        set_metal_attn_slab_reach_probe_(slab_reach)
    }
    return <- r
}

//! the window taken in score-slab pieces against the whole window
def private mirror_pieced(t : T?; m : Model; mw : MirrorWins;
                          r : MirrorRows) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let n_attn = m.config.n_layers
        let slab_reach = get_metal_attn_slab_reach_probe_()
        //! a 300-row slab over 320 keys: 256 a piece, whole tiles
        set_metal_attn_slab_reach_probe_(uint64(m.config.n_heads * 320l * 300l))
        kernel_census_reset_metal()
        var inscope pieced <- two_window_logits(m, mw.w1, mw.w2)
        t |> equal(metal_kernel_count("MetalAttnQKDev128_metal_attn_qk_dev"), n_attn,
            "the 300-row window's first piece is 256 rows, on the 128-row stamp")
        t |> equal(metal_kernel_count("MetalAttnQKDev_metal_attn_qk_dev"), n_attn * 2l,
            "its 44 rows left and the 20-row window take the 32-row stamp")
        let diff_pieced = logits_maxdiff(pieced, r.mirror)
        var inscope pieced_off <- two_window_logits(m, mw.w1, mw.w2_off)
        let diff_pieced_off = logits_maxdiff(pieced_off, r.mirror)
        let prior_mirror = get_metal_prefill_mirror()
        set_metal_prefill_mirror(false)
        kernel_census_reset_metal()
        var inscope pieced_panels <- two_window_logits(m, mw.w1, mw.w2)
        t |> equal(metal_kernel_count("MetalAttnQKDev128_metal_attn_qk_dev"), n_attn,
            "the panels form's 300-row window goes in the same pieces")
        let diff_pieced_panels = logits_maxdiff(pieced_panels, r.panels)
        set_metal_attn_slab_reach_probe_(slab_reach)
        set_metal_prefill_mirror(prior_mirror)
        var pick_pieced, pick_whole = 0l
        with_dasllama_jobque() {
            pick_pieced = argmax_of(pieced)
            pick_whole = argmax_of(r.mirror)
        }
        to_log(LOG_INFO, ("pieced window vs whole, last-row logits max difference: mirror-fed " +
            "{diff_pieced}, panels {diff_pieced_panels}, one token off {diff_pieced_off}; next " +
            "token '{eyeball_text(m, [pick_pieced])}' vs '{eyeball_text(m, [pick_whole])}'\n"))
        t |> equal(diff_pieced, 0.0,
                   "the mirror-fed window in pieces lands the whole window's logits to the bit")
        t |> equal(diff_pieced_panels, 0.0, "and the panels form's")
        t |> success(diff_pieced_off > ATTN_DEV_BAR * r.mag, ("the pieced window one token off " +
            "lands outside the bar ({diff_pieced_off}): the pieces carry the window"))
    }
}

//! the first decode step after each form: the uploads it takes
[unused_argument(t, m, mw)]
def private mirror_steps(t : T?; m : Model;
                         mw : MirrorWins) : array<float> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        select_decode_override("metal")
        set_metal_prefill_mirror(false)
        let up0 = metal_mirror_uploaded_rows()
        var step_panels <- two_window_logits(m, mw.w1, mw.w2, mw.step_tok)
        let up_panels = metal_mirror_uploaded_rows() - up0
        set_metal_prefill_mirror(true)
        var inscope step_mirror <- two_window_logits(m, mw.w1, mw.w2, mw.step_tok)
        let up_mirror = metal_mirror_uploaded_rows() - up0 - up_panels
        t |> equal(up_panels, int64(ATTN_DEV_W1 + MIRROR_STEP_W),
            "after a panels prefill the first decode step uploads every row")
        t |> equal(up_mirror, 0l, "after a mirror-fed prefill it uploads none")
        let smag = logits_maxabs(step_panels)
        let sdiff = logits_maxdiff(step_mirror, step_panels)
        var pick_mirror = 0l
        var pick_panels = 0l
        with_dasllama_jobque() {
            pick_mirror = argmax_of(step_mirror)
            pick_panels = argmax_of(step_panels)
        }
        t |> success(sdiff <= ATTN_DEV_BAR * smag, ("the step's logits sit within the bar across " +
            "the two forms ({sdiff} of {smag}; argmax {pick_mirror} / {pick_panels})"))
        return <- step_panels
    } else {
        return <- array<float>()
    }
}

//! the panel form feeding the mirror, no attn_dev crown
[unused_argument(t, m, mw, step_panels)]
def private mirror_panel_feed(t : T?; m : Model; mw : MirrorWins;
                              step_panels : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        pin_metal_tensor_crowns(CROWN_PANEL)
        metal_prefill_shutdown()
        let pserved0 = metal_prefill_mirror_panel_served()
        let mserved0 = metal_prefill_mirror_served()
        let up_pm0 = metal_mirror_uploaded_rows()
        var inscope step_pm <- two_window_logits(m, mw.w1, mw.w2, mw.step_tok)
        t |> equal(metal_prefill_mirror_panel_served() - pserved0, 2l,
            "both windows ran the panel form into the mirror")
        t |> equal(metal_prefill_mirror_served() - mserved0, 0l, "and neither the mirror-fed form")
        t |> equal(metal_mirror_uploaded_rows() - up_pm0, 0l,
            "after a panel-form prefill into the mirror the first decode step uploads none")
        let smag = logits_maxabs(step_panels)
        let pdiff = logits_maxdiff(step_pm, step_panels)
        t |> success(pdiff <= ATTN_DEV_BAR * smag, ("the step's logits after the panel form into " +
            "the mirror sit within the bar of the panels form's ({pdiff} of {smag})"))
    }
}

struct private AdoptRun {
    logits : array<float>
    matched : int64
    adopted : int64     //!< rows the taker's mirror took from the donor's
    uploaded : int64    //!< rows the taker's mirror took from the host cache
}

//! a paged donor evals `opening` and returns its pages - donated to a
//! prefix cache where `donate` - then a paged taker attaches what the
//! cache holds of `whole` and evals the rest
def private adopt_run(trp : Model; opening, whole : array<int64>;
                      donate : bool) : AdoptRun {   // nolint:LINT012,LINT019 - das_metal half
    var res : AdoptRun
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_dasllama_jobque() {
            var pool <- create_kv_pool_(trp.config, 64l, KVDtype.f16, KVDtype.f16)
            var cache <- create_prefix_cache_()
            var donor <- make_run_state_paged(trp.config, pool)
            eval_(trp, donor, opening)
            if (donate) {
                prefix_insert_(cache, pool, donor, opening)
            }
            release_kv_pages_(donor)
            delete donor
            var taker <- make_run_state_paged(trp.config, pool)
            res.matched = prefix_attach_(cache, pool, taker, whole)
            let adopted0 = metal_mirror_adopted_rows()
            let uploaded0 = metal_mirror_uploaded_rows()
            var rest <- slice_ids(whole, res.matched, long_length(whole))
            eval_(trp, taker, rest)
            delete rest
            res.adopted = metal_mirror_adopted_rows() - adopted0
            res.uploaded = metal_mirror_uploaded_rows() - uploaded0
            res.logits := taker.logits
            release_kv_pages_(taker)
            delete taker
            prefix_release_(cache, pool)
            delete cache
            delete pool
        }
    }
    return <- res
}

//! the prefix cache's pages adopted by the taker's mirror
def private mirror_adopt(t : T?; m : Model;
                         mw : MirrorWins) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_adopt = get_metal_mirror_adopt()
        pin_metal_tensor_crowns(CROWN_MIRROR)
        metal_prefill_shutdown()
        set_metal_mirror_adopt(true)
        var inscope took <- adopt_run(m, mw.w1, mw.whole, true)
        t |> equal(took.matched, 300l,
                   ("the cache hands the taker the opening's four whole pages " +
            "and a copy of the fifth for its 44 rows"))
        t |> equal(took.adopted, 256l,
                   "the taker's mirror takes the whole pages' rows from the donor's")
        t |> equal(took.uploaded, 44l, "and uploads the copied page's rows alone")
        set_metal_mirror_adopt(false)
        var inscope copied <- adopt_run(m, mw.w1, mw.whole, true)
        set_metal_mirror_adopt(prev_adopt)
        t |> equal(copied.adopted, 0l, "the knob off adopts nothing")
        t |> equal(copied.uploaded, copied.matched, "and uploads the attached rows")
        t |> equal(logits_maxdiff(took.logits, copied.logits), 0.0,
            "the adopted rows are the uploaded rows: the logits agree to the bit")
        var inscope fresh <- adopt_run(m, mw.w1, mw.whole, false)
        t |> equal(fresh.matched, 0l, "nothing donated, nothing attached")
        t |> equal(fresh.adopted, 0l, "a reused group id under a new stamp adopts nothing")
    }
}

//! the mirror arm: mirror-fed against panels, pieced windows, the
//! first decode step's uploads, the panel form into the mirror and
//! adopted prefix pages; the drivers rebuilt under the arm's crowns
//! and again under the form's after
def private mirror_metal(t : T?; m : Model) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "mirror"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope mw <- mirror_wins(m)
        let crowns = crown_seat()
        pin_metal_tensor_crowns(CROWN_MIRROR)
        metal_prefill_shutdown()
        let prev_mirror = get_metal_prefill_mirror()
        with_metal_rails(MetalRails(decode = false)) {
            var inscope rows <- mirror_forms(t, m, mw)
            mirror_pieced(t, m, mw, rows)
            var inscope step_panels <- mirror_steps(t, m, mw)
            mirror_panel_feed(t, m, mw, step_panels)
            mirror_adopt(t, m, mw)
        }
        set_metal_prefill_mirror(prev_mirror)
        restore_crowns(crowns)
        metal_prefill_shutdown()
        metal_decode_shutdown()
    }
}

struct private CodecFeed {
    fed : int64
    up : int64
    dec : int64
    stores : int64
    fed_off : int64
    up_off : int64
}

//! one codec's mirror feed, knob on then off, on the seated rails;
//! both legs' tokens kept
[unused_argument(m, cell, w1, w2)]
def private codec_feed(m : Model; var cell : PrefillCell;
                       w1, w2 : array<int64>) : CodecFeed {
    var f : CodecFeed
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let kdt = cell.kdt
        set_metal_prefill_mirror(true)
        kernel_census_reset_metal()
        let fed0 = metal_prefill_mirror_panel_served()
        let up0 = metal_mirror_uploaded_rows()
        let dec0 = metal_decode_stats().decodes
        var inscope gpu <- codec_two_window_greedy(m, w1, w2, kdt, CODEC_FEED_STEPS)
        f.fed = metal_prefill_mirror_panel_served() - fed0
        f.up = metal_mirror_uploaded_rows() - up0
        f.dec = metal_decode_stats().decodes - dec0
        f.stores = metal_kernel_count("metal_kv_rows_q8") + metal_kernel_count("metal_kv_rows_tq4")
        set_metal_prefill_mirror(false)
        let fed1 = metal_prefill_mirror_panel_served()
        let up1 = metal_mirror_uploaded_rows()
        var inscope gpu_off <- codec_two_window_greedy(m, w1, w2, kdt, CODEC_FEED_STEPS)
        f.fed_off = metal_prefill_mirror_panel_served() - fed1
        f.up_off = metal_mirror_uploaded_rows() - up1
        ids_leg(cell, "{kdt}: the device's greedy tokens are the CPU chain's on the same codec",
            "mirror-codec {kdt}", gpu)
        ids_leg(cell, "{kdt} control: the knob off reads the same tokens", "", gpu_off)
    }
    return f
}

//! the mirror-codec arm: a q8_0, tq4 and f32 session's windows feed
//! the mirror through the codec's store, the steps after them upload
//! no row; the knob off the control
[unused_argument(m, kept)]
def private mirror_codec_metal(t : T?; m : Model;
                               var kept : PrefillKept) {
    return if (!arm_on(t, "mirror-codec"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope longp <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
        let w1e = int64(ATTN_DEV_W1)
        var inscope chunks <- cut_chunks(longp, [w1e, w1e + 20l])
        let prev_mirror = get_metal_prefill_mirror()
        kept.cells |> reserve(length(kept.cells) + 3)
        with_metal_rails(MetalRails()) {
            for (kdt in [KVDtype.q8_0, KVDtype.tq4, KVDtype.f32]) {
                var cell <- new_cell(m, PrefillReplay.codec_windows, kdt, CODEC_FEED_STEPS)
                cell.chunks := chunks
                let f = codec_feed(m, cell, chunks[0], chunks[1])
                t |> equal(f.fed, 2l,
                    "{kdt}: both windows fed the mirror through the codec's store")
                t |> equal(f.up, 0l, "{kdt}: the decode steps after the windows upload no row")
                t |> equal(f.dec, int64(CODEC_FEED_STEPS), "{kdt}: every step a device decode")
                if (kdt == KVDtype.f32) {
                    t |> equal(f.stores, 0l, "{kdt}: the rows copy across, no quantizing store")
                } else {
                    t |> success(f.stores > 0l,
                        "{kdt}: the quantizing store stamp ran ({f.stores} dispatches)")
                }
                t |> equal(f.fed_off, 0l,
                    "{kdt} control: the knob off feeds no window into the mirror")
                t |> equal(f.up_off, int64(ATTN_DEV_W1 + 20),
                    "{kdt} control: the first decode step uploads every prompt row")
                kept.cells |> emplace(cell)
            }
        }
        set_metal_prefill_mirror(prev_mirror)
    }
}

//! the mirror-cap arm's 1B leg: under a 64 MiB cap an f32 session of
//! 1100-odd rows prefills and steps on the device, its slice resident
//! past the cap; the decode driver rebuilt under the cap and again
//! under the form's after
def private mirror_cap_metal(t : T?; m : Model;
                             var kept : PrefillKept) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "mirror-cap"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev_cap = get_metal_decode_mirror_cap_mb()
        let row_bytes = kv_row_bytes(KVDtype.f32, kv_row_total(m)) * 2l
        var inscope midp <- encode(m, counting_prompt(420), true, false)
        let np = long_length(midp)
        var cell <- new_cell(m, PrefillReplay.codec_windows, KVDtype.f32, CODEC_FEED_STEPS)
        cell.chunks <- cut_chunks(midp, [np - 20l, np])
        metal_decode_shutdown()
        metal_prefill_shutdown()
        set_metal_decode_mirror_cap_mb(64)
        with_metal_rails(MetalRails()) {
            let dec0 = metal_decode_stats().decodes
            var inscope gpu <- codec_two_window_greedy(m, cell.chunks[0], cell.chunks[1],
                KVDtype.f32, CODEC_FEED_STEPS)
            let mir = metal_decode_mirror_stats()
            t |> success(np * row_bytes > 64l * 1024l * 1024l,
                "the session's rows pass the 64 MiB cap ({np} rows of {row_bytes} bytes)")
            t |> success(mir.bytes > 64ul * 1024ul * 1024ul,
                "the slice stands resident past the cap ({mir.bytes} bytes)")
            t |> equal(metal_decode_stats().decodes - dec0, int64(CODEC_FEED_STEPS),
                "every step past the cap is the device's")
            ids_leg(cell, "past the cap the device's greedy tokens are the CPU chain's",
                "mirror-cap 64 MiB", gpu)
            set_metal_decode_mirror_cap_mb(prev_cap)
            metal_decode_shutdown()
            metal_prefill_shutdown()
        }
        kept.cells |> emplace(cell)
    }
}

//! the long prompt in window-sized calls, then `LONG_STEPS` greedy
//! steps, on one f16 session
def private long_run(m : Model; longp : array<int64>) : array<int64> {
    var ids : array<int64>
    let np = long_length(longp)
    with_dasllama_jobque() {
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        var at = 0l
        while (at < np) {
            let n = min(LONG_WINDOW, np - at)
            var chunk <- slice_ids(longp, at, at + n)
            eval_(m, s, chunk)
            delete chunk
            at += n
        }
        ids <- greedy_eval_steps(m, s, LONG_STEPS) // nolint:PERF030 - fresh; the block runs once
        delete s
    }
    return <- ids
}

//! the mirror-cap arm's 3B leg: an f16 session past 4096 MiB of K/V
//! prefills and steps on the device and counts on; the drivers
//! rebuilt under the arm's crowns and again under the form's after
def private mirror_long_metal(t : T?; m : Model) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "mirror-cap"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_metal_rails(MetalRails()) {
            let crowns = crown_seat()
            pin_metal_tensor_crowns(CROWN_MIRROR)
            metal_prefill_shutdown()
            let lrow = kv_row_bytes(KVDtype.f16, kv_row_total(m)) * 2l
            let last = LONG_COUNT_TO
            var inscope longp <- encode(m, counting_prompt(last), true, false)
            let np = long_length(longp)
            t |> success(np * lrow > 4096l * 1024l * 1024l, ("the prompt's K/V passes 4096 MiB " +
                "({np} rows of {lrow} bytes, {np * lrow / (1024l * 1024l)} MiB)"))
            let pf0 = metal_prefill_stats().prefills
            let ldec0 = metal_decode_stats().decodes
            var inscope ids <- long_run(m, longp)
            let lmir = metal_decode_mirror_stats()
            let text = decode_(m, ids)
            to_log(LOG_INFO, ("mirror-cap long: {np} rows, mirror " +
                "{int64(lmir.bytes / (1024ul * 1024ul))} MiB, continuation '{text}'\n"))
            t |> equal(metal_prefill_stats().prefills - pf0, (np + LONG_WINDOW - 1l) / LONG_WINDOW,
                "every window of the long prompt prefilled on the device")
            t |> equal(metal_decode_stats().decodes - ldec0, int64(LONG_STEPS),
                "every step over the long context is the device's")
            t |> success(lmir.bytes > 4096ul * 1024ul * 1024ul,
                "the mirror holds the session's slice past 4096 MiB ({lmir.bytes} bytes)")
            t |> success(starts_with(text, " {last + 1},"),
                "the continuation counts on from {last} ('{text}')")
            restore_crowns(crowns)
        }
        metal_prefill_shutdown()
        metal_decode_shutdown()
    }
}

//! the segs arm's tails as one window against their own windows
def private segs_tails(t : T?; m : Model; form : string; a1, a2, b1, b2 : array<int64>;
                       ra, rb : TwoWindowRun) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let bar_a = ATTN_DEV_BAR * logits_maxabs(ra.logits)
        let bar_b = ATTN_DEV_BAR * logits_maxabs(rb.logits)
        var sa = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        var sb = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, sa, a1)
        eval_(m, sb, b1)
        segments_window(t, m, sa, sb, a2, b2, "{form}, the tails")
        let da = logits_maxdiff(sa.logits, ra.logits)
        let db = logits_maxdiff(sb.logits, rb.logits)
        to_log(LOG_INFO, ("segs {form}: the tails as one window vs two, last-row logits max " +
            "difference {da} (bar {bar_a}) and {db} (bar {bar_b})\n"))
        t |> success(da <= bar_a,
                     ("{form}: the first segment's logits sit within the bar of its own " +
            "window's ({da} of {bar_a})"))
        t |> success(db <= bar_b,
                     ("{form}: the second segment's logits sit within the bar of its own " +
            "window's ({db} of {bar_b})"))
        let dec0 = metal_decode_stats().decodes
        var inscope ga2 <- greedy_from(m, sa, long_length(a1) + long_length(a2), 8l)
        var inscope gb2 <- greedy_from(m, sb, long_length(b1) + long_length(b2), 8l)
        t |> equal(metal_decode_stats().decodes - dec0, 16l,
                   "{form}: the sixteen greedy steps were the device's")
        log_gen_texts(m, "segs {form} first", ra.ids, ga2)
        log_gen_texts(m, "segs {form} second", rb.ids, gb2)
        t |> success(ids_equal(ra.ids, ga2),
                     "{form}: eight greedy steps off the first segment are its own window's")
        t |> success(ids_equal(rb.ids, gb2),
                     "{form}: eight greedy steps off the second segment are its own window's")
        delete sa
        delete sb
    }
}

//! the segs arm's openings as one window from position zero
def private segs_openings(t : T?; m : Model; form : string; a1, a2, b1, b2 : array<int64>;
                          ra, rb : TwoWindowRun) {
    let bar_a = ATTN_DEV_BAR * logits_maxabs(ra.logits)
    let bar_b = ATTN_DEV_BAR * logits_maxabs(rb.logits)
    var fa = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var fb = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    segments_window(t, m, fa, fb, a1, b1, "{form}, the openings")
    eval_(m, fa, a2)
    eval_(m, fb, b2)
    let fda = logits_maxdiff(fa.logits, ra.logits)
    let fdb = logits_maxdiff(fb.logits, rb.logits)
    t |> success(fda <= bar_a, ("{form}: the first opening as a segment leaves its tail's logits " +
        "within the bar ({fda} of {bar_a})"))
    t |> success(fdb <= bar_b,
                 ("{form}: the second opening as a segment leaves its tail's logits " +
        "within the bar ({fdb} of {bar_b})"))
    var inscope fga <- greedy_from(m, fa, long_length(a1) + long_length(a2), 8l)
    var inscope fgb <- greedy_from(m, fb, long_length(b1) + long_length(b2), 8l)
    t |> success(ids_equal(ra.ids, fga),
                 ("{form}: eight greedy steps after the segmented opening " +
        "are the first session's own"))
    t |> success(ids_equal(rb.ids, fgb),
                 ("{form}: eight greedy steps after the segmented opening " +
        "are the second session's own"))
    delete fa
    delete fb
}

//! the segs arm on the format `fmt`: two sessions' chunks as ONE
//! window against two, on the mirror-fed and the panel form; the
//! drivers rebuilt under each form's crowns and the form's after
def private segs_metal(t : T?; m : Model;
                       fmt : string) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "segs"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let crowns = crown_seat()
        with_metal_rails(MetalRails()) {
            var inscope longp <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
            //! 300 + 200 and 150 + 120: rows off every lattice
            var inscope a1 <- slice_ids(longp, 0l, 300l)
            var inscope a2 <- slice_ids(longp, 300l, 500l)
            var inscope b1 <- slice_ids(longp, 40l, 190l)
            var inscope b2 <- slice_ids(longp, 190l, 310l)
            for (crown in [CROWN_MIRROR, CROWN_PANEL]) {
                let form = "{fmt} {crown == CROWN_MIRROR ? "mirror-fed" : "panel"}"
                pin_metal_tensor_crowns(crown)
                metal_prefill_shutdown()
                with_dasllama_jobque() {
                    var inscope ra <- two_window_greedy(m, a1, a2, 8l)
                    var inscope rb <- two_window_greedy(m, b1, b2, 8l)
                    segs_tails(t, m, form, a1, a2, b1, b2, ra, rb)
                    segs_openings(t, m, form, a1, a2, b1, b2, ra, rb)
                }
            }
        }
        restore_crowns(crowns)
        metal_prefill_shutdown()
        metal_decode_shutdown()
    }
}

//! the 1B Q8 file's mirror and attention arms on the 2048 context
def private q8_mirror_metal(t : T?; m : Model; var kept : PrefillKept) {
    mirror_codec_metal(t, m, kept)
    attn_dev_metal(t, m)
    mirror_metal(t, m)
    mirror_cap_metal(t, m, kept)
}

//! the 1B Q8 file's arms; a declined base prefill skips the rest
def private q8_metal(t : T?; var m : Model; var kept : PrefillKept; prompt : array<int64>) {
    var go = true
    with_ctx_cut(m, HYBRID_CTX) {
        if (arm_on(t, "base")) {
            go = base_metal(t, m, kept, prompt)
        }
        if (go) {
            q8_hybrid_metal(t, m, kept, prompt)
        }
    }
    return if (!go)
    with_ctx_cut(m, MIRROR_CTX) {
        q8_mirror_metal(t, m, kept)
    }
}

//! the narrowed last-layer tail against the full panel's over the
//! first 48 ids of `prompt`: the logits within `bar`, a changed last
//! id outside it, the argmax held on a whole file, row 0 visibly
//! narrowed, `keep_hidden` restoring the full plane bit for bit
[cold_path]
def private lastrow_ab(t : T?; trsg : Model; prompt : array<int64>; tag : string; bar : float;
                       carrier : Carrier) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let nlr = 48l
        var tlr <- slice_ids(prompt, 0l, nlr)
        let dim_s = trsg.config.dim
        let served0 = metal_prefill_stats().prefills
        let prior_lastrow = get_metal_prefill_lastrow()
        set_metal_prefill_lastrow(false)
        var s_off <- make_run_state(trsg.config)
        forward_prefill(trsg, s_off, tlr, nlr, 0l)
        set_metal_prefill_lastrow(true)
        var s_on <- make_run_state(trsg.config)
        forward_prefill(trsg, s_on, tlr, nlr, 0l)
        var s_keep <- make_run_state(trsg.config)
        s_keep.keep_hidden = true
        forward_prefill(trsg, s_keep, tlr, nlr, 0l)
        var tctl := tlr
        tctl[nlr - 1l] = tlr[nlr - 2l]
        set_metal_prefill_lastrow(false)
        var s_ctl <- make_run_state(trsg.config)
        forward_prefill(trsg, s_ctl, tctl, nlr, 0l)
        set_metal_prefill_lastrow(prior_lastrow)
        if (metal_prefill_stats().prefills - served0 == 4l) {
            let lmax = logits_maxdiff(s_on.logits, s_off.logits)
            let lctl = logits_maxdiff(s_on.logits, s_ctl.logits)
            let amax_on = argmax_of(s_on.logits, trsg.config.vocab_size)
            let amax_off = argmax_of(s_off.logits, trsg.config.vocab_size)
            to_log(LOG_INFO,
                   ("{tag} lastrow: narrowed-vs-full logits maxdiff {lmax}, against the " +
                "window with its last id changed {lctl}\n"))
            t |> success(lmax <= bar, ("{tag} lastrow: narrowed logits inside the form-change " +
                "envelope ({lmax}, bar {bar})"))
            t |> success(lctl > bar, ("{tag} lastrow: the window with its last id changed lands " +
                "outside the bar ({lctl}) - the control"))
            var pick_off <- [amax_off]
            var pick_on <- [amax_on]
            to_log(LOG_INFO, ("{tag} lastrow: full pick {amax_off} @{s_off.logits[amax_off]} = " +
                "{eyeball_text(trsg, pick_off)}\n"))
            to_log(LOG_INFO, ("{tag} lastrow: narrowed pick {amax_on} @{s_on.logits[amax_on]} = " +
                "{eyeball_text(trsg, pick_on)}\n"))
            delete pick_off
            delete pick_on
            if (carrier == Carrier.whole_file) {
                t |> equal(amax_on, amax_off, "{tag} lastrow: argmax unchanged")
            }
            var row0 = 0.0
            for (i in range64(dim_s)) {
                row0 = max(row0, nan_wide_diff(s_on.x_b[i], s_off.x_b[i]))
            }
            //! a NaN on either side reads as infinite
            let plane = span_maxdiff(s_keep.x_b, s_off.x_b, 0, 0, int(nlr * dim_s)).diff
            t |> success(row0 > 1e-4,
                         "{tag} lastrow: row 0 visibly narrowed ({row0}) - the arm engaged")
            t |> equal(plane, 0.0, "{tag} lastrow: keep_hidden restores the full plane bit-exactly")
        } else {
            t |> failure(("{tag} lastrow: the metal prefill declined a leg " +
                "({metal_prefill_stats().prefills - served0} of 4 served)"))
        }
        delete s_off
        delete s_on
        delete s_keep
        delete s_ctl
        delete tctl
        delete tlr
    }
}

//! the lastrow-g arm on a gemma sandwich file
def private lastrow_metal(t : T?; m : Model;
                          p : PrefillRow) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "lastrow-g"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (p.blocks > 0l) {
            cut_within(t, "{p.file}: ", m, p.blocks)
        }
        var inscope prompt <- encode(m, counting_prompt(1, 30), true, false)
        with_dasllama_jobque() {
            with_metal_rails(t, p.file, MetalRails(decode = false)) {
                lastrow_ab(t, m, prompt, p.file, p.lastrow_bar,
                    p.whole ? Carrier.whole_file : Carrier.layer_cut)
            }
        }
    }
}

//! the mm-tail arm's SmolLM2 leg: dim 576 fails the b4 stripe's
//! % 128, so r >= 2 refuses the tail while r == 1 rides the gemv;
//! then the narrowed last row against the full panel
def private smol_metal(t : T?; m : Model) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "mm-tail"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope sprompt <- encode(m, counting_prompt(COUNT_TO), true, false)
        let prior_tail = get_metal_prefill_mm_tail()
        let prior_lastrow = get_metal_prefill_lastrow()
        let prior_split = get_metal_prefill_split_k()
        set_metal_prefill_mm_tail(true)
        with_metal_rails(t, "mm-tail smol", MetalRails(decode = false, min_npos = 0l)) {
            set_metal_prefill_lastrow(false)   //! every layer's FFN rides the mm path
            var ssites : int64[4]
            var sserved_all = true
            for (li in range(4)) {
                let nres = li % 2 == 0 ? 65l : 66l
                //! pair 1 on the whole-K tile, pair 2 under the split-K knee
                set_metal_prefill_split_k(li >= 2)
                var inscope ssl <- slice_ids(sprompt, 0l, nres)
                let sserved0 = metal_prefill_stats().prefills
                let stail0 = metal_prefill_tail_stats()
                with_dasllama_jobque() {
                    var s = make_run_state(m.config)
                    eval_(m, s, ssl)
                    delete s
                }
                sserved_all &&= metal_prefill_stats().prefills - sserved0 > 0l
                ssites[li] = metal_prefill_tail_stats().sites - stail0.sites
            }
            if (sserved_all) {
                smol_asserts(t, ssites)
            } else {
                feint("metal prefill declined on smol; the alignment-guard leg skipped\n")
            }
            set_metal_prefill_lastrow(prior_lastrow)
            set_metal_prefill_split_k(prior_split)
            with_dasllama_jobque() {
                lastrow_ab(t, m, sprompt, "smol", 2e-2, Carrier.whole_file)   //! measured ~7e-3
            }
        }
        set_metal_prefill_mm_tail(prior_tail)
    }
}

//! the SmolLM2 leg's per-site guard: at r == 1 every q8 site peels,
//! at r == 2 only the hidden-kdim w2 site
def private smol_asserts(t : T?; ssites : int64[4]) {
    t |> success(ssites[0] > 0l, "smol npos=65: the gemv leg engaged ({ssites[0]} sites)")
    t |> success(ssites[1] < ssites[0],
        "smol npos=66: the alignment guard refused the 576-kdim sites ({ssites[1]} of {ssites[0]})")
    t |> equal(ssites[1] * 7l, ssites[0],
               "smol npos=66: exactly the one hidden-kdim site per layer peeled")
    t |> equal(ssites[2], ssites[0],
        "smol npos=65 under the split-K knee: the one-row remainder still peels at every site")
    t |> equal(ssites[3], 0l,
        ("smol npos=66 under the split-K knee: a two-row remainder rides the split tile, no " +
            "site peels"))
}

//! the bf16-split arm: the per-layer embedding's bf16 projection
//! splits its K on a 24-row window and walks it whole with split-K
//! off; the driver rebuilt under the knee and again after
def private bf16_metal(t : T?; m : Model) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "bf16-split"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!metal_tensor_crowned("mulmm_bf16")) {
            t |> skip(("the box's profile crowns no bf16 tensor tile - the projection has no " +
                "split stamp here"))
            return
        }
        let prior_knee = metal_split_k_tgs()
        let prior_split = get_metal_prefill_split_k()
        var inscope prompt <- encode(m, counting_prompt(1, 30), true, false)
        var inscope w <- slice_ids(prompt, 0l, BF16_SPLIT_ROWS)
        t |> equal(long_length(w), BF16_SPLIT_ROWS, "the window holds {BF16_SPLIT_ROWS} rows")
        t |> success(m.ple_model_bf16 && pf_ple_proj_form(m) == PfPleProjForm.bf16_tile,
            "the projection is the file's bf16 rows on the tile GEMM")
        var inscope split, whole : array<float>
        var split_n, whole_n, served = 0l
        with_metal_rails(t, "bf16-split", MetalRails(decode = false)) {
            set_metal_split_k_tgs(BF16_SPLIT_KNEE)
            set_metal_prefill_split_k(true)
            metal_prefill_shutdown()   //! the driver caches the knee at init
            let served0 = metal_prefill_stats().prefills
            let b0 = bf16_split_dispatches()
            split <- window_last_logits(m, w) // nolint:PERF030 - fresh; the block runs once
            split_n = bf16_split_dispatches() - b0
            set_metal_prefill_split_k(false)
            let b1 = bf16_split_dispatches()
            whole <- window_last_logits(m, w) // nolint:PERF030 - fresh; the block runs once
            whole_n = bf16_split_dispatches() - b1
            served = metal_prefill_stats().prefills - served0
            set_metal_prefill_split_k(prior_split)
            set_metal_split_k_tgs(prior_knee)
        }
        metal_prefill_shutdown()
        if (served != 2l) {
            t |> skip(("the metal prefill declined a window ({served} of 2 served) - no Metal " +
                "device?"))
        } else {
            t |> success(split_n > 0l, ("the {BF16_SPLIT_ROWS}-row window's projection rode the " +
                "bf16 split stamps ({split_n} dispatches)"))
            t |> equal(whole_n, 0l, "the control - split-K off, the projection walked its whole K")
            to_log(LOG_INFO, ("bf16-split: split logits peak {logits_maxabs(split)}, whole-K gap " +
                "{logits_maxdiff(split, whole)}\n"))
        }
    }
}

struct private ShortLegs {
    served : int64
    served_tiles : int64
    engaged : int64
    split_sites : int64
    sk16 : int64
    held_split : int64
    held_sk16 : int64
    off_engaged : int64
    held_gap : float
    short16 : bool
}

def private short_engaged(route : bool) : int64 {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return route ? metal_prefill_moe_gemv_stats() : metal_prefill_tail_stats().kq_sites
    } else {
        return 0l
    }
}

//! the control for the 16-row stamps: the same windows with them held
def private short_held(m : Model; w1, w2 : array<int64>; var r : ShortLegs;
                       gpu : array<float>) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let held_split0 = metal_prefill_split_k_sites()
        let held_sk16_0 = split16_dispatches()
        if (r.short16) {
            let prior_held = set_metal_prefill_held("split_k16", true)
            var inscope gpu_held <- two_window_logits(m, w1, w2)
            set_metal_prefill_held("split_k16", prior_held)
            r.held_gap = logits_maxdiff(gpu_held, gpu)
        }
        r.held_split = metal_prefill_split_k_sites() - held_split0
        r.held_sk16 = split16_dispatches() - held_sk16_0
    }
}

//! one short window's device legs: the form, the control window, the
//! form off (the tiles)
[unused_argument(t, m, p, w1, w2, w2_ctrl, n, win)]
def private short_legs(t : T?; m : Model; p : PrefillRow; w1, w2, w2_ctrl : array<int64>;
                       n : int64;
                       var win : PrefillShortWin) : ShortLegs {
    var r : ShortLegs
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let route = p.short_form == ShortForm.moe_route
        with_metal_rails(t, "{p.short_tag}/{n}", MetalRails(decode = false)) {
            let served0 = metal_prefill_stats().prefills
            let engaged0 = short_engaged(route)
            let split0 = metal_prefill_split_k_sites()
            let prior_split = get_metal_prefill_split_k()
            set_metal_prefill_split_k(n <= 32l && prior_split)
            let sk16_0 = split16_dispatches()
            win.gpu <- two_window_logits(m, w1, w2)
            r.served = metal_prefill_stats().prefills - served0
            r.engaged = short_engaged(route) - engaged0
            r.split_sites = metal_prefill_split_k_sites() - split0
            r.sk16 = split16_dispatches() - sk16_0
            r.short16 = !route && n > 4l && n <= 16l && prior_split
            short_held(m, w1, w2, r, win.gpu)
            win.ctrl <- two_window_logits(m, w1, w2_ctrl)
            let prior_tail = get_metal_prefill_mm_tail()
            if (route) {
                set_metal_moe_gemv_max(0l)
            } else {
                set_metal_prefill_mm_tail(false)
            }
            let off0 = short_engaged(route)
            let served_tiles0 = metal_prefill_stats().prefills
            win.tiles <- two_window_logits(m, w1, w2)
            r.served_tiles = metal_prefill_stats().prefills - served_tiles0
            r.off_engaged = short_engaged(route) - off0
            if (!route) {
                set_metal_prefill_mm_tail(prior_tail)
            }
            set_metal_prefill_split_k(prior_split)
        }
    }
    return r
}

//! the K-quant peel's engage asserts on one window
def private peel_asserts(t : T?; tag : string; n : int64; r : ShortLegs; prior_split : bool) {
    if (n > 32l) {   //! past whole tiles, split-K off: the row forms at the offset
        t |> success(r.engaged > 0l,
            ("{tag}/{n}: the {n % 32l}-row remainder rode the row forms at its offset " +
                "({r.engaged} sites)"))
        t |> equal(r.split_sites, 0l,
                   "{tag}/{n}: with split-K pinned off no site took the split-K tile")
    } elif (n > 4l && prior_split) {   //! the split-K tile takes the sites under its knee
        t |> success(r.split_sites > 0l, ("{tag}/{n}: the split-K tile took the sites under its " +
            "knee ({r.split_sites}), the row forms the rest ({r.engaged})"))
        if (r.short16) {   //! one 32-row tile holds the window: the 16-row split stamps
            t |> success(r.sk16 > 0l,
                ("{tag}/{n}: the {n}-row window's split sites rode the 16-row split stamps " +
                    "({r.sk16} dispatches)"))
            t |> equal(r.held_sk16, 0l,
                       "{tag}/{n}: the control - with the 16-row stamps held none dispatched")
            t |> success(r.held_split > 0l,
                "{tag}/{n}: ... and the 32-row split stamps took the sites ({r.held_split})")
            to_log(LOG_INFO,
                   "{tag}/{n}: 16-row stamps held - logits {r.held_gap} off the 16-row run's\n")
        }
    } else {
        t |> success(r.engaged > 0l,
            ("{tag}/{n}: the short window's K-quant sites rode the row-batched GEMV " +
                "({r.engaged} sites)"))
    }
}

//! one short window of `n` rows after the 300-row one: its device
//! legs and engage asserts, the logits kept for the verdict
def private short_win_metal(t : T?; m : Model; p : PrefillRow; var sw : PrefillShort;
                            prompt : array<int64>;
                            w1n, n : int64) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let route = p.short_form == ShortForm.moe_route
        let tag = p.short_tag
        if (route) {
            set_metal_moe_gemv_max(n == 16l ? -1l : 24l)
            if (n == 16l) {
                t |> equal(16l, pf_moe_gemv_knee(), "{tag}/{n}: the shipped knee is 16 rows")
            }
        }
        let knee = route ? pf_moe_gemv_knee() : 34l
        return if (n > knee)
        var inscope w2 <- slice_ids(prompt, w1n, w1n + n)
        t |> equal(n, long_length(w2), "{tag}/{n}: the short window holds {n} rows")
        var inscope w2_ctrl := w2
        w2_ctrl[n - 1l] = prompt[w1n + n]
        let prior_split = get_metal_prefill_split_k()
        var win <- PrefillShortWin(n = n)
        let r = short_legs(t, m, p, sw.w1, w2, w2_ctrl, n, win)
        if (r.served == 0l) {
            t |> skip(("{tag}/{n}: metal prefill declined (no Metal device?); the " +
                "{sw.form_name}'s parity skipped"))
            delete win
            return
        }
        t |> equal(r.served, 2l, "{tag}/{n}: both windows served on the device")
        t |> equal(r.served_tiles, 2l,
                   "{tag}/{n}: both windows of the tiles leg served on the device")
        if (route) {
            t |> equal(r.engaged, 1l, "{tag}/{n}: the short window alone rode the gathered route")
        } else {
            peel_asserts(t, tag, n, r, prior_split)
        }
        t |> equal(r.off_engaged, 0l, "{tag}/{n}: with the {sw.form_name} off the tiles serve it")
        win.w2 := w2
        win.gpu_piece = piece(m, int64(max_element(win.gpu)))
        win.tiles_piece = piece(m, int64(max_element(win.tiles)))
        sw.wins |> emplace(win)
    }
}

//! the moe-gemv or kq-tail arm: a 300-row window on the tiles, then
//! short windows the form serves, each against the CPU control
[unused_argument(m, p, kept)]
def private short_window_metal(t : T?; m : Model; p : PrefillRow;
                               var kept : PrefillKept) {
    let route = p.short_form == ShortForm.moe_route
    return if (!arm_on(t, route ? "moe-gemv" : "kq-tail"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (p.blocks > 0l) {
            cut_within(t, "{p.short_tag}: ", m, p.blocks)
        }
        var inscope prompt <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
        let w1n = min(300l, long_length(prompt) - 40l)
        t |> equal(300l, w1n, "the first window is the 300-row one the tiles serve")
        kept.window.on = true
        kept.window.tag = p.short_tag
        kept.window.form_name = route ? "route" : "peel"
        kept.window.bar = p.short_bar
        kept.window.whole = p.whole
        kept.window.ctx = m.config.seq_len
        kept.window.w1 <- slice_ids(prompt, 0l, w1n)
        let prior_knee = metal_moe_gemv_max()
        let prior_tail = get_metal_prefill_mm_tail()
        set_metal_prefill_mm_tail(true)   //! the peel's rail decides its engage assert
        //! the route: 2, 7, 24 rows at a knee pinned to 24, 16 at
        //! the shipped knee; the peel's 34: two rows past whole tiles
        var inscope windows <- route ? [2l, 7l, 16l, 24l] : [2l, 4l, 5l, 8l, 34l]
        for (n in windows) {
            short_win_metal(t, m, p, kept.window, prompt, w1n, n)
        }
        if (route) {
            set_metal_moe_gemv_max(prior_knee)
        }
        set_metal_prefill_mm_tail(prior_tail)
    }
}

//! one scratch window's device legs: the panels counted against
//! `scratch`, the dev-W route off, the control window
def private scratch_window_metal(t : T?; m : Model; var sk : PrefillScratch; prompt : array<int64>;
                                 n : int64; scratch : bool;
                                 tag : string) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope w <- slice_ids(prompt, 0l, n)
        var inscope w_ctrl := w
        w_ctrl[n - 1l] = prompt[n]
        var win <- PrefillScratchWin(tag = tag, n = n)
        let served0 = metal_prefill_stats().prefills
        let sites0 = metal_prefill_devw_scratch_stats().sites
        win.gpu <- window_last_logits(m, w)
        win.sites = metal_prefill_devw_scratch_stats().sites - sites0
        let prior_devw = get_metal_prefill_dev_w()
        set_metal_prefill_dev_w(false)
        let own0 = metal_prefill_devw_scratch_stats().sites
        win.own <- window_last_logits(m, w)
        let own_sites = metal_prefill_devw_scratch_stats().sites - own0
        set_metal_prefill_dev_w(prior_devw)
        win.ctrl <- window_last_logits(m, w_ctrl)
        t |> equal(metal_prefill_stats().prefills - served0, 3l,
                   "{tag}/{n}: the three windows served on the device")
        if (scratch) {
            t |> success(win.sites > 0l, ("{tag}/{n}: the window's dense sites served off " +
                "scratch panels ({win.sites} sites)"))
        } else {
            t |> equal(win.sites, 0l,
                       "{tag}/{n}: the crowned twin held the window - no scratch panel")
        }
        t |> equal(own_sites, 0l, "{tag}/{n}: with the dev-W route off no site took a panel")
        sk.wins |> emplace(win)
    }
}

//! the short window's dev-W knobs on the base forms: a grid floor no
//! tile reaches, a half-tile knee its grid fits; each knob put back
//! and the driver rebuilt after
def private scratch_knee_metal(t : T?; m : Model; var sk : PrefillScratch; prompt : array<int64>;
                               tag : string) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope w <- slice_ids(prompt, 0l, SCRATCH_SHORT)
        let prior_floor = metal_devw_min_tgs()
        set_metal_devw_min_tgs(SCRATCH_GRID_FLOOR)
        metal_prefill_shutdown()   //! the driver caches the floor at init
        let floor0 = metal_prefill_devw_scratch_stats().sites
        var inscope floored <- window_last_logits(m, w)
        t |> equal(metal_prefill_devw_scratch_stats().sites - floor0, 0l,
                   ("{tag}/{SCRATCH_SHORT}: " +
            "under a grid floor no tile reaches no site took a panel (the floor-free window " +
            "above took them)"))
        set_metal_devw_min_tgs(prior_floor)
        let prior_knee = metal_split_k_tgs_hmm()
        set_metal_split_k_tgs_hmm(SCRATCH_HMM_KNEE)
        metal_prefill_shutdown()   //! ... and the knees
        let hsk0 = hmm_split_dispatches()
        scratch_window_metal(t, m, sk, prompt, SCRATCH_SHORT, true, "{tag}/hmm-split")
        let hsk = hmm_split_dispatches() - hsk0
        t |> success(hsk > 0l,
                     ("{tag}/hmm-split/{SCRATCH_SHORT}: the panels' half GEMM split its K " +
            "under the {SCRATCH_HMM_KNEE} knee ({hsk} split dispatches)"))
        let prior_split = get_metal_prefill_split_k()
        set_metal_prefill_split_k(false)
        let whole0 = hmm_split_dispatches()
        let sites0 = metal_prefill_devw_scratch_stats().sites
        var inscope whole <- window_last_logits(m, w)
        t |> success(metal_prefill_devw_scratch_stats().sites - sites0 > 0l,
            ("{tag}/hmm-split/{SCRATCH_SHORT}: the control - split-K off, the sites still took " +
                "panels"))
        t |> equal(hmm_split_dispatches() - whole0, 0l,
            "{tag}/hmm-split/{SCRATCH_SHORT}: ... and their half GEMM walked its whole K")
        set_metal_prefill_split_k(prior_split)
        set_metal_split_k_tgs_hmm(prior_knee)
        metal_prefill_shutdown()
    }
}

//! the kq-scratch arm: on the base forms both windows serve off
//! scratch panels; under every twin crown the short window stays on
//! the twins, the long one where the row says; the drivers rebuilt
//! under each crown set and the form's after
def private scratch_metal(t : T?; m : Model; p : PrefillRow;
                          var kept : PrefillKept) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "kq-scratch"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let tag = p.scratch_tag
        let crowns = crown_seat()
        var inscope prompt <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
        t |> success(long_length(prompt) > SCRATCH_LONG,
            "{tag}: the prompt covers the long window ({length(prompt)} tokens)")
        t |> equal(long_length(m.devwf16), 0l, "{tag}: the blob carries no stored dev-W panel")
        kept.panels.on = true
        kept.panels.ctx = m.config.seq_len
        kept.panels.long_w <- slice_ids(prompt, 0l, SCRATCH_LONG)
        with_metal_rails(t, tag, MetalRails(decode = false)) {
            pin_metal_tensor_crowns("")
            metal_prefill_shutdown()
            var inscope probe <- slice_ids(prompt, 0l, SCRATCH_SHORT)
            var inscope warm <- window_last_logits(m, probe)
            if (!pf_hmm_ready()) {
                t |> skip(("{tag}: the half GEMM did not compile (no tensor toolchain) - no site " +
                    "takes a panel on this box"))
            } else {
                scratch_window_metal(t, m, kept.panels, prompt, SCRATCH_SHORT, true, "{tag}/base")
                scratch_window_metal(t, m, kept.panels, prompt, SCRATCH_LONG, true, "{tag}/base")
                scratch_knee_metal(t, m, kept.panels, prompt, "{tag}/base")
                pin_metal_tensor_crowns(SCRATCH_TWIN_CROWNS)
                metal_prefill_shutdown()
                scratch_window_metal(t, m, kept.panels, prompt, SCRATCH_SHORT, false,
                    "{tag}/crowned")
                scratch_window_metal(t, m, kept.panels, prompt, SCRATCH_LONG, p.scratch_twin_long,
                    "{tag}/crowned")
            }
            restore_crowns(crowns)
        }
        metal_prefill_shutdown()
    }
}

//! one routed-GEMM leg: the twins under the format's crown, the
//! same uncrowned, or the gather panel off (the base form); the
//! drivers rebuilt under the leg's crowns
def private moe_base_leg(t : T?; m : Model; p : PrefillRow; var kb : PrefillBase; leg : string;
                         w2_ctrl : array<int64>) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let tag = kb.tag
        let base_leg = leg == "base"
        if (!base_leg && !pf_hmm_ready()) {
            t |> skip("{tag}/{leg}: the tensor toolchain is absent - no twin compiled")
            return
        }
        pin_metal_tensor_crowns(leg == "twins" ? p.base_crown : "")
        //! the routed crown table fills when the kernels compile
        metal_decode_shutdown()
        metal_prefill_shutdown()
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
        t |> success(metal_prefill_pso_init(),
            "{tag}/{leg}: the prefill kernels compiled under the leg's crowns")
        let prior_gx = set_metal_moe_gather_min(base_leg ? 1000000l : 1l)
        let served0 = metal_prefill_stats().prefills
        let base0 = metal_kernel_coverage_of(p.base_kernel)
        var bl <- PrefillBaseLeg(leg = leg)
        bl.gpu <- two_window_logits(m, kb.w1, kb.w2)
        let served = metal_prefill_stats().prefills - served0
        let based = metal_kernel_coverage_of(p.base_kernel) - base0
        bl.ctrl <- two_window_logits(m, kb.w1, w2_ctrl)
        set_metal_moe_gather_min(prior_gx)
        if (served == 0l) {
            t |> skip(("{tag}/{leg}: metal prefill declined (no Metal device?); the routed " +
                "forms' parity skipped"))
            delete bl
            return
        }
        t |> equal(served, 2l, "{tag}/{leg}: both windows served on the device")
        if (base_leg) {
            t |> success(based > 0l, ("{tag}/{leg}: the gate and up sites rode the base form " +
                "({based} dispatches of {p.base_kernel})"))
        } elif (leg == "uncrowned") {
            t |> success(based > 0l, ("{tag}/{leg}: with no {p.base_crown} crown the window past " +
                "the knee rode the base form ({based} dispatches of {p.base_kernel})"))
        } else {
            t |> equal(based, 0l, ("{tag}/{leg}: under the {p.base_crown} crown every routed " +
                "site rode its tensor twin, the base form none"))
        }
        bl.gpu_piece = piece(m, int64(max_element(bl.gpu)))
        kb.legs |> emplace(bl)
    }
}

//! the moe-base arm: a 280-row window then a 20-row one through the
//! routed block's three GEMM forms, each against the CPU control;
//! the drivers rebuilt under each leg's crowns and the form's after
def private moe_base_metal(t : T?; m : Model; p : PrefillRow;
                           var kept : PrefillKept) {   // nolint:LINT012,LINT019 - das_metal half
    return if (!arm_on(t, "moe-base"))
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let tag = p.base_tag
        if (p.blocks > 0l) {
            cut_within(t, "{tag}: ", m, p.blocks)
        }
        var inscope prompt <- encode(m, counting_prompt(ATTN_DEV_COUNT_TO), true, false)
        kept.routed.on = true
        kept.routed.tag = tag
        kept.routed.bar = p.base_bar
        kept.routed.ctx = m.config.seq_len
        kept.routed.w1 <- slice_ids(prompt, 0l, 280l)
        kept.routed.w2 <- slice_ids(prompt, 280l, 300l)
        var inscope w2_ctrl := kept.routed.w2
        w2_ctrl[19] = prompt[300]
        let crowns = crown_seat()
        with_metal_rails(t, tag, MetalRails(decode = false)) {
            t |> success(metal_prefill_pso_init(),
                "{tag}: the prefill kernels compiled - the census reads them")
            kept.routed.legs |> reserve(3)
            for (leg in ["twins", "uncrowned", "base"]) {
                moe_base_leg(t, m, p, kept.routed, leg, w2_ctrl)
            }
            restore_crowns(crowns)
        }
        metal_prefill_shutdown()
        metal_decode_shutdown()
    }
}

//! the main arm list's cells of a file: JIT-only
def private main_metal(t : T?; var m : Model; p : PrefillRow; var kept : PrefillKept) {
    return if (!p.main || !jit_only(t))
    var inscope prompt <- encode(m, counting_prompt(COUNT_TO), true, false)
    if (p.q8_set) {
        q8_metal(t, m, kept, prompt)
    }
    if (!empty(p.cont40)) {
        with_ctx_cut(m, HYBRID_CTX) {
            cont40_metal(t, m, kept, prompt, cont40_of(p.cont40))
        }
    }
    if (!empty(p.segs)) {
        with_ctx_cut(m, MIRROR_CTX) {
            segs_metal(t, m, p.segs)
        }
    }
    if (p.smol) {
        with_ctx_cut(m, LASTROW_CTX) {
            smol_metal(t, m)
        }
    }
    if (p.mirror_long) {
        mirror_long_metal(t, m)
    }
}

//! every Metal cell of the row `p` on the blob twin `m`
//! (`metal_form(ctx = p.ctx)`), each cut to its own context; the
//! sessions, tokens and logits the CPU form and the verdict read go
//! into `kept`; the attention, mirror, segs, span-fused, kq-scratch
//! and bf16-split cells shut the drivers down
[cold_path]
def prefill_parity_metal(t : T?; var m : Model; form : ModelForm; p : PrefillRow;
                         var kept : PrefillKept) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.blob_twin, p.file, "prefill")
            || !family_on(t, p.family))
        if (m.config.seq_len < p.ctx) {
            t |> failure(("{p.file}: the Metal form's context {m.config.seq_len} is under the " +
                "cells' {p.ctx}"))
            return
        }
        kept.file = p.file
        kept.metal_ran = true
        kept.cpu_iq3s = p.cpu_iq3s
        if (!empty(p.base_tag)) {
            with_ctx_cut(m, WINDOW_CTX) {
                moe_base_metal(t, m, p, kept)
            }
        }
        if (!empty(p.short_tag)) {
            with_ctx_cut(m, WINDOW_CTX) {
                short_window_metal(t, m, p, kept)
            }
        }
        if (p.lastrow_bar > 0.0) {
            with_ctx_cut(m, LASTROW_CTX) {
                lastrow_metal(t, m, p)
            }
        }
        main_metal(t, m, p, kept)
        if (!empty(p.scratch_tag)) {
            with_ctx_cut(m, WINDOW_CTX) {
                scratch_metal(t, m, p, kept)
            }
        }
        if (p.bf16) {
            with_ctx_cut(m, LASTROW_CTX) {
                bf16_metal(t, m)
            }
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! the CPU form every control runs on: the kernel backend pinned
//! portable, as the old suite's references loaded; IQ3_S served as
//! `iq3s_serve` where named ("grid": the form a Metal-mode load takes)
def prefill_cpu_form(iq3s_serve : string = "") : ModelForm =>
    cpu_cells_form(backend = "portable", iq3s_serve = iq3s_serve)

//! a cell's CPU control
def private cpu_control(m : Model; cell : PrefillCell) : array<int64> {
    if (cell.replay == PrefillReplay.generate) {
        var ids : array<int64>
        with_dasllama_jobque() {
            var sa = make_run_state(m.config, cell.kdt, cell.kdt)
            var inscope got <- generate(m, sa, cell.chunks[0], length(cell.chunks[0]) + cell.steps)
            ids := got
            delete sa
        }
        return <- ids
    }
    if (cell.replay == PrefillReplay.codec_windows) {
        return <- codec_two_window_greedy(m, cell.chunks[0], cell.chunks[1], cell.kdt, cell.steps)
    }
    var s = make_run_state(m.config, cell.kdt, cell.kdt)
    with_dasllama_jobque() {
        cell_prefill(m, s, cell.replay, cell.chunks, cell.grid, true)
    }
    var ids : array<int64>
    with_dasllama_jobque() {
        ids <- greedy_eval_steps(m, s, cell.steps) // nolint:PERF030 - fresh; the block runs once
    }
    delete s
    return <- ids
}

//! a cell's CPU side: its control, the kept sessions' greedy steps,
//! the deepstack add's rows; each session freed once decoded
def private cpu_cell(t : T?; var m : Model; var cell : PrefillCell) {
    with_ctx_cut(m, cell.ctx) {
        if (m.config.seq_len != cell.ctx) {
            t |> failure(("the CPU form's context {m.config.seq_len} is not the kept " +
                "sessions' {cell.ctx}"))
            return
        }
        let prior = stamp_config(m, cell.replay)
        cell.cpu <- cpu_control(m, cell)
        if (cell.want_len > 0) {
            t |> equal(length(cell.cpu), cell.want_len)
        }
        for (leg in cell.legs) {
            if (leg.decode) {
                with_dasllama_jobque() {
                    leg.ids <- greedy_eval_steps(m, leg.s, cell.steps)
                }
                delete leg.s
            }
            if (!empty(leg.log_tag)) {
                log_gen_texts(m, leg.log_tag, cell.cpu, leg.ids)
            }
        }
        if (cell.replay == PrefillReplay.deepstack) {
            cell.cpu_rows <- [prefill_rows(m, cell.kdt, cell.replay, cell.chunks, cell.grid, true),
                prefill_rows(m, cell.kdt, cell.replay, cell.chunks, cell.grid, false)]
        }
        restore_config(m, prior)
    }
}

//! every control the Metal cells of `kept` wait for, on the CPU form
//! `prefill_cpu_form(kept.cpu_iq3s)` alone: the kept sessions decoded
//! and freed, the short, scratch and moe-base windows' logits; other
//! CPU forms pass by
[cold_path]
def prefill_parity_cpu(t : T?; var m : Model; form : ModelForm; var kept : PrefillKept) {
    return if (!kept.metal_ran || !alone_pass_of(form, prefill_cpu_form(kept.cpu_iq3s)))
    kept.cpu_ran = true
    with_metal_rails(MetalRails(rail = "", min_npos = 0l)) {
        for (cell in kept.cells) {
            cpu_cell(t, m, cell)
        }
        if (kept.window.on) {
            with_ctx_cut(m, kept.window.ctx) {
                for (w in kept.window.wins) {
                    w.cpu <- two_window_logits(m, kept.window.w1, w.w2)
                    w.cpu_piece = piece(m, int64(max_element(w.cpu)))
                }
            }
        }
        if (kept.panels.on && !empty(kept.panels.wins)) {
            with_ctx_cut(m, kept.panels.ctx) {
                var inscope short_w <- slice_ids(kept.panels.long_w, 0l, SCRATCH_SHORT)
                kept.panels.cpu_short <- window_last_logits(m, short_w)
                kept.panels.cpu_long <- window_last_logits(m, kept.panels.long_w)
            }
        }
        if (kept.routed.on && !empty(kept.routed.legs)) {
            with_ctx_cut(m, kept.routed.ctx) {
                kept.routed.cpu <- two_window_logits(m, kept.routed.w1, kept.routed.w2)
                kept.routed.cpu_piece = piece(m, int64(max_element(kept.routed.cpu)))
            }
        }
    }
}

def private deepstack_verdict(t : T?; cell : PrefillCell) {
    let kdt = cell.kdt
    var a_gpu = 0.0
    var dd = 0.0
    for (i in range64(min(long_length(cell.gpu_rows[0]), long_length(cell.cpu_rows[0])))) {
        let ag = cell.gpu_rows[0][i] - cell.gpu_rows[1][i]
        let ac = cell.cpu_rows[0][i] - cell.cpu_rows[1][i]
        a_gpu = max(a_gpu, abs(ag))
        dd = max(dd, nan_wide_diff(ag, ac))
    }
    to_log(LOG_INFO, ("span-ds/{kdt} GPU add-contribution max = {a_gpu}, GPU-vs-CPU contribution " +
        "maxdiff = {dd}\n"))
    //! clean dd reads 10-13% of a_gpu; a skipped add is a_gpu = 0
    t |> success(a_gpu > 0.01, "{kdt} the slice adds reach the GPU prefill logits ({a_gpu})")
    t |> success(dd < 0.5 * a_gpu,
                 "{kdt} the GPU add contribution matches the CPU's ({dd} vs {a_gpu})")
}

def private cell_verdict(t : T?; cell : PrefillCell) {
    for (leg in cell.legs) {
        t |> success(ids_equal(cell.cpu, leg.ids), leg.text)
    }
    if (cell.replay == PrefillReplay.deepstack && length(cell.gpu_rows) == 2
            && length(cell.cpu_rows) == 2) {
        deepstack_verdict(t, cell)
    }
}

def private short_verdict(t : T?; sw : PrefillShort) {
    let fname = sw.form_name
    for (w in sw.wins) {
        let tag = "{sw.tag}/{w.n}"
        let g = logits_maxdiff(w.gpu, w.cpu)
        let gt = logits_maxdiff(w.tiles, w.cpu)
        let gc = logits_maxdiff(w.ctrl, w.cpu)
        let peak = logits_maxabs(w.cpu)
        to_log(LOG_INFO, ("{tag}: {fname} gap {g} of peak {peak} ({g / peak}); tiles gap {gt} " +
            "({gt / peak}); control gap {gc} ({gc / peak})\n"))
        t |> success(g <= sw.bar * peak,
                     ("{tag}: the {fname}'s last-row logits within {sw.bar} of " +
            "the CPU chain's largest ({g} of {peak}; the tiles read {gt})"))
        t |> success(gc > sw.bar * peak, ("{tag}: the control window (its last token changed) " +
            "lands outside the bar ({gc} of {peak})"))
        let am_gpu = max_element(w.gpu)
        let am_cpu = max_element(w.cpu)
        to_log(LOG_INFO, ("{tag}: argmax pieces - {fname} {escape(w.gpu_piece)}, CPU " +
            "{escape(w.cpu_piece)}, tiles {escape(w.tiles_piece)}\n"))
        if (sw.whole) {
            t |> success(am_gpu == am_cpu, ("{tag}: the {fname}'s argmax is the CPU chain's " +
                "({am_gpu} / {am_cpu}; the tiles' {max_element(w.tiles)})"))
        }
    }
}

def private moe_base_verdict(t : T?; kb : PrefillBase) {
    let peak = logits_maxabs(kb.cpu)
    for (bl in kb.legs) {
        let tag = "{kb.tag}/{bl.leg}"
        let g = logits_maxdiff(bl.gpu, kb.cpu)
        let gc = logits_maxdiff(bl.ctrl, kb.cpu)
        to_log(LOG_INFO, ("{tag}: gap {g} of peak {peak} ({g / peak}); control gap {gc} " +
            "({gc / peak}); argmax pieces - GPU {escape(bl.gpu_piece)}, CPU " +
            "{escape(kb.cpu_piece)}\n"))
        t |> success(g <= kb.bar * peak,
            "{tag}: last-row logits within {kb.bar} of the CPU chain's largest ({g} of {peak})")
        t |> success(gc > kb.bar * peak, ("{tag}: the control window (its last token changed) " +
            "lands outside the bar ({gc} of {peak})"))
    }
}

def private scratch_verdict(t : T?; sk : PrefillScratch) {
    for (w in sk.wins) {
        if (w.n == SCRATCH_SHORT) {
            scratch_win_verdict(t, w, sk.cpu_short)
        } else {
            scratch_win_verdict(t, w, sk.cpu_long)
        }
    }
}

def private scratch_win_verdict(t : T?; w : PrefillScratchWin; cpu : array<float>) {
    let tag = "{w.tag}/{w.n}"
    let bar = SCRATCH_BAR
    let peak = logits_maxabs(cpu)
    let g = logits_maxdiff(w.gpu, cpu)
    let go = logits_maxdiff(w.own, cpu)
    let gc = logits_maxdiff(w.ctrl, cpu)
    to_log(LOG_INFO,
           ("{tag}: {w.sites} panel sites, gap {g} of peak {peak} ({g / peak}); own-bytes " +
        "gap {go} ({go / peak}); control gap {gc} ({gc / peak})\n"))
    t |> success(g <= bar * peak,
        ("{tag}: the window's last-row logits within {bar} of the CPU chain's largest ({g} of " +
            "{peak})"))
    t |> success(go <= bar * peak, "{tag}: and the own-bytes window's ({go})")
    t |> success(gc > bar * peak,
        "{tag}: the control window (its last token changed) lands outside the bar ({gc} of {peak})")
    t |> equal(max_element(w.gpu), max_element(cpu),
               "{tag}: the window's argmax is the CPU chain's")
}

//! the compares of `kept` after its block: every Metal leg against
//! its CPU control, a failure where the Metal legs ran and no control
//! did; frees what `kept` holds
[cold_path]
def prefill_parity_verdict(t : T?; var kept : PrefillKept) {
    let waits = (!empty(kept.cells) || !empty(kept.window.wins) || !empty(kept.panels.wins)
        || !empty(kept.routed.legs))
    if (kept.metal_ran && waits && !kept.cpu_ran) {
        t |> failure(("{kept.file}: the Metal legs ran and no CPU control did - the block gives " +
            "no `prefill_cpu_form(\"{kept.cpu_iq3s}\")`"))
    } elif (kept.cpu_ran) {
        for (cell in kept.cells) {
            cell_verdict(t, cell)
        }
        short_verdict(t, kept.window)
        scratch_verdict(t, kept.panels)
        moe_base_verdict(t, kept.routed)
    }
    delete kept
}

//! the rows of the model files, one a file
def prefill_llama_1b_q8() : PrefillRow => PrefillRow(file = "Llama-3.2-1B-Instruct-Q8_0.gguf",
    family = "llama", ctx = PREFILL_LLAMA_1B_Q8_CTX, main = true, q8_set = true, segs = "Q8_0",
    scratch_tag = "kq-scratch/llama-q8")

def prefill_llama_1b_q4km() : PrefillRow => PrefillRow(file = "Llama-3.2-1B-Instruct-Q4_K_M.gguf",
    family = "llama", ctx = PREFILL_LLAMA_1B_Q4KM_CTX, main = true, cont40 = "kq", segs = "Q4_K_M",
    short_tag = "kq-tail/llama-k4", short_bar = 0.08, short_form = ShortForm.kq_tail, whole = true,
    scratch_tag = "kq-scratch/llama-k4", scratch_twin_long = true)

def prefill_llama_1b_iq4xs() : PrefillRow => PrefillRow(file = "Llama-3.2-1B-Instruct-IQ4_XS.gguf",
    family = "llama", ctx = PREFILL_LLAMA_1B_IQ4XS_CTX, main = true, segs = "IQ4_XS",
    short_tag = "kq-tail/llama-iq4xs", short_bar = 0.08, short_form = ShortForm.kq_tail,
    whole = true,
    scratch_tag = "kq-scratch/llama-iq4xs", scratch_twin_long = true)

def prefill_llama_1b_q51() : PrefillRow =>
    PrefillRow(file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf",
    family = "llama", ctx = PREFILL_LLAMA_1B_Q51_CTX, short_tag = "kq-tail/llama-q51",
    short_bar = 0.08,
    short_form = ShortForm.kq_tail, whole = true, scratch_tag = "kq-scratch/llama-q51",
    scratch_twin_long = true)

def prefill_llama_3b() : PrefillRow => PrefillRow(file = "Llama-3.2-3B-Instruct-Q8_0.gguf",
    family = "llama", ctx = PREFILL_LLAMA_3B_CTX, main = true, mirror_long = true)

def prefill_llama31_8b() : PrefillRow => PrefillRow(file = "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf",
    family = "llama", ctx = PREFILL_LLAMA31_8B_CTX, main = true, cont40 = "dim")

def prefill_smol() : PrefillRow => PrefillRow(file = "SmolLM2-135M-Instruct-Q8_0.gguf",
    family = "llama", ctx = PREFILL_SMOL_CTX, main = true, smol = true)

def prefill_gemma3_1b() : PrefillRow => PrefillRow(file = "gemma-3-1b-it-Q8_0.gguf",
    family = "gemma3", ctx = PREFILL_GEMMA3_1B_CTX, whole = true, lastrow_bar = 0.05)

//! the 12B's first twelve blocks: rows to a 1000 peak, its own bar
def prefill_gemma4_12b_12l() : PrefillRow => PrefillRow(file = "gemma-4-12B-12L-Q8_0-local.gguf",
    family = "gemma4", ctx = PREFILL_GEMMA4_12B_12L_CTX, blocks = 12l, lastrow_bar = 1.0)

def prefill_gemma4_e2b() : PrefillRow => PrefillRow(file = "gemma-4-E2B-it-Q8_0.gguf",
    family = "gemma4e", ctx = PREFILL_GEMMA4_E2B_CTX, whole = true, lastrow_bar = 0.05, bf16 = true)

def prefill_qwen15_moe_4l() : PrefillRow =>
    PrefillRow(file = "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf",
    family = "qwen2moe", ctx = PREFILL_QWEN15_MOE_4L_CTX, short_tag = "moe-gemv/qwen2moe-q8",
    short_bar = 0.08, short_form = ShortForm.moe_route, blocks = 4l)

def prefill_qwen3_30b_7l() : PrefillRow => PrefillRow(file = "Qwen3-30B-A3B-7L-Q4_K_M-local.gguf",
    family = "qwen3moe", ctx = PREFILL_QWEN3_30B_7L_CTX, short_tag = "moe-gemv/qwen3moe-k4",
    short_bar = 0.10, short_form = ShortForm.moe_route, blocks = 7l)

def prefill_gptoss_4l() : PrefillRow => PrefillRow(file = "gpt-oss-4L-mxfp4-local.gguf",
    family = "gptoss", ctx = PREFILL_GPTOSS_4L_CTX, short_tag = "moe-gemv/gptoss-mx4",
    short_bar = 0.12, short_form = ShortForm.moe_route, blocks = 4l)

def prefill_qwen15_moe() : PrefillRow => PrefillRow(file = "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf",
    family = "qwen2moe", ctx = PREFILL_QWEN15_MOE_CTX, short_tag = "moe-gemv/qwen2moe-q8-whole",
    short_bar = 0.08, short_form = ShortForm.moe_route, whole = true)

def prefill_qwen3_30b() : PrefillRow => PrefillRow(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf",
    family = "qwen3moe", ctx = PREFILL_QWEN3_30B_CTX, short_tag = "moe-gemv/qwen3moe-k4-whole",
    short_bar = 0.10, short_form = ShortForm.moe_route, whole = true)

def prefill_qwen3_4b() : PrefillRow => PrefillRow(file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf",
    family = "qwen3", ctx = PREFILL_QWEN3_4B_CTX, short_tag = "kq-tail/qwen3-k4", short_bar = 0.08,
    short_form = ShortForm.kq_tail, whole = true)

def prefill_qwen25_iq4nl() : PrefillRow =>
    PrefillRow(file = "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf",
    family = "qwen2", ctx = PREFILL_QWEN25_IQ4NL_CTX, short_tag = "kq-tail/qwen2-iq4nl32",
    short_bar = 0.08, short_form = ShortForm.kq_tail, whole = true)

//! the Q3_K expert stack: its CPU control on the grid-served IQ3_S
def prefill_qwen3_30b_7l_q3kxl() : PrefillRow => PrefillRow(
    file = "Qwen3-30B-A3B-7L-UD-Q3_K_XL-local.gguf", family = "qwen3moe",
    ctx = PREFILL_QWEN3_30B_7L_Q3KXL_CTX, base_tag = "moe-base/qwen3moe-k3", blocks = 7l,
    base_kernel = "metal_moe_mulmm_k3", base_crown = "moe_mulmm_k3", base_bar = 0.10,
    cpu_iq3s = "grid")

//! the IQ2 expert stacks: their CPU control on the grid-served IQ3_S
def prefill_qwen3_30b_7l_iq2xxs() : PrefillRow => PrefillRow(
    file = "Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf", family = "qwen3moe",
    ctx = PREFILL_QWEN3_30B_7L_IQ2XXS_CTX, base_tag = "moe-base/qwen3moe-iq2xxs", blocks = 7l,
    base_kernel = "metal_moe_mulmm_iq2xxs", base_crown = "moe_mulmm_iq2xxs", base_bar = 0.15,
    cpu_iq3s = "grid")
