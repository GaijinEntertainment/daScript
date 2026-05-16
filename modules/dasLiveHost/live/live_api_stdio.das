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
//!
//! JSON-RPC 2.0 parsing/envelope/batch handling lives in ``daslib/jsonrpc``;
//! this module is the live_host bridge that wires ``dispatch_command`` to
//! that library plus the stdin/stdout transport. Permissive ``jsonrpc``
//! field handling is the default — pass ``true`` to ``dispatch_line`` for
//! strict §4 compliance.

require daslib/json_boost
require daslib/jsonrpc
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

// --- Per-request dispatch ---

def handle_jsonrpc_line(line : string) : string {
    //! Dispatch a JSON-RPC 2.0 wire line (single or §6 batch) through
    //! ``dispatch_command``. Returns the response envelope ready for the wire,
    //! or an empty string for notifications / all-notification batches.
    return jsonrpc::dispatch_line(line, false) $(method, params_json) {
        let method_str = write_json(JV(method))
        let inner = "\{\"name\":{method_str},\"args\":{params_json}}"
        return jsonrpc::compact_json_whitespace(dispatch_command(inner))
    }
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
