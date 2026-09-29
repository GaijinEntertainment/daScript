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

//! the command that runs `root` (a file of this folder) under `envs` on this platform's shell, stderr folded into stdout
def private child_cmd(root : string; envs : array<tuple<name : string; value : string>>) : string {
    let win = get_platform_name() == "windows"
    let prefix = build_string() $(w) {
        for (e in envs) {
            w |> write(win ? "set \"{e.name}={e.value}\"&& " : "{e.name}={e.value} ")
        }
    }
    return "{prefix}\"{das_exe()}\" -dasroot \"{get_das_root()}\" \"{get_das_root()}/modules/dasLLAMA/tests/{root}\" 2>&1"
}

[test]
def test_bare_queue_panics(t : T?) {
    let cmd = child_cmd("_jobque_tripwire_root.das", [(name = "DASLLAMA_ALLOW_BARE_JOBQUE", value = "0")])
    var out : string
    let rc = run(cmd, out)
    t |> success(rc != 0, "a bare queue's first engine dispatch panics (rc {rc}):\n{out}")
    t |> success(find(out, "setup_dasllama_jobque()") >= 0, "the panic names the call to make:\n{out}")
    t |> success(find(out, "TRIPWIRE_DONE") < 0, "the program did not run on:\n{out}")
}

[test]
def test_allow_bare_jobque_warns_and_runs(t : T?) {
    let cmd = child_cmd("_jobque_tripwire_root.das", [(name = "DASLLAMA_ALLOW_BARE_JOBQUE", value = "1")])
    var out : string
    let rc = run(cmd, out)
    t |> equal(rc, 0, "the escape hatch lets the program run:\n{out}")
    t |> success(find(out, "not configured") >= 0, "the warning still says so:\n{out}")
    t |> success(find(out, "TRIPWIRE_DONE") >= 0, "and the program ran on:\n{out}")
}

[test]
def test_team_job_panic_exits_nonzero(t : T?) {
    // the fallback tune policy: the root carries the engine's tune scope, and the default policy would mint and re-exec the child at startup - under a pipe that chain never closes
    // no worker pin: the child inherits this process's affinity mask, and a hard-pinned worker outside it never runs the chunk
    // the dispatch must reach the queue: an inherited DASLLAMA_SINGLE_THREAD=1 would run the chunk inline, with no job exception to report
    let cmd = child_cmd("_team_panic_root.das", [(name = "DAS_TUNE_POLICY", value = "fallback"), (name = "DAS_JOBQUE_AFFINITY", value = "0"),
        (name = "DAS_JOBQUE_THREADS", value = "2"), (name = "DASLLAMA_SINGLE_THREAD", value = "0")])
    var out : string
    let rc = run(cmd, out)
    t |> success(rc != 0, "a panic on a team chunk ends the process non-zero (rc {rc}):\n{out}")
    t |> success(find(out, "JOB EXCEPTION") >= 0 && find(out, "TEAM_PANIC") >= 0, "the report names the job exception and the panic:\n{out}")
    t |> success(find(out, "TEAM_DONE") < 0, "the program did not run on:\n{out}")
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
