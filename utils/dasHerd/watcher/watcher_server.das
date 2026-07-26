options gen2
options rtti
options persistent_heap
options gc
options indenting = 4
options no_unused_function_arguments = false
options no_unused_block_arguments = false

module watcher_server shared public

require dashv/dashv_boost public
require ./watcher_core.das public
require ./repository_core.das public
require ./herd_sessions.das public
require ./file_inspection.das
require daslib/json_boost
require daslib/base64
require strings

let private MAX_REQUEST_BODY_BYTES = 1024 * 1024
let private CONTROLLER_HEARTBEAT_TIMEOUT_MS = 5000l
var private g_watcher_gc_requested = false

def public watcher_request_gc() : bool {
    let coalesced = g_watcher_gc_requested
    g_watcher_gc_requested = true
    return coalesced
}

def public watcher_take_gc_request() : bool {
    let requested = g_watcher_gc_requested
    g_watcher_gc_requested = false
    return requested
}

def public watcher_reset_gc_request() {
    g_watcher_gc_requested = false
}

struct private WatcherWsMessage {
    op : string
    session_id : string
    command_line : string
    argv : array<string>
    cwd : string
    display_name : string
    kind : string
    target_id : string
    repository_id : string
    repository_path : string
    worktree_path : string
    new_worktree_path : string
    branch : string
    base_ref : string
    purpose : string
    task_context : string
    file_path : string
    comparison : string
    revision : string
    action : string
    diff_context : string
    request_id : int
    text : string
    columns : int
    rows : int
    timeout_ms : int64
    control : bool
    raw : bool
    direction : string
    sender : string = ""
    subject : string
    reply_to : string
    bundle_id : string
    message_id : string
    status : string
    notify : bool = true
    name : string
    profile_id : string
    workflow : string
    herd_session_id : string
    focus : WatcherFocusSet = WatcherFocusSet()
    bundle : WatcherReviewBundle = WatcherReviewBundle()
}

struct public RepositoryFileInspectionMessage {
    @rename = "type"
    _type : string = "file_inspection"
    request_id : int
    repository_id : string
    worktree_path : string
    file_path : string
    comparison : string
    revision : string
    diff_context : string
    task_session_id : string
    loading : bool
    patch : string
    view_text : string
    view_base64 : string
    view_byte_count : int
    view_available : bool
    status : string
    error : string
}

struct public RepositoryGitFileActionMessage {
    @rename = "type"
    _type : string = "git_file_action"
    request_id : int
    repository_id : string
    worktree_path : string
    file_path : string
    action : string
    task_session_id : string
    loading : bool
    status : string
    error : string
}

struct public RepositoryWorktreeFilesMessage {
    @rename = "type"
    _type : string = "worktree_files"
    request_id : int
    repository_id : string
    worktree_path : string
    loading : bool
    listing : string
    error : string
}

struct private WatcherTerminalCheckpointMessage {
    @rename = "type"
    _type : string = "terminal_checkpoint"
    offset : int64
    columns : int
    rows : int
    ansi : string
}

struct private OrphanedGitFileAction {
    session_id : string
    repository_id : string
    worktree_path : string
    file_path : string
    action : string
}

struct private PendingAgentWorktreeLaunch {
    create_session_id : string
    client_id : int
    repository_id : string
    source_worktree_path : string
    new_worktree_path : string
    branch : string
    task_context : string
    profile_id : string
    name : string
    workflow : string
}

class private WatcherClient {
    @safe_when_uninitialized
    channel : WebSocketChannel = default<WebSocketChannel>
    id : int
    session_id : string
    controls : bool
    last_heartbeat : int64
    last_update_key : string
    raw : bool
    raw_offset : int64
    repository_revision : int64 = -1l
    herd_revision : int64 = -1l
    mailbox_revision : int64 = -1l
    bundles_revision : int64 = -1l
    inspection_request_id : int
    inspection_repository_id : string
    inspection_worktree_path : string
    inspection_file_path : string
    inspection_comparison : string
    inspection_revision : string
    inspection_diff_context : string
    inspection_session_id : string
    inspection_expected_exit_code : int64
    inspection_delivered : bool = true
    inspection_phase : string
    inspection_patch : string
    inspection_artifact_path : string
    action_request_id : int
    action_repository_id : string
    action_worktree_path : string
    action_file_path : string
    action : string
    action_session_id : string
    action_delivered : bool = true
    files_request_id : int
    files_session_id : string
    files_artifact_path : string
    files_repository_id : string
    files_worktree_path : string
    files_delivered : bool = true
}

struct private WatcherOk {
    ok : bool = true
}

struct private WatcherError {
    ok : bool
    error : string
}

def private json_error(message : string) : string {
    return sprint_json(WatcherError(error = message), false)
}

def private hex_nibble(value : int) : int {
    if (is_number(value)) return value - '0'
    if (value >= 'a' && value <= 'f') return value - 'a' + 10
    if (value >= 'A' && value <= 'F') return value - 'A' + 10
    return -1
}

def private percent_decode(value : string) : string {
    var inscope decoded : array<uint8>
    decoded |> reserve(length(value))
    peek_data(value) $(data) {
        var index = 0
        while (index < length(value)) {
            let byte = int(data[index])
            if (byte == '%' && index + 2 < length(value)) {
                let high = hex_nibble(int(data[index + 1]))
                let low = hex_nibble(int(data[index + 2]))
                if (high >= 0 && low >= 0) {
                    decoded |> push(uint8((high << 4) | low))
                    index += 3
                    continue
                }
            }
            decoded |> push(uint8(byte == '+' ? ' ' : byte))
            index++
        }
    }
    return string(decoded)
}

def private query_value(url, key : string) : string {
    let query = find(url, "?")
    if (query < 0) return ""
    let wanted = key + "="
    let url_length = length(url)
    var position = query + 1
    while (position < url_length) {
        let amp = find(url, "&", position)
        let end = amp < 0 ? url_length : amp
        if (starts_with(slice(url, position, end), wanted)) {
            return percent_decode(slice(url, position + length(wanted), end))
        }
        if (amp < 0) break
        position = amp + 1
    }
    return ""
}

def private query_value_raw(url, key : string) : string {
    let query = find(url, "?")
    if (query < 0) return ""
    let wanted = key + "="
    let url_length = length(url)
    var position = query + 1
    while (position < url_length) {
        let amp = find(url, "&", position)
        let end = amp < 0 ? url_length : amp
        if (starts_with(slice(url, position, end), wanted)) {
            return slice(url, position + length(wanted), end)
        }
        if (amp < 0) break
        position = amp + 1
    }
    return ""
}

class DasHerdWatcherServer : HvWebServer {
    token : string
    agent_url : string
    agent_skill_path : string
    control_html : string
    xterm_js : string
    xterm_css : string
    xterm_fit_js : string
    xterm_unicode_js : string
    xterm_web_links_js : string
    clients : array<WatcherClient?>
    pending_inspection_artifacts : array<string>
    orphaned_git_actions : array<OrphanedGitFileAction>
    pending_agent_launches : array<PendingAgentWorktreeLaunch>
    next_client_id : int

    def authorized(url : string) : bool {
        if (empty(token)) return false
        // The browser panel sends the token percent-ENCODED (URLSearchParams
        // decodes, encodeURIComponent re-encodes); the native clients and the
        // printed control URL splice it RAW. A token containing '%' or '+'
        // satisfies only one spelling — accept both interpretations.
        return (query_value(url, "token") == token
            || query_value_raw(url, "token") == token)
    }

