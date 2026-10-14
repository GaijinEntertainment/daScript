options gen2
options indenting = 4

require daslib/clargs
require daslib/fio
require strings
require daslib/strings_boost
require line_length

[CommandLineArgs]
struct Config {
    @clarg_doc = "Base branch; the diff starts at its merge-base with origin/<base-ref>"
    base_ref : string = "master"
    @clarg_doc = "Base ref used as given - a SHA, a local branch, another remote"
    base : string = ""
}

struct GitResult {
    ok : bool
    out : string
}

//! non-ASCII paths come back raw, not in git's quoted-escaped form
def private git(args : array<string>) : GitResult {
    var argv <- ["git", "-c", "core.quotePath=false"]
    argv |> push_from(args)
    var res : GitResult
    let ec = run_and_capture(argv, res.out, 0.0)
    res.ok = ec == 0
    if (!res.ok) {
        to_log(LOG_ERROR, "line-length: git {join(args, " ")} failed ({ec})\n{res.out}\n")
    }
    return res
}

[export]
def main : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        return 1
    }
    let cfg <- r |> move_unwrap
    let root = git(["rev-parse", "--show-toplevel"])
    let merge_base = git(["merge-base", diff_base(cfg.base, cfg.base_ref), "HEAD"])
    return 1 if (!root.ok || !merge_base.ok)
    let diff = git([
        "diff", "-U0", "--no-color", "--no-ext-diff", "--diff-filter=AMR", strip(merge_base.out),
        "--", "*.das"
    ])
    let untracked = git(["ls-files", "--others", "--exclude-standard", "--full-name",
                         "--", "*.das"])
    return 1 if (!diff.ok || !untracked.ok)
    let top = strip(root.out)
    var total = 0
    var changed <- added_lines(diff.out)
    for (path in untracked_paths(untracked.out)) {
        changed[path] <- all_lines(fread(path_join(top, path)))
    }
    for (path, lines in keys(changed), values(changed)) {
        let text = fread(path_join(top, path))
        for (f in find_violations(path, text, lines)) {
            to_log(LOG_ERROR, "{path}:{f.line}: {f.message}\n")
            total++
        }
    }
    if (total != 0) {
        to_log(LOG_ERROR, "line-length: {total} added line(s) over the limit "
            + "(code {CODE_LIMIT}, comments {COMMENT_LIMIT})\n")
        return 1
    }
    return 0
}
