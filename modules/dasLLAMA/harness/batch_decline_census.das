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
require daslib/fio
require daslib/rtti   // this_context().last_exception: a dying per-row fallback is read as text
require math
require strings

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
    var npl = 4l
    var ctx = 4096l
    var kv = KVDtype.f16
    var seen_sep = false
    var pending = ""   // the option waiting for its value
    for (a in args) {
        if (!seen_sep) {
            seen_sep = a == "--"
        } elif (!empty(pending)) {
            if (pending == "--npl") {
                npl = to_int64(a, false)
            } elif (pending == "--ctx") {
                ctx = to_int64(a, false)
            } elif (a == "f16") {
                kv = KVDtype.f16
            } elif (a == "f32") {
                kv = KVDtype.f32
            } elif (a == "q8_0") {
                kv = KVDtype.q8_0
            } elif (a == "tq4") {
                kv = KVDtype.tq4
            } else {
                panic("batch_decline_census: --kv takes f16 | f32 | q8_0 | tq4, not '{a}'")
            }
            pending = ""
        } elif (a == "--npl" || a == "--kv" || a == "--ctx") {
            pending = a
        } else {
            paths |> push(a)
        }
    }
    if (empty(paths) || npl < 2l) {
        print("usage: batch_decline_census.das -- [--npl N] [--ctx ROWS] [--kv f16|f32|q8_0|tq4] <gguf> [<gguf> ...]\n")
        return
    }
    set_metal_mode(MetalMode.when_available)
    with_job_que() {
        setup_dasllama_jobque_()
        for (path in paths) {
            var t <- load_model_cached(path, QuantMode.q8)
            t.config.seq_len = min(t.config.seq_len, ctx)
            let c0 = batch_step_census()
            var r = (tps = 0.0lf, why = "")
            try {
                r = bench_tg_batched_rep(t, npl, 32l, 8l, 1l, false, kv)
            } recover {
                r.why = "PANIC {this_context().last_exception |> rtrim}"   //! a declined step's per-row fallback can die on a blob-only model: the census names it beside the declines
            }
            let c1 = batch_step_census()
            print("CENSUS {base_name(path)} npl={npl} kv={kv} ctx={t.config.seq_len}: tps={r.tps} why='{r.why}' steps device={c1.device - c0.device} cpu_stack={c1.cpu_stack - c0.cpu_stack} per_row={c1.per_row - c0.per_row} declines:{declines_text()}\n")
            delete t
        }
    }
}
