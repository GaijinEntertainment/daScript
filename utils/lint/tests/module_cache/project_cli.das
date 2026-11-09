options gen2
options indenting = 4

require _project_common public

require strings
require daslib/strings_boost
require daslib/fio
require daslib/json_boost

[test(description = "a missing or broken project map fails the lint run and names the map, serial or parallel", timeout = 1200)]
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

[test(description = "a mapped file that fails to compile fails the run, and an unavailable native library is a skip", timeout = 1200)]
def test_project_compile_errors(t : T?) {
    for (kind in ["error", "library"]) {
        t |> run("mapped compile failure: {kind}") @(t : T?) {
            let root = build_tree("compile-{kind}", "{kind}.das")
            // the library arm asks the host which native modules it
            // loads; the standalone exe answers for itself alone
            let result = run_lint(root, ["--project",
                path_join(root, "map.das_project")], kind != "library")
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

[test(description = "--fix with a project map keeps the map and applies the fix", timeout = 1200)]
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

[test(description = "a --fix whose patched result cannot compile is rolled back, leaving the source and one STYLE042 finding", timeout = 1200)]
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

[test(description = "a --fix that breaks a shared mapped module is rolled back and reported", timeout = 1200)]
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

[test(description = "a project compile never takes a clean result from the cache, and a changed map skips instead", timeout = 1200)]
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

[test(description = "a flag-shaped project map value is consumed as the map, serial or parallel, not as a flag", timeout = 1200)]
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

[test(description = "a project value that spells a flag is taken as the map by the driver", timeout = 1200)]
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

[test(description = "a mapped shared module lints the same with and without sharing", timeout = 1200)]
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

[test(description = "a project map resolves requires that lint otherwise skips, in text and JSON output, serial or parallel", timeout = 1200)]
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
