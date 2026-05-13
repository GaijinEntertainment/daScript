options gen2
require daslib/cpp_gen

[export]
def main {

    // configure

    blocked_functions <- [
        "operator delete",
        "operator new",
        "__security_init_cookie",
        "__security_check_cookie"
    ]

    // generate

    let args <- get_command_line_arguments()
    bindHeaders(args[3], args[4])
}
