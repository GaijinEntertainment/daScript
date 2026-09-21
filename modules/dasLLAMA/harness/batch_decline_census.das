options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// The batched-step decline census: for every GGUF named on the command line, one host-cached
// four-stream batched row through the scheduler under the Metal batch driver, then the step
// census (device / cpu_stack / per_row) and the driver's declines-by-reason table - the
// instrument that names which serving shapes still step a row at a time.
//
//   daslang -jit modules/dasLLAMA/harness/batch_decline_census.das -- [--npl N] [--ctx ROWS] [--kv f16|f32|q8_0|tq4] <gguf> [<gguf> ...]

require dasllama/dasllama_transformer   // umbrella: every arch [init] registration
require dasllama/dasllama_image   // load_model_cached: the .dlim rail the rows load on
require dasllama/dasllama_bench   // bench_tg_batched_rep: the batched row and its refusal
require dasllama/dasllama_math    // setup_dasllama_jobque_ + set_metal_mode
require dasllama/dasllama_common  // batch_step_census, allow_cpu_prefill
require ?das_metal dasllama/dasllama_metal_common   // metal_batch_decode_declines + declines_line
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require daslib/rtti   // this_context().last_exception: a dying per-row fallback is read as text
require math
require strings

def declines_text() : string {
    static_if (typeinfo module_exists(dasllama_metal_common)) {
        return "{declines_line(metal_batch_decode_declines())} single-row:{declines_line(metal_decode_declines())}"
    } else {
        return " (no dasMetal)"
    }
}

[CommandLineArgs]
struct Args {
    @clarg_doc = "Streams per row (2 or more)"
    npl : int = 4

    @clarg_doc = "The session context the row loads at"
    ctx : int = 4096

    @clarg_doc = "The KV codec: f16 | f32 | q8_0 | tq4"
    kv : KVDtype = KVDtype.f16

    @clarg_positional
    @clarg_doc = "GGUF paths"
    models : array<string>

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() : int {
    allow_cpu_prefill()
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "batch_decline_census")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.models) || cfg.npl < 2) {
        print_help(get_command_info(type<Args>), "batch_decline_census")
        return cfg.help ? 0 : 1
    }
    let npl = int64(cfg.npl)
    let kv = cfg.kv
    set_metal_mode(MetalMode.when_available)
    with_job_que() {
        setup_dasllama_jobque_()
        for (path in cfg.models) {
            var t <- load_model_cached(path, QuantMode.q8)
            t.config.seq_len = min(t.config.seq_len, int64(cfg.ctx))
            let c0 = batch_step_census()
            var rep = (tps = 0.0lf, why = "")
            try {
                rep = bench_tg_batched_rep(t, npl, 32l, 8l, 1l, false, kv)
            } recover {
                rep.why = "PANIC {this_context().last_exception |> rtrim}"   //! a declined step's per-row fallback can die on a blob-only model: the census names it beside the declines
            }
            let c1 = batch_step_census()
            print("CENSUS {base_name(path)} npl={npl} kv={kv} ctx={t.config.seq_len}: tps={rep.tps} why='{rep.why}' steps device={c1.device - c0.device} cpu_stack={c1.cpu_stack - c0.cpu_stack} per_row={c1.per_row - c0.per_row} declines:{declines_text()}\n")
            delete t
        }
    }
    return 0
}
