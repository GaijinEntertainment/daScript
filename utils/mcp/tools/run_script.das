options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def do_run_script(file, code : string) : string {
    let exe = get_daslang_exe()
    if (empty(exe)) {
        return make_tool_result("Cannot determine daslang executable path", true)
    }
    // if code is provided, write to temp file
    var script_path = file
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
    }
    if (empty(script_path)) {
        return make_tool_result("Provide either 'file' or 'code' argument", true)
    }
    var output : string
    let exit_code = run_and_capture("{exe} {script_path}", output)
    if (exit_code != 0) {
        return make_tool_result("Script failed (exit code {exit_code}):\n{output}", true)
    }
    return make_tool_result(output)
}
