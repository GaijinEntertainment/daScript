options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each turn frees its model before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier   // moe_gpu_installed_marks_armed: did the per-op tier take the model
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take it; arm / hydrate / drop: the slot swap's residency rail
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
    with_dasllama_jobque() {
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
[test(description = "The pre-load drop on an installed GPU tier clears a stale model's dense marks while keeping this load's MoE layer-count request")]
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

// Model-free: the marks save/restore pair carries the per-layer-embedding projection arm, so a
// model whose pre-step sits on the device does not take the CPU form as well after a slot switch.
[test(description = "Saving the GPU model marks turns the per-layer-embedding projection arm off and restoring them turns it back on")]
def test_gpu_marks_carry_ple_projection(t : T?) {
    t |> run("the marks swap carries the PLE projection arm: off across the save, back on the restore") @(t : T?) {
        var st <- gpu_model_marks_init()
        rdec_ple_proj_set(true)
        moe_gpu_model_marks_save(st)
        t |> success(!rdec_ple_proj_on(), "the save leaves the projection arm reading as no-model")
        moe_gpu_model_marks_restore(st)
        t |> success(rdec_ple_proj_on(), "the restore puts the projection arm back")
        rdec_ple_proj_set(false)
    }
}

[test(description = "On the armed GPU tier, Qwen3-0.6B and SmolLM2-135M loaded alternately in one process each reproduce their own first-in-process 8 greedy token ids exactly, with model A arming the same driver both times")]
def test_gpu_model_swap(t : T?) {
    t |> run("a second model loaded after the first is deleted decodes its own weights (needs Qwen3-0.6B + SmolLM2-135M, DASLLAMA_GPU=1)") @(t : T?) {
        return if (!jit_model_available(t, SWAP_MODEL_A) || !model_available(t, SWAP_MODEL_B))
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

// The multi-model switch cycle on a REAL model + the resident driver: prefill + decode on
// device, hydrate + whole-model drop, CPU continuation over the hydrated host KV, re-arm, a
// fresh device trajectory — every leg must stay token-for-token on the control trajectory.
// Runs only where tinyllama + a Vulkan device are present (loud skip otherwise); the
// synthetic model-free teardown coverage is test_vulkan_tier's drop-model cycles.

def private synth_id(i : int64) : int64 => 1000l + ((i * 17l + 131l) % 5000l)

def private run_ids(tr : Model; var s : Session; plen, ngen : int64) : array<int64> {
    let prompt <- [for (i in range64(plen)); synth_id(i)]
    forward_prefill(tr, s, prompt, plen, 0l)
    return <- greedy_from(tr, s, plen, ngen)
}

struct private SwapResult {
    armed0 : bool       // the resident driver armed at load (false = skip the rest, no device)
    dropped_off : bool  // moe_gpu_drop_model_ deactivated it
    rearm_on : bool     // re-arm re-activated it
    a_par : bool        // hydrate + drop + CPU continuation stayed on the control trajectory
    b_par : bool        // post-re-arm fresh trajectory matches token-for-token
    d_par : bool        // second drop/re-arm cycle matches token-for-token
    rearm_us : int64
}

// Kept free of T? so the whole engine chain stays JIT-served (the dastest class would pin it
// to the interpreter stack).
def private slot_swap_cycle(path : string) : SwapResult {
    var r = SwapResult()
    // the facade's alias and the struct both answer the bare name here, so the type is spelled
    var want : dasllama_gpu_tier::GpuTierWant
    want.auto_tier = true
    set_gpu_tier_want_(want)
    moe_gpu_tier_arm_()
    with_dasllama_jobque_() {
        var tr <- load_model_(path, QuantMode.q8)
        r.armed0 = moe_gpu_resident_active()
        if (r.armed0) {
            let plen = 64l
            // CTRL: the truth trajectory, straight through on the resident driver
            var sc = make_run_state(tr.config)
            var ctrl <- run_ids(tr, sc, plen, 32l)
            // A: 16 resident tokens, hydrate + drop, 16 more on the CPU over the hydrated KV
            var sa = make_run_state(tr.config)
            var a1 <- run_ids(tr, sa, plen, 16l)
            moe_gpu_hydrate_session_(tr, sa)
            moe_gpu_drop_model_()
            r.dropped_off = !moe_gpu_resident_active()
            var a2 <- greedy_from(tr, sa, plen + 16l, 16l)
            r.a_par = ids_equal(copy_span(ctrl, 0, 16), a1) && ids_equal(copy_span(ctrl, 16, 16), a2)
            // B: re-arm (the switch-in path) + a fresh trajectory
            let t0 = ref_time_ticks()
            set_gpu_tier_want_(want)
            moe_gpu_tier_arm_()
            moe_gpu_upload_resident(tr)
            r.rearm_us = int64(get_time_usec(t0))
            r.rearm_on = moe_gpu_resident_active()
            var sb = make_run_state(tr.config)
            var b <- run_ids(tr, sb, plen, 32l)
            r.b_par = ids_equal(copy_span(ctrl, 0, 32), b)
            // D: a second full drop/re-arm cycle (registry regrowth smoke)
            moe_gpu_drop_model_()
            moe_gpu_upload_resident(tr)
            var sd = make_run_state(tr.config)
            var d <- run_ids(tr, sd, plen, 32l)
            r.d_par = ids_equal(copy_span(ctrl, 0, 32), d) && moe_gpu_resident_active()
            delete sc
            delete sa
            delete sb
            delete sd
            delete ctrl
            delete a1
            delete a2
            delete b
            delete d
        }
        // leave the device bare + the process-global want clear for later tests in the sweep
        moe_gpu_drop_model_()
        let no_want : dasllama_gpu_tier::GpuTierWant
        set_gpu_tier_want_(no_want)
        delete tr
    }
    return r
}

[test(description = "On TinyLlama Q8_0 a resident run hydrated and dropped mid-stream continues on the CPU, and two drop and re-arm cycles each emit the straight-through resident control trajectory token for token")]
def test_gpu_slot_swap(t : T?) {
    t |> run("resident drop/re-arm slot swap - IDS parity across two cycles (needs tinyllama q8 + vulkan)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            let r = slot_swap_cycle(path)
            if (!r.armed0) {
                t |> skip("resident driver did not arm (no vulkan device, or budget refused)")
                return
            }
            t |> success(r.dropped_off, "drop deactivates the resident driver")
            t |> success(r.a_par, "hydrated CPU continuation stays on the control trajectory")
            t |> success(r.rearm_on, "re-arm re-activates the resident driver ({r.rearm_us / 1000l}ms)")
            t |> success(r.b_par, "post-re-arm trajectory matches token-for-token")
            t |> success(r.d_par, "second drop/re-arm cycle matches token-for-token")
        } else {
            feint("dasVulkan not installed; slot swap untestable here\n")
        }
    }
}
