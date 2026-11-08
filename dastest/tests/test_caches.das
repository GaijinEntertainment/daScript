options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/ast
require daslib/base64
require daslib/clargs
require daslib/fio
require strings

struct DastestRun {
    exit_code : int
    output : string
}

def cache_count(folder : string) : int {
    var count = 0
    dir(folder) $(name) {
        if (ends_with(name, ".dascache")) count++
    }
    return count
}

def run_cached(t : T?; folder, cache, phase : string;
               markers : array<string>;
               isolated = false; verbose = true;
               file = "mc_key_case.das"; environment : string = "") {
    var argv <- [get_host_binary(), "-dasroot", get_das_root(),
        "-module-cache", path_join(folder, "host.bin"),
        path_join(get_das_root(), "dastest/dastest.das"), "--",
        "--test", path_join(folder, file),
        "--module-cache-dir", cache]
    if (verbose) argv |> push("--verbose")
    if (isolated) {
        argv |> push_from(["--isolated-mode", "--isolated-mode-threads",
            "1"])
    }
    var output : string
    var env <- ["TMPDIR={folder}", "TMP={folder}", "TEMP={folder}"]
    if (!empty(environment)) env |> push(environment)
    let code = run_and_capture(argv, "", env, output, 300.0)
    t |> equal(code, 0, "{phase}: {output}")
    for (marker in markers) {
        t |> success(find(output, marker) >= 0, "{phase}: {output}")
    }
    if (!verbose) {
        t |> success(find(output, "module cache ") < 0, "{phase}: {output}")
    }
}

def private result_fixture_dir() : string => "{get_das_root()}/dastest/tests/_result_cache_fixture"

def private result_fixture_copy(folder : string) : string {
    let dst = path_join(test_temp_dir(), folder)
    mkdir_rec(dst)
    for (name in ["_helper.das", "test_uses_helper.das", "test_fails.das"]) {
        fwrite(path_join(dst, name), fread(path_join(result_fixture_dir(), name)))
    }
    return dst
}

def private run_result_cache(test_file, args : string) : DastestRun {
    let exe = get_command_line_arguments()[0]
    let cmd = "{exe} {get_das_root()}/dastest/dastest.das -- --test {test_file}{args}"
    var exit_code = 0
    let output = build_string() $(var w) {
        exit_code = unsafe(popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    w |> write(fgets(f))
                }
            }
        })
    }
    return DastestRun(exit_code = exit_code, output = output)
}

def private served_from_cache(run : DastestRun) : bool => find(run.output, "served from the result cache") >= 0

[test(description = "a module cache file name is stable per source path, ends in .dascache, and changes with the path or the compile policies")]
def test_module_cache_filenames(t : T?) {
    var policies = CodeOfPolicies()
    let first = module_cache_path("first/test.das", policies)
    t |> equal(first, module_cache_path("first/test.das", policies))
    t |> success(first != module_cache_path("second/test.das", policies))
    t |> success(ends_with(first, ".dascache"))
    t |> success(!empty(module_cache_path("", policies)))
    policies.no_optimizations = true
    t |> success(first != module_cache_path("first/test.das", policies))
}

[test(description = "a fresh runner rejects a malformed worker module-cache path with exit code 1 and a named message", timeout = 1200)]
def test_invalid_worker_module_cache_path(t : T?) {
    let malformed = base64_encode(test_temp_dir()) + "=ignored"
    for (encoded in ["#", malformed]) {
        var inscope argv <- [get_host_binary(), "-dasroot", get_das_root(),
            "-no-module-cache", path_join(get_das_root(),
                "dastest/dastest.das"), "--", "--worker-module-cache-dir",
            encoded, "--test", path_join(get_das_root(),
                "dastest/tests/_fixtures/mc_key_case.das")]
        var output : string
        let code = run_and_capture(argv, output, 300.0)
        t |> equal(code, 1, output)
        t |> success(find(output, "invalid worker module-cache path") >= 0,
            output)
    }
}

