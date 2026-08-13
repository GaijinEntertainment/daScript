options gen2
options rtti
options persistent_heap
options gc
options indenting = 4

module watcher_core shared public

require terminal/terminal public
require dashv/dashv_boost
require daslib/fio public
require daslib/json_boost public
require daslib/strings_boost
require daslib/base64
require daslib/random
require strings public
require math

let private DEFAULT_DRAIN_QUIET_MS = 250l
// Structured non-interactive tasks (git observation, capture helpers) parse
// machine output after exit. Across 1968 logged sessions the pipe outlived the
// conservative 250ms quiet interval in all but 2; output was always complete
// at process exit, so a short quiet window only guards ConHost flush latency.
let public WATCHER_STRUCTURED_DRAIN_QUIET_MS = 40l
let private MAX_COMMAND_BYTES = 32 * 1024
let private MAX_INPUT_BYTES = 1024 * 1024
let private MAX_RAW_HISTORY_BYTES = 8 * 1024 * 1024
let private RAW_HISTORY_TRIM_SLACK = 1024 * 1024
let private WATCHER_GC_STRING_PRESSURE_BYTES = 64ul * 1024ul * 1024ul
let private WATCHER_GC_HEAP_PRESSURE_BYTES = 128ul * 1024ul * 1024ul

// Detached PTY hosts (PTY_HOST_DESIGN.md): durable herd sessions run their
// ConPTY inside a dasherd-ptyhost process the watcher merely talks to.
let private HOST_PROTOCOL_VERSION = 1
let private HOST_PORT_BASE = 9700
let private HOST_PORT_SPAN = 200
let private HOST_PORT_ATTEMPTS = 8
let private HOST_STAMP_TIMEOUT_MS = 10000l
let private HOST_HANDSHAKE_TIMEOUT_MS = 10000l
let private HOST_ADOPT_HANDSHAKE_TIMEOUT_MS = 3000l
let private HOST_STAT_POLL_MS = 500l

def public watcher_gc_reason(string_used, string_total,
                             heap_used, heap_total : uint64) : string {
    if (string_used >= WATCHER_GC_STRING_PRESSURE_BYTES) return "string_pressure"
    if (heap_used >= WATCHER_GC_HEAP_PRESSURE_BYTES) return "heap_pressure"
    if (string_total > 0ul && string_used * 3ul < string_total * 2ul) {
        return "string_fragmentation"
    }
    if (heap_total > 0ul && heap_used * 3ul < heap_total) {
        return "heap_fragmentation"
    }
    return ""
}

struct public WatcherLaunchRequest {
    command_line : string
    argv : array<string>
    cwd : string
    display_name : string
    kind : string
    target_id : string = "local"
    repository_id : string
    worktree_path : string
    purpose : string
    task_context : string
    // Profile intro_lines joined for the initial launch prompt (agents only).
    intro_text : string
    agent_url : string
    agent_token : string
    agent_skill_path : string
    // Durable herd session hosting this PTY (empty for plumbing/legacy launches).
    herd_session_id : string
    // Relaunch of a durable session: argv is used verbatim, no task prompt.
    resume : bool
    columns : int = 100
    rows : int = 30
    timeout_ms : int64
    // 0 keeps the conservative interactive default (DEFAULT_DRAIN_QUIET_MS).
    drain_quiet_ms : int64
    // Extra KEY=VALUE entries merged over the inherited environment for the
    // child process (in-process ConPTY launches only).
    env : array<string>
}

def public watcher_kind_is_agent(kind : string) : bool => starts_with(kind, "agent-")

def public watcher_target_id_or_local(target_id : string) : string {
    return empty(target_id) ? "local" : target_id
}

struct public WatcherInputRequest {
    session_id : string
    text : string
}

struct public WatcherResizeRequest {
    session_id : string
    columns : int
    rows : int
}

struct public WatcherSessionRequest {
    session_id : string
}

struct public WatcherFocusRange {
    start_byte : int = -1
    end_byte : int = -1
    caption : string
}

struct public WatcherFocusTarget {
    repository_id : string
    worktree_path : string
    comparison : string
    revision : string
    role : string
    file_path : string
    whole_file : bool
    ranges : array<WatcherFocusRange>
}

struct public WatcherFocusSet {
    repository_id : string
    worktree_path : string
    comparison : string = "working"
    revision : string
    summary : string
    targets : array<WatcherFocusTarget>
}

struct public WatcherMailboxPostRequest {
    session_id : string
    direction : string
    sender : string = ""
    kind : string = "note"
    subject : string
    reply_to : string
    bundle_id : string
    notify : bool = true
    focus : WatcherFocusSet = WatcherFocusSet()
}

struct public WatcherMailboxStateRequest {
    session_id : string
    message_id : string
    status : string
}

struct public WatcherMailboxMessage {
    id : string
    session_id : string
    direction : string
    sender : string
    kind : string
    subject : string
    reply_to : string
    bundle_id : string
    status : string = "new"
    notified : bool
    notification_error : string
    fetched : bool
    created_ms : int64
    revision : int64
    focus : WatcherFocusSet = WatcherFocusSet()
}

struct public WatcherMailboxResult {
    ok : bool
    message_id : string
    revision : int64
    error : string
}

struct public WatcherBundleFile {
    path : string
    role : string
}

struct public WatcherBundleParticipant {
    repository_id : string
    worktree_path : string
    role : string
    base_revision : string
    head_revision : string
    state : string
    files : array<WatcherBundleFile>
}

struct public WatcherReviewBundle {
    id : string
    session_id : string
    title : string
    kind : string = "change"
    status : string = "draft"
    summary : string
    revision : int64
    updated_ms : int64
    participants : array<WatcherBundleParticipant>
    focus : WatcherFocusSet = WatcherFocusSet()
}

struct public WatcherBundleSyncRequest {
    session_id : string
    bundle : WatcherReviewBundle = WatcherReviewBundle()
}

struct public WatcherBundleResult {
    ok : bool
    bundle_id : string
    revision : int64
    error : string
}

struct private WatcherPendingNotification {
    message_id : string
    text : string
}

struct public WatcherEvent {
    sequence : int64
    wall_time_seconds : int64
    session_id : string
    event : string
    elapsed_ms : int64
    detail : string
    state : string
    kind : string
    purpose : string
    repository_id : string
    worktree_path : string
    pid : int
    controller_id : int
}

struct public WatcherSessionInfo {
    id : string
    command_line : string
    cwd : string
    display_name : string
    kind : string
    target_id : string
    repository_id : string
    worktree_path : string
    purpose : string
    task_context : string
    context_path : string
    herd_session_id : string
    state : string
    reason : string
    pid : int = 0
    exit_code : int64 = -1l
    elapsed_ms : int64
    output_bytes : int64
    input_bytes : int64
    columns : int
    rows : int
    revision : int64
    status_revision : int64
    event_sequence : int64
    controller_id : int
    drained : bool
}

struct public WatcherSessionSnapshot {
    session : WatcherSessionInfo = WatcherSessionInfo()
    screen_text : string
    title : string
    current_directory : string
    scrollback_rows : int
    cursor_row : int
    cursor_column : int
    cursor_visible : bool
    cursor_blinking : bool
}

struct public WatcherLaunchResult {
    ok : bool
    session_id : string
    error : string
}

struct public WatcherOutputChunk {
    bytes : array<uint8>
    next_offset : int64
    reset : bool
}

struct public WatcherTerminalCheckpoint {
    ok : bool
    ansi : string
    output_offset : int64
    columns : int
    rows : int
}

// Mirror of the dasherd-ptyhost discovery stamp (ptyhost/main.das HostStamp).
struct private WatcherHostStamp {
    version : int
    session : string
    port : int
    token : string
    child_pid : int
    journal_bytes : int64
    exited : bool
    exit_code : int64 = -1l
    drained : bool
    error : string
}

struct private HostStatMessage {
    @rename = "type" _type : string
    child_pid : int
    journal_bytes : int64
    exited : bool
    exit_code : int64 = -1l
    drained : bool
    error : string
}

class private WatcherHostLink : HvWebSocketClient {
    token : string
    attach_from : int64
    authorized : bool
    attach_sent : bool
    attached : bool
    closed : bool
    stat_seen : bool
    stat_child_pid : int
    stat_journal_bytes : int64
    stat_exited : bool
    stat_exit_code : int64 = -1l
    stat_drained : bool
    last_error : string
    received : array<uint8>
    pending_ops : array<string>

    def override onOpen {
        send("\{\"op\":\"hello\",\"token\":{sprint_json(token, false)},\"protocol\":{HOST_PROTOCOL_VERSION}\}")
    }

    def override onClose {
        closed = true
    }

    def override onMessage(msg : string#) {
        var incoming = HostStatMessage()
        if (!sscan_json(string(msg), incoming)) return
        if (incoming._type == "stat") {
            stat_seen = true
            stat_child_pid = incoming.child_pid
            stat_journal_bytes = incoming.journal_bytes
            stat_exited = incoming.exited
            stat_exit_code = incoming.exit_code
            stat_drained = incoming.drained
            if (!authorized) {
                authorized = true
                for (op in pending_ops) {
                    send(op)
                }
                pending_ops |> clear()
            }
            if (!attach_sent) {
                attach_sent = true
                send("\{\"op\":\"attach\",\"from_byte\":{attach_from}\}")
            }
        } elif (incoming._type == "attached") {
            attached = true
        } elif (incoming._type == "error") {
            last_error = incoming.error
        }
    }

    def override onMessageFrame(msg : string#; byte_count : int; opcode : ws_opcode) {
        if (opcode != ws_opcode.WS_OPCODE_BINARY) {
            onMessage(msg)
            return
        }
        if (byte_count <= 0) return
        let offset = length(received)
        received |> ensure_capacity(offset + byte_count)
        received |> resize_no_init(offset + byte_count)
        unsafe {
            memcpy(addr(received[offset]), reinterpret<void?>(msg), byte_count)
        }
    }

    def queue_or_send(op : string) : bool {
        // Ops issued between connect and the hello acknowledgement queue and
        // flush in order; the host rejects anything before authorization.
        if (closed) return false
        if (!authorized) {
            pending_ops |> push(op)
            return true
        }
        send(op)
        return true
    }

    def operator delete {
        delete received
        delete pending_ops
        delete super.self
    }
}

