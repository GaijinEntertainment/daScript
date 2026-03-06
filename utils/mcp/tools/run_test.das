options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_run_test(file : string) : string {
    let args <- get_command_line_arguments()
    if (empty(args)) {
        return make_tool_result("Cannot determine daslang executable path", true)
    }
    let exe = args[0]
    let cmd = "{exe} {get_das_root()}/dastest/dastest.das -- --test {file}"
    var output : string
    var exit_code = 0
    unsafe {
        exit_code = popen(cmd) <| $(f) {
            if (f == null) {
                output = "Failed to execute: {cmd}"
                return
            }
            output = build_string() <| $(var w) {
                while (!feof(f)) {
                    write(w, fgets(f))
                }
            }
        }
    }
    if (exit_code != 0) {
        return make_tool_result("Tests FAILED (exit code {exit_code}):\n{output}", true)
    }
    return make_tool_result(output)
}
