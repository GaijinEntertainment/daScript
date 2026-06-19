// Shared MCP protocol core for the daslang MCP server.
//
// Generic JSON-RPC framing + the data-driven tool registry (array<ToolDef>):
// each entry carries its advertised schema, validation rules, the JSON arg
// names mapped positionally to arg1..arg6, and a uniform handler function
// pointer. handle_tools_list / handle_tools_call iterate the registry passed
// in by the entry point, so main.das (full set) and cpp_main.das (cpp/agnostic
// subset) reuse this dispatch over a different registry. The registrations
// themselves live in registry_das.das / registry_cpp.das.

options gen2
options rtti

module protocol_core

require tools/common public
require daslib/json
require daslib/json_boost public
require daslib/jsonrpc
require daslib/jobque_boost
require strings
require daslib/fio
require daslib/logger public

let HEAP_COLLECT_THRESHOLD = 1024ul * 1024ul  // 1 MB
let MCP_LOG_NAME = "mcp_server"

// ── Logging ───────────────────────────────────────────────────────────
//
// Thin wrappers around daslib/logger. Categories are dotted so consumers
// can filter (e.g. `logger_set_category_level("mcp.tool", LOG_DEBUG)`).
//
// CRITICAL: MCP uses stdio JSON-RPC. ANY write to the parent process's
// stdout corrupts protocol framing. Every diagnostic must go to the log
// file (or stderr fallback) — never stdout. `logger_install_hook()` in
// main() enforces this for accidental print() / to_log() calls — a
// single global install covers main + every new_thread worker.

var ast_grep_available : bool = false

def log_request(method, body : string) {
    let truncated = length(body) > 500 ? slice(body, 0, 500) + "..." : body
    logger_info("mcp.req", "{method} {truncated}")
}

def log_response(method : string; elapsed_sec : double) {
    let ms = int(elapsed_sec * 1000.0lf)
    logger_info("mcp.res", "{method} ({ms}ms)")
}

def log_tool(tool_name : string; args : string) {
    logger_info("mcp.tool", "{tool_name} {args}")
}

def log_tool_done(tool_name : string; elapsed_sec : double; is_error : bool) {
    let ms = int(elapsed_sec * 1000.0lf)
    let status = is_error ? "ERR" : "OK"
    if (is_error) {
        logger_warning("mcp.tool", "{tool_name} {status} ({ms}ms)")
    } else {
        logger_info("mcp.tool", "{tool_name} {status} ({ms}ms)")
    }
}

def log_error(msg : string) {
    logger_error("mcp", msg)
}

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

// ── MCP protocol handlers ─────────────────────────────────────────────
// JSON-RPC envelope helpers (response, error, serialize_id) live in daslib/jsonrpc.

def handle_initialize(id_json : string) : string {
    let result = InitializeResult(
        protocolVersion = "2025-11-25",
        capabilities = Capabilities(),
        serverInfo = ServerInfo(name = "daslang", version = "0.1.0")
    )
    return jsonrpc::response(id_json, sprint_json(result, false))
}

// Declared dependency-first (PropertySchema -> InputSchema -> MakeFileTool):
// the AOT C++ emitter writes structs in declaration order, and a by-value
// member needs its type complete first, so MakeFileTool (which holds an
// InputSchema by value) must come after InputSchema, which must come after
// PropertySchema (held by value inside its `properties` table).
struct PropertySchema {
    @rename = "type" _type : string
    description : string
    @optional items : PropertySchema?
}

struct InputSchema {
    @rename = "type" _type : string
    properties : table<string; PropertySchema>
    required : array<string>
}

struct MakeFileTool {
    name : string
    description : string
    inputSchema : InputSchema
}

let PROJECT_PROP = PropertySchema(_type = "string", description = "Path to a .das_project file for custom module resolution")
let PROJECT_ROOT_PROP = PropertySchema(_type = "string", description = "Project root directory (parent of modules/) for daspkg-style module resolution; equivalent to daslang's -project_root CLI flag. Use when working on an external module via a <DummyRoot>/modules/<your-module> junction.")
let LOAD_MODULES_PROP = PropertySchema(
    _type = "array",
    description = "List of individual module folders (each containing .das_module) to load directly, equivalent to daslang's -load_module CLI flag (repeatable). Each path is the module's own folder — no <project_root>/modules/<name> wrapper. Path basenames shadow same-named entries in dasroot and project_root. Use to work on an external module without the junction trick.",
    items = new PropertySchema(_type = "string", description = "Path to a module folder (the one containing .das_module). Absolute paths are recommended; live_launch absolutizes them before spawning daslang-live so its -cwd flag doesn't shift the base, but other tools forward verbatim.")
)

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
                ),
                "project" => PROJECT_PROP,
                "project_root" => PROJECT_ROOT_PROP,
                "load_modules" => LOAD_MODULES_PROP
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

