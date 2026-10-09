options gen2

require daslib/fio
require strings

[export]
def main() : int {
    //! a child for test_process.das: ends through exit with the code its last argument names
    let args <- get_command_line_arguments()
    let code = empty(args) ? 42 : to_int(args[length(args) - 1])
    unsafe {
        exit(code)
    }
    return 1
}
