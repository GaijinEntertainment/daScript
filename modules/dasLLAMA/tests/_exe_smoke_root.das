options gen2
options stack = 524288   // every dasllama program root takes this budget

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio

// The standalone-exe smoke root (test_exe_smoke builds it with -jit -exe and runs the artifact):
// one tiny load + a 4-token generate through the facade. A serialized exe restores globals as
// DATA - function values arrive null unless a boot-restore [init] heals them - so this is the
// one context the -jit suites can never cover; the vulkan decode-mirror claim shipped crashing
// exactly here while every script-mode gate stayed green.

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
