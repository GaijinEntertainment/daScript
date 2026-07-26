// dasherd MCP shim — stdio MCP server exposing the dasHerd coordination
// contract (mailbox, bundles, repositories) as typed tools, translating to
// the watcher's loopback HTTP API. Configuration comes from the environment
// the watcher injects at agent launch: DASHERD_URL, DASHERD_TOKEN,
// DASHERD_SESSION_ID, DASHERD_HERD_SESSION_ID.
//
// Run:   daslang.exe utils/dasHerd/mcp_main.das
// Config in .mcp.json:
//   "mcpServers": { "dasherd": { "command": "daslang.exe", "args": ["utils/dasHerd/mcp_main.das"] } }

options gen2
options persistent_heap
options gc
options multiple_contexts
options rtti
options no_unused_function_arguments = false

require ../mcp/protocol_core.das public
require dashv/dashv_boost
require daslib/json_boost
require daslib/strings_boost
require daslib/fio
require strings

def private herd_base_url : string {
    var url = get_env_variable("DASHERD_URL")
    if (empty(url)) {
        url = "http://127.0.0.1:9191"
    }
    while (ends_with(url, "/")) {
        url = slice(url, 0, length(url) - 1)
    }
    return url
}

def private herd_url(path : string) : string {
    let separator = find(path, "?") >= 0 ? "&" : "?"
    let token = get_env_variable("DASHERD_TOKEN")
    return "{herd_base_url()}{path}{separator}token={token}"
}

def private herd_http(post : bool; path, body : string) : string {
    let url = herd_url(path)
    var status = -1
    var text = ""
    if (post) {
        POST(url, body) $(resp) {
            if (resp != null) {
                status = int(resp.status_code)
                text = string(resp.body)
            }
        }
    } else {
        GET(url) $(resp) {
            if (resp != null) {
                status = int(resp.status_code)
                text = string(resp.body)
            }
        }
    }
    if (status < 0) {
        return make_tool_result("dasherd watcher unreachable at {herd_base_url()} — " +
            "is the watcher running, and are DASHERD_URL/DASHERD_TOKEN set?", true)
    }
    // Any 2xx is success — POST /api/v1/mailbox answers 201 Created, and the
    // strict ==200 check made outbox_send/outbox_reply fail on acceptance.
    if (status < 200 || status >= 300) {
        return make_tool_result("watcher returned HTTP {status}: {text}", true)
    }
    return make_tool_result(text)
}

def private herd_session_id : string => get_env_variable("DASHERD_SESSION_ID")

def private session_missing_result : string {
    return make_tool_result("DASHERD_SESSION_ID is not set — this tool only works " +
        "inside a dasHerd-managed agent session", true)
}

// Reject malformed inline JSON before splicing it into a request body.
def private valid_json(text : string) : bool {
    var parse_error : string
    var parsed = read_json(text, parse_error)
    let ok = parsed != null
    unsafe {
        delete parsed
    }
    return ok
}

let private EMPTY_FOCUS = ("\{\"repository_id\":\"\",\"worktree_path\":\"\"," +
    "\"comparison\":\"working\",\"revision\":\"\",\"summary\":\"\",\"targets\":[]\}")

def private do_whoami : string {
    let session = herd_session_id()
    let sender = empty(session) ? "" : "agent_session:{session}"
    let configured = !empty(get_env_variable("DASHERD_TOKEN")) && !empty(session)
    return make_tool_result(write_json(JV((sender = sender, session_id = session,
        url = herd_base_url(), herd_session_id = get_env_variable("DASHERD_HERD_SESSION_ID"),
        configured = configured))))
}

def private do_inbox(message_id : string) : string {
    let session = herd_session_id()
    if (empty(session)) return session_missing_result()
    var path = "/api/v1/mailbox?session_id={session}&direction=inbox"
    if (!empty(message_id)) {
        path += "&message_id={message_id}"
    }
    return herd_http(false, path, "")
}

def private do_inbox_state(message_id, status : string) : string {
    let session = herd_session_id()
    if (empty(session)) return session_missing_result()
    if (empty(message_id)) return make_tool_result("message_id is required", true)
    let body = write_json(JV((session_id = session, message_id = message_id,
        status = status)))
    return herd_http(true, "/api/v1/mailbox/state", body)
}

def private do_outbox(subject, reply_to, bundle_id, focus_json : string) : string {
    let session = herd_session_id()
    if (empty(session)) return session_missing_result()
    if (empty(strip(subject))) return make_tool_result("subject is required", true)
    var focus = EMPTY_FOCUS
    if (!empty(strip(focus_json))) {
        if (!valid_json(focus_json)) {
            return make_tool_result("focus_json is not valid JSON", true)
        }
        focus = focus_json
    }
    let sender = "agent_session:{session}"
    let body = ("\{\"session_id\":{write_json(JV(session))},\"direction\":\"outbox\"," +
        "\"sender\":{write_json(JV(sender))},\"kind\":\"focus_set\"," +
        "\"subject\":{write_json(JV(subject))},\"reply_to\":{write_json(JV(reply_to))}," +
        "\"bundle_id\":{write_json(JV(bundle_id))},\"notify\":false,\"focus\":{focus}\}")
    return herd_http(true, "/api/v1/mailbox", body)
}

def private do_bundle_list : string {
    let session = herd_session_id()
    if (empty(session)) return session_missing_result()
    return herd_http(false, "/api/v1/bundles?session_id={session}", "")
}

