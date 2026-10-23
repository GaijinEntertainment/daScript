options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/strings_boost
require daslib/clargs
require daslib/fio

struct LintRun {
    code : int
    output : string
}

def private fixture(name : string) : string {
    return path_join(get_das_root(),
        "utils/lint/tests/module_cache/_fixtures/_fixture_{name}")
}

def private build_tree(name, source : string; files : int = 1) : string {
    let root = path_join(test_temp_dir(), name)
    let src = path_join(root, "src")
    let dep = path_join(root, "dep")
    mkdir_rec(src)
    mkdir_rec(dep)
    fwrite(path_join(root, "README.md"), "## Outside \{#outside\}\n")
    fwrite(path_join(dep, "hidden.das"), fread(fixture("hidden.das")))
    fwrite(path_join(dep, "invalid.das"), fread(fixture("invalid.das")))
    fwrite(path_join(root, "map.das_project"),
        fread(fixture("map.das")))
    for (i in range(files)) {
        fwrite(path_join(src, "f{i}.das"), fread(fixture(source)))
    }
    return root
}

//! Fresh processes expose CLI exits and worker flag forwarding.
def private run_lint(root : string; extra : array<string>) : LintRun {
    var argv <- [get_host_binary(), "-dasroot", get_das_root(),
        "-no-module-cache", path_join(get_das_root(), "utils/lint/main.das"),
        "--", "--cache-dir", path_join(root, "cache"),
        path_join(root, "src")]
    argv |> push_from(extra)
    let env <- ["TMPDIR={root}", "TMP={root}", "TEMP={root}"]
    var result : LintRun
    result.code = run_and_capture(argv, root, env, result.output)
    return result
}

[test]
def test_project_resolution(t : T?) {
    for (workers in [1, 2]) {
        let files = workers == 1 ? 1 : 16
        let root = build_tree("workers{workers}", "warning.das", files)
        t |> run("unmapped requires skip with {workers} workers") @(t : T?) {
            let result = run_lint(root, ["-j", "{workers}"])
            t |> equal(result.code, 0, result.output)
            t |> equal(count(result.output, "SKIP"), files, result.output)
            t |> equal(count(result.output, "STYLE027"), 0, result.output)
        }
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

[test]
def test_project_errors(t : T?) {
    for (workers in [1, 2]) {
        for (kind in ["missing", "broken"]) {
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
            t |> equal(result.code, kind == "error" ? 1 : 0, result.output)
            t |> equal(count(result.output, "FAIL "),
                kind == "error" ? 1 : 0, result.output)
            t |> equal(count(result.output, "SKIP"),
                kind == "library" ? 1 : 0, result.output)
            t |> equal(count(result.output, "native library unavailable"),
                kind == "library" ? 1 : 0, result.output)
        }
    }
}

[test]
def test_project_fix(t : T?) {
    for (workers in [1, 2]) {
        t |> run("fix keeps map, {workers} workers") @(t : T?) {
            let files = workers == 1 ? 1 : 16
            let root = build_tree("fix{workers}", "fix.das", files)
            let result = run_lint(root, ["--fix", "-j", "{workers}",
                "--project", path_join(root, "map.das_project")])
            t |> equal(result.code, 0, result.output)
            t |> equal(count(result.output, "SKIP"), 0, result.output)
            t |> equal(count(result.output, "FIXED"), files,
                result.output)
            for (i in range(files)) {
                let fixed = fread(path_join(root, "src/f{i}.das"))
                t |> success(find(fixed, "=> hidden_value()") >= 0,
                    fixed)
            }
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
    for (workers in [1, 2]) {
        t |> run("consumed map value, {workers} workers") @(t : T?) {
            let files = workers == 1 ? 1 : 16
            let root = build_tree("driver-flag{workers}", "fix.das", files)
            let original = fread(fixture("fix.das"))
            fwrite(path_join(root, "--fix"), fread(fixture("map.das")))
            let result = run_lint(root, ["-j", "{workers}",
                "--style-only", "--enable=STYLE042", "--project", "--fix"])
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
def test_project_shared_fix_recovery(t : T?) {
    for (workers in [1, 2]) {
        for (kind in ["invalid", "missing"]) {
            t |> run("shared recovery: {kind}, {workers} workers") @(t : T?) {
                let files = workers == 1 ? 1 : 16
                let root = build_tree("shared-{kind}{workers}",
                    "fix.das", files)
                let original = fread(fixture("fix.das"))
                fwrite(path_join(root, "dep/hidden.das"),
                    fread(fixture("hidden_shared.das")))
                fwrite(path_join(root, "reject_fix"), kind)
                let result = run_lint(root, ["--fix", "-j", "{workers}",
                    "--project", path_join(root, "map.das_project")])
                t |> equal(result.code, 2, result.output)
                t |> equal(count(result.output, "SKIP"), 0, result.output)
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
}

[test]
def test_project_shared_modules(t : T?) {
    for (workers in [1, 2]) {
        t |> run("mapped shared module, {workers} workers") @(t : T?) {
            let files = workers == 1 ? 2 : 16
            let root = build_tree("shared{workers}", "shared_user.das",
                files - 1)
            fwrite(path_join(root, "src/z_impl.das"),
                fread(fixture("shared.das")))
            for (isolated in [true, false]) {
                var inscope extra <- ["-j", "{workers}", "--style-only",
                    "--enable=STYLE042", "--project",
                    path_join(root, "map.das_project")]
                extra |> push("--no-shared-modules") if (isolated)
                let result = run_lint(root, extra)
                t |> equal(result.code, 0,
                    "isolated={isolated}: {result.output}")
                t |> equal(count(result.output, "PASS"), files,
                    result.output)
                t |> equal(count(result.output, "SKIP"), 0,
                    result.output)
            }
        }
    }
}
