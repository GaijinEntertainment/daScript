options gen2
options persistent_heap   // + explicit deletes: the models free before the process exits
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 — the public facade (engine + arch registrations + the metal overrides); only the metal half uses more than dasllama_common
require dasllama/dasllama_math   // set_wscale_f16 — the s16 axis
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_common  // the coverage census rail
require ?das_metal dasllama/dasllama_metal_decode  // decode stats + shutdown rails
require ?das_metal metal/das_metal_boost  // device probe
require ?vulkan dasllama/dasllama_vulkan_common  // the vulkan census rail (spec-only here)
require daslib/jobque_boost
require daslib/fio  // the zoo paths
require math  // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half
require strings
require _model_tier
require dasllama/dasllama_env

// The KERNEL COVERAGE census (CODEREVIEW rule 17): drive the small-model zoo across the
// format / graph / batch-shape / KV-codec axes and report which compiled kernels were never
// dispatched — LOUD WARNINGS, never an auto-dead verdict. A zero means "nothing this zoo runs
// dispatched it", never "unreachable" — deleting a kernel additionally needs a reachability
// audit of its dispatch predicate (models/quants/shapes not stocked here still count). This
// suite is the gate a kernel deletion must run before ("was it ever dispatched by anything
// we run?") and the home a NEW kernel's small-model run joins. Expected warnings on the small tier: MoE / mx4 / suppress and other
// kernels whose carriers sit above the tier (their census rows serve under
// DASLLAMA_PARITY_FULL=1); the report stays honest about what THIS run covered. The vulkan
// half here is the device-free rail unit (spec parse + zero table); the serving vulkan census
// runs on the PC box beside the vulkan tier suites.

def private counting_prompt(start, last : int) : string {
    return build_string() $(var w) {
        for (i in range(start, last + 1)) {
            w |> write(i)
            w |> write(i < last ? ", " : ",")
        }
    }
}

def private greedy_top(lg : array<float>; vocab : int64) : int64 {
    var best = 0l
    for (v in range64(1l, vocab)) {
        if (lg[v] > lg[best]) {
            best = v
        }
    }
    return best
}

[unused_argument(t, path, family, kdts, nbs)]   // off-Apple the gated body is empty
def private cov_model(t : T?; path, family : string; kdts : array<KVDtype>; nbs : array<int>) {
    // one model's coverage sweep: per KV codec — GPU prefill (the ~120-id counting prompt
    // clears the npos gate), two greedy decode steps, then one batched step per requested B.
    // No parity asserts here — the census wants DISPATCH DIVERSITY, the parity suites own truth.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, family) || !arm_on(t, "coverage") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        if (!convert_model_to_metal_blob(tr)) {
            t |> failure("{base_name(path)}: metal-blob transform failed")
            delete tr
            return
        }
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let vocab = tr.config.vocab_size
        select_prefill_override("metal")
        select_decode_override("metal")
        with_job_que() {
            setup_dasllama_jobque()
            for (kdt in kdts) {
                var s = make_run_state(tr.config, kdt, kdt)
                let p <- encode(tr, counting_prompt(1, 40), true, false)
                eval_(tr, s, p)
                var cur = greedy_top(s.logits, vocab)
                for (_i in range(2)) {
                    var one <- [cur]
                    eval_(tr, s, one)
                    delete one
                    cur = greedy_top(s.logits, vocab)
                }
                // continuation prefill (start_pos > 0) — the pf window-landing utilities only
                // serve when a second window lands on an already-warm cache
                var p2 <- encode(tr, counting_prompt(50, 90), false, false)
                eval_(tr, s, p2)
                delete p2
                delete s
                for (nb in nbs) {
                    var ss : array<Session>
                    ss |> reserve(nb)
                    var sess : array<Session?>
                    var curs : array<int64>
                    for (i in range(nb)) {
                        ss |> emplace <| make_run_state(tr.config, kdt, kdt)
                        let pb <- encode(tr, counting_prompt(3 + i, 44 + i), true, false)
                        eval_(tr, ss[i], pb)
                        unsafe {
                            sess |> push(addr(ss[i]))
                        }
                        curs |> push(greedy_top(ss[i].logits, vocab))
                    }
                    var ws <- create_batch_workspace_(tr)
                    eval_batch_(tr, ws, sess, curs)
                    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
                    unsafe {
                        delete ws
                        delete sess
                    }
                    delete curs
                    delete ss
                }
            }
        }
        select_prefill_override("")
        select_decode_override("")
        delete tr
    }
}

