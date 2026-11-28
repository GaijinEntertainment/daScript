options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

// The host twin's model-level matrix (the `qwen15-moe` category: `--category qwen15-moe` opens its files): Qwen1.5-MoE-A2.7B-Chat's
// routed experts requantized to one format a file
// (`moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-<TYPE>.gguf`, the rest Q8_0; the down stacks' 1408-wide rows take the type's
// per-32 fallback where a superblock type is asked), four routed layers' experts pinned to the host under the streamed
// split, the rare rail reading them off the tier's pinned device-layout planes through the host twin: the split token
// command and the cut window chain against the CPU chain, one window and two; the pool's two knobs (the DMA fill, the
// decode meet) and the rare rail's three A/B arms off, each pinned in process on a carrier of the matrix; the NVFP4 row
// runs the same two cells on Qwen3.6-35B-A3B's own NVFP4 file (the qwen35moe-35b category: no cut of it). Each file is ONE
// block (`with_cpu_and_gpu_model`): the four-layer form feeds the format cells and the rail arms, the share's pinned pool is a
// form of its own, and each pool cell whose witness is a cold pool's fills opens a twenty-layer form of its own; the CPU
// form's feeds answer every cell. Skips: a low-bit carrier's Metal form on a build without das_metal, its crowned window
// where the box compiles no Metal-4 tensor twin, a file not
// minted (the 35B NVFP4 not stocked - it is downloaded, not minted), DASLLAMA_PARITY_FULL unset for the large tier, no armed
// Vulkan tier, the knob cells where the plan found no room for the pool, the expert-cache cell under
// DASLLAMA_IMAGE=0 (it reads the lane), the NVFP4 cut's FP4-chain fault cell (the CUDA home's alone) where the card sits
// under the FP4 rung or DASLLAMA_CUDA_MMQ=0 stands the route down. Run the format cells under DASLLAMA_IMAGE=0: a dozen carriers would mint a dozen
// images beside them (the image rail's own coverage is the image suites'), and the planes in RAM reach the same kernels.

require dastest/testing_boost public
require _moe_resident
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_env
require daslib/fio
require dasllama/dasllama_blocks
require dasllama/dasllama_math   // with_dasllama_jobque_: the cut block's compares
require dasllama/dasllama_common   // Model
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require _format_census   // census_cell: the cut's every site in its own format
require _kquant_cells   // kq_expert_sites_cell: the 1-bit carriers' K-quant native CPU form
require _metal_format_census_cells   // format_census_*: the low-bit carriers' Metal census row
require _kv_codec_cells   // codec_env_why: the block's f16 mirror ask
require daslib/strings_boost   // split: a fixture's census sites
require dasllama/dasllama   // create_session: the fault form's refused window
require dasllama/dasllama_cuda_decode   // cuda_window_planes_held: the FP4-chain fault form's planes
require daslib/rtti   // this_context().last_exception: the refused window's text
require strings   // find: the refusal's words
require _metal_format_census_cells   // format_census_*: the NVFP4 cut's Metal census row

let private QWEN15_MOE_EXPERTS = 60l   //! the carrier's routed experts a layer (Qwen1.5-MoE-A2.7B): a pool at or past it never fills
let private SHARE_CELL_SLOTS = 16l     //! the share cell's pool: a quarter of the experts, so a step's misses outrun the free slots
let private MATRIX_CTX = 32768l        //! the carrier's own context: the matrix's mirror arms uncapped
let private POOL_HOST_LAYERS = 20l     //! the pool cells' host layers: past them the plan's room arms the unasked pool


//! `body` with the meet's host counter as the witness: the cell's windows handed the host experts; under `dev_rows` the
//! one-pass rail read them off the device planes (the three-pass rail reads the planar planes); under `one_row_rail` the fed
//! steps' picks went through the one-row rail (a decode-meet cell routes them through the window's meet instead); a cell that
//! served no window (its carrier not minted, the driver declined) skipped and is not asked
def private witnessed(t : T?; one_row_rail, dev_rows : bool; body : block<() : void>) {
    let at = host_rail_at()
    let p0 = gpu_resident_prefills_()
    invoke(body)
    return if (gpu_resident_prefills_() == p0)
    held_on(t) $(h) {
        witness_since(h, at, one_row_rail, dev_rows, "the cell's windows")
    }
}

//! the host rail's counters read now: the meet's host experts, the rare rail's window rows and step pairs
struct private HostRailAt {
    met : int64
    dev_rows : int64
    dec_rows : int64
}

def private host_rail_at() : HostRailAt => HostRailAt(met = g_moe_meet_cpu_experts, dev_rows = g_rare_dev_rows, dec_rows = g_rare_dec_rows)

//! since `at`, held into `h`: the meet handed the host experts over `what`; under `dev_rows` the rare rail read rows off the
//! device planes; under `one_row_rail` the fed steps' host picks went through the rail
def private witness_since(var h : Held; at : HostRailAt; one_row_rail, dev_rows : bool; what : string) {
    if (home_has_meet_chain()) {
        h |> held_fact(g_moe_meet_cpu_experts > at.met, "the meet handed the host {g_moe_meet_cpu_experts - at.met} experts over {what}")
    } else {
        h |> held_cell_skip("the meet's host share over {what}: this home has no f16-fed expert chain (a Vulkan device without NV_coopmat2), so no window meets")
    }
    if (dev_rows) {
        h |> held_fact(g_rare_dev_rows > at.dev_rows, "the rare rail read {g_rare_dev_rows - at.dev_rows} rows off the device planes over {what} - the kernels' rows are in the bar")
    }
    if (one_row_rail) {
        h |> held_fact(g_rare_dec_rows > at.dec_rows, "the fed steps' host picks read {g_rare_dec_rows - at.dec_rows} pairs off the device planes through the rail - the steps' logits are in the bar")
    }
}

