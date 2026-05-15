options gen2
options rtti
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_api_stdio shared public

//! Stdin/stdout JSON-RPC 2.0 transport for live commands.
//!
//! Reads newline-delimited JSON-RPC 2.0 messages from stdin, dispatches
//! them through ``live_dispatch_command``, and writes responses to stdout.
//! No HTTP server, no dasHV dependency — pair with
//! ``require live/live_api`` when you want HTTP instead.
//!
//! Wire format::
//!
//!     → {"jsonrpc":"2.0","id":1,"method":"set_color","params":{"r":1.0}}
//!     ← {"jsonrpc":"2.0","id":1,"result":<value>}
//!
//! ``method`` is a live command name (any ``[live_command]`` function,
//! plus the built-ins from ``live/live_api_builtins``). ``params`` is
//! passed verbatim as the command's input ``JsonValue?``.
//!
//! IMPORTANT: stdout is the response channel. Scripts that use this
//! transport must redirect ``print()`` to stderr or a log file, or the
//! application's output will interleave with JSON-RPC responses.

require daslib/json
require daslib/json_boost
require daslib/jobque_boost
require daslib/debugger
require daslib/fio
require strings
require live/live_api_builtins public
require live_host

// --- Reader thread → channel handoff ---

struct StdioMsg {
    line : string
}

// --- JSON-RPC 2.0 envelope ---

def public json_rpc_response(id_json : string; result_json : string) : string {
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id_json},\"result\":{result_json}}"
}

def public json_rpc_error(id_json : string; code : int; message : string) : string {
    let msg_json = write_json(JV(message))
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id_json},\"error\":\{\"code\":{code},\"message\":{msg_json}}}"
}

def public serialize_id(id_val : JsonValue?) : string {
    return id_val == null ? "null" : write_json(id_val)
}

// --- Pure parsing layer (transport-agnostic, testable without dispatch) ---

struct public ParsedJsonRpc {
    id_str : string         //!< Serialized JSON-RPC `id` field; "null" when missing or parse failed before id was reached.
    method : string         //!< Live command name; empty when the request was malformed.
    inner_json : string     //!< Synthesized `{"name":<method>,"args":<params>}` for live_host dispatch; empty when invalid.
    error_envelope : string //!< Full JSON-RPC error response when the request was malformed; empty when valid.
    is_notification : bool  //!< True when the request omits `id` (JSON-RPC 2.0 notification — server MUST NOT respond).
}

def public parse_jsonrpc_request(line : string) : ParsedJsonRpc {
    //! Parse a single JSON-RPC 2.0 request line into its components without touching dispatch.
    //! Pure function — testable in isolation from a live_host runtime.
    //!
    //! Permissive subset: the `jsonrpc` member is optional (a strict JSON-RPC
    //! 2.0 server would reject requests omitting `"jsonrpc":"2.0"` with
    //! `-32600`; this transport accepts them, since real-world clients are
    //! lazy about the field). All other JSON-RPC 2.0 rules — id type
    //! restrictions, notification semantics, required `method` — are
    //! enforced.
    var result = ParsedJsonRpc(id_str = "null")
    var parse_error = ""
    var parsed = read_json(line, parse_error)
    if (parsed == null) {
        result.error_envelope = json_rpc_error("null", -32700, "parse error: {parse_error}")
        return result
    }
    if (!(parsed.value is _object)) {
        result.error_envelope = json_rpc_error("null", -32600, "invalid request: not a JSON object")
        return result
    }
    let body & = unsafe(parsed.value as _object)
    let id_v = unsafe(reinterpret<JsonValue?>(body?["id"] ?? null))
    let method_v = unsafe(reinterpret<JsonValue?>(body?["method"] ?? null))
    let params_v = unsafe(reinterpret<JsonValue?>(body?["params"] ?? null))
    // JSON-RPC 2.0 §4: id MUST be string, number, or null. Reject objects /
    // arrays / booleans — they'd also break framing since write_json would
    // pretty-print them with embedded newlines.
    if (id_v != null && !(id_v.value is _string) && !(id_v.value is _number) && !(id_v.value is _longint) && !(id_v.value is _null)) {
        result.error_envelope = json_rpc_error("null", -32600, "invalid request: id must be string, number, or null")
        return result
    }
    result.is_notification = (id_v == null)
    result.id_str = serialize_id(id_v)
    if (method_v == null || !(method_v.value is _string)) {
        result.error_envelope = json_rpc_error(result.id_str, -32600, "invalid request: missing or non-string method")
        return result
    }
    result.method = method_v.value as _string
    let method_str = write_json(JV(result.method))
    let params_str = params_v == null ? "null" : write_json(params_v)
    result.inner_json = "\{\"name\":{method_str},\"args\":{params_str}}"
    return result
}

