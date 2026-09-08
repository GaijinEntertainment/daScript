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
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW and the resolved coopmat mode, where the module exists
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// The whole-model resident driver on a deltanet hybrid (Qwen3.5-0.8B-Q8_0 under DASLLAMA_GPU=1):
// the resident window chain prefills the prompt (the recurrent layers through the conv + chunked
// scan over their device state, the gated partial-rope attention over the mirror), then the
// resident decode steps every layer - the recurrent ones through the fused deltanet step - as one
// submit per token. The GPU chains and the CPU chain are not bit-identical (GPU exp / f32
// accumulation order), so the gate is the forced-feed logits-tolerance form: the same fixed tokens
// fed to both arms, the logits within a bar of the CPU chain after the prefill and at every step,
// and the one-step-off control that must exceed it. Four prompt lengths: one window, two windows
// (the conv tail and the state carry across the window seam), one past the window, and two tokens
// (both conv-tail edges: the last window shorter than the taps).

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit| - the deltanet bar test_deltanet.das holds too
//! the K-quant bar on the f16 mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit at every step (the Q8 file 0.19-0.36), with no growth and the one-step-off control well past it
let private DN_LOGIT_BAR_REL_KQ = 0.06
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT = 0.10
let private PF_WINDOW_ROWS = 512l     //! the engine's PF_WINDOW, asserted equal by the two-window cells: a prompt past it runs as sequential windows

//! the served tile arm, so a red names which prefill tile it came from
def private tile_arm_tag() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return "coopmat mode {g_gpu.coopmat_mode}"
    } else {
        return "no vulkan module"
    }
}

def private window_pin(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
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

//! the coverage census's count of Q8_K requant dispatches so far; -1 with no vulkan module
def private q8k_requants() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_dispatches("cls_q8k_rq_spv")
    } else {
        return -1l
    }
}

//! true where the prefill's f16 feed admits the K-quant planes: an NV_coopmat2 device, or the KHR kq tile at subgroup 32
def private kq_coopmat_feed() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu.coopmat_mode == COOPMAT_CM2 || (g_gpu.coopmat_mode == COOPMAT_MM && khr_kq_tile_on())
    } else {
        return false
    }
}

struct FedLogits {
    rows : array<array<float>>   //! after the prefill, then after each fed step
    prefill_q8k_rq : int64       //! Q8_K requants the prefill alone dispatched (-1 with no vulkan module)
}

// prefill `prompt`, then feed `toks` one per step; the logits after the prefill and after each step
def private feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : FedLogits {
    let n = long_length(prompt)
    var out : FedLogits
    out.rows |> reserve(long_length(toks) + 1l)
    let rq0 = q8k_requants()
    forward_prefill(tr, s, prompt, n, 0l)
    out.prefill_q8k_rq = rq0 < 0l ? -1l : q8k_requants() - rq0
    out.rows |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out.rows |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- out
}

// prefill `n` tokens + 8 fed steps on the resident driver, then the same on the all-CPU chain; the
// prefill logits and every step's within the deltanet bar, the one-step-off control past it.
def private hybrid_forced_feed(t : T?; n : int64; file : string; kq : bool; q8out : bool = false) {
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
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = moe_gpu_resident_prefills()
        var gpu <- feed_logits(tr, sg, prompt, toks)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt")
        if (kq && gpu.prefill_q8k_rq >= 0l) {
            // the x feed's routing witness: a recurrent layer's qkv/z pair rides the coopmat tiles off an f16 feed,
            // or requantizes the block input to Q8_K for the sdot4 tile - one requant per layer per window
            to_log(LOG_INFO, "prefill ({n} tokens) [{tile_arm_tag()}]: {gpu.prefill_q8k_rq} Q8_K requants over {nrec} recurrent layers, coopmat feed {kq_coopmat_feed()}\n")
            if (kq_coopmat_feed()) {
                t |> success(gpu.prefill_q8k_rq < nrec, "on the coopmat feed the prefill never requantized the block input to Q8_K for the qkv/z pair ({gpu.prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            } else {
                t |> success(gpu.prefill_q8k_rq >= nrec, "off the coopmat feed the K-quant qkv/z pair reads the Q8_K form ({gpu.prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers) [{tile_arm_tag()}]")
            }
        }
        delete sg
        moe_gpu_drop_model()   // every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        var cpu <- feed_logits(tr, sc, prompt, toks)
        delete sc
        let rel = kq ? (kq_coopmat_feed() ? DN_LOGIT_BAR_REL_KQ : DN_LOGIT_BAR_REL_KQ_QUANT) : DN_LOGIT_BAR_REL
        for (st in range64(long_length(cpu.rows))) {
            let diff = logits_maxdiff(gpu.rows[st], cpu.rows[st])
            let bar = rel * logits_maxabs(cpu.rows[st])
            let ag = argmax_of(gpu.rows[st])
            let ac = argmax_of(cpu.rows[st])
            let what = (st == 0l ? "prefill ({n} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
            to_log(LOG_INFO, "{what}: argmax cpu {ac} ({cpu.rows[st][ac]}), resident {ag} ({gpu.rows[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
            t |> success(diff <= bar, "{what} logits: resident within {bar} of the CPU chain (maxdiff {diff})")
            if (st > 0l) {   // the control: the previous step's CPU logits are a disconnected mechanism the bar must RED
                let off = logits_maxdiff(gpu.rows[st], cpu.rows[st - 1l])
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
def test_gpu_resident_hybrid_decode(t : T?) {
    t |> run("Qwen3.5-0.8B: resident prefill + decode with deltanet layers on device vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q8_0.gguf", false)
    }
}

[test]
def test_gpu_resident_hybrid_two_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: the resident prefill across two windows (the conv tail and state carry the seam)") @(t : T?) {
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q8_0.gguf", false)
    }
}

// the conv-tail edge: a prompt one past the window (the last window would carry one row, fewer
// than the three conv taps - the chain keeps it at the taps by shortening the window before), and
// a prompt shorter than the taps (one window, the ring's leading rows zero)
[test]
def test_gpu_resident_hybrid_short_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: a prompt one past the window - the last window still carries the conv taps") @(t : T?) {
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 1l, "Qwen3.5-0.8B-Q8_0.gguf", false)
    }
    t |> run("Qwen3.5-0.8B: a two-token prompt - one window shorter than the conv taps") @(t : T?) {
        hybrid_forced_feed(t, 2l, "Qwen3.5-0.8B-Q8_0.gguf", false)
    }
}

[test]
def test_gpu_resident_hybrid_kq(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M: K-quant deltanet planes on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M.gguf", true)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M: the same across two windows") @(t : T?) {
        window_pin(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M.gguf", true)
    }
}

// the mixed twin (K-quant qkv/z, the out plane Q8_0 - the unsloth UD files' shape): a recurrent
// layer whose x feed and o feed part ways on the KHR arm - the kq tiles for qkv/z, the q8 route for out
[test]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: K-quant qkv/z with a Q8_0 out plane on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", true, true)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: the same across two windows") @(t : T?) {
        window_pin(t)
        hybrid_forced_feed(t, PF_WINDOW_ROWS + 88l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", true, true)
    }
}
