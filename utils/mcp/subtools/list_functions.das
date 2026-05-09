options gen2
options rtti
options indenting = 4
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require ../tools/common.das public

//! Subprocess form of `list_functions`. Argv: <file> <project>.

def run_list_functions(file : string; project : string = "") : string {
    return compile_and_simulate(file, project) $(program; issues) {
        let res = build_string() $(var writer) {
            write_function_list(writer, program)
        }
        return empty(res) ? "(no functions found)" : res
    }
}

[export]
def main {
    let raw <- get_command_line_arguments()
    let args <- subtool_user_args(raw)
    if (length(args) < 2) {
        print(make_tool_result("list_functions subtool: expected 2 args (file, project), got {length(args)}", true))
        return
    }
    let file = string(args[0])
    let project = string(args[1])
    print(run_list_functions(file, project))
}
