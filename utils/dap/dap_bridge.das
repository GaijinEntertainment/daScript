options gen2
options indenting = 4
options rtti

require daslib/fio
require daslib/json
require daslib/json_boost
require daslib/network
require daslib/strings_boost
require daslib/strings_convert
require daslib/logger
require strings
require math

//! The DAP bridge: one MCP server that owns one connection to a daslang Debug Adapter Protocol
//! server and, for `debug_launch`, the daslang process it started. Every `debug_*` tool is a
//! DAP request or a wait on the event queue; the reply carries the DAP body. There is no
//! thread: the socket and the child's output are drained between MCP requests and while a
//! tool waits, so an event or an exit is never missed, only read a little later.

let public DEFAULT_HOST = "127.0.0.1"
let public DEFAULT_PORT = 10000
let public DEFAULT_TIMEOUT = 90.0
let private EVENT_LIMIT = 2000
let private OUTPUT_LIMIT = 200
let private HISTORY_LIMIT = 200
let public DAP_HEADER_LINE_LIMIT = 8192
let public DAP_HEADER_LIMIT = 65536
let public DAP_PAYLOAD_LIMIT = 64 * 1024 * 1024
let private PUMP_SLEEP_MS = 1u

// ===== time =====

//! seconds since a moment, on the monotonic tick clock - `get_clock` counts whole wall seconds
//! - re-based before the 32-bit microsecond count can wrap
struct private Stopwatch {
    started : int64
    banked : double
}

def private stopwatch() : Stopwatch {
    return Stopwatch(started = ref_time_ticks())
}

def private elapsed_sec(var sw : Stopwatch) : double {
    let usec = get_time_usec(sw.started)
    if (usec > 1000000000) {
        sw.banked += double(usec) / 1000000.0lf
        sw.started = ref_time_ticks()
        return sw.banked
    }
    return sw.banked + double(usec) / 1000000.0lf
}

// ===== frames =====

//! one parse of a Content-Length frame off a byte buffer: `message` with `consumed` bytes, or
//! `error` when the bytes break the protocol, or neither when more bytes are needed
struct public FrameParse {
    message : JsonValue?
    consumed : int
    error : string
}

def private bytes_string(data : array<uint8>; from, to : int) : string {
    var tmp : array<uint8>
    tmp |> reserve(to - from)
    for (i in range(from, to)) {
        tmp |> push(data[i])
    }
    return string(tmp)
}

def private find_newline(data : array<uint8>; from : int) : int {
    for (i in range(from, length(data))) {
        if (data[i] == uint8('\n')) return i
    }
    return -1
}

//! the bytes at the head of `data` as one DAP frame. `at_eof` says no more bytes will come, so
//! an incomplete frame is an error rather than a wait.
def public parse_dap_frame(data : array<uint8>; at_eof : bool) : FrameParse {
    var pos = 0
    var header_size = 0
    var content_length = -1
    var have_length = false
    while (true) {
        let nl = find_newline(data, pos)
        if (nl < 0) {
            let partial = length(data) - pos
            if (partial > DAP_HEADER_LINE_LIMIT) return FrameParse(error = "DAP header line is too long")
            if (!at_eof) return FrameParse()
            if (partial > 0) return FrameParse(error = "truncated DAP header line")
            if (header_size > 0) return FrameParse(error = "truncated DAP headers")
            return FrameParse()
        }
        let line_len = nl - pos + 1
        if (line_len > DAP_HEADER_LINE_LIMIT) return FrameParse(error = "DAP header line is too long")
        header_size += line_len
        if (header_size > DAP_HEADER_LIMIT) return FrameParse(error = "DAP headers are too large")
        let line = rtrim(bytes_string(data, pos, nl), "\r\n")
        pos = nl + 1
        break if (empty(line))
        let colon = find(line, ":")
        if (colon < 0) return FrameParse(error = "invalid DAP header")
        if (to_lower(slice(line, 0, colon)) == "content-length") {
            let parsed = try_to_int(strip(slice(line, colon + 1)))
            have_length = is_ok(parsed)
            if (have_length) {
                content_length = unwrap(parsed)
            }
        }
    }
    if (!have_length) return FrameParse(error = "missing or invalid DAP Content-Length")
    if (content_length < 0 || content_length > DAP_PAYLOAD_LIMIT) return FrameParse(error = "DAP Content-Length is outside 0..{DAP_PAYLOAD_LIMIT}")
    if (length(data) - pos < content_length) {
        return at_eof ? FrameParse(error = "truncated DAP payload") : FrameParse()
    }
    var payload : array<uint8>
    payload |> reserve(content_length)
    for (i in range(pos, pos + content_length)) {
        payload |> push(data[i])
    }
    var err : string
    var message = read_json(payload, err)
    if (message == null) return FrameParse(error = "invalid DAP payload: {err}")
    if (!(message.value is _object)) {
        delete_json(message)
        return FrameParse(error = "DAP payload must be a JSON object")
    }
    return FrameParse(message = message, consumed = pos + content_length)
}

def public dap_frame_text(message_json : string) : string {
    return "Content-Length: {length(message_json)}\r\n\r\n{message_json}"
}

// ===== JSON helpers =====

def private delete_nodes(var nodes : array<JsonValue?>) {
    for (n in nodes) {
        var node = n
        delete_json(node)
    }
    nodes |> clear()
}

def public json_copy(v : JsonValue?) : JsonValue? {
    if (v == null) return JVNull()
    var err : string
    var copy = read_json(write_json_compact(v), err)
    return copy == null ? JVNull() : copy
}


def private is_int_node(v : JsonValue?) : bool {
    if (v == null) return false
    if (v.value is _longint) return true
    if (v.value is _number) {
        let d = v.value as _number
        return double(int64(d)) == d
    }
    return false
}

def private int_of(v : JsonValue?) : int64 {
    if (v.value is _longint) return v.value as _longint
    return int64(v.value as _number)
}