class private WatcherSession {
    id : string
    command_line : string
    cwd : string
    display_name : string
    kind : string
    target_id : string
    repository_id : string
    worktree_path : string
    purpose : string
    task_context : string
    context_path : string
    herd_session_id : string
    session_dir : string
    output_path : string
    events_path : string
    mailbox_path : string
    bundles_path : string
    state : string = "starting"
    reason : string
    pid : int = 0
    terminal : Terminal = Terminal()
    output_file : FILE const? = null
    events_file : FILE const? = null
    started : int64
    timeout_ms : int64
    drain_quiet_ms : int64
    last_data_ms : int64
    exit_seen_ms : int64 = -1l
    exit_code : int64 = -1l
    output_bytes : int64
    input_bytes : int64
    raw_history : array<uint8>
    raw_history_start : int64
    status_revision : int64
    event_sequence : int64
    controller_id : int
    drained : bool
    mailbox : array<WatcherMailboxMessage>
    mailbox_revision : int64
    mailbox_file : FILE const? = null
    bundles : array<WatcherReviewBundle>
    bundles_revision : int64
    bundles_file : FILE const? = null
    input_line_bytes : int
    input_escape_state : int
    pending_notifications : array<WatcherPendingNotification>
    // Detached-host transport (null = classic in-process ConPTY session).
    host : WatcherHostLink?
    host_port : int
    host_stamp_path : string
    host_last_stat_ms : int64
    host_shutdown_sent : bool
    host_archived : bool
    def operator delete {
        if (output_file != null) {
            fflush(output_file)
            fclose(output_file)
            output_file = null
        }
        if (events_file != null) {
            fflush(events_file)
            fclose(events_file)
            events_file = null
        }
        if (mailbox_file != null) {
            fflush(mailbox_file)
            fclose(mailbox_file)
            mailbox_file = null
        }
        if (bundles_file != null) {
            fflush(bundles_file)
            fclose(bundles_file)
            bundles_file = null
        }
        if (host != null) {
            let _closed = host->close()
            unsafe {
                delete host
            }
            host = null
        }
        terminal_dispose(terminal)
        delete raw_history
        delete mailbox
        delete bundles
        delete pending_notifications
    }
}

def private retain_raw_output(var session : WatcherSession?; bytes : array<uint8> const) {
    if (empty(bytes)) return
    let old_length = length(session.raw_history)
    session.raw_history |> resize(old_length + length(bytes))
    unsafe {
        memcpy(addr(session.raw_history[old_length]), addr(bytes[0]), length(bytes))
    }
    if (length(session.raw_history) > MAX_RAW_HISTORY_BYTES + RAW_HISTORY_TRIM_SLACK) {
        let discard = length(session.raw_history) - MAX_RAW_HISTORY_BYTES
        session.raw_history |> erase(0, discard)
        session.raw_history_start += int64(discard)
    }
}

var private g_sessions : array<WatcherSession?>
var private g_log_root : string
var private g_next_session_id = 0
var private g_next_mailbox_id = 0
var private g_mailbox_revision : int64
var private g_next_bundle_id = 0
var private g_bundle_revision : int64
var private g_ptyhost_dir : string
var private g_ptyhost_prefix : array<string>
var private g_host_port_cursor = 0

def private elapsed_ms(session : WatcherSession?) : int64 {
    return get_time_nsec(session.started) / 1000000l
}

def private write_text_file(path, body : string) : bool {
    var file = fopen(path, "wb")
    if (file == null) return false
    fwrite(file, body)
    fflush(file)
    fclose(file)
    return true
}

def private append_event(var session : WatcherSession?; event, detail : string) {
    if (session.events_file == null) return
    session.event_sequence++
    let line = sprint_json(WatcherEvent(sequence = session.event_sequence,
        wall_time_seconds = int64(get_clock()), session_id = session.id,
        event = event, elapsed_ms = elapsed_ms(session), detail = detail,
        state = session.state, kind = session.kind, purpose = session.purpose,
        repository_id = session.repository_id, worktree_path = session.worktree_path,
        pid = session.pid, controller_id = session.controller_id), false) + "\n"
    fwrite(session.events_file, line)
    fflush(session.events_file)
}

def public watcher_running_worktree_user(worktree_path : string) : string {
    //! Display name of a RUNNING PTY session whose cwd/worktree is this path,
    //! or "". Exited task sessions don't hold the tree.
    let full = get_full_file_name(worktree_path)
    let wanted = get_platform_name() == "windows" ? to_lower(full) : full
    for (session in g_sessions) {
        if (session == null || session.state != "running"
                || empty(session.worktree_path)) continue
        let candidate = get_full_file_name(session.worktree_path)
        let canonical = (get_platform_name() == "windows"
            ? to_lower(candidate) : candidate)
        if (canonical == wanted) {
            return empty(session.display_name) ? session.id : session.display_name
        }
    }
    return ""
}

def public watcher_record_event(session_id, event, detail : string) : bool {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null || empty(event)) return false
    append_event(g_sessions[index], event, detail)
    return true
}

def private session_info(session : WatcherSession?) : WatcherSessionInfo {
    return WatcherSessionInfo(id = session.id, command_line = session.command_line,
        cwd = session.cwd, display_name = session.display_name, kind = session.kind,
        target_id = session.target_id, repository_id = session.repository_id,
        worktree_path = session.worktree_path,
        purpose = session.purpose, task_context = session.task_context,
        context_path = session.context_path,
        herd_session_id = session.herd_session_id,
        state = session.state, reason = session.reason,
        pid = session.pid, exit_code = session.exit_code,
        elapsed_ms = elapsed_ms(session), output_bytes = session.output_bytes,
        input_bytes = session.input_bytes,
        columns = session.terminal.columns, rows = session.terminal.rows,
        revision = terminal_revision(session.terminal), status_revision = session.status_revision,
        event_sequence = session.event_sequence,
        controller_id = session.controller_id,
        drained = session.drained)
}

def public watcher_init(log_root : string) : string {
    g_log_root = log_root
    var error : string
    if (!mkdir_rec(g_log_root, error)) return error
    return ""
}

def public watcher_find_session(session_id : string) : int {
    for (index in range(length(g_sessions))) {
        if (g_sessions[index] != null && g_sessions[index].id == session_id) return index
    }
    return -1
}

def public watcher_accepts_input(session_id : string) : bool {
    let index = watcher_find_session(session_id)
    return (index >= 0 && g_sessions[index] != null &&
        g_sessions[index].state == "running" && !g_sessions[index].drained)
}

def public watcher_configure_ptyhosts(ptyhost_dir : string;
                                      launcher_prefix : array<string>) : string {
    //! Enables launch-via-host for durable (herd) sessions. ``ptyhost_dir``
    //! holds per-session journals and discovery stamps; ``launcher_prefix`` is
    //! the host command (release bundle exe, or interpreter + script + "--").
    //! Never configured = every launch stays a classic in-process ConPTY.
    g_ptyhost_dir = ""
    delete g_ptyhost_prefix
    if (empty(ptyhost_dir)) return ""
    var error : string
    if (!mkdir_rec(ptyhost_dir, error)) return error
    g_ptyhost_dir = ptyhost_dir
    g_ptyhost_prefix := launcher_prefix
    return ""
}

def private read_host_stamp(path : string; var stamp : WatcherHostStamp) : bool {
    if (!fexist(path)) return false
    return sscan_json(fread(path), stamp)
}

def private host_journal_path(session_id : string) : string {
    return path_join(g_ptyhost_dir, "{session_id}.raw")
}

def private host_stamp_path(session_id : string) : string {
    return path_join(g_ptyhost_dir, "{session_id}.host.json")
}

def private host_lock_path(session_id : string) : string {
    return path_join(g_ptyhost_dir, "{session_id}.lock")
}

// The host holds its .lock open for its whole life; Windows CRT handles carry
// no FILE_SHARE_DELETE, so remove() fails while the host lives and succeeds
// the instant it dies - crash included. Removing a dead host's lock here is
// the cleanup, not a side effect.
def private host_lock_held(session_id : string) : bool {
    let lock_path = host_lock_path(session_id)
    return fexist(lock_path) && !remove(lock_path)
}

def private archive_host_files(var session : WatcherSession?) {
    // Once the journal is replicated into the watcher (ring + registry fold),
    // the stamp and journal move aside so the next adoption scan stays clean.
    // Best-effort: a file the (dying) host still holds is picked up by the
    // next watcher start instead.
    if (session.host_archived) return
    session.host_archived = true
    let archive_dir = path_join(g_ptyhost_dir, "archive")
    var mkdir_error : string
    if (!mkdir_rec(archive_dir, mkdir_error)) return
    let stamp_path = host_stamp_path(session.id)
    if (fexist(stamp_path)) {
        let _ = rename(stamp_path, path_join(archive_dir, "{session.id}.host.json"))
    }
    let journal_path = host_journal_path(session.id)
    if (fexist(journal_path)) {
        let archived_journal = path_join(archive_dir, "{session.id}.raw")
        if (rename(journal_path, archived_journal)) {
            session.output_path = archived_journal
        }
    }
    let log_path = path_join(g_ptyhost_dir, "{session.id}.log")
    if (fexist(log_path)) {
        let _ = rename(log_path, path_join(archive_dir, "{session.id}.log"))
    }
    // A crashed host leaves its lock file behind unlockable; sweep it.
    let lock_path = host_lock_path(session.id)
    if (fexist(lock_path)) {
        let _ = remove(lock_path)
    }
}

def private session_process_exited(session : WatcherSession?) : bool {
    return (session.host != null ? session.host.stat_exited
        : terminal_process_exited(session.terminal))
}

def private session_exit_code(session : WatcherSession?) : int64 {
    return (session.host != null ? session.host.stat_exit_code
        : terminal_exit_code(session.terminal))
}

def private session_transport_error(session : WatcherSession?) : string {
    if (session.host == null) return terminal_transport_error(session.terminal)
    if (!empty(session.host.last_error)) return session.host.last_error
    return session.host.closed ? "host connection lost" : ""
}

def private session_write_bytes_host(var session : WatcherSession?;
                                     bytes : array<uint8> const) : bool {
    if (empty(bytes)) return true
    return session.host->queue_or_send(
        "\{\"op\":\"input\",\"base64\":\"{base64_encode(bytes)}\"\}")
}

def private session_write_text(var session : WatcherSession?; text : string) : bool {
    if (session.host == null) return terminal_write(session.terminal, text)
    var inscope bytes : array<uint8>
    let count = length(text)
    if (count > 0) {
        bytes |> resize_no_init(count)
        peek_data(text) $(view) {
            unsafe {
                memcpy(addr(bytes[0]), addr(view[0]), count)
            }
        }
    }
    return session_write_bytes_host(session, bytes)
}

