options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio

// The fixture test_exe_smoke builds with `-jit -exe` and runs: one tiny load plus a 4-token
// generate through the facade, on the serialized-globals rail no `-jit` suite can reach.

[export]
def main {
    let path = get_env_variable("DASLLAMA_EXE_SMOKE_MODEL")
    if (empty(path)) {
        print("EXE_SMOKE_SKIP no model\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
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
