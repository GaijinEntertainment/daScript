options gen2
options indenting = 4

// The ONLY sanctioned way to run the dasLLAMA Metal suites (see modules/dasLLAMA/tests/CLAUDE.md).
// Enforces the iteration discipline that keeps suite walls honest: --arm (filtered iteration,
// minutes) scoped to the arms a change can actually affect. --full is REFUSED while the Metal
// build-out is in progress — a whole-zoo pass buys soak time, not coverage; narrow the scope.
// Every run's COMPLETE output lands in a log file whose path is printed — never pipe suite
// output through a grep at capture time.

require daslib/clargs
require daslib/fio
require strings
require math

[CommandLineArgs]
struct RunArgs {
    @clarg_short = "a"
    @clarg_doc = "Arm filter — comma-separated substrings matched against arm names (e.g. arm12,batchB8); sets DASLLAMA_TEST_ARMS"
    arm : string

    @clarg_short = "f"
    @clarg_doc = "REFUSED while the Metal build-out is in progress — narrow the scope with --arm instead"
    full : bool

    @clarg_short = "s"
    @clarg_doc = "Suite: decode | prefill | matrix | kernels | image | all (default all)"
    suite : string = "all"

    @clarg_doc = "Family filter — comma-separated family tokens (llama, qwen2, qwen3, phi3, gemma, ultravox, whisper, voxtral); only blocks of these model families run (model-free blocks always run); sets DASLLAMA_TEST_FAMILY"
    family : string

    @clarg_short = "n"
    @clarg_doc = "Repeat count — explicit only (default 1, never best-of-N)"
    nreps : int = 1

