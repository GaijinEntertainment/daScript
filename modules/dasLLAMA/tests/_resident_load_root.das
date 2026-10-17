options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_gpu_tier   // the resident route and the context pins
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, gpu_resident_host_layers: what the load armed
require dasllama/dasllama_math   // with_dasllama_jobque_
require strings
require daslib/strings_convert

// The child the resident gemma cells spawn to read the whole-model driver's load log, which to_log prints and no in-process
// cell reads: one resident load of the gguf it is handed (`-- <path> <ctx>`, `ctx` the context asked whole, 0 none) under the
// knobs the cell passes in its environment, then the driver's state on one line.

[export]
def main {
    let cmd <- get_command_line_arguments()
    let n = length(cmd)
    let path = n >= 2 ? cmd[n - 2] : ""
    if (!ends_with(path, ".gguf")) {
        print("RESIDENT_LOAD_SKIP no model\n")
        return
    }
    let ctx_r = try_to_int64(cmd[n - 1])
    let ctx = ctx_r |> is_err ? 0l : ctx_r |> unwrap
    allow_cpu_prefill()   //! no prefill runs; the root loads and reports
    if (ctx > 0l) {
        set_gpu_ctx_max(ctx)
    }
    set_gpu_ctx_strict(ctx > 0l)
    set_gpu_resident_route(true)
    with_dasllama_jobque_() {
        var m <- load_model_(path, QuantMode.q8)
        print("RESIDENT_LOAD active={moe_gpu_resident_active()} host_layers={gpu_resident_host_layers()}\n")
        moe_gpu_drop_model()
        delete m
    }
    print("ROOT_DONE\n")
}