def private launch_session_host(var session : WatcherSession?; // nolint:STYLE038
                                child_argv : array<string> const;
                                cwd : string; columns, rows : int) : string {
    var seed = random_seed(int(ref_time_ticks()))
    let token = "{random_uint(seed)}-{random_uint(seed)}-{random_uint(seed)}-{random_uint(seed)}"
    session.host_stamp_path = host_stamp_path(session.id)
    var last_error = "no free ptyhost port"
    for (attempt in range(HOST_PORT_ATTEMPTS)) {
        let port = HOST_PORT_BASE + ((g_host_port_cursor + attempt) % HOST_PORT_SPAN)
        if (fexist(session.host_stamp_path)) {
            // A previous attempt's bind-failure stamp must not satisfy the
            // wait loop below.
            let _ = remove(session.host_stamp_path)
        }
        var inscope host_argv := g_ptyhost_prefix
        host_argv |> reserve(length(host_argv) + 12 + length(child_argv) * 2)
        host_argv |> push_from(["--session", session.id, "--port", "{port}",
            "--journal", g_ptyhost_dir, "--cwd", cwd,
            "--columns", "{columns}", "--rows", "{rows}"])
        for (argument in child_argv) {
            host_argv |> push("--child")
            host_argv |> push(argument)
        }
        var spawn_pid = 0
        var spawn_error = ""
        let _spawned = spawn_detached(host_argv, "", ["DASHERD_HOST_TOKEN={token}"]) $(pid, launch_error) {
            spawn_pid = pid
            spawn_error := launch_error
        }
        if (spawn_pid == 0) return "could not spawn ptyhost: {spawn_error}"
        if (!empty(spawn_error)) {
            // Degraded breakaway: the host would die with the watcher's job -
            // the exact guarantee this architecture exists to provide.
            return "ptyhost spawn degraded: {spawn_error}"
        }
        var stamp = WatcherHostStamp()
        var stamp_seen = false
        let stamp_started = ref_time_ticks()
        while (get_time_nsec(stamp_started) / 1000000l < HOST_STAMP_TIMEOUT_MS) {
            if (read_host_stamp(session.host_stamp_path, stamp)) {
                stamp_seen = true
                break
            }
            sleep(5u)
        }
        if (!stamp_seen) return "ptyhost produced no discovery stamp"
        if (!empty(stamp.error)) {
            last_error = stamp.error
            continue if (find(stamp.error, "could not bind") >= 0)
            return stamp.error
        }
        var link = new WatcherHostLink(token = token, attach_from = 0l)
        if (link->init("ws://127.0.0.1:{port}/") != 0) {
            unsafe {
                delete link
            }
            last_error = "could not connect to ptyhost on port {port}"
            continue
        }
        let handshake_started = ref_time_ticks()
        while (!link.attached && !link.closed && empty(link.last_error)
                && get_time_nsec(handshake_started) / 1000000l < HOST_HANDSHAKE_TIMEOUT_MS) {
            link->process_event_que()
            sleep(2u)
        }
        if (!link.attached) {
            let handshake_error = (!empty(link.last_error)
                ? link.last_error : "ptyhost handshake timeout")
            let _closed = link->close()
            unsafe {
                delete link
            }
            return handshake_error
        }
        g_host_port_cursor = (port - HOST_PORT_BASE + 1) % HOST_PORT_SPAN
        session.host = link
        session.host_port = port
        session.pid = link.stat_child_pid
        return ""
    }
    return last_error
}

def private pump_host_session(var session : WatcherSession?; now_ms : int64) {
    var link = session.host
    link->process_event_que()
    if (!empty(link.received)) {
        session.last_data_ms = now_ms
        session.output_bytes += long_length(link.received)
        retain_raw_output(session, link.received)
        if (session.output_file != null) {
            let _ = fwrite(session.output_file, link.received)
            fflush(session.output_file)
        }
        terminal_feed_bytes(session.terminal, link.received)
        link.received |> clear()
        // Parser auto-replies (DA / DSR / CPR) must still reach the child.
        var inscope replies : array<array<uint8>>
        terminal_drain_replies_bytes(session.terminal, replies)
        for (reply in replies) {
            let _ = session_write_bytes_host(session, reply)
        }
    }
    if (session.pid == 0 && link.stat_child_pid != 0) {
        session.pid = link.stat_child_pid
        session.status_revision++
    }
    observe_exit(session, now_ms)
    enforce_timeout(session, now_ms)
    if (link.closed) {
        if (!session.drained) {
            // The host vanished mid-stream: that one session is lost (the
            // failure matrix's "host dies" row), never the whole herd.
            if (session.state == "running" || session.state == "terminating") {
                session.state = "failed"
                session.reason = "host_disconnected"
                session.status_revision++
                append_event(session, "transport_error", "host_disconnected")
            }
            finish_drain(session, "host_disconnected")
        }
        if (session.host_shutdown_sent) {
            archive_host_files(session)
        }
        return
    }
    if (!session.drained && link.stat_seen
            && session.output_bytes >= link.stat_journal_bytes) {
        if (link.stat_drained) {
            finish_drain(session, "host_drained")
        } elif (session.exit_seen_ms >= 0l) {
            // The host never observes pipe closure while it holds the HPCON,
            // so the exit fold reuses the direct path's quiet interval.
            let quiet_ms = (session.drain_quiet_ms > 0l
                ? session.drain_quiet_ms : DEFAULT_DRAIN_QUIET_MS)
            if (now_ms - session.last_data_ms >= quiet_ms) {
                finish_drain(session, "quiet_interval")
            }
        }
    }
    if (session.drained && session.exit_seen_ms >= 0l && !session.host_shutdown_sent) {
        // Journal fully replicated: release the host instead of letting it
        // linger. The stamp archives once the host acknowledges by exiting.
        session.host_shutdown_sent = true
        link->send("\{\"op\":\"shutdown\"\}")
        append_event(session, "host_released", "journal_bytes={session.output_bytes}")
    }
    if (!session.drained && link.authorized
            && now_ms - session.host_last_stat_ms >= HOST_STAT_POLL_MS) {
        session.host_last_stat_ms = now_ms
        link->send("\{\"op\":\"stat\"\}")
    }
}

def private numeric_id_suffix(value, prefix : string) : int {
    if (!starts_with(value, prefix)) return 0
    return to_int(slice(value, length(prefix)))
}

// Replays the session's persisted jsonl records after adoption so mailbox and
// bundle state (and their global id counters) survive a watcher restart.
def private restore_session_records(var session : WatcherSession?) {
    if (fexist(session.events_path)) {
        for (line in split(fread(session.events_path), "\n")) {
            continue if (empty(strip(line)))
            var event = WatcherEvent()
            if (sscan_json(line, event)) {
                session.event_sequence = max(session.event_sequence, event.sequence)
            }
        }
    }
    if (fexist(session.mailbox_path)) {
        for (line in split(fread(session.mailbox_path), "\n")) {
            continue if (empty(strip(line)))
            var message = WatcherMailboxMessage()
            continue if (!sscan_json(line, message))
            g_next_mailbox_id = max(g_next_mailbox_id, numeric_id_suffix(message.id, "hf_"))
            g_mailbox_revision = max(g_mailbox_revision, message.revision)
            session.mailbox_revision = max(session.mailbox_revision, message.revision)
            var found = -1
            for (index in range(length(session.mailbox))) {
                if (session.mailbox[index].id == message.id) {
                    found = index
                    break
                }
            }
            if (found >= 0) {
                session.mailbox[found] := message
            } else {
                session.mailbox |> emplace(message)
            }
        }
    }
    if (fexist(session.bundles_path)) {
        for (line in split(fread(session.bundles_path), "\n")) {
            continue if (empty(strip(line)))
            var bundle = WatcherReviewBundle()
            continue if (!sscan_json(line, bundle))
            g_next_bundle_id = max(g_next_bundle_id, numeric_id_suffix(bundle.id, "rb_"))
            g_bundle_revision = max(g_bundle_revision, bundle.revision)
            session.bundles_revision = max(session.bundles_revision, bundle.revision)
            var found = -1
            for (index in range(length(session.bundles))) {
                if (session.bundles[index].id == bundle.id) {
                    found = index
                    break
                }
            }
            if (found >= 0) {
                session.bundles[found] := bundle
            } else {
                session.bundles |> emplace(bundle)
            }
        }
    }
}

// Loads the journal tail of a dead host into the replay ring and the screen
// model. Returns the journal's total size (the session's final output_bytes).
def private seed_ring_from_journal(var session : WatcherSession?) : int64 {
    let journal = host_journal_path(session.id)
    let journal_stat = stat(journal)
    if (!journal_stat.is_valid) return 0l
    let total = int64(journal_stat.size)
    let from = max(0l, total - int64(MAX_RAW_HISTORY_BYTES))
    session.raw_history_start = from
    fopen(journal, "rb") $(reader) {
        if (reader == null) return
        var inscope chunk : array<uint8>
        chunk |> resize(64 * 1024)
        var skipped = 0l
        while (skipped < from) {
            let remaining = from - skipped
            let want = int(remaining > int64(64 * 1024) ? int64(64 * 1024) : remaining)
            if (want < length(chunk)) chunk |> resize(want)
            let got = fread(reader, chunk)
            if (got <= 0) break
            skipped += int64(got)
            if (length(chunk) < 64 * 1024) chunk |> resize(64 * 1024)
        }
        while (true) {
            let got = fread(reader, chunk)
            if (got <= 0) break
            if (got < length(chunk)) chunk |> resize(got)
            retain_raw_output(session, chunk)
            if (length(chunk) < 64 * 1024) chunk |> resize(64 * 1024)
        }
    }
    session.output_bytes = session.raw_history_start + long_length(session.raw_history)
    terminal_feed_bytes(session.terminal, session.raw_history)
    return total
}

