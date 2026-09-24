options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen35)
require dasllama/dasllama   // create_session, moe_gpu_drop_model

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the hybrid
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode and the kernel census, where the module exists
require ?vulkan dasllama/dasllama_vulkan_prefill   // pf_f16_feed: the feed admission the driver acts on
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // resident_then_cpu_feed, feed_steps_within, tile_arm_tag

// The whole-model resident driver on a deltanet hybrid under DASLLAMA_GPU=1: the resident window
// chain prefills the prompt (the recurrent layers through the conv + chunked scan over their device
// state, the gated partial-rope attention over the mirror), then the resident decode steps every
// layer - the recurrent ones through the fused deltanet step - as one submit per token. The GPU
// chains and the CPU chain are not bit-identical (GPU exp / f32 accumulation order), so the gate is
// the forced-feed logits-tolerance form: the same fixed tokens fed to both arms, the logits within
// a bar of the CPU chain after the prefill and at every step, and the one-step-off control that
// must exceed it. Three fixtures: the Q8_0 file over four prompt lengths - one window, two windows
// (the conv tail and the state carry across the window seam), one past the window, and two tokens
// (both conv-tail edges: the last window shorter than the taps); the K-quant twin (Q4_K_M) over one
// and two windows; and the mixed twin (Q4_K_M with the deltanet out plane at Q8_0) over the same
// two, whose recurrent layers take a K-quant x feed and a Q8_0 o feed - the routing witness below
// holds that the x feed rode the coopmat tiles.

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit| - the deltanet bar test_deltanet.das holds too
//! the K-quant bar on the f16 feed and mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit at every step (the Q8 file 0.19-0.36), with no growth and the one-step-off control well past it
let private DN_LOGIT_BAR_REL_KQ_F16_FEED = 0.06
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell under `DASLLAMA_COOPMAT=sdot4` on the RTX 5060 Ti reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT_FEED = 0.10
let private PF_WINDOW_ROWS = 512l     //! the engine's PF_WINDOW, asserted equal by the two-window cells: a prompt past it runs as sequential windows
let private RQ_NO_CENSUS = -1l        //! the census count with no vulkan module: the routing witness stands down

//! the plane formats a fixture's deltanet layers carry: all Q8_0, all K-quant, or K-quant qkv/z beside a Q8_0 out plane
enum HybridPlanes {
    q8
    kq
    kq_q8out
}

def private window_pin(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
    }
}

//! the coverage census's count of Q8_K requant dispatches so far; RQ_NO_CENSUS with no vulkan module
def private q8k_requants() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_coverage_of("cls_q8k_rq_spv")
    } else {
        return RQ_NO_CENSUS
    }
}

//! the driver's own answer: does the prefill's f16 feed admit a K-quant plane on the armed coopmat mode
def private f16_feed_admits_kq() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return pf_f16_feed(int(KqFmt.k4))
    } else {
        return false
    }
}

