// The MoE cells of the whole-model resident driver, shared by the per-model test files
// (test_gpu_resident_qwen_moe.das, test_gpu_resident_gptoss.das): the forced-feed logits-tolerance form of
// test_gpu_resident_hybrid - the same fixed tokens through the resident driver and the all-CPU chain,
// the prefill logits and every step's within the bar, the one-step-off control past it, and the census
// witnesses that the routed block ran on the device for every MoE layer (the window's per window, the
// token command's once at its recording).

options gen2
options _dasllama_internal = true

module _moe_resident shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2moe, gptoss)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the MoE
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 — the grid helpers, read under the vulkan static_if, unused in the darwin world
require dasllama/dasllama_moe   // heat_totals, heat_reset and the shared expert's row counters: the per-op heat cell's witnesses
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode and the kernel census, where the module exists
require ?cuda dasllama/dasllama_cuda_decode   // the same witnesses on the CUDA home
require ?cuda dasllama/dasllama_cuda_stream   // cud_stream_stats: the host-layers forms' stream witnesses
require ?cuda _cuda_feed   // stream_at, stream_witness: the CUDA home's host-layers cells
require dasllama/dasllama_kqformat
require dasllama/dasllama_env   // the chunked scan's knob, the census's gate
require daslib/enum_trait   // the KqFmt members as a loop
require daslib/strings_boost   // split: a fixture's census sites
require _model_tier public   // models_dir() + model_available() presence/size-tier gate
require _resident_feed public   // resident_ready, load_resident, resident_prompt, resident_then_cpu_feed, feed_steps_within, tile_arm_tag
require _format_census   // census_cell: a census fixture's every site in its own format
require _cpu_gpu_model   // ModelForm, FeedRows, feed_rows: the forced feed's collector form

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}

//! a fixture file, whether it carries a shared expert, its logits bar as a fraction of the step's max |logit|, and the keys its
//! sliding layers attend (0 = no sliding layers claimed)
struct MoeFixture {
    file : string
    shexp : bool
    bar_rel : float
    window : int64 = 0l
    expect_host_layers : bool = false   //! the card does not hold the experts whole: the driver arms with some routed layers' experts on the host
    strict_ctx : int64 = 0l      //! > 0: the load pins this context whole (`set_gpu_ctx_max` + `set_gpu_ctx_strict`), and the armed region holds it
    census : bool = false        //! every site the file carries read in its own format (`census_cell`): a file whose planes all sit on their formats' lattices
    census_sites : string        //! the sites the census binds to their own format, `;`-separated; empty = every site (a carrier whose BF16 planes serve as q8 names its own-bytes sites)
    image : bool = false         //! the load goes through the prepared lane (minted beside the file on the first load): the cells that read off it
    host_layers_ask : int64 = 0l //! > 0: the load asks at least this many routed layers' experts onto the host (`set_gpu_host_layers`) whatever the card holds - the host-layers form on any card
    fp4_chain : bool = false     //! every routed stack mx4 or nv4: on the CUDA home at the rung the census holds the FP4 expert chain's m stamps to the window's pieces, not the cm2 e tiles alone
    dn_chunk : bool = false      //! a deltanet carrier: where the home has the tiles and the knob is on, the census holds the chunked scan's stamp to the windows
}

//! the chunked deltanet scan serves the window on this home: the knob on, and on the Vulkan home the KHR tiles at subgroup 32
//! under a served set that leaves the scan on the chunked stamp
def private dn_chunk_served() : bool {
    return false if (g_env_vulkan.dn_chunk == 0l)
    return true if (gpu_want_cuda())
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_dn_chunk_wanted()
    } else {
        return false
    }
}

//! the FP4 expert chain's m stamps: the CUDA home's census alone - the Vulkan census seeds no such name and panics on a read of it
def private fp4_census() : int64 {
    return 0l if (!gpu_want_cuda())
    return max(0l, census("mx4_fp4_mmq_moe_cuda")) + max(0l, census("nv4_fp4_mmq_moe_cuda"))
}

//! the CUDA FP4 expert chain serves the window's routed pieces on this box
def private fp4_chain_served() : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return gpu_want_cuda() && cuda_fp4_chain_served()
    } else {
        return false
    }
}

