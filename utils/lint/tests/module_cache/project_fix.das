options gen2
options indenting = 4

require _project_common public

require strings
require daslib/strings_boost
require daslib/fio

[test]
def test_project_fix(t : T?) {
    t |> run("fix keeps map") @(t : T?) {
        let root = build_tree("fix", "fix.das")
        let result = run_lint(root, ["--fix", "-j", "1", "--project",
            path_join(root, "map.das_project")])
        t |> equal(result.code, 0, result.output)
        t |> equal(count(result.output, "SKIP"), 0, result.output)
        t |> equal(count(result.output, "FIXED"), 1, result.output)
        let fixed = fread(path_join(root, "src/f0.das"))
        t |> success(find(fixed, "=> hidden_value()") >= 0, fixed)
    }
}

[test]
def test_project_fix_recovery(t : T?) {
    for (kind in ["invalid", "missing"]) {
        t |> run("fix recovery: {kind} mapped module") @(t : T?) {
            let root = build_tree("recovery-{kind}", "fix.das")
            let source = path_join(root, "src/f0.das")
            let original = fread(source)
            fwrite(path_join(root, "reject_fix"), kind)
            let result = run_lint(root, ["--fix", "--project",
                path_join(root, "map.das_project")])
            t |> equal(result.code, 2, result.output)
            t |> equal(count(result.output, "SKIP"), 0, result.output)
            t |> equal(count(result.output, "STYLE042"), 1, result.output)
            t |> equal(count(result.output, "FIXED"), 0, result.output)
            t |> equal(fread(source), original)
            t |> equal(fread(path_join(root, "compile_log")),
                "original\npatched\npatched\noriginal\n")
        }
    }
}

[test]
def test_project_shared_fix_recovery(t : T?) {
    for (kind in ["invalid", "missing"]) {
        t |> run("shared recovery: {kind}") @(t : T?) {
            let root = build_tree("shared-{kind}", "fix.das")
            let original = fread(fixture("fix.das"))
            fwrite(path_join(root, "dep/hidden.das"),
                fread(fixture("hidden_shared.das")))
            fwrite(path_join(root, "reject_fix"), kind)
            let result = run_lint(root, ["--fix", "-j", "1", "--project",
                path_join(root, "map.das_project")])
            t |> equal(result.code, 2, result.output)
            t |> equal(count(result.output, "SKIP"), 0, result.output)
            t |> equal(count(result.output, "STYLE042"), 1, result.output)
            t |> equal(count(result.output, "FIXED"), 0, result.output)
            t |> equal(fread(path_join(root, "src/f0.das")), original)
        }
    }
}

[test]
def test_project_cache(t : T?) {
    t |> run("project compiles bypass clean results") @(t : T?) {
        let root = build_tree("cache", "fix.das")
        let project = path_join(root, "map.das_project")
        let fixed = run_lint(root, ["--fix", "--project", project])
        t |> equal(fixed.code, 0, fixed.output)
        for (attempt in range(2)) {
            let clean = run_lint(root, ["--project", project])
            t |> equal(clean.code, 0, "run {attempt}: {clean.output}")
            t |> equal(count(clean.output, "PASS"), 1, clean.output)
            t |> equal(count(clean.output, "CACHED"), 0, clean.output)
        }
        fwrite(project, fread(fixture("unmapped.das")))
        let changed = run_lint(root, ["--project", project])
        t |> equal(changed.code, 0, changed.output)
        t |> equal(count(changed.output, "SKIP"), 1, changed.output)
        t |> equal(count(changed.output, "CACHED"), 0, changed.output)
    }
}