// --- Compact serialization (write_json pretty-prints; the wire is newline-delimited) ---

def public compact_json_whitespace(s : string) : string {
    //! Strip JSON pretty-printer whitespace (spaces, tabs, newlines, carriage returns)
    //! that appears outside string literals. JSON whitespace outside strings is
    //! non-significant, so this is safe; whitespace inside `"..."` is preserved
    //! verbatim. Used to flatten dispatch_command output for one-message-per-line
    //! framing on the stdio wire.
    return build_string() $(var w) {
        peek_data(s) $(bytes) {
            var in_string = false
            var escape = false
            for (b in bytes) {
                if (in_string) {
                    write_char(w, int(b))
                    if (escape) {
                        escape = false
                    } elif (b == uint8('\\')) {
                        escape = true
                    } elif (b == uint8('"')) {
                        in_string = false
                    }
                } else {
                    if (b == uint8('"')) {
                        in_string = true
                        write_char(w, int(b))
                    } elif (b != uint8(' ') && b != uint8('\n') && b != uint8('\t') && b != uint8('\r')) {
                        write_char(w, int(b))
                    }
                }
            }
        }
    }
}

// --- Per-request dispatch ---

def handle_jsonrpc_line(line : string) : string {
    //! Dispatches a single JSON-RPC request line. Returns the response envelope
    //! as a string, or an empty string for notifications (id-less requests) per
    //! JSON-RPC 2.0 — callers must not write anything to the wire when the
    //! return value is empty.
    let req = parse_jsonrpc_request(line)
    // Parse / validation errors: per spec, notifications still must not get a
    // response even when malformed.
    if (!empty(req.error_envelope)) return req.is_notification ? "" : req.error_envelope
    let result_json = compact_json_whitespace(dispatch_command(req.inner_json))
    if (req.is_notification) return ""
    return json_rpc_response(req.id_str, result_json)
}

// --- Reader thread ---

def stdio_reader_main(ch : Channel?) {
    //! Blocking-read stdin lines; push each into the channel. Exits on EOF.
    let sin = fstdin()
    while (!feof(sin)) {
        let msg = build_string() $(var w) {
            while (!feof(sin)) {
                let chunk = fgets(sin) // nolint:PERF003
                let len = length(chunk)
                if (len == 0) break
                if (character_at(chunk, len - 1) == '\n') { // nolint:PERF003
                    if (len > 1 && character_at(chunk, len - 2) == '\r') { // nolint:PERF003
                        write(w, slice(chunk, 0, len - 2))
                    } else {
                        write(w, slice(chunk, 0, len - 1))
                    }
                    break
                } else {
                    write(w, chunk)
                }
            }
        }
        if (!empty(msg)) {
            ch |> push_clone(StdioMsg(line = msg))
        }
    }
}

// --- Agent ---

class LiveApiStdioAgent : DapiDebugAgent {
    inbox : Channel?
    sout : FILE const?
    thread_started : bool = false

    def LiveApiStdioAgent() {
        var ch = unsafe(channel_create())
        inbox = ch
        sout = fstdout()
    }

    def override onTick() : void {
        if (inbox == null || sout == null) return
        if (!thread_started) {
            thread_started = true
            var ch_capture = capture_jobque_channel(inbox)
            new_thread() <| @capture(= ch_capture) {
                this_context().name := "live_api_stdio_reader"
                stdio_reader_main(ch_capture)
                ch_capture = null
            }
        }
        var draining = true
        while (draining) {
            draining = inbox |> try_pop_clone() $(msg : StdioMsg#) {
                let response = handle_jsonrpc_line(string(msg.line))
                if (empty(response)) return  // notification — no response per JSON-RPC 2.0
                fprint(sout, response)
                fprint(sout, "\n")
                fflush(sout)
            }
        }
    }
}

def debug_agent(ctx : Context) {
    assert(this_context().category.debug_context)
    install_new_debug_agent(new LiveApiStdioAgent(), "live_api_stdio")
}

[_macro]
def installing() {
    if (is_compiling_macros_in_module("live_api_stdio") && !is_in_completion()) {
        if (!is_in_debug_agent_creation() && is_live_mode()) {
            if (!has_debug_agent_context("live_api_stdio")) {
                fork_debug_agent_context(@@debug_agent)
            }
        }
    }
}