def private is_number_node(v : JsonValue?) : bool {
    return v != null && (v.value is _longint || v.value is _number)
}

def private number_of(v : JsonValue?) : double {
    if (v.value is _longint) return double(v.value as _longint)
    return v.value as _number
}

// ===== the DAP connection =====

//! the client end of a DAP session: frames off the socket, responses by request seq, events
//! in a queue the tools wait on, and a history the session snapshot shows
class public DapClient : Client {
    buffer : array<uint8>
    responses : table<int; JsonValue?>
    answered : table<int; bool>
    duplicate : JsonValue?
    events : array<JsonValue?>
    history : array<JsonValue?>
    next_seq : int = 1
    reader_error : string
    stopped : bool = true
    closing : bool = false
    endpoint_host : string
    endpoint_port : int = 0
    last_host : string
    last_port : int = 0
    has_endpoint : bool = false
    has_last_endpoint : bool = false
    capabilities : JsonValue?
    terminated_enqueued : bool = false
    last_termination_body : JsonValue?
    close_reason : string

    def DapClient() {
        super()
        make_client_adapter()
        last_termination_body = JV(new_object())
    }

    def finalize() {
        drop_session_state()
        delete_json(last_termination_body)
        last_termination_body = null
        delete super.self
    }

    def private drop_session_state() {
        for (r in values(responses)) {
            var node = r
            delete_json(node)
        }
        responses |> clear()
        answered |> clear()
        delete_json(duplicate)
        duplicate = null
        for (e in events) {
            var node = e
            delete_json(node)
        }
        events |> clear()
        for (h in history) {
            var node = h
            delete_json(node)
        }
        history |> clear()
        delete_json(capabilities)
        capabilities = null
        buffer |> clear()
    }

    def connected() : bool {
        return is_connected() && !stopped
    }

    //! connects, retrying a refused socket until `timeout` seconds pass - a just-launched
    //! debuggee opens its port a moment after it starts
    def connect_with_timeout(host : string; port : int; timeout : float) : string {
        if (connected()) {
            if (endpoint_host == host && endpoint_port == port) return ""
            return "already connected to {endpoint_host}:{endpoint_port}"
        }
        if (is_connected()) {
            end_session("client_close")
        }
        var sw = stopwatch()
        while (true) {
            let left = double(timeout) - elapsed_sec(sw)
            if (left <= 0.0lf) return "timed out connecting to DAP at {host}:{port}"
            break if (connect(host, port, int(left * 1000.0lf) + 1))
            sleep(50u)
        }
        drop_session_state()
        delete_json(last_termination_body)
        last_termination_body = JV(new_object())
        next_seq = 1
        reader_error = ""
        stopped = false
        closing = false
        endpoint_host = host
        endpoint_port = port
        last_host = host
        last_port = port
        has_endpoint = true
        has_last_endpoint = true
        terminated_enqueued = false
        close_reason = ""
        return ""
    }

    def override onConnect() {
        pass
    }

    def override onLog(msg : string) {
        logger_info("dap.socket", msg)
    }

    def override onError(msg : string; code : int) {
        logger_warning("dap.socket", "{msg} ({code})")
    }

    def override onData(buf : uint8?; size : int) {
        for (i in range(size)) {
            buffer |> push(unsafe(buf[i]))
        }
        while (!empty(buffer)) {
            var parsed = parse_dap_frame(buffer, false)
            if (!empty(parsed.error)) {
                reader_error = parsed.error
                if (!closing) {
                    close_reason = "reader_error: {parsed.error}"
                }
                end_session("reader_error")
                return
            }
            break if (parsed.message == null)
            buffer |> erase(0, parsed.consumed)
            take_message(parsed.message)
        }
    }

    def override onDisconnect() {
        stopped = true
        if (!closing) {
            if (empty(close_reason)) {
                close_reason = "connection_closed"
            }
            enqueue_event("terminated", JV({"reason" => JV("connection_closed"), "bridgeSynthetic" => JV(true)}))
        }
    }

    def private take_message(var message : JsonValue?) {
        let kind = message?["type"] ?? ""
        if (kind == "response") {
            let seq_node = message?["request_seq"]
            if (is_int_node(seq_node)) {
                let seq = int(int_of(seq_node))
                if (answered |> key_exists(seq)) {
                    if (duplicate == null) {
                        duplicate = message
                        return
                    }
                } else {
                    answered[seq] = true
                    responses[seq] = message
                    return
                }
            }
            delete_json(message)
        } elif (kind == "event") {
            enqueue_event_node(message)
        } else {
            delete_json(message)
        }
    }

    def enqueue_event(event : string; var body : JsonValue?) {
        enqueue_event_node(JV({"type" => JV("event"), "event" => JV(event), "body" => body}))
    }