def private adopt_host_session(stamp : WatcherHostStamp) : bool { // nolint:STYLE038
    let session_dir = path_join(g_log_root, stamp.session)
    let request_path = path_join(session_dir, "session.json")
    if (!fexist(request_path)) return false
    var inscope persisted = WatcherLaunchRequest()
    if (!sscan_json(fread(request_path), persisted)) return false
    var visible_command = persisted.display_name
    if (empty(visible_command)) {
        visible_command = (!empty(persisted.command_line)
            ? persisted.command_line : join(persisted.argv, " "))
    }
    let columns = clamp(persisted.columns > 0 ? persisted.columns : 120, 20, 400)
    let rows = clamp(persisted.rows > 0 ? persisted.rows : 36, 5, 512)
    var session = new WatcherSession(id = stamp.session,
        command_line = visible_command, cwd = persisted.cwd,
        display_name = persisted.display_name, kind = persisted.kind,
        target_id = watcher_target_id_or_local(persisted.target_id),
        repository_id = persisted.repository_id,
        worktree_path = persisted.worktree_path, purpose = persisted.purpose,
        task_context = persisted.task_context,
        herd_session_id = persisted.herd_session_id,
        session_dir = session_dir,
        output_path = host_journal_path(stamp.session),
        events_path = path_join(session_dir, "events.jsonl"),
        mailbox_path = path_join(session_dir, "mailbox.jsonl"),
        bundles_path = path_join(session_dir, "bundles.jsonl"),
        started = ref_time_ticks(), timeout_ms = 0l, drain_quiet_ms = 0l)
    session.context_path = path_join(session_dir, "context.md")
    session.host_stamp_path = host_stamp_path(stamp.session)
    session.events_file = fopen(session.events_path, "ab")
    session.mailbox_file = fopen(session.mailbox_path, "ab")
    session.bundles_file = fopen(session.bundles_path, "ab")
    restore_session_records(session)
    g_next_session_id = max(g_next_session_id,
        numeric_id_suffix(slice(stamp.session, find(stamp.session, "_") + 1), ""))
    var inscope model <- terminal_create(columns, rows)
    session.terminal := model
    // The lock probe answers dead-or-alive instantly and race-free; only a
    // held lock is worth the connect handshake (whose timeout stays as the
    // fallback for a host that is alive but wedged).
    let attach_from = max(0l, stamp.journal_bytes - int64(MAX_RAW_HISTORY_BYTES))
    var link = new WatcherHostLink(token = stamp.token, attach_from = attach_from)
    var adopted_live = false
    if (host_lock_held(stamp.session)
            && link->init("ws://127.0.0.1:{stamp.port}/") == 0) {
        let handshake_started = ref_time_ticks()
        while (!link.attached && !link.closed && empty(link.last_error)
                && get_time_nsec(handshake_started) / 1000000l < HOST_ADOPT_HANDSHAKE_TIMEOUT_MS) {
            link->process_event_que()
            sleep(2u)
        }
        adopted_live = link.attached
    }
    if (adopted_live) {
        session.host = link
        session.host_port = stamp.port
        session.pid = link.stat_child_pid
        session.output_bytes = attach_from
        session.raw_history_start = attach_from
        session.state = "running"
        session.status_revision++
        append_event(session, "session_adopted", ("live_host; port={stamp.port}; " +
            "journal_bytes={stamp.journal_bytes}; from_byte={attach_from}"))
    } else {
        let _closed = link->close()
        unsafe {
            delete link
        }
        let journal_total = seed_ring_from_journal(session)
        session.pid = stamp.child_pid
        session.exit_code = stamp.exited ? stamp.exit_code : -1l
        session.state = stamp.exited ? "exited" : "failed"
        session.reason = stamp.exited ? "process_exited" : "host_lost"
        session.exit_seen_ms = 0l
        session.drained = true
        session.status_revision++
        append_event(session, "session_adopted", ("dead_host; exited={stamp.exited}; " +
            "exit_code={stamp.exit_code}; journal_bytes={journal_total}"))
        archive_host_files(session)
    }
    g_sessions |> push(session)
    return true
}

def public watcher_adopt_hosts : int {
    //! Scans ptyhost discovery stamps: sessions of live hosts resurrect as
    //! RUNNING; dead hosts fold their recorded exit into an exited session
    //! (replay served from the journal tail) and archive their files. Runs
    //! before herd_init so registry records keep their running state. Returns
    //! the number of sessions adopted.
    if (empty(g_ptyhost_dir) || empty(g_log_root)) return 0
    var inscope stamp_files : array<string>
    dir(g_ptyhost_dir) $(name) {
        if (ends_with(name, ".host.json")) {
            stamp_files |> push(clone_string(name))
        }
    }
    var adopted = 0
    for (stamp_file in stamp_files) {
        var stamp = WatcherHostStamp()
        continue if (!read_host_stamp(path_join(g_ptyhost_dir, stamp_file), stamp)
            || empty(stamp.session) || stamp.version != HOST_PROTOCOL_VERSION
            || watcher_find_session(stamp.session) >= 0)
        if (adopt_host_session(stamp)) adopted++
    }
    return adopted
}

def public watcher_launch(request : WatcherLaunchRequest) : WatcherLaunchResult { // nolint:STYLE037,STYLE038
    if (empty(request.command_line) && empty(request.argv)) {
        return WatcherLaunchResult(error = "command_line or argv is required")
    }
    if (watcher_kind_is_agent(request.kind)) {
        // Herd-managed agent sessions may be task-less (human drives from the
        // terminal); direct agent launches keep requiring an assigned task.
        if (empty(request.herd_session_id) && empty(strip(request.task_context))) {
            return WatcherLaunchResult(error = "agent task is required")
        }
        if (length(request.task_context) > MAX_COMMAND_BYTES / 2) {
            return WatcherLaunchResult(error = "agent task is too large")
        }
    }
    if (length(request.command_line) > MAX_COMMAND_BYTES) {
        return WatcherLaunchResult(error = "command_line is too large")
    }
    if (length(request.display_name) > MAX_COMMAND_BYTES) {
        return WatcherLaunchResult(error = "display_name is too large")
    }
    var argv_bytes = max(0, length(request.argv) - 1)
    for (argument in request.argv) {
        argv_bytes += length(argument)
        if (argv_bytes > MAX_COMMAND_BYTES) {
            return WatcherLaunchResult(error = "argv is too large")
        }
    }
    if (empty(g_log_root)) {
        return WatcherLaunchResult(error = "watcher core is not initialized")
    }
    g_next_session_id++
    let session_id = "s{ref_time_ticks()}_{g_next_session_id}"
    let session_dir = path_join(g_log_root, session_id)
    var mkdir_error : string
    if (!mkdir_rec(session_dir, mkdir_error)) {
        return WatcherLaunchResult(error = "could not create session directory: {mkdir_error}")
    }

    var visible_command = request.display_name
    if (empty(visible_command)) {
        visible_command = !empty(request.command_line) ? request.command_line : join(request.argv, " ")
    }
    var session = new WatcherSession(id = session_id,
        command_line = visible_command, cwd = request.cwd,
        display_name = request.display_name, kind = request.kind,
        target_id = request.target_id, repository_id = request.repository_id,
        worktree_path = request.worktree_path, purpose = request.purpose,
        task_context = request.task_context,
        herd_session_id = request.herd_session_id,
        session_dir = session_dir,
        output_path = path_join(session_dir, "output.raw"),
        events_path = path_join(session_dir, "events.jsonl"),
        mailbox_path = path_join(session_dir, "mailbox.jsonl"),
        bundles_path = path_join(session_dir, "bundles.jsonl"),
        started = ref_time_ticks(), timeout_ms = max(0l, request.timeout_ms),
        drain_quiet_ms = max(0l, request.drain_quiet_ms))
    session.context_path = path_join(session_dir, "context.md")
    // Durable herd sessions run in a detached ptyhost when one is configured;
    // ephemeral plumbing (git captures, task terminals) stays in-process.
    let host_backed = (!empty(request.herd_session_id)
        && !empty(g_ptyhost_dir) && !empty(g_ptyhost_prefix)
        && (watcher_kind_is_agent(request.kind) || !empty(request.argv)))
    if (host_backed) {
        // The host journal IS the raw output record - no duplicate local copy.
        session.output_path = host_journal_path(session_id)
    } else {
        session.output_file = fopen(session.output_path, "wb")
    }
    session.events_file = fopen(session.events_path, "wb")
    session.mailbox_file = fopen(session.mailbox_path, "wb")
    session.bundles_file = fopen(session.bundles_path, "wb")
    if ((!host_backed && session.output_file == null) || session.events_file == null
            || session.mailbox_file == null || session.bundles_file == null) {
        unsafe { delete session }
        return WatcherLaunchResult(error = "could not open session logs")
    }
    let columns = clamp(request.columns, 20, 400)
    let rows = clamp(request.rows, 5, 512)
    var inscope persisted_request := request
    persisted_request.columns = columns
    persisted_request.rows = rows
    persisted_request.agent_token = ""
    // Env riders never persist: the value can be a secret, and session.json
    // outlives the process that needed it.
    persisted_request.env |> clear()
    if (!write_text_file(path_join(session_dir, "session.json"), sprint_json(persisted_request, true))) {
        unsafe { delete session }
        return WatcherLaunchResult(error = "could not write session metadata")
    }
    var session_created_detail = "cwd={request.cwd}; display={visible_command}"
    if (watcher_kind_is_agent(request.kind)) {
        if (!write_text_file(session.context_path,
                agent_context_markdown(request, session_id, session.context_path))) {
            unsafe { delete session }
            return WatcherLaunchResult(error = "could not write agent session context")
        }
        session_created_detail += "; context={session.context_path}"
        ensure_agent_local_instructions(request, session_created_detail)
    }
    append_event(session, "session_created", session_created_detail)

    var inscope child_argv : array<string>
    if (watcher_kind_is_agent(request.kind)) {
        append_event(session, "agent_bootstrap_prepared",
            "context={session.context_path}; skill={request.agent_skill_path}; " +
            "task_bytes={length(request.task_context)}")
        child_argv <- agent_launch_argv(request, session_id, session.context_path)  // nolint:PERF030 - first assignment; declared empty above
    } else {
        child_argv := request.argv
    }
    if (host_backed) {
        // The screen model lives here; the ConPTY lives in the host process.
        var inscope model <- terminal_create(columns, rows)
        session.terminal := model
        let host_error = launch_session_host(session, child_argv, request.cwd,
            columns, rows)
        if (!empty(host_error)) {
            session.state = "failed"
            session.reason = host_error
            session.drained = true
            session.status_revision++
            append_event(session, "launch_failed", session.reason)
        } else {
            session.state = "running"
            session.status_revision++
            append_event(session, "launched",
                "pid={session.pid}; host_port={session.host_port}")
        }
    } else {
        if (!empty(child_argv)) {
            if (!empty(request.env)) {
                var inscope launched <- terminal_launch_argv_env(child_argv,
                    request.cwd, request.env, columns, rows)
                session.terminal := launched
            } else {
                var inscope launched <- terminal_launch_argv(child_argv, request.cwd, columns, rows)
                session.terminal := launched
            }
        } else {
            var inscope launched <- terminal_launch(request.command_line, request.cwd, columns, rows)
            session.terminal := launched
        }
        if (terminal_process_id(session.terminal) == 0) {
            session.state = "failed"
            session.reason = terminal_transport_error(session.terminal)
            if (empty(session.reason)) { session.reason = "terminal launch failed" }
            session.drained = true
            session.status_revision++
            append_event(session, "launch_failed", session.reason)
        } else {
            session.pid = terminal_process_id(session.terminal)
            session.state = "running"
            session.status_revision++
            append_event(session, "launched", "pid={terminal_process_id(session.terminal)}")
        }
    }
    g_sessions |> push(session)
    if (session.state == "failed") {
        return WatcherLaunchResult(session_id = session_id, error = session.reason)
    }
    return WatcherLaunchResult(ok = true, session_id = session_id)
}

