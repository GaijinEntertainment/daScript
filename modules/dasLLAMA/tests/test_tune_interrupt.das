options gen2
options indenting = 4
options persistent_heap

require dastest/testing_boost public
require daslib/command_line
require daslib/strings_boost
require daslib/fio
require strings

// The supervisor cancel-minting rail, end to end in a child process: while the file named by
// DAS_TUNE_CONTROL exists, interrupt_gate() aborts between kernel families - the @tune abort
// event fires and nothing past the gate runs (minting is end-gated, so an interrupted run
// writes no sidecar). The env is load-once, so a spawned child is the only honest probe.

def private run_probe(control_env : string) : string {
    let win = get_platform_name() == "windows"
    // an empty control_env is spelled as an explicit UNSET - the parent (the watchdog sets
    // this very variable) must never leak its own value into the "unset" cell
    let envs = (empty(control_env)
        ? (win ? "set \"DAS_TUNE_CONTROL=\"&& " : "env -u DAS_TUNE_CONTROL ")
        : (win ? "set \"DAS_TUNE_CONTROL={control_env}\"&& "
               : "DAS_TUNE_CONTROL=\"{control_env}\" "))
    let cmd = "{envs}\"{get_das_exe()}\" \"modules/dasLLAMA/tests/_interrupt_probe.das\" -dasroot \"{get_das_root()}\" 2>&1"
    var out = ""
    unsafe {
        popen(cmd) $(f) {
            if (f != null) {
                out = build_string() $(w) {
                    while (!feof(f)) {
                        w |> write(fgets(f))
                    }
                }
            }
        }
    }
    return out
}

[test]
def test_interrupt_aborts_without_minting(t : T?) {
    t |> run("a present control file aborts at the gate") @(tt : T?) {
        let res = create_temp_file_result("tune_control_", ".stop")
        if (res is error) {
            tt |> failure("could not create a temp control file")
            return
        }
        let control = res as value
        let out = run_probe(control)
        tt |> success(out |> find("PROBE: before gate") >= 0, "the probe ran to the gate: {out}")
        tt |> success(out |> find("@tune abort reason=interrupted at=probe_family") >= 0, "the abort event fired: {out}")
        tt |> success(out |> find("gate passed through") < 0, "nothing past the gate ran - minting is end-gated, so nothing was written: {out}")
        remove(control)
    }
    t |> run("a named-but-absent control file passes through") @(tt : T?) {
        let res = create_temp_file_result("tune_control_", ".stop")
        if (res is error) {
            tt |> failure("could not create a temp control file")
            return
        }
        let control = res as value
        remove(control)
        let out = run_probe(control)
        tt |> success(out |> find("gate passed through") >= 0, "the gate is armed only while the file EXISTS: {out}")
        tt |> success(out |> find("@tune abort") < 0, "no abort without the file: {out}")
    }
    t |> run("no DAS_TUNE_CONTROL passes through") @(tt : T?) {
        let out = run_probe("")
        tt |> success(out |> find("gate passed through") >= 0, "unset env never interrupts: {out}")
        tt |> success(out |> find("@tune abort") < 0, "no abort with the env unset: {out}")
    }
}