//! the logits bar as a fraction of the step's max |logit|: the routed chain's CPU-vs-device noise class (test_gpu_resident_qwen_moe.das's
//! reading) - the arms agree to 1e-8 at layer 0 and part from layer 1 on, where the device chain's summation order moves the
//! residual by ulps, the next layer's Q8 requant rounds a few elements the other way and the router flips a near-tie; against
//! the all-CPU chain that reads 0.07-0.11 on the twin and the 30B at 40 and 600 tokens and 0.09-0.16 on the 35B, whose
//! two-window step 4 sits on a router near-tie that any change of the device's summation order flips (0.04 under one order,
//! 0.155-0.16 under two others); the one-step-off control reads 0.42-0.95, so the bar discriminates it by 2x and more
let MOE_LOGIT_BAR_REL = 0.20
//! ... on a device whose compiler rounds a shader at a time (`home_rounds_alike`): on the Radeon 8060S the Qwen3-30B
//! seven-block cut's 40-token prefill row reads past the bar above under every alternate set, its argmax across a
//! near-tie; the one-step-off control reads well past this bar
let MOE_LOGIT_BAR_REL_APART = 0.25

//! the routed bar a carrier's device feeds hold against the CPU chain on the armed home - read while the device is up: a
//! compare made after the load is down sees no device
def moe_carrier_bar_rel() : float => home_rounds_alike() ? MOE_LOGIT_BAR_REL : MOE_LOGIT_BAR_REL_APART

//! the rarest experts the meet hands the host before the device
//! claims: the host's rows on every carrier
let MOE_HOST_FIRST = 8l
//! the MoE cuts' mirror context
let MOE_CUT_CTX = 8192l
let FED_STEPS = 6l

//! the token command's attention split count on the armed discrete home (0 without a module or the driver)
def private attn_nsplit : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_attn_nsplit() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_attn_nsplit()
    }
    return 0l
}

//! the tokens the token command's unsplit twin served so far, on the armed discrete home
def private unsplit_tokens : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_unsplit_tokens() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_unsplit_tokens()
    }
    return 0l
}

let private UNSPLIT_UNDECIDED = -1l   //! the steps straddle the twin's threshold, or no vulkan module: the witness stands down

//! the steps of `steps` fed after an `nprompt`-row prompt the
//! unsplit twin must serve: every one under RD_UNSPLIT_POS, none past
def private unsplit_want(nprompt, steps : int64) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return steps if (nprompt + steps <= RD_UNSPLIT_POS)
        return nprompt >= RD_UNSPLIT_POS ? 0l : UNSPLIT_UNDECIDED
    } else {
        return UNSPLIT_UNDECIDED
    }
}

//! the census over every `KqFmt` member the discrete homes stamp, one tile column - `cm2e` (the expert schedule's m pieces in cm2 mode; a 64-step
//! format's e column is its m stamp, KQ_CM2E_ALIASES_M), `khr` (every piece in mm mode), `sdot4` (every piece on the Q8-fed chain's
//! batch tiles) or `cuda_e` (the CUDA home's cm2 e tiles plus the FP4 expert chain's m stamps) - in the format the box serves the file's planes
def private column_census(col : string) : int64 {
    if (col == "cuda_e") {
        return column_census("cm2e") + fp4_census()
    }
    var n = 0l
    for (f in type<KqFmt>) {
        continue if (kq_discrete_unstamped(f))
        let name = "{f}"
        if (col == "sdot4") {
            n += census("{kq_tile_stem(name)}_cls_spv")
        } else {
            n += census("{kq_tile_stem(name)}_{col == "cm2e" && kq_cm2e_aliases_m(name) ? "cm2m" : col}_cls_spv")
        }
    }
    return n
}

//! the expert pieces' tile column on this device, and the dispatches it takes per MoE layer per window: the e column's one m dispatch a
//! plane in cm2 mode; in mm mode the KHR tile takes both the s and the m dispatch of every plane. Either way the count is a floor: the
//! KHR tile and the 64-step formats' m stamp both serve the window's dense GEMMs too
def private expert_column() : tuple<col : string; per_plane : int64> {
    if (gpu_want_cuda()) {   // the cm2 e tile or the FP4 expert chain's m stamp (mx4 / nv4 stacks under the knob): one counted dispatch a plane either way
        return (col = "cuda_e", per_plane = 1l)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!device_has_expert_tiles()) {
            return (col = "sdot4", per_plane = 1l)
        }
        return g_gpu.coopmat_mode == COOPMAT_MM ? (col = "khr", per_plane = 2l) : (col = "cm2e", per_plane = 1l)
    } else {
        return (col = "cm2e", per_plane = 1l)
    }
}

