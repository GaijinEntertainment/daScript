options gen2

require strings
require daslib/strings_boost
require daslib/fio

def file_has_line(path : string; pred : block<(ln : string) : bool>) : bool {
    for (ln in split(fread(path), "\n")) {
        return true if (invoke(pred, strip(ln)))
    }
    return false
}

//! the extra paths a changed file answers to under the root REVIEW.md's by-kind routing
def routed_paths(root : string; f : string) : array<string> {
    var out : array<string>
    if (starts_with(f, "include/daScript/") && (ends_with(f, ".h") || ends_with(f, ".inc"))) {
        out |> push("src/{slice(f, length("include/daScript/"))}")
    }
    let path = path_join(root, f)
    if (ends_with(f, ".das") && !starts_with(f, "modules/") && fexist(path)) {
        if (file_has_line(path, $(ln) => starts_with(ln, "[test]") || starts_with(ln, "[test(") || starts_with(ln, "[test,"))) {
            out |> push("tests/{base_name(f)}")
        }
    }
    if (ends_with(f, ".cpp") && fexist(path)) {
        if (file_has_line(path, $(ln) => starts_with(ln, "#include") && find(ln, "doctest") >= 0)) {
            out |> push("tests-cpp/small/{base_name(f)}")
        }
    }
    return <- out
}

//! Every REVIEW.md that governs one of the changed paths, deduped and sorted - the folder walk
//! plus the root REVIEW.md's routing. Paths are repo-relative; `root` prefixes the filesystem
//! checks only.
def affected_review_md(root : string; changed : array<string>) : array<string> {
    var inscope seen : table<string>
    var found, walked : array<string>
    for (f in changed) {
        continue if (empty(f))
        walked |> push(f)
        walked |> push_from(routed_paths(root, f))
    }
    for (f in walked) {
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
def changed_from_git_output(out : string) : array<string> =>
    <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]

//! The REVIEW.das siblings of the given REVIEW.md paths, for the checklists that have one.
def sibling_gates(root : string; mds : array<string>) : array<string> {
    var out : array<string>
    for (md in mds) {
        let d = dir_name(md)   // "" on Windows, "." on POSIX for a bare filename
        let rel = (empty(d) || d == ".") ? "REVIEW.das" : "{d}/REVIEW.das"
        if (fexist(path_join(root, rel))) {
            out |> push(rel)
        }
    }
    return <- out
}

//! The finding line both walkers print for an orphan gate.
def orphan_finding(gate : string) : string =>
    "{gate}: REVIEW.das without a sibling REVIEW.md — the checklist keeps the judgment half and the weakening-is-a-defect rule"

//! Gates missing their sibling REVIEW.md — a REVIEW.das without its checklist is a defect; a deleted gate is none.
def orphan_gates(root : string; gates : array<string>) : array<string> {
    var out : array<string>
    for (g in gates) {
        continue if (!fexist(path_join(root, g)))
        let d = dir_name(g)   // "" on Windows, "." on POSIX for a bare filename
        let rel = (empty(d) || d == ".") ? "REVIEW.md" : "{d}/REVIEW.md"
        if (!fexist(path_join(root, rel))) {
            out |> push(g)
        }
    }
    return <- out
}
