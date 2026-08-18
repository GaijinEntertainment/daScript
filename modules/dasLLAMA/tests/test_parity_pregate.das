options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // explicit deletes: each arm frees its model before the next loads

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // parity_check — the board pregate's verdict, driven with a controlled spec
require ../performance/model_specs.das   // spec_by_file — the table rows the pregate binds
require dasllama/dasllama_common       // get_kquant_native — the mode the kq arm must restore
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
