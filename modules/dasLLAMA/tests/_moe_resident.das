// The MoE cells of the whole-model resident driver, shared by the per-model test files
// (test_gpu_resident_moe.das, test_gpu_resident_gptoss.das): the forced-feed logits-tolerance form of
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
require dasllama/dasllama_kqformat
require daslib/enum_trait   // the KqFmt members as a loop
require daslib/strings_boost   // split: a fixture's census sites
require _model_tier public   // models_dir() + model_available() presence/size-tier gate
require _resident_feed public   // resident_ready, load_resident, resident_prompt, resident_then_cpu_feed, feed_steps_within, tile_arm_tag
require _kv_codec_cells   // codec_distance_over: the three-chain form of the block-codec cells
require _format_census   // census_cell: a census fixture's every site in its own format

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
}

//! the logits bar as a fraction of the step's max |logit|: the routed chain's CPU-vs-device noise class (test_gpu_moe_shexp.das's
//! reading) - the arms agree to 1e-8 at layer 0 and part from layer 1 on, where the device chain's summation order moves the
//! residual by ulps, the next layer's Q8 requant rounds a few elements the other way and the router flips a near-tie; against
//! the all-CPU chain that reads 0.07-0.11 on the twin and the 30B at 40 and 600 tokens and 0.09-0.16 on the 35B, whose
//! two-window step 4 sits on a router near-tie that any change of the device's summation order flips (0.04 under one order,
//! 0.155-0.16 under two others); the one-step-off control reads 0.42-0.95, so the bar discriminates it by 2x and more
let MOE_LOGIT_BAR_REL = 0.20
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

//! the steps of a prompt of `nprompt` rows the unsplit twin must serve: every one under RD_UNSPLIT_POS, none past it
def private unsplit_want(nprompt : int64) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return nprompt + FED_STEPS <= RD_UNSPLIT_POS ? FED_STEPS : (nprompt >= RD_UNSPLIT_POS ? 0l : UNSPLIT_UNDECIDED)
    } else {
        return UNSPLIT_UNDECIDED
    }
}

