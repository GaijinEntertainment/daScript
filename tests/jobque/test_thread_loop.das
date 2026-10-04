options gen2
require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require daslib/strings_boost
[test]
def thread_steps_collect_at_a_native_boundary(t : T?) {
    var output : string
    let result = run_and_capture([get_host_binary(), "{get_das_root()}/tests/jobque/_fixture_thread_loop_collect.das"], output, 30.0)
    t |> equal(result, 0, output)
    t |> success(contains(output, "THREAD_LOOP_COLLECT_OK"), output)
}