def private observe_exit(var session : WatcherSession?; now_ms : int64) {
    if (session.exit_seen_ms >= 0l || !session_process_exited(session)) return
    session.exit_seen_ms = now_ms
    session.exit_code = session_exit_code(session)
    if (session.state != "failed") {
        session.state = "exited"
    }
    session.status_revision++
    if (session.last_data_ms == 0l) {
        session.last_data_ms = now_ms
    }
    append_event(session, "process_exited", "exit_code={session.exit_code}")
}

def private finish_drain(var session : WatcherSession?; detail : string) {
    if (session.drained) return
    session.drained = true
    session.status_revision++
    if (session.output_file != null) {
        fflush(session.output_file)
        fclose(session.output_file)
        session.output_file = null
    }
    append_event(session, "output_drained", detail)
}

def private enforce_timeout(var session : WatcherSession?; now_ms : int64) {
    if (session.state != "running" || session.timeout_ms <= 0l || now_ms < session.timeout_ms) return
    session.reason = "wall_timeout"
    session.state = "terminating"
    session.status_revision++
    let terminated = terminal_terminate(session.terminal)
    append_event(session, "wall_timeout", "terminate={terminated}")
}

def public watcher_tick(maximum_bytes_per_session : int = 64 * 1024) {
    let bounded_bytes = clamp(maximum_bytes_per_session, 1024, 1024 * 1024)
    for (session in g_sessions) {
        if (session == null) continue
        let now_ms = elapsed_ms(session)
        if (session.host != null) {
            pump_host_session(session, now_ms)
            continue
        }
        if (session.drained) {
            // Pipe closure can race slightly ahead of the process handle. Keep
            // observing lifecycle/timeout state after output is fully drained.
            observe_exit(session, now_ms)
            enforce_timeout(session, now_ms)
            continue
        }
        let status = terminal_poll(session.terminal, bounded_bytes)
        if (status == TerminalPollStatus.data) {
            session.last_data_ms = now_ms
            session.output_bytes += long_length(session.terminal.read_buffer)
            retain_raw_output(session, session.terminal.read_buffer)
            if (session.output_file != null) {
                let _ = fwrite(session.output_file, session.terminal.read_buffer)
                fflush(session.output_file)
            }
        } elif (status == TerminalPollStatus.error) {
            session.state = "failed"
            session.reason = terminal_transport_error(session.terminal)
            session.status_revision++
            append_event(session, "transport_error", session.reason)
            finish_drain(session, "transport_error")
            continue
        }
        observe_exit(session, now_ms)
        enforce_timeout(session, now_ms)
        if (status == TerminalPollStatus.closed) {
            finish_drain(session, "pipe_closed")
        } else {
            let quiet_ms = (session.drain_quiet_ms > 0l
                ? session.drain_quiet_ms : DEFAULT_DRAIN_QUIET_MS)
            if (session.exit_seen_ms >= 0l && now_ms - session.last_data_ms >= quiet_ms) {
                finish_drain(session, "quiet_interval")
            }
        }
    }
}

def public watcher_output_since(session_id : string; requested_offset : int64;
                                maximum_bytes : int = 256 * 1024) : WatcherOutputChunk {
    var result = WatcherOutputChunk()
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return <- result
    let session = g_sessions[index]
    var offset = requested_offset
    if (offset < session.raw_history_start || offset > session.output_bytes) {
        offset = session.raw_history_start
        result.reset = true
    }
    let available = session.output_bytes - offset
    let count = min(int64(clamp(maximum_bytes, 1024, 1024 * 1024)), available)
    if (count > 0l) {
        let source = int(offset - session.raw_history_start)
        result.bytes |> resize(count)
        unsafe {
            memcpy(addr(result.bytes[0]), addr(session.raw_history[source]), count)
        }
    }
    result.next_offset = offset + count
    return <- result
}

def public watcher_terminal_checkpoint(session_id : string) : WatcherTerminalCheckpoint {
    var result = WatcherTerminalCheckpoint()
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return <- result
    let session = g_sessions[index]
    result.ansi = terminal_checkpoint_ansi(session.terminal)
    result.output_offset = session.output_bytes
    result.columns = session.terminal.columns
    result.rows = session.terminal.rows
    result.ok = true
    return <- result
}

def public watcher_get_session_info(session_id : string; var info : WatcherSessionInfo) : bool {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return false
    info = session_info(g_sessions[index])
    return true
}

def public watcher_output_text(session_id : string) : string {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return ""
    return string(g_sessions[index].raw_history)
}

def public watcher_strip_terminal_controls(value : string) : string { // nolint:STYLE038
    // Task processes launched through ConPTY are wrapped in cursor, screen,
    // title, and mode sequences even when the child itself emits plain text.
    // Machine consumers need the printable byte stream, not a bounded screen;
    // cursor moves are intentionally normalized into spaces/newlines so
    // ConPTY-expanded field and record separators remain machine-parseable.
    var inscope output : array<uint8>
    output |> reserve(length(value))
    var state = 0 // 0=text, 1=ESC, 2=CSI, 3=OSC, 4=OSC ESC, 5=ST string, 6=ST ESC
    var csi_first = 0
    var csi_second = 0
    var csi_field = 0
    var csi_has_first = false
    var csi_has_second = false
    var cursor_row = 1
    var cursor_column = 1
    var utf8_codepoint = 0u
    var utf8_minimum = 0u
    var utf8_remaining = 0
    peek_data(value) $(bytes) { // nolint:STYLE037
        for (index in range(length(value))) {
            let byte = int(bytes[index])
            if (state == 0) {
                if (byte == 0x1b) {
                    state = 1
                }
                elif (byte == '\t' || byte == '\r' || byte == '\n' || byte >= 0x20) {
                    output |> push(uint8(byte))
                    if (byte == '\r') {
                        cursor_column = 1
                        utf8_remaining = 0
                    } elif (byte == '\n') {
                        cursor_row++
                        cursor_column = 1
                        utf8_remaining = 0
                    } elif (byte < 0x80) {
                        cursor_column++
                        utf8_remaining = 0
                    } elif (utf8_remaining > 0 && (byte & 0xc0) == 0x80) {
                        utf8_codepoint = (utf8_codepoint << 6u) | uint(byte & 0x3f)
                        utf8_remaining--
                        if (utf8_remaining == 0) {
                            let valid = (utf8_codepoint >= utf8_minimum &&
                                utf8_codepoint <= 0x10ffffu &&
                                !(utf8_codepoint >= 0xd800u && utf8_codepoint <= 0xdfffu))
                            cursor_column += terminal_codepoint_width(
                                valid ? utf8_codepoint : 0xfffdu)
                        }
                    } else {
                        if (utf8_remaining > 0) cursor_column++
                        if ((byte & 0xe0) == 0xc0) {
                            utf8_codepoint = uint(byte & 0x1f)
                            utf8_minimum = 0x80u
                            utf8_remaining = 1
                        } elif ((byte & 0xf0) == 0xe0) {
                            utf8_codepoint = uint(byte & 0x0f)
                            utf8_minimum = 0x800u
                            utf8_remaining = 2
                        } elif ((byte & 0xf8) == 0xf0) {
                            utf8_codepoint = uint(byte & 0x07)
                            utf8_minimum = 0x10000u
                            utf8_remaining = 3
                        } else {
                            cursor_column++
                            utf8_remaining = 0
                        }
                    }
                }
            } elif (state == 1) {
                if (byte == '[') {
                    state = 2
                    csi_first = 0
                    csi_second = 0
                    csi_field = 0
                    csi_has_first = false
                    csi_has_second = false
                } elif (byte == ']') {
                    state = 3
                } elif (byte == 'P' || byte == '^' || byte == '_' || byte == 'X') {
                    state = 5
                } else {
                    state = 0
                }
            } elif (state == 2) {
                if (is_number(byte)) {
                    if (csi_field == 0) {
                        csi_first = min(4096, csi_first * 10 + byte - '0')
                        csi_has_first = true
                    } else {
                        csi_second = min(4096, csi_second * 10 + byte - '0')
                        csi_has_second = true
                    }
                } elif (byte == ';') {
                    csi_field = 1
                } elif (byte >= 0x40 && byte <= 0x7e) {
                    // ConPTY expands horizontal tabs emitted by console
                    // applications into CSI cursor commands. Preserve their
                    // field and record-separating effect for machine consumers.
                    if (byte == 'C') {
                        let spaces = csi_has_first ? max(1, csi_first) : 1
                        for (_ in range(spaces)) {
                            output |> push(uint8(' '))
                        }
                        cursor_column += spaces
                    } elif (byte == 'G') {
                        let target_column = csi_has_first ? max(1, csi_first) : 1
                        while (cursor_column < target_column) {
                            output |> push(uint8(' '))
                            cursor_column++
                        }
                    } elif (byte == 'H' || byte == 'f') {
                        let target_row = csi_has_first ? max(1, csi_first) : 1
                        let target_column = csi_has_second ? max(1, csi_second) : 1
                        while (cursor_row < target_row) {
                            output |> push(uint8('\n'))
                            cursor_row++
                            cursor_column = 1
                        }
                        while (cursor_column < target_column) {
                            output |> push(uint8(' '))
                            cursor_column++
                        }
                    }
                    state = 0
                }
            } elif (state == 3) {
                if (byte == 0x07) {
                    state = 0
                } elif (byte == 0x1b) {
                    state = 4
                }
            } elif (state == 4) {
                state = byte == '\\' ? 0 : 3
            } elif (state == 5) {
                if (byte == 0x1b) {
                    state = 6
                }
            } else {
                state = byte == '\\' ? 0 : 5
            }
        }
    }
    return string(output)
}

def public watcher_machine_output_text(session_id : string;
                                       var truncated : bool&) : string {
    truncated = false
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return ""
    let session = g_sessions[index]
    truncated = session.raw_history_start != 0l
    return watcher_strip_terminal_controls(string(session.raw_history))
}

