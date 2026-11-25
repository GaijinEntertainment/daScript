options gen2
options indenting = 4

require dastest/testing_boost public
require dastest/check_cache
require daslib/ast
require daslib/base64
require daslib/clargs
require daslib/fio
require daslib/strings_boost
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
    for (name in ["_helper.das", "test_uses_helper.das", "test_fails.das", "test_reads_verdict.das",
            "_gate_macro.das", "_gate_lib.das", "test_gated.das"]) {
        fwrite(path_join(dst, name), fread(path_join(result_fixture_dir(), name)))
    }
    fwrite(path_join(dst, "verdict.txt"), "pass")
    return dst
}

def private run_result_cache(test_file, args : string) : DastestRun =>
    run_dastest("--test {test_file}{args}")

//! a child dastest over `args` (split at spaces), its own module
//! cache under the test's folder, `env` a NAME=VALUE of its
//! environment or "", `cwd` its working directory or "" for this
//! one's
def private run_dastest(args : string; env : string = ""; cwd : string = "") : DastestRun {
    var argv <- [get_command_line_arguments()[0], "-dasroot", get_das_root(),
        "-module-cache", path_join(test_temp_dir(), "result_cache_host.bin"),
        path_join(get_das_root(), "dastest/dastest.das"), "--"]
    argv |> push_from([for (word in split(args, " ")); word; where !empty(word)])
    var envs : array<string>
    if (!empty(env)) {
        envs |> push(env)
    }
    var output : string
    let exit_code = run_and_capture(argv, cwd, envs, output, 300.0)
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

[test(description = "a fresh runner rejects a malformed worker module-cache path, run-env pair or policy pair with exit code 1 and a message naming which", timeout = 1200)]
def test_invalid_worker_module_cache_path(t : T?) {
    let malformed = base64_encode(test_temp_dir()) + "=ignored"
    let flags = [("--worker-module-cache-dir", "invalid worker module-cache path"),
        ("--worker-run-env", "invalid worker run-env pair"),
        ("--worker-policy", "invalid worker policy pair")]
    for (flag in flags) {
        for (encoded in ["#", malformed]) {
            var inscope argv <- [get_host_binary(), "-dasroot", get_das_root(),
                "-no-module-cache", path_join(get_das_root(),
                    "dastest/dastest.das"), "--", flag._0,
                encoded, "--test", path_join(get_das_root(),
                    "dastest/tests/_fixtures/mc_key_case.das")]
            var output : string
            let code = run_and_capture(argv, output, 300.0)
            t |> equal(code, 1, output)
            t |> success(find(output, flag._1) >= 0, output)
        }
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

def private skipped_as_cached(run : DastestRun; file : string) : bool =>
    find(run.output, "CACHED {file}") >= 0

//! the flags of a `--skip-fully-cached` run whose records go under
//! `dir`
def private skip_run(dir : string) : string =>
    " --module-cache-dir {path_join(dir, "records")} --skip-fully-cached"

//! the folder of pass marks under `cache`, "" where none is
def private pass_marks_under(cache : string) : string {
    var found = ""
    dir(cache) $(name) {
        if (ends_with(name, ".passed")) {
            found = path_join(cache, name)
        }
    }
    return found
}

[test(description = "--skip-fully-cached reports a file from its last pass when its program loaded whole from its record - only under the flag, from a pass a run without the flag left too, and file by file under --together with no simulate where every file is skipped - and without --module-cache-dir stops dastest with exit code 2", timeout = 1200)]
def test_skip_fully_cached(tt : T?) {
    tt |> run("a pass is skipped on the next run, and only under the flag") <| @(t : T?) {
        let dir = result_fixture_copy("skip_warm")
        let file = path_join(dir, "test_uses_helper.das")
        let cached = " --module-cache-dir {path_join(dir, "records")}"
        let cold = run_result_cache(file, "{cached} --skip-fully-cached")
        t |> equal(cold.exit_code, 0, cold.output)
        t |> success(!skipped_as_cached(cold, file), "a cold record runs the file:\n{cold.output}")
        let plain = run_result_cache(file, cached)
        t |> success(!skipped_as_cached(plain, file),
            "without the flag the file runs:\n{plain.output}")
        let warm = run_result_cache(file, "{cached} --skip-fully-cached --verbose")
        t |> equal(warm.exit_code, 0, warm.output)
        t |> success(skipped_as_cached(warm, file),
            "the flagged run skips the pass, under a report-only flag too:\n{warm.output}")
        t |> success(find(warm.output, "1 tests, 1 passed") >= 0,
            "a skipped file still counts its tests:\n{warm.output}")
    }

    tt |> run("a run without the flag keeps the pass a flagged run skips") <| @(t : T?) {
        let dir = result_fixture_copy("skip_primed")
        let file = path_join(dir, "test_uses_helper.das")
        let cached = " --module-cache-dir {path_join(dir, "records")}"
        run_result_cache(file, cached)
        let warm = run_result_cache(file, "{cached} --skip-fully-cached")
        t |> success(skipped_as_cached(warm, file),
            "the plain run's pass is on record:\n{warm.output}")
    }

    tt |> run("--together skips file by file, and the whole program when all passed") <| @(t : T?) {
        let dir = result_fixture_copy("skip_together")
        let passing = path_join(dir, "test_uses_helper.das")
        let failing = path_join(dir, "test_fails.das")
        let twin = path_join(dir, "test_reads_verdict.das")
        let trace = path_join(dir, "started.txt")
        let flags = " --module-cache-dir {path_join(dir, "records")} --skip-fully-cached"
        let mixed = "--together {passing} --together {failing}{flags}"
        run_dastest(mixed)
        let rerun = run_dastest(mixed)
        t |> success(rerun.exit_code != 0, "the failing file fails again:\n{rerun.output}")
        t |> success(skipped_as_cached(rerun, passing),
            "the passing file of the program is skipped:\n{rerun.output}")
        t |> success(!skipped_as_cached(rerun, failing), "the failing one runs:\n{rerun.output}")
        let green = "--together {passing} --together {twin}{flags}"
        run_dastest(green)
        t |> success(fexist(trace) && remove(trace),
            "control: a program that runs leaves its [init]'s trace")
        let warm = run_dastest(green)
        t |> success(!fexist(trace),
            "a program whose every file is skipped is not simulated: no [init] ran")
        t |> equal(warm.exit_code, 0, warm.output)
        t |> success(skipped_as_cached(warm, passing) && skipped_as_cached(warm, twin),
            "both files are skipped:\n{warm.output}")
        t |> success(find(warm.output, "2 tests, 2 passed") >= 0, "and counted:\n{warm.output}")
    }

    tt |> run("the flag without --module-cache-dir is refused") <| @(t : T?) {
        let dir = result_fixture_copy("skip_alone")
        let file = path_join(dir, "test_uses_helper.das")
        let refused = run_result_cache(file, " --skip-fully-cached")
        t |> equal(refused.exit_code, 2, refused.output)
        let plain = run_dastest("--test {file}", "", dir)
        t |> equal(plain.exit_code, 0, plain.output)
        t |> success(!fexist(path_join(dir, ".passed")), "a run with no record keeps no pass mark")
        t |> success(find(refused.output,
            "--skip-fully-cached needs --module-cache-dir") >= 0, refused.output)
    }
}

[test(description = "--skip-fully-cached runs a file again on an edited module, after a failure - one with nothing changed too -, and on marks a dead compile left held", timeout = 1200)]
def test_skip_fully_cached_reruns(tt : T?) {
    tt |> run("an edited module runs the file again, and a failure leaves no pass") <| @(t : T?) {
        let dir = result_fixture_copy("skip_edited")
        let file = path_join(dir, "test_uses_helper.das")
        let cached = " --module-cache-dir {path_join(dir, "records")}"
        run_result_cache(file, "{cached} --skip-fully-cached")
        let helper = path_join(dir, "_helper.das")
        let good = fread(helper)
        fwrite(helper, replace(good, "42", "41"))
        let broken = run_result_cache(file, cached)
        t |> success(broken.exit_code != 0,
            "the edit fails the test, in a run without the flag:\n{broken.output}")
        let again = run_result_cache(file, "{cached} --skip-fully-cached")
        t |> success(again.exit_code != 0,
            "the record the failing run rewrote carries no pass:\n{again.output}")
        t |> success(!skipped_as_cached(again, file), "a failure runs again:\n{again.output}")
        fwrite(helper, good)
        let fixed = run_result_cache(file, "{cached} --skip-fully-cached")
        t |> equal(fixed.exit_code, 0, fixed.output)
        t |> success(!skipped_as_cached(fixed, file), "the fix runs:\n{fixed.output}")
        let warm = run_result_cache(file, "{cached} --skip-fully-cached")
        t |> success(skipped_as_cached(warm, file), "and its pass is skipped after:\n{warm.output}")
    }

    tt |> run("a failure with nothing changed takes the pass off the record") <| @(t : T?) {
        let dir = result_fixture_copy("skip_flaky")
        let file = path_join(dir, "test_reads_verdict.das")
        let cached = " --module-cache-dir {path_join(dir, "records")}"
        run_result_cache(file, "{cached} --skip-fully-cached")
        fwrite(path_join(dir, "verdict.txt"), "fail")
        let red = run_result_cache(file, cached)
        t |> success(red.exit_code != 0,
            "the file a test reads at run time fails it, the record unchanged:\n{red.output}")
        fwrite(path_join(dir, "verdict.txt"), "pass")
        let after = run_result_cache(file, "{cached} --skip-fully-cached")
        t |> equal(after.exit_code, 0, after.output)
        t |> success(!skipped_as_cached(after, file),
            "the pass before the failure vouches for nothing:\n{after.output}")
        let helper = path_join(dir, "test_uses_helper.das")
        let records = path_join(dir, "together")
        let together = "--together {file} --together {helper} --module-cache-dir {records}"
        run_dastest("{together} --skip-fully-cached")
        fwrite(path_join(dir, "verdict.txt"), "fail")
        run_dastest(together)
        fwrite(path_join(dir, "verdict.txt"), "pass")
        let mixed = run_dastest("{together} --skip-fully-cached")
        t |> success(!skipped_as_cached(mixed, file) && skipped_as_cached(mixed, helper),
            "under --together the failed file alone lost its pass:\n{mixed.output}")
    }

    tt |> run("marks a dead compile left held are not trusted") <| @(t : T?) {
        let dir = result_fixture_copy("skip_held")
        let file = path_join(dir, "test_uses_helper.das")
        let records = path_join(dir, "records")
        let flags = " --module-cache-dir {records} --skip-fully-cached"
        run_result_cache(file, flags)
        let marks = pass_marks_under(records)
        t |> success(!empty(marks), "the pass left its marks beside the record")
        t |> success(rename(marks, "{marks}.held"), "stage the state a compile that died leaves")
        let rerun = run_result_cache(file, flags)
        t |> equal(rerun.exit_code, 0, rerun.output)
        t |> success(!skipped_as_cached(rerun, file),
            "held marks vouch for nothing:\n{rerun.output}")
    }
}

[test(description = "--skip-fully-cached skips on a record read as partial - a module that registers at compile time compiles again in place - until that compile fails, never skips under --bench or --cov-path, and a panic leaves no pass", timeout = 1200)]
def test_skip_fully_cached_partial_and_never(tt : T?) {
    tt |> run("a record read as partial vouches too, until its compile fails") <| @(t : T?) {
        let dir = result_fixture_copy("skip_partial")
        let file = path_join(dir, "test_gated.das")
        let flags = " --module-cache-dir {path_join(dir, "records")} --skip-fully-cached --verbose"
        run_result_cache(file, flags)
        let warm = run_result_cache(file, flags)
        t |> success(find(warm.output, "module cache partial:") >= 0,
            "a module that registers at compile time is compiled again in place:\n{warm.output}")
        t |> success(skipped_as_cached(warm, file),
            "its unchanged program is skipped all the same:\n{warm.output}")
        fwrite(path_join(dir, "verdict.txt"), "compile-fail")
        let broken = run_result_cache(file, flags)
        t |> success(broken.exit_code != 0,
            "the module compiled in place fails, every record matched:\n{broken.output}")
        fwrite(path_join(dir, "verdict.txt"), "pass")
        let after = run_result_cache(file, flags)
        t |> equal(after.exit_code, 0, after.output)
        t |> success(!skipped_as_cached(after, file),
            "a compile that failed left no pass:\n{after.output}")
    }

    tt |> run("a benchmark run and a coverage run never skip") <| @(t : T?) {
        let dir = result_fixture_copy("skip_bench")
        let file = path_join(dir, "test_uses_helper.das")
        for (measured in ["--bench", "--cov-path {path_join(dir, "cov.txt")}"]) {
            let flags = "{skip_run(dir)} {measured}"
            run_result_cache(file, flags)
            let rerun = run_result_cache(file, flags)
            t |> success(!skipped_as_cached(rerun, file),
                "{measured} runs what it measures:\n{rerun.output}")
        }
    }

    tt |> run("a panic leaves no pass") <| @(t : T?) {
        let dir = result_fixture_copy("skip_panic")
        let file = path_join(dir, "test_reads_verdict.das")
        let flags = " --module-cache-dir {path_join(dir, "records")} --skip-fully-cached"
        fwrite(path_join(dir, "verdict.txt"), "panic")
        run_result_cache(file, flags)
        let rerun = run_result_cache(file, flags)
        t |> success(rerun.exit_code != 0 && !skipped_as_cached(rerun, file),
            "an error is no pass:\n{rerun.output}")
    }
}

[test(description = "a pass --skip-fully-cached trusts is kept by the caller's --pass-stamp, in an isolated worker too, and by the DAS* environment of the compile", timeout = 1200)]
def test_skip_fully_cached_keys(tt : T?) {
    tt |> run("another --pass-stamp runs the file") <| @(t : T?) {
        let dir = result_fixture_copy("skip_stamp")
        let file = path_join(dir, "test_uses_helper.das")
        let flags = "{skip_run(dir)} --pass-stamp"
        run_result_cache(file, "{flags} one")
        let other = run_result_cache(file, "{flags} two")
        t |> success(!skipped_as_cached(other, file), "the caller's stamp changed:\n{other.output}")
        let same = run_result_cache(file, "{flags} one")
        t |> success(skipped_as_cached(same, file),
            "the first stamp's pass still holds:\n{same.output}")
    }

    tt |> run("an isolated worker keeps its pass by the stamp") <| @(t : T?) {
        let dir = result_fixture_copy("skip_worker_stamp")
        let file = path_join(dir, "test_reads_verdict.das")
        let flags = "{skip_run(dir)} --isolated-mode --isolated-mode-threads 1 --pass-stamp"
        let first = run_result_cache(file, "{flags} one")
        t |> equal(first.exit_code, 0, first.output)
        fwrite(path_join(dir, "verdict.txt"), "fail")
        let same = run_result_cache(file, "{flags} one")
        t |> equal(same.exit_code, 0,
            "under the same stamp the worker skips, the failing file unread:\n{same.output}")
        let other = run_result_cache(file, "{flags} two")
        t |> success(other.exit_code != 0,
            "under another stamp the worker runs the file:\n{other.output}")
    }

    tt |> run("another DAS* environment value runs the file") <| @(t : T?) {
        let dir = result_fixture_copy("skip_env")
        let file = path_join(dir, "test_uses_helper.das")
        let args = "--test {file}{skip_run(dir)}"
        run_dastest(args, "DAS_CACHE_KEY_PROBE=1111")
        let same = run_dastest(args, "DAS_CACHE_KEY_PROBE=1111")
        t |> success(skipped_as_cached(same, file),
            "the same environment skips the pass:\n{same.output}")
        let other = run_dastest(args, "DAS_CACHE_KEY_PROBE=2222")
        t |> equal(other.exit_code, 0, other.output)
        t |> success(!skipped_as_cached(other, file),
            "another value names another record, with no pass beside it:\n{other.output}")
    }
}

[test(description = "--run-env sets a variable for a compiled test's run under --test, --together and isolated workers, byte for byte, two values of it share one compile record where the same values in the environment make two, and a pair with no NAME stops dastest with exit code 2", timeout = 1200)]
def test_run_env(t : T?) {
    let exe = get_command_line_arguments()[0]
    let root = get_das_root()
    let fixture = "{root}/dastest/tests/_fixture_run_env.das"
    let stamp = get_time_usec(0l)
    var output : string
    let unset <- ["DASTEST_RUN_ENV_PROBE="]
    let host_cache = path_join(test_temp_dir(), "run_env_host_{stamp}.bin")
    var plain <- [exe, "-dasroot", root, "-module-cache", host_cache,
        "{root}/dastest/dastest.das", "--", "--failures-only", "--test", fixture]
    let plain_rc = run_and_capture(plain, "", unset, output, 300.0)
    t |> success(plain_rc != 0, "control: without the flag the probe is not set: {output}")
    let by_flag = path_join(test_temp_dir(), "run_env_flag_{stamp}")
    let by_env = path_join(test_temp_dir(), "run_env_env_{stamp}")
    for (value in ["one", "two"]) {
        var inscope flagged := plain
        flagged |> push_from(["--module-cache-dir", by_flag,
            "--run-env", "DASTEST_RUN_ENV_PROBE={value}"])
        let by_flag_rc = run_and_capture(flagged, "", unset, output, 300.0)
        t |> equal(by_flag_rc, 0, "the flag sets the probe to '{value}' for the run: {output}")
        var inscope exported := plain
        exported |> push_from(["--module-cache-dir", by_env])
        let by_env_rc = run_and_capture(exported, "", ["DASTEST_RUN_ENV_PROBE={value}"],
            output, 300.0)
        t |> equal(by_env_rc, 0, "the environment sets it too: {output}")
    }
    t |> equal(cache_count(by_flag), 1, "two values by the flag compile once: one record")
    t |> equal(cache_count(by_env), 2, "control: two values in the environment name two records")
    let together <- [exe, "-dasroot", root, "-module-cache", host_cache,
        "{root}/dastest/dastest.das", "--", "--failures-only",
        "--together", fixture, "--run-env", "DASTEST_RUN_ENV_PROBE=on"]
    let together_rc = run_and_capture(together, "", unset, output, 300.0)
    t |> equal(together_rc, 0, "the flag reaches a --together run: {output}")
    let shell_text = "a b $HOME \"quoted\" 'single'"
    var inscope isolated := plain
    isolated |> push_from(["--isolated-mode", "--isolated-mode-threads", "1",
        "--run-env", "DASTEST_RUN_ENV_PROBE={shell_text}"])
    let isolated_env <- ["DASTEST_RUN_ENV_PROBE=", "DASTEST_RUN_ENV_EXPECT={shell_text}"]
    let isolated_rc = run_and_capture(isolated, "", isolated_env, output, 300.0)
    t |> equal(isolated_rc, 0, "an isolated worker gets the value byte for byte: {output}")
    for (bad in ["DASTEST_RUN_ENV_PROBE", "=no_name"]) {
        var inscope refused := plain
        refused |> push_from(["--run-env", bad])
        let refused_rc = run_and_capture(refused, "", unset, output, 300.0)
        t |> equal(refused_rc, 2, "'{bad}' is not NAME=VALUE: {output}")
        let named = find(output, "--run-env '{bad}'") >= 0
        t |> success(named, "the line names the flag and the pair: {output}")
    }
    var err : string
    rmdir_rec(by_flag, err)
    rmdir_rec(by_env, err)
}

[test(description = "a recorded pass of an external check is found again under its kind and key and under no other, the marker file is the whole record, and a tool no name answers to has no path and no stamp")]
def test_check_cache(t : T?) {
    set_check_cache_dir(path_join(test_temp_dir(), "checks"))
    let key = hash("test_check_cache")
    t |> success(!check_passed("selftest", key), "nothing is recorded for a new key")
    record_check_passed("selftest", key)
    t |> success(check_passed("selftest", key), "a recorded pass is found again")
    t |> success(!check_passed("selftest", key + 1ul), "another key did not pass")
    t |> success(!check_passed("selftest2", key), "another kind did not pass")
    remove(passed_marker_path("selftest", key))
    t |> success(!check_passed("selftest", key), "the marker file is the whole record")
    set_check_cache_dir("")
    let absent = vulkan_sdk_tool("no-such-tool-for-check-cache")
    t |> success(empty(absent.exe) && empty(absent.stamp),
        "an absent tool has no path and no stamp")
}

[test(description = "a tool the Vulkan SDK folder holds is found there with no child started, and its stamp is its path, size and time")]
def test_vulkan_sdk_tool(t : T?) {
    let sdk = path_join(test_temp_dir(), "fake_sdk")
    mkdir_rec(path_join(sdk, "Bin"))
    mkdir_rec(path_join(sdk, "bin"))
    // not a program: the SDK arm must take it without running it
    fwrite(path_join(sdk, "Bin/check-cache-fake.exe"), "not a program")
    fwrite(path_join(sdk, "bin/check-cache-fake"), "not a program")
    with_env_variables([(name = "VULKAN_SDK", value = sdk)]) {
        let tool = vulkan_sdk_tool("check-cache-fake")
        t |> success(tool.exe |> starts_with(sdk), "the SDK's file is the tool: {tool.exe}")
        let sized = (tool.stamp |> starts_with("{tool.exe}|")
            && length(tool.stamp) > length(tool.exe) + 2)
        t |> success(sized, "the stamp is the path with the file's size and time: {tool.stamp}")
    }
}
