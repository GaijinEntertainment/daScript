options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_compile_check(file : string; project : string = "") : string {
    return compile_and_simulate(file, project) <| $(program; issues) {
        var warnings = ""
        if (!empty(issues)) {
            warnings = "Warnings:\n{issues}\n"
        }
        var funcs = build_string() <| $(var writer) {
            write_function_list(writer, program)
        }
        return "{warnings}Compilation OK.\n{funcs}"
    }
}