//! the run's home has the f16-fed expert chain a window's meet claims regions through: the CUDA home's, the Vulkan home's on an
//! NV_coopmat2 device
def home_has_meet_chain() : bool {
    return true if (gpu_want_cuda())
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu != null && g_gpu.has_coopmat2
    } else {
        return false
    }
}

//! the device carries an f16-fed tile family for the routed block's expert pieces: the cm2 s and e stamps in cm2 mode, the KHR
//! 128 x 128 tile in mm mode at subgroup 32; without either the Q8-fed chain's batch tiles serve them
def private device_has_expert_tiles() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu != null && ((g_gpu.has_coopmat2 && g_gpu.coopmat_mode == COOPMAT_CM2) || (g_gpu.coopmat_mode == COOPMAT_MM && khr_kq_tile_on()))
    } else {
        return false
    }
}

//! the driver's admission of the fixture and the host-layer count it armed, against the fixture's expectation;
//! -1 where the cell ends here - a skip the box earns, or the red that names the load log
def moe_fixture_admitted(t : T?; tr : Model; fx : MoeFixture) : int64 {
    var host_layers = -1l
    held_on(t) $(var h) {
        host_layers = moe_fixture_admitted(h, tr, fx)
    }
    return host_layers
}

//! `moe_fixture_admitted` read now, held into `h`
def moe_fixture_admitted(var h : Held; tr : Model; fx : MoeFixture) : int64 {
    h |> held_fact(tr.config.n_expert > 0l && (tr.config.n_ff_shexp > 0l) == fx.shexp,
        "the fixture routes experts {fx.shexp ? "beside" : "with no"} shared expert")
    if (fx.window > 0l) {
        h |> held_equal(tr.config.sliding_window, fx.window,
            "the fixture's sliding layers attend {fx.window} keys")
    }
    if ((fx.expect_host_layers || fx.host_layers_ask > 0l) && !moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        h |> held_skip(
            "the card holds neither the mirror asked whole ({fx.strict_ctx}) nor the dense " +
            "planes beside it - the host-layer form needs a larger card (the load log names " +
            "the room)")
        return -1l
    }
    h |> held_fact(moe_gpu_resident_active(),
        "the whole-model driver took {fx.file} (a decline names its reason in the load log: " +
        "the plan, a seat, a plane's format)")
    return -1l if (!moe_gpu_resident_active())
    mirror_codec_held(h)
    let host_layers = gpu_resident_host_layers()
    if (fx.expect_host_layers && host_layers == 0l) {
        h |> held_skip(
            "the card holds every expert stack beside the {fx.strict_ctx}-position mirror - " +
            "the host-layer form needs a smaller card")
        return -1l
    }
    if (fx.host_layers_ask > 0l) {   //! the ask is a floor: a card that holds the rest moves exactly that many, one that does not moves more
        h |> held_fact(host_layers >= fx.host_layers_ask && host_layers < tr.config.n_layers,
            "at least the first {fx.host_layers_ask} routed layers' experts serve on the host " +
            "as the fixture asked (set_gpu_host_layers), and fewer than all " +
            "{tr.config.n_layers}: {host_layers} do")
    } elif (fx.expect_host_layers) {
        h |> held_fact(host_layers < tr.config.n_layers,
            "the card does not hold the experts whole: the first {host_layers} of " +
            "{tr.config.n_layers} routed layers' experts serve on the host, the rest on the " +
            "device")
    } else {
        h |> held_equal(host_layers, 0l,
            "every expert on the device (the cell pins ctx_max {fx.strict_ctx} and strict " +
            "{fx.strict_ctx > 0l}; DASLLAMA_GPU_CTX_MAX / DASLLAMA_GPU_CTX_STRICT in the " +
            "environment override the pins)")
    }
    if (fx.strict_ctx > 0l) {
        h |> held_equal(gpu_device_session_ctx(), fx.strict_ctx,
            "the context asked whole is the region's: the experts yielded, not the mirror")
    }
    return host_layers
}

