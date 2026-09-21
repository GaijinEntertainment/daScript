options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW and the resolved coopmat mode, where the module exists
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // resident_then_cpu_feed, feed_steps_within

// The whole-model resident driver on a qwen2 (Qwen2.5-0.5B-Instruct-Q8_0 under DASLLAMA_GPU=1): the
// q/k/v projection bias folds into the rope stage on the device. The forced-feed logits-tolerance
// form of test_gpu_resident_hybrid: the same fixed tokens through the resident chain and the
// all-CPU chain, the prefill logits and every step's within the bar, the one-step-off control past
// it - a dropped or misplaced bias moves the logits far past the bar (the bias is a large share of
// a 0.5B's k rows), so the bar is the bias arm's gate. Two prompt lengths: one window, two windows.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}

//! of the step's max |logit|: the 0.5B's CPU-vs-device rounding sits flat at 0.30-0.50 of a 9.5-18 max logit
//! at every step, while a dropped bias lands whole logits off - the hybrid file's K-quant bar.
let private LOGIT_BAR_REL = 0.06
let private PF_WINDOW_ROWS = 512l  //! the engine's PF_WINDOW, asserted equal by the two-window cell: a prompt past it runs as sequential windows

//! the served tile arm, so a red names which prefill tile it came from
def private tile_arm_tag() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return "coopmat mode {g_gpu.coopmat_mode}"
    } else {
        return "no vulkan module"
    }
}

def private qwen2_forced_feed(t : T?; n : int64) {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return
    }
    let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
    if (!model_available(t, path)) return
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return
    }
    var tr <- load_model_(path, QuantMode.q8)
    t |> success(tr.config.attn_qkv_bias, "the qwen2 carries the q/k/v projection bias the arm exists for")
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the biased model (the load log names any decline)")
    with_job_que() {
        setup_dasllama_jobque_()
        var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]
        var toks <- [for (i in range64(8l)); 2000l + (i * 53l) % 4000l]
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks) $ {}
        feed_steps_within(t, "", n, fed.gpu, fed.cpu, toks, LOGIT_BAR_REL)
        delete fed
        delete prompt
        delete toks
    }
    delete tr
}

[test]
def test_gpu_resident_qwen2_bias(t : T?) {
    t |> run("Qwen2.5-0.5B: resident prefill + decode with the q/k/v bias on device vs the CPU chain, one window") @(t : T?) {
        qwen2_forced_feed(t, 40l)
    }
    t |> run("Qwen2.5-0.5B: the same across two windows") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
        }
        qwen2_forced_feed(t, PF_WINDOW_ROWS + 88l)
    }
}
