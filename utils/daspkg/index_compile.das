options gen2

require strings
require daslib/clargs
require daslib/fio
require daslib/strings_boost
require dastest/review_gate

//! Compiles every module an installed package registers - what a
//! consumer of the package can `require`. Exit 0 when each compiles
//! (a package that registers none passes), 1 otherwise.

[CommandLineArgs]
struct Config {
    @clarg_doc = "Installed package: the folder under modules/ that holds its .das_module"
    package : string = ""
    @clarg_doc = "Project root the package is installed under, when it is not the daslang root"
    project_root : string = ""
}

let private COMPILE_TIMEOUT_S = 600.0

def private registered_files(package : string) : array<string> {
    let marker = "/modules/{package}/"
    var files : array<string>
    for_each_registered_native_path() $(_mod_name, _src_path, dst_path) {
        if (find(dst_path, marker) >= 0) {
            files |> push(clone_string(dst_path))
        }
    }
    files |> sort
    return <- files
}

[export]
def main : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (empty(cfg.package)) {
        to_log(LOG_ERROR, "error: --package is required\n")
        return 1
    }
    var inscope files <- registered_files(cfg.package)
    let self_binary = get_command_line_arguments()[0]
    var failed = 0
    for (path in files) {
        var inscope cmd <- [self_binary]
        if (!empty(cfg.project_root)) {
            cmd |> push("-project_root")
            cmd |> push(cfg.project_root)
        }
        cmd |> push("-compile-only")
        cmd |> push(path)
        var output = ""
        let rc = run_and_capture(cmd, output, COMPILE_TIMEOUT_S)
        if (rc != 0) {
            failed++
            let why = first_error_line(output)
            to_log(LOG_ERROR, "{cfg.package}: {path} does not compile (exit {rc}): {why}\n")
        }
    }
    to_log(failed == 0 ? LOG_INFO : LOG_ERROR,
        "{cfg.package}: {length(files)} registered module(s), {failed} failed to compile\n")
    return failed == 0 ? 0 : 1
}
