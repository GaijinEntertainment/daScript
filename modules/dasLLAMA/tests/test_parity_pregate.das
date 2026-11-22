options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // explicit deletes: each arm frees its model before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // parity_check — the board pregate's verdict, driven with a controlled spec
require ../performance/model_specs.das   // spec_by_file — the table rows the pregate binds
require dasllama/dasllama_common       // get_kquant_native — the mode the kq arm must restore
require dasllama/dasllama_image   // load_model_uncached: the product's load with no image
require daslib/fio
require _model_tier                    // models_dir + model_available (presence/size gate)
require _pregate_cells                 // fixture_gate_cell: the gate cell on a loaded model
require _model_form                   // own_load: each cell declares the load it owns

// The board parity pregate (lcpp_bench --parity), fed a controlled spec: the real fixture
// passes, a deliberately wrong fixture fails, an evidence-less spec is refused, the text-form
// prompt encodes through the tokenizer, the kq-native load arm engages, and a `prefix_bar`
// fixture holds as a half-span prefix. Small carriers
// only; every arm skips honestly when its model is absent. Requires the bench by relative
// path (like test_tok_seed), so it pays the bench's full engine compile.

[test(description = "The board pregate `parity_check` passes Qwen3-0.6B's pinned-ids fixture, fails it with the last id flipped, and refuses a spec with no evidence")]
def test_parity_pregate_ids_form(t : T?) {
    own_load("the cell loads the model inside lcpp_bench's `parity_check` (the board pregate): a bench the model's block file must not compile")
    t |> run("the pinned-ids fixture passes; one flipped id fails; no evidence refuses") @(t : T?) {
        return if (!jit_only(t))
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

[test(description = "The timed run's `fixture_gate_check` passes the table's fixture on a loaded Qwen3-0.6B, refuses an opening with every id shifted, and passes a row with no evidence")]
def test_fixture_gate(t : T?) {
    own_load("the cell's fixture module requires lcpp_bench (`fixture_gate_check`): a bench the model's block file must not compile")
    return if (!jit_only(t))
    var inscope spec <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
    let path = path_join(models_dir(), spec.file)
    if (!model_available(t, path)) return
    var m <- load_model_uncached(path, spec.parity.quant)
    fixture_gate_cell(t, m)
    delete m
}

// The half-span prefix bar of a `prefix_bar` carrier (the gemma-4-26B MoE's form, driven here on a
// small stand-in): a fixture whose tail half is flipped holds, a flip inside the first half reds it,
// and the strict form reds the same flipped tail - the flag is what admits it.
[test(description = "With prefix_bar set, a Llama-3.2-1B fixture whose tail half is flipped still passes `parity_check` while a flip inside the first half fails, and the strict form fails the same flipped tail")]
def test_parity_pregate_prefix_bar(t : T?) {
    own_load("the cell loads the model inside lcpp_bench's `parity_check` (the board pregate): a bench the model's block file must not compile")
    t |> run("a prefix_bar fixture holds on a flipped tail half, reds on a flip inside the first half and under the strict form") @(t : T?) {
        return if (!jit_only(t))
        var inscope spec <- spec_by_file("Llama-3.2-1B-Instruct-Q4_K_M.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        let n = length(spec.parity.gen)
        t |> success(!spec.parity.prefix_bar && n >= 4, "the stand-in's own fixture is the strict form over {n} ids")
        var inscope tail <- spec_by_file(spec.file)
        tail.parity.prefix_bar = true
        for (i in range(n / 2, n)) {
            tail.parity.gen[i] = tail.parity.gen[i] + 1l
        }
        t |> success(parity_check(path, tail), "a flipped tail half holds under the half-span bar")
        var inscope head <- spec_by_file(spec.file)
        head.parity.prefix_bar = true
        head.parity.gen[n / 2 - 1] = head.parity.gen[n / 2 - 1] + 1l
        t |> success(!parity_check(path, head), "a flip inside the first half reds the half-span bar")
        var inscope strict <- spec_by_file(spec.file)
        for (i in range(n / 2, n)) {
            strict.parity.gen[i] = strict.parity.gen[i] + 1l
        }
        t |> success(!parity_check(path, strict), "the strict form reds the same flipped tail")
    }
}

[test(description = "The gemma-4-E2B text-form fixture, encoded through the model's tokenizer, reproduces its pinned continuation under `parity_check`")]
def test_parity_pregate_text_form(t : T?) {
    own_load("the cell loads the model inside lcpp_bench's `parity_check` (the board pregate): a bench the model's block file must not compile")
    t |> run("a text-form fixture encodes through the model's tokenizer") @(t : T?) {
        return if (!jit_only(t))
        var inscope spec <- spec_by_file("gemma-4-E2B-it-Q8_0.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        t |> success(!empty(spec.parity.prompt_text), "the E2B fixture is the text form")
        t |> success(parity_check(path, spec), "the encoded prompt reproduces the pinned continuation")
    }
}

[test(description = "The Qwen3-4B Q4_K_M row's kq-native load reproduces its pinned continuation under `parity_check`, and the pregate restores the kquant-native mode it flipped")]
def test_parity_pregate_kq_native(t : T?) {
    own_load("the cell loads the model inside lcpp_bench's `parity_check` (the board pregate): a bench the model's block file must not compile")
    t |> run("a kq_native fixture loads native K-quant planes and reproduces") @(t : T?) {
        return if (!jit_only(t))
        var inscope spec <- spec_by_file("Qwen3-4B-Instruct-2507-Q4_K_M.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        t |> success(spec.parity.kq_native, "the Q4_K_M row declares the kq-native arm")
        let before = get_kquant_native()
        t |> success(parity_check(path, spec), "the kq-native load reproduces the pinned continuation")
        t |> equal(get_kquant_native(), before, "the pregate restores the kq-native mode it flipped")
    }
}

