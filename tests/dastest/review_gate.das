options gen2

require dastest/testing_boost public
require dastest/review_gate
require daslib/fio
require daslib/strings_boost
require strings

def private gate_findings_for(root : string; var rc : int&;
                              verb : string = " writes `") : array<string> {
    let exe = get_command_line_arguments()[0]
    var output : string
    rc = run_and_capture(
                         [exe, "-dasroot", get_das_root(), "-no-module-cache",
                          "{get_das_root()}/tests/REVIEW.das", "--", "--root", root], output)
    var findings : array<string>
    for (line in split(output, "\n")) {
        let at = find(line, ": ")
        if (find(line, verb) >= 0 && at >= 0) {
            findings |> push(strip(slice(line, at + 2)))
        }
    }
    return <- findings
}

//! the gate's `path: finding` lines over `root`, as `<file name>:
//! <finding>`
def private shape_findings_for(root : string; cap : int; var rc : int&;
                               testsOnly : bool = false) : array<string> {
    let exe = get_command_line_arguments()[0]
    var output : string
    var cmd <- [exe, "-dasroot", get_das_root(), "{get_das_root()}/tests/REVIEW.das",
                "--", "--root", root, "--cap", "{cap}"]
    if (testsOnly) {
        cmd |> push("--tests-only")
    }
    rc = run_and_capture(cmd, output)
    var findings : array<string>
    for (line in split(output, "\n")) {
        let at = find(line, ": ")
        continue if (at < 0 || find(line, ".das") < 0 || line |> starts_with("REVIEW.das"))
        let fileName = base_name(slice(line, 0, at))
        let stem = find(fileName, ".das")
        findings |> push("{slice(fileName, 0, stem + 4)}: {strip(slice(line, at + 2))}")
    }
    return <- findings
}

[test(description = "every file-shape rule fires once on the fixture that breaks it, and the well-formed files are clean")]
def test_tests_review_gate_shape(tt : T?) {
    tt |> run("the shape findings, one per broken fixture file") <| @(t : T?) {
        var rc = 0
        var inscope findings <- shape_findings_for(
            "{get_das_root()}/tests/dastest/_tests_shape_fixture", 3, rc)
        t |> equal(rc, 1)
        let expected = [
            "lonely.das: a category file holds three or more `[test]` functions; this one holds 1",
            "nodesc.das: `test_undescribed` has no description",
            "failed_nohead.das: a compile-fail fixture states its claim as a `//!` line " +
                "above its first declaration",
            "plain.das: a run file with no `[test]` and no `expect` proves nothing by name",
            "spill.das: the `[test(` annotation does not close on its line",
            "big.das: 4 `[test]` functions, over the cap of 3; split the file",
            "uncategorized_rtti.das: `uncategorized_rtti.das` carries no fence option",
            "uncategorized_3.das: `uncategorized_3.das` continues `uncategorized_2.das`, " +
                "which does not exist",
            "uncategorized_persistent_heap_2.das: `uncategorized_persistent_heap_2.das` " +
                "continues `uncategorized_persistent_heap.das`, which holds 1 `[test]` " +
                    "functions, under the cap of 3",
            "noprefix.das: a compile-fail fixture's name starts with `failed_`, `cant_` " +
                "or `invalid_`",
            "uncategorized_heap.das: `uncategorized_heap.das` is named apart from its " +
                "fence signature [persistent_heap]; a fenced uncategorized file is named " +
                    "by its signature, `uncategorized_persistent_heap.das`"
        ]
        for (e in expected) {
            t |> success(findings |> has_value_prefix(e),
                         "missing: {e}\nfound:\n{join(findings, "\n")}")
        }
        t |> equal(length(findings), length(expected), "found:\n{join(findings, "\n")}")
        for (f in findings) {
            t |> success(!(f |> starts_with("uncategorized.das:"))
                         && !(f |> starts_with("uncategorized_persistent_heap.das:"))
                && !(f |> starts_with("alone.das:")), "a well-formed file fired: {f}")
        }
    }

    tt |> run("tests-only mode skips the bare script and the fixture-name rule, " +
        "nothing else") <| @(t : T?) {
        var rc = 0
        var inscope findings <- shape_findings_for(
            "{get_das_root()}/tests/dastest/_tests_shape_fixture", 3, rc, true)
        t |> equal(rc, 1)
        t |> equal(length(findings), 9, "found:\n{join(findings, "\n")}")
        for (f in findings) {
            t |> success(!(f |> starts_with("plain.das:")) && !(f |> starts_with("noprefix.das:")),
                         "a tool-tree source or fixture fired: {f}")
        }
        t |> success(findings |> has_value_prefix("lonely.das: a category file holds three"),
                     "the category rule still fires:\n{join(findings, "\n")}")
    }

    tt |> run("a plain uncategorized file that carries a fence option is a finding") <| @(t : T?) {
        var rc = 0
        var inscope findings <- shape_findings_for(
            "{get_das_root()}/tests/dastest/_tests_shape_fixture_plain", 3, rc)
        t |> equal(rc, 1)
        t |> equal(length(findings), 1, "found:\n{join(findings, "\n")}")
        let want = "uncategorized.das: `uncategorized.das` carries fence options [persistent_heap]"
        t |> success(findings |> has_value_prefix(want), "found:\n{join(findings, "\n")}")
    }
}