def public watcher_terminal_text(session_id : string; var scrollback_rows : int) : string {
    scrollback_rows = 0
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return ""
    var viewport = TerminalViewportSnapshot()
    terminal_viewport_snapshot(g_sessions[index].terminal, 0, viewport)
    scrollback_rows = viewport.buffer.scrollback_rows
    let result = viewport.screen_text
    terminal_viewport_snapshot_dispose(viewport)
    return result
}

def public watcher_compact_session(session_id : string;
                                   columns : int = 160; rows : int = 50) : bool {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return false
    var session = g_sessions[index]
    if (!session.drained || session.state == "running" || session.state == "terminating") return false
    let compact_columns = clamp(columns, 20, 400)
    let compact_rows = clamp(rows, 5, 512)
    if (session.terminal.transport == null && session.terminal.columns == compact_columns
            && session.terminal.rows == compact_rows) return true
    var inscope compact <- terminal_create(compact_columns, compact_rows)
    terminal_feed_bytes(compact, session.raw_history)
    terminal_dispose(session.terminal)
    session.terminal := compact
    session.status_revision++
    append_event(session, "terminal_compacted", "{compact_columns}x{compact_rows}")
    return true
}

def public watcher_session_artifact_path(session_id : string; name : string) : string {
    if (empty(name) || name == "." || name == ".." || find(name, "/") >= 0
            || find(name, "\\") >= 0 || find(name, ":") >= 0) return ""
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return ""
    return path_join(g_sessions[index].session_dir, name)
}

// A capture path that exists before its producing task session does.
def public watcher_scratch_artifact_path(name : string) : string {
    if (empty(g_log_root) || empty(name) || name == "." || name == ".."
            || find(name, "/") >= 0 || find(name, "\\") >= 0
            || find(name, ":") >= 0) return ""
    let scratch_dir = path_join(g_log_root, "_capture")
    var mkdir_error : string
    if (!mkdir_rec(scratch_dir, mkdir_error)) return ""
    return path_join(scratch_dir, name)
}

def public watcher_prune_completed_sessions(kind : string;
                                            repository_id : string;
                                            worktree_path : string) : int {
    var removed = 0
    var index = length(g_sessions)
    while (index > 0) {
        index--
        var session = g_sessions[index]
        if (session == null || !session.drained || session.state == "running"
                || session.state == "terminating" || session.kind != kind
                || session.repository_id != repository_id
                || session.worktree_path != worktree_path) continue
        unsafe { delete session }
        g_sessions |> erase(index)
        removed++
    }
    return removed
}

def public watcher_kind_running(kind : string) : bool {
    //! True while any session of this kind is still producing output. The
    //! index-lock guard between the single-flight file action and the
    //! operation rail queries THIS instead of keeping a flag that orphaned
    //! sessions would leave stuck.
    for (session in g_sessions) {
        if (session != null && session.kind == kind && !session.drained) {
            return true
        }
    }
    return false
}

def private input_session(var session : WatcherSession?; text : string; client_id : int) : string {
    if (session.controller_id != 0 && session.controller_id != client_id) return "controller lease is held by another client"
    if (session.state != "running" || session.drained) return "session is not running"
    if (!session_write_text(session, text)) {
        let error = session_transport_error(session)
        return empty(error) ? "input write failed" : error
    }
    session.input_bytes += long_length(text)
    return ""
}

def private powershell_literal(value : string) : string {
    return "'" + replace(value, "'", "''") + "'"
}

let private DASHERD_LOCAL_MARKER = "<!-- generated by dasHerd; edits will be overwritten -->"

def private agent_local_instructions(request : WatcherLaunchRequest const) : string {
    return ("{DASHERD_LOCAL_MARKER}\n\n# dasHerd\n\n" +
        "This worktree hosts dasHerd-managed agent sessions. When the " +
        "DASHERD_SESSION_ID environment variable is set, this session is one of " +
        "them: read the session context markdown at DASHERD_CONTEXT_PATH, and " +
        "use the dasHerd skill at `{request.agent_skill_path}` for coordination " +
        "— the inbox/outbox mailbox, declaring participating repositories, and " +
        "Review Bundles. CLI: `utils/dasHerd/dasherd.ps1` " +
        "(whoami / inbox / outbox / bundle / repository).\n")
}

// Keep the dropped file out of git status without touching tracked files:
// register it in the worktree's own info/exclude (per-worktree for linked
// worktrees, .git/info/exclude for a main checkout). Best-effort.
def private ensure_git_exclude(worktree_path, pattern : string;
                               var detail : string&) {
    let git_marker = path_join(worktree_path, ".git")
    let marker_stat = stat(git_marker)
    if (!marker_stat.is_valid) return
    var git_dir = git_marker
    if (!marker_stat.is_dir) {
        let content = strip(fread(git_marker))
        if (!starts_with(content, "gitdir:")) return
        git_dir = strip(slice(content, length("gitdir:")))
        if (!is_absolute(git_dir)) {
            git_dir = path_join(worktree_path, git_dir)
        }
    }
    let exclude_path = path_join(path_join(git_dir, "info"), "exclude")
    let existing = fexist(exclude_path) ? fread(exclude_path) : ""
    for (line in split(existing, "\n")) {
        if (strip(line) == pattern) return
    }
    var mkdir_error : string
    if (!mkdir_rec(dir_name(exclude_path), mkdir_error)) {
        detail += "; exclude=mkdir_failed"
        return
    }
    let payload = ((empty(existing) || ends_with(existing, "\n"))
        ? existing + pattern + "\n" : existing + "\n" + pattern + "\n")
    if (!write_text_file(exclude_path, payload)) {
        detail += "; exclude=write_failed"
    }
}

// Passive discovery for agents that read CLAUDE.local.md: a marker-guarded
// pointer at the dasHerd contract, written into the session worktree at agent
// launch. A hand-written file without our marker is never touched.
def private ensure_agent_local_instructions(request : WatcherLaunchRequest const;
                                            var detail : string&) {
    // A launch without a cwd would resolve the drop into the WATCHER's own
    // working directory — skip rather than litter it.
    if (empty(request.cwd)) {
        detail += "; local_md=skipped_no_cwd"
        return
    }
    let local_path = path_join(request.cwd, "CLAUDE.local.md")
    let existing = fexist(local_path) ? fread(local_path) : ""
    if (!empty(existing) && find(existing, DASHERD_LOCAL_MARKER) < 0) {
        detail += "; local_md=kept_user_file"
        return
    }
    let desired = agent_local_instructions(request)
    if (existing != desired && !write_text_file(local_path, desired)) {
        detail += "; local_md=write_failed"
        return
    }
    detail += "; local_md={local_path}"
    ensure_git_exclude(request.cwd, "CLAUDE.local.md", detail)
}

def private agent_context_markdown(request : WatcherLaunchRequest const;
                                   session_id, context_path : string) : string {
    return ("# dasHerd session\n\n" +
        "Session: `{session_id}`\n\n" +
        "Purpose: `{request.purpose}`\n\n" +
        "Origin repository: `{request.repository_id}`\n\n" +
        "Origin worktree: `{request.worktree_path}`\n\n" +
        "## Task\n\n{request.task_context}\n\n" +
        "## Cooperation contract\n\n" +
        "This is a dasHerd-managed session. Keep the launch worktree as the immutable " +
        "session origin. Use the dasHerd skill and CLI to read Inbox requests, declare " +
        "every additional repository/worktree participating in the task, publish exact " +
        "review focus, and sync the complete review bundle before declaring the work ready. " +
        "Do not attribute unrelated dirty files to this session.\n\n" +
        "Skill: `{request.agent_skill_path}`\n\n" +
        "Context artifact: `{context_path}`\n")
}

def private agent_launch_argv(request : WatcherLaunchRequest const;
                              session_id, context_path : string) : array<string> {
    var command_argv := request.argv
    if (empty(command_argv)) command_argv |> push("codex")
    // A resume relaunch runs the profile's resume argv verbatim: the agent CLI
    // restores its own conversation, so re-sending the prompt would duplicate it.
    if (!request.resume) {
        var prompt = strip(request.intro_text)
        if (!empty(strip(request.task_context))) {
            if (!empty(prompt)) {
                prompt += "\n\n"
            }
            prompt += ("Your assigned debug task is:\n\n{request.task_context}\n\n" +
                "Read the dasHerd session context at {context_path} before acting. " +
                "Use the dasHerd skill for session coordination, then begin the assigned task.")
        }
        if (!empty(prompt)) {
            command_argv |> push(prompt)
        }
    }
    if (get_platform_name() == "windows") {
        var command = ("$env:DASHERD_URL = {powershell_literal(request.agent_url)}; " +
            "$env:DASHERD_TOKEN = {powershell_literal(request.agent_token)}; " +
            "$env:DASHERD_SESSION_ID = {powershell_literal(session_id)}; " +
            "$env:DASHERD_SESSION_KIND = {powershell_literal(request.purpose)}; " +
            "$env:DASHERD_HERD_SESSION_ID = {powershell_literal(request.herd_session_id)}; " +
            "$env:DASHERD_CONTEXT_PATH = {powershell_literal(context_path)}; & ")
        var quoted : array<string>
        quoted |> reserve(length(command_argv))
        for (argument in command_argv) {
            quoted |> push(powershell_literal(argument))
        }
        command += join(quoted, " ") + "; exit $LASTEXITCODE"
        return <- ["powershell.exe", "-NoLogo", "-NoProfile", "-Command", command]
    }
    var result <- ["env", "DASHERD_URL={request.agent_url}",
        "DASHERD_TOKEN={request.agent_token}", "DASHERD_SESSION_ID={session_id}",
        "DASHERD_SESSION_KIND={request.purpose}",
        "DASHERD_HERD_SESSION_ID={request.herd_session_id}",
        "DASHERD_CONTEXT_PATH={context_path}"]
    result |> push_from(command_argv)
    return <- result
}

def private observe_input_line(var session : WatcherSession?; text : string) {
    peek_data(text) $(bytes) {
        for (byte_u8 in bytes) {
            let byte = int(byte_u8)
            if (session.input_escape_state == 1) {
                session.input_escape_state = (byte == '[' || byte == 'O') ? 2 : 0
            } elif (session.input_escape_state == 2) {
                if (byte >= 0x40 && byte <= 0x7e) {
                    session.input_escape_state = 0
                }
            } elif (byte == 0x1b) {
                session.input_escape_state = 1
            } elif (byte == '\r' || byte == '\n') {
                session.input_line_bytes = 0
            } elif (byte == 0x03 || byte == 0x15) {
                session.input_line_bytes = 0
            } elif (byte == 0x08 || byte == 0x7f) {
                session.input_line_bytes = max(0, session.input_line_bytes - 1)
            } elif (byte >= 0x20) {
                session.input_line_bytes++
            }
        }
    }
}