// prefill `n` tokens + 8 fed steps on the resident driver, then the same on the all-CPU chain; the
// prefill logits and every step's within the deltanet bar, the one-step-off control past it.
def private hybrid_forced_feed(t : T?; n : int64; file : string; planes : HybridPlanes) {
    let kq = planes != HybridPlanes.q8
    let q8out = planes == HybridPlanes.kq_q8out
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return
    }
    let path = path_join(models_dir(), file)
    if (!model_available(t, path)) return
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return
    }
    if (kq && mirror_kv_dtype() == KVDtype.f32) {
        t |> skip("the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
        return
    }
    var tr <- load_model_(path, QuantMode.q8)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the hybrid (the load log names any decline)")
    var nrec = 0l
    if (kq) {
        var native = 0l
        for (l in range64(tr.config.n_layers)) {
            if (!layer_is_recurrent(tr.config, l)) continue
            nrec++
            if (fmt_at(tr.dnqkv_fmt, l) != KqFmt.q8 && fmt_at(tr.dngate_fmt, l) != KqFmt.q8
                    && ((fmt_at(tr.dnout_fmt, l) == KqFmt.q8) == q8out)) {
                native++
            }
        }
        let out_form = q8out ? "the out plane Q8_0" : "the out plane too"
        t |> success(native > 0l, "the loader kept the deltanet qkv/z planes in their K-quant file formats, {out_form} ({native} recurrent layers)")
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
        var toks <- [for (i in range64(8l)); 2000l + (i * 53l) % 4000l]
        let rq0 = q8k_requants()
        var prefill_q8k_rq = RQ_NO_CENSUS   //! Q8_K requants the resident prefill alone dispatched (RQ_NO_CENSUS with no vulkan module)
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
            prefill_q8k_rq = rq0 == RQ_NO_CENSUS ? RQ_NO_CENSUS : q8k_requants() - rq0
        }) $ {}
        if (kq && prefill_q8k_rq != RQ_NO_CENSUS) {
            //! off the f16 feed the x route requants once per recurrent layer per window - the threshold both arms read
            to_log(LOG_INFO, "prefill ({n} tokens) [{tile_arm_tag()}]: {prefill_q8k_rq} Q8_K requants over {nrec} recurrent layers, f16 feed admits kq {f16_feed_admits_kq()}\n")
            if (f16_feed_admits_kq()) {
                t |> success(prefill_q8k_rq < nrec, "on the f16 feed the prefill never requantized the block input to Q8_K for the qkv/z pair ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            } else {
                t |> success(prefill_q8k_rq >= nrec, "off the f16 feed the K-quant qkv/z pair reads the Q8_K form ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            }
        }
        let rel = kq ? (f16_feed_admits_kq() ? DN_LOGIT_BAR_REL_KQ_F16_FEED : DN_LOGIT_BAR_REL_KQ_QUANT_FEED) : DN_LOGIT_BAR_REL
        feed_steps_within(t, "", n, fed.gpu, fed.cpu, toks, rel)
        delete fed
        delete prompt
        delete toks
    }
    delete tr
}

[test]
def test_gpu_resident_hybrid_decode(t : T?) {
    t |> run("Qwen3.5-0.8B: resident prefill + decode with deltanet layers on device vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

[test]
def test_gpu_resident_hybrid_two_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: the resident prefill across two windows (the conv tail and state carry the seam)") @(t : T?) {
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

// the conv-tail edge: a prompt one past the window (the last window would carry one row, fewer
// than the three conv taps - the chain keeps it at the taps by shortening the window before), and
// a prompt shorter than the taps (one window, the ring's leading rows zero)
[test]
def test_gpu_resident_hybrid_short_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: a prompt one past the window - the last window still carries the conv taps") @(t : T?) {
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 1l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
    t |> run("Qwen3.5-0.8B: a two-token prompt - one window shorter than the conv taps") @(t : T?) {
        hybrid_forced_feed(t, 2l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

[test]
def test_gpu_resident_hybrid_kq(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M: K-quant deltanet planes on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M: the same across two windows") @(t : T?) {
        window_pin(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
}

[test]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: K-quant qkv/z with a Q8_0 out plane on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: the same across two windows") @(t : T?) {
        window_pin(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
}

// model-free: the routing witness stands on the census answering by name - a name nothing seeded panics instead of reading a zero count
[test]
def test_kernel_census_by_name(t : T?) {
    t |> run("the kernel census refuses a name no kernel seeded") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var tripped = false
            try {
                vk_kernel_coverage_of("no_such_kernel_spv")
            } recover {
                tripped = true
            }
            t |> success(tripped, "an unseeded census name panics, so a misspelt key cannot read as a zero count")
            t |> equal(vk_kernel_coverage_of("cls_q8k_rq_spv") >= 0l, true, "a seeded class kernel answers by its SPIR-V name, device or not")
        } else {
            t |> skip("no vulkan module in this build")
        }
    }
}