// ── Tool registry ────────────────────────────────────────────────────
//
// A ToolHandler maps the extracted positional args + project plumbing to a
// `do_*` call; it's a no-capture function pointer so it copies freely across
// the new_thread boundary in run_tool (same program -> same function table).

typedef ToolHandler = function<(arg1 : string; arg2 : string; arg3 : string; arg4 : string; arg5 : string; arg6 : string; project : string; project_root : string; load_modules : array<string>) : string>

struct ToolDef {
    tool : MakeFileTool                 // advertised name + description + inputSchema
    required : array<string>            // every name here must be non-empty, else -32602
    any_required : array<string>        // at least one must be non-empty, else -32602 (empty = no constraint)
    arg_names : array<string>           // JSON arg names mapped positionally to arg1..arg6
    needs_ast_grep : bool               // only advertised when ast-grep is available
    main_thread : bool                  // run on the main thread (live_* / shutdown), not a worker
    handler : ToolHandler
}

def find_tool(reg : array<ToolDef>; name : string) : int {
    for (i in range(length(reg))) {
        if (reg[i].tool.name == name) return i
    }
    return -1
}

// shutdown is shared by every entry point (main + cpp_main). It runs on the
// main thread and exits the process before the response is written.
def register_shutdown(var reg : array<ToolDef>) {
    reg |> emplace(ToolDef(
        tool <- make_tool(
            "shutdown",
            "Shut down the MCP server. Claude Code will auto-restart it, picking up any code changes to MCP tools. Tool registration changes still require a manual restart.",
            { "unused" => PropertySchema(_type = "string", description = "ignored") },
            []),
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) : string {
            unsafe { exit(0); }
            return make_tool_result("shutting down")
        }))
}

def handle_tools_list(reg : array<ToolDef>; id_json : string) : string {
    // Serialize each MakeFileTool in place rather than cloning into a
    // ToolsListResult — PropertySchema carries a pointer field (`items`) that
    // can't be cloned from const, and the list is static so a fresh build per
    // (rare) call is fine.
    let json = build_string() $(var w) {
        w |> write("\{\"tools\":[")
        var first = true
        for (td in reg) {
            if (td.needs_ast_grep && !ast_grep_available) continue
            if (!first) w |> write(",")
            first = false
            w |> write(sprint_json(td.tool, false))
        }
        w |> write("]\}")
    }
    return jsonrpc::response(id_json, json)
}

// ── Tool dispatch (runs in cloned context via new_thread) ────────────

struct ToolMessage {
    text : string
}

def get_string_arg(args : JsonValue?; name : string) : string {
    if (args == null || !(args.value is _object)) return ""
    let v = (args.value as _object)?[name] ?? null
    if (v == null) return ""
    if (v.value is _string) return v.value as _string
    if (v.value is _bool) return v.value as _bool ? "true" : "false"
    return ""
}

def get_string_array_arg(args : JsonValue?; name : string) : array<string> {
    var result : array<string>
    if (args == null || !(args.value is _object)) return <- result
    let v = (args.value as _object)?[name] ?? null
    if (v == null || !(v.value is _array)) return <- result
    result |> reserve(length(v.value as _array))
    // Filter out non-string and empty-string elements so callers don't have
    // to re-validate. Empty path entries would otherwise resolve to das_root
    // downstream (resolve_path("") -> get_das_root()) and load it as a module.
    for (elem in v.value as _array) {
        if (elem != null && elem.value is _string) {
            let s = elem.value as _string
            if (!empty(s)) {
                result |> push(s)
            }
        }
    }
    return <- result
}

