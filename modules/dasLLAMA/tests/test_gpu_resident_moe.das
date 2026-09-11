options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2moe)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the MoE
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode and the kernel census, where the module exists
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// The whole-model resident driver on a MoE (Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local, the Q4_K_M mint of
// the Q8_0 carrier, under DASLLAMA_GPU=1): the expert stacks in the arena beside the attention
// quads and the shared experts, the window chain's routed block (the router GEMM, the per-row
// select, the device bucket schedule, the f16 gather, the expert tiles, the gated combine) and the
// token command's routed block (the router GEMV, the top-k writing the expert GEMVs' slot regions,
// the combine). The forced-feed logits-tolerance form of test_gpu_resident_hybrid: the same fixed
// tokens through the resident driver and the all-CPU chain, the prefill logits and every step's
// within the bar, the one-step-off control past it, and the census witnesses that the routed
// block ran on the device for every MoE layer (the window's per window, the token command's once
// at its recording). Two prompt lengths: one window, two windows. Three fixtures: the twin, the
// Qwen3.6-35B UD-IQ2_XXS hybrid, whose recurrent layers take the routed block after the deltanet head,
// and the Qwen3-30B UD-IQ2_XXS, the MoE with no shared expert (the residual step's add partner off).
// All three fixtures are large-tier (DASLLAMA_PARITY_FULL=1); the file is -jit only, and skips
// without the file, the armed tier, or a device with no cm2 tile family (the routed block's only
// tile); on a cm2 device the driver's admission of every fixture is asserted, never skipped.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}

//! a fixture file and whether it carries a shared expert
struct private Fixture {
    file : string
    shexp : bool
}

let private TWIN = Fixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", shexp = true)   //! the Q4_K_M mint of the Q8_0 carrier (its recipe row in ../performance/model_specs.das)
let private HYBRID = Fixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true)   //! the deltanet hybrid MoE with a gated shared expert, the 16 GB card's whole-model fit
let private NO_SHEXP = Fixture(file = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf", shexp = false)   //! the MoE with no shared expert: the routed block alone at the FFN site
//! the logits bar as a fraction of the step's max |logit|: the routed chain's CPU-vs-device noise class (test_gpu_moe_shexp.das's
//! reading) - the arms agree to 1e-8 at layer 0 and part from layer 1 on, where the device chain's summation order moves the
//! residual by ulps, the next layer's Q8 requant rounds a few elements the other way and the router flips a near-tie; against
//! the all-CPU chain that reads 0.07-0.11 on the twin and the 30B at 40 and 600 tokens and 0.09-0.16 on the 35B, whose
//! two-window step 4 sits on a router near-tie that any change of the device's summation order flips (0.04 under one order,
//! 0.155-0.16 under two others); the one-step-off control reads 0.42-0.95, so the bar discriminates it by 2x and more
let private MOE_LOGIT_BAR_REL = 0.20
let private PF_WINDOW_ROWS = 512l  //! the engine's PF_WINDOW, asserted equal by the two-window cell: a prompt past it runs as sequential windows
let private FED_STEPS = 6l
let private NO_CENSUS = -1l   //! the census count with no vulkan module: the routed-block witnesses stand down

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

//! the census over every format's stamp of one cm2 column (the expert planes carry whichever format the file holds)
def private column_census(col : string) : int64 {
    var n = census("q8_batch_cm2{col}_cls_spv")
    for (f in ["k4", "k5", "k6", "q40", "iq4xs", "k3", "iq3s", "iq3xxs", "iq4nl", "k2", "iq2s", "iq2xs", "iq2xxs"]) {
        n += census("kq_batch_{f}_cm2{col}_cls_spv")
    }
    return n
}

//! the device carries the cm2 tile family the routed block's expert tiles need (the driver declines a MoE without it)
def private device_has_cm2() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu != null && g_gpu.has_coopmat2 && g_gpu.coopmat_mode == COOPMAT_CM2
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
def private moe_forced_feed(t : T?; n : int64; fx : Fixture) {   // nolint:STYLE038 — one linear forced-feed rig: the window feed, the census witnesses, the CPU control, the per-step bars
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
    if (!device_has_cm2()) {
        t |> skip("the device has no cm2 tile family - the whole-model driver declines a MoE by design")
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
        let e0 = column_census("e")
        var gpu <- feed_logits(tr, sg, prompt, toks)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt ({nrec} recurrent layers of {layers})")
        if (sched0 != NO_CENSUS) {
            t |> equal(census("moe_sched_cls_spv") - sched0, layers * windows, "the device bucket schedule ran once per MoE layer per window ({windows} windows)")
            t |> equal(census("topk_rows_cls_spv") - rows0, layers * windows, "the window's per-row select ran once per MoE layer per window")
            //! the token command records once and resubmits: the census counts its recording, one top-k per MoE layer
            let topk = census("topk_cls_spv") - topk0
            t |> success(topk >= layers && topk % layers == 0l, "the token command recorded a top-k per MoE layer ({topk} over {layers} layers)")
            t |> equal(column_census("e") - e0, 3l * layers * windows, "the expert schedule's m pieces dispatched the e column on the three expert planes, once per MoE layer per window")
        }
        delete sg
        moe_gpu_drop_model()   //! every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        var cpu <- feed_logits(tr, sc, prompt, toks)
        delete sc
        for (st in range64(long_length(cpu))) {
            let diff = logits_maxdiff(gpu[st], cpu[st])
            let bar = MOE_LOGIT_BAR_REL * logits_maxabs(cpu[st])
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

[test]
def test_gpu_resident_moe(t : T?) {
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the resident MoE prefill window + the routed token command vs the CPU chain, one window") @(t : T?) {
        moe_forced_feed(t, 40l, TWIN)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the same across two windows") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
        }
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, TWIN)
    }
}

[test]
def test_gpu_resident_moe_hybrid(t : T?) {
    t |> run("Qwen3.6-35B-A3B-UD-IQ2_XXS: the deltanet hybrid MoE - the recurrent head beside the routed block, one window") @(t : T?) {
        moe_forced_feed(t, 40l, HYBRID)
    }
    t |> run("Qwen3.6-35B-A3B-UD-IQ2_XXS: the same across two windows (the conv tail and the state carry the seam)") @(t : T?) {
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, HYBRID)
    }
}

[test]
def test_gpu_resident_moe_no_shexp(t : T?) {
    t |> run("Qwen3-30B-A3B-UD-IQ2_XXS: the MoE with no shared expert - the routed block alone at the FFN site, one window") @(t : T?) {
        moe_forced_feed(t, 40l, NO_SHEXP)
    }
    t |> run("Qwen3-30B-A3B-UD-IQ2_XXS: the same across two windows") @(t : T?) {
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, NO_SHEXP)
    }
}
