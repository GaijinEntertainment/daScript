options gen2
options rtti
options persistent_heap
options gc
options indenting = 4

// dasherd-ptyhost: a terminal that depends on nothing (PTY_HOST_DESIGN.md).
// Owns exactly one ConPTY + child + journal and serves protocol v1 over a
// loopback WebSocket. No watcher, no client, no UI - callers are disposable.
// This file stays boring on purpose: a crash here kills a live session.

require dashv/dashv_boost
require terminal/terminal
require daslib/clargs
require daslib/fio
require daslib/json_boost
require daslib/base64
require daslib/strings_boost
require strings
require math

let PROTOCOL_VERSION = 1
let JOURNAL_CHUNK = 65536
let STAMP_PERIOD_MS = 1000l

[CommandLineArgs]
struct HostConfig {
    @clarg_required
    @clarg_doc = "Session id this host serves"
    session : string

    @clarg_required
    @clarg_doc = "Loopback port to serve protocol v1 on"
    port : int

    @clarg_required
    @clarg_doc = "Directory for the journal and the discovery stamp"
    journal : string

    @clarg_doc = "Child working directory"
    cwd : string

    @clarg_doc = "Initial terminal columns (default 120)"
    columns : int

    @clarg_doc = "Initial terminal rows (default 36)"
    rows : int

    @clarg_doc = "How long to keep serving after the child exits, ms (default 24h)"
    linger_ms : int

    @clarg_doc = "Child argv token, repeatable and ordered"
    child : array<string>

    @clarg_doc = "Show this help and exit"
    help : bool
}

struct HostStamp {
    version : int = PROTOCOL_VERSION
    session : string
    port : int
    host_pid : int
    child_pid : int
    started_wall_s : int64
    updated_wall_s : int64
    journal_bytes : int64
    exited : bool
    exit_code : int64 = -1l
    drained : bool
    error : string
}

var private g_stamp = HostStamp()
var private g_stamp_path : string
var private g_journal_path : string
var private g_journal : file
var private g_shutdown_requested = false
var private g_child_exit_wall_ms : int64
// The one terminal this host exists for (smart_ptr; released at exit).
var private g_pty : Pty

def private wall_ms : int64 => int64(get_clock()) * 1000l

def private save_stamp {
    // Same durability boundary as the herd registry: write the temp fully,
    // then remove + rename (Windows _wrename cannot replace in place).
    g_stamp.updated_wall_s = int64(get_clock())
    let temp_path = "{g_stamp_path}.tmp"
    if (!fwrite(temp_path, sprint_json(g_stamp, true))
            || (fexist(g_stamp_path) && !remove(g_stamp_path))) return
    let _ = rename(temp_path, g_stamp_path)
}

struct private HostClient {
    @safe_when_uninitialized channel : WebSocketChannel
    authorized : bool
    attached : bool
    offset : int64
}

def private child_environment : array<string> {
    //! Child env rides host env vars, never any command line: the key list in
    //! DASHERD_CHILD_ENV_KEYS (';'-separated), each value in DASHERD_CHILD_<KEY>.
    var pairs : array<string>
    let keys = get_env_variable("DASHERD_CHILD_ENV_KEYS")
    if (empty(keys)) return <- pairs
    for (key in split(keys, ";")) {
        let name = strip(key)
        if (empty(name)) continue
        pairs |> push("{name}={get_env_variable("DASHERD_CHILD_" + name)}")
    }
    return <- pairs
}

class PtyHostServer : HvWebServer {
    token : string
    clients : array<HostClient?>

    def override onInit {}
    def override onTick {}

    def override onWsOpen(channel : WebSocketChannel; url : string#) {
        clients |> push(new HostClient(channel = channel))
    }

    def override onWsClose(channel : WebSocketChannel) {
        var index = 0
        while (index < length(clients)) {
            if (clients[index] != null && clients[index].channel == channel) {
                unsafe {
                    delete clients[index]
                }
                clients |> erase(index)
            } else {
                index++
            }
        }
    }

    def find_client(channel : WebSocketChannel) : int {
        for (index in range(length(clients))) {
            if (clients[index] != null && clients[index].channel == channel) {
                return index
            }
        }
        return -1
    }

