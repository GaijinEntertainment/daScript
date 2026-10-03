options gen2
require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require _exe_test_common

[test]
def thread_loop_collects_after_steps_and_preserves_capture_ownership(t : T?) {
    let root = get_das_root()
    let fixture = "{root}/tests/jobque/_fixture_thread_loop_collect.das"
    var output : string
    let interpreted = run_and_capture([get_host_binary(), "-no-module-cache", fixture], output, 60.0)
    t |> equal(interpreted, 0, output)
    t |> success(contains(output, "THREAD_LOOP_COLLECT_OK"), output)
    let temporary = create_temp_directory_result("thread_loop_collect")
    t |> success(temporary is value)
    if (!(temporary is value)) { return }
    let dir = temporary as value
    let built = run_and_capture([get_host_binary(), "-jit", "-exe", "-output", "{dir}/worker", fixture], output, 180.0)
    t |> equal(built, 0, output)
    if (built == 0 && fexist("{dir}/worker.exe")) {
        let ran = run_standalone("{dir}/worker.exe", "", output, 60.0)
        t |> equal(ran, 0, output)
        t |> success(contains(output, "THREAD_LOOP_COLLECT_OK"), output)
    } else { t |> failure("standalone artifact missing") }
    let removed = rmdir_rec_result(dir)
    t |> success(removed is value && removed as value)
}

[test]
def thread_loop_requires_release_at_finish_and_keeps_nested_checks(t : T?) {
    let temporary = create_temp_directory_result("thread_loop_release")
    t |> success(temporary is value)
    if (!(temporary is value)) { return }
    let dir = temporary as value
    let fixture = "{get_das_root()}/tests/jobque/_fixture_thread_loop_release.das"
    var output : string
    let built = run_and_capture([get_host_binary(), "-jit", "-exe", "-output", "{dir}/worker", fixture], output, 180.0)
    t |> equal(built, 0, output)
    for (mode in ["final", "nested"]) {
        let result = run_and_capture([get_host_binary(), "-no-module-cache", fixture, "--", mode], output, 20.0)
        t |> success(result != 0, output)
        t |> success(contains(output, mode == "final" ? "thread loop's final step" : "missing channel|>release"), output)
        if (built == 0 && fexist("{dir}/worker.exe")) {
            let native_result = run_standalone("{dir}/worker.exe", "-- {mode}", output, 20.0)
            t |> success(native_result != 0, output)
            t |> success(contains(output, mode == "final" ? "thread loop's final step" : "missing channel|>release"), output)
        } else { t |> failure("standalone artifact missing") }
    }
    let removed = rmdir_rec_result(dir)
    t |> success(removed is value && removed as value)
}
