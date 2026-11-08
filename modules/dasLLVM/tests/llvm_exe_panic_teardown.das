options gen2
require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings
require _exe_test_common

// A standalone exe whose main panics while a job is parked on a
// channel exits with code 1: jit_run_main_guarded asks the que to
// quiesce and exits without the teardown that would join the parked
// worker forever. The fixture is the interpreter test's, under
// tests/jobque, built here as an exe.

[test(description = "a standalone exe whose main panics with a job parked on a channel exits with code 1 instead of hanging", timeout = 1200)]
def test_exe_panic_with_parked_job_exits(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_panic_teardown")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("the exe prints the panic and exits 1 within the bound") @(t : T?) {
        let root = "tests/jobque/_fixture_panic_parked_job.das"
        let exe = path_join(dir, "panic_teardown.exe")
        var bout = ""
        let brc = run_and_capture([das_exe(), "-jit", "-exe", "-output", exe, root], bout, 300.0)
        t |> equal(0, brc, "the -exe build succeeds: {tail(bout)}")
        if (brc != 0 || !stat(exe).is_valid) return
        var out = ""
        let rc = run_standalone(exe, "", out, 60.0)
        t |> equal(1, rc, "the exe exits 1: {tail(out)}")
        t |> success(find(out, "EXCEPTION: boom") >= 0, "the panic is printed: {tail(out)}")
        t |> success(find(out, "exiting without teardown") >= 0,
                     "and the host says why it skipped teardown: {tail(out)}")
    }
    rmdir_rec_result(dir)
}
