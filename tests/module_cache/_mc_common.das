options gen2
options indenting = 4

module _mc_common shared public

require dastest/testing_boost public

require strings
require daslib/fio

//! the daslang binary to spawn - dastest runs as `daslang(.exe) dastest/dastest.das ...`,
//! so argv[0] is the interpreter, not the test script
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! the child's environment, as a command prefix: the scan trace on, so the trace names each
//! descriptor's verdict and each shared-module load; the prefix also keeps the command from
//! opening with a quote, which cmd.exe would strip
def trace_prefix() : string {
    return get_platform_name() == "windows" ? "set \"DAS_TRACE_MODULE_LOAD=1\"&& " : "DAS_TRACE_MODULE_LOAD=1 "
}

//! stderr joins stdout: the scan trace goes through the logger
def run_child(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout("{cmd} 2>&1", 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def report_child(t : T?; phase : string; rc : int; out : string; marker : string) : bool {
    let ok = rc == 0 && find(out, marker) >= 0
    if (!ok) {
        t |> failure("{phase} child: rc={rc}, marker '{marker}' {find(out, marker) >= 0 ? "found" : "MISSING"}")
        t |> failure("{phase} child output follows:\n{out}")
    }
    return ok
}

//! runs `cmd` and reports it as report_child does; `out` keeps the child's output for the
//! caller's own reads. The run is a statement of its own: `run_child(cmd, out)` nested beside
//! `out` in one argument list is read before or after the run by the C++ compiler's choice
//! under AOT, and MSVC reads `out` first.
def run_child_reported(t : T?; phase : string; cmd : string; var out : string&; marker : string) : bool {
    let rc = run_child(cmd, out)
    return report_child(t, phase, rc, out, marker)
}
