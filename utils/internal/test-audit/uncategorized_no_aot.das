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

[test(description = "the audit counts a tree's run files by class, --readme writes the index from the descriptions, and --verify exits 0 on a matching index (LF or CRLF) and 1 on an edited one", timeout = 1200)]
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

    tt |> run("--readme writes the index and --verify accepts it") <| @(t : T?) {
        var output : string
        let wrote = run_audit(["--root", tree, "--readme"], output)
        t |> equal(wrote, 0, output)
        let index = fread(path_join(tree, "README.md"))
        t |> success(index |> contains("## alpha/"), "expected the folder section, got:\n{index}")
        t |> success(index |> contains("- `test_sum` - one and one make two"),
                     "expected the described test, got:\n{index}")
        t |> success(index |> contains("- `test_undescribed` - *(no description)*"),
                     "expected the undescribed marker, got:\n{index}")
        t |> success(index |> contains("compile-fail fixture: a string is not an int"),
                     "expected the fixture's claim, got:\n{index}")
        t |> success(!(index |> contains("_helper")), "a _ file is not a run file:\n{index}")
        var verifyOutput : string
        t |> equal(run_audit(["--root", tree, "--verify"], verifyOutput), 0, verifyOutput)
    }

    tt |> run("--verify accepts the index checked out with CRLF line endings") <| @(t : T?) {
        let indexPath = path_join(tree, "README.md")
        let lf = fread(indexPath)
        t |> success(fwrite(indexPath, lf |> replace("\n", "\r\n")))
        var output : string
        t |> equal(run_audit(["--root", tree, "--verify"], output), 0, output)
        t |> success(fwrite(indexPath, lf))
    }

    tt |> run("--verify exits 1 on an index that differs from the census") <| @(t : T?) {
        let indexPath = path_join(tree, "README.md")
        t |> success(fwrite(indexPath, fread(indexPath) + "\nhand edit\n"))
        var output : string
        let rc = run_audit(["--root", tree, "--verify"], output)
        t |> equal(rc, 1, output)
        t |> success(output |> contains("differs from the census"),
                     "expected the mismatch message, got:\n{output}")
    }
}
