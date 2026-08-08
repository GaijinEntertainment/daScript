options gen2
options persistent_heap   // explicit deletes below — the model frees before the test exits
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math
require dasllama/dasllama_gpu_resident   // the residency rail this suite swaps: arm / hydrate / drop
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 — only used inside the builtin_module_exists(vulkan) static_if; dead on a no-vulkan lint binary
require _model_tier

// The multi-model switch cycle on a REAL model + the resident driver: prefill + decode on
// device, hydrate + whole-model drop, CPU continuation over the hydrated host KV, re-arm, a
// fresh device trajectory — every leg must stay token-for-token on the control trajectory.
// Runs only where tinyllama + a Vulkan device are present (loud skip otherwise); the
// synthetic model-free teardown coverage is test_vulkan_tier's drop-model cycles.

def private synth_id(i : int64) : int64 => 1000l + ((i * 17l + 131l) % 5000l)

def private greedy_step(tr : Model; s : Session) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (v in range64(tr.config.vocab_size)) {
        if (s.logits[v] > bv) {
            bv = s.logits[v]
            best = v
        }
    }
    return best
}

def private run_ids(tr : Model; var s : Session; plen, ngen : int64; var ids : array<int64>) {
    let prompt <- [for (i in range64(plen)); synth_id(i)]
    forward_prefill(tr, s, prompt, plen, 0l)
    ids |> reserve(ngen)
    var pos = plen
    for (_g in range64(ngen)) {
        let best = greedy_step(tr, s)
        ids |> push(best)
        forward(tr, s, best, pos)
        pos++
    }
}

def private same_ids(a, b : array<int64>; a0, n : int) : bool {
    for (i in range(n)) {
        if (a[a0 + i] != b[i]) {
            return false
        }
    }
    return true
}

struct private SwapResult {
    armed0 : bool       // the resident driver armed at load (false = skip the rest, no device)
    dropped_off : bool  // moe_gpu_drop_model deactivated it
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
    var want = GpuTierWant()
    want.auto_tier = true
    set_gpu_tier_want(want)
    moe_gpu_tier_arm()
    with_job_que() {
        setup_dasllama_jobque_()
        var tr <- load_model_(path, QuantMode.q8)
        r.armed0 = moe_gpu_resident_active()
        if (r.armed0) {
            let plen = 64l
            // CTRL: the truth trajectory, straight through on the resident driver
            var sc = make_run_state(tr.config)
            var ctrl : array<int64>
            run_ids(tr, sc, plen, 32l, ctrl)
            // A: 16 resident tokens, hydrate + drop, 16 more on the CPU over the hydrated KV
            var sa = make_run_state(tr.config)
            var a1 : array<int64>
            run_ids(tr, sa, plen, 16l, a1)
            moe_gpu_hydrate_session(tr, sa)
            moe_gpu_drop_model()
            r.dropped_off = !moe_gpu_resident_active()
            var pos = plen + 16l
            var a2 : array<int64>
            for (_g in range64(16l)) {
                let best = greedy_step(tr, sa)
                a2 |> push(best)
                forward(tr, sa, best, pos)
                pos++
            }
            r.a_par = same_ids(ctrl, a1, 0, 16) && same_ids(ctrl, a2, 16, 16)
            // B: re-arm (the switch-in path) + a fresh trajectory
            let t0 = ref_time_ticks()
            set_gpu_tier_want(want)
            moe_gpu_tier_arm()
            moe_gpu_upload_resident(tr)
            r.rearm_us = int64(get_time_usec(t0))
            r.rearm_on = moe_gpu_resident_active()
            var sb = make_run_state(tr.config)
            var b : array<int64>
            run_ids(tr, sb, plen, 32l, b)
            r.b_par = same_ids(ctrl, b, 0, 32)
            // D: a second full drop/re-arm cycle (registry regrowth smoke)
            moe_gpu_drop_model()
            moe_gpu_upload_resident(tr)
            var sd = make_run_state(tr.config)
            var d : array<int64>
            run_ids(tr, sd, plen, 32l, d)
            r.d_par = same_ids(ctrl, d, 0, 32) && moe_gpu_resident_active()
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
        moe_gpu_drop_model()
        set_gpu_tier_want(GpuTierWant())
        delete tr
    }
    return r
}

[test]
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
