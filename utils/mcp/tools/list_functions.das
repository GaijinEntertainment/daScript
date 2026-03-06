options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_list_functions(file : string) : string {
    return compile_and_simulate(file) <| $(program; issues) {
        var res = build_string() <| $(var writer) {
            write_function_list(writer, program)
        }
        return empty(res) ? "(no functions found)" : res
    }
}
