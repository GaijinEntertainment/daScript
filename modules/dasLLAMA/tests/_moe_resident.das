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
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode and the kernel census, where the module exists
require dasllama/dasllama_kqformat
require daslib/enum_trait   // the KqFmt members as a loop
require _model_tier public   // models_dir() + model_available() presence/size-tier gate
require _resident_feed public   // resident_ready, load_resident, resident_prompt, resident_then_cpu_feed, feed_steps_within, tile_arm_tag
require _kv_codec_cells   // codec_distance_over: the three-chain form of the block-codec cells

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
}

//! the logits bar as a fraction of the step's max |logit|: the routed chain's CPU-vs-device noise class (test_gpu_moe_shexp.das's
//! reading) - the arms agree to 1e-8 at layer 0 and part from layer 1 on, where the device chain's summation order moves the
//! residual by ulps, the next layer's Q8 requant rounds a few elements the other way and the router flips a near-tie; against
//! the all-CPU chain that reads 0.07-0.11 on the twin and the 30B at 40 and 600 tokens and 0.09-0.16 on the 35B, whose
//! two-window step 4 sits on a router near-tie that any change of the device's summation order flips (0.04 under one order,
//! 0.155-0.16 under two others); the one-step-off control reads 0.42-0.95, so the bar discriminates it by 2x and more
let MOE_LOGIT_BAR_REL = 0.20
let PF_WINDOW_ROWS = 512l  //! the engine's PF_WINDOW, asserted equal by the two-window cell: a prompt past it runs as sequential windows
let FED_STEPS = 6l

def assert_pf_window(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
    }
}

//! the token command's attention split count (0 without the module or the driver)
def private attn_nsplit : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_attn_nsplit()
    } else {
        return 0l
    }
}

//! the tokens the token command's unsplit twin served so far
def private unsplit_tokens : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_unsplit_tokens()
    } else {
        return 0l
    }
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

//! the census over every `KqFmt` member's stamp of one tile column - `cm2e` (the expert schedule's m pieces in cm2 mode) or `khr`
//! (every piece in mm mode) - the expert planes carrying whichever format the file holds and the box serves it in (IQ3_S as
//! `iq3s` or `iq3s4`); a 64-step format's e column is its m stamp (KQ_CM2E_ALIASES_M), so `cm2e` reads that stamp's census for it
def private column_census(col : string) : int64 {
    var n = 0l
    for (f in type<KqFmt>) {
        let name = "{f}"
        n += census("{kq_tile_stem(name)}_{col == "cm2e" && kq_cm2e_aliases_m(name) ? "cm2m" : col}_cls_spv")
    }
    return n
}

//! the expert pieces' tile column on this device, and the dispatches it takes per MoE layer per window: the e column's one m dispatch a
//! plane in cm2 mode; in mm mode the KHR tile takes both the s and the m dispatch of every plane. Either way the count is a floor: the
//! KHR tile and the 64-step formats' m stamp both serve the window's dense GEMMs too
def private expert_column() : tuple<col : string; per_plane : int64> {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu.coopmat_mode == COOPMAT_MM ? (col = "khr", per_plane = 2l) : (col = "cm2e", per_plane = 1l)
    } else {
        return (col = "cm2e", per_plane = 1l)
    }
}

//! the device carries a tile family for the routed block's expert pieces: the cm2 s and e stamps in cm2 mode, the KHR 128 x 128
//! tile in mm mode at subgroup 32 (the driver declines a MoE without either)
def private device_has_expert_tiles() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu != null && ((g_gpu.has_coopmat2 && g_gpu.coopmat_mode == COOPMAT_CM2) || (g_gpu.coopmat_mode == COOPMAT_MM && khr_kq_tile_on()))
    } else {
        return false
    }
}

