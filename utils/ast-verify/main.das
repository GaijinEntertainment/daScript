options gen2

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings

// --ast-verify-batch prints its findings and exits 0, so the verdict is read from the output.

let CHANGED_TIMEOUT = 300.0
let FILE_TIMEOUT = 120.0

[CommandLineArgs]
struct Config {
    @clarg_doc = "Verify this one file"
    file : string

    @clarg_doc = "Verify every .das changed against origin/<base-ref>"
    changed : bool

    @clarg_doc = "Base ref the changed set is taken against; DAS_CI_BASE_REF, else master"
    base_ref : string
}

struct ChangedSet {
    ok : bool
    files : array<string>
}

def private report(out, needle : string; limit : int) {
    var seen : table<string>
    var shown = 0
    for (line in split(out, "\n")) {
        continue if (find(line, needle) < 0 || seen |> key_exists(line))
        seen |> insert(line)
        to_log(LOG_ERROR, "  {line}\n")
        shown++
        break if (shown >= limit)
    }
}

def private verify_one(daslang, file : string; changed_mode : bool) : bool {
    let name = base_name(file)
    if (name |> starts_with("cant_") || name |> starts_with("failed_") || name |> starts_with("invalid_")
            || (changed_mode && find(file, "ast-fuzz/selftest/") >= 0)) {
        return true
    }
    let timeout = changed_mode ? CHANGED_TIMEOUT : FILE_TIMEOUT
    let argv <- (changed_mode
        ? [daslang, "-dry-run", "--ast-verify-batch", file]
        : [daslang, "--ast-verify-batch", "-compile-only", file])
    var out : string
    let ec = run_and_capture(argv, out, timeout)
    if (changed_mode && find(out, "error[20510]") >= 0) {
        to_log(LOG_INFO, "ast-verify: {file} is in the verifier's own require closure - not verifiable via --ast-verify (error 20510)\n")
        return true
    }
    if (find(out, "AST verify") >= 0) {
        to_log(LOG_ERROR, "ast-verify reported on {file}\n")
        report(out, "AST verify", 5)
        return false
    }
    if (ec == popen_timed_out) {
        to_log(LOG_ERROR, "ast-verify timed out ({int(timeout)}s) on {file}\n")
        return false
    }
    if (find(out, "CRASH:") >= 0) {
        to_log(LOG_ERROR, "ast-verify crashed (exit {ec}) on {file}\n")
        report(out, "CRASH:", 4)
        return false
    }
    if (ec >= 128) {
        to_log(LOG_ERROR, "ast-verify killed (exit {ec}) on {file} - no crash report: the runner (memory), an external kill, or a stack overflow off the main thread\n")
        return false
    }
    return true
}

def private changed_files(base_ref : string) : ChangedSet {
    var out : string
    let ec = run_and_capture([
        "git", "diff", "--name-only", "--diff-filter=AM", "origin/{base_ref}...HEAD", "--", "*.das"
    ], out, 0.0)
    var res : ChangedSet
    if (ec != 0) {
        to_log(LOG_ERROR, "git diff against origin/{base_ref} failed ({ec})\n")
        return <- res
    }
    res.ok = true
    res.files <- [for (line in split(out, "\n")); line; where !empty(line)]
    return <- res
}

[export]
def main() : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        return 1
    }
    let cfg <- r |> move_unwrap
    let daslang = get_host_binary()
    if (!empty(cfg.file)) {
        return verify_one(daslang, cfg.file, false) ? 0 : 1
    }
    if (!cfg.changed) {
        to_log(LOG_ERROR, "error: pass --file <path> or --changed\n")
        return 1
    }
    var base = cfg.base_ref
    if (empty(base)) {
        base = has_env_variable("DAS_CI_BASE_REF") ? get_env_variable("DAS_CI_BASE_REF") : ""
    }
    if (empty(base)) {
        base = "master"
    }
    let set <- changed_files(base)
    if (!set.ok) {
        return 1
    }
    var failed = 0
    for (f in set.files) {
        if (!verify_one(daslang, f, true)) {
            failed++
        }
    }
    if (failed != 0) {
        to_log(LOG_ERROR, "ast-verify: {failed} file(s) reported\n")
        return 1
    }
    return 0
}
