options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_par
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require strings

// The bare-queue tripwire: the engine's first dispatch on a job queue nobody configured panics and
// names the setup call; DASLLAMA_ALLOW_BARE_JOBQUE=1 keeps a warning and runs; a configured queue
// dispatches silently. The bare arms run in child processes - the check fires once per process and
// the env knob loads at context init - so a child's stdout and exit code are the instrument.

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def private run(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout(cmd, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def private fixture_cmd() : string {
    return "\"{das_exe()}\" -dasroot \"{get_das_root()}\" \"{get_das_root()}/modules/dasLLAMA/tests/_jobque_tripwire_root.das\" 2>&1"
}

[test]
def test_bare_queue_panics(t : T?) {
    var out : string
    let rc = run(fixture_cmd(), out)
    t |> success(rc != 0, "a bare queue's first engine dispatch panics (rc {rc}):\n{out}")
    t |> success(find(out, "setup_dasllama_jobque()") >= 0, "the panic names the call to make:\n{out}")
    t |> success(find(out, "TRIPWIRE_DONE") < 0, "the program did not run on:\n{out}")
}

[test]
def test_allow_bare_jobque_warns_and_runs(t : T?) {
    let win = get_platform_name() == "windows"
    let cmd = win ? "set \"DASLLAMA_ALLOW_BARE_JOBQUE=1\"&& {fixture_cmd()}" : "DASLLAMA_ALLOW_BARE_JOBQUE=1 {fixture_cmd()}"
    var out : string
    let rc = run(cmd, out)
    t |> equal(rc, 0, "the escape hatch lets the program run:\n{out}")
    t |> success(find(out, "not configured") >= 0, "the warning still says so:\n{out}")
    t |> success(find(out, "TRIPWIRE_DONE") >= 0, "and the program ran on:\n{out}")
}

[test]
def test_configured_queue_is_silent(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        let before = get_disp_count()
        maybe_parallel_for(0, 4096, 4) $(rb, re) {
            if (rb > re) {
                to_log(LOG_ERROR, "never: {rb} {re}\n")
            }
        }
        t |> success(get_disp_count() > before, "the dispatch went through the counted parallel arm")
    }
}
