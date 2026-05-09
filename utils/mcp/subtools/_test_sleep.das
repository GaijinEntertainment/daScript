options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require ../tools/common.das public
require daslib/jobque

//! Test fixture for run_mcp_subtool. Sleeps for N milliseconds (argv[0])
//! and exits clean. Used to drive the timeout path in the wrapper.

[export]
def main {
    let raw <- get_command_line_arguments()
    let args <- subtool_user_args(raw)
    if (empty(args)) {
        print(make_tool_result("_test_sleep: expected 1 arg (milliseconds)", true))
        return
    }
    let ms = to_int(string(args[0]))
    sleep(uint(ms))
    print(make_tool_result("DONE_AFTER_{ms}ms"))
}