    @clarg_doc = "daslang binary (default: the binary running this script — a hardcoded ./bin/daslang picked up a STALE top-level exe on the MSVC layout)"
    das : string = ""

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private suite_files(name : string) : array<string> {
    if (name == "decode") {
        return <- [ "modules/dasLLAMA/tests/test_metal_decode_parity.das" ]
    } elif (name == "prefill") {
        return <- [ "modules/dasLLAMA/tests/test_metal_prefill_parity.das" ]
    } elif (name == "matrix") {
        return <- [ "modules/dasLLAMA/tests/test_metal_support_matrix.das" ]
    } elif (name == "kernels") {
        return <- [ "modules/dasLLAMA/tests/test_metal_prefill_kernels.das" ]
    } elif (name == "image") {
        return <- [ "modules/dasLLAMA/tests/test_model_image.das" ]
    } elif (name == "image-vulkan") {
        return <- [ "modules/dasLLAMA/tests/test_model_image_vulkan.das" ]
    } elif (name == "all") {
        return <- [ "modules/dasLLAMA/tests/test_metal_decode_parity.das",
                    "modules/dasLLAMA/tests/test_metal_prefill_parity.das",
                    "modules/dasLLAMA/tests/test_metal_support_matrix.das",
                    "modules/dasLLAMA/tests/test_metal_prefill_kernels.das",
                    "modules/dasLLAMA/tests/test_model_image.das" ]
    }
    var none : array<string>
    return <- none
}

// the trailing "N tests, ..." summary line of a completed dastest log ("" when the run died
// before printing one — crash, kill, or compile error)
def private log_summary(log_path : string) : string {
    var summary = ""
    fopen(log_path, "rb") $(f) {
        if (f != null) {
            let text = fread(f)
            peek_data(text) $(bytes) {
                var at = 0
                var line_start = 0
                while (at < length(bytes)) {
                    if (bytes[at] == uint8('\n')) {
                        let line = slice(text, line_start, at)
                        if (find(line, " tests, ") >= 0) {
                            summary = clone_string(line)
                        }
                        line_start = at + 1
                    }
                    at++
                }
            }
        }
    }
    return summary
}

[export]
def main() : int {
    var r <- parse_args(type<RunArgs>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<RunArgs>), "modules/dasLLAMA/tests/run.das")
        return 2
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<RunArgs>), "modules/dasLLAMA/tests/run.das")
        return 0
    }
    if (cfg.full == !empty(cfg.arm)) {   // both set, or neither
        to_log(LOG_ERROR, "pass --arm <filter> scoped to what the change can affect (--full is disabled while the Metal build-out is in progress; large tier = DASLLAMA_PARITY_FULL=1 with --arm)\n")
        return 2
    }
    if (cfg.full) {
        to_log(LOG_ERROR, "please narrow the scope to the correct set of gates, metal is not fully implemented yet\n")
        return 2
    }
    var files <- suite_files(cfg.suite)
    if (empty(files)) {
        to_log(LOG_ERROR, "unknown --suite '{cfg.suite}' (decode | prefill | matrix | kernels | image | image-vulkan | all)\n")
        return 2
    }
    var tmp = get_env_variable("TMPDIR")
    if (empty(tmp)) {
        tmp = get_env_variable("TEMP")   // the Windows spelling; TMPDIR is the POSIX one
    }
    if (empty(tmp)) {
        tmp = "/tmp"
    }
    // CPU_PREFILL: the suites' CPU truth halves and parity controls prefill on the CPU by intent.
    // HAZARD_STRICT: the scheduling-unification ratchet's runtime half — an undeclared dispatch
    // on an armed hazard oracle PANICS here, so declaration gaps can never ship quietly.
    // cmd.exe has no VAR=val prefix — `set "VAR=val"&&` is the safe form (llvm_tune's tuner_cmd)
    let win = get_platform_name() == "windows"
    var envs = ""
    if (win) {
        envs = ((cfg.full ? "set \"DASLLAMA_PARITY_FULL=1\"&& " : "set \"DASLLAMA_TEST_ARMS={cfg.arm}\"&& ")
            + "set \"DASLLAMA_CPU_PREFILL=1\"&& set \"DASLLAMA_METAL_HAZARD_STRICT=1\"&& ")
        if (!empty(cfg.family)) {   // composes with either mode: a family-scoped full gate is the profiling-cadence default
            envs = "{envs}set \"DASLLAMA_TEST_FAMILY={cfg.family}\"&& "
        }
    } else {
        envs = (cfg.full ? "DASLLAMA_PARITY_FULL=1" : "DASLLAMA_TEST_ARMS='{cfg.arm}'") + " DASLLAMA_CPU_PREFILL=1 DASLLAMA_METAL_HAZARD_STRICT=1 "
        if (!empty(cfg.family)) {
            envs = "DASLLAMA_TEST_FAMILY='{cfg.family}' {envs}"
        }
    }
    if (cfg.suite == "image-vulkan") {
        // the offline-bake suite: arm the tier so the probed config carries a vulkan section —
        // the bake itself is DRY (no device calls), so this is safe on GPU-less boxes too
        envs = (win ? "{envs}set \"DASLLAMA_GPU=1\"&& set \"DASLLAMA_GPU_VRAM_MB=2000\"&& "
            : "DASLLAMA_GPU=1 DASLLAMA_GPU_VRAM_MB=2000 {envs}")
    }
    // the full gate holds the 8B/70B tails — the stock 1200s dastest wall is an arm-mode budget
    let tmo = cfg.full ? 3600 : 1200
    let reps = max(cfg.nreps, 1)
    let das_exe = empty(cfg.das) ? get_command_line_arguments()[0] : cfg.das
    var failures = 0
    for (rep in range(reps)) {
        for (file in files) {
            let tag = base_name(file)
            let log_path = path_join(tmp, "dasllama_run_{tag}_{rep}_{ref_time_ticks()}.log")
            let cmd = "{envs}\"{das_exe}\" -jit dastest/dastest.das -- --test \"{file}\" --timeout {tmo} > \"{log_path}\" 2>&1"
            let rep_tag = reps > 1 ? " rep {rep + 1}/{reps}" : ""
            to_log(LOG_INFO, "START {tag}{rep_tag}: {cmd}\n")
            let t0 = ref_time_ticks()
            var rc = 0
            unsafe {
                rc = popen(cmd) $(f) {
                    let _drain = fread_to_eof(f)   // output is redirected; drain keeps the pipe contract
                }
            }
            let secs = double(get_time_usec(t0)) / 1000000.0lf
            let summary = log_summary(log_path)
            let verdict = !empty(summary) ? summary : "no dastest summary — the run died early (read the log)"
            to_log(rc == 0 ? LOG_INFO : LOG_ERROR, "DONE {tag}: exit={rc} {verdict} elapsed={secs}s log={log_path}\n")
            if (rc != 0) {
                failures++
            }
        }
    }
    return failures
}