//! prefill `n` tokens + the fed steps on the resident driver, then the same on the all-CPU chain; the
//! prefill logits and every step's within the bar, the one-step-off control past it, the routed
//! block's census growing by the MoE layer count per window and per step
[cold_path]
def moe_forced_feed(t : T?; n : int64; fx : MoeFixture) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (fx.fp4_chain && gpu_want_cuda()) {   //! the FP4 chain's census assert reads the MMQ route: pinned to the winners, so `DASLLAMA_CUDA_MMQ` cannot stand it down
            with_cuda_mmq_pin(CUDA_MMQ_WINNERS) {
                moe_forced_feed_pinned(t, n, fx)
            }
            return
        }
    }
    moe_forced_feed_pinned(t, n, fx)
}

[cold_path]
def private moe_forced_feed_pinned(t : T?; n : int64; fx : MoeFixture) {
    let path = path_join(models_dir(), fx.file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident_host(path, 1l, fx.strict_ctx, fx.host_layers_ask, fx.image)   //! one region: the context asked whole is the region's
    let host_layers = moe_fixture_admitted(t, tr, fx)
    if (host_layers < 0l) {
        delete tr
        return
    }
    if (fx.census) {
        var inscope any_sites, bound : array<string>
        if (!empty(fx.census_sites)) {
            delete bound
            bound <- split(fx.census_sites, ";")
        }
        census_cell(t, tr, path, "vulkan load", any_sites, bound)
    }
    with_dasllama_jobque_() {
        let at = moe_census_at(tr, host_layers)
        forced_feed_over(t, tr, "", n, FED_STEPS, fx.bar_rel) $ {
            held_on(t) $(var h) {
                moe_census_rows(h, at, fx, n, FED_STEPS)
            }
        }
    }
    delete tr
}

//! the routed block's counters as a forced feed found them
//! (`moe_census_rows` reads what moved): the keys of the model's
//! expert count, the expert pieces' tile column, the routed layers
//! the admission found on the host and the device-routed rest, and
//! each count
struct private MoeCensusAt {
    sched_key : string
    rows_key : string
    topk_key : string
    col : string
    per_plane : int64
    host_layers : int64
    device_routed_layers : int64   //! a host layer runs its router alone
    sched : int64
    rows : int64
    topk : int64
    pieces : int64   //! the expert pieces' dispatches on the tile column
    fp4 : int64
    chunk : int64
    unsplit : int64
}

//! the chunked deltanet scan's stamp; `census` reads the CUDA home's
//! twin under its own name
let private DN_CHUNK_KEY = "dn_scan_chunk_cls_spv"

//! the bucket schedule's dispatches under `key` and under its slot-row-order twin, the stamp a layer with int8 expert
//! tiles takes (`moe_sched_enc`'s `ordered`); the plain count alone on a home with no such twin
def private sched_census(key : string) : int64 {
    let plain = census(key)
    let ordered = census(replace(key, "_cls_spv", "_ord_cls_spv"))
    return (plain == NO_CENSUS || ordered == NO_CENSUS) ? plain : plain + ordered
}

def private moe_census_at(tr : Model; host_layers : int64) : MoeCensusAt {
    let deep = tr.config.n_expert > 256l   //! the deep twins serve past 256 experts
    let ecol = expert_column()
    var at = MoeCensusAt(sched_key = deep ? "moe_sched2_cls_spv" : "moe_sched_cls_spv",
        rows_key = deep ? "topk_rows2_cls_spv" : "topk_rows_cls_spv",
        topk_key = deep ? "topk_n2_cls_spv" : "topk_n_cls_spv",
        col = ecol.col, per_plane = ecol.per_plane, host_layers = host_layers,
        device_routed_layers = tr.config.n_layers - host_layers)
    at.sched = sched_census(at.sched_key)
    at.rows = census(at.rows_key)
    at.topk = census(at.topk_key)
    at.pieces = column_census(ecol.col)
    at.fp4 = fp4_census()
    at.chunk = census(DN_CHUNK_KEY)
    at.unsplit = unsplit_tokens()
    return at
}

//! the routed block's census over an `n`-row prompt and `steps` fed
//! steps since `at`, held into `h`: the bucket schedule and the
//! per-row select once per device-routed layer per window, the
//! token command's top-k recorded per device-routed layer, the
//! expert pieces on the tile column (and the FP4 chain's or the
//! chunked scan's stamps where the fixture claims them), and on a
//! split device with every expert on the device the unsplit twin's
//! share of the steps
def private moe_census_rows(var h : Held; at : MoeCensusAt; fx : MoeFixture; n, steps : int64;
                            first : bool = true) {
    let windows = prompt_windows(n)
    let unsplit = unsplit_want(n, steps)
    //! a split device records the unsplit twin: it serves the steps
    //! under RD_UNSPLIT_POS and none past it
    if (attn_nsplit() > 1l && unsplit != UNSPLIT_UNDECIDED && at.host_layers == 0l) {
        h |> held_equal(unsplit_tokens() - at.unsplit, unsplit,
            "the token command's unsplit twin served {unsplit} of the {steps} steps after {n} " +
            "rows")
    }
    return if (at.sched == NO_CENSUS)
    h |> held_equal(sched_census(at.sched_key) - at.sched, at.device_routed_layers * windows,
        "the device bucket schedule ({at.sched_key} or its slot-row-order twin) ran once per device-routed layer per " +
        "window ({at.device_routed_layers} layers, {windows} windows)")
    h |> held_equal(census(at.rows_key) - at.rows, at.device_routed_layers * windows,
        "the window's per-row select ({at.rows_key}) ran once per device-routed layer per " +
        "window")
    //! the token command records once and resubmits: the census counts its recording, one top-k per MoE layer per recorded form (a split device records the unsplit twin too)
    let topk = census(at.topk_key) - at.topk
    //! a load's first feed records it; a later feed on the load
    //! records at most a form the first never reached
    let recorded = !first || topk >= at.device_routed_layers
    h |> held_fact(recorded && topk % at.device_routed_layers == 0l,
        "the token command recorded a top-k per device-routed layer ({topk} over " +
        "{at.device_routed_layers} layers{first ? "" : ", a later feed of the load"})")
    let edelta = column_census(at.col) - at.pieces
    let ewant = 3l * at.per_plane * at.device_routed_layers * windows
    if (fx.fp4_chain && fp4_chain_served()) {   //! the FP4 chain's own stamps, apart from the cm2 e tiles the column counts beside them
        let fdelta = fp4_census() - at.fp4
        h |> held_fact(fdelta >= ewant,
            "the FP4 expert chain's m stamps served the window's routed pieces ({fdelta} over " +
            "a floor of {ewant}): the stacks on the tensor rung, not the cm2 tiles")
    }
    if (fx.dn_chunk && dn_chunk_served()) {
        let cdelta = census(DN_CHUNK_KEY) - at.chunk
        h |> held_fact(cdelta >= windows,
            "the chunked deltanet scan ({DN_CHUNK_KEY}) served the recurrent layers of every " +
            "window ({cdelta} dispatches over {windows} window(s))")
    }
    h |> held_fact(edelta >= ewant,
        "the expert schedule's pieces dispatched the {at.col} column at least {at.per_plane} " +
        "a plane per device-routed layer per window ({edelta} over a floor of {ewant})")
}

//! the forced feed's device side on a form the caller's block armed:
//! an `n`-token `resident_prompt` and `steps` fed tokens through a
//! fresh session of the form's codec (`resident_feed_rows`), the
//! routed block's census held into `h` as `moe_forced_feed` holds it,
//! `host_layers` the routed layers the admission found on the host,
//! `first` whether the feed is the load's first
def moe_feed_rows(var h : Held; m : Model; form : ModelForm; fx : MoeFixture;
                  n, steps, host_layers : int64; first : bool) : FeedRows {
    let at = moe_census_at(m, host_layers)
    var rows <- resident_feed_rows(m, form, n, steps)
    moe_census_rows(h, at, fx, n, steps, first)
    return <- rows
}

//! the forced feed of each of `prompts` onto `feeds` (`moe_feed_rows`, `FED_STEPS` fed tokens), the census bound to the
//! first feed `feeds` takes
def moe_prompt_feeds(var feeds : array<FeedRows>; var h : Held; m : Model; form : ModelForm; fx : MoeFixture;
                     prompts : array<int64>; host_layers : int64) {
    feeds |> reserve(length(prompts))
    for (n in prompts) {
        feeds |> emplace <| moe_feed_rows(h, m, form, fx, n, FED_STEPS, host_layers, empty(feeds))
    }
}

//! where the per-op tier's walk put a fixture's routed layers: the
//! routed layers, and those whose stacks the device holds; zeros
//! where the heat cell skipped
struct HeatPlacement {
    routed : int64
    placed : int64
}

let HEAT_CELL_STEPS = 80l   //! fed steps past the heat pool's re-pin interval (64 tokens a layer), so every pooled layer is pinned and then hit

//! what the heat cell's device side hands its CPU side: where the
//! walk put the routed layers, the window's and the fed steps'
//! rows, the codec they ran on
struct HeatKept {
    placement : HeatPlacement
    gpu : array<array<float>>
    kv : KVDtype
    ran : bool       //! the device side fed its steps
    cpu_ran : bool   //! the CPU side fed and compared them
}

let private HEAT_PROMPT = 40l

//! the heat cell's device side on `tr`, loaded on the per-op tier
//! under the heat pool's want: the walk's placement, a
//! `HEAT_PROMPT`-token prompt and `HEAT_CELL_STEPS` fed steps, the
//! pool's hits and the shared expert's rows
def private heat_device_side(t : T?; tr : Model; kv : KVDtype; fx : MoeFixture) : HeatKept {
    var kept : HeatKept
    t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the per-op rails serve")
    var routed = 0l
    var placed = 0l
    for (l in range64(tr.config.n_layers)) {
        continue if (!layer_is_moe(tr, l))
        routed++
        placed += moe_gpu_layer_on_gpu(l) ? 1l : 0l
    }
    t |> success(placed > 0l && placed < routed, "the expert walk placed {placed} of {routed} routed layers, the rest streamed under the heat pool")
    kept.placement = HeatPlacement(routed = routed, placed = placed)
    kept.kv = kv
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(HEAT_PROMPT)
        var toks <- resident_feed(HEAT_CELL_STEPS)
        let heat0 = heat_totals()
        let over0 = shexp_gpu_decode_rows()
        let after0 = shexp_gpu_after_rows()
        let gemm0 = shexp_gpu_gemm_rows()
        var sg <- create_session(tr, kept.kv)
        kept.gpu <- feed_logits(tr, sg, prompt, toks)
        delete sg
        let heat = heat_totals()
        to_log(LOG_INFO, "heat pool: {heat.hits - heat0.hits} of {heat.picks - heat0.picks} streamed picks served by the slots; shared expert rows overlapped {shexp_gpu_decode_rows() - over0}, after {shexp_gpu_after_rows() - after0}, GEMMs {shexp_gpu_gemm_rows() - gemm0}\n")
        t |> success(heat.hits > heat0.hits, "the heat pool served {heat.hits - heat0.hits} of {heat.picks - heat0.picks} streamed picks on the device")
        if (fx.shexp) {
            t |> equal(shexp_gpu_after_rows() - after0, HEAT_CELL_STEPS * routed, "every routed layer's shared expert ran on the device after the routed chain or the pool's hits, every step")
            t |> equal(shexp_gpu_decode_rows() - over0, 0l, "no shared expert overlapped CPU experts under the heat pool")
            t |> equal(shexp_gpu_gemm_rows() - gemm0, 0l, "no shared expert on the three dense GEMMs: one activation form a layer")
        }
        delete prompt
        delete toks
    }
    kept.ran = true
    return <- kept
}

