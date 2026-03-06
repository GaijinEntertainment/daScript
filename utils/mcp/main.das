// Minimal MCP (Model Context Protocol) server for daslang
//
// Exposes compiler diagnostics and program introspection over HTTP.
// Uses Streamable HTTP transport (POST to /mcp endpoint).
//
// Run: daslang.exe utils/mcp/main.das -- [port]
// Config in Claude Code settings.json:
//   "mcpServers": { "daslang": { "url": "http://localhost:9500/mcp" } }

options gen2
options persistent_heap
options gc
options multiple_contexts
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dashv/dashv_boost
require daslib/json
require daslib/json_boost public
require daslib/jobque_boost
require strings

require tools/compile_check public
require tools/list_functions public
require tools/list_types public
require tools/run_test public
require tools/list_module_api public
require tools/list_modules public
require tools/find_symbol public
require tools/format_file public
require tools/run_script public
require tools/ast_dump public

let MCP_PORT = 9500
let HEAP_COLLECT_THRESHOLD = 1024ul * 1024ul  // 1 MB
let HEAP_LOG_INTERVAL = 60000l                // log heap stats every 60 seconds

// ── MCP protocol structs ─────────────────────────────────────────────

struct ServerInfo {
    name : string
    version : string
}

struct ToolsCapability {}

struct Capabilities {
    tools : ToolsCapability
}

struct InitializeResult {
    protocolVersion : string
    capabilities : Capabilities
    serverInfo : ServerInfo
}

struct RpcError {
    code : int
    message : string
}

// ── JSON-RPC envelope (built as string since id is dynamic) ──────────

def json_value_to_string(v : JsonValue?) : string {
    if (v == null) {
        return "null"
    }
    return write_json(v)
}

def json_rpc_response(id_json, result_json : string) : string {
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id_json},\"result\":{result_json}\}"
}

def json_rpc_error(id_json : string; code : int; message : string) : string {
    let err = RpcError(code = code, message = message)
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id_json},\"error\":{sprint_json(err, false)}\}"
}

// ── MCP protocol handlers ─────────────────────────────────────────────

def handle_initialize(id_json : string) : string {
    let result = InitializeResult(
        protocolVersion = "2025-11-25",
        capabilities = Capabilities(),
        serverInfo = ServerInfo(name = "daslang", version = "0.1.0")
    )
    return json_rpc_response(id_json, sprint_json(result, false))
}

struct MakeFileTool {
    name : string
    description : string
    inputSchema : InputSchema
}

struct InputSchema {
    @rename = "type" _type : string
    properties : table<string; PropertySchema>
    required : array<string>
}

struct PropertySchema {
    @rename = "type" _type : string
    description : string
}

struct ToolsListResult {
    tools : array<MakeFileTool>
}

def make_file_tool(name, description : string) : MakeFileTool {
    return MakeFileTool(
        name = name,
        description = description,
        inputSchema = InputSchema(
            _type = "object",
            properties = {
                "file" => PropertySchema(
                    _type = "string",
                    description = "Path to the .das file"
                )
            },
            required = ["file"]
        )
    )
}

def make_tool(name, description : string; var props : table<string; PropertySchema>; var req : array<string>) : MakeFileTool {
    return MakeFileTool(
        name = name,
        description = description,
        inputSchema = InputSchema(
            _type = "object",
            properties <- props,
            required <- req
        )
    )
}

