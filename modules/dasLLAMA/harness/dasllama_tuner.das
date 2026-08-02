options gen2

require daslib/module_path
require daslib/command_line
require daslib/fio
require daslib/jobque_boost   // get_total_hw_cores: the measurement-regime lane count
require tuner_cli               // --tune-paranoid, shared with both halves
require math

// The dasllama scope tuner — the wrapper the policy rail (auto default / --tune / daspkg
// release) spawns. Runs BOTH halves as child daslang processes, each upserting the same
// env-pointed sidecar (DAS_TUNE_MODE/DAS_TUNE_MANIFEST inherit):
//   1. gen_tune_probe   — the [tune] generator-grid winners (q8q8 tile family)
//   2. tune_kernels     — the [tuned] loop-hint winners + the "runtime" knob snapshot
// A child failure is reported but does not abort the other half — the completeness check
// upstream names whatever ends up missing.

// Two modes only: default and `-- --tune-paranoid` (3x budget, 1% gate) — `daspkg release
// --paranoid` forwards the flag here. There is no fast RACE mode; the debugging concession is
// `daspkg release --quick`, which accepts the existing sidecar and never spawns this at all.
// Both halves take real measurements (the confirm gate is a full prefill), so they must run
// under the box measurement regime — 16 cores with affinity on the big-SMT x64 boxes, not the
// fresh-que default (all cores): a crown confirmed at the wrong width ships the wrong kernel.
// Explicit DAS_JOBQUE_* envs on the tuner itself still win — the children inherit them.
def private regime_env_prefix() : string {
    if (get_platform_name() != "windows") {
        return ""
    }
    var pfx = ""
    if (!has_env_variable("DAS_JOBQUE_THREADS")) {
        let lanes = min(get_total_hw_cores(), 16)
        pfx = "set DAS_JOBQUE_THREADS={lanes}&& "
    }
    if (!has_env_variable("DAS_JOBQUE_AFFINITY")) {
        pfx = "{pfx}set DAS_JOBQUE_AFFINITY=2&& "   // hard mask — mode 1's hint re-rolls SMT placement per que
    }
    return pfx
}

def private run_half(name : string; paranoid : bool) : bool {
    let t0 = ref_time_ticks()
    let pfx = regime_env_prefix()
    let user_args = paranoid ? " -- --tune-paranoid" : ""
    let cmd = "{pfx}\"{get_das_exe()}\" -jit \"{path_join(get_this_module_dir(), name)}\" -dasroot \"{get_das_root()}\"{user_args} 2>&1"
    // cmd.exe strips the first and last quote of a line that starts with a quote — wrap in a
    // sacrificial outer pair on windows UNLESS the env prefix already leads (the daspkg popen trap)
    let full = get_platform_name() == "windows" && empty(pfx) ? "\"{cmd}\"" : cmd
    print("dasllama_tuner: {cmd}\n")
    var rc : int
    unsafe {
        rc = popen_timeout(full, 3600.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                print(fgets(f))
            }
        }
    }
    let elapsed_ms = get_time_usec(t0) / 1000
    print("dasllama_tuner: {name} finished in {elapsed_ms} ms (rc={rc})\n")
    if (rc != 0) {
        print("dasllama_tuner: {name} FAILED (rc={rc})\n")
    }
    return rc == 0
}

[export]
def main : int {
    let t0 = ref_time_ticks()
    let paranoid = tuner_paranoid_requested()
    print("dasllama_tuner: sidecar = {get_env_variable("DAS_TUNE_MANIFEST")}\n")
    let modeDesc = (paranoid
        ? "PARANOID (kernels 20 screening / 240 max, generators best-of-18, 1% gate, validation)"
        : "default (kernels 20 screening / 80 max, generators best-of-6, validation)")
    print("dasllama_tuner: mode = {modeDesc}\n")
    let ok1 = run_half("gen_tune_probe.das", paranoid)
    let ok2 = run_half("tune_kernels.das", paranoid)
    print("dasllama_tuner: total {get_time_usec(t0) / 1000} ms\n")
    return ok1 && ok2 ? 0 : 1
}