//! the lever a matrix cell pins around its feed - a run-time setting the rare rail and the pool read a window or a step at a
//! time: the matrix's plain rail, the rare rail's three A/B arms off, the pool's knobs
enum private CellRail {
    plain       //! the one-copy rail as the block pins it
    three_pass  //! set_rare_onepass(0)
    row_core    //! set_rare_batched(0)
    planar      //! set_rare_arena(0)
    share       //! the DMA fill on, the fill's share at 1
    dma         //! the DMA fill on
    no_route    //! the DMA fill off and the decode meet off
    dec_meet    //! the decode meet on
    host_fill   //! the DMA fill off: the pool's slots filled off the host
}

//! the form a matrix cell feeds on: its routed layers on the host, the pool's slots asked at the load (0 the plan's), and among
//! the twenty-layer forms of no slot ask its turn - a pool cell whose witness is a cold pool's fills opens a load of its own
struct private CellForm {
    layers : int64
    slots : int64
    pool : int
}

def private same_form(a, b : CellForm) : bool => a.layers == b.layers && a.slots == b.slots && a.pool == b.pool

//! one matrix cell: its title (the subtest its compares run as), its form, its prompt, its lever, what its feed held and
//! collected; `skipped` where a skip held at the build leaves it unfed
struct private MatrixCell {
    title : string
    at : CellForm
    n : int64
    rail : CellRail
    h : Held
    gpu : FeedRows
    fed : bool
    skipped : bool
}

//! a carrier's block: its fixture, the forms' admissions, the cells in their feed order, the prompts they feed, the CPU form's
//! feeds of them, the twenty-layer forms the block has seen
struct private MatrixRows {
    fx : MoeFixture = MoeFixture()
    h : Held
    cells : array<MatrixCell>
    prompts : array<int64>
    cpu : array<FeedRows>
    pools_seen : int
}

//! which of a carrier's cells its block runs: the two format cells, with the pool's knob cells, or every cell (the Q4_K file's:
//! the rail arms and the hot pool beside them)
enum private MatrixCells {
    formats
    knobs
    every
}

//! a cell onto `r`, unfed after `skip_why` held where it is not empty
def private add_cell(var r : MatrixRows; title : string; at : CellForm; n : int64; rail : CellRail; skip_why : string = "") {
    var c = MatrixCell(title = title, at = at, n = n, rail = rail, skipped = !empty(skip_why))
    if (c.skipped) {
        c.h |> held_skip(skip_why)
    }
    r.cells |> emplace(c)
}

let private FOUR_LAYERS = CellForm(layers = 4l)

//! why the pool cells skip: the environment asks no hot pool (the plan then sizes none, whatever the card's room); "" else
def private pool_unasked_why() : string {
    return "DASLLAMA_GPU_HEAT=0 asks no hot expert pool - the cell needs that unset" if (g_env_gpu.gpu_heat ?? 1l == 0l)
    return ""
}

//! the matrix's two cells over any host-pinned carrier: `tag` names the expert format in the cell's title
def private add_format_cells(var r : MatrixRows; tag : string) {
    add_cell(r, "{tag} experts on at least four host layers through the one-copy rail: the split token command and the cut window chain vs the CPU chain, one window", FOUR_LAYERS, 40l, CellRail.plain)
    add_cell(r, "{tag} experts on at least four host layers through the one-copy rail: the same across two windows", FOUR_LAYERS, TWO_WINDOWS, CellRail.plain,
        pf_window_why(PF_WINDOW_ROWS))
}

//! the pool's two knobs pinned on in process, one carrier a family of the matrix: the DMA fill (a token's misses copied into
//! the pool on the transfer queue, no host gather) and the decode meet (a token's picks claimed through the prefill's meet) -
//! the fed steps' picks against the CPU chain at the matrix's bar, each knob's own counter the witness that it ran; the share's
//! cell on its pool pinned to fewer slots than the carrier's experts, so the pool fills and the heat gate has misses to leave
def private add_knob_cells(var r : MatrixRows; tag : string) {
    let why = pool_unasked_why()
    add_cell(r, "{tag} experts on twenty host layers, the fill's share of a step's misses past the heat gate: the token command vs the CPU chain",
        CellForm(layers = POOL_HOST_LAYERS, slots = SHARE_CELL_SLOTS), 40l, CellRail.share, why)
    add_cell(r, "{tag} experts on twenty host layers, the pool's misses filled by DMA: the token command vs the CPU chain",
        CellForm(layers = POOL_HOST_LAYERS), 40l, CellRail.dma, why)
    add_cell(r, "{tag} experts on twenty host layers, the DMA fill off and the meet off: no expert lands by DMA, the token steps' misses take the host's upload beside the window's placements",
        CellForm(layers = POOL_HOST_LAYERS, pool = 1), 40l, CellRail.no_route, why)
    add_cell(r, "{tag} experts on twenty host layers, a token's picks through the decode meet: the token command vs the CPU chain",
        CellForm(layers = POOL_HOST_LAYERS), 40l, CellRail.dec_meet)
}

