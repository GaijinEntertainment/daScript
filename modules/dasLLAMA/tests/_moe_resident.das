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
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the MoE
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode and the kernel census, where the module exists
require _model_tier public   // models_dir() + model_available() presence/size-tier gate

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
let private NO_CENSUS = -1l   //! the census count with no vulkan module: the routed-block witnesses stand down

def assert_pf_window(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
    }
}

//! the served tile arm, so a red names which prefill tile it came from
def private tile_arm_tag() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return "coopmat mode {g_gpu.coopmat_mode}"
    } else {
        return "no vulkan module"
    }
}

def private argmax_of(v : array<float>) : int64 {
    var best = -1e30
    var bi = 0l
    for (i in range64(long_length(v))) {
        if (v[i] > best) {
            best = v[i]
            bi = i
        }
    }
    return bi
}

//! the coverage census's count of a routed-block kernel's dispatches so far; NO_CENSUS with no vulkan module
def private census(kernel : string) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_coverage_of(kernel)
    } else {
        return NO_CENSUS
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

//! the census over every format's stamp of one tile column - `cm2e` (the expert schedule's m pieces in cm2 mode) or `khr` (every
//! piece in mm mode) - the expert planes carrying whichever format the file holds: the kq formats, q8, the per-32 q51 and mx4 rails
def private column_census(col : string) : int64 {
    var n = census("q8_batch_{col}_cls_spv") + census("q51_batch_{col}_cls_spv") + census("mx4_batch_{col}_cls_spv")
    for (f in ["k4", "k5", "k6", "q40", "iq4xs", "k3", "iq3s", "iq3xxs", "iq4nl", "k2", "iq2s", "iq2xs", "iq2xxs"]) {
        n += census("kq_batch_{f}_{col}_cls_spv")
    }
    return n
}

//! the expert pieces' tile column on this device, and the dispatches it takes per MoE layer per window: the e column's one m dispatch a
//! plane in cm2 mode; in mm mode the KHR tile takes both the s and the m dispatch of every plane
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

def private feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> {
    let n = long_length(prompt)
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- out
}

//! prefill `n` tokens + the fed steps on the resident driver, then the same on the all-CPU chain; the
//! prefill logits and every step's within the bar, the one-step-off control past it, the routed
//! block's census growing by the MoE layer count per window and per step
def moe_forced_feed(t : T?; n : int64; fx : MoeFixture) {   // nolint:STYLE038 — one linear forced-feed rig: the window feed, the census witnesses, the CPU control, the per-step bars
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return
    }
    let path = path_join(models_dir(), fx.file)
    if (!model_available(t, path)) return
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return
    }
    let route0 = gpu_want_resident()
    set_gpu_resident_route(true)   //! the whole-model driver's arm, whatever lever an earlier cell left
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_resident_route(route0)
    t |> success(tr.config.n_expert > 0l && (tr.config.n_ff_shexp > 0l) == fx.shexp, "the fixture routes experts {fx.shexp ? "beside" : "with no"} shared expert")
    if (fx.window > 0l) {
        t |> equal(tr.config.sliding_window, fx.window, "the fixture's sliding layers attend {fx.window} keys")
    }
    if (!device_has_expert_tiles()) {
        t |> skip("the device has no tile family for the expert pieces (cm2, or KHR at subgroup 32) - the whole-model driver declines a MoE by design")
        delete tr
        return
    }
    t |> success(moe_gpu_resident_active(), "the whole-model driver took {fx.file} (a decline names its reason in the load log: the plan, a seat, a plane's format)")
    if (!moe_gpu_resident_active()) {
        delete tr
        return
    }
    let layers = tr.config.n_layers
    var nrec = 0l
    for (l in range64(layers)) {
        if (layer_is_recurrent(tr.config, l)) {
            nrec++
        }
    }
    let windows = (n + PF_WINDOW_ROWS - 1l) / PF_WINDOW_ROWS
    with_job_que() {
        setup_dasllama_jobque_()
        var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
        var toks <- [for (i in range64(FED_STEPS)); 2000l + (i * 53l) % 4000l]
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = moe_gpu_resident_prefills()
        let sched0 = census("moe_sched_cls_spv")
        let rows0 = census("topk_rows_cls_spv")
        let topk0 = census("topk_cls_spv")
        let ecol = expert_column()
        let e0 = column_census(ecol.col)
        let un0 = unsplit_tokens()
        var gpu <- feed_logits(tr, sg, prompt, toks)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        if (attn_nsplit() > 1l && unsplit_want(n) != UNSPLIT_UNDECIDED) {   //! a split device records the unsplit twin: it serves the steps under RD_UNSPLIT_POS and none past it
            t |> equal(unsplit_tokens() - un0, unsplit_want(n), "the token command's unsplit twin served {unsplit_want(n)} of the {FED_STEPS} steps after {n} rows")
        }
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt ({nrec} recurrent layers of {layers})")
        if (sched0 != NO_CENSUS) {
            t |> equal(census("moe_sched_cls_spv") - sched0, layers * windows, "the device bucket schedule ran once per MoE layer per window ({windows} windows)")
            t |> equal(census("topk_rows_cls_spv") - rows0, layers * windows, "the window's per-row select ran once per MoE layer per window")
            //! the token command records once and resubmits: the census counts its recording, one top-k per MoE layer per recorded form (a split device records the unsplit twin too)
            let topk = census("topk_cls_spv") - topk0
            t |> success(topk >= layers && topk % layers == 0l, "the token command recorded a top-k per MoE layer ({topk} over {layers} layers)")
            let edelta = column_census(ecol.col) - e0
            let ewant = 3l * ecol.per_plane * layers * windows
            if (ecol.per_plane == 1l) {
                t |> equal(edelta, ewant, "the expert schedule's m pieces dispatched the e column on the three expert planes, once per MoE layer per window")
            } else {   //! the KHR tile also serves the window's dense GEMMs, so its count is a floor: the expert pieces' dispatches at least
                t |> success(edelta >= ewant, "the expert schedule's pieces dispatched the KHR tile at least twice a plane per MoE layer per window ({edelta} over a floor of {ewant})")
            }
        }
        delete sg
        moe_gpu_drop_model()   //! every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        var cpu <- feed_logits(tr, sc, prompt, toks)
        delete sc
        for (st in range64(long_length(cpu))) {
            let diff = logits_maxdiff(gpu[st], cpu[st])
            let bar = fx.bar_rel * logits_maxabs(cpu[st])
            let ag = argmax_of(gpu[st])
            let ac = argmax_of(cpu[st])
            let what = (st == 0l ? "prefill ({n} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
            to_log(LOG_INFO, "{what}: argmax cpu {ac} ({cpu[st][ac]}), resident {ag} ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
            t |> success(diff <= bar, "{what} logits: resident within {bar} of the CPU chain (maxdiff {diff})")
            if (st > 0l) {   //! the control: the previous step's CPU logits are a disconnected mechanism the bar must RED
                let off = logits_maxdiff(gpu[st], cpu[st - 1l])
                to_log(LOG_INFO, "{what}: one-step-off control maxdiff {off} (bar {bar})\n")
                t |> success(off > bar, "{what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
            }
        }
        delete gpu
        delete cpu
        delete prompt
        delete toks
    }
    delete tr
}
