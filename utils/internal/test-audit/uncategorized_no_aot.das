options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

def private run_audit(args : array<string>; var output : string&) : int {
    let exe = get_command_line_arguments()[0]
    let root = get_das_root()
    var cmd <- [exe, "-dasroot", root, "{root}/utils/internal/test-audit/main.das", "--"]
    cmd |> push_from(args)
    return run_and_capture(cmd, output)
}

//! a two-folder test tree: a described suite, an undescribed one, a
//! compile-fail fixture
def private make_tree(t : T?) : string {
    let tree = path_join(test_temp_dir(), "audit_tree")
    mkdir_rec(path_join(tree, "alpha"))
    mkdir_rec(path_join(tree, "beta"))
    t |> success(fwrite(path_join(tree, "alpha/uncategorized.das"),
        "options gen2\nrequire dastest/testing_boost public\n\n" +
        "[test(description = \"one and one make two\")]\ndef test_sum(t : T?) \{\n" +
        "    t |> equal(1 + 1, 2)\n\}\n"))
    t |> success(fwrite(path_join(tree, "beta/pairs.das"), "options gen2\nrequire " +
        "dastest/testing_boost public\n\n[test]\ndef test_undescribed(t : T?) \{\n\}\n"))
    t |> success(fwrite(path_join(tree, "beta/failed_bad.das"), "//! a string is not an " +
        "int\nexpect 30101\ndef main() \{\n    let x : int = \"no\"\n\}\n"))
    t |> success(fwrite(path_join(tree, "beta/_helper.das"), "module _helper\n"))
    return tree
}

[test(description = "the audit counts a tree's run files by class and its undescribed tests, --tests-only leaves a tool's sources and an installed package out, and several roots share one audit", timeout = 1200)]
def test_audit(tt : T?) {
    let tree = make_tree(tt)

    tt |> run("the audit reports files, suites, fixtures and the undescribed count") <| @(t : T?) {
        var output : string
        let rc = run_audit(["--root", tree], output)
        t |> equal(rc, 0, output)
        t |> success(output |> contains("files: 3  (suites 2, expect fixtures 1, bare scripts 0)"),
                     "expected the class counts, got:\n{output}")
        t |> success(output |> contains("tests without a description: 1"),
                     "expected one undescribed test, got:\n{output}")
    }

    tt |> run("--tests-only leaves a tool's sources out") <| @(t : T?) {
        let tool = path_join(test_temp_dir(), "audit_tool_tree")
        mkdir_rec(path_join(tool, "lint/tests"))
        t |> success(fwrite(path_join(tool, "lint/main.das"),
            "options gen2\n[export]\ndef main() \{\n    print(\"a tool\")\n\}\n"))
        t |> success(fwrite(path_join(tool, "lint/tests/uncategorized.das"),
            "options gen2\nrequire dastest/testing_boost public\n\n" +
            "[test(description = \"the tool answers\")]\ndef test_answers(t : T?) \{\n" +
            "    t |> equal(1, 1)\n\}\n"))
        var output : string
        t |> equal(run_audit(["--root", tool], output), 0, output)
        t |> success(output |> contains("bare scripts 1"),
                     "without --tests-only the source counts as a script, got:\n{output}")
        t |> equal(run_audit(["--root", tool, "--tests-only"], output), 0, output)
        t |> success(output |> contains("files: 1  (suites 1, expect fixtures 0, bare scripts 0)"),
                     "with --tests-only the source is out, got:\n{output}")
        // a daspkg-installed package under the tool is not the tool's tests
        mkdir_rec(path_join(tool, "lint/modules/pkg"))
        t |> success(fwrite(path_join(tool, "lint/.das_package"), "options gen2\n"))
        t |> success(fwrite(path_join(tool, "lint/modules/pkg/test_pkg.das"),
            "options gen2\nrequire dastest/testing_boost public\n\n[test]\n" +
            "def test_pkg(t : T?) \{\n\}\n"))
        t |> equal(run_audit(["--root", tool, "--tests-only"], output), 0, output)
        t |> success(output |> contains("files: 1  (suites 1,"),
                     "the installed package's test is not counted, got:\n{output}")
    }

    tt |> run("several --root flags share one audit") <| @(t : T?) {
        var output : string
        let roots <- ["--root", path_join(tree, "alpha"), "--root", path_join(tree, "beta")]
        t |> equal(run_audit(roots, output), 0, output)
        t |> success(output |> contains("files: 3  (suites 2, expect fixtures 1, bare scripts 0)"),
                     "expected both roots counted together, got:\n{output}")
    }
}