//! the rare rail's three A/B arms pinned off on the Q4_K file, one cell each: the three region-list passes in place of the
//! one-pass lane, the rows core a row in place of the batched cores, and the planar planes in place of the arena - the same
//! compare at the matrix's bar, the arm's counter the witness; then the hot pool over twenty streamed layers, its slots filled
//! off the layers' pinned mirrors
def private add_rail_cells(var r : MatrixRows) {
    add_cell(r, "Q4_K experts on four host layers through the three-pass rail (DASLLAMA_RARE_ONEPASS=0): the token command vs the CPU chain", FOUR_LAYERS, 40l, CellRail.three_pass)
    add_cell(r, "Q4_K experts on four host layers, the rows core a row (DASLLAMA_RARE_BATCHED=0): the token command vs the CPU chain", FOUR_LAYERS, 40l, CellRail.row_core)
    add_cell(r, "Q4_K experts on four host layers off the planar planes (DASLLAMA_RARE_ARENA=0): the token command vs the CPU chain", FOUR_LAYERS, 40l, CellRail.planar)
    add_cell(r, "Q4_K experts on twenty host layers with the hot pool: the slots filled off the mirrors, the token command vs the CPU chain",
        CellForm(layers = POOL_HOST_LAYERS, pool = 2), 40l, CellRail.host_fill, pool_unasked_why())
}

//! a carrier's block on `fx`, its cells `set` (titles naming the format `tag`): the cells, the prompts its fed cells take, the
//! forms they feed on - on an f16 mirror (a codec the environment keeps off held as the block's skip), one region, the planes
//! kept on the host
def private matrix_rows(tag : string; fx : MoeFixture; set : MatrixCells) : MatrixRows {
    var r = MatrixRows(fx = fx)
    add_format_cells(r, tag)
    if (set != MatrixCells.formats) {
        add_knob_cells(r, tag)
    }
    if (set == MatrixCells.every) {
        add_rail_cells(r)
    }
    for (c in r.cells) {
        if (!c.skipped && !has_value(r.prompts, c.n)) {
            r.prompts |> push(c.n)
        }
    }
    return <- r
}

//! the block's device forms, one a distinct `CellForm` of the fed cells in their order
def private matrix_forms(var r : MatrixRows) : array<ModelForm> {
    var forms : array<ModelForm>
    let why = codec_env_why(KVDtype.f16)
    if (!empty(why)) {
        r.h |> held_skip("the matrix's f16 mirror: {why}")
        return <- forms
    }
    var seen : array<CellForm>
    for (c in r.cells) {
        continue if (c.skipped)
        let at = find_index_if(seen) $(s) => same_form(s, c.at)
        continue if (at >= 0)
        seen |> push(c.at)
        forms |> push(device_form(regions = 1l, host = true, host_layers = c.at.layers, pool_slots = c.at.slots) |> asking(KVDtype.f16))
    }
    return <- forms
}

//! the cell form a device form of the block is: the twenty-layer forms of no slot ask in their turn
def private cell_form_of(var r : MatrixRows; form : ModelForm) : CellForm {
    return CellForm(layers = form.host_layers, slots = form.pool_slots) if (form.host_layers != POOL_HOST_LAYERS || form.pool_slots > 0l)
    r.pools_seen++
    return CellForm(layers = form.host_layers, pool = r.pools_seen - 1)
}

//! the pool's counters a cell reads across its feed: the DMA-placed experts, the share's fills and candidates, the host's
//! uploads, the met steps, the slot copies off the pinned mirrors
struct private PoolAt {
    dma : int64
    share_fills : int64
    share_seen : int64
    uploads : int64
    meet_steps : int64
    copies : int64
}

def private pool_at() : PoolAt {
    let s = gpu_resident_hot_stats()
    return PoolAt(dma = s.dma, share_fills = s.share_fills, share_seen = s.share_seen, uploads = s.uploads, meet_steps = s.meet_steps,
        copies = g_rare_slot_copies)
}

//! the cell's lever pinned for its feed; the stream pins put it back
def private rail_pin(rail : CellRail) {
    if (rail == CellRail.three_pass) {
        set_rare_onepass(0)
    } elif (rail == CellRail.row_core) {
        set_rare_batched(0)
    } elif (rail == CellRail.planar) {
        set_rare_arena(0)
    } elif (rail == CellRail.share || rail == CellRail.dma) {
        set_gpu_hot_dma(1)
        if (rail == CellRail.share) {
            set_gpu_hot_share(1.0)
        }
    } elif (rail == CellRail.no_route || rail == CellRail.host_fill) {
        set_gpu_hot_dma(0)
        if (rail == CellRail.no_route) {
            set_gpu_dec_meet(0)   //! the DMA cell's no-route leg: the meet's claims ride DMA whatever the knob says
        }
    } elif (rail == CellRail.dec_meet) {
        set_gpu_dec_meet(1)
    }
}

//! false after the held skip where the plan sized no pool beside the mirror: the pool's counters reset at the arm, so a pool
//! that served the form's feeds counted its picks
def private pool_armed(var h : Held) : bool {
    if (gpu_resident_hot_stats().picks == 0l) {
        h |> held_skip("the plan found no room for the pool beside the mirror on this card")
        return false
    }
    return true
}

//! the share's witnesses since `p0`: the fill landed experts by DMA, and on a pool short of the carrier's experts the heat
//! gate left the share misses, every one admitted
def private share_witness(var h : Held; p0 : PoolAt) {
    let p1 = pool_at()
    h |> held_fact(p1.dma > p0.dma, "the fill landed {p1.dma - p0.dma} experts by DMA over the cell")
    if (gpu_resident_hot_slots() >= QWEN15_MOE_EXPERTS) {
        h |> held_skip("the pool holds every expert of the carrier ({gpu_resident_hot_slots()} slots): it never fills, and the heat gate leaves the share nothing - the pool was armed before this cell's slot pin")
        return
    }
    h |> held_fact(p1.share_seen > p0.share_seen, "the heat gate left the share misses to admit ({p1.share_seen - p0.share_seen}) with {gpu_resident_hot_slots()} slots over {QWEN15_MOE_EXPERTS} experts")
    h |> held_equal(p1.share_fills - p0.share_fills, p1.share_seen - p0.share_seen, "at share 1 every miss the heat gate left ({p1.share_seen - p0.share_seen}) was admitted - every expert of the carrier is a copy the tier can make")
}