def handle_tools_list(id_json : string) : string {
    var result : ToolsListResult
    result.tools |> emplace(make_file_tool("compile_check", "Compile a daScript file and return errors or success"))
    result.tools |> emplace(make_file_tool("list_functions", "Compile a daScript file and list all functions (after macro expansion)"))
    result.tools |> emplace(make_file_tool("list_types", "Compile a daScript file and list all structs, classes, enums, and type aliases"))
    result.tools |> emplace(make_file_tool("run_test", "Run dastest on a .das test file and return pass/fail results"))
    result.tools |> emplace(make_file_tool("format_file", "Format a .das source file in place using the daslang code formatter"))
    result.tools |> emplace(make_tool(
        "run_script",
        "Run a daslang script and return its stdout output. Provide either a file path or inline code.",
        {
            "file" => PropertySchema(_type = "string", description = "Path to the .das file to run"),
            "code" => PropertySchema(_type = "string", description = "Inline daslang code to run (written to temp file and executed)")
        },
        []
    ))
    result.tools |> emplace(make_tool(
        "ast_dump",
        "Dump AST of a daslang expression or function. mode='source' shows post-macro daslang code (via describe), mode='ast' (default) shows S-expression with node types.",
        {
            "expression" => PropertySchema(_type = "string", description = "A daslang expression to dump (e.g. 'a.b[0]', 'foo(x, y + 1)')"),
            "file" => PropertySchema(_type = "string", description = "Path to a .das file — dumps AST of compiled functions"),
            "function" => PropertySchema(_type = "string", description = "Function name to dump (optional, defaults to all functions)"),
            "mode" => PropertySchema(_type = "string", description = "Output mode: 'ast' (default) for S-expression, 'source' for post-macro daslang code")
        },
        []
    ))
    result.tools |> emplace(make_tool(
        "list_modules",
        "List all available daslang modules (builtin and daslib)",
        {},
        []
    ))
    result.tools |> emplace(make_tool(
        "find_symbol",
        "Search for a symbol (function, struct, enum, global) across all registered modules by substring match",
        {
            "query" => PropertySchema(_type = "string", description = "Case-insensitive substring search. Prefix with '=' for exact case-sensitive match (e.g. '=print')"),
            "kind" => PropertySchema(_type = "string", description = "Limit to kind: 'function', 'generic', 'struct', 'handled', 'field', 'enum', 'global'"),
            "file" => PropertySchema(_type = "string", description = "Optional .das file — if provided, searches all modules loaded by that file (including daslib)")
        },
        ["query"]
    ))
    result.tools |> emplace(make_tool(
        "list_module_api",
        "List all functions, types, enums, and globals exported by a daslang module (e.g. 'math', 'strings', 'fio', 'ast', 'daslib/json')",
        {
            "module" => PropertySchema(_type = "string", description = "Module name (e.g. 'math', 'strings', 'fio', 'ast', 'daslib/json')"),
            "filter" => PropertySchema(_type = "string", description = "Substring filter on names (e.g. 'for_each', 'json')"),
            "section" => PropertySchema(_type = "string", description = "Limit to section: 'functions', 'generics', 'operators', 'structs', 'handled', 'enums', 'globals'")
        },
        ["module"]
    ))
    return json_rpc_response(id_json, sprint_json(result, false))
}

// ── Tool dispatch (runs in cloned context via new_thread) ────────────

struct ToolMessage {
    text : string
}

def get_string_arg(args : JsonValue?; name : string) : string {
    if (args == null || !(args.value is _object)) {
        return ""
    }
    let v = (args.value as _object)?[name] ?? null
    if (v == null || !(v.value is _string)) {
        return ""
    }
    return v.value as _string
}

def dispatch_tool(tool_name, arg1, arg2, arg3, arg4 : string) : string {
    if (tool_name == "compile_check") {
        return do_compile_check(arg1)
    } elif (tool_name == "list_functions") {
        return do_list_functions(arg1)
    } elif (tool_name == "list_types") {
        return do_list_types(arg1)
    } elif (tool_name == "run_test") {
        return do_run_test(arg1)
    } elif (tool_name == "format_file") {
        return do_format_file(arg1)
    } elif (tool_name == "run_script") {
        return do_run_script(arg1, arg2)
    } elif (tool_name == "ast_dump") {
        return do_ast_dump(arg1, arg2, arg3, arg4)
    } elif (tool_name == "list_modules") {
        return do_list_modules()
    } elif (tool_name == "find_symbol") {
        return do_find_symbol(arg1, arg2, arg3)
    } elif (tool_name == "list_module_api") {
        return do_list_module_api(arg1, arg2, arg3)
    } else {
        return make_tool_result("unknown tool: {tool_name}", true)
    }
}

def run_tool(tool_name, arg1, arg2, arg3, arg4 : string) : string {
    var result : string
    with_channel(1) <| $(ch) {
        new_thread() <| @() {
            let tool_result = dispatch_tool(tool_name, arg1, arg2, arg3, arg4)
            ch |> push_clone(ToolMessage(text = tool_result))
            ch |> notify_and_release()
        }
        for_each_clone(ch) <| $(msg : ToolMessage#) {
            result = clone_string(msg.text)
        }
    }
    return result
}