def private do_bundle_sync(bundle_json : string) : string {
    let session = herd_session_id()
    if (empty(session)) return session_missing_result()
    if (empty(strip(bundle_json))) return make_tool_result("bundle_json is required", true)
    if (!valid_json(bundle_json)) {
        return make_tool_result("bundle_json is not valid JSON", true)
    }
    let body = "\{\"session_id\":{write_json(JV(session))},\"bundle\":{bundle_json}\}"
    return herd_http(true, "/api/v1/bundles/sync", body)
}

def private do_repository_list : string {
    return herd_http(false, "/api/v1/repositories", "")
}

def private do_repository_add(path, display_name : string) : string {
    if (empty(strip(path))) return make_tool_result("path is required", true)
    let body = write_json(JV((path = get_full_file_name(path),
        display_name = display_name, target_id = "local",
        herd_session_id = get_env_variable("DASHERD_HERD_SESSION_ID"))))
    return herd_http(true, "/api/v1/repositories", body)
}

def private build_dasherd_tools(var reg : array<ToolDef>) {
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_whoami",
            "Identify this dasHerd session: sender, session id, watcher URL, and whether the environment is configured.",
            { "unused" => PropertySchema(_type = "string", description = "ignored") }, []),
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_whoami()))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_inbox_list",
            "List this session's inbox messages (human/system -> agent requests).",
            { "unused" => PropertySchema(_type = "string", description = "ignored") }, []),
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_inbox("")))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_inbox_get",
            "Fetch one inbox message by id, including its focus targets.",
            { "message_id" => PropertySchema(_type = "string", description = "Mailbox message id (e.g. hf_3)") }, ["message_id"]),
        required <- ["message_id"], arg_names <- ["message_id"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_inbox(arg1)))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_inbox_ack",
            "Acknowledge an inbox message (you have seen it and are working on it).",
            { "message_id" => PropertySchema(_type = "string", description = "Mailbox message id (e.g. hf_3)") }, ["message_id"]),
        required <- ["message_id"], arg_names <- ["message_id"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_inbox_state(arg1, "acknowledged")))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_inbox_complete",
            "Mark an inbox message completed (the request has been handled).",
            { "message_id" => PropertySchema(_type = "string", description = "Mailbox message id (e.g. hf_3)") }, ["message_id"]),
        required <- ["message_id"], arg_names <- ["message_id"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_inbox_state(arg1, "completed")))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_outbox_send",
            "Send a message to the human (agent -> human Attention). Optional focus_json is a WatcherFocusSet object (repository_id, worktree_path, comparison, revision, summary, targets with UTF-8 byte ranges); optional bundle_id links a Review Bundle.",
            {
                "subject" => PropertySchema(_type = "string", description = "Short free-text subject shown in the Attention panel"),
                "focus_json" => PropertySchema(_type = "string", description = "Optional inline JSON focus set"),
                "bundle_id" => PropertySchema(_type = "string", description = "Optional Review Bundle id this message belongs to")
            }, ["subject"]),
        required <- ["subject"], arg_names <- ["subject", "focus_json", "bundle_id"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_outbox(arg1, "", arg3, arg2)))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_outbox_reply",
            "Reply to an inbox message (agent -> human). Same payload as dasherd_outbox_send plus the inbox message id being answered.",
            {
                "reply_to" => PropertySchema(_type = "string", description = "Inbox message id being answered (e.g. hf_3)"),
                "subject" => PropertySchema(_type = "string", description = "Short free-text subject"),
                "focus_json" => PropertySchema(_type = "string", description = "Optional inline JSON focus set"),
                "bundle_id" => PropertySchema(_type = "string", description = "Optional Review Bundle id")
            }, ["reply_to", "subject"]),
        required <- ["reply_to", "subject"], arg_names <- ["reply_to", "subject", "focus_json", "bundle_id"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_outbox(arg2, arg1, arg4, arg3)))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_bundle_list",
            "List this session's declared Review Bundles.",
            { "unused" => PropertySchema(_type = "string", description = "ignored") }, []),
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_bundle_list()))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_bundle_sync",
            "Declaratively sync this session's Review Bundle from an inline JSON manifest (id, title, status, summary, participants with roles and files). The watcher projects it into the human's Attention surface.",
            { "bundle_json" => PropertySchema(_type = "string", description = "Inline JSON bundle manifest") },
            ["bundle_json"]),
        required <- ["bundle_json"], arg_names <- ["bundle_json"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_bundle_sync(arg1)))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_repository_list",
            "List repositories the watcher observes.",
            { "unused" => PropertySchema(_type = "string", description = "ignored") }, []),
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_repository_list()))
    reg |> emplace(ToolDef(
        tool <- make_tool("dasherd_repository_add",
            "Register a repository checkout or worktree with the watcher and announce it as part of this session's workspace.",
            {
                "path" => PropertySchema(_type = "string", description = "Checkout or worktree path"),
                "display_name" => PropertySchema(_type = "string", description = "Optional display name")
            }, ["path"]),
        required <- ["path"], arg_names <- ["path", "display_name"],
        main_thread = true,
        handler = @@(arg1, arg2, arg3, arg4, arg5, arg6, project, project_root : string; load_modules : array<string>) => do_repository_add(arg1, arg2)))
}

[export]
def main() {
    logger_init("dasherd-mcp")
    logger_info("mcp", "Starting dasherd MCP shim (stdio transport, watcher at {herd_base_url()})")
    var reg : array<ToolDef>
    build_dasherd_tools(reg)
    register_shutdown(reg)
    serve_stdio(das_mcp_server("dasherd", "0.1.0"), reg)
}
