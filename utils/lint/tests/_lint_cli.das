options gen2
options indenting = 4

//! how a lint test spawns the lint CLI: the standalone exe the build
//! leaves at `bin/das-lint.exe` when a fresh one exists, else the
//! script through the host binary
module _lint_cli shared public

require strings
require daslib/fio
require daslib/command_line   // get_das_exe

var private g_lint_exe_probed = false
var private g_lint_exe = ""

//! the standalone lint exe, when it is newer than every lint source
//! and starts (an MSVC layout keeps the runtime DLL beside daslang,
//! not beside it); "" when it is absent, stale or cannot run
def lint_exe() : string {
    if (!g_lint_exe_probed) {
        g_lint_exe_probed = true
        g_lint_exe = fresh_lint_exe()
        if (!empty(g_lint_exe)) {
            var probe : string
            // a hang at start (a loader stall, a missing-DLL prompt) is
            // "cannot run" too, not a stuck test run
            if (run_and_capture([g_lint_exe, "--help"], probe, 30.0) != 0) {
                g_lint_exe = ""
            }
        }
    }
    return g_lint_exe
}

def private fresh_lint_exe() : string {
    let exe = path_join(get_das_root(), "bin/das-lint.exe")
    let st = stat(exe)
    return "" if (!st.is_valid)
    // the runner reaches far into daslib (fio, json, clargs, ...), so
    // every daslib module counts, not only the lint ones
    var newest = 0l
    for (folder in ["utils/lint", "utils/common", "daslib"]) {
        dir(path_join(get_das_root(), folder)) $(name) {
            return if (!ends_with(name, ".das"))
            let src = stat(path_join(get_das_root(), "{folder}/{name}"))
            let srcTime = int64(src.mtime)
            if (srcTime > newest) {
                newest = srcTime
            }
        }
    }
    return int64(st.mtime) >= newest ? exe : ""
}

//! the lint CLI as argv; `host` are daslang's flags before the script
//! in the script form - the exe form has no host and takes none, so a
//! caller whose host flags change what the lint sees passes
//! `exe_ok = false` - and `after` follows either form
def lint_cli_argv(host, after : array<string>; exe_ok : bool = true) : array<string> {
    let exe = exe_ok ? lint_exe() : ""
    if (!empty(exe)) {
        var argv <- [exe]
        argv |> push_from(after)
        return <- argv
    }
    var argv <- [get_das_exe(), "-dasroot", get_das_root()]
    argv |> push_from(host)
    argv |> push("{get_das_root()}/utils/lint/main.das")
    argv |> push("--")
    argv |> push_from(after)
    return <- argv
}