//! the heat cell's CPU side on `tr`, its device state gone: the same
//! prompt and fed steps on the CPU chain against the device side's
//! rows at the fixture's bar, the rows freed after
def private heat_cpu_side(t : T?; tr : Model; fx : MoeFixture; var kept : HeatKept) {
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(HEAT_PROMPT)
        var toks <- resident_feed(HEAT_CELL_STEPS)
        var cpu <- feed_alone(tr, kept.kv, prompt, toks)
        feed_steps_within(t, tr, "heat pool ", HEAT_PROMPT, kept.gpu, cpu, toks, fx.bar_rel,
            "per-op heat")
        delete cpu
        delete prompt
        delete toks
    }
    delete kept.gpu
    kept.cpu_ran = true
}

//! the heat cell's device side on a block's per-op form (`m`, loaded
//! on the per-op tier under the heat pool's want: `fx`'s resident
//! routed layers placed, the rest streamed under the pool's slots),
//! its rows kept for `moe_perop_heat_cpu`; no load
def moe_perop_heat_rows(t : T?; var m : Model; form : ModelForm; fx : MoeFixture;
                        var kept : HeatKept) {
    return if (!vulkan_home_asked(t, "the per-op tier and its heat pool are the Vulkan home's"))
    if (!form.perop) {
        t |> failure(("{fx.file}: the heat cell's device side runs on a per-op form, not " +
            "{form_name(form)}"))
        return
    }
    delete kept
    kept <- heat_device_side(t, m, form.kv, fx)   //! no mirror arms on a per-op load: the form names the codec
}

