options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // explicit deletes: each arm frees its model before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // parity_check — the board pregate's verdict, driven with a controlled spec
require ../performance/model_specs.das   // spec_by_file — the table rows the pregate binds
require dasllama/dasllama_common       // get_kquant_native — the mode the kq arm must restore
require dasllama/dasllama_load          // load_model_ — the slice cell loads its own carrier
require dasllama/dasllama_math          // setup_dasllama_jobque_ — the slice cell's queue (engine-level, not re-exported)
require dasllama/dasllama_gpu_resident  // moe_gpu_resident_active — the slice cell needs the driver armed
require ?vulkan dasllama/dasllama_vulkan_prefill   // g_pf_ffn_slice — the slice's A/B arm
require daslib/jobque_boost             // with_job_que — a 512-token prefill dispatches its CPU stages as jobs
require daslib/fio
require _model_tier                    // models_dir + model_available (presence/size gate)

// The board parity pregate (lcpp_bench --parity), fed a controlled spec: the real fixture
// passes, a deliberately wrong fixture fails, an evidence-less spec is refused, the text-form
// prompt encodes through the tokenizer, and the kq-native load arm engages. Small carriers
// only; every arm skips honestly when its model is absent. Requires the bench by relative
// path (like test_tok_seed), so it pays the bench's full engine compile.

[test]
def test_parity_pregate_ids_form(t : T?) {
    t |> run("the pinned-ids fixture passes; one flipped id fails; no evidence refuses") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        var inscope spec <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        t |> success(parity_check(path, spec), "the table's own fixture reproduces")
        var inscope wrong <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
        wrong.parity.gen[length(wrong.parity.gen) - 1]++
        t |> success(!parity_check(path, wrong), "a flipped last id is a mismatch")
        var inscope bare <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
        bare.parity.gen |> clear()
        t |> success(!parity_check(path, bare), "no evidence is refused")
    }
}

[test]
def test_parity_pregate_text_form(t : T?) {
    t |> run("a text-form fixture encodes through the model's tokenizer") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        var inscope spec <- spec_by_file("gemma-4-E2B-it-Q8_0.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        t |> success(!empty(spec.parity.prompt_text), "the E2B fixture is the text form")
        t |> success(parity_check(path, spec), "the encoded prompt reproduces the pinned continuation")
    }
}

[test]
def test_parity_pregate_kq_native(t : T?) {
    t |> run("a kq_native fixture loads native K-quant planes and reproduces") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        var inscope spec <- spec_by_file("Qwen3-4B-Instruct-2507-Q4_K_M.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        t |> success(spec.parity.kq_native, "the Q4_K_M row declares the kq-native arm")
        let before = get_kquant_native()
        t |> success(parity_check(path, spec), "the kq-native load reproduces the pinned continuation")
        t |> equal(get_kquant_native(), before, "the pregate restores the kq-native mode it flipped")
    }
}

// The last layer's FFN slice against the whole window: one 512-token prefill each way on the
// resident Vulkan driver, logits exact - the frozen fixtures' 5-20-token prompts never reach it.
[test]
def test_parity_last_layer_slice(t : T?) {
    t |> run("the last layer's 32-row FFN slice reproduces the whole window's logits bit for bit") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            var inscope spec <- spec_by_file("Llama-3.2-1B-Instruct-Q8_0.gguf")
            let path = path_join(models_dir(), spec.file)
            if (!model_available(t, path)) return
            var t8 <- load_model_(path, QuantMode.q8)
            if (!moe_gpu_resident_active()) {
                t |> skip("the resident Vulkan driver is not armed (DASLLAMA_GPU=1 on a coopmat2 device)")
                delete t8
                return
            }
            let plen = 512l
            var prompt : array<int64>
            prompt |> resize(int(plen))
            for (i in range(int(plen))) {
                prompt[i] = 1000l + ((int64(i) * 17l + 999l * 131l) % 5000l)
            }
            var want : array<float>
            var got : array<float>
            var served_whole = false
            var served_slice = false
            with_job_que() {
                setup_dasllama_jobque_()
                g_pf_ffn_slice = false
                var s0 <- make_run_state(t8.config, KVDtype.f16, KVDtype.f16)
                forward_prefill(t8, s0, prompt, plen, 0l)
                served_whole = s0.rdec_gen != 0l   // the driver stamps the session it served; a declined call leaves 0
                want := s0.logits
                delete s0
                g_pf_ffn_slice = true
                var s1 <- make_run_state(t8.config, KVDtype.f16, KVDtype.f16)
                forward_prefill(t8, s1, prompt, plen, 0l)
                served_slice = s1.rdec_gen != 0l
                got := s1.logits
                delete s1
            }
            t |> success(served_whole && served_slice, "both arms were served by the resident driver, not the CPU rail")
            var bad = 0
            for (w, g in want, got) {
                if (w != g) {
                    bad++
                }
            }
            t |> success(length(want) == int(t8.config.vocab_size) && bad == 0,
                "the sliced last layer reproduces the whole window ({bad} of {length(want)} logits diverge)")
            delete want
            delete got
            delete prompt
            delete t8
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
