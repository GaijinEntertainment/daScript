options gen2
options indenting = 4

require _project_common public

require daslib/strings_boost
require daslib/fio

[test]
def test_project_flag_value(t : T?) {
    for (workers in [1, 2]) {
        t |> run("flag-shaped map, {workers} workers") @(t : T?) {
            let files = workers == 1 ? 1 : 16
            let root = build_tree("flag{workers}", "plain_fix.das", files)
            let original = fread(fixture("plain_fix.das"))
            fwrite(path_join(root, "--fix"), fread(fixture("map.das")))
            let result = run_lint(root, ["-j", "{workers}",
                "--style-only", "--enable=STYLE042", "--project=--fix"])
            t |> equal(result.code, 2, result.output)
            t |> equal(count(result.output, "STYLE042"), files,
                result.output)
            t |> equal(count(result.output, "FIXED"), 0, result.output)
            for (i in range(files)) {
                t |> equal(fread(path_join(root, "src/f{i}.das")),
                    original, "worker {workers}, file {i}")
            }
        }
    }
}

[test]
def test_project_driver_flag_value(t : T?) {
    t |> run("consumed map value") @(t : T?) {
        let root = build_tree("driver-flag", "fix.das")
        let original = fread(fixture("fix.das"))
        fwrite(path_join(root, "--fix"), fread(fixture("map.das")))
        let result = run_lint(root, ["-j", "1", "--style-only",
            "--enable=STYLE042", "--project", "--fix"])
        t |> equal(result.code, 2, result.output)
        t |> equal(count(result.output, "STYLE042"), 1, result.output)
        t |> equal(count(result.output, "FIXED"), 0, result.output)
        t |> equal(fread(path_join(root, "src/f0.das")), original)
    }
}

[test]
def test_project_shared_modules(t : T?) {
    t |> run("mapped shared module") @(t : T?) {
        let root = build_tree("shared", "shared_user.das")
        fwrite(path_join(root, "src/z_impl.das"),
            fread(fixture("shared.das")))
        for (isolated in [true, false]) {
            var inscope extra <- ["-j", "1", "--style-only",
                "--enable=STYLE042", "--project",
                path_join(root, "map.das_project")]
            extra |> push("--no-shared-modules") if (isolated)
            let result = run_lint(root, extra)
            t |> equal(result.code, 0,
                "isolated={isolated}: {result.output}")
            t |> equal(count(result.output, "PASS"), 2, result.output)
            t |> equal(count(result.output, "SKIP"), 0, result.output)
        }
    }
}