//! the CPU load the heat cell's CPU side runs on: the q8 CPU chain
//! the block's device forms answer to
def heat_cpu_form() : ModelForm => cpu_quant_form(QuantMode.q8)

//! the heat cell's CPU side on the first pass of `heat_cpu_form()`'s
//! load (`m`, no device state), where the device side ran; other
//! passes pass by; no load
def moe_perop_heat_cpu(t : T?; var m : Model; form : ModelForm; fx : MoeFixture;
                       var kept : HeatKept) {
    return if (!kept.ran || kept.cpu_ran || form.on_gpu || !same_cpu_load(form, heat_cpu_form()))
    heat_cpu_side(t, m, fx, kept)
}

//! after the block: a device side whose CPU side never ran fails
//! naming the file; where the walk put the routed layers returned,
//! the rows freed
def moe_perop_heat_verdict(t : T?; fx : MoeFixture; var kept : HeatKept) : HeatPlacement {
    if (kept.ran && !kept.cpu_ran) {
        t |> failure(("{fx.file}: the heat pool's steps lost their CPU side (no " +
            "`heat_cpu_form` pass)"))
    }
    let placement = kept.placement
    delete kept
    return placement
}

//! what a MoE cut's host-layers forms collect - the prompt stream on
//! and off - the routed layers asked onto the host by `fx`:
//! - the pinned feeds: `prompts` on the stream-on form, the routed
//!   census bound to the load's first feed;
//! - the stream twin: a 40-token feed and eight steps on each form
//!   with the device claiming first;
//! - on the CUDA home (`cuda`) the host cells: the stream-on feeds of
//!   `prompts` and the stream-off feed under the CUDA driver's
//!   counters, the meet handing the host its `host_first` rarest;
//! `kv` the codec the forms armed, `h` every reading
struct MoeHostRows {
    fx : MoeFixture = MoeFixture()
    prompts : array<int64>
    cuda : bool
    host_first : int64
    h : Held
    kv : KVDtype
    ran : bool   //! the prompt-stream form ran, armed at `kv`
    pinned : array<FeedRows>
    cuda_on : array<FeedRows>
    cuda_off : FeedRows
    cuda_off_ran : bool
    twin_on : FeedRows
    twin_off : FeedRows
    twin_on_ran : bool
    twin_off_ran : bool
    twin : bool = true   //! the stream twin collects on these forms
}

