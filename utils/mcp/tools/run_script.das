options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_run_script(file, code : string) : string {
    let args <- get_command_line_arguments()
    if (empty(args)) {
        return make_tool_result("Cannot determine daslang executable path", true)
    }
    let exe = args[0]
    // if code is provided, write to temp file
    var script_path = file
    var used_temp = false
    if (!empty(code)) {
        script_path = "{get_das_root()}/_mcp_stub.das"
        var write_ok = false
        fopen(script_path, "wb") <| $(fw) {
            if (fw == null) {
                return
            }
            write_ok = true
            fw |> fprint(code)
        }
        if (!write_ok) {
            return make_tool_result("Cannot write temp file: {script_path}", true)
        }
        used_temp = true
    }
    if (empty(script_path)) {
        return make_tool_result("Provide either 'file' or 'code' argument", true)
    }
    let cmd = "{exe} {script_path}"
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
        return make_tool_result("Script failed (exit code {exit_code}):\n{output}", true)
    }
    return make_tool_result(output)
}
