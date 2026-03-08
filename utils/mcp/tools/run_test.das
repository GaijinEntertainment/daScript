options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_run_test(file : string; project : string = "") : string {
    let exe = get_daslang_exe()
    if (empty(exe)) {
        return make_tool_result("Cannot determine daslang executable path", true)
    }
    var cmd = "{exe} {get_das_root()}/dastest/dastest.das -- --test {file}"
    if (!empty(project)) {
        cmd = "{cmd} --project {project}"
    }
    var output : string
    let exit_code = run_and_capture(cmd, output)
    if (exit_code != 0) {
        return make_tool_result("Tests FAILED (exit code {exit_code}):\n{output}", true)
    }
    return make_tool_result(output)
}
