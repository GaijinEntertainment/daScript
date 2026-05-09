options gen2
options indenting = 4
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

//! `compile_check` MCP wrapper. Forwards to subtools/compile_check.das so
//! every call gets a fresh daslang.exe — no stale [function_macro] /
//! [call_macro] state across calls. The subtool's stdout (already a
//! make_tool_result(...) JSON envelope) is returned verbatim.

def do_compile_check(file : string; project : string = ""; json : bool = false) : string {
    return run_mcp_subtool("compile_check", [file, project, json ? "true" : "false"])
}
