options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! Thin popen wrapper. Real logic lives in subtools/aot.das so macro
//! state from compile_file doesn't leak across MCP calls.

def do_aot(file, func_name : string; project : string = "") : string {
    return run_mcp_subtool("aot", [file, func_name, project])
}
