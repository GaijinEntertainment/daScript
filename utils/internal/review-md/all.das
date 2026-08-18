options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/clargs
require scan
require gate

// Runs every REVIEW.das in the tree, committed or newly written — the CI counterpart of the
// per-diff walk in main.das. Exit 0 = all green; 1 = a red gate or a REVIEW.das without its
// REVIEW.md.

[CommandLineArgs]
struct Config {
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() : int {
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "review-md all")
        return 1
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "review-md all")
        return 0
    }
    let root_r = run_argv(["git", "rev-parse", "--show-toplevel"])
    let root = strip(replace(root_r.out, "\r", ""))
    if (root_r.rc != 0 || empty(root)) {
        to_log(LOG_ERROR, "review-md: not inside a git repository\n")
        return 1
    }
    // --others --exclude-standard: a not-yet-committed gate in the working tree runs too
    let ls_r = run_argv(["git", "-C", root, "ls-files", "--cached", "--others", "--exclude-standard", "*REVIEW.das"])
    if (ls_r.rc != 0) {
        to_log(LOG_ERROR, "review-md: git ls-files failed\n{ls_r.out}")
        return 1
    }
    var inscope listed <- changed_from_git_output(ls_r.out)
    var gates <- [for (p in listed); p; where base_name(p) == "REVIEW.das" && !starts_with(p, "history/")]
    gates |> sort
    var rc = 0
    var inscope orphans <- orphan_gates(root, gates)
    for (o in orphans) {
        print("{o}: REVIEW.das without a sibling REVIEW.md — the checklist keeps the judgment half and the weakening-is-a-defect rule\n")
        rc = 1
    }
    let failed = run_gates(root, gates)
    if (failed != 0) {
        rc = 1
    }
    // an empty walk must be visible — a discovery miss silently passing is worse than red
    print("review-md all: {length(gates)} gate(s), {failed} red, {length(orphans)} orphan(s)\n")
    return rc
}