    //! `terminated` is one event however many sources report it: later reports merge into the
    //! first one's body, and the reasons that differ are listed
    def private enqueue_event_node(var event : JsonValue?) {
        let name = event?["event"] ?? ""
        if (name == "terminated") {
            var body = event?["body"]
            if (body == null || !(body.value is _object)) {
                var stale_body = (event.value as _object)?["body"] ?? null
                delete_json(stale_body)
                (event.value as _object)["body"] = JV(new_object())
                body = event?["body"]
            }
            let previous_reason = last_termination_body?["reason"] ?? ""
            let current_reason = body?["reason"] ?? ""
            var merged = json_copy(last_termination_body)
            for (key, value in keys(body.value as _object), values(body.value as _object)) {
                var old = (merged.value as _object)?[key] ?? null
                delete_json(old)
                (merged.value as _object)[key] = json_copy(value)
            }
            if (!empty(previous_reason) && !empty(current_reason) && previous_reason != current_reason) {
                var reasons : array<string>
                let old_reasons = last_termination_body?["reasons"]
                if (old_reasons != null && old_reasons.value is _array) {
                    for (r in old_reasons.value as _array) {
                        let text : string = r ?? ""
                        if (!empty(text) && find_index(reasons, text) < 0) {
                            reasons |> push(text)
                        }
                    }
                }
                for (r in [previous_reason, current_reason]) {
                    if (find_index(reasons, r) < 0) {
                        reasons |> push(r)
                    }
                }
                var old = (merged.value as _object)?["reasons"] ?? null
                delete_json(old)
                (merged.value as _object)["reasons"] = JV(reasons)
            }
            delete_json(last_termination_body)
            last_termination_body = merged
            var stale = (event.value as _object)?["body"] ?? null
            delete_json(stale)
            (event.value as _object)["body"] = json_copy(merged)
            remember(event)
            if (terminated_enqueued) {
                for (queued in events) {
                    if ((queued?["event"] ?? "") == "terminated") {
                        var old_body = (queued.value as _object)?["body"] ?? null
                        delete_json(old_body)
                        (queued.value as _object)["body"] = json_copy(merged)
                        break
                    }
                }
                delete_json(event)
                return
            }
            terminated_enqueued = true
        } else {
            remember(event)
        }
        if (length(events) >= EVENT_LIMIT) {
            var drop = 0
            for (i in range(length(events))) {
                if ((events[i]?["event"] ?? "") == "output") {
                    drop = i
                    break
                }
            }
            var victim = events[drop]
            delete_json(victim)
            events |> erase(drop)
        }
        events |> push(event)
    }

    def private remember(event : JsonValue?) {
        if (length(history) >= HISTORY_LIMIT) {
            var oldest = history[0]
            delete_json(oldest)
            history |> erase(0)
        }
        history |> push(json_copy(event))
    }

    //! sends one request; the reply is the response's body under `command`/`success`, or an
    //! error text. `idle` runs on every turn of the wait - the bridge drains its debuggee there,
    //! or a chatty child fills its pipe and never answers.
    def request(command : string; var arguments : JsonValue?; timeout : float; var out : JsonValue?&; idle : block<() : void>) : string {
        if (!connected()) {
            delete_json(arguments)
            return "not connected to a DAP server; call debug_connect or debug_launch"
        }
        if (duplicate != null) {
            let seq_text = write_json_compact(duplicate?["request_seq"])
            let cmd = duplicate?["command"] ?? ""
            let text = write_json_compact(duplicate)
            delete_json(arguments)
            return "DAP server answered request_seq {seq_text} ({cmd}) twice: {text}"
        }
        let seq = next_seq
        next_seq++
        var message = JV({"seq" => JV(seq), "type" => JV("request"), "command" => JV(command), "arguments" => arguments})
        let wire = dap_frame_text(write_json_compact(message))
        delete_json(message)
        let sent = send(unsafe(reinterpret<uint8?>(wire)), length(wire))
        if (!sent) return "failed to send DAP {command}"
        var sw = stopwatch()
        while (!(responses |> key_exists(seq))) {
            if (!empty(reader_error)) return "DAP reader failed: {reader_error}"
            if (stopped) return "DAP connection closed before replying to {command}"
            if (elapsed_sec(sw) >= double(timeout)) return "DAP request timed out: {command}"
            tick()
            invoke(idle)
            sleep(PUMP_SLEEP_MS)
        }
        var response = responses[seq]
        responses |> erase(seq)
        let success = response?["success"] ?? false
        if (!success) {
            let detail = key_exists(response, "message") ? (response?["message"] ?? "") : write_json_compact(response)
            delete_json(response)
            return "DAP {command} failed: {detail}"
        }
        var body = key_exists(response, "body") ? json_copy(response?["body"]) : JVNull()
        delete_json(response)
        out = JV({"command" => JV(command), "success" => JV(true), "body" => body})
        return ""
    }

    def initialize(timeout : float; var out : JsonValue?&; idle : block<() : void>) : string {
        if (capabilities != null) {
            out = JV({"command" => JV("initialize"), "success" => JV(true), "body" => json_copy(capabilities), "cached" => JV(true)})
            return ""
        }
        var args = JV({
            "clientID" => JV("codex"),
            "clientName" => JV("daslang DAP MCP bridge"),
            "adapterID" => JV("daslang"),
            "pathFormat" => JV("path"),
            "linesStartAt1" => JV(true),
            "columnsStartAt1" => JV(true),
            "supportsVariableType" => JV(true),
            "supportsVariablePaging" => JV(true),
            "supportsRunInTerminalRequest" => JV(false)
        })
        let err = request("initialize", args, timeout, out, idle)
        if (!empty(err)) return err
        let body = out?["body"]
        capabilities = body != null && body.value is _object ? json_copy(body) : JV(new_object())
        return ""
    }

    //! the next queued event, the next one named `name` when given; a timeout or a closed
    //! connection answers with an event-less record that says which
    def wait_event(name : string; timeout : float; idle : block<() : void>) : JsonValue? {
        var sw = stopwatch()
        while (true) {
            for (i in range(length(events))) {
                if (empty(name) || (events[i]?["event"] ?? "") == name) {
                    var event = events[i]
                    events |> erase(i)
                    return event
                }
            }
            if (!empty(reader_error)) {
                return JV({"type" => JV("event"), "event" => JVNull(), "body" => JV({"readerError" => JV(reader_error), "requestedEvent" => JV(name)})})
            }
            let remaining = double(timeout) - elapsed_sec(sw)
            if (remaining <= 0.0lf || stopped) {
                return JV({"type" => JV("event"), "event" => JVNull(), "body" => JV({"timedOut" => JV(remaining <= 0.0lf), "connectionClosed" => JV(stopped), "requestedEvent" => JV(name)})})
            }
            tick()
            invoke(idle)
            sleep(PUMP_SLEEP_MS)
        }
        return null
    }

    def end_session(reason : string) {
        closing = true
        if (connected()) {
            close_reason = reason
        }
        if (is_connected()) {
            close()
        }
        stopped = true
        has_endpoint = false
        delete_json(capabilities)
        capabilities = null
    }
}

