options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_gpu_tier   // the resident route and the context pins
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, gpu_resident_host_layers: what the load armed
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_common
require _compares
require _model_tier
require _resident_feed
require strings
require daslib/strings_convert

// The child the resident cells spawn to read the whole-model driver's load log, which to_log prints and no in-process
// cell reads: one resident load of the gguf it is handed (`-- <path> <ctx> [step]`, `ctx` the context asked whole, 0 none)
// under the knobs the cell passes in its environment, then the driver's state on one line; with `step` it also prefills a
// short prompt and decodes one token through the armed driver, for a knob whose path runs past the load.

let private STEP_PROMPT = 8l

[export, cold_path]
def main {
    let cmd <- get_command_line_arguments()
    let n = length(cmd)
    let step = n >= 3 && cmd[n - 1] == "step"
    let at = step ? n - 1 : n
    let path = at >= 2 ? cmd[at - 2] : ""
    if (!ends_with(path, ".gguf")) {
        print("RESIDENT_LOAD_SKIP no model\n")
        return
    }
    let ctx_r = try_to_int64(cmd[at - 1])
    let ctx = ctx_r |> is_err ? 0l : ctx_r |> unwrap
    allow_cpu_prefill()   // the tripwire is disarmed so a declined load still reports its plan; the RESIDENT_LOAD line's `active` is the witness that the driver served a `step` run's prefill
    if (ctx > 0l) {
        set_gpu_ctx_max(ctx)
    }
    set_gpu_ctx_strict(ctx > 0l)
    set_gpu_resident_route(true)
    with_dasllama_jobque_() {
        var m <- load_model_(path, QuantMode.q8)
        print("RESIDENT_LOAD active={moe_gpu_resident_active()} host_layers={gpu_resident_host_layers()}\n")
        if (step && moe_gpu_resident_active()) {
            var prompt <- resident_prompt(STEP_PROMPT)
            var s <- create_session(m, mirror_kv_dtype())
            forward_prefill(m, s, prompt, STEP_PROMPT, 0l)
            s.n_past = STEP_PROMPT
            let tok = argmax_of(s.logits)
            forward(m, s, tok, STEP_PROMPT)
            print("RESIDENT_STEP prefill={STEP_PROMPT} token={tok} next={argmax_of(s.logits)}\n")
            delete s
            delete prompt
        }
        moe_gpu_drop_model()
        delete m
    }
    print("ROOT_DONE\n")
}
