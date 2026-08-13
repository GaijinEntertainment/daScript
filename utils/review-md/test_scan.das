options gen2

require dastest/testing_boost public
require daslib/fio
require scan

let FIXTURE = "review_md_test_fixture"

def write_marker(path : string) {
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, "rules")
        }
    }
}

[test]
def test_walk_collects_governing_checklists(t : T?) {
    rmdir_rec_result(FIXTURE)
    t |> success(mkdir_rec("{FIXTURE}/a/b"))
    t |> success(mkdir_rec("{FIXTURE}/c"))
    t |> success(mkdir_rec("{FIXTURE}/history"))
    write_marker("{FIXTURE}/REVIEW.md")
    write_marker("{FIXTURE}/a/REVIEW.md")
    write_marker("{FIXTURE}/history/REVIEW.md")

    // a/b/code.das is governed by a/ and the root; c/ has no checklist of its
    // own, so only the root governs it; top.md exercises the root-level walk;
    // the history/ path binds nothing — archives are frozen.
    var inscope hits <- affected_review_md(FIXTURE, ["a/b/code.das", "c/other.das", "top.md", "history/old.das"])
    t |> equal(2, length(hits))  // nolint:STYLE040 — same pins repeat below on a different input, deliberately
    t |> equal("REVIEW.md", hits[0])
    t |> equal("a/REVIEW.md", hits[1])

    // root-only and nested-only inputs pin the root normalization and the climb
    // to root independently of each other (each is invisible in the combined run).
    var inscope root_only <- affected_review_md(FIXTURE, ["top.md"])
    t |> equal(1, length(root_only))
    t |> equal("REVIEW.md", root_only[0])

    var inscope nested_only <- affected_review_md(FIXTURE, ["a/b/code.das"])
    t |> equal(2, length(nested_only))
    t |> equal("REVIEW.md", nested_only[0])
    t |> equal("a/REVIEW.md", nested_only[1])

    var inscope blank_only <- affected_review_md(FIXTURE, [""])
    t |> equal(0, length(blank_only))

    var inscope no_files : array<string>
    var inscope none <- affected_review_md(FIXTURE, no_files)
    t |> equal(0, length(none))

    rmdir_rec_result(FIXTURE)
}

[test]
def test_git_output_parsing(t : T?) {
    var inscope parsed <- changed_from_git_output("a.txt\r\n\nb/c.txt\n")
    t |> equal(2, length(parsed))
    t |> equal("a.txt", parsed[0])
    t |> equal("b/c.txt", parsed[1])

    var inscope none <- changed_from_git_output("")
    t |> equal(0, length(none))
}