def private flush_pending_notifications(var session : WatcherSession?) {
    if (session.input_line_bytes != 0 || empty(session.pending_notifications)) return
    var delivered = 0
    while (delivered < length(session.pending_notifications)) {
        let pending & = unsafe(session.pending_notifications[delivered])
        if (!session_write_text(session, pending.text)) break
        session.input_bytes += long_length(pending.text)
        if (!empty(pending.message_id)) {
            let _ = watcher_mailbox_set_notification(session.id, pending.message_id)
            append_event(session, "mailbox_notified",
                "id={pending.message_id}; delivery=deferred")
        }
        delivered++
    }
    if (delivered > 0) {
        session.pending_notifications |> erase(0, delivered)
    }
}

def public watcher_input(session_id : string; text : string; client_id : int = 0) : string {
    let index = watcher_find_session(session_id)
    if (index < 0) return "unknown session"
    if (length(text) > MAX_INPUT_BYTES) return "input is too large"
    let error = input_session(g_sessions[index], text, client_id)
    if (!empty(error)) return error
    observe_input_line(g_sessions[index], text)
    flush_pending_notifications(g_sessions[index])
    return ""
}

def public watcher_notify(session_id : string; text : string;
                          message_id : string = "") : string {
    //! A mailbox wake-up is one serialized terminal write. It deliberately
    //! bypasses controller ownership: the controller is UI transport state,
    //! not an authorization boundary for Herder's trusted local mailbox.
    let index = watcher_find_session(session_id)
    if (index < 0) return "unknown session"
    if (length(text) > MAX_INPUT_BYTES) return "input is too large"
    if (g_sessions[index].input_line_bytes != 0) {
        var pending = WatcherPendingNotification(
            message_id = message_id, text = text)
        g_sessions[index].pending_notifications |> emplace(pending)
        if (!empty(message_id)) {
            let _ = watcher_mailbox_set_notification(
                session_id, message_id, "queued")
            append_event(g_sessions[index], "mailbox_notification_queued",
                "id={message_id}; partial_input_bytes={g_sessions[index].input_line_bytes}")
        } else {
            append_event(g_sessions[index], "terminal_notification_queued",
                "partial_input_bytes={g_sessions[index].input_line_bytes}")
        }
        return "queued"
    }
    let controller_id = g_sessions[index].controller_id
    return input_session(g_sessions[index], text, controller_id)
}

def private valid_focus(focus : WatcherFocusSet const) : string {
    for (target in focus.targets) {
        if (empty(target.file_path)) return "focus target file_path is required"
        if (empty(target.repository_id) && empty(focus.repository_id)) {
            return "focus target repository_id is required"
        }
        if (empty(target.worktree_path) && empty(focus.worktree_path)) {
            return "focus target worktree_path is required"
        }
        for (focus_range in target.ranges) {
            if (focus_range.start_byte < 0
                    || focus_range.end_byte <= focus_range.start_byte) {
                return "focus ranges require non-negative, increasing byte offsets"
            }
        }
    }
    return ""
}

def public watcher_focus_repository(focus : WatcherFocusSet const;
                                    target : WatcherFocusTarget const) : string {
    return !empty(target.repository_id) ? target.repository_id : focus.repository_id
}

def public watcher_focus_worktree(focus : WatcherFocusSet const;
                                  target : WatcherFocusTarget const) : string {
    return !empty(target.worktree_path) ? target.worktree_path : focus.worktree_path
}

def public watcher_focus_comparison(focus : WatcherFocusSet const;
                                    target : WatcherFocusTarget const) : string {
    return !empty(target.comparison) ? target.comparison : focus.comparison
}

def public watcher_focus_revision(focus : WatcherFocusSet const;
                                  target : WatcherFocusTarget const) : string {
    return !empty(target.revision) ? target.revision : focus.revision
}

def private append_mailbox_record(var session : WatcherSession?;
                                  message : WatcherMailboxMessage const) {
    if (session.mailbox_file == null) return
    fwrite(session.mailbox_file, sprint_json(message, false) + "\n")
    fflush(session.mailbox_file)
}

def public watcher_mailbox_unread_outbox(session_id : string) : int {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return 0
    var count = 0
    for (message in g_sessions[index].mailbox) {
        if (message.direction == "outbox" && message.status == "new") count++
    }
    return count
}

def public watcher_mailbox_session_revision(session_id : string) : int64 {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return -1l
    return g_sessions[index].mailbox_revision
}

def public watcher_mailbox_post(request : WatcherMailboxPostRequest) : WatcherMailboxResult {
    let index = watcher_find_session(request.session_id)
    if (index < 0 || g_sessions[index] == null) {
        return WatcherMailboxResult(error = "unknown session")
    }
    if (request.direction != "inbox" && request.direction != "outbox") {
        return WatcherMailboxResult(error = "direction must be inbox or outbox")
    }
    if (empty(request.subject)) return WatcherMailboxResult(error = "subject is required")
    let focus_error = valid_focus(request.focus)
    if (!empty(focus_error)) return WatcherMailboxResult(error = focus_error)
    let sender = (request.direction == "outbox"
        ? "agent_session:{request.session_id}"
        : "human:local")
    g_next_mailbox_id++
    g_mailbox_revision++
    var message = WatcherMailboxMessage(
        id = "hf_{g_next_mailbox_id}", session_id = request.session_id,
        direction = request.direction, sender = sender,
        kind = request.kind, subject = request.subject,
        reply_to = request.reply_to, bundle_id = request.bundle_id,
        created_ms = elapsed_ms(g_sessions[index]),
        revision = g_mailbox_revision, focus := request.focus)
    let message_id = message.id
    let message_revision = message.revision
    g_sessions[index].mailbox |> emplace(message)
    g_sessions[index].mailbox_revision = g_mailbox_revision
    append_mailbox_record(g_sessions[index],
        g_sessions[index].mailbox[length(g_sessions[index].mailbox) - 1])
    append_event(g_sessions[index], "mailbox_posted",
        "id={message_id}; direction={request.direction}; sender={sender}")
    return WatcherMailboxResult(ok = true, message_id = message_id,
        revision = message_revision)
}

def public watcher_mailbox_set_notification(session_id, message_id : string;
                                            error : string = "") : WatcherMailboxResult {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) {
        return WatcherMailboxResult(error = "unknown session")
    }
    var session = g_sessions[index]
    for (message_index in range(length(session.mailbox))) {
        if (session.mailbox[message_index].id != message_id) continue
        g_mailbox_revision++
        session.mailbox[message_index].notified = empty(error)
        session.mailbox[message_index].notification_error = error
        session.mailbox[message_index].revision = g_mailbox_revision
        session.mailbox_revision = g_mailbox_revision
        append_mailbox_record(session, session.mailbox[message_index])
        return WatcherMailboxResult(ok = true, message_id = message_id,
            revision = g_mailbox_revision)
    }
    return WatcherMailboxResult(error = "unknown mailbox message")
}

def public watcher_mailbox_mark_fetched(session_id, message_id : string) : WatcherMailboxResult {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) {
        return WatcherMailboxResult(error = "unknown session")
    }
    var session = g_sessions[index]
    for (message_index in range(length(session.mailbox))) {
        if (session.mailbox[message_index].id != message_id) continue
        if (session.mailbox[message_index].fetched) {
            return WatcherMailboxResult(ok = true, message_id = message_id,
                revision = session.mailbox[message_index].revision)
        }
        g_mailbox_revision++
        session.mailbox[message_index].fetched = true
        session.mailbox[message_index].revision = g_mailbox_revision
        session.mailbox_revision = g_mailbox_revision
        append_mailbox_record(session, session.mailbox[message_index])
        append_event(session, "mailbox_fetched", "id={message_id}")
        return WatcherMailboxResult(ok = true, message_id = message_id,
            revision = g_mailbox_revision)
    }
    return WatcherMailboxResult(error = "unknown mailbox message")
}

def public watcher_mailbox_set_state(request : WatcherMailboxStateRequest) : WatcherMailboxResult {
    let index = watcher_find_session(request.session_id)
    if (index < 0 || g_sessions[index] == null) {
        return WatcherMailboxResult(error = "unknown session")
    }
    if (request.status != "new" && request.status != "acknowledged"
            && request.status != "completed") {
        return WatcherMailboxResult(error = "status must be new, acknowledged, or completed")
    }
    var session = g_sessions[index]
    for (message_index in range(length(session.mailbox))) {
        if (session.mailbox[message_index].id != request.message_id) continue
        g_mailbox_revision++
        session.mailbox[message_index].status = request.status
        session.mailbox[message_index].revision = g_mailbox_revision
        session.mailbox_revision = g_mailbox_revision
        append_mailbox_record(session, session.mailbox[message_index])
        append_event(session, "mailbox_state",
            "id={request.message_id}; status={request.status}")
        return WatcherMailboxResult(ok = true,
            message_id = request.message_id, revision = g_mailbox_revision)
    }
    return WatcherMailboxResult(error = "unknown mailbox message")
}

def public watcher_mailbox_json(session_id : string; direction : string = "";
                                message_id : string = "") : string {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return "[]"
    return build_string() $(var writer) {
        writer |> write("[")
        var first = true
        for (message in g_sessions[index].mailbox) {
            if ((!empty(direction) && message.direction != direction)
                    || (!empty(message_id) && message.id != message_id)) continue
            if (!first) {
                writer |> write(",")
            }
            writer |> write(sprint_json(message, false))
            first = false
        }
        writer |> write("]")
    }
}

def private valid_bundle(bundle : WatcherReviewBundle const) : string {
    if (empty(bundle.title)) return "bundle title is required"
    if (bundle.status != "draft" && bundle.status != "ready"
            && bundle.status != "published" && bundle.status != "merged"
            && bundle.status != "complete") {
        return "bundle status must be draft, ready, published, merged, or complete"
    }
    if (empty(bundle.participants)) return "bundle requires at least one participant"
    for (participant in bundle.participants) {
        if (empty(participant.repository_id)) return "participant repository_id is required"
        if (empty(participant.worktree_path)) return "participant worktree_path is required"
        for (file in participant.files) {
            if (empty(file.path)) return "participant file path is required"
        }
    }
    let focus_error = valid_focus(bundle.focus)
    if (!empty(focus_error)) return focus_error
    for (target in bundle.focus.targets) {
        let repository_id = watcher_focus_repository(bundle.focus, target)
        let worktree_path = watcher_focus_worktree(bundle.focus, target)
        var participant_found = false
        for (participant in bundle.participants) {
            if (participant.repository_id == repository_id
                    && participant.worktree_path == worktree_path) {
                participant_found = true
                break
            }
        }
        if (!participant_found) return "focus target is not in a bundle participant"
    }
    return ""
}

