options gen2

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify
require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings
require ../common/verify_pool.das

// --ast-verify-batch prints its findings and exits 0, so the verdict is read from the output.

//! a whole-engine root verifies in about a minute on a desktop and
//! ten minutes on a 3-core CI runner with two siblings in flight
//! (565-601 s measured), so the cap is twice that: a hang guard, not
//! a cost bar
let CHANGED_TIMEOUT = 1200.0
let FILE_TIMEOUT = 120.0

[CommandLineArgs]
struct Config {
    @clarg_doc = "Verify this file (repeatable: the files compile in one process, and a report reruns them one by one)"
    file : array<string>

    @clarg_doc = "Verify every .das changed against origin/<base-ref>"
    changed : bool

    @clarg_doc = "Base ref the changed set is taken against; DAS_CI_BASE_REF, else master"
    base_ref : string
}

struct ChangedSet {
    ok : bool
    files : array<string>
}

def private report(findings : string; limit : int) {
    var seen : table<string>
    var shown = 0
    for (line in split(findings, "\n")) {
        continue if (empty(line) || seen |> key_exists(line))
        seen |> insert(line)
        to_log(LOG_ERROR, "  {line}\n")
        shown++
        break if (shown >= limit)
    }
}

def private is_unverifiable(file : string; changed_mode : bool) : bool =>
    ast_verify_skips(file, changed_mode)

//! Gen2 only - gen1 does not stamp ranges; one ModuleGroup for the
//! batch compiles each require once. Answers the files reported on.
def private range_failures(files : array<string>; changed_mode : bool) : table<string> {
    var failed : table<string>
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            for (file in files) {
                continue if (is_unverifiable(file, changed_mode))
                parse_file(file, access, unsafe(addr(mg)), cop) $(parsed; program; _issues) {
                    let gen2 = program._options |> find_arg("gen2") ?as tBool ?? false
                    return if (!parsed || !gen2)
                    if (verify_parsed_module(program, get_this_module(program)) != 0) {
                        to_log(LOG_ERROR, "ast-verify: source ranges reported on {file}\n")
                        failed |> insert(file)
                    }
                }
            }
        }
    }
    return <- failed
}

def private report_result(file : string; r : VerifyResult) : bool {
    if (r.unverifiable) {
        to_log(LOG_INFO, "ast-verify: {file} is in the verifier's own require closure - not verifiable via --ast-verify (error 20510)\n")
        return true
    }
    return true if (empty(r.findings))
    to_log(LOG_ERROR, "ast-verify reported on {file} ({r.verdict})\n")
    report(r.findings, 5)
    return false
}

def private verify_one(daslang, file : string; file_timeout : float = FILE_TIMEOUT) : bool {
    return true if (is_unverifiable(file, false))
    return false if (!empty(range_failures([file], false)))
    var out : string
    let argv <- [daslang, "--ast-verify-batch", "-compile-only", file]
    let ec = run_and_capture(argv, out, file_timeout)
    return report_result(file, verify_verdict(out, ec, file_timeout))
}

def private verify_batch(daslang; files : array<string>;
                         file_timeout : float = FILE_TIMEOUT) : bool {
    if (!empty(range_failures(files, false))) {
        return false
    }
    var argv <- [daslang, "--ast-verify-batch", "-compile-only"]
    argv |> push_from([for (f in files); f; where !is_unverifiable(f, false)])
    if (length(argv) == 3) {
        return true
    }
    var out : string
    let timeout = file_timeout * float(length(argv) - 3)
    let ec = run_and_capture(argv, out, timeout)
    let r = verify_verdict(out, ec, timeout)
    if (!r.unverifiable && empty(r.findings)) {
        return true
    }
    var ok = true
    for (f in files) {
        if (!verify_one(daslang, f, file_timeout)) {
            ok = false
        }
    }
    return ok
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
    if (length(cfg.file) == 1) {
        return verify_one(daslang, cfg.file[0]) ? 0 : 1
    }
    if (length(cfg.file) > 1) {
        return verify_batch(daslang, cfg.file) ? 0 : 1
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
    let files <- [for (f in set.files); f; where !is_unverifiable(f, true)]
    return verify_batch(daslang, files, CHANGED_TIMEOUT) ? 0 : 1
}
