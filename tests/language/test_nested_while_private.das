options gen2
options no_aot // the shape under test runs in a spawned interpreter child

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require daslib/command_line

//! a return that leaves only the inner loop hangs the child, and only
//! a parent process can time a hang out
[test]
def test_a_private_function_ending_in_a_nested_while_true_returns(t : T?) {
    if (get_running_platform_name() == "emscripten") {
        t |> skip("no child process on the web")
        return
    }
    let fixture = path_join(get_das_root(), "tests/language/_fixture_nested_while_private.das")
    var output : string
    let argv <- [get_das_exe(), "-dasroot", get_das_root(), "-no-module-cache", fixture]
    let rc = run_and_capture(argv, output, child_deadline(20.0))
    t |> equal(0, rc, "the child returned from run and exited, got:\n{output}")
    t |> success(output |> contains("2129000"),
                 "the child printed the accumulated value, got:\n{output}")
}