def private new_object() : table<string; JsonValue?> {
    var o : table<string; JsonValue?>
    return <- o
}

// ===== the bridge =====

struct public ToolOutcome {
    ok : bool
    text : string
}

def private failed(text : string) : ToolOutcome {
    return ToolOutcome(ok = false, text = text)
}

def private succeeded(var value : JsonValue?) : ToolOutcome {
    let text = write_json(value)
    delete_json(value)
    return ToolOutcome(ok = true, text = text)
}

let private RESUME_COMMANDS <- {"debug_continue" => "continue", "debug_step_in" => "stepIn", "debug_step_over" => "next", "debug_step_out" => "stepOut"}

//! the DAP command a resuming tool sends; empty for a tool that is not one
def public resume_command(tool : string) : string {
    return RESUME_COMMANDS?[tool] ?? ""
}

class public DapBridge {
    repo_root : string
    timeout : float
    executable : string
    dap : DapClient?
    proc : process
    pid : int = 0
    exit_code : int = 0
    exited : bool = false
    process_output : array<string>
    owned_command : array<string>

    def DapBridge(root : string; timeout_sec : float; exe : string) {
        repo_root = get_full_file_name(root)
        timeout = timeout_sec
        executable = empty(exe) ? "" : get_full_file_name(exe)
        dap = new DapClient()
    }

    def finalize() {
        close()
        unsafe {
            delete dap
        }
    }

    def close() {
        dap->end_session("client_close")
        terminate_owned()
    }

    // ----- the child -----

    def private drain_process() {
        if (proc == null) return
        var lines : array<string>
        unsafe(process_drain(proc) $(line) {
            lines |> push(clone_string(line))
        })
        for (line in lines) {
            if (length(process_output) >= OUTPUT_LIMIT) {
                process_output |> erase(0)
            }
            process_output |> push("output: {line}")
        }
    }

    //! the child's exit, once: the code is kept and a synthetic `terminated` lands on the queue
    def private poll_process() {
        if (proc == null || exited) return
        drain_process()
        let rc = unsafe(process_poll(proc))
        if (rc == process_running) return
        drain_process()
        exited = true
        exit_code = rc
        dap->enqueue_event("terminated", JV({"exitCode" => JV(rc), "reason" => JV("process_exit"), "bridgeSynthetic" => JV(true)}))
    }

    //! the socket and the child, between MCP requests; a wait on the socket polls the child
    //! itself, on every turn
    def pump() {
        dap->tick()
        poll_process()
    }

    def private wait_owned_process(timeout_sec : float) {
        if (proc == null || exited) return
        var sw = stopwatch()
        while (true) {
            poll_process()
            break if (exited || elapsed_sec(sw) >= double(timeout_sec))
            sleep(PUMP_SLEEP_MS)
        }
    }

    def private terminate_owned() {
        if (proc == null) return
        poll_process()
        if (!exited) {
            unsafe(process_terminate(proc))
            wait_owned_process(3.0)
            if (!exited) {
                unsafe(process_kill(proc))
                wait_owned_process(3.0)
            }
        }
        drain_process()
        unsafe(close_process(proc))
        proc = null
    }

    def private forget_process() {
        if (proc != null) {
            unsafe(close_process(proc))
            proc = null
        }
        pid = 0
        exited = false
        exit_code = 0
        process_output |> clear()
        owned_command |> clear()
    }

    def session_snapshot() : JsonValue? {
        poll_process()
        var events : array<JsonValue?>
        events |> reserve(length(dap.history))
        for (h in dap.history) {
            events |> push(json_copy(h))
        }
        let host = dap.has_endpoint ? dap.endpoint_host : dap.last_host
        let port = dap.has_endpoint ? dap.endpoint_port : dap.last_port
        let known = dap.has_endpoint || dap.has_last_endpoint
        var snapshot <- {
            "connected" => JV(dap->connected()),
            "host" => known ? JV(host) : JVNull(),
            "port" => known ? JV(port) : JVNull(),
            "pid" => pid > 0 ? JV(pid) : JVNull(),
            "return_code" => exited ? JV(exit_code) : JVNull(),
            "close_reason" => empty(dap.close_reason) ? JVNull() : JV(dap.close_reason),
            "last_dap_termination" => json_copy(dap.last_termination_body),
            "recent_dap_events" => JV(events),
            "process_output_tail" => JV(process_output)
        }
        return JV(snapshot)
    }

    //! an error with the session behind it, when there is one
    def error_text(error : string) : string {
        if (!(dap.has_endpoint || dap.has_last_endpoint) && pid == 0) return error
        var snapshot = session_snapshot()
        let text = write_json_compact(snapshot)
        delete_json(snapshot)
        return "{error}; session: {text}"
    }

    // ----- arguments -----

    def private path_arg(v : JsonValue?; must_exist : bool; var path : string&) : string {
        if (v == null || !(v.value is _string) || empty(v.value as _string)) return "path must be a non-empty string"
        return path_text_arg(v.value as _string, must_exist, path)
    }

    def private path_text_arg(text : string; must_exist : bool; var path : string&) : string {
        if (empty(text)) return "path must be a non-empty string"
        var p = text
        if (!is_absolute(p)) {
            p = path_join(repo_root, p)
        }
        p = get_full_file_name(p)
        if (must_exist && !fexist(p)) return "path does not exist: {p}"
        path = p
        return ""
    }

