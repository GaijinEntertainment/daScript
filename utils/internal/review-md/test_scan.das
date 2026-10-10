options gen2

require dastest/testing_boost public
require daslib/fio
require scan

def write_marker(path : string) {
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, "rules")
        }
    }
}

[test]
def test_walk_collects_governing_checklists(t : T?) {
    let fixture_r = create_temp_directory_result("review_md_scan")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    t |> success(mkdir_rec("{fixture}/a/b"))
    t |> success(mkdir_rec("{fixture}/c"))
    write_marker("{fixture}/REVIEW.md")
    write_marker("{fixture}/a/REVIEW.md")

    // a/b/code.das is governed by a/ and the root; c/ has no checklist of its
    // own, so only the root governs it; top.md exercises the root-level walk.
    var inscope hits <- affected_review_md(fixture, ["a/b/code.das", "c/other.das", "top.md"])
    t |> equal(2, length(hits))  // nolint:STYLE040 — same pins repeat below on a different input, deliberately
    t |> equal("REVIEW.md", hits[0])
    t |> equal("a/REVIEW.md", hits[1])

    // root-only and nested-only inputs pin the root normalization and the climb
    // to root independently of each other (each is invisible in the combined run).
    var inscope root_only <- affected_review_md(fixture, ["top.md"])
    t |> equal(1, length(root_only))
    t |> equal("REVIEW.md", root_only[0])

    var inscope nested_only <- affected_review_md(fixture, ["a/b/code.das"])
    t |> equal(2, length(nested_only))
    t |> equal("REVIEW.md", nested_only[0])
    t |> equal("a/REVIEW.md", nested_only[1])

    var inscope blank_only <- affected_review_md(fixture, [""])
    t |> equal(0, length(blank_only))

    var inscope no_files : array<string>
    var inscope none <- affected_review_md(fixture, no_files)
    t |> equal(0, length(none))

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

[test]
def test_walk_follows_the_root_routing(t : T?) {
    let fixture_r = create_temp_directory_result("review_md_routes")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    for (d in ["include/daScript/misc", "src/misc", "tests", "tests-cpp/small", "utils/x", "modules/m", "c"]) {
        t |> success(mkdir_rec("{fixture}/{d}"))
    }
    for (md in ["REVIEW.md", "src/misc/REVIEW.md", "tests/REVIEW.md", "tests-cpp/small/REVIEW.md"]) {
        write_marker("{fixture}/{md}")
    }
    for (file in [
            ("utils/x/test_x.das", "options gen2\n\n[test]\ndef test_x(t : T?) \{\n\}\n"),
            ("utils/x/test_y.das", "options gen2\n\n[test, no_jit]\ndef test_y(t : T?) \{\n\}\n"),
            ("utils/x/test_z.das", "options gen2\n\n[test(slow)]\ndef test_z(t : T?) \{\n\}\n"),
            ("utils/x/tool.das", "options gen2\n\n// [test] in a comment does not count\n"),
            ("utils/x/notes.txt", "[test]\n"),
            ("modules/m/test_m.das", "options gen2\n\n[test]\ndef test_m(t : T?) \{\n\}\n"),
            ("c/check.cpp", "#include \"doctest/doctest.h\"\n"),
            ("c/plain.cpp", "#include \"daScript/daScript.h\"\n"),
            ("c/mention.cpp", "// built without doctest\nint x;\n")]) {
        t |> success(fwrite("{fixture}/{file._0}", file._1), "wrote {file._0}")
    }
    t |> run("a header under include/daScript/<dir>/ answers to src/<dir>/REVIEW.md") @(t : T?) {
        t |> equal(affected_review_md(fixture, ["include/daScript/misc/sysos.h"]), ["REVIEW.md", "src/misc/REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["include/daScript/misc/ops.inc"]), ["REVIEW.md", "src/misc/REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["include/daScript/misc/NOTES.md"]), ["REVIEW.md"])
    }
    t |> run("a [test] file outside modules/ answers to tests/REVIEW.md, a file without one does not") @(t : T?) {
        t |> equal(affected_review_md(fixture, ["utils/x/test_x.das"]), ["REVIEW.md", "tests/REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["utils/x/test_y.das"]), ["REVIEW.md", "tests/REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["utils/x/test_z.das"]), ["REVIEW.md", "tests/REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["utils/x/tool.das"]), ["REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["utils/x/notes.txt"]), ["REVIEW.md"])
    }
    t |> run("a [test] file under modules/ keeps its own folder's checklists") @(t : T?) {
        t |> equal(affected_review_md(fixture, ["modules/m/test_m.das"]), ["REVIEW.md"])
    }
    t |> run("a doctest .cpp answers to tests-cpp/small/REVIEW.md, another .cpp does not") @(t : T?) {
        t |> equal(affected_review_md(fixture, ["c/check.cpp"]), ["REVIEW.md", "tests-cpp/small/REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["c/plain.cpp"]), ["REVIEW.md"])
        t |> equal(affected_review_md(fixture, ["c/mention.cpp"]), ["REVIEW.md"])
    }
    t |> run("a deleted file routes by its path alone") @(t : T?) {
        t |> equal(affected_review_md(fixture, ["c/gone.cpp", "include/daScript/misc/gone.h"]), ["REVIEW.md", "src/misc/REVIEW.md"])
    }
    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

[test]
def test_sibling_and_orphan_gates(t : T?) {
    let fixture_r = create_temp_directory_result("review_md_gates")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    t |> success(mkdir_rec("{fixture}/a"))
    t |> success(mkdir_rec("{fixture}/b"))
    t |> success(mkdir_rec("{fixture}/c"))
    write_marker("{fixture}/REVIEW.md")
    write_marker("{fixture}/a/REVIEW.md")
    write_marker("{fixture}/b/REVIEW.md")
    write_marker("{fixture}/a/REVIEW.das")
    write_marker("{fixture}/c/REVIEW.das")

    var inscope gates <- sibling_gates(fixture, ["REVIEW.md", "a/REVIEW.md", "b/REVIEW.md"])
    t |> equal(1, length(gates), "only a/ pairs a checklist with a gate")
    t |> equal("a/REVIEW.das", gates[0])

    var inscope orphans <- orphan_gates(fixture, ["a/REVIEW.das", "c/REVIEW.das"])
    t |> equal(1, length(orphans), "c/ has a gate and no checklist")
    t |> equal("c/REVIEW.das", orphans[0])

    var inscope deleted_orphans <- orphan_gates(fixture, ["d/REVIEW.das"])
    t |> equal(0, length(deleted_orphans), "a gate the diff deletes with its checklist is no orphan")

    // root arms: a gate beside the root checklist, and a root gate with no checklist
    write_marker("{fixture}/REVIEW.das")
    var inscope root_gate <- sibling_gates(fixture, ["REVIEW.md"])
    t |> equal(1, length(root_gate))
    t |> equal("REVIEW.das", root_gate[0])
    var inscope root_orphan <- orphan_gates("{fixture}/c", ["REVIEW.das"])
    t |> equal(1, length(root_orphan), "c/ as root has a gate and no checklist")

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
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
