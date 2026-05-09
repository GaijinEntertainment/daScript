options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! Thin popen wrapper. Real logic lives in subtools/find_symbol.das so
//! macro state from compile_file doesn't leak across MCP calls.

def do_find_symbol(query : string; kind : string = ""; file : string = ""; project : string = ""; with_cpp_source : bool = false; cpp_dirs : string = "") : string {
    return run_mcp_subtool("find_symbol", [query, kind, file, project, with_cpp_source ? "true" : "false", cpp_dirs])
}
