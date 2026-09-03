options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each turn frees its model before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_gpu_tier   // moe_gpu_installed_marks_armed: did the per-op tier take the model
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: or the whole-model driver
require daslib/jobque_boost
require daslib/fio
require math
require strings
require _model_tier

// Two models through one process on the armed GPU tier: each model, loaded behind the other,
// decodes its own weights. The witness is the second model's greedy tokens after a swap
// against its own first-in-process tokens: same box, same kernels, so equality is exact.

let SWAP_MODEL_A = path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf")
let SWAP_MODEL_B = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")

def private greedy_turn(tag : string; path : string; n_tokens : int64) : array<int64> {
    var ids : array<int64>
    var pieces : array<string>
    var m <- load_model_(path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, 256l)
    with_job_que() {
        setup_dasllama_jobque()
        var s <- make_run_state(m.config)
        var prompt <- encode(m, "Once upon a time, in a small village", true, false)
        generate(m, s, prompt, SamplingParams(), n_tokens) $(id, piece) {
            ids |> push(id)
            pieces |> push(piece)
            return true
        }
        delete prompt
        delete s
    }
    delete m
    let text = build_string() $(var w) {
        for (p in pieces) {
            w |> write(p)
        }
    }
    to_log(LOG_INFO, "{tag}: \"{text}\" {ids}\n")
    delete pieces
    return <- ids
}

// Model-free: the drop the upload rail runs before a load clears the marks a deleted model left
// and keeps the load's own MoE layer request, the one mark the whole-model drop resets.
[test]
def test_gpu_drop_before_load_keeps_request(t : T?) {
    t |> run("the pre-load drop clears a stale model's marks and keeps the MoE layer request") @(t : T?) {
        if (!moe_gpu_tier_installed()) {
            t |> skip("no GPU tier installed on this box")
            return
        }
        set_moe_gpu_dense_off(0, 4096l)   // a stale model's mark
        set_moe_gpu_layer_count(3l)   // this load's request
        t |> success(moe_gpu_installed_marks_armed(), "the stale mark reads as an installed model")
        moe_gpu_drop_installed_before_load(8l)
        t |> success(!moe_gpu_installed_marks_armed(), "the drop cleared the stale model's marks")
        t |> equal(moe_gpu_layer_count(), 3l, "the MoE layer request survives the drop")
        set_moe_gpu_layer_count(0l)
    }
}

[test]
def test_gpu_model_swap(t : T?) {
    t |> run("a second model loaded after the first is deleted decodes its own weights (needs Qwen3-0.6B + SmolLM2-135M, DASLLAMA_GPU=1)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!model_available(t, SWAP_MODEL_A) || !model_available(t, SWAP_MODEL_B)) {
            return
        }
        var a_first <- greedy_turn("A first", SWAP_MODEL_A, 8l)
        let armed = moe_gpu_installed_marks_armed() || moe_gpu_resident_active()
        if (!gpu_want_auto()) {
            t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the model")
            delete a_first
            return
        }
        t |> success(armed, "DASLLAMA_GPU=1: the tier took model A (marks or the resident driver)")
        let a_resident_first = moe_gpu_resident_active()
        var b_first <- greedy_turn("B first", SWAP_MODEL_B, 8l)
        var a_after_b <- greedy_turn("A behind B", SWAP_MODEL_A, 8l)
        t |> equal(moe_gpu_resident_active(), a_resident_first, "model A's second load armed the same driver as its first: one code path")
        var b_after_a <- greedy_turn("B behind A", SWAP_MODEL_B, 8l)
        t |> success(ids_equal(a_after_b, a_first), "model A decodes its own weights after model B was loaded in between ({a_after_b} vs {a_first})")
        t |> success(ids_equal(b_after_a, b_first), "model B decodes its own weights after model A was loaded in between ({b_after_a} vs {b_first})")
        delete a_first
        delete b_first
        delete a_after_b
        delete b_after_a
    }
}