[test(description = "the persistent module cache keys one record per test file and survives across processes, plain and isolated, falling back on a same-stamp source edit", timeout = 1200)]
def test_dastest_cache_per_test_file(t : T?) {
    let folder = test_temp_dir()
    let fixtures = path_join(get_das_root(), "dastest/tests/_fixtures")
    for (name in ["mc_key_mod.das", "mc_key_case.das"]) {
        t |> success(fwrite(path_join(folder, name),
            fread(path_join(fixtures, name))))
    }
    let module_path = path_join(folder, "mc_key_mod.das")
    let staged_path = path_join(folder, "module.edit")
    let original = fread(module_path)
    var staged = false
    var attempts = 0
    while (attempts < 10) {
        attempts++
        if (fwrite(module_path, original)
                && fwrite(staged_path, replace(original, "1111", "2222"))
                && stat(module_path).mtime == stat(staged_path).mtime) {
            staged = true
            break
        }
    }
    t |> strictEqual(staged, true, "stage a same-stamp source edit")
    let cache = path_join(folder, "caches with spaces")
    run_cached(t, folder, cache, "cold", ["key=1111", "module cache none:"])
    t |> equal(cache_count(cache), 1)
    run_cached(t, folder, cache, "warm", ["key=1111", "module cache clean:"])
    let before = stat(module_path)
    t |> success(remove(module_path))
    t |> success(rename(staged_path, module_path))
    let after = stat(module_path)
    t |> equal(before.mtime, after.mtime)
    t |> equal(before.size, after.size)
    run_cached(t, folder, cache, "same-stamp edit",
        ["key=2222", "module cache fallback:"])
    let worker_cache = path_join(folder, get_platform_name() == "windows"
        ? "worker caches %PATH%/" : "worker caches $USER ' \" quotes")
    run_cached(t, folder, worker_cache, "isolated cold",
        ["key=2222", "module cache none:"], true)
    t |> equal(cache_count(worker_cache), 1)
    run_cached(t, folder, worker_cache, "isolated warm",
        ["key=2222", "module cache clean:"], true)
    run_cached(t, folder, worker_cache, "isolated quiet", ["key=2222"],
        true, false)
    t |> success(fwrite(path_join(folder, "mc_other_case.das"),
        fread(path_join(fixtures, "mc_key_case.das"))))
    run_cached(t, folder, cache, "another test file",
        ["key=2222", "module cache none:"], false, true, "mc_other_case.das")
    t |> equal(cache_count(cache), 2)
}

[test(description = "a changed compile-time environment value misses the module cache in plain and isolated runs", timeout = 1200)]
def test_dastest_cache_compile_environment(t : T?) {
    let folder = test_temp_dir()
    let fixtures = path_join(get_das_root(), "dastest/tests/_fixtures")
    for (name in ["mc_env_mod.das", "mc_env_value.das", "mc_env_case.das"]) {
        t |> success(fwrite(path_join(folder, name),
            fread(path_join(fixtures, name))))
    }
    for (isolated in [false, true]) {
        let cache = path_join(folder, isolated ? "workers" : "plain")
        run_cached(t, folder, cache, "environment cold",
            ["env=1111", "module cache none:"], isolated, true,
            "mc_env_case.das", "DAS_CACHE_KEY_PROBE=1111")
        run_cached(t, folder, cache, "environment warm",
            ["env=1111", "module cache clean:"], isolated, true,
            "mc_env_case.das", "DAS_CACHE_KEY_PROBE=1111")
        run_cached(t, folder, cache, "environment changed",
            ["env=2222", "module cache none:"], isolated, true,
            "mc_env_case.das", "DAS_CACHE_KEY_PROBE=2222")
    }
}

