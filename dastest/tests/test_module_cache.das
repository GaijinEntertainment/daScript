options gen2
options indenting = 4

require dastest/testing_boost public
require dastest/fs
require daslib/base64
require daslib/clargs
require daslib/fio
require strings

def cache_count(folder : string) : int {
    var count = 0
    dir(folder) $(name) {
        if (ends_with(name, ".bin")) count++
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
    var env : array<string>
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

[test]
def test_module_cache_filenames(t : T?) {
    let first = module_cache_file("cache", "first/test.das")
    t |> equal(first, module_cache_file("cache", "first/test.das"))
    t |> success(first != module_cache_file("cache", "second/test.das"))
    t |> success(ends_with(first, ".bin"))
}

//! A separate runner exposes invalid internal arguments through its exit code.
[test]
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

//! Separate runners prove cache reads across fresh processes and workers.
[test]
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
}

//! Fresh processes expose compiled macro values reused across environments.
[test]
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
