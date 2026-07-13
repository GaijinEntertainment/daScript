options gen2
options persistent_heap   // + explicit deletes: the model frees before the process exits

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — pulls the engine + arch registrations + (on Apple) the metal prefill override
require dasllama/dasllama_math   // set_wscale_f16 — the resident path reads the f32 scale plane
require ?das_metal dasllama/dasllama_metal_prefill  // nolint:STYLE030 — min-npos setter + stats + the shutdown/leak rail
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — metal_live_object_count for the leak gate
require daslib/jobque_boost
require daslib/fio  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require strings
require _model_tier

// End-to-end gate for the dasMetal whole-prefill override (Phase 6): greedy continuation of a
// confident counting prompt, GPU-resident prefill vs the CPU control ON THE SAME BINARY — decode
// is the identical CPU path in both runs, so any prefill-induced logit shift beyond the shared
// tolerance envelope surfaces as a token flip. The counting prompt is the kernel-robust fixture
// family test_parity.das established (high-entropy prompts flip on genuine near-ties, which are
// not bugs). Skips cleanly without the model, off-Apple, or with no Metal device.

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

// "1, 2, …, 120," — long enough to engage the resident path with margin (~300 llama-3 BPE tokens)
def private counting_prompt() : string {
    return build_string() $(var w) {
        for (i in range(1, 121)) {
            w |> write(i)
            w |> write(i < 120 ? ", " : ",")
        }
    }
}

def private continuation(t : Model; prompt : array<int64>; n_gen : int) : array<int64> {
    var got : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(t.config)
        let steps = length(prompt) + n_gen
        let ids <- generate(t, s, prompt, steps)
        let echo = length(prompt) - 1
        got <- [for (i in range(n_gen)); ids[echo + i]; where echo + i < length(ids)]
        delete s
    }
    return <- got
}

[test]
def test_metal_prefill_parity(t : T?) {
    t |> run("GPU-resident prefill == CPU control, 40-token greedy continuation (Llama-3.2-1B Q8)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            let prior_wscale = get_wscale_f16()             // process-wide knobs — restored below
            let prior_backend = active_kernel_backend()     // (both exit paths run through the restore)
            set_wscale_f16(false)       // the resident path reads the f32 scale plane (declines on s16)
            pin_kernel_backend("portable")  // ...and ROW-MAJOR weights — a repack backend's layout is unreadable (5c donor contract)
            var tr <- load_model(path, QuantMode.q8)
            if (tr.config.seq_len > 2048l) {
                tr.config.seq_len = 2048l
            }
            let prompt <- encode(tr, counting_prompt(), true, false)
            let cpu <- continuation(tr, prompt, 40)
            t |> equal(length(cpu), 40)
            t |> success(select_prefill_override("metal"), "override registered")
            set_metal_prefill_min_npos(1l)
            let before = metal_prefill_stats().prefills
            let gpu <- continuation(tr, prompt, 40)         // default = lcpp mul_mm (f32 activations)
            let served = metal_prefill_stats().prefills - before
            set_metal_prefill_mulmm_legacy(true)            // the quantized path — bit-identical to CPU
            let before2 = metal_prefill_stats().prefills
            let gpu_legacy <- continuation(tr, prompt, 40)
            let served2 = metal_prefill_stats().prefills - before2
            set_metal_prefill_mulmm_legacy(false)
            select_prefill_override("")
            set_metal_prefill_min_npos(64l)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
            if (served == 0l || served2 == 0l) {
                metal_prefill_shutdown()
                delete tr
                feint("metal prefill declined (no Metal device?); GPU parity skipped\n")
                return
            }
            t |> success(same(cpu, gpu_legacy), "40-token continuation matches the CPU control (legacy quantized GEMM)")
            t |> success(same(cpu, gpu), "40-token continuation matches the CPU control (lcpp mul_mm, f32 activations)")
            metal_prefill_shutdown()
            t |> equal(metal_live_object_count(), 0l)
            delete tr
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}
