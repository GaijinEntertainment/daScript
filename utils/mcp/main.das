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
require strings

require tools/compile_check public
require tools/list_functions public
require tools/list_types public
require tools/run_test public
require tools/list_module_api public
require tools/list_modules public
require tools/find_symbol public

let MCP_PORT = 9500

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

def handle_tools_call(id_json : string; params : JsonValue?) : string {
    let tool_name = params?.name
    let args = params?.arguments
    if (tool_name == null) {
        return json_rpc_error(id_json, -32602, "missing tool name")
    }
    let name = tool_name.value as _string
    if (name == "compile_check") {
        let file_arg = args?.file
        if (file_arg == null || !(file_arg.value is _string)) {
            return json_rpc_error(id_json, -32602, "missing 'file' argument")
        }
        return json_rpc_response(id_json, do_compile_check(file_arg.value as _string))
    } elif (name == "list_functions") {
        let file_arg = args?.file
        if (file_arg == null || !(file_arg.value is _string)) {
            return json_rpc_error(id_json, -32602, "missing 'file' argument")
        }
        return json_rpc_response(id_json, do_list_functions(file_arg.value as _string))
    } elif (name == "list_types") {
        let file_arg = args?.file
        if (file_arg == null || !(file_arg.value is _string)) {
            return json_rpc_error(id_json, -32602, "missing 'file' argument")
        }
        return json_rpc_response(id_json, do_list_types(file_arg.value as _string))
    } elif (name == "run_test") {
        let file_arg = args?.file
        if (file_arg == null || !(file_arg.value is _string)) {
            return json_rpc_error(id_json, -32602, "missing 'file' argument")
        }
        return json_rpc_response(id_json, do_run_test(file_arg.value as _string))
    } elif (name == "list_modules") {
        return json_rpc_response(id_json, do_list_modules())
    } elif (name == "find_symbol") {
        let query_arg = (args != null && args.value is _object) ? (args.value as _object)?["query"] ?? null : null
        if (query_arg == null || !(query_arg.value is _string)) {
            return json_rpc_error(id_json, -32602, "missing 'query' argument")
        }
        let kind_arg = (args != null && args.value is _object) ? (args.value as _object)?["kind"] ?? null : null
        let file_arg = (args != null && args.value is _object) ? (args.value as _object)?["file"] ?? null : null
        let kind_str = kind_arg != null && kind_arg.value is _string ? kind_arg.value as _string : ""
        let file_str = file_arg != null && file_arg.value is _string ? file_arg.value as _string : ""
        return json_rpc_response(id_json, do_find_symbol(query_arg.value as _string, kind_str, file_str))
    } elif (name == "list_module_api") {
        let mod_arg = (args != null && args.value is _object) ? (args.value as _object)?["module"] ?? null : null
        if (mod_arg == null || !(mod_arg.value is _string)) {
            return json_rpc_error(id_json, -32602, "missing 'module' argument")
        }
        let filter_arg = (args != null && args.value is _object) ? (args.value as _object)?["filter"] ?? null : null
        let section_arg = (args != null && args.value is _object) ? (args.value as _object)?["section"] ?? null : null
        let filter_str = filter_arg != null && filter_arg.value is _string ? filter_arg.value as _string : ""
        let section_str = section_arg != null && section_arg.value is _string ? section_arg.value as _string : ""
        return json_rpc_response(id_json, do_list_module_api(mod_arg.value as _string, filter_str, section_str))
    } else {
        return json_rpc_error(id_json, -32602, "unknown tool: {name}")
    }
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
    while (true) {
        server->tick()
        sleep(10u)
    }
}
