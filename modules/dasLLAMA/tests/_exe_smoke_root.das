options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama
require daslib/jobque_boost
require strings

// The fixture test_exe_smoke builds with `-jit -exe` and runs: one tiny load plus a 4-token
// generate through the facade, on the serialized-globals rail no `-jit` suite can reach.

[export]
def main {
    let cmd <- get_command_line_arguments()
    let path = empty(cmd) ? "" : cmd[length(cmd) - 1]
    if (!ends_with(path, ".gguf")) {
        print("EXE_SMOKE_SKIP no model\n")
        return
    }
    var m <- load_model_(path, QuantMode.q8)   //! the internal loader: the image rail is not the subject, and the facade would mint a .dlim beside the stocked model
    allow_cpu_prefill()
    with_job_que() {
        setup_dasllama_jobque()
        var s <- create_session(m)
        var prompt <- encode(m, "Once upon a time", true)
        let params = SamplingParams()
        let n = generate(m, s, prompt, params, 4l) $(_id : int64; _piece : string) : bool {
            return true
        }
        print("EXE_SMOKE_OK {n}\n")
        delete prompt
        delete s
    }
    delete m
}