    def private host_port_timeout(args : JsonValue?; var host : string&; var port : int&; var timeout_sec : float&) : string {
        host = DEFAULT_HOST
        port = DEFAULT_PORT
        timeout_sec = min(timeout, 30.0)
        if (key_exists(args, "host")) {
            let h = args?["host"]
            if (!(h.value is _string) || empty(h.value as _string)) return "host must be a non-empty string"
            host = h.value as _string
        }
        if (key_exists(args, "port")) {
            let p = args?["port"]
            if (!is_int_node(p) || int_of(p) < 1l || int_of(p) > 65535l) return "port must be an integer from 1 to 65535"
            port = int(int_of(p))
        }
        if (key_exists(args, "timeout_sec")) {
            let t = args?["timeout_sec"]
            if (!is_number_node(t) || number_of(t) <= 0.0lf) return "timeout_sec must be positive"
            timeout_sec = float(number_of(t))
        }
        return ""
    }

    def private integer_arg(args : JsonValue?; name : string; minimum : int; var value : int64&) : string {
        let v = args?[name]
        if (!key_exists(args, name) || !is_int_node(v) || int_of(v) < int64(minimum)) return "{name} must be an integer >= {minimum}"
        value = int_of(v)
        return ""
    }

    def private bool_arg(args : JsonValue?; name : string; fallback : bool; var value : bool&) : string {
        value = fallback
        if (!key_exists(args, name)) return ""
        let v = args?[name]
        if (!(v.value is _bool)) return "{name} must be a boolean"
        value = v.value as _bool
        return ""
    }

    def private string_list_arg(args : JsonValue?; name : string; var list : array<string>&) : string {
        list |> clear()
        if (!key_exists(args, name)) return ""
        let v = args?[name]
        if (!(v.value is _array)) return "{name} must be an array of strings"
        list |> reserve(length(v.value as _array))
        for (item in v.value as _array) {
            if (item == null || !(item.value is _string)) return "{name} must be an array of strings"
            list |> push(item.value as _string)
        }
        return ""
    }

    //! the launch/attach arguments the DAP server reads, absolutized
    def private session_settings(args : JsonValue?; var out : JsonValue?&) : string {
        var settings = JV(new_object())
        let err = fill_session_settings(args, settings)
        if (!empty(err)) {
            delete_json(settings)
            return err
        }
        out = settings
        return ""
    }

    def private fill_session_settings(args : JsonValue?; var settings : JsonValue?) : string {
        if (key_exists(args, "cwd")) {
            var cwd : string
            let err = path_arg(args?["cwd"], true, cwd)
            if (!empty(err)) return err
            (settings.value as _object)["cwd"] = JV(cwd)
        }
        if (key_exists(args, "paths")) {
            var paths : array<string>
            let err = string_list_arg(args, "paths", paths)
            if (!empty(err)) return err
            var resolved : array<string>
            resolved |> reserve(length(paths))
            for (p in paths) {
                var full : string
                let perr = path_text_arg(p, true, full)
                if (!empty(perr)) return perr
                resolved |> push(full)
            }
            (settings.value as _object)["paths"] = JV(resolved)
        }
        if (key_exists(args, "path_aliases")) {
            let aliases = args?["path_aliases"]
            if (!(aliases.value is _object)) return "path_aliases must map strings to strings"
            for (value in values(aliases.value as _object)) {
                if (value == null || !(value.value is _string)) return "path_aliases must map strings to strings"
            }
            (settings.value as _object)["pathAliases"] = json_copy(aliases)
        }
        for (pair in [("inline_preview_limit", "inlinePreviewLimit"), ("max_children_count", "maxChildrenCount")]) {
            if (key_exists(args, pair._0)) {
                let v = args?[pair._0]
                if (!is_int_node(v) || int_of(v) < 0l) return "{pair._0} must be a non-negative integer"
                (settings.value as _object)[pair._1] = JV(int_of(v))
            }
        }
        if (key_exists(args, "collect_all_globals")) {
            var collect = false
            let err = bool_arg(args, "collect_all_globals", false, collect)
            if (!empty(err)) return err
            (settings.value as _object)["collectAllGlobals"] = JV(collect)
        }
        return ""
    }

    def private pick_executable(args : JsonValue?; var exe : string&) : string {
        var requested = ""
        if (key_exists(args, "executable")) {
            let err = path_arg(args?["executable"], true, requested)
            if (!empty(err)) return err
        } elif (!empty(executable)) {
            requested = executable
        }
        if (!empty(requested)) {
            var st : FStat
            if (!stat(requested, st) || !st.is_reg || !st.is_exec) return "not an executable file: {requested}"
            exe = requested
            return ""
        }
        var newest = ""
        var newest_time : clock
        for (rel in ["bin/daslang", "bin/daslang.exe", "build/daslang", "build/daslang.exe", "bin/Release/daslang", "bin/Release/daslang.exe"]) {
            let cand = path_join(repo_root, rel)
            var st : FStat
            if (stat(cand, st) && st.is_reg && st.is_exec && (empty(newest) || (st.mtime - newest_time) > 0.0lf)) {
                newest = cand
                newest_time = st.mtime
            }
        }
        if (empty(newest)) return "no daslang executable found; pass executable explicitly"
        exe = newest
        return ""
    }

    // ----- the tools -----

