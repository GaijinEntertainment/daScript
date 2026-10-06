options gen2
options persistent_heap
options gc
options rtti

require daslib/clargs
require daslib/json_boost
require daslib/jsonrpc
require daslib/logger
require daslib/strings_convert
require ../mcp/mcp_core.das
require ../mcp/tool_result.das
require dap_bridge

//! The daslang DAP MCP bridge, stdio transport: `daslang utils/dap/main.das -- --repo-root <root>
//! --executable <daslang>`. One process owns one DAP connection and the debuggee it launched;
//! the `debug_*` tools are its surface. Runs under the watchdog's `--stdio` front like the
//! daslang MCP server.

var private g_bridge : DapBridge?

let private DAP_LOG_NAME = "dap_bridge"

let private INSTRUCTIONS = "Stateful daScript DAP bridge. For debug_launch: set breakpoints, call debug_threads, then debug_configuration_done. DAP ids returned by one stop are inputs to stack/scopes/variables/evaluate tools."

let private HOST_PORT = "\"host\":\{\"type\":\"string\",\"description\":\"DAP host; defaults to 127.0.0.1.\"\},\"port\":\{\"type\":\"integer\",\"minimum\":1,\"maximum\":65535,\"description\":\"DAP TCP port; defaults to 10000.\"\},\"timeout_sec\":\{\"type\":\"number\",\"exclusiveMinimum\":0\}"
let private LAUNCH_HOST_PORT = "\"host\":\{\"type\":\"string\",\"description\":\"DAP host; defaults to 127.0.0.1.\"\},\"port\":\{\"type\":\"integer\",\"minimum\":1,\"maximum\":65535,\"description\":\"DAP TCP port. When omitted, debug_launch selects an available local port.\"\},\"timeout_sec\":\{\"type\":\"number\",\"exclusiveMinimum\":0\}"
let private SESSION_SETTINGS = "\"cwd\":\{\"type\":\"string\"\},\"paths\":\{\"type\":\"array\",\"items\":\{\"type\":\"string\"\}\},\"path_aliases\":\{\"type\":\"object\",\"additionalProperties\":\{\"type\":\"string\"\}\},\"inline_preview_limit\":\{\"type\":\"integer\",\"minimum\":0\},\"max_children_count\":\{\"type\":\"integer\",\"minimum\":0\},\"collect_all_globals\":\{\"type\":\"boolean\"\}"
let private THREAD_ID = "\"thread_id\":\{\"type\":\"integer\",\"minimum\":1,\"description\":\"DAP thread id.\"\}"

def private schema(properties : string; required : string) : string =>
    "\{\"type\":\"object\",\"properties\":\{{properties}\},\"required\":[{required}],\"additionalProperties\":false\}"

def private tool_json(name, description, properties, required : string) : string =>
    "\{\"name\":{quoted(name)},\"description\":{quoted(description)},\"inputSchema\":{schema(properties, required)}\}"

def private call_bridge_tool(name : string; args : JsonValue?) : string {
    let outcome = g_bridge->call(name, args)
    if (outcome.ok) return make_tool_result(outcome.text)
    return make_tool_result(g_bridge->error_text(outcome.text), true)
}

def private add_tool(var reg : array<ToolDef>; name, description, properties, required : string) {
    var td : ToolDef
    td.tool.name = name
    td.tool.description = description
    td.json_handler = @@call_bridge_tool
    td.tool_json = tool_json(name, description, properties, required)
    reg |> emplace(td)
}

