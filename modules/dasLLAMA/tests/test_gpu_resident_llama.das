options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (llama)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW and the resolved coopmat mode, where the module exists
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // tile_arm_tag, argmax_of, feed_logits
require dasllama/dasllama_bench   // bench_tg_batched_rep: the batched row's refusal and its served rep

// The whole-model resident driver on the llama family (`DASLLAMA_GPU=1`): the NORM rope (adjacent pairs),
// no q/k/v bias, no q/k norm, GQA, the tied classifier of the 3.2 files. The forced-feed logits-tolerance
// form of test_gpu_resident_qwen2: the same fixed tokens through the resident chain and the all-CPU chain,
// the prefill logits and every step's within the bar, the one-step-off control past it. Three carriers -
// Llama-3.2-1B Q8_0, Llama-3.2-3B Q8_0, Llama-3.1-8B Q4_K_M (the batched step's Q8_0 and K-quant planes) -
// at one window and two windows; the 1B also runs the batched bench row's refusal and one served rep.
// Every cell skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, or without its file
// under models_dir() (the 8B is large-tier: DASLLAMA_PARITY_FULL=1).

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}

//! of the step's max |logit|: the qwen2 file's K-quant bar
let private LOGIT_BAR_REL = 0.06
let private PF_WINDOW_ROWS = 512l  //! the engine's PF_WINDOW, asserted equal by the two-window cell: a prompt past it runs as sequential windows

//! the carrier's gate: the JIT, the armed tier and the file; "" where a cell skips
def private llama_ready(t : T?; file : string) : string {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return ""
    }
    let path = path_join(models_dir(), file)
    if (!model_available(t, path)) return ""
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return ""
    }
    return path
}

//! `file` through the resident chain and the CPU chain over an `n`-token prompt and eight steps
def private llama_forced_feed(t : T?; file : string; n : int64) {
    let path = llama_ready(t, file)
    if (empty(path)) return
    var tr <- load_model_(path, QuantMode.q8)
    t |> equal(tr.arch, "llama", "the file is a llama-family model")
    t |> success(!tr.config.attn_qkv_bias && !tr.config.rope_neox, "the llama arm: no q/k/v bias, the NORM rope")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
    t |> equal(get_jobque_spin_in_force(), get_jobque_spin_gpu_us(), "the armed driver put the GPU-served spin window in force")
    with_job_que() {
        setup_dasllama_jobque_()
        var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
        var toks <- [for (i in range64(8l)); 2000l + (i * 53l) % 4000l]
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = moe_gpu_resident_prefills()
        var gpu <- feed_logits(tr, sg, prompt, toks)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt")
        var passes <- gpu_cpu_passes_()
        t |> equal(length(passes), 0, "the device served every call of the resident run: {passes}")
        delete passes
        delete sg
        moe_gpu_drop_model()   //! every later step is the CPU chain
        t |> equal(get_jobque_spin_in_force(), get_jobque_spin_us(), "the dropped driver handed the pool the CPU window back")
        var sc <- create_session(tr, mirror_kv_dtype())
        var cpu <- feed_logits(tr, sc, prompt, toks)
        delete sc
        for (st in range64(long_length(cpu))) {
            let diff = logits_maxdiff(gpu[st], cpu[st])
            let bar = LOGIT_BAR_REL * logits_maxabs(cpu[st])
            let ag = argmax_of(gpu[st])
            let ac = argmax_of(cpu[st])
            let what = (st == 0l ? "prefill ({n} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
            to_log(LOG_INFO, "{file} {what}: argmax cpu {ac} ({cpu[st][ac]}), resident {ag} ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
            t |> success(diff <= bar, "{file} {what} logits: resident within {bar} of the CPU chain (maxdiff {diff})")
            if (st > 0l) {   //! the control: the previous step's CPU logits are a disconnected mechanism the bar must RED
                let off = logits_maxdiff(gpu[st], cpu[st - 1l])
                t |> success(off > bar, "{file} {what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
            }
        }
        delete gpu
        delete cpu
        delete prompt
        delete toks
    }
    delete tr
}

def private llama_cells(t : T?; file : string) {
    t |> run("{file}: resident prefill + decode vs the CPU chain, one window") @(t : T?) {
        llama_forced_feed(t, file, 40l)
    }
    t |> run("{file}: the same across two windows") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
        }
        llama_forced_feed(t, file, PF_WINDOW_ROWS + 88l)
    }
}

[test]
def test_gpu_resident_llama_1b(t : T?) {
    llama_cells(t, "Llama-3.2-1B-Instruct-Q8_0.gguf")
}

//! the batched bench row on the 1B: a refusal names the streams the device homes against the row's ask and
//! reads 0, and a rep the device can home serves - a rate, no refusal, every stream parked after it
[test]
def test_gpu_resident_llama_batched_row(t : T?) {
    t |> run("bench_tg_batched_rep: the refusal past the homed streams, then one served rep") @(t : T?) {
        let path = llama_ready(t, "Llama-3.2-1B-Instruct-Q8_0.gguf")
        if (empty(path)) return
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
        with_job_que() {
            setup_dasllama_jobque_()
            let homes = gpu_device_sessions()
            t |> success(homes >= 1l, "the armed driver homes a stream ({homes})")
            let refused = bench_tg_batched_rep(tr, homes + 63l, 32l, 4l, 1l)
            t |> equal(refused.tps, 0.0lf, "a row asking more streams than the device homes reads 0")
            t |> equal(refused.why, "the device homes {homes} stream(s), the row asks {homes + 63l}", "... and names the refusal")
            let served = bench_tg_batched_rep(tr, homes, 32l, 4l, 1l)
            t |> equal(served.why, "", "a row the device homes is served: {served.why}")
            t |> success(served.tps > 0.0lf, "... at a rate ({served.tps} tok/s summed over {homes} stream(s))")
            t |> equal(gpu_device_sessions(), homes, "the rep parked every stream: the device homes as many as before")
            var passes <- gpu_cpu_passes_()
            t |> equal(length(passes), 0, "the device served every call of the rep: {passes}")
            delete passes
        }
        delete tr
    }
}

[test]
def test_gpu_resident_llama_3b(t : T?) {
    llama_cells(t, "Llama-3.2-3B-Instruct-Q8_0.gguf")
}

[test]
def test_gpu_resident_llama_8b_k(t : T?) {
    llama_cells(t, "Meta-Llama-3.1-8B-Instruct-Q4_K_M.gguf")
}