    def call(name : string; args : JsonValue?) : ToolOutcome {   // nolint:STYLE037 - flat tool dispatch, one arm per tool
        if (args != null && !(args.value is _object) && !(args.value is _null)) return failed("tool arguments must be an object")
        pump()
        if (name == "debug_connect") return tool_connect(args)
        if (name == "debug_initialize") {
            var out : JsonValue?
            let err = dap->initialize(timeout, out) $ {
                poll_process()
            }
            return empty(err) ? succeeded(out) : failed(err)
        }
        if (name == "debug_launch") return tool_launch(args)
        if (name == "debug_attach") return tool_attach(args)
        if (name == "debug_set_breakpoints") return tool_set_breakpoints(args)
        if (name == "debug_data_breakpoint_info") {
            var reference = 0l
            let err = integer_arg(args, "variables_reference", 1, reference)
            if (!empty(err)) return failed(err)
            let variable = args?["name"]
            if (!key_exists(args, "name") || !(variable.value is _string) || empty(variable.value as _string)) return failed("name must be a non-empty string")
            return dap_request("dataBreakpointInfo", JV({"variablesReference" => JV(reference), "name" => JV(variable.value as _string)}), timeout)
        }
        if (name == "debug_set_data_breakpoints") return tool_set_data_breakpoints(args)
        if (name == "debug_configuration_done") return dap_request("configurationDone", JV(new_object()), timeout)
        if (name == "debug_threads") return dap_request("threads", JV(new_object()), timeout)
        if (name == "debug_stack_trace") return tool_stack_trace(args)
        if (name == "debug_scopes") {
            var frame = 0l
            let err = integer_arg(args, "frame_id", 1, frame)
            if (!empty(err)) return failed(err)
            return dap_request("scopes", JV({"frameId" => JV(frame)}), timeout)
        }
        if (name == "debug_variables") return tool_variables(args)
        if (name == "debug_evaluate") return tool_evaluate(args)
        if (!empty(resume_command(name))) return tool_resume(resume_command(name), args)
        if (name == "debug_pause") {
            var thread = 0l
            let err = integer_arg(args, "thread_id", 1, thread)
            if (!empty(err)) return failed(err)
            return dap_request("pause", JV({"threadId" => JV(thread)}), timeout)
        }
        if (name == "debug_terminate") return tool_terminate()
        if (name == "debug_disconnect") return tool_disconnect()
        if (name == "debug_wait_event") return tool_wait_event(args)
        return failed("unknown tool: {name}")
    }

    def private dap_request(command : string; var arguments : JsonValue?; timeout_sec : float) : ToolOutcome {
        var out : JsonValue?
        let err = request_pumping(command, arguments, timeout_sec, out)
        return empty(err) ? succeeded(out) : failed(err)
    }

    //! `DapClient.request` with the child polled on every turn of the wait
    def private request_pumping(command : string; var arguments : JsonValue?; timeout_sec : float; var out : JsonValue?&) : string {
        return dap->request(command, arguments, timeout_sec, out) $ {
            poll_process()
        }
    }

    def private tool_connect(args : JsonValue?) : ToolOutcome {
        var host : string
        var port = 0
        var timeout_sec = 0.0
        let err = host_port_timeout(args, host, port, timeout_sec)
        if (!empty(err)) return failed(err)
        let already = dap->connected() && dap.endpoint_host == host && dap.endpoint_port == port
        let cerr = dap->connect_with_timeout(host, port, timeout_sec)
        if (!empty(cerr)) return failed(cerr)
        return succeeded(JV({"host" => JV(host), "port" => JV(port), "already_connected" => JV(already)}))
    }

    def private tool_launch(args : JsonValue?) : ToolOutcome {   // nolint:STYLE037,STYLE038 - the launch command line, one arm per argument
        var file : string
        var err = path_arg(args?["file"], true, file)
        if (!empty(err)) return failed(err)
        var exe : string
        err = pick_executable(args, exe)
        if (!empty(err)) return failed(err)
        var host : string
        var port = 0
        var connect_timeout = 0.0
        err = host_port_timeout(args, host, port, connect_timeout)
        if (!empty(err)) return failed(err)
        if (host != "127.0.0.1" && host != "localhost") return failed("debug_launch supports only the IPv4 loopback host")
        if (key_exists(args, "port")) {
            if (probe_local_port(host, port) < 0) return failed("DAP launch port {host}:{port} is unavailable")
        } else {
            port = probe_local_port(host, 0)
            if (port < 0) return failed("no free local port for the DAP server")
        }
        var cwd = repo_root
        if (key_exists(args, "cwd")) {
            err = path_arg(args?["cwd"], true, cwd)
            if (!empty(err)) return failed(err)
        }
        var command <- [exe, "--das-wait-debugger"]
        var optimize = true
        err = bool_arg(args, "optimize", true, optimize)
        if (!empty(err)) return failed(err)
        if (!optimize) {
            command |> push("-no-optimization")
        }
        if (key_exists(args, "project")) {
            var project : string
            err = path_arg(args?["project"], true, project)
            if (!empty(err)) return failed(err)
            command |> push_from(["-project", project])
        }
        if (key_exists(args, "project_root")) {
            var project_root : string
            err = path_arg(args?["project_root"], true, project_root)
            if (!empty(err)) return failed(err)
            command |> push_from(["-project_root", project_root])
        }
        var modules : array<string>
        err = string_list_arg(args, "load_modules", modules)
        if (!empty(err)) return failed(err)
        for (m in modules) {
            var full : string
            err = path_text_arg(m, true, full)
            if (!empty(err)) return failed(err)
            command |> push_from(["-load_module", full])
        }
        var program_args : array<string>
        err = string_list_arg(args, "program_args", program_args)
        if (!empty(err)) return failed(err)
        var stepping = false
        err = bool_arg(args, "stepping_debugger", false, stepping)
        if (!empty(err)) return failed(err)
        command |> push_from([file, "--", "--das-debug-port", "{port}"])
        if (stepping) {
            command |> push("--das-stepping-debugger")
        }
        command |> push_from(program_args)
        if (dap->connected()) return failed("a DAP session is already connected; disconnect it first")
        if (proc != null && !exited) {
            poll_process()
            if (!exited) return failed("bridge already owns running debuggee pid {pid}")
        }
        forget_process()
        let noenv : array<string>
        proc = unsafe(spawn_process(command, cwd, noenv))
        if (proc == null) return failed("failed to launch debug session: cannot spawn {exe}")
        pid = unsafe(process_pid(proc))
        owned_command := command
        var connection : JsonValue?
        var initialized : JsonValue?
        var launched : JsonValue?
        var initialized_event : JsonValue?
        let failure = launch_session(args, host, port, connect_timeout, cwd, connection, initialized, launched, initialized_event)
        if (!empty(failure)) {
            dap->end_session("client_close")
            terminate_owned()
            delete_json(connection)
            delete_json(initialized)
            delete_json(launched)
            delete_json(initialized_event)
            var output_json = JV(process_output)
            let tail = empty(process_output) ? "" : "; debuggee output: {write_json_compact(output_json)}"
            delete_json(output_json)
            return failed("failed to launch debug session: {failure}{tail}")
        }
        return succeeded(JV({
            "pid" => JV(pid),
            "command" => JV(command),
            "connection" => connection,
            "initialize" => initialized,
            "launch" => launched,
            "initialized_event" => initialized_event
        }))
    }