//! the driver's admission of the fixture and the host-layer count it armed, against the fixture's expectation;
//! -1 where the cell ends here - a skip the box earns, or the red that names the load log
def private moe_fixture_admitted(t : T?; tr : Model; fx : MoeFixture) : int64 {
    t |> success(tr.config.n_expert > 0l && (tr.config.n_ff_shexp > 0l) == fx.shexp, "the fixture routes experts {fx.shexp ? "beside" : "with no"} shared expert")
    if (fx.window > 0l) {
        t |> equal(tr.config.sliding_window, fx.window, "the fixture's sliding layers attend {fx.window} keys")
    }
    if (!device_has_expert_tiles()) {
        t |> skip("the device has no tile family for the expert pieces (cm2, or KHR at subgroup 32) - the whole-model driver declines a MoE by design")
        return -1l
    }
    if (fx.expect_host_layers && !moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        t |> skip("the card holds neither the mirror asked whole ({fx.strict_ctx}) nor the dense planes beside it - the host-layer form needs a larger card (the load log names the room)")
        return -1l
    }
    t |> success(moe_gpu_resident_active(), "the whole-model driver took {fx.file} (a decline names its reason in the load log: the plan, a seat, a plane's format)")
    return -1l if (!moe_gpu_resident_active())
    let asked = gpu_kv_dtype_pin()
    if (asked.pinned) {
        t |> equal(mirror_kv_dtype(), asked.dt, "the mirror armed at the codec the cell asks")
    }
    let host_layers = gpu_resident_host_layers()
    if (fx.expect_host_layers && host_layers == 0l) {
        t |> skip("the card holds every expert stack beside the {fx.strict_ctx}-position mirror - the host-layer form needs a smaller card")
        return -1l
    }
    if (fx.expect_host_layers) {
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
def moe_forced_feed(t : T?; n : int64; fx : MoeFixture) {
    let path = path_join(models_dir(), fx.file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path, 1l, fx.strict_ctx)   //! one region: the context asked whole is the region's
    let host_layers = moe_fixture_admitted(t, tr, fx)
    if (host_layers < 0l) {
        delete tr
        return
    }
    let device_routed_layers = tr.config.n_layers - host_layers   // a host layer runs its router alone
    let windows = (n + PF_WINDOW_ROWS - 1l) / PF_WINDOW_ROWS
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(n)
        var toks <- resident_feed(FED_STEPS)
        let sched0 = census("moe_sched_cls_spv")
        let rows0 = census("topk_rows_cls_spv")
        let topk0 = census("topk_n_cls_spv")
        let ecol = expert_column()
        let e0 = column_census(ecol.col)
        let un0 = unsplit_tokens()
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks) $ {
        if (attn_nsplit() > 1l && unsplit_want(n) != UNSPLIT_UNDECIDED && host_layers == 0l) {   //! a split device records the unsplit twin: it serves the steps under RD_UNSPLIT_POS and none past it
            t |> equal(unsplit_tokens() - un0, unsplit_want(n), "the token command's unsplit twin served {unsplit_want(n)} of the {FED_STEPS} steps after {n} rows")
        }
        if (sched0 != NO_CENSUS) {
            t |> equal(census("moe_sched_cls_spv") - sched0, device_routed_layers * windows, "the device bucket schedule ran once per device-routed layer per window ({device_routed_layers} layers, {windows} windows)")
            t |> equal(census("topk_rows_cls_spv") - rows0, device_routed_layers * windows, "the window's per-row select ran once per device-routed layer per window")
            //! the token command records once and resubmits: the census counts its recording, one top-k per MoE layer per recorded form (a split device records the unsplit twin too)
            let topk = census("topk_n_cls_spv") - topk0
            t |> success(topk >= device_routed_layers && topk % device_routed_layers == 0l, "the token command recorded a top-k per device-routed layer ({topk} over {device_routed_layers} layers)")
            let edelta = column_census(ecol.col) - e0
            let ewant = 3l * ecol.per_plane * device_routed_layers * windows
            t |> success(edelta >= ewant, "the expert schedule's pieces dispatched the {ecol.col} column at least {ecol.per_plane} a plane per device-routed layer per window ({edelta} over a floor of {ewant})")
        }
        }
        feed_steps_within(t, tr, "", n, fed.gpu, fed.cpu, toks, fx.bar_rel)
        delete fed
        delete prompt
        delete toks
    }
    delete tr
}

//! the K/V codec rig's distance form (`codec_distance_over`) on a MoE fixture the driver admitted, the mirror asked at `kv`
//! by the caller's pin: the device at `kv`, the CPU at `kv` and the CPU at f16 over an `n`-token prompt and the rig's fed steps
def moe_codec_distance(t : T?; n : int64; fx : MoeFixture; kv : KVDtype; dist_rel, pair_rel : float) {
    let path = path_join(models_dir(), fx.file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path, 1l, fx.strict_ctx)
    if (moe_fixture_admitted(t, tr, fx) >= 0l) {
        with_dasllama_jobque_() {
            codec_distance_over(t, tr, n, kv, false, dist_rel, pair_rel)
        }
    }
    delete tr
}
