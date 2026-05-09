options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! Thin popen wrapper. Real logic lives in subtools/list_module_api.das so
//! macro state from compile_file doesn't leak across MCP calls.

def do_list_module_api(module_name : string; filter : string = ""; section : string = ""; compact_str : string = ""; project : string = "") : string {
    return run_mcp_subtool("list_module_api", [module_name, filter, section, compact_str, project])
}