    def send_error(channel : WebSocketChannel; text : string) {
        send(channel, "\{\"type\":\"error\",\"error\":{sprint_json(text, false)}\}",
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def send_stat(channel : WebSocketChannel) {
        send(channel, ("\{\"type\":\"stat\",\"protocol\":{PROTOCOL_VERSION}," +
            "\"session\":{sprint_json(g_stamp.session, false)}," +
            "\"child_pid\":{g_stamp.child_pid}," +
            "\"journal_bytes\":{g_stamp.journal_bytes}," +
            "\"exited\":{g_stamp.exited},\"exit_code\":{g_stamp.exit_code}," +
            "\"drained\":{g_stamp.drained}\}"),
            ws_opcode.WS_OPCODE_TEXT, true)
    }

    def stream_journal_from(var client : HostClient?) {
        //! Catch-up: everything already journaled from the requested offset,
        //! as binary frames. Live bytes append via broadcast_output after.
        if (client.offset >= g_stamp.journal_bytes) {
            client.offset = g_stamp.journal_bytes
            return
        }
        fopen(g_journal_path, "rb") $(reader) {
            if (reader == null) return
            var skipped = 0l
            var inscope chunk : array<uint8>
            chunk |> resize(JOURNAL_CHUNK)
            while (skipped < client.offset) {
                let remaining = client.offset - skipped
                let want = int(remaining > int64(JOURNAL_CHUNK)
                    ? int64(JOURNAL_CHUNK) : remaining)
                if (want < length(chunk)) chunk |> resize(want)
                let got = fread(reader, chunk)
                if (got <= 0) break
                skipped += int64(got)
                if (length(chunk) < JOURNAL_CHUNK) chunk |> resize(JOURNAL_CHUNK)
            }
            while (true) {
                let got = fread(reader, chunk)
                if (got <= 0) break
                if (got < length(chunk)) chunk |> resize(got)
                unsafe {
                    send(client.channel, reinterpret<string#>(addr(chunk[0])),
                        length(chunk), ws_opcode.WS_OPCODE_BINARY, true)
                }
                client.offset += int64(got)
                if (length(chunk) < JOURNAL_CHUNK) chunk |> resize(JOURNAL_CHUNK)
            }
        }
    }

    def broadcast_output(bytes : array<uint8>) {
        if (empty(bytes)) return
        for (client in clients) {
            if (client == null || !client.attached) continue
            unsafe {
                send(client.channel, reinterpret<string#>(addr(bytes[0])),
                    length(bytes), ws_opcode.WS_OPCODE_BINARY, true)
            }
            client.offset = g_stamp.journal_bytes
        }
    }

    def override onWsMessage(channel : WebSocketChannel; msg : string#) {
        var incoming = HostWsMessage()
        if (!sscan_json(string(msg), incoming)) {
            send_error(channel, "invalid message")
            return
        }
        let index = find_client(channel)
        if (index < 0) return
        if (incoming.op == "hello") {
            if (incoming.token != token) {
                send_error(channel, "bad token")
                return
            }
            if (incoming.protocol != PROTOCOL_VERSION) {
                send_error(channel, "protocol {incoming.protocol} unsupported; host speaks {PROTOCOL_VERSION}")
                return
            }
            clients[index].authorized = true
            send_stat(channel)
            return
        }
        if (!clients[index].authorized) {
            send_error(channel, "hello first")
            return
        }
        if (incoming.op == "attach") {
            clients[index].offset = max(0l, incoming.from_byte)
            clients[index].attached = true
            send(channel, "\{\"type\":\"attached\",\"from_byte\":{clients[index].offset}\}",
                ws_opcode.WS_OPCODE_TEXT, true)
            stream_journal_from(clients[index])
        } elif (incoming.op == "input") {
            if (g_pty == null || g_stamp.exited) {
                send_error(channel, "child is not running")
                return
            }
            var inscope bytes : array<uint8>
            if (base64_decode(incoming.base64, bytes) >= 0 && !empty(bytes)) {
                let _ = _pty_write(g_pty, bytes)
            }
        } elif (incoming.op == "resize") {
            if (g_pty != null && incoming.columns > 0 && incoming.rows > 0) {
                let _ = _pty_resize(g_pty, incoming.columns, incoming.rows)
            }
        } elif (incoming.op == "stat") {
            send_stat(channel)
        } elif (incoming.op == "terminate") {
            if (g_pty != null && !g_stamp.exited) {
                let _ = _pty_terminate(g_pty, int(incoming.exit_code))
            }
        } elif (incoming.op == "shutdown") {
            g_shutdown_requested = true
        } else {
            send_error(channel, "unknown operation")
        }
    }
}

struct HostWsMessage {
    op : string
    token : string
    protocol : int
    from_byte : int64
    base64 : string
    columns : int
    rows : int
    exit_code : int64
}

def private append_journal(bytes : array<uint8>) {
    if (g_journal == null || empty(bytes)) return
    let written = fwrite(g_journal, bytes)
    if (written > 0) {
        g_stamp.journal_bytes += int64(written)
        fflush(g_journal)
    }
}

[export]
def main : int {
    var parsed <- parse_args(type<HostConfig>)
    if (parsed |> is_err) {
        to_log(LOG_ERROR, "dasherd-ptyhost: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<HostConfig>), "dasherd-ptyhost")
        return 2
    }
    let config <- parsed |> move_unwrap
    if (config.help) {
        print_help(get_command_info(type<HostConfig>), "dasherd-ptyhost")
        return 0
    }
    if (empty(config.child)) {
        to_log(LOG_ERROR, "dasherd-ptyhost: --child argv is required\n")
        return 2
    }
    let token = get_env_variable("DASHERD_HOST_TOKEN")
    if (empty(token)) {
        to_log(LOG_ERROR, "dasherd-ptyhost: DASHERD_HOST_TOKEN is not set\n")
        return 2
    }
    let columns = config.columns > 0 ? config.columns : 120
    let rows = config.rows > 0 ? config.rows : 36
    let linger_ms = int64(config.linger_ms > 0 ? config.linger_ms : 24 * 3600 * 1000)

    var mkdir_error : string
    if (!mkdir_rec(config.journal, mkdir_error)) {
        to_log(LOG_ERROR, "dasherd-ptyhost: journal dir: {mkdir_error}\n")
        return 3
    }
    g_journal_path = path_join(config.journal, "{config.session}.raw")
    g_stamp_path = path_join(config.journal, "{config.session}.host.json")
    // Append: a relaunched host for the same session continues the stream.
    g_journal = fopen(g_journal_path, "ab")
    if (g_journal == null) {
        to_log(LOG_ERROR, "dasherd-ptyhost: could not open {g_journal_path}\n")
        return 3
    }
    g_stamp.session = config.session
    g_stamp.port = config.port
    // host_pid stays 0: das has no own-pid accessor yet, and liveness is
    // decided by connecting to the port, never by pid probing.
    g_stamp.started_wall_s = int64(get_clock())
    let journal_stat = stat(g_journal_path)
    if (journal_stat.is_valid) {
        g_stamp.journal_bytes = int64(journal_stat.size)
    }

    var inscope env <- child_environment()
    unsafe {
        g_pty <- _pty_launch_argv_env(config.child, config.cwd,
            length(config.cwd), env, columns, rows)
    }
    let launch_error = _pty_error(g_pty)
    if (_pty_process_id(g_pty) == 0) {
        g_stamp.exited = true
        g_stamp.drained = true
        g_stamp.error = (empty(launch_error)
            ? "child failed to launch" : launch_error)
        save_stamp()
        to_log(LOG_ERROR, "dasherd-ptyhost: launch failed: {g_stamp.error}\n")
        return 4
    }
    g_stamp.child_pid = _pty_process_id(g_pty)

    var server = new PtyHostServer(token = token)
    server->init(config.port)
    let _bind = server->set_bind_host("127.0.0.1")
    if (server->start() != 0) {
        g_stamp.error = "could not bind 127.0.0.1:{config.port}"
        g_stamp.exited = true
        save_stamp()
        let _ = _pty_terminate(g_pty, 1)
        to_log(LOG_ERROR, "dasherd-ptyhost: {g_stamp.error}\n")
        return 5
    }
    save_stamp()
    to_log(LOG_INFO, "dasherd-ptyhost: session {config.session} child {g_stamp.child_pid} on 127.0.0.1:{config.port}\n")

    var inscope chunk : array<uint8>
    var last_stamp = wall_ms()
    while (!g_shutdown_requested) {
        server->tick()
        var pumped = false
        if (!g_stamp.drained) {
            let status = unsafe(reinterpret<TerminalPollStatus>(
                _pty_read(g_pty, chunk, JOURNAL_CHUNK)))
            if (status == TerminalPollStatus.data) {
                pumped = true
                append_journal(chunk)
                server->broadcast_output(chunk)
            } elif (status == TerminalPollStatus.closed) {
                // Only pipe closure authoritatively ends the stream: the root
                // process can exit while descendants still hold the console.
                g_stamp.drained = true
            }
            if (!g_stamp.exited && _pty_process_exited(g_pty)) {
                g_stamp.exited = true
                g_stamp.exit_code = _pty_exit_code(g_pty)
                g_child_exit_wall_ms = wall_ms()
                save_stamp()
            }
        }
        if (g_stamp.exited && g_stamp.drained
                && wall_ms() - g_child_exit_wall_ms > linger_ms) {
            break
        }
        if (wall_ms() - last_stamp >= STAMP_PERIOD_MS) {
            last_stamp = wall_ms()
            save_stamp()
        }
        if (!pumped) sleep(1u)
    }

    if (!g_stamp.exited) {
        // Shutdown op with a live child: the child dies with the host's job.
        let _ = _pty_terminate(g_pty, 1)
        g_stamp.exited = true
        g_stamp.exit_code = _pty_exit_code(g_pty)
        g_stamp.drained = true
    }
    save_stamp()
    server->stop()
    server->cleanup()
    unsafe {
        delete server
    }
    unsafe {
        g_pty <- default<Pty>
    }
    fclose(g_journal)
    g_journal = null
    return 0
}