def run_tool(handler : ToolHandler; tool_name : string; main_thread : bool;
             arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string;
             var load_modules : array<string>) : string {
    log_tool(tool_name, "{arg1} {arg2} {arg3} {arg4} {arg5} {arg6}")
    let t0 = get_clock()
    var result : string
    // Live tools (and shutdown) run on the main thread — they use system() and
    // sleep() which don't work well from new_thread, and they don't need
    // compilation context.
    if (main_thread) {
        result = invoke(handler, arg1, arg2, arg3, arg4, arg5, arg6, project, project_root, load_modules)
    } else {
        with_channel(1) $(ch) {
            new_thread() <| @ capture(<- load_modules) () {
                // logger_install_hook in main() registered a GLOBAL debug
                // agent — worker contexts inherit it automatically, no
                // re-install needed here. Worker print()/to_log() routes
                // through the agent's owning context (main) into the
                // shared log file.
                let tool_result = invoke(handler, arg1, arg2, arg3, arg4, arg5, arg6, project, project_root, load_modules)
                ch |> push_clone(ToolMessage(text = tool_result))
                ch |> notify_and_release()
            }
            for_each_clone(ch) $(msg : ToolMessage#) {
                result = clone_string(msg.text)
            }
        }
    }
    let is_error = find(result, "\"isError\":true") >= 0
    log_tool_done(tool_name, get_clock() - t0, is_error)
    return result
}

def handle_tools_call(reg : array<ToolDef>; id_json : string; params : JsonValue?) : string {
    let tool_name_val = params?.name
    let args = params?.arguments
    if (tool_name_val == null || !(tool_name_val.value is _string)) return jsonrpc::error(id_json, -32602, "missing or non-string tool name")
    let name = tool_name_val.value as _string
    let idx = find_tool(reg, name)
    if (idx < 0) return jsonrpc::error(id_json, -32602, "unknown tool: {name}")
    // ast-grep tools are hidden from tools/list when ast-grep is missing; reject
    // a direct call too, rather than letting do_* build a command off an empty path.
    if (reg[idx].needs_ast_grep && !ast_grep_available) return jsonrpc::error(id_json, -32602, "tool '{name}' requires ast-grep (sg), which was not found on PATH")
    // AND-required: every listed arg must be present.
    for (rn in reg[idx].required) {
        if (empty(get_string_arg(args, rn))) return jsonrpc::error(id_json, -32602, "missing '{rn}' argument")
    }
    // OR-required: at least one of the listed args must be present.
    if (!empty(reg[idx].any_required)) {
        var has_any = false
        for (an in reg[idx].any_required) {
            if (!empty(get_string_arg(args, an))) {
                has_any = true
                break
            }
        }
        if (!has_any) return jsonrpc::error(id_json, -32602, "missing input — provide one of: {join(reg[idx].any_required, ", ")}")
    }
    let cnt = length(reg[idx].arg_names)
    let arg1 = cnt > 0 ? get_string_arg(args, reg[idx].arg_names[0]) : ""
    let arg2 = cnt > 1 ? get_string_arg(args, reg[idx].arg_names[1]) : ""
    let arg3 = cnt > 2 ? get_string_arg(args, reg[idx].arg_names[2]) : ""
    let arg4 = cnt > 3 ? get_string_arg(args, reg[idx].arg_names[3]) : ""
    let arg5 = cnt > 4 ? get_string_arg(args, reg[idx].arg_names[4]) : ""
    let arg6 = cnt > 5 ? get_string_arg(args, reg[idx].arg_names[5]) : ""
    let handler = reg[idx].handler
    let main_thread = reg[idx].main_thread
    let project = get_string_arg(args, "project")
    let project_root = get_string_arg(args, "project_root")
    var load_modules <- get_string_array_arg(args, "load_modules")
    return jsonrpc::response(id_json, run_tool(handler, name, main_thread, arg1, arg2, arg3, arg4, arg5, arg6, project, project_root, load_modules))
}

// ── JSON-RPC dispatcher ──────────────────────────────────────────────

