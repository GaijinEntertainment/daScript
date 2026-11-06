options gen2
options indenting = 4

require _project_common public

require daslib/strings_boost
require daslib/fio
require daslib/json_boost

[test]
def test_project_resolution(t : T?) {
    let serial = build_tree("serial", "warning.das")
    t |> run("unmapped requires skip") @(t : T?) {
        let result = run_lint(serial, ["-j", "1"])
        t |> equal(result.code, 0, result.output)
        t |> equal(count(result.output, "SKIP"), 1, result.output)
        t |> equal(count(result.output, "STYLE027"), 0, result.output)
    }
    t |> run("project JSON") @(t : T?) {
        let result = run_lint(serial, ["-j", "1", "--json", "--style-only",
            "--enable=STYLE027", "--project",
            path_join(serial, "map.das_project")])
        t |> equal(result.code, 2, result.output)
        var error = ""
        var report = read_json(result.output, error)
        t |> strictEqual(error, "", result.output)
        t |> equal(report?["totals"]?["issues"] ?? -1, 1)
        t |> equal(report?["totals"]?["errors"] ?? -1, 0)
        t |> equal(report?["totals"]?["skipped"] ?? -1, 0)
        t |> equal(report?["results"]?[0]?["count"] ?? -1, 1)
        t |> equal(report?["results"]?[0]?["issues"]?[0]?["code"] ?? "",
            "STYLE027")
        unsafe { delete report; }
    }
    for (workers in [1, 2]) {
        let files = workers == 1 ? 1 : 16
        let root = build_tree("workers{workers}", "warning.das", files)
        t |> run("project requires lint with {workers} workers") @(t : T?) {
            let result = run_lint(root, ["-j", "{workers}", "--project",
                path_join(root, "map.das_project")])
            t |> equal(result.code, 2, result.output)
            t |> equal(count(result.output, "SKIP"), 0, result.output)
            t |> equal(count(result.output, "STYLE027"), files,
                result.output)
            t |> equal(count(result.output, "LINT026"), 0, result.output)
        }
    }
}