let private TWIN_IDS = 40l     //! the stream twin's prompt
let private TWIN_STEPS = 8l    //! ... and its fed steps

//! the CUDA stream chain's biased acts so far; -1 off the CUDA home
def private stream_biased() : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_stream_stats().biased if (gpu_want_cuda())
    }
    return -1l
}

//! the stream twin's feed on a host-layers form, the device claiming
//! first (`set_moe_host_first(0)`, put back); on the CUDA home a plain
//! model's chain takes no bias row
def private moe_twin_feed(var r : MoeHostRows; m : Model; form : ModelForm) : FeedRows {
    let pins = pin_stream(0l, plain_steps = false)
    let b0 = stream_biased()
    var f <- resident_feed_rows(m, form, TWIN_IDS, TWIN_STEPS)
    if (b0 >= 0l) {
        r.h |> held_equal(stream_biased(), b0,
            "control: the stream chain ran no biased act on a plain model")
    }
    restore_stream_pins(pins)
    return <- f
}

//! the CUDA home's host cells on a host-layers form, the meet's host
//! share pinned `r.host_first` (put back): with the stream on the
//! exact host-layer count asked, each prompt's feed under the driver's
//! counters, the stream's windows and the meet's split, the one-window
//! feed's N-row decline; with it off the control - no window streamed,
//! no expert met
def private moe_cuda_host_rows(var r : MoeHostRows; m : Model; form : ModelForm) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        let pins = pin_stream(r.host_first, plain_steps = false)
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(0)
        if (form.pf_stream) {
            r.h |> held_equal(gpu_resident_host_layers(), r.fx.host_layers_ask,
                "exactly the first {r.fx.host_layers_ask} routed layers' experts serve on " +
                "the host as asked (set_gpu_host_layers; the card holds the rest)")
            r.cuda_on |> reserve(length(r.prompts))
            for (n in r.prompts) {
                let at = stream_at()
                r.cuda_on |> emplace <| cuda_feed_rows(r.h, m, form, n, prompt_windows(n))
                stream_witness(r.h, r.fx.file, at, r.host_first)
                if (n == r.prompts[0]) {
                    let hot = cud_rdec_hot_stats()
                    to_log(LOG_INFO, "{r.fx.file}: {gpu_resident_hot_slots()} pool slots a " +
                        "layer; the pool launched {hot.submits} hit chains serving " +
                        "{hot.hits} picks\n")
                    r.h |> held_equal(rdec_token_n_rows(), 0l,
                        "the N-row command declines a plan with routed experts on the host: " +
                        "the split command steps a row at a time")
                }
            }
        } else {
            let at = stream_at()
            delete r.cuda_off
            r.cuda_off <- cuda_feed_rows(r.h, m, form, r.prompts[0], 1l)
            stream_off_witness(r.h, r.fx.file, at)
            r.cuda_off_ran = true
        }
        restore_stream_pins(pins)
    }
}

