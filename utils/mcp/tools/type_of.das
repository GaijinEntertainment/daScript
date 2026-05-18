options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! Thin popen wrapper. Real logic lives in subtools/type_of.das so macro
//! state from compile_file doesn't leak across MCP calls AND so any shared
//! module the compiled program requires (dasModuleImgui, etc.) loads +
//! unloads on the subtool's process lifecycle, not the MCP server's.

def public do_type_of(file : string; line_str, col_str : string; no_opt_str : string = ""; project : string = ""; project_root : string = "") : string {
    return run_mcp_subtool("type_of", [file, line_str, col_str, no_opt_str, project], project_root)
}