    //! the DAP handshake with a just-spawned debuggee: connect, initialize, launch, the
    //! `initialized` event; an exit anywhere in between is the failure text
    def private launch_session(args : JsonValue?; host : string; port : int; connect_timeout : float; cwd : string; var connection, initialized, launched, initialized_event : JsonValue?&) : string {
        let cerr = dap->connect_with_timeout(host, port, connect_timeout)
        if (!empty(cerr)) return cerr
        connection = JV({"host" => JV(host), "port" => JV(port), "already_connected" => JV(false)})
        poll_process()
        if (exited) return "debuggee exited before DAP initialization with {exit_code}"
        let ierr = dap->initialize(connect_timeout, initialized) $ {
            poll_process()
        }
        if (!empty(ierr)) return ierr
        var settings : JsonValue?
        let serr = session_settings(args, settings)
        if (!empty(serr)) return serr
        if (!key_exists(settings, "cwd")) {
            (settings.value as _object)["cwd"] = JV(cwd)
        }
        let lerr = request_pumping("launch", settings, timeout, launched)
        if (!empty(lerr)) return lerr
        initialized_event = wait_event_pumping("initialized", min(timeout, 10.0))
        poll_process()
        if (exited) return "debuggee exited during DAP launch with {exit_code}"
        return ""
    }

    //! `wait_event` with the child polled alongside the socket, so a dying debuggee ends the
    //! wait with its synthetic `terminated`
    def private wait_event_pumping(name : string; timeout_sec : float) : JsonValue? {
        return dap->wait_event(name, timeout_sec) $ {
            poll_process()
        }
    }

    def private tool_attach(args : JsonValue?) : ToolOutcome {
        if (proc != null && !exited) {
            poll_process()
            if (!exited) return failed("bridge already owns running debuggee pid {pid}")
        }
        forget_process()
        var host : string
        var port = 0
        var timeout_sec = 0.0
        var err = host_port_timeout(args, host, port, timeout_sec)
        if (!empty(err)) return failed(err)
        let already = dap->connected() && dap.endpoint_host == host && dap.endpoint_port == port
        err = dap->connect_with_timeout(host, port, timeout_sec)
        if (!empty(err)) return failed(err)
        var initialized : JsonValue?
        err = dap->initialize(timeout_sec, initialized) $ {
            poll_process()
        }
        if (!empty(err)) return failed(err)
        var settings : JsonValue?
        err = session_settings(args, settings)
        if (!empty(err)) {
            delete_json(initialized)
            return failed(err)
        }
        var attached : JsonValue?
        err = request_pumping("attach", settings, timeout, attached)
        if (!empty(err)) {
            delete_json(initialized)
            return failed(err)
        }
        var initialized_event = wait_event_pumping("initialized", min(timeout, 10.0))
        return succeeded(JV({
            "connection" => JV({"host" => JV(host), "port" => JV(port), "already_connected" => JV(already)}),
            "initialize" => initialized,
            "attach" => attached,
            "initialized_event" => initialized_event
        }))
    }

    def private tool_set_breakpoints(args : JsonValue?) : ToolOutcome {
        var file : string
        let err = path_arg(args?["file"], true, file)
        if (!empty(err)) return failed(err)
        let lines = args?["lines"]
        if (!key_exists(args, "lines") || !(lines.value is _array)) return failed("lines must be an array of positive integers")
        var breakpoints : array<JsonValue?>
        breakpoints |> reserve(length(lines.value as _array))
        for (line in lines.value as _array) {
            if (!is_int_node(line) || int_of(line) <= 0l) {
                delete_nodes(breakpoints)
                return failed("lines must be an array of positive integers")
            }
            breakpoints |> push(JV({"line" => JV(int_of(line))}))
        }
        var modified = false
        let merr = bool_arg(args, "source_modified", false, modified)
        if (!empty(merr)) {
            delete_nodes(breakpoints)
            return failed("source_modified must be a boolean")
        }
        return dap_request("setBreakpoints", JV({
            "source" => JV({"name" => JV(base_name(file)), "path" => JV(file)}),
            "breakpoints" => JV(breakpoints),
            "sourceModified" => JV(modified)
        }), timeout)
    }

    def private tool_set_data_breakpoints(args : JsonValue?) : ToolOutcome {
        let items = args?["breakpoints"]
        if (!key_exists(args, "breakpoints") || !(items.value is _array)) return failed("breakpoints must be an array")
        var converted : array<JsonValue?>
        converted |> reserve(length(items.value as _array))
        for (item in items.value as _array) {
            let data_id = item?["data_id"]
            if (item == null || !(item.value is _object) || !key_exists(item, "data_id") || !(data_id.value is _string)) {
                delete_nodes(converted)
                return failed("each data breakpoint needs a string data_id")
            }
            let enabled = key_exists(item, "enabled") ? (item?["enabled"] ?? true) : true
            converted |> push(JV({
                "dataId" => JV(data_id.value as _string),
                "accessType" => JV(item?["access_type"] ?? "write"),
                "condition" => JV(item?["condition"] ?? ""),
                "hitCondition" => JV(item?["hit_condition"] ?? ""),
                "description" => JV(item?["description"] ?? ""),
                "enabled" => JV(enabled)
            }))
        }
        return dap_request("setDataBreakpoints", JV({"breakpoints" => JV(converted)}), timeout)
    }

