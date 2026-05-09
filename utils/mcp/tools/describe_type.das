options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! Thin popen wrapper. Real logic lives in subtools/describe_type.das so
//! macro state from compile_file doesn't leak across MCP calls.

def do_describe_type(name, module_name : string; project : string = "") : string {
    return run_mcp_subtool("describe_type", [name, module_name, project])
}
