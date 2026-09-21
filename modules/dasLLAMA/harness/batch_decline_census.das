options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// The batched-step decline census: for every GGUF named on the command line, one host-cached
// four-stream batched row through the scheduler under the Metal batch driver, then the step
// census (device / cpu_stack / per_row) and the driver's declines-by-reason table - the
// instrument that names which serving shapes still step a row at a time.
//
//   daslang -jit modules/dasLLAMA/harness/batch_decline_census.das -- <gguf> [<gguf> ...]

require dasllama/dasllama_transformer   // umbrella: every arch [init] registration
require dasllama/dasllama_image   // load_model_cached: the .dlim rail the rows load on
require dasllama/dasllama_bench   // bench_tg_batched_rep: the batched row and its refusal
require dasllama/dasllama_math    // setup_dasllama_jobque_ + set_metal_mode
require dasllama/dasllama_common  // batch_step_census, allow_cpu_prefill
require ?das_metal dasllama/dasllama_metal_common   // metal_batch_decode_declines + declines_line
require daslib/jobque_boost
require daslib/fio
require math

def declines_text() : string {
    static_if (typeinfo module_exists(dasllama_metal_common)) {
        return declines_line(metal_batch_decode_declines())
    } else {
        return " (no dasMetal)"
    }
}

[export]
def main() {
    allow_cpu_prefill()
    let args <- get_command_line_arguments()
    var paths : array<string>
    var seen_sep = false
    for (a in args) {
        if (seen_sep) {
            paths |> push(a)
        } elif (a == "--") {
            seen_sep = true
        }
    }
    if (empty(paths)) {
        print("usage: batch_decline_census.das -- <gguf> [<gguf> ...]\n")
        return
    }
    set_metal_mode(MetalMode.when_available)
    with_job_que() {
        setup_dasllama_jobque_()
        for (path in paths) {
            var t <- load_model_cached(path, QuantMode.q8)
            t.config.seq_len = min(t.config.seq_len, 4096l)
            let c0 = batch_step_census()
            let r = bench_tg_batched_rep(t, 4l, 32l, 8l, 1l)
            let c1 = batch_step_census()
            print("CENSUS {base_name(path)}: tps={r.tps} why='{r.why}' steps device={c1.device - c0.device} cpu_stack={c1.cpu_stack - c0.cpu_stack} per_row={c1.per_row - c0.per_row} declines:{declines_text()}\n")
            delete t
        }
    }
}
