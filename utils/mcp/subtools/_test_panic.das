options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require ../tools/common.das public

//! Test fixture for run_mcp_subtool. Prints a marker to stdout, then panics
//! to drive a non-zero exit code through the wrapper.

[export]
def main {
    print("STDOUT_BEFORE_PANIC")
    panic("intentional panic from _test_panic subtool")
}
