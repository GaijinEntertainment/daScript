options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_compile_check(file : string) : string {
    var inscope access <- make_file_access("")
    var result : string
    var had_error = false
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(file, access, unsafe(addr(mg)), cop) <| $(ok; program; issues) {
                if (!ok) {
                    result = "Compilation FAILED.\n{issues}"
                    had_error = true
                } else {
                    simulate(program) <| $(sok; ctx; serrors) {
                        if (!sok) {
                            result = "Simulation FAILED.\n{serrors}"
                            had_error = true
                        } else {
                            var warnings = ""
                            if (!empty(issues)) {
                                warnings = "Warnings:\n{issues}\n"
                            }
                            var funcs = build_string() <| $(var writer) {
                                write_function_list(writer, program)
                            }
                            result = "{warnings}Compilation OK.\n{funcs}"
                        }
                    }
                }
            }
        }
    }
    return make_tool_result(result, had_error)
}