def private census_zoo(t : T?) {
    // llama: the dim-ladder base — all three KV codecs, the B=2/4/8 batch shapes
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "llama",
        [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4], [2, 4, 8])
    // the s16 axis (f16 weight-scale plane) on the same carrier
    set_wscale_f16(true)
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "llama",
        [KVDtype.f16], [2])
    set_wscale_f16(false)
    // the q5_K tail on the same carrier — Q5_K_M rows drive the k5 GEMV/mulmm/batch tiles
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q5_K_M.gguf"), "llama",
        [KVDtype.f16], [2, 4, 8])
    // gemma2: K-quant weights x softcap/sliding/geglu/wide heads; B=4/8 drive the
    // k4/k6 batch tiles
    cov_model(t, path_join(models_dir(), "gemma-2-2b-it-Q4_K_M.gguf"), "gemma2",
        [KVDtype.f16, KVDtype.q8_0], [2, 4, 8])
    // qwen3: qk_norm + qd != dim
    cov_model(t, path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf"), "qwen3",
        [KVDtype.f16, KVDtype.tq4], [2])
    // qwen2: qkv_bias
    cov_model(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"), "qwen2",
        [KVDtype.f16, KVDtype.q8_0], [2])
    // qwen3.5: the DeltaNet hybrid (batch = per-row fallback by design — still coverage)
    cov_model(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35",
        [KVDtype.f16], [2])
    // gemma4e: PLE + V-from-K (no PLE batch arm — a blob-twin CPU batch fallback
    // would trip the blob-only panic, so no batch shapes here); large tier
    let none : array<int>
    cov_model(t, path_join(models_dir(), "gemma-4-E4B-it-Q8_0.gguf"), "gemma4e",
        [KVDtype.f16], none)
    // gemma4: the checkpoint suppress list (suppress_tokens metadata rides the 12B
    // family, NOT the E-series) + K-quant at 12B shapes; large tier
    cov_model(t, path_join(models_dir(), "gemma-4-12B-it-Q4_K_M.gguf"), "gemma4",
        [KVDtype.f16], [2])
    // qwen3moe: the whole-graph GPU MoE family (router/bucket/select/combine +
    // expert GEMV/GEMMs); no batch shapes — MoE batch declines are graph-level and a
    // blob-twin CPU batch fallback would trip the blob-only panic; large tier
    cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe",
        [KVDtype.f16], none)
    // gptoss: the MXFP4 expert stacks + swiglu_oai + attention sinks — the only carrier for the
    // mx4 expert kernels; no batch shapes for the same reason as the other MoE rows; large tier
    cov_model(t, path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf"), "gptoss",
        [KVDtype.f16], none)
    // qwen2moe: q8 expert stacks + the sigmoid-gated shared expert on std attention; large tier
    cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"), "qwen2moe",
        [KVDtype.f16], none)
}

[test]
def test_metal_kernel_coverage(tst : T?) {
    tst |> run("Metal kernel coverage census: the small-model zoo across format/graph/batch/KV axes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!arm_on(t, "coverage")) {
                return
            }
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            var dev = metal_create_system_default_device()
            if (dev == null) {
                feint("no Metal device; kernel coverage census skipped\n")
                return
            }
            metal_release(dev)
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            set_wscale_f16(false)
            pin_kernel_backend("portable")   // row-major weights — the blob transform's donor contract
            metal_kernel_coverage_reset()
            census_zoo(t)
            // the census verdict: the report prints counts + LOUD WARNINGS for the zeros;
            // the assert pins only that the RAIL counts — coverage gaps are human calls
            metal_kernel_coverage_report()
            var cov <- metal_kernel_coverage()
            var served = 0
            for (n in values(cov)) {
                if (n > 0l) {
                    served++
                }
            }
            let famfilter = g_env_test.test_family
            if (empty(famfilter)) {
                t |> success(served >= 40,
                    "the census counted {served} distinct dispatched kernels (rail floor 40)")
            } else {
                // a family filter narrows the zoo by design — the floor only holds unfiltered
                to_log(LOG_INFO, "census floor not asserted (family filter '{famfilter}'): {served} served\n")
            }
            delete cov
            metal_decode_shutdown()
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
        } else {
            feint("das_metal is not built on this platform; nothing to census\n")
        }
    }
}

[test]
def test_vulkan_kernel_coverage_rail(tst : T?) {
    tst |> run("Vulkan kernel coverage rail: the lens spec seeds the census (device-free)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!arm_on(t, "coverage")) {
                return
            }
            // no device, no dispatches — the unit pins that the compile-time lens spec parses
            // into the census universe and every kernel starts at zero. The serving census runs
            // on the PC box beside the vulkan tier suites.
            var cov <- vk_kernel_coverage()
            t |> success(length(cov) >= 40, "the lens spec seeds {length(cov)} kernels (floor 40)")
            var zeros = 0
            for (n in values(cov)) {
                if (n == 0l) {
                    zeros++
                }
            }
            t |> equal(zeros, length(cov))
            delete cov
        } else {
            feint("vulkan is not built on this platform; nothing to census\n")
        }
    }
}
