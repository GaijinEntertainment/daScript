options gen2
options indenting = 4

require _project_common public

require strings
require daslib/strings_boost
require daslib/fio

[test]
def test_project_errors(t : T?) {
    for (workers in [1, 2]) {
        for (kind in ["missing", "broken"]) {
            continue if (workers == 2 && kind == "missing")
            t |> run("{kind} map, {workers} workers") @(t : T?) {
                let files = workers == 1 ? 1 : 16
                let root = build_tree("{kind}{workers}", "warning.das", files)
                let project = path_join(root, "{kind}.das_project")
                if (kind == "broken") {
                    fwrite(project, fread(fixture("broken_map.das")))
                }
                let result = run_lint(root, ["-j", "{workers}",
                    "--project", project])
                t |> success(result.code != 0, result.output)
                t |> success(find(result.output, project) >= 0, result.output)
            }
        }
    }
}

[test]
def test_project_compile_errors(t : T?) {
    for (kind in ["error", "library"]) {
        t |> run("mapped compile failure: {kind}") @(t : T?) {
            let root = build_tree("compile-{kind}", "{kind}.das")
            let result = run_lint(root, ["--project",
                path_join(root, "map.das_project")])
            let extern_disabled = (kind == "library"
                && get_running_platform_name() == "windows"
                && typeinfo sizeof(type<void?>) == 4)
            let failed = kind == "error" || extern_disabled
            let skipped = kind == "library" && !extern_disabled
            t |> equal(result.code, failed ? 1 : 0, result.output)
            t |> equal(count(result.output, "FAIL "),
                failed ? 1 : 0, result.output)
            t |> equal(count(result.output, "SKIP"),
                skipped ? 1 : 0, result.output)
            t |> equal(count(result.output, "native library unavailable"),
                skipped ? 1 : 0, result.output)
            t |> equal(count(result.output, "extern functions disabled"),
                extern_disabled ? 1 : 0, result.output)
        }
    }
}
