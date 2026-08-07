options gen2
options indenting = 4
options rtti

require verify_core
require daslib/clargs
require daslib/fio
require strings

//! Compile-check every curated playground sample against the shipped manifest.
//! Run from the repo root:
//!   daslang utils/dasweb-verify/main.das
//! Exit code 0 = every sample compiles; 1 = at least one failure (each named).

[CommandLineArgs]
struct Config {
    @clarg_doc = "Samples root holding data.json (default: web/examples/ui/samples)"
    samples_root : string

    @clarg_doc = "Repo root for generated samples' canonical sources (default: .)"
    repo_root : string

    @clarg_short = "f"
    @clarg_doc = "Only check samples whose name or entry file contains this substring"
    filter : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "dasweb-verify")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "dasweb-verify")
        return 0
    }
    let root = empty(cfg.samples_root) ? "web/examples/ui/samples" : cfg.samples_root
    let repo_root = empty(cfg.repo_root) ? "." : cfg.repo_root
    var error = ""
    let samples <- load_manifest(path_join(root, "data.json"), error)
    if (!empty(error)) {
        to_log(LOG_ERROR, "dasweb-verify: {error}\n")
        return 1
    }
    var checked = 0
    var failed = 0
    for (sample in samples) {
        if (!empty(cfg.filter) && find(sample.name, cfg.filter) < 0 && find(sample.files[0], cfg.filter) < 0) {
            continue
        }
        checked++
        let result = check_sample(root, repo_root, sample)
        if (result.ok) {
            to_log(LOG_INFO, "PASS {result.entry} ({result.name})\n")
        } else {
            failed++
            to_log(LOG_ERROR, "FAIL {result.entry} ({result.name}): {result.message}\n")
        }
    }
    to_log(failed == 0 ? LOG_INFO : LOG_ERROR, "{checked} samples checked, {checked - failed} passed, {failed} failed\n")
    return failed == 0 ? 0 : 1
}