//! the census over every `KqFmt` member's stamp of one tile column - `cm2e` (the expert schedule's m pieces in cm2 mode), `khr`
//! (every piece in mm mode) or `sdot4` (every piece on the Q8-fed chain's batch tiles) - the expert planes carrying whichever format
//! the file holds and the box serves it in (IQ3_S as `iq3s` or `iq3s4`); a 64-step format's e column is its m stamp
//! (KQ_CM2E_ALIASES_M), so `cm2e` reads that stamp's census for it
def private column_census(col : string) : int64 {
    var n = 0l
    for (f in type<KqFmt>) {
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
    if (gpu_want_cuda()) {   // the CUDA home's expert pieces ride its cm2 e and s tiles (the FP4 leaf's pieces count on no column)
        return (col = "cm2e", per_plane = 1l)
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
    t |> success(tr.config.n_expert > 0l && (tr.config.n_ff_shexp > 0l) == fx.shexp, "the fixture routes experts {fx.shexp ? "beside" : "with no"} shared expert")
    if (fx.window > 0l) {
        t |> equal(tr.config.sliding_window, fx.window, "the fixture's sliding layers attend {fx.window} keys")
    }
    if ((fx.expect_host_layers || fx.host_layers_ask > 0l) && !moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        t |> skip("the card holds neither the mirror asked whole ({fx.strict_ctx}) nor the dense planes beside it - the host-layer form needs a larger card (the load log names the room)")
        return -1l
    }
    t |> success(moe_gpu_resident_active(), "the whole-model driver took {fx.file} (a decline names its reason in the load log: the plan, a seat, a plane's format)")
    return -1l if (!moe_gpu_resident_active())
    mirror_codec_held(t)
    let host_layers = gpu_resident_host_layers()
    if (fx.expect_host_layers && host_layers == 0l) {
        t |> skip("the card holds every expert stack beside the {fx.strict_ctx}-position mirror - the host-layer form needs a smaller card")
        return -1l
    }
    if (fx.host_layers_ask > 0l) {   //! the ask is a floor: a card that holds the rest moves exactly that many, one that does not moves more
        t |> success(host_layers >= fx.host_layers_ask, "at least the first {fx.host_layers_ask} routed layers' experts serve on the host as the fixture asked (set_gpu_host_layers): {host_layers} of {tr.config.n_layers} do")
    } elif (fx.expect_host_layers) {
        t |> success(host_layers < tr.config.n_layers, "the card does not hold the experts whole: the first {host_layers} of {tr.config.n_layers} routed layers' experts serve on the host, the rest on the device")
    } else {
        t |> equal(host_layers, 0l, "every expert on the device (the cell pins ctx_max {fx.strict_ctx} and strict {fx.strict_ctx > 0l}; DASLLAMA_GPU_CTX_MAX / DASLLAMA_GPU_CTX_STRICT in the environment override the pins)")
    }
    if (fx.strict_ctx > 0l) {
        t |> equal(gpu_device_session_ctx(), fx.strict_ctx, "the context asked whole is the region's: the experts yielded, not the mirror")
    }
    return host_layers
}

//! prefill `n` tokens + the fed steps on the resident driver, then the same on the all-CPU chain; the
//! prefill logits and every step's within the bar, the one-step-off control past it, the routed
//! block's census growing by the MoE layer count per window and per step
[cold_path]
def moe_forced_feed(t : T?; n : int64; fx : MoeFixture) {
    let path = path_join(models_dir(), fx.file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path, 1l, fx.strict_ctx, fx.host_layers_ask, fx.image)   //! one region: the context asked whole is the region's
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
    let device_routed_layers = tr.config.n_layers - host_layers   // a host layer runs its router alone
    let windows = (n + PF_WINDOW_ROWS - 1l) / PF_WINDOW_ROWS
    let runs_deep_twins = tr.config.n_expert > 256l
    let sched_key = runs_deep_twins ? "moe_sched2_cls_spv" : "moe_sched_cls_spv"
    let rows_key = runs_deep_twins ? "topk_rows2_cls_spv" : "topk_rows_cls_spv"
    let topk_key = runs_deep_twins ? "topk_n2_cls_spv" : "topk_n_cls_spv"
    with_dasllama_jobque_() {
        let sched0 = census(sched_key)
        let rows0 = census(rows_key)
        let topk0 = census(topk_key)
        let ecol = expert_column()
        let e0 = column_census(ecol.col)
        let un0 = unsplit_tokens()
        forced_feed_over(t, tr, "", n, FED_STEPS, fx.bar_rel) $ {
            if (attn_nsplit() > 1l && unsplit_want(n) != UNSPLIT_UNDECIDED && host_layers == 0l) {   //! a split device records the unsplit twin: it serves the steps under RD_UNSPLIT_POS and none past it
                t |> equal(unsplit_tokens() - un0, unsplit_want(n), "the token command's unsplit twin served {unsplit_want(n)} of the {FED_STEPS} steps after {n} rows")
            }
            if (sched0 != NO_CENSUS) {
                t |> equal(census(sched_key) - sched0, device_routed_layers * windows, "the device bucket schedule ({sched_key}) ran once per device-routed layer per window ({device_routed_layers} layers, {windows} windows)")
                t |> equal(census(rows_key) - rows0, device_routed_layers * windows, "the window's per-row select ({rows_key}) ran once per device-routed layer per window")
                //! the token command records once and resubmits: the census counts its recording, one top-k per MoE layer per recorded form (a split device records the unsplit twin too)
                let topk = census(topk_key) - topk0
                t |> success(topk >= device_routed_layers && topk % device_routed_layers == 0l, "the token command recorded a top-k per device-routed layer ({topk} over {device_routed_layers} layers)")
                let edelta = column_census(ecol.col) - e0
                let ewant = 3l * ecol.per_plane * device_routed_layers * windows
                t |> success(edelta >= ewant, "the expert schedule's pieces dispatched the {ecol.col} column at least {ecol.per_plane} a plane per device-routed layer per window ({edelta} over a floor of {ewant})")
            }
        }
    }
    delete tr
}

let HEAT_CELL_STEPS = 80l   //! fed steps past the heat pool's re-pin interval (64 tokens a layer), so every pooled layer is pinned and then hit

//! the per-op tier's heat pool on a MoE fixture: the whole-model route off, the last `resident` routed layers' stacks placed and
//! the rest streamed under `slots` hot experts a layer; a 40-token prompt and `HEAT_CELL_STEPS` fed steps against the CPU chain at
//! the fixture's bar, the pool serving some of the streamed layers' picks, and on a shared-expert fixture every routed layer's
//! shared expert on the device after the routed chain or the pool's hits - none overlapped, none on the CPU, none on the GEMMs
def moe_perop_heat_cell(t : T?; fx : MoeFixture; resident, slots : int64) {
    if (gpu_want_cuda()) {
        t |> skip("the CUDA home is asked: the per-op tier and its heat pool are the Vulkan home's")
        return
    }
    let path = path_join(models_dir(), fx.file)
    if (!resident_ready(t, path)) return
    let route = gpu_resident_route_lever()
    let want0 = g_gpu_want
    set_gpu_resident_route(false)
    var want = want0
    want.moe_layers = resident
    want.moe_stream = -1l
    want.heat = slots
    set_gpu_tier_want_(want)
    t |> success(moe_gpu_tier_arm_(), "the tier re-armed at {resident} resident routed layers")
    heat_reset()
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_resident_route(route)
    t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the per-op rails serve")
    var routed = 0l
    var placed = 0l
    for (l in range64(tr.config.n_layers)) {
        continue if (!layer_is_moe(tr, l))
        routed++
        placed += moe_gpu_layer_on_gpu(l) ? 1l : 0l
    }
    t |> success(placed > 0l && placed < routed, "the expert walk placed {placed} of {routed} routed layers, the rest streamed under the heat pool")
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(40l)
        var toks <- resident_feed(HEAT_CELL_STEPS)
        let heat0 = heat_totals()
        let over0 = shexp_gpu_decode_rows()
        let after0 = shexp_gpu_after_rows()
        let gemm0 = shexp_gpu_gemm_rows()
        var sg <- create_session(tr, mirror_kv_dtype())
        var gpu <- feed_logits(tr, sg, prompt, toks)
        delete sg
        let heat = heat_totals()
        to_log(LOG_INFO, "heat pool: {heat.hits - heat0.hits} of {heat.picks - heat0.picks} streamed picks served by the slots; shared expert rows overlapped {shexp_gpu_decode_rows() - over0}, after {shexp_gpu_after_rows() - after0}, GEMMs {shexp_gpu_gemm_rows() - gemm0}\n")
        t |> success(heat.hits > heat0.hits, "the heat pool served {heat.hits - heat0.hits} of {heat.picks - heat0.picks} streamed picks on the device")
        if (fx.shexp) {
            t |> equal(shexp_gpu_after_rows() - after0, HEAT_CELL_STEPS * routed, "every routed layer's shared expert ran on the device after the routed chain or the pool's hits, every step")
            t |> equal(shexp_gpu_decode_rows() - over0, 0l, "no shared expert overlapped CPU experts under the heat pool")
            t |> equal(shexp_gpu_gemm_rows() - gemm0, 0l, "no shared expert on the three dense GEMMs: one activation form a layer")
        }
        set_gpu_tier_want_(want0)
        moe_gpu_tier_arm_()
        heat_reset()
        moe_gpu_drop_model()
        var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
        feed_steps_within(t, tr, "heat pool ", 40l, gpu, cpu, toks, fx.bar_rel, "per-op heat")
        delete gpu
        delete cpu
        delete prompt
        delete toks
    }
    set_gpu_tier_want_(want0)
    moe_gpu_tier_arm_()
    delete tr
}

//! the K/V codec rig's distance form (`codec_distance_over`) on a MoE fixture the driver admitted, the mirror asked at `kv`
//! by the caller's pin: the device at `kv`, the CPU at `kv` and the CPU at f16 over an `n`-token prompt and the rig's fed steps
[cold_path]
def moe_codec_distance(t : T?; n : int64; fx : MoeFixture; kv : KVDtype; dist_rel, pair_rel : float) {
    let path = path_join(models_dir(), fx.file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path, 1l, fx.strict_ctx, fx.host_layers_ask)
    if (moe_fixture_admitted(t, tr, fx) >= 0l) {
        with_dasllama_jobque_() {
            codec_distance_over(t, tr, n, kv, false, dist_rel, pair_rel)
        }
    }
    delete tr
}
