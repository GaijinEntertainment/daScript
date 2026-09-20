options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade - engine + arch registrations + (on Apple) the metal overrides
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the blob-flavor transform
require dasllama/dasllama_batch   // batch_step_census - how the scheduler's steps were served
require dasllama/dasllama_bench   // bench_tg_batched_rep - the batched row
require ?das_metal dasllama/dasllama_metal_common   // metal_batch_decode_stats - the batched driver's own count
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the das_metal halves
require math
require _model_tier

// The batched bench row on the Metal batched driver: `bench_tg_batched_rep` homes its streams on the
// host where no whole-model driver homes them, and every timed step goes through ONE batched
// dispatch - the llama carrier serves at a rate, and a blob-only carrier the batched driver
// declines (gemma-4-E2B: the per-layer embedding has no batch arm) steps its rows one at a time
// without a panic and the row refuses by name. Skips off the JIT, without dasMetal, or without the
// carrier under models_dir().

def private blob_carrier(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}

[test]
def test_metal_batched_row_llama(t : T?) {
    t |> run("the batched row serves host-cached streams through the batched driver (Llama-3.2-1B Q8)") <| @(t : T?) {
        if (!family_on(t, "llama")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) return
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_carrier(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
                t |> equal(gpu_device_sessions(), 0l, "no whole-model driver homes a stream: the row's streams are host-cached")
                let census0 = batch_step_census()
                let steps0 = metal_batch_decode_stats().steps
                let served = bench_tg_batched_rep(tr, 4l, 32l, 8l, 1l)
                t |> equal(served.why, "", "the row is served: {served.why}")
                t |> success(served.tps > 0.0lf, "... at a rate ({served.tps:.1f} tok/s summed over 4 streams)")
                let census1 = batch_step_census()
                t |> success(census1.device - census0.device >= 8l, "the timed steps went through the device batched driver ({census1.device - census0.device} device steps)")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "no step ran on the CPU batched stack")
                t |> success(metal_batch_decode_stats().steps - steps0 >= 8l, "the Metal batched driver counts the steps as its own")
            }
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}

[test]
def test_metal_batched_row_declined_carrier(t : T?) {
    t |> run("a blob-only carrier the batched driver declines steps a row at a time and the row refuses by name (gemma-4-E2B Q8)") <| @(t : T?) {
        if (!family_on(t, "gemma")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
            if (!model_available(t, path)) return
            set_metal_mode(MetalMode.when_available)
            var tr <- blob_carrier(t, path, 256l)
            with_job_que() {
                setup_dasllama_jobque_()
                let census0 = batch_step_census()
                let refused = bench_tg_batched_rep(tr, 2l, 16l, 4l, 1l)   //! the panic this guards against would end the test here
                t |> equal(refused.why, "4 of the 4 timed steps ran their rows one at a time", "the row names the per-row steps")
                t |> equal(refused.tps, 0.0lf, "... and reads 0")
                let census1 = batch_step_census()
                t |> success(census1.per_row - census0.per_row >= 4l, "the census counted the per-row steps ({census1.per_row - census0.per_row})")
                t |> equal(census1.cpu_stack, census0.cpu_stack, "the blob-only model never reached the CPU batched stack")
            }
            delete tr
        } else {
            t |> skip("dasMetal not built")
        }
    }
}
