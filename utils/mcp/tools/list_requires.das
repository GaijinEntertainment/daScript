options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! Thin popen wrapper. Real logic lives in subtools/list_requires.das so
//! macro state from compile_file doesn't leak across MCP calls AND so
//! any shared module the compiled program requires (dasModuleImgui, etc.)
//! loads + unloads on the subtool's process lifecycle, not the MCP server's.

def do_list_requires(file : string; project : string = ""; project_root : string = ""; json : bool = false) : string {
    return run_mcp_subtool("list_requires", [file, project, json ? "true" : "false"], project_root)
}