def dispatch_one(reg : array<ToolDef>; req : jsonrpc::ParsedRequest; body : string) : string {
    // Malformed entry — parser already built the envelope. Notifications skip the wire.
    if (!empty(req.error_envelope)) {
        if (!req.is_notification) log_error(req.error_envelope)
        return req.is_notification ? "" : req.error_envelope
    }
    let id_json = req.id_str
    let method = req.method
    log_request(method, body)
    let t0 = get_clock()
    var result : string
    if (method == "initialize") {
        result = handle_initialize(id_json)
    } elif (method == "notifications/initialized" || method == "initialized") {
        return ""
    } elif (method == "tools/list") {
        result = handle_tools_list(reg, id_json)
    } elif (method == "tools/call") {
        result = handle_tools_call(reg, id_json, req.params)
    } elif (method == "ping") {
        result = jsonrpc::response(id_json, "\{\}")
    } else {
        log_error("method not found: {method}")
        result = jsonrpc::error(id_json, jsonrpc::METHOD_NOT_FOUND, "method not found: {method}")
    }
    log_response(method, get_clock() - t0)
    return req.is_notification ? "" : result
}

def dispatch_jsonrpc(reg : array<ToolDef>; body : string) : string {
    let pb = jsonrpc::parse_batch(body)
    if (!empty(pb.framing_error)) {
        log_error(pb.framing_error)
        return pb.framing_error
    }
    if (!pb.is_batch) return dispatch_one(reg, pb.requests[0], body)
    // §6 batch: collect non-empty per-entry responses, wrap as JSON array; "" if all-notifications.
    var parts : array<string>
    parts |> reserve(length(pb.requests))
    for (req in pb.requests) {
        let r = dispatch_one(reg, req, body)
        if (!empty(r)) parts |> push(r)
    }
    if (empty(parts)) return ""
    return build_string() $(var w) {
        write_char(w, '[')
        var first = true
        for (p in parts) {
            if (!first) write_char(w, ',')
            first = false
            write(w, p)
        }
        write_char(w, ']')
    }
}

// ── stdio server loop ────────────────────────────────────────────────
//
// Shared by every entry point: read newline-delimited JSON-RPC from stdin,
// dispatch against `reg`, write one-line responses to stdout. The entry point
// owns logger_init + ast-grep detection + registry assembly, then calls this.

def serve_stdio(reg : array<ToolDef>) {
    let sin = fstdin()
    let sout = fstdout()
    while (!feof(sin)) {
        // read one line (up to 16KB per fgets call; loop for longer messages)
        let msg = build_string() $(var w) {
            while (!feof(sin)) {
                let chunk = fgets(sin)
                let len = length(chunk)
                if (len == 0) break
                if (character_at(chunk, len - 1) == '\n') { // nolint:PERF003
                    // got full line — append without the trailing newline
                    if (len > 1 && character_at(chunk, len - 2) == '\r') { // nolint:PERF003
                        write(w, slice(chunk, 0, len - 2))
                    } else {
                        write(w, slice(chunk, 0, len - 1))
                    }
                    break
                } else {
                    // buffer full, line continues — append and keep reading
                    write(w, chunk)
                }
            }
        }

        if (empty(msg)) continue

        var response = dispatch_jsonrpc(reg, msg)
        if (!empty(response)) {
            // A JSON-RPC frame is one line; the trailing "\n" below is the only
            // delimiter. Embedded newlines mean stray output (a subtool GC-leak
            // dump, a panic backtrace, a stray print) leaked into the payload and
            // would desync the client's line framing for the rest of the session.
            // Collapse to a single line so only this one request fails, not the
            // stream, and log it loudly so the leak is diagnosable.
            if (find(response, "\n") >= 0) {
                logger_error("mcp.frame", "response contains embedded newline(s) — stray output leaked into JSON-RPC frame; collapsing to keep the stream aligned. preview: {slice(response, 0, 240)}")
                response = replace(response, "\n", "\\n")
            }
            fprint(sout, response)
            fprint(sout, "\n")
            fflush(sout)
        }

        // collect heap after each request if over threshold
        let heap_used = heap_bytes_allocated()
        let str_used = string_heap_bytes_allocated()
        if (heap_used > HEAP_COLLECT_THRESHOLD || str_used > HEAP_COLLECT_THRESHOLD) {
            logger_info("mcp.heap", "collecting — heap: {int(heap_used)}, string heap: {int(str_used)}")
            unsafe {
                heap_collect(str_used > HEAP_COLLECT_THRESHOLD, false)
            }
        }
    }
    logger_info("mcp", "stdin closed, shutting down")
}