def private build_tools(var reg : array<ToolDef>) {
    let launch_props = "\"file\":\{\"type\":\"string\",\"description\":\"Script path.\"\},\"executable\":\{\"type\":\"string\",\"description\":\"Optional daslang binary.\"\},\"program_args\":\{\"type\":\"array\",\"items\":\{\"type\":\"string\"\}\},\"stepping_debugger\":\{\"type\":\"boolean\",\"description\":\"Use native statement stepping instead of the default instrumentation debugger; defaults to false so callers choose the debugging mode explicitly.\"\},\"optimize\":\{\"type\":\"boolean\",\"description\":\"Compile the program optimized (the default, what a plain run executes) or not (-no-optimization: every source statement and call survives, so the debugger stops where the source says; constant-folded calls otherwise never run).\"\},\"project\":\{\"type\":\"string\"\},\"project_root\":\{\"type\":\"string\"\},\"load_modules\":\{\"type\":\"array\",\"items\":\{\"type\":\"string\"\}\},{LAUNCH_HOST_PORT},{SESSION_SETTINGS}"
    let data_breakpoints = "\"breakpoints\":\{\"type\":\"array\",\"items\":\{\"type\":\"object\",\"properties\":\{\"data_id\":\{\"type\":\"string\"\},\"access_type\":\{\"type\":\"string\"\},\"condition\":\{\"type\":\"string\"\},\"hit_condition\":\{\"type\":\"string\"\},\"description\":\{\"type\":\"string\"\},\"enabled\":\{\"type\":\"boolean\"\}\},\"required\":[\"data_id\"],\"additionalProperties\":false\}\}"
    add_tool(reg, "debug_connect", "Connect to an already running daScript DAP TCP server.", HOST_PORT, "")
    add_tool(reg, "debug_initialize", "Initialize the connected DAP session and return debugger capabilities.", "", "")
    add_tool(reg, "debug_launch", "Launch a .das program with --das-wait-debugger, connect, initialize DAP, and send launch. Set breakpoints, call debug_threads, then debug_configuration_done to release startup.", launch_props, "\"file\"")
    add_tool(reg, "debug_attach", "Connect if needed, initialize DAP, and attach to a running daScript runtime.", "{HOST_PORT},{SESSION_SETTINGS}", "")
    add_tool(reg, "debug_set_breakpoints", "Replace source breakpoints for one file; pass an empty lines array to clear them.", "\"file\":\{\"type\":\"string\"\},\"lines\":\{\"type\":\"array\",\"items\":\{\"type\":\"integer\",\"minimum\":1\}\},\"source_modified\":\{\"type\":\"boolean\"\}", "\"file\",\"lines\"")
    add_tool(reg, "debug_data_breakpoint_info", "Resolve a visible variable to a daScript hardware data-breakpoint id.", "\"variables_reference\":\{\"type\":\"integer\",\"minimum\":1\},\"name\":\{\"type\":\"string\"\}", "\"variables_reference\",\"name\"")
    add_tool(reg, "debug_set_data_breakpoints", "Replace data breakpoints; pass an empty array to clear them.", data_breakpoints, "\"breakpoints\"")
    add_tool(reg, "debug_configuration_done", "Tell DAP that breakpoints and startup configuration are complete.", "", "")
    add_tool(reg, "debug_threads", "Return debuggee threads/contexts.", "", "")
    add_tool(reg, "debug_stack_trace", "Return the call stack of a DAP thread.", "{THREAD_ID},\"start_frame\":\{\"type\":\"integer\",\"minimum\":0\},\"levels\":\{\"type\":\"integer\",\"minimum\":1\}", "\"thread_id\"")
    add_tool(reg, "debug_scopes", "Return locals, arguments, state, and globals scopes for a stack frame.", "\"frame_id\":\{\"type\":\"integer\",\"minimum\":1\}", "\"frame_id\"")
    add_tool(reg, "debug_variables", "Expand a DAP variables reference.", "\"variables_reference\":\{\"type\":\"integer\",\"minimum\":1\},\"start\":\{\"type\":\"integer\",\"minimum\":0\},\"count\":\{\"type\":\"integer\",\"minimum\":1\}", "\"variables_reference\"")
    add_tool(reg, "debug_evaluate", "Evaluate an expression in a paused stack frame.", "\"expression\":\{\"type\":\"string\"\},\"frame_id\":\{\"type\":\"integer\",\"minimum\":1\},\"context\":\{\"type\":\"string\"\}", "\"expression\",\"frame_id\"")
    add_tool(reg, "debug_continue", "Continue a paused thread.", THREAD_ID, "\"thread_id\"")
    add_tool(reg, "debug_pause", "Request a running thread to pause.", THREAD_ID, "\"thread_id\"")
    add_tool(reg, "debug_step_in", "Step into the next call or statement.", THREAD_ID, "\"thread_id\"")
    add_tool(reg, "debug_step_over", "Step over the next statement (DAP next).", THREAD_ID, "\"thread_id\"")
    add_tool(reg, "debug_step_out", "Continue until the current function returns.", THREAD_ID, "\"thread_id\"")
    add_tool(reg, "debug_terminate", "Terminate the debuggee through DAP.", "", "")
    add_tool(reg, "debug_disconnect", "Disconnect DAP; daScript resumes threads and clears source breakpoints.", "", "")
    add_tool(reg, "debug_wait_event", "Wait for the next DAP event, optionally filtering by event name.", "\"event\":\{\"type\":\"string\"\},\"timeout_sec\":\{\"type\":\"number\",\"minimum\":0,\"maximum\":3600\}", "")
}

def private idle() {
    g_bridge->pump()
}

[export]
def main() : int {
    let args <- get_cli_arguments()
    var root = get_das_root()
    var exe = ""
    var timeout = DEFAULT_TIMEOUT
    var i = 0
    while (i < length(args)) {
        let arg = args[i]
        i++
        if (i >= length(args)) {
            to_log(LOG_ERROR, "dap bridge: {arg} needs a value\n")
            return 2
        }
        let value = args[i]
        i++
        if (arg == "--repo-root") {
            root = value
        } elif (arg == "--executable") {
            exe = value
        } elif (arg == "--timeout") {
            let parsed = try_to_float(value)
            if (!is_ok(parsed) || unwrap(parsed) <= 0.0) {
                to_log(LOG_ERROR, "dap bridge: --timeout needs a positive number, got {value}\n")
                return 2
            }
            timeout = unwrap(parsed)
        } else {
            to_log(LOG_ERROR, "dap bridge: unknown argument {arg}; the options are --repo-root, --executable, --timeout\n")
            return 2
        }
    }
    logger_init(DAP_LOG_NAME)
    logger_info("dap", "starting the daslang DAP MCP bridge: root {root}, executable {exe}")
    g_bridge = new DapBridge(root, timeout, exe)
    var reg : array<ToolDef>
    build_tools(reg)
    idle_hook = @@idle
    let server = McpServer(info = ServerInfo(name = "daslang-dap", version = "0.2.0"), instructions = INSTRUCTIONS)
    serve_stdio(server, reg)
    unsafe {
        delete g_bridge
    }
    return 0
}
