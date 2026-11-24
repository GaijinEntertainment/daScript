options gen2
options _dasllama_internal = true

module _pregate_cells public

//! the board pregate's fixture gate on a model its caller loaded (it requires lcpp_bench); none loads one

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // fixture_gate_check — the timed run's fixture gate, driven with a controlled spec
require ../performance/model_specs.das   // spec_by_file — the table rows the gate binds
require dasllama/dasllama_common
require dasllama/dasllama_math          // with_dasllama_jobque_ — the cell's queue (engine-level, not re-exported)
require daslib/jobque_boost
require _ffn_slice_cells public   // last_layer_slice_cell: the bench-free cell, re-exported to this module's requirers

// The board pregate's cells on a loaded model, on the loads `test_parity_pregate.das` makes: the timed run's fixture gate on
// Qwen3-0.6B (`load_model_uncached` in its row's parity quant), which drives the bench's own gate and so requires the bench;
// and, through `_ffn_slice_cells.das` (bench-free), the last layer's FFN slice against the whole window on the resident
// Vulkan driver over Llama-3.2-1B Q8_0 (`load_model_uncached`, the driver armed by the load).

// The timed run's fixture gate (the sanity line every lcpp_bench row set opens with): fed the
// table's own fixture the served model reproduces the opening; an opening that matches nothing
// refuses; a row with no evidence stands aside and leaves the finiteness gate alone.

//! `fixture_gate_check` on Qwen3-0.6B `m`: the table's fixture passes, an opening with every id shifted refuses, a row with no
//! evidence passes
def fixture_gate_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("the timed run's fixture gate passes the table's fixture, refuses a wrong opening, stands aside without evidence") @capture(& m)(t : T ?) {
            with_cpu_prefill(true) {
                var inscope spec <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
                with_dasllama_jobque_() {
                    t |> success(fixture_gate_check(m, spec), "the table's own fixture reproduces on the served model")
                    var inscope wrong <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
                    for (i in range(length(wrong.parity.gen))) {
                        wrong.parity.gen[i] = wrong.parity.gen[i] + 1l
                    }
                    t |> success(!fixture_gate_check(m, wrong), "an opening that matches nothing is refused")
                    var inscope bare <- spec_by_file("Qwen3-0.6B-Q8_0.gguf")
                    bare.parity.gen |> clear()
                    t |> success(fixture_gate_check(m, bare), "a row with no evidence stands aside")
                }
            }
        }
    }
}