    def private tool_stack_trace(args : JsonValue?) : ToolOutcome {
        var thread = 0l
        let err = integer_arg(args, "thread_id", 1, thread)
        if (!empty(err)) return failed(err)
        var start_frame = 0l
        if (key_exists(args, "start_frame")) {
            let v = args?["start_frame"]
            if (!is_int_node(v) || int_of(v) < 0l) return failed("start_frame must be a non-negative integer")
            start_frame = int_of(v)
        }
        var levels = 1000l
        if (key_exists(args, "levels")) {
            let v = args?["levels"]
            if (!is_int_node(v) || int_of(v) < 1l) return failed("levels must be a positive integer")
            levels = int_of(v)
        }
        return dap_request("stackTrace", JV({"threadId" => JV(thread), "startFrame" => JV(start_frame), "levels" => JV(levels)}), timeout)
    }

    def private tool_variables(args : JsonValue?) : ToolOutcome {
        var reference = 0l
        var err = integer_arg(args, "variables_reference", 1, reference)
        if (!empty(err)) return failed(err)
        var start = 0l
        if (key_exists(args, "start")) {
            err = integer_arg(args, "start", 0, start)
            if (!empty(err)) return failed(err)
        }
        var count = 0l
        if (key_exists(args, "count")) {
            err = integer_arg(args, "count", 1, count)
            if (!empty(err)) return failed(err)
        }
        var params <- {"variablesReference" => JV(reference)}
        if (key_exists(args, "start")) {
            params["start"] = JV(start)
        }
        if (key_exists(args, "count")) {
            params["count"] = JV(count)
        }
        return dap_request("variables", JV(params), timeout)
    }

    def private tool_evaluate(args : JsonValue?) : ToolOutcome {
        let expression = args?["expression"]
        if (!key_exists(args, "expression") || !(expression.value is _string) || empty(expression.value as _string)) return failed("expression must be a non-empty string")
        var context = "repl"
        if (key_exists(args, "context")) {
            let c = args?["context"]
            if (!(c.value is _string)) return failed("context must be a string")
            context = c.value as _string
        }
        var frame = 0l
        let err = integer_arg(args, "frame_id", 1, frame)
        if (!empty(err)) return failed(err)
        return dap_request("evaluate", JV({"expression" => JV(expression.value as _string), "frameId" => JV(frame), "context" => JV(context)}), timeout)
    }

    def private tool_resume(command : string; args : JsonValue?) : ToolOutcome {
        var thread = 0l
        let err = integer_arg(args, "thread_id", 1, thread)
        if (!empty(err)) return failed(err)
        var out : JsonValue?
        let rerr = request_pumping(command, JV({"threadId" => JV(thread)}), timeout, out)
        if (!empty(rerr)) return failed(rerr)
        dap->enqueue_event("continued", JV({"threadId" => JV(thread), "allThreadsContinued" => JV(false), "bridgeSynthetic" => JV(true), "command" => JV(command)}))
        return succeeded(out)
    }

    def private tool_terminate() : ToolOutcome {
        var out : JsonValue?
        let err = request_pumping("terminate", JV(new_object()), min(timeout, 10.0), out)
        if (!empty(err)) return failed(err)
        poll_process()
        (out.value as _object)["process_output_tail"] = JV(process_output)
        return succeeded(out)
    }

    def private tool_disconnect() : ToolOutcome {
        if (!dap->connected()) {
            wait_owned_process(min(timeout, 1.0))
            dap->end_session("client_close")
            return succeeded(JV({"command" => JV("disconnect"), "success" => JV(true), "already_disconnected" => JV(true), "session" => session_snapshot()}))
        }
        var out : JsonValue?
        let err = request_pumping("disconnect", JV(new_object()), timeout, out)
        if (!empty(err)) {
            if (dap->connected()) return failed(err)
            dap->end_session("client_close")
            wait_owned_process(min(timeout, 1.0))
            return succeeded(JV({"command" => JV("disconnect"), "success" => JV(true), "already_disconnected" => JV(true), "disconnect_error" => JV(err), "session" => session_snapshot()}))
        }
        dap->end_session("disconnect_request")
        wait_owned_process(min(timeout, 3.0))
        (out.value as _object)["session"] = session_snapshot()
        return succeeded(out)
    }

    def private tool_wait_event(args : JsonValue?) : ToolOutcome {
        var name = ""
        if (key_exists(args, "event")) {
            let e = args?["event"]
            if (!(e.value is _string)) return failed("event must be a string")
            name = e.value as _string
        }
        var timeout_sec = min(timeout, 30.0)
        if (key_exists(args, "timeout_sec")) {
            let t = args?["timeout_sec"]
            if (!is_number_node(t) || number_of(t) < 0.0lf) return failed("timeout_sec must be non-negative")
            timeout_sec = float(number_of(t))
        }
        var event = wait_event_pumping(name, timeout_sec)
        let got = event?["event"]
        var body = event?["body"]
        if (key_exists(body, "readerError")) {
            let reason : string = body?["readerError"] ?? ""
            delete_json(event)
            return failed("DAP reader failed: {reason}")
        }
        if (body == null || !(body.value is _object)) {
            var stale = (event.value as _object)?["body"] ?? null
            delete_json(stale)
            (event.value as _object)["body"] = JV(new_object())
            body = event?["body"]
        }
        if (got != null && got.value is _string && (got.value as _string) == "terminated") {
            wait_owned_process(min(timeout, 1.0))
            for (key, value in keys(dap.last_termination_body.value as _object), values(dap.last_termination_body.value as _object)) {
                var old = (body.value as _object)?[key] ?? null
                delete_json(old)
                (body.value as _object)[key] = json_copy(value)
            }
            (body.value as _object)["session"] = session_snapshot()
        }
        if (got == null || !(got.value is _string)) {
            poll_process()
            (body.value as _object)["processOutputTail"] = JV(process_output)
        }
        return succeeded(event)
    }
}
