options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_list_functions(file : string) : string {
    var inscope access <- make_file_access("")
    var result : string
    var had_error = false
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(file, access, unsafe(addr(mg)), cop) <| $(ok; program; issues) {
                if (!ok) {
                    result = "Compilation failed:\n{issues}"
                    had_error = true
                } else {
                    simulate(program) <| $(sok; ctx; serrors) {
                        if (!sok) {
                            result = "Simulation failed:\n{serrors}"
                            had_error = true
                            return
                        }
                        result = build_string() <| $(var writer) {
                            write_function_list(writer, program)
                        }
                        if (empty(result)) {
                            result = "(no functions found)"
                        }
                    }
                }
            }
        }
    }
    return make_tool_result(result, had_error)
}