def handle_tools_call(id_json : string; params : JsonValue?) : string {
    let tool_name_val = params?.name
    let args = params?.arguments
    if (tool_name_val == null) {
        return json_rpc_error(id_json, -32602, "missing tool name")
    }
    let name = tool_name_val.value as _string
    var arg1, arg2, arg3, arg4 : string
    if (name == "compile_check" || name == "list_functions" || name == "list_types" || name == "run_test" || name == "format_file") {
        arg1 = get_string_arg(args, "file")
        if (empty(arg1)) {
            return json_rpc_error(id_json, -32602, "missing 'file' argument")
        }
    } elif (name == "run_script") {
        arg1 = get_string_arg(args, "file")
        arg2 = get_string_arg(args, "code")
    } elif (name == "ast_dump") {
        arg1 = get_string_arg(args, "expression")
        arg2 = get_string_arg(args, "file")
        arg3 = get_string_arg(args, "function")
        arg4 = get_string_arg(args, "mode")
    } elif (name == "find_symbol") {
        arg1 = get_string_arg(args, "query")
        if (empty(arg1)) {
            return json_rpc_error(id_json, -32602, "missing 'query' argument")
        }
        arg2 = get_string_arg(args, "kind")
        arg3 = get_string_arg(args, "file")
    } elif (name == "list_module_api") {
        arg1 = get_string_arg(args, "module")
        if (empty(arg1)) {
            return json_rpc_error(id_json, -32602, "missing 'module' argument")
        }
        arg2 = get_string_arg(args, "filter")
        arg3 = get_string_arg(args, "section")
    } elif (name == "list_modules") {
        // no args needed
    } else {
        return json_rpc_error(id_json, -32602, "unknown tool: {name}")
    }
    return json_rpc_response(id_json, run_tool(name, arg1, arg2, arg3, arg4))
}

// ── HTTP server ───────────────────────────────────────────────────────

def dispatch_jsonrpc(body : string) : string {
    var err : string
    var parsed = read_json(body, err)
    if (parsed == null) {
        return json_rpc_error("null", -32700, "parse error: {err}")
    }
    let id_json = json_value_to_string(parsed?.id)
    let method_val = parsed?.method
    let params_val = parsed?.params
    if (method_val == null || !(method_val.value is _string)) {
        unsafe { delete parsed; }
        return json_rpc_error(id_json, -32600, "invalid request: missing method")
    }
    let method = method_val.value as _string
    var result : string
    if (method == "initialize") {
        result = handle_initialize(id_json)
    } elif (method == "notifications/initialized" || method == "initialized") {
        unsafe { delete parsed; }
        return ""
    } elif (method == "tools/list") {
        result = handle_tools_list(id_json)
    } elif (method == "tools/call") {
        result = handle_tools_call(id_json, params_val)
    } elif (method == "ping") {
        result = json_rpc_response(id_json, "\{\}")
    } else {
        result = json_rpc_error(id_json, -32601, "method not found: {method}")
    }
    unsafe { delete parsed; }
    return result
}

class McpServer : HvWebServer {
    def override onInit {
        allow_cors()
        POST("/mcp") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = string(req.body)
            print("[MCP] POST /mcp: {body}\n")
            let response = dispatch_jsonrpc(body)
            if (empty(response)) {
                return http_status.NO_CONTENT
            }
            let accept = string(get_header(req, "accept"))
            if (find(accept, "text/event-stream") >= 0) {
                set_header(resp, "Content-Type", "text/event-stream")
                return resp |> TEXT_PLAIN("event: message\ndata: {response}\n\n")
            }
            set_header(resp, "Content-Type", "application/json")
            return resp |> TEXT_PLAIN(response)
        }
        GET("/mcp") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return http_status.METHOD_NOT_ALLOWED
        }
        GET("/health") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
    }
}

[export]
def main() {
    var port = MCP_PORT
    let args <- get_command_line_arguments()
    if (length(args) > 0) {
        let maybe_port = to_int(args[length(args) - 1])
        if (maybe_port > 0) {
            port = maybe_port
        }
    }
    print("Starting daslang MCP server on port {port}...\n")
    print("Configure Claude Code with: \"url\": \"http://localhost:{port}/mcp\"\n")
    var server = new McpServer()
    server->init(port)
    server->start()
    print("Server running. Press Ctrl+C to stop.\n")
    var log_ref_time = ref_time_ticks()
    while (true) {
        server->tick()
        sleep(10u)
        // periodic heap stats logging
        let elapsed = get_time_usec(log_ref_time) / 1000
        if (elapsed > int(HEAP_LOG_INTERVAL)) {
            log_ref_time = ref_time_ticks()
            let heap_used = heap_bytes_allocated()
            let str_used = string_heap_bytes_allocated()
            print("[MCP] heap: {int(heap_used)} bytes ({heap_depth()} depth), string heap: {int(str_used)} bytes ({string_heap_depth()} depth)\n")
        }
        // collect if over threshold
        let heap_used = heap_bytes_allocated()
        let str_used = string_heap_bytes_allocated()
        if (heap_used > HEAP_COLLECT_THRESHOLD || str_used > HEAP_COLLECT_THRESHOLD) {
            print("[MCP] collecting — heap: {int(heap_used)}, string heap: {int(str_used)}\n")
            unsafe {
                heap_collect(str_used > HEAP_COLLECT_THRESHOLD, false)
            }
            print("[MCP] after collect — heap: {int(heap_bytes_allocated())}, string heap: {int(string_heap_bytes_allocated())}\n")
        }
    }
}