//! a pool cell's witnesses since `p0`, its pool armed: the share's, the DMA fill's landings, the no-route leg's control and
//! uploads, the host fill's uploads three plane copies each
def private pool_witness(var h : Held; rail : CellRail; p0 : PoolAt) {
    if (rail == CellRail.dec_meet) {
        let steps = gpu_resident_hot_stats().meet_steps - p0.meet_steps
        h |> held_fact(steps > 0l, "the decode meet claimed {steps} token steps' picks over the cell")
        return
    }
    return if (!pool_armed(h))
    let p1 = pool_at()
    if (rail == CellRail.share) {
        share_witness(h, p0)
    } elif (rail == CellRail.dma) {
        h |> held_fact(p1.dma > p0.dma, "the DMA fill landed {p1.dma - p0.dma} experts in the pool over the cell")
    } elif (rail == CellRail.no_route) {
        h |> held_equal(p1.dma - p0.dma, 0l, "control: with the fill off no expert landed by DMA over the cell")
        h |> held_fact(p1.uploads > p0.uploads, "the window's placements and the steps' misses took the host's upload ({p1.uploads - p0.uploads})")
    } else {
        h |> held_fact(p1.uploads > p0.uploads, "the plan armed the hot pool beside the mirror and it uploaded {p1.uploads - p0.uploads} experts over the cell")
        h |> held_equal(p1.copies - p0.copies, 3l * (p1.uploads - p0.uploads), "every slot filled off the pinned mirrors: three plane copies an upload, no planar plane read")
    }
}

//! one cell's feed on its form: its lever pinned around a forced feed of its prompt (`moe_feed_rows`, the census bound to the
//! form's first feed), then the rail's witnesses - the arena off reading no device row, the meet's and the rare rail's
//! counters, the pool's - held into the cell
def private matrix_feed(var c : MatrixCell; m : Model; form : ModelForm; fx : MoeFixture; host_layers : int64; first : bool) {
    let pins = stream_pins()
    rail_pin(c.rail)
    let at = host_rail_at()
    let p0 = pool_at()
    c.gpu <- moe_feed_rows(c.h, m, form, fx, c.n, FED_STEPS, host_layers, first)
    c.fed = true
    if (c.rail == CellRail.planar) {
        c.h |> held_equal(g_rare_dev_rows, at.dev_rows, "the arena off: the rail read no row off the device planes")
    } else {
        let one_row = c.rail == CellRail.plain || c.rail == CellRail.three_pass || c.rail == CellRail.row_core || c.rail == CellRail.host_fill
        witness_since(c.h, at, one_row, c.rail != CellRail.three_pass, "the cell's windows")
        if (c.rail != CellRail.plain && c.rail != CellRail.three_pass && c.rail != CellRail.row_core) {
            pool_witness(c.h, c.rail, p0)
        }
    }
    restore_stream_pins(pins)
}

//! one form's share of a carrier's block: on the device the admission at the form's host layers, the census on the first form
//! where the fixture claims it, each cell of the form fed in its order; on the CPU form the prompts' feeds, once
def private matrix_form_rows(t : T?; var r : MatrixRows; m : Model; form : ModelForm) {
    if (!form.on_gpu) {
        return if (!empty(r.cpu))
        r.cpu |> reserve(length(r.prompts))
        for (n in r.prompts) {
            r.cpu |> emplace <| resident_feed_rows(m, form, n, FED_STEPS)
        }
        return
    }
    let at = cell_form_of(r, form)
    var fx = r.fx
    fx.host_layers_ask = at.layers
    let host_layers = moe_fixture_admitted(r.h, m, fx)
    return if (host_layers < 0l)
    if (fx.census && same_form(at, FOUR_LAYERS)) {
        var inscope any_sites, bound : array<string>
        if (!empty(fx.census_sites)) {
            delete bound
            bound <- split(fx.census_sites, ";")
        }
        census_cell(t, m, path_join(models_dir(), fx.file), "vulkan load", any_sites, bound)
    }
    var first = true
    for (c in r.cells) {
        continue if (c.skipped || !same_form(c.at, at))
        matrix_feed(c, m, form, fx, host_layers, first)
        first = false
    }
}

//! a cell's compares after the block: its device feed against the CPU form's feed of its prompt at the fixture's bar, then
//! what its feed held; a cell its form never fed skips
def private matrix_cell_compares(t : T?; r : MatrixRows; c : MatrixCell) {
    if (c.fed) {
        let what = "{r.fx.file}, {c.n} tokens: "
        let k = find_index(r.prompts, c.n)
        if (twin_collected(t, k, length(r.cpu), what)) {
            with_dasllama_jobque_() {
                feed_rows_within(t, what, c.gpu, r.cpu[k], r.fx.bar_rel)
            }
        }
    } elif (!c.skipped) {
        t |> skip("{r.fx.file}: the cell's form did not run")
    }
    held_within(t, c.h)
}

//! cell `i`'s subtest, its compares under its title
def private matrix_cell_within(t : T?; r : MatrixRows; i : int) {
    unsafe {
        t |> run(r.cells[i].title) @capture(& r, = i)(ct : T ?) {
            matrix_cell_compares(ct, r, r.cells[i])
        }
    }
}

