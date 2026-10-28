options gen2

require common public

//! Thin popen wrapper. Real logic lives in subtools/lint_tool.das so
//! macro state from compile_file doesn't leak across MCP calls.

//! With ``fix`` or ``json`` the CLI runner does the work: its fix
//! passes, its rollback of an edit that breaks the compile, and its
//! report - as text, or as its one JSON document.
def do_lint(file : string; project : string = ""; project_root : string = "";
            load_modules : array<string> = array<string>(); fix : bool = false;
            json : bool = false) : string {
    if (!fix && !json) return run_mcp_subtool("lint_tool", [file, project], project_root,
                                              load_modules)
    return make_tool_result("lint: fix and json take no project", true) if (!empty(project))
    var files : array<string>
    parse_file_list(file, files)
    return make_tool_result("no files matched: {file}", true) if (empty(files))
    var argv <- [get_daslang_exe(), path_join(get_das_root(), "utils/lint/main.das"), "--"]
    argv |> push_from(files)
    if (fix) {
        argv |> push("--fix")
    }
    if (json) {
        argv |> push("--json")
    }
    var output : string
    let rc = run_and_capture(argv, output, 600.0)
    return make_tool_result(output, rc != 0 && rc != 2)
}