//! one host-layers form's share: with the prompt stream on the
//! fixture's admission, the pinned feeds, the twin's streamed half and
//! the CUDA home's streamed cells; with it off the twin's host-sums
//! half and the CUDA home's control
def moe_host_rows(var r : MoeHostRows; m : Model; form : ModelForm) {
    if (form.pf_stream) {
        r.kv = form.kv
        r.ran = true
        let host_layers = moe_fixture_admitted(r.h, m, r.fx)
        return if (host_layers < 0l)
        moe_prompt_feeds(r.pinned, r.h, m, form, r.fx, r.prompts, host_layers)
        if (r.twin) {
            delete r.twin_on
            r.twin_on <- moe_twin_feed(r, m, form)
            r.twin_on_ran = true
        }
    } elif (r.twin) {
        delete r.twin_off
        r.twin_off <- moe_twin_feed(r, m, form)
        r.twin_off_ran = true
    }
    if (r.cuda) {
        moe_cuda_host_rows(r, m, form)
    }
}

//! the host-layers forms' compares, `cpu` the CPU form's feeds of the
//! same prompts (eight fed steps) on the codec the forms armed: what
//! the forms held and the twin's streamed windows within the stream
//! twin's bar of the host's sums whatever the CPU side; the pinned
//! feeds within the fixture's bar and the CUDA home's feeds within the
//! routed bar, each missing CPU twin a failure (`twin_collected`)
def moe_host_rows_within(t : T?; tag : string; r : MoeHostRows; cpu : array<FeedRows>) {
    held_within(t, r.h, tag, false)
    moe_host_twin_within(t, tag, r)
    for (f, i in r.pinned, count()) {
        let what = "{tag}{f.nprompt} tokens: "
        continue if (!twin_collected(t, i, length(cpu), what))
        var inscope c <- feed_prefix(cpu[i], FED_STEPS)
        feed_rows_within(t, what, f, c, r.fx.bar_rel)
    }
    for (f, i in r.cuda_on, count()) {
        let what = "{tag}cuda, {f.nprompt} tokens: "
        continue if (!twin_collected(t, i, length(cpu), what))
        feed_rows_within(t, what, f, cpu[i], MOE_LOGIT_BAR_REL)
    }
    let off = "{tag}cuda, the prompt stream off: "
    if (r.cuda_off_ran && twin_collected(t, 0, length(cpu), off)) {
        feed_rows_within(t, off, r.cuda_off, cpu[0], MOE_LOGIT_BAR_REL)
    }
}

//! the stream twin's compare: its streamed windows within the stream
//! twin's bar of the host's sums, both off the device
def private moe_host_twin_within(t : T?; tag : string; r : MoeHostRows) {
    if (r.twin_on_ran && r.twin_off_ran) {
        if (r.twin_on.cuda >= 0l) {
            t |> equal(r.twin_on.cuda, TWIN_IDS + TWIN_STEPS,
                "{tag}the CUDA driver served every position of the streamed prompt's load")
        }
        feed_steps_within(t, "{tag}the stream twin: ", TWIN_IDS, r.twin_on.rows, r.twin_off.rows,
                r.twin_on.toks, STREAM_TWIN_BAR_REL, "the streamed windows",
                "the host's sums") $(id) {
            return feed_piece(r.twin_on, r.twin_off, id)
        }
    }
}