//! a carrier's block: its rows and forms built (`matrix_rows`), `blk` - the test's `with_cpu_and_gpu_model` call over them,
//! whether a device form ran its result - under the matrix's stream pins (the meet's host share, the prompt stream, the
//! arena), then the forms' admissions and each cell's subtest
def private with_matrix(t : T?; tag : string; fx : MoeFixture; set : MatrixCells;
                        blk : block<(var r : MatrixRows; forms : array<ModelForm>) : bool>) {
    var r <- matrix_rows(tag, fx, set)
    var forms <- matrix_forms(r)
    let pins = pin_stream(MOE_HOST_FIRST, pf_stream = 1, arena = 1, plain_steps = false)
    let ran = invoke(blk, r, forms)
    restore_stream_pins(pins)
    held_within(t, r.h, "{fx.file}: ", false)
    if (ran) {
        for (i in range(length(r.cells))) {
            matrix_cell_within(t, r, i)
        }
    }
    delete r
    delete forms
}

//! a matrix carrier's fixture: the format `tag`'s file of form `stem` (`exps` every routed stack, `exps3` three layers'), four host layers asked
def private carrier(tag : string; stem : string = "exps") : MoeFixture => MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-{stem}-{tag}.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)

//! one [test] a format, so a run names one carrier a process (`--test-names test_moe_formats_<TYPE>`): under DASLLAMA_IMAGE=0 a loaded model's planes stay in the heap past its delete, and four carriers in one process exhaust the box
[test(description = "Q2_K routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q2_K(t : T?) {
    with_matrix(t, "Q2_K", carrier("Q2_K"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q2_K.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q3_K routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q3_K(t : T?) {
    with_matrix(t, "Q3_K", carrier("Q3_K"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q3_K.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q4_K routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows, and each rare-rail A/B arm pinned off - the three-pass rail, the row-at-a-time core, the planar planes reading no device row - still holds them; with Q4_K experts on twenty host layers the hot pool's DMA fill, heat-gate share and decode meet each run as their counters show, DMA-off the control, the unasked hot pool fills each slot off the pinned mirrors - three plane copies an upload, no planar read - and the fed steps hold the CPU chain within the routed bar; each form loads once, a pool cell on a form of its own")]
def test_moe_formats_Q4_K(t : T?) {
    with_matrix(t, "Q4_K", carrier("Q4_K"), MatrixCells.every) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_K.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q5_K routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q5_K(t : T?) {
    with_matrix(t, "Q5_K", carrier("Q5_K"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q5_K.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q6_K routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q6_K(t : T?) {
    with_matrix(t, "Q6_K", carrier("Q6_K"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q6_K.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q4_0 routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q4_0(t : T?) {
    with_matrix(t, "Q4_0", carrier("Q4_0"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_0.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q8_0 routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; with Q8_0 experts on twenty host layers, the hot pool's DMA fill, heat-gate share and decode meet each run as their counters show, DMA-off the control, and the fed steps hold the CPU chain within the routed bar; each form loads once, a pool cell on a form of its own")]
def test_moe_formats_Q8_0(t : T?) {   //! the source file itself, linked into the matrix folder under the matrix's naming
    with_matrix(t, "Q8_0", carrier("Q8_0"), MatrixCells.knobs) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q8_0.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "IQ4_XS routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; with IQ4_XS experts on twenty host layers, the hot pool's DMA fill, heat-gate share and decode meet each run as their counters show, DMA-off the control, and the fed steps hold the CPU chain within the routed bar; each form loads once, a pool cell on a form of its own")]
def test_moe_formats_IQ4_XS(t : T?) {
    with_matrix(t, "IQ4_XS", carrier("IQ4_XS"), MatrixCells.knobs) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ4_XS.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

//! the MoE NVFP4 carrier on every run: the Qwen3-30B-A3B `--pure`
//! NVFP4 mint cut to its first seven blocks - every routed stack, the
//! attention q/k/v/o, the token table and the classifier nv4, the
//! router and the norms F32, no shared expert; its feeds hold the
//! routed bar, the CUDA prefill row reading 2.59 against 2.795 on the
//! RTX PRO 4500 (1.08x), its one-step-off control 16.0
let private NV4_CUT = MoeFixture(file = "Qwen3-30B-A3B-7L-NVFP4-pure-local.gguf", shexp = false,
    census = true, fp4_chain = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)
let private NV4_CUT_BLOCKS = 7l

//! the cut's block: the host-pinned form's census, its feeds of
//! `prompts` with the one-copy rail's witnesses, the CPU form's feeds
struct private Nv4CutRows {
    prompts : array<int64>
    h : Held
    gpu : array<FeedRows>
    cpu : array<FeedRows>
    kv : KVDtype
}

//! the FP4 expert chain's planes are sized where the routed block's stacks are, on the CUDA home: under the FP4-chain
//! fault seat (the fault form, `cuda_mmq_fault`) the driver arms, holds no window chain, and a prompt window refuses by name
def private fp4_chain_fault_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("{NV4_CUT.file}: an FP4 expert chain whose planes found no room leaves the driver armed, and a prompt window refuses by name") @capture(& m)(t : T ?) {
            if (!cuda_fp4_chain_served()) {
                t |> skip("the card sits under the FP4 rung, or DASLLAMA_CUDA_MMQ=0 stands the route down: no routed stack rides the FP4 expert chain, so the seat has no planes to refuse")
                return
            }
            t |> success(moe_gpu_resident_active(), "under the seat the whole-model driver still arms (the load log names any decline)")
            t |> success(!cuda_window_planes_held(), "... holding no window chain: the FP4 expert chain's planes are part of it")
            var inscope prompt <- resident_prompt(40l)
            var inscope toks <- resident_feed(2l)
            var s <- create_session(m, mirror_kv_dtype())
            var why = ""
            try {
                var rows <- feed_logits(m, s, prompt, toks)
                delete rows
            } recover {
                why = this_context().last_exception
            }
            t |> success(find(why, "found no room on the device when the driver armed") >= 0, "a prompt window refuses by name: '{why}'")
            delete s
        }
    }
}

//! one form's share: on the device the cut's blocks, the admission,
//! the census, each prompt's feed - the meet handing the host experts,
//! the rare rail reading the window's rows and the fed steps' picks off
//! the device planes; on the CPU the same prompts on the device's codec;
//! the fault form's share is its refusal cell alone, and the form after
//! it holds the window chain again
def private nv4_cut_rows(t : T?; var r : Nv4CutRows; var m : Model; form : ModelForm) {
    if (form.cuda_mmq_fault) {
        fp4_chain_fault_cell(t, m)
        return
    }
    if (!form.on_gpu) {
        return if (form.kv != r.kv || empty(r.gpu))
        r.cpu |> reserve(length(r.prompts))
        for (n in r.prompts) {
            r.cpu |> emplace <| resident_feed_rows(m, form, n, FED_STEPS)
        }
        return
    }
    r.kv = form.kv
    if (gpu_want_cuda()) {
        r.h |> held_fact(cuda_window_planes_held(), "the fault seat lifted, the next load holds its window chain before any prompt (the fault form's control)")
    }
    cut_held(r.h, m, NV4_CUT_BLOCKS)
    let host_layers = moe_fixture_admitted(r.h, m, NV4_CUT)
    return if (host_layers < 0l)
    var inscope any_sites, bound : array<string>
    census_cell(t, m, path_join(models_dir(), NV4_CUT.file), "vulkan load", any_sites, bound)
    r.gpu |> reserve(length(r.prompts))
    for (n in r.prompts) {
        let at = host_rail_at()
        r.gpu |> emplace <| moe_feed_rows(r.h, m, form, NV4_CUT, n, FED_STEPS, host_layers, empty(r.gpu))
        witness_since(r.h, at, true, true, "the {n}-token feed's windows")
    }
}

[test(description = "Qwen3-30B-A3B-7L NVFP4-pure, every weight site nv4 on the seven-block cut, with four of its seven routed layers on the host reads every site in its own format, and the one-copy host rail plus the nv4 kernels hold the CPU chain within the routed bar over one and two windows; on the Metal home's blob twin every routed stack loads as nv4 with every site native and the window and six steps ride the nv4 routed stamps and GEMV within 3.0 of the portable CPU chain; each form loads once")]
def test_moe_formats_NVFP4_30B_cut(t : T?) {
    return if (!family_on(t, "qwen3moe"))
    var r : Nv4CutRows
    r.prompts <- window_prompts(r.h)
    var forms : array<ModelForm>
    let why = codec_env_why(KVDtype.f16)
    if (!empty(why)) {
        r.h |> held_skip("the cut's f16 mirror: {why}")
    } else {
        // the fault form first: the form after it is the load that
        // finds the seat lifted
        forms_or_skips(t, forms, NV4_CUT.file,
            [device_form(regions = 1l, host_layers = NV4_CUT.host_layers_ask, cuda_mmq_fault = true) |> asking(KVDtype.f16)],
            cuda_home_why())
        forms |> push(device_form(regions = 1l, host_layers = NV4_CUT.host_layers_ask) |> asking(KVDtype.f16))
    }
    let census = format_census_nv4_moe()
    let census_cpu = cpu_cells_form(backend = "portable")
    var census_kept : CensusKept
    if (any_arm_on([census.arm])) {
        forms |> push(metal_form(ctx = CENSUS_CELLS_CTX))
        if (empty(metal_build_why())) {
            forms |> push(census_cpu)
        }
    }
    let pins = pin_stream(MOE_HOST_FIRST, pf_stream = 1, arena = 1, plain_steps = false)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-30B-A3B-7L-NVFP4-pure-local.gguf", MOE_CUT_CTX, forms) $(var m, form) {
            if (form.on_metal) {
                format_census_metal(t, m, form, census, census_kept)
            } elif (!form.on_gpu && same_cpu_load(form, census_cpu)) {
                format_census_cpu(t, m, form, census, census_kept)
            } else {
                nv4_cut_rows(t, r, m, form)
            }
        }
    }
    restore_stream_pins(pins)
    format_census_verdict(t, census, census_kept)
    held_within(t, r.h, "{NV4_CUT.file}: ", false)
    if (ran) {
        with_dasllama_jobque_() {
            feeds_within(t, "{NV4_CUT.file}, ", r.gpu, r.cpu, NV4_CUT.bar_rel)
        }
    }
    delete r
    delete forms
}

//! the carrier is the 35B NVFP4 file itself (the qwen35moe-35b category): its routed stacks reach the nv4 row-major kernels under the per-tensor scale word
[test(description = "Qwen3.6-35B NVFP4 with four host layers reads its attention and expert sites in nv4 under the per-tensor scale, and the host rail plus nv4 kernels hold the CPU chain within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_NVFP4_35B(t : T?) {
    //! the census binds the file's nv4 sites; its BF16 deltanet planes, table and classifier serve as q8 by the tier's design
    let fx = MoeFixture(file = "Qwen3.6-35B-A3B-NVFP4.gguf", shexp = true, census = true, fp4_chain = true, dn_chunk = true,
        census_sites = "attention q/k/v/o;routed experts;shared expert", bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)
    with_matrix(t, "NVFP4 (Qwen3.6-35B-A3B)", fx, MatrixCells.formats) $(var r, forms) {
        var ran = false
        with_cuda_mmq_winners() {   //! the FP4 chain's census reads the MMQ route: pinned to the winners, so `DASLLAMA_CUDA_MMQ` cannot stand it down
            ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.6-35B-A3B-NVFP4.gguf", MATRIX_CTX, forms) $(m, form) {
                matrix_form_rows(t, r, m, form)
            }
        }
        return ran
    }
}

[test(description = "IQ4_NL routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_IQ4_NL(t : T?) {
    with_matrix(t, "IQ4_NL", carrier("IQ4_NL"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ4_NL.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "IQ3_XXS routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_IQ3_XXS(t : T?) {
    with_matrix(t, "IQ3_XXS", carrier("IQ3_XXS"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ3_XXS.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "IQ3_S routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_IQ3_S(t : T?) {
    with_matrix(t, "IQ3_S", carrier("IQ3_S"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ3_S.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "IQ2_S routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_IQ2_S(t : T?) {
    with_matrix(t, "IQ2_S", carrier("IQ2_S"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ2_S.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "IQ2_XS routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_IQ2_XS(t : T?) {
    with_matrix(t, "IQ2_XS", carrier("IQ2_XS"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ2_XS.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "IQ2_XXS routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_IQ2_XXS(t : T?) {
    with_matrix(t, "IQ2_XXS", carrier("IQ2_XXS"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-IQ2_XXS.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q5_0 routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q5_0(t : T?) {
    with_matrix(t, "Q5_0", carrier("Q5_0"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q5_0.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

[test(description = "Q5_1 routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_Q5_1(t : T?) {
    with_matrix(t, "Q5_1", carrier("Q5_1"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q5_1.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

//! a 1-bit carrier's K-quant native CPU form: its cell reads layer 3's routed gate and up stacks off their own bytes
def private lowbit_sites_form() : ModelForm => cpu_cells_form(kq_native = KqNative.on_q40)

//! the matrix's forms of a low-bit carrier, under `sites` its K-quant native CPU form (`lowbit_sites_form`), and where the
//! arm filter reaches the Metal census row `cs` the blob twin and, on a das_metal build, the row's CPU form
def private lowbit_carrier_forms(forms : array<ModelForm>; cs : CensusRow; sites : bool) : array<ModelForm> {
    var all := forms
    if (sites) {
        all |> push(lowbit_sites_form())
    }
    if (any_arm_on([cs.arm])) {
        all |> push(metal_form(ctx = CENSUS_CELLS_CTX))
        if (empty(metal_build_why())) {
            all |> push(census_cpu_form(cs))
        }
    }
    return <- all
}

//! one form's rows of a low-bit member's routed carrier (member `f` on the gate and up stacks of three layers), under the member's
//! census: the device form ran its stamps, a CPU form none; on the K-quant native CPU form (`lowbit_sites_form`) layer 3's routed
//! gate and up stacks serve their own bytes (`kq_expert_sites_cell`); on the blob twin the Metal census row `cs`, its controls
//! on the row's CPU form
def private lowbit_matrix_rows(t : T?; var r : MatrixRows; var m : Model; form : ModelForm; cs : CensusRow;
                               var kept : CensusKept) {
    return if (format_census_pass(t, m, form, cs, kept))
    let f = cs.f
    lowbit_census_ran(t, f, form, "the {f} gate and up stacks") {
        if (!alone_pass_of(form, lowbit_sites_form())) {
            matrix_form_rows(t, r, m, form)
            return
        }
        let path = path_join(models_dir(), r.fx.file)
        unsafe {   //! the cell reads the block's model by reference
            t |> run("{r.fx.file}: layer 3's routed gate and up stacks serve their own {f} bytes on the CPU") @capture(& m)(t : T ?) {
                kq_expert_sites_cell(t, m, path, f, 3l, false)
            }
        }
    }
}

[test(description = "TQ2_0 routed gate and up experts on layers 3, 11 and 19 (their down stacks the file's Q4_0, the 1408-wide rows off the 256 lattice; every other routed stack Q8_0) under four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows, the device form counting the tq2 stamps and a CPU form none of them; on the Metal blob twin (arm `census-moe-tq2`) the window and its decode steps hold the CPU chain's reading those stacks off their own bytes, the tq2 stacks on their routed GEMVs and, under the family's crown, their tensor twins; each form loads once")]
def test_moe_formats_TQ2_0(t : T?) {
    let cs = format_census_moe_lowbit("moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-TQ2_0.gguf", KqFmt.tq2, 6l)
    var kept : CensusKept
    with_matrix(t, "TQ2_0", carrier("TQ2_0", "exps3"), MatrixCells.formats) $(var r, forms) {
        var inscope all <- lowbit_carrier_forms(forms, cs, false)
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-TQ2_0.gguf", MATRIX_CTX, all) $(var m, form) {
            lowbit_matrix_rows(t, r, m, form, cs, kept)
        }
    }
    format_census_verdict(t, cs, kept)
}

[test(description = "TQ1_0 routed gate and up experts on layers 3, 11 and 19 (their down stacks the file's Q4_0, the 1408-wide rows off the 256 lattice; every other routed stack Q8_0) under four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows, the device form counting the tq1 stamps and a CPU form none of them; on the Metal blob twin (arm `census-moe-tq1`) the window and its decode steps hold the CPU chain's reading those stacks off their own bytes, the tq1 stacks on their routed GEMVs and, under the family's crown, their tensor twins; each form loads once")]
def test_moe_formats_TQ1_0(t : T?) {
    let cs = format_census_moe_lowbit("moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-TQ1_0.gguf", KqFmt.tq1, 6l)
    var kept : CensusKept
    with_matrix(t, "TQ1_0", carrier("TQ1_0", "exps3"), MatrixCells.formats) $(var r, forms) {
        var inscope all <- lowbit_carrier_forms(forms, cs, false)
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-TQ1_0.gguf", MATRIX_CTX, all) $(var m, form) {
            lowbit_matrix_rows(t, r, m, form, cs, kept)
        }
    }
    format_census_verdict(t, cs, kept)
}

[test(description = "IQ1_S routed gate and up experts on layers 3, 11 and 19 (their down stacks the file's IQ4_NL, the 1408-wide rows off the 256 lattice; every other routed stack Q8_0) under four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows, the device form counting the iq1s stamps and a CPU form none of them; on the K-quant native CPU form, on every box, the routed sites load native - layer 3's gate and up stacks as iq1s - and their expert 0 rows match the file rows' dot within f32 reassociation; on the Metal blob twin (arm `census-moe-iq1s`) the window and its decode steps hold the CPU chain's reading those stacks off their own bytes, the iq1s stacks on their routed GEMVs and, under the family's crown, their tensor twins; each form loads once")]
def test_moe_formats_IQ1_S(t : T?) {
    let cs = format_census_moe_lowbit("moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-IQ1_S.gguf", KqFmt.iq1s, 6l)
    var kept : CensusKept
    with_matrix(t, "IQ1_S", carrier("IQ1_S", "exps3"), MatrixCells.formats) $(var r, forms) {
        var inscope all <- lowbit_carrier_forms(forms, cs, true)
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-IQ1_S.gguf", MATRIX_CTX, all) $(var m, form) {
            lowbit_matrix_rows(t, r, m, form, cs, kept)
        }
    }
    format_census_verdict(t, cs, kept)
}

[test(description = "IQ1_M routed gate and up experts on layers 3, 11 and 19 (their down stacks the file's IQ4_NL; every other routed stack Q8_0) under four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows, the device form counting the iq1m stamps and a CPU form none of them; on the K-quant native CPU form, on every box, the routed sites load native - layer 3's gate and up stacks as iq1m - and their expert 0 rows match the file rows' dot within f32 reassociation; on the Metal blob twin (arm `census-moe-iq1m`) the window and its decode steps hold the CPU chain's reading those stacks off their own bytes, the iq1m stacks on their routed GEMVs and, under the family's crown, their tensor twins; each form loads once")]
def test_moe_formats_IQ1_M(t : T?) {
    let cs = format_census_moe_lowbit("moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-IQ1_M.gguf", KqFmt.iq1m, 6l)
    var kept : CensusKept
    with_matrix(t, "IQ1_M", carrier("IQ1_M", "exps3"), MatrixCells.formats) $(var r, forms) {
        var inscope all <- lowbit_carrier_forms(forms, cs, true)
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps3-IQ1_M.gguf", MATRIX_CTX, all) $(var m, form) {
            lowbit_matrix_rows(t, r, m, form, cs, kept)
        }
    }
    format_census_verdict(t, cs, kept)
}

[test(description = "MXFP4 routed experts on four host layers, read off the device planes through the one-copy rare rail, hold the CPU chain's logits within the routed bar over one and two windows; the form loads once")]
def test_moe_formats_MXFP4(t : T?) {
    with_matrix(t, "MXFP4", carrier("MXFP4"), MatrixCells.formats) $(var r, forms) {
        return with_cpu_and_gpu_model(t, "llm", "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-MXFP4.gguf", MATRIX_CTX, forms) $(m, form) {
            matrix_form_rows(t, r, m, form)
        }
    }
}

//! the expert cache at half: twenty of the Q4_K file's routed layers streamed under a budget keeping about half of each layer's experts resident, the misses read off the lane into the ring - the fed steps' picks against the CPU chain, the on-demand reads the witness
[test(description = "Under a 3500 MB expert cache over twenty Q4_K host layers the misses read off the lane on demand within the CPU chain's routed bar, and a second feed with the saved heat profile reads fewer planes")]
def test_moe_formats_expert_cache(t : T?) {
    own_load("reloads the model: the Q4_K file loaded twice through the image rail, the first load's drop writing beside its device lane the heat profile the second load reads")
    let fx = MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_K.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 20l, image = true)
    t |> run("Q4_K experts on twenty host layers under a 3500 MB expert cache: the misses read off the lane, the token command vs the CPU chain") @(ct : T?) {
        if (!g_env_engine.image) {
            ct |> skip("the expert cache reads its misses off the lane file, and DASLLAMA_IMAGE=0 mints none")
            return
        }
        let pins = pin_stream(MOE_HOST_FIRST, pf_stream = 1, arena = 1, plain_steps = false)
        let cache_was = expert_cache_mb_pin()
        set_expert_cache_mb(3500l)
        let home = gpu_want_cuda() ? "cuda" : "vulkan"
        remove("{path_join(models_dir(), fx.file)}.{home}.dlim.heat")   //! an earlier run's profile would seat the first feed's hottest experts already
        let trim_pin = vulkan_trim_pin()   //! the lane mints untrimmed: the cell's reference pass is the all-CPU chain, which reads the planar planes
        set_vulkan_trim(false)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let f0 = moe_gpu_stream_cache_stats().fetches
            let p0 = gpu_resident_prefills_()
            witnessed(ct, true, true) {
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0) {
                let reads1 = moe_gpu_stream_cache_stats().fetches - f0
                ct |> success(reads1 > 0l, "the cache read {reads1} planes off the lane on demand over the cell")
                //! the drop wrote the picks' profile beside the lane: the same feed again keeps each layer's hottest experts resident and misses less
                let f1 = moe_gpu_stream_cache_stats().fetches
                moe_forced_feed(ct, 40l, fx)
                let reads2 = moe_gpu_stream_cache_stats().fetches - f1
                ct |> success(reads2 < reads1, "the profile-ordered cache read {reads2} planes on demand against {reads1} with the first ids resident")
            }
        }
        restore_vulkan_trim(trim_pin)
        set_expert_cache_mb(cache_was)
        restore_stream_pins(pins)
    }
}
