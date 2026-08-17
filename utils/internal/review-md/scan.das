options gen2

require strings
require daslib/strings_boost
require daslib/fio

//! Every REVIEW.md that governs one of the changed paths, deduped and sorted. Paths are
//! repo-relative; `root` prefixes the filesystem checks only.
def affected_review_md(root : string; changed : array<string>) : array<string> {
    var inscope seen : table<string>
    var found : array<string>
    for (f in changed) {
        continue if (empty(f) || f |> starts_with("history/"))   // archives are frozen
        var d = dir_name(f)
        if (empty(d)) {
            d = "."
        }
        while (!empty(d)) {
            break if (key_exists(seen, d))   // a seen dir's ancestors are seen too
            seen |> insert(d)
            let rel = d == "." ? "REVIEW.md" : "{d}/REVIEW.md"
            if (fexist(path_join(root, rel))) {
                found |> push(rel)
            }
            let up = dir_name(d)
            d = empty(up) ? "." : up
        }
    }
    found |> sort
    return <- found
}

//! Repo-relative paths from `git diff --name-only` output: one per line, CR and blank
//! lines dropped.
def changed_from_git_output(out : string) : array<string> {
    return <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]
}