def private append_bundle_record(var session : WatcherSession?;
                                 bundle : WatcherReviewBundle const) {
    if (session.bundles_file == null) return
    fwrite(session.bundles_file, sprint_json(bundle, false) + "\n")
    fflush(session.bundles_file)
}

def private bundle_publishes_attention(bundle : WatcherReviewBundle const) : bool {
    if (empty(bundle.focus.targets)) return false
    return (bundle.status == "ready" || bundle.status == "published"
        || bundle.status == "merged" || bundle.status == "complete"
    )
}

def private upsert_bundle_attention(var session : WatcherSession?;
                                    bundle : WatcherReviewBundle const;
                                    publish, reopen : bool) {
    for (message_index in range(length(session.mailbox))) {
        if (session.mailbox[message_index].direction != "outbox"
                || session.mailbox[message_index].kind != "bundle_focus"
                || session.mailbox[message_index].bundle_id != bundle.id) continue
        g_mailbox_revision++
        session.mailbox[message_index].subject = bundle.title
        session.mailbox[message_index].focus := bundle.focus
        if (!publish) {
            session.mailbox[message_index].status = "completed"
        } elif (reopen) {
            session.mailbox[message_index].status = "new"
        }
        session.mailbox[message_index].revision = g_mailbox_revision
        session.mailbox_revision = g_mailbox_revision
        append_mailbox_record(session, session.mailbox[message_index])
        append_event(session, publish ? "bundle_attention_updated"
                : "bundle_attention_completed",
            "bundle={bundle.id}; message={session.mailbox[message_index].id}; " +
            "targets={length(bundle.focus.targets)}")
        return
    }
    if (!publish || empty(bundle.focus.targets)) return
    let sender = "agent_session:{session.id}"
    g_next_mailbox_id++
    g_mailbox_revision++
    var message = WatcherMailboxMessage(
        id = "hf_{g_next_mailbox_id}", session_id = session.id,
        direction = "outbox", sender = sender, kind = "bundle_focus",
        subject = bundle.title, bundle_id = bundle.id,
        created_ms = elapsed_ms(session), revision = g_mailbox_revision,
        focus := bundle.focus)
    session.mailbox |> emplace(message)
    session.mailbox_revision = g_mailbox_revision
    append_mailbox_record(session, session.mailbox[length(session.mailbox) - 1])
    append_event(session, "bundle_attention_created",
        "bundle={bundle.id}; message={session.mailbox[length(session.mailbox) - 1].id}; " +
        "targets={length(bundle.focus.targets)}")
}

def public watcher_bundle_session_revision(session_id : string) : int64 {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return -1l
    return g_sessions[index].bundles_revision
}

def public watcher_bundle_sync(request : WatcherBundleSyncRequest) : WatcherBundleResult {
    let index = watcher_find_session(request.session_id)
    if (index < 0 || g_sessions[index] == null) {
        return WatcherBundleResult(error = "unknown session")
    }
    let validation_error = valid_bundle(request.bundle)
    if (!empty(validation_error)) return WatcherBundleResult(error = validation_error)
    var bundle := request.bundle
    if (empty(bundle.id)) {
        g_next_bundle_id++
        bundle.id = "rb_{g_next_bundle_id}"
    }
    bundle.session_id = request.session_id
    g_bundle_revision++
    bundle.revision = g_bundle_revision
    bundle.updated_ms = elapsed_ms(g_sessions[index])
    let bundle_id = bundle.id
    var replaced = false
    var previously_published_attention = false
    for (bundle_index in range(length(g_sessions[index].bundles))) {
        if (g_sessions[index].bundles[bundle_index].id != bundle_id) continue
        previously_published_attention = bundle_publishes_attention(
            g_sessions[index].bundles[bundle_index])
        g_sessions[index].bundles[bundle_index] := bundle
        replaced = true
        break
    }
    if (!replaced) g_sessions[index].bundles |> emplace(bundle)
    g_sessions[index].bundles_revision = g_bundle_revision
    var stored_index = 0
    for (bundle_index in range(length(g_sessions[index].bundles))) {
        if (g_sessions[index].bundles[bundle_index].id == bundle_id) {
            stored_index = bundle_index
            break
        }
    }
    append_bundle_record(g_sessions[index], g_sessions[index].bundles[stored_index])
    let stored_status = g_sessions[index].bundles[stored_index].status
    let publish_attention = bundle_publishes_attention(
        g_sessions[index].bundles[stored_index])
    upsert_bundle_attention(g_sessions[index],
        g_sessions[index].bundles[stored_index], publish_attention,
        publish_attention && !previously_published_attention)
    append_event(g_sessions[index], "bundle_synced",
        "id={bundle_id}; status={stored_status}; participants={length(g_sessions[index].bundles[stored_index].participants)}")
    return WatcherBundleResult(ok = true, bundle_id = bundle_id,
        revision = g_bundle_revision)
}

def public watcher_bundles_json(session_id : string) : string {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return "[]"
    return sprint_json(g_sessions[index].bundles, false)
}

def private resize_session(var session : WatcherSession?; columns, rows, client_id : int) : string {
    if (session.controller_id != 0 && session.controller_id != client_id) return "controller lease is held by another client"
    terminal_resize(session.terminal, clamp(columns, 20, 400), clamp(rows, 5, 512))
    if (session.host != null) {
        let _ = session.host->queue_or_send(("\{\"op\":\"resize\"," +
            "\"columns\":{session.terminal.columns},\"rows\":{session.terminal.rows}\}"))
    }
    append_event(session, "resized", "{session.terminal.columns}x{session.terminal.rows}")
    return ""
}

def public watcher_resize(session_id : string; columns : int; rows : int;
                          client_id : int) : string {
    let index = watcher_find_session(session_id)
    if (index < 0) return "unknown session"
    return resize_session(g_sessions[index], columns, rows, client_id)
}

def private terminate_session(var session : WatcherSession?; detail : string) : string {
    if ((session.state != "running" && session.state != "terminating")
            || session_process_exited(session)) return ""
    session.reason = detail
    session.state = "terminating"
    session.status_revision++
    if (session.host != null) {
        if (!session.host->queue_or_send("\{\"op\":\"terminate\",\"exit_code\":1\}")) {
            return "host connection lost"
        }
    } elif (!terminal_terminate(session.terminal)) {
        return terminal_transport_error(session.terminal)
    }
    append_event(session, "terminate_requested", detail)
    return ""
}

def public watcher_terminate(session_id : string; detail : string = "requested") : string {
    let index = watcher_find_session(session_id)
    if (index < 0) return "unknown session"
    return terminate_session(g_sessions[index], detail)
}

def private claim_session(var session : WatcherSession?; client_id : int) : bool {
    if (session.controller_id != 0 && session.controller_id != client_id) {
        append_event(session, "controller_claim_rejected",
            "client={client_id}; held_by={session.controller_id}")
        return false
    }
    let newly_claimed = session.controller_id == 0
    session.controller_id = client_id
    session.status_revision++
    if (newly_claimed) append_event(session, "controller_claimed", "client={client_id}")
    return true
}

def public watcher_claim_controller(session_id : string; client_id : int) : bool {
    let index = watcher_find_session(session_id)
    if (index < 0 || client_id == 0) return false
    return claim_session(g_sessions[index], client_id)
}

def private release_session(var session : WatcherSession?; client_id : int; detail : string) {
    if (session.controller_id != client_id) return
    session.controller_id = 0
    session.status_revision++
    append_event(session, "controller_released", "client={client_id}; reason={detail}")
}

def public watcher_release_controller(session_id : string; client_id : int; detail : string = "released") {
    let index = watcher_find_session(session_id)
    if (index < 0) return
    release_session(g_sessions[index], client_id, detail)
}

def public watcher_list_json : string {
    var inscope result : array<WatcherSessionInfo>
    result |> reserve(length(g_sessions))
    for (session in g_sessions) {
        if (session != null) result |> push(session_info(session))
    }
    return sprint_json(result, false)
}

def private snapshot_session_json(session : WatcherSession?) : string {
    var viewport = TerminalViewportSnapshot()
    terminal_viewport_snapshot(session.terminal, 0, viewport)
    let cursor = viewport.buffer.cursor
    let snapshot = WatcherSessionSnapshot(session = session_info(session),
        screen_text = viewport.screen_text, title = session.terminal.title,
        current_directory = session.terminal.current_directory,
        scrollback_rows = viewport.buffer.scrollback_rows,
        cursor_row = cursor.row, cursor_column = cursor.column,
        cursor_visible = cursor.visible, cursor_blinking = cursor.blinking)
    let result = sprint_json(snapshot, false)
    terminal_viewport_snapshot_dispose(viewport)
    return result
}

def public watcher_snapshot_json(session_id : string) : string {
    let index = watcher_find_session(session_id)
    if (index < 0) return ""
    return snapshot_session_json(g_sessions[index])
}

def public watcher_update_key(session_id : string) : string {
    let index = watcher_find_session(session_id)
    if (index < 0 || g_sessions[index] == null) return ""
    let session = g_sessions[index]
    return "{terminal_revision(session.terminal)}:{session.status_revision}"
}

def public watcher_shutdown(terminate_running : bool = false) {
    for (session in g_sessions) {
        if (session == null) continue
        if (session.host != null) {
            // Default shutdown leaves hosts (and their children) running -
            // that survival is the entire point of the detached architecture.
            if (terminate_running && !session.host.closed) {
                let _t = session.host->queue_or_send("\{\"op\":\"terminate\",\"exit_code\":1\}")
                let _s = session.host->queue_or_send("\{\"op\":\"shutdown\"\}")
                session.host->process_event_que()
            }
        } elif (terminate_running && !terminal_process_exited(session.terminal)) {
            let _ = terminal_terminate(session.terminal)
        }
        if (session.output_file != null) {
            fflush(session.output_file)
            fclose(session.output_file)
            session.output_file = null
        }
        if (session.events_file != null) {
            append_event(session, "watcher_shutdown", "terminate_running={terminate_running}")
            fflush(session.events_file)
            fclose(session.events_file)
            session.events_file = null
        }
        unsafe { delete session }
    }
    g_sessions |> clear()
    unsafe { delete g_sessions }
}

def public option_value(prefix : string; fallback : string = "") : string {
    for (argument in get_command_line_arguments()) {
        if (starts_with(argument, prefix)) return slice(argument, length(prefix))
    }
    return fallback
}
