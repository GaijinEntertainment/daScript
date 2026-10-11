options gen2

require dastest/testing_boost public
require daslib/fio
require strings

struct DastestRun {
    exit_code : int
    output : string
}

def private fixture_dir() : string => "{get_das_root()}/dastest/tests/_result_cache_fixture"

def private fixture_copy(folder : string) : string {
    let dst = path_join(test_temp_dir(), folder)
    mkdir_rec(dst)
    for (name in ["_helper.das", "test_uses_helper.das", "test_fails.das"]) {
        fwrite(path_join(dst, name), fread(path_join(fixture_dir(), name)))
    }
    return dst
}

def private run_dastest(test_file, args : string) : DastestRun {
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

[test]
def test_result_cache(tt : T?) {
    tt |> run("a passing file is served from the cache on the next run") <| @(t : T?) {
        let dir = fixture_copy("warm")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        let first = run_dastest(file, cache)
        t |> equal(first.exit_code, 0)
        t |> success(!served_from_cache(first), "a cold cache runs the file:\n{first.output}")
        let second = run_dastest(file, cache)
        t |> equal(second.exit_code, 0)
        t |> success(served_from_cache(second), "the second run reuses the pass:\n{second.output}")
        t |> success(find(second.output, "1 tests, 1 passed") >= 0, "a cached file still counts its tests:\n{second.output}")
    }

    tt |> run("a report-only flag keeps the entry") <| @(t : T?) {
        let dir = fixture_copy("report_only")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_dastest(file, cache)
        let rerun = run_dastest(file, "{cache} --failures-only --isolated-mode")
        t |> success(served_from_cache(rerun), "--failures-only and --isolated-mode change no result:\n{rerun.output}")
    }

    tt |> run("a pass an isolated worker reported is recorded") <| @(t : T?) {
        let dir = fixture_copy("isolated")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_dastest(file, "{cache} --isolated-mode")
        let rerun = run_dastest(file, cache)
        t |> success(served_from_cache(rerun), "the worker's report carries its inputs:\n{rerun.output}")
    }

    tt |> run("an argument that changes the run misses") <| @(t : T?) {
        let dir = fixture_copy("semantic_arg")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_dastest(file, cache)
        let rerun = run_dastest(file, "{cache} --test-names test_helper")
        t |> success(!served_from_cache(rerun), "--test-names picks other tests:\n{rerun.output}")
    }

    tt |> run("an edited required module runs the file again") <| @(t : T?) {
        let dir = fixture_copy("edited")
        let file = path_join(dir, "test_uses_helper.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_dastest(file, cache)
        let helper = path_join(dir, "_helper.das")
        fwrite(helper, "{fread(helper)}\n")
        let rerun = run_dastest(file, cache)
        t |> equal(rerun.exit_code, 0)
        t |> success(!served_from_cache(rerun), "the helper's content changed:\n{rerun.output}")
    }

    tt |> run("a failing file is never cached") <| @(t : T?) {
        let dir = fixture_copy("failing")
        let file = path_join(dir, "test_fails.das")
        let cache = " --cache-dir {path_join(dir, "cache")}"
        run_dastest(file, cache)
        let rerun = run_dastest(file, cache)
        t |> success(rerun.exit_code != 0, "the fixture fails:\n{rerun.output}")
        t |> success(!served_from_cache(rerun), "a failure runs again:\n{rerun.output}")
    }

    tt |> run("no --cache-dir caches nothing") <| @(t : T?) {
        let dir = fixture_copy("opt_in")
        let file = path_join(dir, "test_uses_helper.das")
        run_dastest(file, "")
        let rerun = run_dastest(file, "")
        t |> success(!served_from_cache(rerun), "the cache is opt-in:\n{rerun.output}")
    }
}