[test(description = "the tests/ gate flags every write whose path is built from the repo root, through helpers and bound variables alike, and nothing else")]
def test_tests_review_gate(tt : T?) {
    tt |> run("every write built from the repo root is a finding, and nothing else is") <| @(t : T?) {
        var rc = 0
        var inscope findings <- gate_findings_for("{get_das_root()}/tests/dastest/_tests_gate_fixture", rc)
        t |> equal(rc, 1)
        let expected = [
            "fopen(...) writes `out`",
            "fopen(...) writes `fixture_path(\"y.tmp\")`",
            "fopen(...) writes `\"relative.tmp\"`",
            "mkdir_rec(...) writes `GLOBAL_OUT`",
            "remove(...) writes `fixture_path(\"y.tmp\")`",
            "rename(...) writes `out`",
            "rename(...) writes `path_join(root, \"renamed.tmp\")`",
            "copy_file(...) writes `path_join(root, \"copied.tmp\")`",
            "fwrite(...) writes `path_join(get_das_root(), \"written.tmp\")`",
            "mkdir_rec(...) writes `server.upload_dir`",
            "save(...) writes `fixture_path(\"image.png\")`"
        ]
        for (e in expected) {
            t |> success(findings |> has_value_prefix(e),
                         "missing: {e}\nfound:\n{join(findings, "\n")}")
        }
        t |> equal(length(findings), length(expected), "found:\n{join(findings, "\n")}")
    }
}

[test(description = "the tests/ gate flags a language test that compiles a file from the repo tree with no web skip, and passes one that skips or supplies the source", timeout = 1200)]
def test_tests_review_gate_web_read(t : T?) {
    var rc = 0
    let fixture = "{get_das_root()}/tests/dastest/_tests_gate_fixture"
    var inscope reads <- gate_findings_for(fixture, rc, " reads `")
    t |> equal(length(reads), 1, "found:\n{join(reads, "\n")}")
    t |> success(reads |> has_value_prefix("compile_file(...) reads `path`"),
                 "found:\n{join(reads, "\n")}")
}

def private has_value_prefix(lines : array<string>; prefix : string) : bool {
    for (l in lines) {
        return true if (starts_with(l, prefix))
    }
    return false
}

[test(description = "doctest_case_names reads the quoted name of every TEST_CASE, TEST_CASE_FIXTURE and SCENARIO line with its line number, and nothing from other lines")]
def test_doctest_case_names(t : T?) {
    let text = ("#include <doctest/doctest.h>\n" +
        "TEST_CASE(\"first, plain\") \{\n" +
        "    int x = 0; const char * s = \"body; text\";\n" +
        "\}\n" +
        "TEST_CASE_FIXTURE(Fx, \"second; split\") \{\}\n" +
        "SCENARIO (\"third\") \{\}\n" +
        "TEST_CASE(bare)\n")
    var inscope cases <- doctest_case_names(text)
    t |> equal(length(cases), 3)
    return if (length(cases) != 3)
    t |> equal(cases[0].name, "first, plain")
    t |> equal(cases[0].line, 2)
    t |> equal(cases[1].name, "second; split")
    t |> equal(cases[1].line, 5)
    t |> equal(cases[2].name, "third")
    t |> equal(cases[2].line, 6)
}