    def respond_error(var resp : HttpResponse?; status : http_status; message : string) : http_status {
        return resp |> JSON(json_error(message), status)
    }

    def require_auth(var req : HttpRequest?; var resp : HttpResponse?) : bool {
        if (req != null && authorized(string(req.url))) return true
        let _ = respond_error(resp, http_status.UNAUTHORIZED, "invalid watcher token")
        return false
    }

    def maybe_notify_inbox_message(session_id, direction, message_id : string;
                                   notify : bool) {
        if (direction != "inbox" || !notify) return
        let nudge_error = watcher_notify(session_id,
            "dasHerd review request {message_id} is ready. " +
            "Use the dasHerd skill to fetch and acknowledge it.\r",
            message_id)
        if (!empty(nudge_error) && nudge_error != "queued") {
            print("dasHerd watcher: mailbox {message_id} persisted; " +
                "TTY nudge failed: {nudge_error}\n")
        }
        if (nudge_error != "queued") {
            let _ = watcher_mailbox_set_notification(
                session_id, message_id, nudge_error)
        }
    }

    def override onInit {
        GET("/") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> SERVE_FILE(self.control_html)
        }
        GET("/vendor/xterm.js") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> SERVE_FILE(self.xterm_js)
        }
        GET("/vendor/xterm.css") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> SERVE_FILE(self.xterm_css)
        }
        GET("/vendor/addon-fit.js") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> SERVE_FILE(self.xterm_fit_js)
        }
        GET("/vendor/addon-unicode11.js") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> SERVE_FILE(self.xterm_unicode_js)
        }
        GET("/vendor/addon-web-links.js") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> SERVE_FILE(self.xterm_web_links_js)
        }
        GET("/api/v1/health") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> JSON("\{\"ok\":true,\"service\":\"dasHerd-watcher\"\}", http_status.OK)
        }
        GET("/api/v1/sessions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> JSON(watcher_list_json(), http_status.OK)
        }
        GET("/api/v1/repositories") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> JSON(repository_list_json(), http_status.OK)
        }
        GET("/api/v1/herd_sessions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            return resp |> JSON("\{\"revision\":{herd_revision()}," +
                "\"profiles\":{herd_profiles_json()},\"data\":{herd_sessions_json()}\}",
                http_status.OK)
        }
        GET("/api/v1/mailbox") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            let url = req != null ? string(req.url) : ""
            let session_id = query_value(url, "session_id")
            if (empty(session_id)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "session_id is required")
            }
            if (watcher_find_session(session_id) < 0) {
                return self.respond_error(resp, http_status.NOT_FOUND, "unknown session")
            }
            let direction = query_value(url, "direction")
            if (!empty(direction) && direction != "inbox" && direction != "outbox") {
                return self.respond_error(resp, http_status.BAD_REQUEST,
                    "direction must be inbox or outbox")
            }
            let message_id = query_value(url, "message_id")
            if (direction == "inbox" && !empty(message_id)) {
                let fetched = watcher_mailbox_mark_fetched(session_id, message_id)
                if (!fetched.ok) {
                    return self.respond_error(resp, http_status.NOT_FOUND, fetched.error)
                }
            }
            return resp |> JSON(watcher_mailbox_json(session_id,
                direction, message_id), http_status.OK)
        }
        GET("/api/v1/bundles") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            let url = req != null ? string(req.url) : ""
            let session_id = query_value(url, "session_id")
            if (empty(session_id)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "session_id is required")
            }
            if (watcher_find_session(session_id) < 0) {
                return self.respond_error(resp, http_status.NOT_FOUND, "unknown session")
            }
            return resp |> JSON(watcher_bundles_json(session_id), http_status.OK)
        }
        POST("/api/v1/gc") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            let coalesced = watcher_request_gc()
            return resp |> JSON("\{\"ok\":true,\"scheduled\":true,\"coalesced\":{coalesced}\}",
                http_status.ACCEPTED)
        }
        POST("/api/v1/sessions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var inscope launch = WatcherLaunchRequest()
            if (!sscan_json(string(req.body), launch)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid launch JSON")
            }
            if (watcher_kind_is_agent(launch.kind)) {
                launch.agent_url = self.agent_url
                launch.agent_token = self.token
                launch.agent_skill_path = self.agent_skill_path
            }
            let result = watcher_launch(launch)
            return resp |> JSON(sprint_json(result, false),
                result.ok ? http_status.CREATED : http_status.BAD_REQUEST)
        }
        POST("/api/v1/repositories") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var request = RepositoryAddRequest()
            if (!sscan_json(string(req.body), request)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid repository JSON")
            }
            let error = repository_add(request)
            // An announce for an already-registered repository is a success:
            // the point is attaching it to the session's workspace.
            let announced = !empty(request.herd_session_id)
            if (!empty(error) && !(announced && error == "repository is already registered")) {
                return self.respond_error(resp, http_status.CONFLICT, error)
            }
            if (announced) {
                let satellite_error = herd_add_satellite(request.herd_session_id,
                    repository_id_for_path(request.path),
                    get_full_file_name(request.path), "agent")
                if (!empty(satellite_error)) {
                    return self.respond_error(resp, http_status.CONFLICT, satellite_error)
                }
            }
            return resp |> JSON(sprint_json(WatcherOk(), false), http_status.CREATED)
        }
        POST("/api/v1/repositories/refresh") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var request = RepositoryRefreshRequest()
            if (!sscan_json(string(req.body), request)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid refresh JSON")
            }
            let error = repository_refresh(request.repository_id)
            if (!empty(error)) return self.respond_error(resp, http_status.CONFLICT, error)
            return resp |> JSON(sprint_json(WatcherOk(), false), http_status.ACCEPTED)
        }
        POST("/api/v1/input") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var input : WatcherInputRequest
            if (!sscan_json(string(req.body), input)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid input JSON")
            }
            let error = watcher_input(input.session_id, input.text)
            if (!empty(error)) return self.respond_error(resp, http_status.CONFLICT, error)
            return resp |> JSON(sprint_json(WatcherOk(), false), http_status.OK)
        }
        POST("/api/v1/resize") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var resize : WatcherResizeRequest
            if (!sscan_json(string(req.body), resize)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid resize JSON")
            }
            let error = watcher_resize(resize.session_id, resize.columns, resize.rows, 0)
            if (!empty(error)) return self.respond_error(resp, http_status.CONFLICT, error)
            return resp |> JSON(sprint_json(WatcherOk(), false), http_status.OK)
        }
        POST("/api/v1/terminate") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var request : WatcherSessionRequest
            if (!sscan_json(string(req.body), request)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid terminate JSON")
            }
            let error = watcher_terminate(request.session_id)
            if (!empty(error)) return self.respond_error(resp, http_status.CONFLICT, error)
            return resp |> JSON(sprint_json(WatcherOk(), false), http_status.OK)
        }
        POST("/api/v1/mailbox") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var request = WatcherMailboxPostRequest()
            if (!sscan_json(string(req.body), request)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid mailbox JSON")
            }
            var result = watcher_mailbox_post(request)
            if (!result.ok) return self.respond_error(resp, http_status.BAD_REQUEST, result.error)
            self.maybe_notify_inbox_message(request.session_id,
                request.direction, result.message_id, request.notify)
            result.revision = watcher_mailbox_session_revision(request.session_id)
            return resp |> JSON(sprint_json(result, false), http_status.CREATED)
        }
        POST("/api/v1/mailbox/state") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var request = WatcherMailboxStateRequest()
            if (!sscan_json(string(req.body), request)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid mailbox state JSON")
            }
            let result = watcher_mailbox_set_state(request)
            if (!result.ok) return self.respond_error(resp, http_status.BAD_REQUEST, result.error)
            return resp |> JSON(sprint_json(result, false), http_status.OK)
        }
        POST("/api/v1/bundles/sync") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!self.require_auth(req, resp)) return http_status.UNAUTHORIZED
            if (req == null || length(req.body) > MAX_REQUEST_BODY_BYTES) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "request body is too large")
            }
            var request = WatcherBundleSyncRequest()
            if (!sscan_json(string(req.body), request)) {
                return self.respond_error(resp, http_status.BAD_REQUEST, "invalid bundle JSON")
            }
            let result = watcher_bundle_sync(request)
            if (!result.ok) return self.respond_error(resp, http_status.BAD_REQUEST, result.error)
            return resp |> JSON(sprint_json(result, false), http_status.OK)
        }
    }

    def find_client(channel : WebSocketChannel) : int {
        for (index in range(length(clients))) {
            if (clients[index] != null && clients[index].channel == channel) return index
        }
        return -1
    }

    def send_error(channel : WebSocketChannel; message : string) {
        let body = "\{\"type\":\"error\",\"error\":{sprint_json(message, false)}\}"
        send(channel, body, ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_sessions(channel : WebSocketChannel) {
        send(channel, "\{\"type\":\"sessions\",\"data\":{watcher_list_json()}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_repositories(var client : WatcherClient?; force : bool = false) {
        if (client == null) return
        let revision = repository_revision()
        if (!force && client.repository_revision == revision) return
        client.repository_revision = revision
        send(client.channel, "\{\"type\":\"repositories\",\"data\":{repository_list_json()}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_herd_sessions(var client : WatcherClient?; force : bool = false) {
        if (client == null) return
        let revision = herd_revision()
        if (!force && client.herd_revision == revision) return
        client.herd_revision = revision
        send(client.channel, "\{\"type\":\"herd_sessions\",\"revision\":{revision}," +
            "\"profiles\":{herd_profiles_json()},\"data\":{herd_sessions_json()}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    // Launches (or relaunches) the PTY for a durable herd session and links it.
    def launch_herd_session(herd_id : string; initial : bool; var error : string&) : string {
        var inscope request = WatcherLaunchRequest()
        if (!herd_build_launch(herd_id, initial, self.agent_url, self.token,
                self.agent_skill_path, request, error)) {
            return ""
        }
        let launched = watcher_launch(request)
        if (!launched.ok) {
            let _ = herd_mark_launch_failed(herd_id, launched.error)
            error = launched.error
            return ""
        }
        let link_error = herd_link_pty(herd_id, launched.session_id)
        if (!empty(link_error)) {
            print("dasHerd watcher: herd registry save failed: {link_error}\n")
        }
        return launched.session_id
    }

    def send_mailbox(var client : WatcherClient?; force : bool = false) {
        if (client == null || empty(client.session_id)) return
        let revision = watcher_mailbox_session_revision(client.session_id)
        if (revision < 0l) {
            send_error(client.channel, "unknown session")
            return
        }
        if (!force && client.mailbox_revision == revision) return
        client.mailbox_revision = revision
        send(client.channel, "\{\"type\":\"mailbox\",\"session_id\":" +
            "{sprint_json(client.session_id, false)},\"revision\":{revision},\"data\":" +
            "{watcher_mailbox_json(client.session_id)}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_bundles(var client : WatcherClient?; force : bool = false) {
        if (client == null || empty(client.session_id)) return
        let revision = watcher_bundle_session_revision(client.session_id)
        if (revision < 0l || (!force && client.bundles_revision == revision)) return
        client.bundles_revision = revision
        send(client.channel, "\{\"type\":\"bundles\",\"session_id\":" +
            "{sprint_json(client.session_id, false)},\"revision\":{revision},\"data\":" +
            "{watcher_bundles_json(client.session_id)}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_file_inspection(var client : WatcherClient?;
                             loading : bool; patch, error : string;
                             view_text : string = ""; status : string = "";
                             view_base64 : string = "";
                             view_byte_count : int = 0;
                             view_available : bool = false) {
        if (client == null) return
        let message = RepositoryFileInspectionMessage(
            request_id = client.inspection_request_id,
            repository_id = client.inspection_repository_id,
            worktree_path = client.inspection_worktree_path,
            file_path = client.inspection_file_path,
            comparison = client.inspection_comparison,
            revision = client.inspection_revision,
            diff_context = client.inspection_diff_context,
            task_session_id = client.inspection_session_id,
            loading = loading, patch = patch, view_text = view_text,
            view_base64 = view_base64, view_byte_count = view_byte_count,
            view_available = view_available,
            status = status, error = error)
        send(client.channel, sprint_json(message, false),
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def remember_inspection_artifact(path : string) {
        for (pending in pending_inspection_artifacts) {
            if (pending == path) return
        }
        pending_inspection_artifacts |> push(clone_string(path))
    }

    def retry_inspection_artifact_cleanup() {
        var index = length(pending_inspection_artifacts)
        while (index > 0) {
            index--
            let path = pending_inspection_artifacts[index]
            if (!fexist(path) || remove(path)) {
                pending_inspection_artifacts |> erase(index)
            }
        }
    }

    def cleanup_inspection_artifact(var client : WatcherClient?) {
        if (client == null || empty(client.inspection_artifact_path)) return
        let path = client.inspection_artifact_path
        if (fexist(path) && !remove(path)) {
            self.remember_inspection_artifact(path)
        }
        client.inspection_artifact_path = ""
    }

    def cleanup_files_artifact(var client : WatcherClient?) {
        if (client == null || empty(client.files_artifact_path)) return
        let path = client.files_artifact_path
        if (fexist(path) && !remove(path)) {
            self.remember_inspection_artifact(path)
        }
        client.files_artifact_path = ""
    }

    def send_worktree_files(var client : WatcherClient?; loading : bool;
                            listing : string; error : string = "") {
        if (client == null) return
        let message = RepositoryWorktreeFilesMessage(
            request_id = client.files_request_id,
            repository_id = client.files_repository_id,
            worktree_path = client.files_worktree_path,
            loading = loading, listing = listing, error = error)
        send(client.channel, sprint_json(message, false),
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def start_worktree_files(var client : WatcherClient?;
                             incoming : WatcherWsMessage const) {
        if (client == null || empty(incoming.repository_id)
                || empty(incoming.worktree_path)) return
        client.files_request_id = incoming.request_id
        client.files_repository_id = incoming.repository_id
        client.files_worktree_path = incoming.worktree_path
        client.files_session_id = ""
        self.cleanup_files_artifact(client)
        let artifact_path = watcher_scratch_artifact_path(
            "files-{client.id}-{incoming.request_id}.txt")
        if (empty(artifact_path)) {
            client.files_delivered = true
            self.send_worktree_files(client, false, "",
                "could not allocate the file-listing capture path")
            return
        }
        var inscope listing_argv <- ["git", "-C", incoming.worktree_path,
            "-c", "core.quotePath=false", "ls-files", "--cached", "--others",
            "--exclude-standard"]
        var inscope argv : array<string>
        let command_error = repository_capture_argv(listing_argv,
            artifact_path, 0l, argv)
        if (!empty(command_error)) {
            client.files_delivered = true
            self.send_worktree_files(client, false, "", command_error)
            return
        }
        let _pruned = watcher_prune_completed_sessions(
            "git-ls-files", incoming.repository_id, incoming.worktree_path)
        var inscope request = WatcherLaunchRequest(
            cwd = incoming.worktree_path,
            display_name = "Git files: {incoming.worktree_path}",
            kind = "git-ls-files", target_id = "local",
            repository_id = incoming.repository_id,
            worktree_path = incoming.worktree_path,
            columns = 80, rows = 25, timeout_ms = 15000l,
            drain_quiet_ms = WATCHER_STRUCTURED_DRAIN_QUIET_MS)
        request.argv := argv
        let result = watcher_launch(request)
        client.files_session_id = result.session_id
        client.files_artifact_path = artifact_path
        if (!result.ok) {
            client.files_delivered = true
            self.cleanup_files_artifact(client)
            self.send_worktree_files(client, false, "", result.error)
            return
        }
        client.files_delivered = false
        self.send_worktree_files(client, true, "")
    }

    def poll_worktree_files(var client : WatcherClient?) {
        if (client == null || client.files_delivered
                || empty(client.files_session_id)) return
        var info = WatcherSessionInfo()
        if (!watcher_get_session_info(client.files_session_id, info)
                || !info.drained) return
        client.files_delivered = true
        var truncated = false
        let output = watcher_machine_output_text(client.files_session_id, truncated)
        let _compacted = watcher_compact_session(client.files_session_id, 80, 25)
        if (info.exit_code != 0l) {
            self.cleanup_files_artifact(client)
            self.send_worktree_files(client, false, "",
                "git ls-files failed: {output}")
            return
        }
        var listing = ""
        let read_error = repository_read_text_file(
            client.files_artifact_path, MAX_FILE_PREVIEW_RAW_BYTES, listing)
        self.cleanup_files_artifact(client)
        if (!empty(read_error)) {
            self.send_worktree_files(client, false, "", read_error)
            return
        }
        self.send_worktree_files(client, false, listing)
    }

    def send_git_file_action(var client : WatcherClient?; loading : bool;
                             status : string; error : string = "") {
        if (client == null) return
        let message = RepositoryGitFileActionMessage(
            request_id = client.action_request_id,
            repository_id = client.action_repository_id,
            worktree_path = client.action_worktree_path,
            file_path = client.action_file_path,
            action = client.action,
            task_session_id = client.action_session_id,
            loading = loading, status = status, error = error)
        send(client.channel, sprint_json(message, false),
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def start_git_file_action(var client : WatcherClient?;
                              incoming : WatcherWsMessage const) {
        if (client == null) return
        if (!client.action_delivered) {
            self.send_git_file_action(client, false, "failed",
                "another Git file action is still running")
            return
        }
        client.action_request_id = incoming.request_id
        client.action_repository_id = incoming.repository_id
        client.action_worktree_path = incoming.worktree_path
        client.action_file_path = incoming.file_path
        client.action = incoming.action
        client.action_session_id = ""
        var inscope argv : array<string>
        let command_error = repository_file_action_argv(
            incoming.repository_id, incoming.worktree_path,
            incoming.file_path, incoming.action, argv)
        if (!empty(command_error)) {
            self.send_git_file_action(client, false, "failed", command_error)
            return
        }
        let kind = incoming.action == "stage" ? "git-stage" : "git-unstage"
        let _pruned = watcher_prune_completed_sessions(
            kind, incoming.repository_id, incoming.worktree_path)
        var inscope request = WatcherLaunchRequest(
            cwd = incoming.worktree_path,
            display_name = "Git {incoming.action}: {incoming.file_path}",
            kind = kind, target_id = "local",
            repository_id = incoming.repository_id,
            worktree_path = incoming.worktree_path,
            columns = 160, rows = 50, timeout_ms = 15000l,
            drain_quiet_ms = WATCHER_STRUCTURED_DRAIN_QUIET_MS)
        request.argv := argv
        let result = watcher_launch(request)
        client.action_session_id = result.session_id
        if (!result.ok) {
            self.send_git_file_action(client, false, "failed", result.error)
            return
        }
        client.action_delivered = false
        self.send_git_file_action(client, true, "running")
    }

    def poll_git_file_action(var client : WatcherClient?) {
        if (client == null || client.action_delivered
                || empty(client.action_session_id)) return
        var info = WatcherSessionInfo()
        if (!watcher_get_session_info(client.action_session_id, info)
                || !info.drained) return
        client.action_delivered = true
        var truncated = false
        let output = watcher_machine_output_text(client.action_session_id, truncated)
        let _compacted = watcher_compact_session(client.action_session_id, 160, 50)
        var failure = ""
        if (truncated) {
            failure = "Git action output exceeded the retained machine-output limit"
        } elif (info.exit_code != 0l) {
            failure = "git exited with {info.exit_code}: {output}"
        }
        if (!empty(failure)) {
            let _failure_error = repository_record_task_failure(
                client.action_repository_id, client.action_session_id, failure)
            self.send_git_file_action(client, false, "failed", failure)
        } else {
            self.send_git_file_action(client, false, "completed")
        }
        let _refresh_error = repository_refresh(client.action_repository_id)
        let _review_error = repository_review(
            client.action_repository_id, client.action_worktree_path)
    }

    def remember_orphaned_git_action(var client : WatcherClient?) {
        if (client == null || client.action_delivered
                || empty(client.action_session_id)) return
        orphaned_git_actions |> push(OrphanedGitFileAction(
            session_id = client.action_session_id,
            repository_id = client.action_repository_id,
            worktree_path = client.action_worktree_path,
            file_path = client.action_file_path,
            action = client.action))
        print("dasHerd watcher: retaining Git {client.action} task " +
            "{client.action_session_id} for {client.action_file_path} " +
            "after client disconnect\n")
    }

    def poll_orphaned_git_actions() {
        var index = length(orphaned_git_actions)
        while (index > 0) {
            index--
            let action = orphaned_git_actions[index]
            var info = WatcherSessionInfo()
            if (!watcher_get_session_info(action.session_id, info)) {
                let failure = "orphaned Git action session disappeared before observation"
                let _failure_error = repository_record_task_failure(
                    action.repository_id, action.session_id, failure)
                let _refresh_error = repository_refresh(action.repository_id)
                let _review_error = repository_review(
                    action.repository_id, action.worktree_path)
                print("dasHerd watcher: Git {action.action} task " +
                    "{action.session_id} for {action.file_path} disappeared\n")
                orphaned_git_actions |> erase(index)
                continue
            }
            if (!info.drained) continue
            var truncated = false
            let output = watcher_machine_output_text(action.session_id, truncated)
            let _compacted = watcher_compact_session(
                action.session_id, 160, 50)
            var failure = ""
            if (truncated) {
                failure = "Git action output exceeded the retained machine-output limit"
            } elif (info.exit_code != 0l) {
                failure = "git exited with {info.exit_code}: {output}"
            }
            if (!empty(failure)) {
                let _failure_error = repository_record_task_failure(
                    action.repository_id, action.session_id, failure)
                print("dasHerd watcher: orphaned Git {action.action} task " +
                    "{action.session_id} for {action.file_path} failed: {failure}\n")
            } else {
                print("dasHerd watcher: orphaned Git {action.action} task " +
                    "{action.session_id} for {action.file_path} completed\n")
            }
            let _refresh_error = repository_refresh(action.repository_id)
            let _review_error = repository_review(
                action.repository_id, action.worktree_path)
            orphaned_git_actions |> erase(index)
        }
    }

    def start_agent_worktree_launch(var client : WatcherClient?;
                                    incoming : WatcherWsMessage const) {
        if (client == null) return
        // Legacy task-driven launches carry no profile; herd-managed launches
        // pick a profile and may be task-less (the human drives the agent).
        if (empty(incoming.profile_id) && empty(strip(incoming.task_context))) {
            send_error(client.channel, "agent task is required")
            return
        }
        if (empty(incoming.repository_id) || empty(incoming.worktree_path)
                || empty(incoming.new_worktree_path) || empty(incoming.branch)) {
            send_error(client.channel,
                "repository, source worktree, new worktree path, and branch are required")
            return
        }
        var create_argv : array<string>
        let command_error = repository_build_worktree_create_argv(
            incoming.worktree_path, incoming.new_worktree_path,
            incoming.branch, incoming.base_ref, create_argv)
        if (!empty(command_error)) {
            send_error(client.channel, command_error)
            return
        }
        let created = watcher_launch(WatcherLaunchRequest(argv <- create_argv,
            cwd = incoming.worktree_path, display_name = "Create {incoming.branch}",
            kind = "worktree-create", target_id = "local",
            repository_id = incoming.repository_id,
            worktree_path = incoming.new_worktree_path,
            purpose = "debug-setup", task_context = incoming.task_context,
            columns = 120, rows = 30, timeout_ms = 0l,
            drain_quiet_ms = WATCHER_STRUCTURED_DRAIN_QUIET_MS))
        if (!created.ok) {
            send_error(client.channel, created.error)
            return
        }
        pending_agent_launches |> push(PendingAgentWorktreeLaunch(
            create_session_id = created.session_id, client_id = client.id,
            repository_id = incoming.repository_id,
            source_worktree_path = incoming.worktree_path,
            new_worktree_path = incoming.new_worktree_path,
            branch = incoming.branch,
            task_context = incoming.task_context,
            profile_id = incoming.profile_id,
            name = incoming.name,
            workflow = incoming.workflow))
        let _ = watcher_record_event(created.session_id, "agent_worktree_queued",
            "source={incoming.worktree_path}; target={incoming.new_worktree_path}; " +
            "branch={incoming.branch}; base={incoming.base_ref}")
        send(client.channel, "\{\"type\":\"launched\",\"session_id\":" +
            "{sprint_json(created.session_id, false)}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
        send_sessions(client.channel)
    }

    def poll_agent_worktree_launches() {
        var index = length(pending_agent_launches) - 1
        while (index >= 0) {
            let pending = pending_agent_launches[index]
            var info = WatcherSessionInfo()
            if (!watcher_get_session_info(pending.create_session_id, info)
                    || !info.drained) {
                index--
                continue
            }
            var client : WatcherClient? = null
            for (candidate in clients) {
                if (candidate != null && candidate.id == pending.client_id) {
                    client = candidate
                    break
                }
            }
            if (info.exit_code != 0l) {
                let worktree_failure_logged = watcher_record_event(pending.create_session_id,
                    "agent_worktree_failed", "exit_code={info.exit_code}")
                if (client != null) {
                    send_error(client.channel,
                        "worktree creation failed; inspect session {pending.create_session_id}")
                }
                pending_agent_launches |> erase(index)
                index--
                continue
            }
            let worktree_created_logged = watcher_record_event(pending.create_session_id,
                "agent_worktree_created", "target={pending.new_worktree_path}")
            let repository_refreshed = repository_refresh(pending.repository_id)
            var create_error : string
            let fallback_name = (!empty(pending.branch) ? pending.branch
                : base_name(pending.new_worktree_path))
            let herd_id = herd_create(HerdCreateRequest(
                name = !empty(strip(pending.name)) ? pending.name : fallback_name,
                profile_id = !empty(pending.profile_id) ? pending.profile_id : "codex",
                workflow = !empty(pending.workflow) ? pending.workflow : "debug",
                repository_id = pending.repository_id,
                worktree_path = pending.new_worktree_path,
                task_context = pending.task_context), create_error)
            var launch_error : string
            var pty_session_id : string
            if (empty(herd_id)) {
                launch_error = create_error
            } else {
                pty_session_id = self.launch_herd_session(herd_id, true, launch_error)
            }
            if (empty(pty_session_id)) {
                let launch_failure_logged = watcher_record_event(pending.create_session_id,
                    "agent_launch_failed", launch_error)
            } else {
                let launch_started_logged = watcher_record_event(pending.create_session_id,
                    "agent_launch_started", "session={pty_session_id}; herd={herd_id}")
            }
            if (client != null) {
                if (empty(pty_session_id)) {
                    send_error(client.channel, launch_error)
                } else {
                    send(client.channel, "\{\"type\":\"launched\",\"session_id\":" +
                        "{sprint_json(pty_session_id, false)}\}",
                        ws_opcode.WS_OPCODE_TEXT, true)
                }
                send_sessions(client.channel)
                send_herd_sessions(client, true)
            }
            pending_agent_launches |> erase(index)
            index--
        }
    }

    def start_indexed_file_view(var client : WatcherClient?; patch : string) {
        if (client == null) return
        let artifact_path = watcher_session_artifact_path(
            client.inspection_session_id, "git-view.raw")
        var inscope argv : array<string>
        var command_error = ""
        if (client.inspection_comparison == "commit") {
            command_error = repository_commit_file_view_argv(
                client.inspection_worktree_path, client.inspection_revision,
                client.inspection_file_path, artifact_path, argv)
        } else {
            command_error = repository_staged_file_view_argv(
                client.inspection_worktree_path, client.inspection_file_path,
                artifact_path, argv)
        }
        if (!empty(command_error)) {
            client.inspection_delivered = true
            self.send_file_inspection(client, false, patch, command_error)
            return
        }
        let _pruned = watcher_prune_completed_sessions(
            "git-file-view", client.inspection_repository_id,
            client.inspection_worktree_path)
        var inscope request = WatcherLaunchRequest(
            cwd = client.inspection_worktree_path,
            display_name = "Git file view: {client.inspection_file_path}",
            kind = "git-file-view", target_id = "local",
            repository_id = client.inspection_repository_id,
            worktree_path = client.inspection_worktree_path,
            columns = 80, rows = 25, timeout_ms = 15000l,
            drain_quiet_ms = WATCHER_STRUCTURED_DRAIN_QUIET_MS)
        request.argv := argv
        let result = watcher_launch(request)
        client.inspection_phase = "indexed-view"
        client.inspection_patch = clone_string(patch)
        client.inspection_artifact_path = artifact_path
        client.inspection_session_id = result.session_id
        client.inspection_expected_exit_code = 0l
        if (!result.ok) {
            client.inspection_delivered = true
            self.cleanup_inspection_artifact(client)
            client.inspection_patch = ""
            self.send_file_inspection(client, false, patch, result.error)
            return
        }
        client.inspection_delivered = false
        self.send_file_inspection(client, true, "", "", "",
            "Reading full Git file (15 second limit)...")
    }

    def start_file_inspection(var client : WatcherClient?;
                              incoming : WatcherWsMessage const) {
        if (client == null) return
        if (!empty(client.inspection_session_id)
                && watcher_accepts_input(client.inspection_session_id)) {
            let _ = watcher_terminate(client.inspection_session_id, "superseded")
        }
        self.cleanup_inspection_artifact(client)
        client.inspection_request_id = incoming.request_id
        client.inspection_repository_id = incoming.repository_id
        client.inspection_worktree_path = incoming.worktree_path
        client.inspection_file_path = incoming.file_path
        client.inspection_comparison = incoming.comparison
        client.inspection_revision = incoming.revision
        client.inspection_diff_context = incoming.diff_context
        if (empty(client.inspection_diff_context)) {
            client.inspection_diff_context = "compact"
        }
        client.inspection_session_id = ""
        client.inspection_phase = "diff"
        client.inspection_patch = ""
        client.inspection_delivered = true

        var inscope diff_argv : array<string>
        var expected_exit_code = 0l
        var command_error = repository_file_diff_argv(
            incoming.repository_id, incoming.worktree_path,
            incoming.file_path, incoming.comparison,
            diff_argv, expected_exit_code, client.inspection_diff_context,
            client.inspection_revision)
        if (!empty(command_error)) {
            self.send_file_inspection(client, false, "", command_error)
            return
        }
        // The patch travels through a byte-exact file capture, never through
        // the task PTY: ConPTY wraps long diff lines at the terminal width and
        // the wrapped tails would parse as phantom context lines.
        let artifact_path = watcher_scratch_artifact_path(
            "diff-{client.id}-{incoming.request_id}.patch")
        if (empty(artifact_path)) {
            self.send_file_inspection(client, false, "",
                "could not allocate the diff capture path")
            return
        }
        var inscope argv : array<string>
        command_error = repository_capture_argv(diff_argv, artifact_path,
            expected_exit_code, argv)
        if (!empty(command_error)) {
            self.send_file_inspection(client, false, "", command_error)
            return
        }
        let _pruned = watcher_prune_completed_sessions(
            "git-file-diff", incoming.repository_id, incoming.worktree_path)
        var inscope request = WatcherLaunchRequest(
            cwd = incoming.worktree_path,
            display_name = "Git diff: {incoming.file_path}",
            kind = "git-file-diff", target_id = "local",
            repository_id = incoming.repository_id,
            worktree_path = incoming.worktree_path,
            columns = 80, rows = 25, timeout_ms = 15000l,
            drain_quiet_ms = WATCHER_STRUCTURED_DRAIN_QUIET_MS)
        request.argv := argv
        let result = watcher_launch(request)
        client.inspection_session_id = result.session_id
        client.inspection_artifact_path = artifact_path
        client.inspection_expected_exit_code = 0l
        if (!result.ok) {
            self.cleanup_inspection_artifact(client)
            self.send_file_inspection(client, false, "", result.error)
            return
        }
        client.inspection_delivered = false
        self.send_file_inspection(client, true, "", "", "",
            "Computing {client.inspection_diff_context} Git diff (15 second limit)...")
    }

    def poll_file_inspection(var client : WatcherClient?) {
        if (client == null || client.inspection_delivered
                || empty(client.inspection_session_id)) return
        var info = WatcherSessionInfo()
        if (!watcher_get_session_info(client.inspection_session_id, info)
                || !info.drained) return
        var truncated = false
        let output = watcher_machine_output_text(client.inspection_session_id, truncated)
        let _compacted = watcher_compact_session(client.inspection_session_id, 80, 25)
        if (client.inspection_phase == "indexed-view") {
            client.inspection_delivered = true
            if (truncated) {
                self.cleanup_inspection_artifact(client)
                let patch = client.inspection_patch
                client.inspection_patch = ""
                self.send_file_inspection(client, false, patch,
                    "Git file command exceeded the retained machine-output limit")
                return
            }
            if (info.exit_code != client.inspection_expected_exit_code) {
                self.cleanup_inspection_artifact(client)
                let patch = client.inspection_patch
                client.inspection_patch = ""
                self.send_file_inspection(client, false, patch,
                    "git show exited with {info.exit_code}: {output}")
                return
            }
            var view_text = ""
            var view_base64 = ""
            var view_byte_count = 0
            var view_error = ""
            if (git_patch_is_binary(client.inspection_patch)) {
                var inscope bytes : array<uint8>
                view_error = repository_read_file_bytes(
                    client.inspection_artifact_path,
                    MAX_FILE_PREVIEW_RAW_BYTES, bytes, view_byte_count)
                if (empty(view_error) && !empty(bytes)) {
                    view_base64 = base64_encode(bytes)
                }
            } else {
                view_error = repository_read_text_file(
                    client.inspection_artifact_path,
                    MAX_FILE_PREVIEW_RAW_BYTES, view_text)
            }
            self.cleanup_inspection_artifact(client)
            let patch = client.inspection_patch
            client.inspection_patch = ""
            if (!empty(view_error)) {
                self.send_file_inspection(client, false, patch, view_error)
                return
            }
            self.send_file_inspection(client, false, patch, "", view_text,
                "", view_base64, view_byte_count, true)
            return
        }
        client.inspection_delivered = true
        if (truncated) {
            self.cleanup_inspection_artifact(client)
            self.send_file_inspection(client, false, "",
                "diff exceeded the retained machine-output limit")
        } elif (info.exit_code != client.inspection_expected_exit_code) {
            self.cleanup_inspection_artifact(client)
            self.send_file_inspection(client, false, "",
                "git diff failed: {output}")
        } else {
            var patch = ""
            let patch_error = repository_read_text_file(
                client.inspection_artifact_path,
                MAX_FILE_PREVIEW_RAW_BYTES, patch)
            self.cleanup_inspection_artifact(client)
            if (!empty(patch_error)) {
                self.send_file_inspection(client, false, "", patch_error)
                return
            }
            if (client.inspection_comparison == "staged"
                    || client.inspection_comparison == "pr"
                    || client.inspection_comparison == "commit") {
                if (git_patch_result_is_deleted(patch)) {
                    self.send_file_inspection(client, false, patch, "",
                        "", "", "", 0, true)
                    return
                }
                self.start_indexed_file_view(client, patch)
                return
            }
            var view_text = ""
            var view_base64 = ""
            var view_byte_count = 0
            if (client.inspection_comparison == "working") {
                let binary = git_patch_is_binary(patch)
                var view_error = ""
                if (binary) {
                    var inscope bytes : array<uint8>
                    view_byte_count = 0
                    view_error = repository_working_file_bytes(
                        client.inspection_repository_id,
                        client.inspection_worktree_path,
                        client.inspection_file_path,
                        MAX_FILE_PREVIEW_RAW_BYTES, bytes, view_byte_count)
                    if (empty(view_error)) {
                        if (!empty(bytes)) {
                            view_base64 = base64_encode(bytes)
                        }
                    }
                } else {
                    view_error = repository_working_file_text(
                        client.inspection_repository_id,
                        client.inspection_worktree_path,
                        client.inspection_file_path,
                        MAX_FILE_PREVIEW_RAW_BYTES, view_text)
                }
                if (!empty(view_error)) {
                    self.send_file_inspection(client, false, patch, view_error)
                    return
                }
            }
            self.send_file_inspection(client, false, patch, "", view_text,
                "", view_base64, view_byte_count, true)
        }
    }

    def send_snapshot(var client : WatcherClient?; force : bool = false) {
        if (client == null || empty(client.session_id)) return
        let update_key = watcher_update_key(client.session_id)
        if (!force && update_key == client.last_update_key) return
        let snapshot = watcher_snapshot_json(client.session_id)
        if (empty(snapshot)) {
            send_error(client.channel, "unknown session")
            return
        }
        client.last_update_key = update_key
        send(client.channel, "\{\"type\":\"snapshot\",\"data\":{snapshot}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_raw_output(var client : WatcherClient?) {
        if (client == null || !client.raw || empty(client.session_id)) return
        var inscope chunk <- watcher_output_since(client.session_id, client.raw_offset)
        if (chunk.reset) {
            let start_offset = chunk.next_offset - int64(length(chunk.bytes))
            send(client.channel, "\{\"type\":\"raw_reset\",\"offset\":{start_offset}\}",
                ws_opcode.WS_OPCODE_TEXT, true)
        }
        client.raw_offset = chunk.next_offset
        if (!empty(chunk.bytes)) {
            unsafe {
                send(client.channel, reinterpret<string#>(addr(chunk.bytes[0])), length(chunk.bytes),
                    ws_opcode.WS_OPCODE_BINARY, true)
            }
        }
    }

    def send_terminal_checkpoint(var client : WatcherClient?) {
        if (client == null || !client.raw || empty(client.session_id)) return
        let checkpoint <- watcher_terminal_checkpoint(client.session_id)
        if (!checkpoint.ok) {
            send_error(client.channel, "unknown session")
            return
        }
        client.raw_offset = checkpoint.output_offset
        let message = WatcherTerminalCheckpointMessage(
            offset = checkpoint.output_offset,
            columns = checkpoint.columns, rows = checkpoint.rows,
            ansi = checkpoint.ansi)
        send(client.channel, sprint_json(message, false),
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def detach(var client : WatcherClient?; detail : string) {
        if (client == null) return
        let session_id = client.session_id
        let had_control = client.controls
        if (had_control && !empty(session_id)) {
            watcher_release_controller(session_id, client.id, detail)
        }
        if (!empty(session_id)) {
            let _ = watcher_record_event(session_id, "client_detached",
                "client={client.id}; control={had_control}; reason={detail}")
        }
        client.session_id = ""
        client.controls = false
        client.last_update_key = ""
        client.raw = false
        client.raw_offset = 0l
    }

    def override onWsOpen(channel : WebSocketChannel; url : string#) {
        if (!authorized(string(url))) {
            send_error(channel, "invalid watcher token")
            close(channel)
            return
        }
        next_client_id++
        clients |> push(new WatcherClient(channel = channel, id = next_client_id,
            last_heartbeat = ref_time_ticks()))
        print("dasHerd watcher: client {next_client_id} connected\n")
        send(channel, "\{\"type\":\"connected\",\"client_id\":{next_client_id}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
        send_sessions(channel)
        send_repositories(clients[length(clients) - 1], true)
        send_herd_sessions(clients[length(clients) - 1], true)
    }

    def override onWsClose(channel : WebSocketChannel) {
        let index = find_client(channel)
        if (index < 0) return
        var client = clients[index]
        let client_id = client.id
        detach(client, "client_closed")
        self.cleanup_inspection_artifact(client)
        self.cleanup_files_artifact(client)
        self.remember_orphaned_git_action(client)
        clients |> erase(index)
        unsafe { delete client }
        print("dasHerd watcher: client {client_id} disconnected\n")
    }

    def override onWsMessage(channel : WebSocketChannel; message : string#) {
        let index = find_client(channel)
        if (index < 0) return
        var inscope incoming = WatcherWsMessage()
        if (length(message) > MAX_REQUEST_BODY_BYTES || !sscan_json(string(message), incoming)) {
            send_error(channel, "invalid WebSocket message")
            return
        }
        var client = clients[index]
        if (client == null) return
        client.last_heartbeat = ref_time_ticks()
        if (incoming.op == "heartbeat") {
            send(channel, "\{\"type\":\"heartbeat\"\}", ws_opcode.WS_OPCODE_TEXT, true)
        } elif (incoming.op == "list") {
            send_sessions(channel)
        } elif (incoming.op == "repository_add") {
            let error = repository_add(RepositoryAddRequest(path = incoming.repository_path,
                display_name = incoming.display_name,
                target_id = watcher_target_id_or_local(incoming.target_id)))
            if (!empty(error)) {
                send_error(channel, error)
                return
            }
        } elif (incoming.op == "repository_refresh") {
            let error = repository_refresh(incoming.repository_id)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "repository_review") {
            let error = repository_review(
                incoming.repository_id, incoming.worktree_path)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "repository_history") {
            let error = repository_history(
                incoming.repository_id, incoming.worktree_path,
                incoming.revision)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "repository_commit_files") {
            let error = repository_commit_files(
                incoming.repository_id, incoming.worktree_path,
                incoming.revision)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "repository_refs") {
            let error = repository_refs(
                incoming.repository_id, incoming.worktree_path)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "git_file_action") {
            self.start_git_file_action(client, incoming)
        } elif (incoming.op == "file_inspect") {
            self.start_file_inspection(client, incoming)
        } elif (incoming.op == "worktree_files") {
            self.start_worktree_files(client, incoming)
        } elif (incoming.op == "mailbox_post") {
            let request = WatcherMailboxPostRequest(
                session_id = incoming.session_id,
                direction = incoming.direction,
                sender = incoming.sender,
                kind = incoming.kind,
                subject = incoming.subject,
                reply_to = incoming.reply_to,
                bundle_id = incoming.bundle_id,
                notify = incoming.notify,
                focus := incoming.focus)
            var result = watcher_mailbox_post(request)
            if (!result.ok) {
                send_error(channel, result.error)
                return
            }
            self.maybe_notify_inbox_message(request.session_id,
                request.direction, result.message_id, request.notify)
            result.revision = watcher_mailbox_session_revision(request.session_id)
            send(channel, "\{\"type\":\"mailbox_posted\",\"message_id\":" +
                "{sprint_json(result.message_id, false)},\"revision\":{result.revision}\}",
                ws_opcode.WS_OPCODE_TEXT, true)
            self.send_mailbox(client, true)
        } elif (incoming.op == "mailbox_state") {
            let result = watcher_mailbox_set_state(WatcherMailboxStateRequest(
                session_id = incoming.session_id,
                message_id = incoming.message_id, status = incoming.status))
            if (!result.ok) {
                send_error(channel, result.error)
                return
            }
            self.send_mailbox(client, true)
        } elif (incoming.op == "bundle_sync") {
            let result = watcher_bundle_sync(WatcherBundleSyncRequest(
                session_id = incoming.session_id, bundle := incoming.bundle))
            if (!result.ok) {
                send_error(channel, result.error)
                return
            }
            self.send_bundles(client, true)
            self.send_mailbox(client, true)
        } elif (incoming.op == "launch_agent_worktree") {
            self.start_agent_worktree_launch(client, incoming)
        } elif (incoming.op == "launch") {
            var inscope launch = WatcherLaunchRequest(
                command_line = incoming.command_line, cwd = incoming.cwd,
                display_name = incoming.display_name, kind = incoming.kind,
                target_id = watcher_target_id_or_local(incoming.target_id),
                repository_id = incoming.repository_id,
                worktree_path = incoming.worktree_path,
                purpose = incoming.purpose, task_context = incoming.task_context,
                columns = incoming.columns, rows = incoming.rows,
                timeout_ms = incoming.timeout_ms)
            launch.argv := incoming.argv
            if (watcher_kind_is_agent(launch.kind)) {
                launch.agent_url = self.agent_url
                launch.agent_token = self.token
                launch.agent_skill_path = self.agent_skill_path
            }
            let result = watcher_launch(launch)
            if (!result.ok) {
                send_error(channel, result.error)
                return
            }
            send(channel, "\{\"type\":\"launched\",\"session_id\":{sprint_json(result.session_id, false)}\}",
                ws_opcode.WS_OPCODE_TEXT, true)
            send_sessions(channel)
        } elif (incoming.op == "herd_create") {
            var create_error : string
            let herd_id = herd_create(HerdCreateRequest(name = incoming.name,
                host = watcher_target_id_or_local(incoming.target_id),
                profile_id = incoming.profile_id, workflow = incoming.workflow,
                repository_id = incoming.repository_id,
                worktree_path = incoming.worktree_path,
                task_context = incoming.task_context,
                columns = incoming.columns > 0 ? incoming.columns : 120,
                rows = incoming.rows > 0 ? incoming.rows : 36), create_error)
            if (empty(herd_id)) {
                send_error(channel, create_error)
                return
            }
            var launch_error : string
            let pty_session_id = self.launch_herd_session(herd_id, true, launch_error)
            if (empty(pty_session_id)) {
                send_error(channel, launch_error)
            } else {
                send(channel, "\{\"type\":\"herd_created\",\"herd_session_id\":" +
                    "{sprint_json(herd_id, false)},\"session_id\":" +
                    "{sprint_json(pty_session_id, false)}\}",
                    ws_opcode.WS_OPCODE_TEXT, true)
            }
            send_sessions(channel)
            send_herd_sessions(client, true)
        } elif (incoming.op == "herd_relaunch") {
            var inscope record = HerdSession()
            if (!herd_get(incoming.herd_session_id, record)) {
                send_error(channel, "unknown herd session")
                return
            }
            if (record.state != "exited") {
                send_error(channel, "herd session is {record.state}; only exited sessions relaunch")
                return
            }
            var launch_error : string
            let pty_session_id = self.launch_herd_session(incoming.herd_session_id,
                record.launch_count == 0, launch_error)
            if (empty(pty_session_id)) {
                send_error(channel, launch_error)
            } else {
                send(channel, "\{\"type\":\"herd_relaunched\",\"herd_session_id\":" +
                    "{sprint_json(incoming.herd_session_id, false)},\"session_id\":" +
                    "{sprint_json(pty_session_id, false)}\}",
                    ws_opcode.WS_OPCODE_TEXT, true)
            }
            send_sessions(channel)
            send_herd_sessions(client, true)
        } elif (incoming.op == "herd_close") {
            var pty_to_terminate : string
            let close_error = herd_close(incoming.herd_session_id, pty_to_terminate)
            if (!empty(close_error)) {
                send_error(channel, close_error)
                return
            }
            if (!empty(pty_to_terminate)) {
                let _ = watcher_terminate(pty_to_terminate, "herd_session_closed")
            }
            send_sessions(channel)
            send_herd_sessions(client, true)
        } elif (incoming.op == "herd_rename") {
            let rename_error = herd_rename(incoming.herd_session_id, incoming.name)
            if (!empty(rename_error)) {
                send_error(channel, rename_error)
                return
            }
            send_herd_sessions(client, true)
        } elif (incoming.op == "herd_add_satellite") {
            let satellite_error = herd_add_satellite(incoming.herd_session_id,
                incoming.repository_id, incoming.worktree_path, "user")
            if (!empty(satellite_error)) {
                send_error(channel, satellite_error)
                return
            }
            send_herd_sessions(client, true)
        } elif (incoming.op == "attach") {
            if (watcher_find_session(incoming.session_id) < 0) {
                send_error(channel, "unknown session")
                return
            }
            detach(client, "reattach")
            client.session_id = incoming.session_id
            let accepts_input = watcher_accepts_input(incoming.session_id)
            client.controls = incoming.control && accepts_input && watcher_claim_controller(incoming.session_id, client.id)
            client.raw = incoming.raw
            client.raw_offset = 0l
            let _ = watcher_record_event(incoming.session_id, "client_attached",
                "client={client.id}; control_requested={incoming.control}; " +
                "control_granted={client.controls}; raw={incoming.raw}; " +
                "accepts_input={accepts_input}")
            if (incoming.control && !accepts_input) {
                send_error(channel, "session is not running; output is replay-only")
            } elif (incoming.control && !client.controls) {
                send_error(channel, "controller lease is held by another client")
            }
            send(channel, "\{\"type\":\"attached\",\"session_id\":{sprint_json(client.session_id, false)},\"controls\":{client.controls}\}",
                ws_opcode.WS_OPCODE_TEXT, true)
            send_snapshot(client, true)
            self.send_terminal_checkpoint(client)
            send_raw_output(client)
            self.send_mailbox(client, true)
            self.send_bundles(client, true)
        } elif (incoming.op == "claim_control") {
            if (empty(client.session_id)) {
                send_error(channel, "client is not attached")
                return
            }
            let accepts_input = watcher_accepts_input(client.session_id)
            if (!accepts_input) {
                send_error(channel, "session is not running; output is replay-only")
                return
            }
            if (!client.controls) {
                client.controls = watcher_claim_controller(client.session_id, client.id)
            }
            if (!client.controls) {
                send_error(channel, "controller lease is held by another client")
                return
            }
            send(channel, "\{\"type\":\"attached\",\"session_id\":" +
                "{sprint_json(client.session_id, false)},\"controls\":true\}",
                ws_opcode.WS_OPCODE_TEXT, true)
        } elif (incoming.op == "detach") {
            detach(client, "client_detached")
            send(channel, "\{\"type\":\"detached\"\}", ws_opcode.WS_OPCODE_TEXT, true)
        } elif (incoming.op == "input") {
            if (!client.controls) {
                send_error(channel, "client does not hold the controller lease")
                return
            }
            let error = watcher_input(client.session_id, incoming.text, client.id)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "resize") {
            if (!client.controls) {
                send_error(channel, "client does not hold the controller lease")
                return
            }
            let error = watcher_resize(client.session_id, incoming.columns, incoming.rows, client.id)
            if (!empty(error)) send_error(channel, error)
        } elif (incoming.op == "terminate") {
            let session_id = !empty(incoming.session_id) ? incoming.session_id : client.session_id
            let error = watcher_terminate(session_id)
            if (!empty(error)) send_error(channel, error)
            send_sessions(channel)
        } else {
            send_error(channel, "unknown operation")
        }
    }

    def override onTick {
        watcher_tick()
        herd_tick()
        repository_tick()
        self.retry_inspection_artifact_cleanup()
        self.poll_orphaned_git_actions()
        self.poll_agent_worktree_launches()
        for (index in range(length(clients))) {
            var client = clients[index]
            if (client == null) continue
            if (client.controls && get_time_nsec(client.last_heartbeat) / 1000000l >= CONTROLLER_HEARTBEAT_TIMEOUT_MS) {
                watcher_release_controller(client.session_id, client.id, "heartbeat_timeout")
                client.controls = false
                send_error(client.channel, "controller lease expired")
            }
            send_snapshot(client)
            send_raw_output(client)
            send_repositories(client)
            send_herd_sessions(client)
            send_mailbox(client)
            send_bundles(client)
            self.poll_git_file_action(client)
            self.poll_file_inspection(client)
            self.poll_worktree_files(client)
        }
    }

    def cleanup_clients {
        for (index in range(length(clients))) {
            var client = clients[index]
            if (client != null) {
                detach(client, "server_shutdown")
                self.cleanup_inspection_artifact(client)
                unsafe { delete client }
            }
        }
        clients |> clear()
        unsafe { delete clients }
    }

    def cleanup_inspection_artifacts {
        self.retry_inspection_artifact_cleanup()
        delete pending_inspection_artifacts
        delete orphaned_git_actions
        delete pending_agent_launches
    }
}
