options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/clargs
require scan
require gate

// Lists every REVIEW.md whose folder the changed set touches (skills/internal/make_pr.md step 0a).
// Runs their REVIEW.das gates first: a red gate is fail-fix, no agent review until green.
// Changed set = `git diff --name-only <base>` — committed and working-tree changes alike.

[CommandLineArgs]
struct Config {
    @clarg_short = "b"
    @clarg_doc = "Git base of the changed set (default: origin/master)"
    base : string = "origin/master"

    @clarg_positional
    @clarg_doc = "Explicit changed paths (skips git entirely)"
    files : array<string>

    @clarg_doc = "List affected checklists only — skip executing their REVIEW.das gates"
    list_only : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() : int {
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "review-md")
        return 1
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "review-md")
        return 0
    }
    let root_r = run_argv(["git", "rev-parse", "--show-toplevel"])
    let root = strip(replace(root_r.out, "\r", ""))
    if (root_r.rc != 0 || empty(root)) {
        to_log(LOG_ERROR, "review-md: not inside a git repository\n")
        return 1
    }
    var git_out = ""
    if (empty(cfg.files)) {
        let diff_r = run_argv(["git", "-C", root, "diff", "--name-only", cfg.base])
        if (diff_r.rc != 0) {
            to_log(LOG_ERROR, "review-md: git diff --name-only {cfg.base} failed\n{diff_r.out}")
            return 1
        }
        git_out = diff_r.out
    }
    var inscope changed <- (empty(cfg.files)
        ? changed_from_git_output(git_out)
        : clone_to_move(cfg.files))
    var inscope found <- affected_review_md(root, changed)
    // a changed REVIEW.das without its checklist is a structural defect — caught here
    // even under --list-only, before any gate runs
    let changed_gates <- [for (f in changed); f; where base_name(f) == "REVIEW.das"]
    var inscope orphans <- orphan_gates(root, changed_gates)
    if (!empty(orphans)) {
        for (o in orphans) {
            print("{orphan_finding(o)}\n")
        }
        return 1
    }
    if (!cfg.list_only) {
        var inscope gates <- sibling_gates(root, found)
        let failed = run_gates(root, gates)
        if (failed != 0) {
            to_log(LOG_ERROR, "review-md: {failed} REVIEW.das gate(s) red — fix before agent review\n")
            return 1
        }
    }
    for (p in found) {
        print("{p}\n")
    }
    return 0
}