[test(description = "a fresh runner reloads project policies at startup, so a project that forbids persistent_heap fails and writes no cache", timeout = 1200)]
def test_dastest_cache_project_options(t : T?) {
    let folder = test_temp_dir()
    let fixtures = path_join(get_das_root(), "dastest/tests/_fixtures")
    let name = "mc_project_case.das"
    t |> success(fwrite(path_join(folder, name),
        fread(path_join(fixtures, name))))
    let source = fread(path_join(fixtures, "mc_project.das_project"))
    let project = path_join(folder, "mc_project.das_project")
    let cache = path_join(folder, "project caches")
    let argv <- [get_host_binary(), "-dasroot", get_das_root(),
        "-no-module-cache", path_join(get_das_root(), "dastest/dastest.das"),
        "--", "--test", path_join(folder, name), "--compile-only",
        "--test-project", project, "--module-cache-dir", cache, "--verbose"]
    for (allow in [true, false]) {
        t |> success(fwrite(project, allow ? source
            : replace(source, "ALLOW_HEAP = true", "ALLOW_HEAP = false")))
        var output : string
        let code = run_and_capture(argv, output, 300.0)
        t |> equal(code, allow ? 0 : 1, output)
        if (!allow) {
            t |> success(find(output,
                "option persistent_heap is not allowed here") >= 0, output)
        }
    }
    t |> success(!fexist(cache), "custom project contexts are not cached")
}

[test(description = "the dastest result cache serves an unchanged passing file, keeps its entry across report-only flags, and misses on changed arguments, edited modules, failures and no --cache-dir", timeout = 1200)]
def test_result_cache(tt : T?) {
    tt |> run("a passing file is served from the cache on the next run") <| @(t : T?) {
        let dir = result_fixture_copy("warm")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        let first = run_result_cache(file, cache)
        t |> equal(first.exit_code, 0)
        t |> success(!served_from_cache(first), "a cold cache runs the file:\n{first.output}")
        let second = run_result_cache(file, cache)
        t |> equal(second.exit_code, 0)
        t |> success(served_from_cache(second), "the second run reuses the pass:\n{second.output}")
        t |> success(find(second.output, "1 tests, 1 passed") >= 0, "a cached file still counts its tests:\n{second.output}")
    }

    tt |> run("a report-only flag keeps the entry") <| @(t : T?) {
        let dir = result_fixture_copy("report_only")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_result_cache(file, cache)
        let rerun = run_result_cache(file, "{cache} --failures-only --isolated-mode")
        t |> success(served_from_cache(rerun), "--failures-only and --isolated-mode change no result:\n{rerun.output}")
    }

    tt |> run("a pass an isolated worker reported is recorded") <| @(t : T?) {
        let dir = result_fixture_copy("isolated")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_result_cache(file, "{cache} --isolated-mode")
        let rerun = run_result_cache(file, cache)
        t |> success(served_from_cache(rerun), "the worker's report carries its inputs:\n{rerun.output}")
    }

    tt |> run("an argument that changes the run misses") <| @(t : T?) {
        let dir = result_fixture_copy("semantic_arg")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_result_cache(file, cache)
        let rerun = run_result_cache(file, "{cache} --test-names test_helper")
        t |> success(!served_from_cache(rerun), "--test-names picks other tests:\n{rerun.output}")
    }

    tt |> run("an edited required module runs the file again") <| @(t : T?) {
        let dir = result_fixture_copy("edited")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_result_cache(file, cache)
        let helper = path_join(dir, "_helper.das")
        fwrite(helper, "{fread(helper)}\n")
        let rerun = run_result_cache(file, cache)
        t |> equal(rerun.exit_code, 0)
        t |> success(!served_from_cache(rerun), "the helper's content changed:\n{rerun.output}")
    }

    tt |> run("a failing file is never cached") <| @(t : T?) {
        let dir = result_fixture_copy("failing")
        let file = path_join(dir, "test_fails.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_result_cache(file, cache)
        let rerun = run_result_cache(file, cache)
        t |> success(rerun.exit_code != 0, "the fixture fails:\n{rerun.output}")
        t |> success(!served_from_cache(rerun), "a failure runs again:\n{rerun.output}")
    }

    tt |> run("no --cache-dir caches nothing") <| @(t : T?) {
        let dir = result_fixture_copy("opt_in")
        let file = path_join(dir, "test_uses_helper.das")
        run_result_cache(file, "")
        let rerun = run_result_cache(file, "")
        t |> success(!served_from_cache(rerun), "the cache is opt-in:\n{rerun.output}")
    }
}
