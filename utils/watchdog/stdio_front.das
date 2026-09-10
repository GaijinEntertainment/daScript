options gen2

require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings
require watchdog

//! The stdio front: this process is the client's newline-delimited JSON-RPC pipe and the child is
//! the server. It answers `initialize` and `ping` itself, spawns the child on the first `tools/*`
//! request with the client's `initialize` replayed, forwards one request at a time, and respawns a
//! child that died before a request was delivered - never after one was, since a re-sent tool call
//! could run twice. The log goes to its file only: stdout is the protocol.

//! the daslang MCP server's own InitializeResult (utils/mcp/mcp_core.das), answered here so the
//! client connects before any child exists
struct private ServerInfo {
    name : string
    version : string
}

struct private ToolsCapability {
}

struct private Capabilities {
    tools : ToolsCapability
}

struct private InitializeResult {
    protocolVersion : string
    capabilities : Capabilities
    serverInfo : ServerInfo
}

def public stdio_init_result() : string {
    let result = InitializeResult(protocolVersion = "2025-11-25", serverInfo = ServerInfo(name = "daslang", version = "0.1.0"))
    return sprint_json(result, false)
}

let private SPAWN_ATTEMPTS = 3
let private NOISE_LINES = 40

//! one line without its newline; the server's own loop, since fgets caps a chunk at 16 KB and a
//! tool call's arguments or a listing's result run past that
def private read_line(f : file) : string {
    return build_string() $(var w) {
        while (!feof(f)) {
            let chunk = fgets(f)
            let len = length(chunk)
            if (len == 0) break
            if (character_at(chunk, len - 1) == '\n') { // nolint:PERF003
                let cr = len > 1 && character_at(chunk, len - 2) == '\r' // nolint:PERF003
                write(w, slice(chunk, 0, len - (cr ? 2 : 1)))
                break
            }
            write(w, chunk)
        }
    }
}

struct private ClientMessage {
    line : string
    method : string
    has_id : bool
    id_text : string
}

def private classify(line : string) : ClientMessage {
    var msg = ClientMessage(line = line)
    var err : string
    var js = read_json(line, err)
    if (js == null || !(js.value is _object)) {
        msg.method = ""
    } else {
        let method = js?["method"]
        if (method != null && method.value is _string) {
            msg.method = method.value as _string
        }
        if ((js.value as _object) |> key_exists("id")) {
            msg.has_id = true
            let id = js?["id"]
            msg.id_text = id == null ? "null" : write_json_compact(id)
        }
    }
    unsafe {
        delete js
    }
    return msg
}

def private jsonrpc_result(id_text, result_json : string) : string {
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id_text},\"result\":{result_json}\}"
}

def private jsonrpc_error(id_text : string; code : int; message : string) : string {
    return "\{\"jsonrpc\":\"2.0\",\"id\":{id_text},\"error\":\{\"code\":{code},\"message\":{write_json_compact(JV(message))}\}\}"
}

class public StdioFront {
    cfg : WatchdogConfig = WatchdogConfig()
    log : Emitter?
    command : array<string>
    init_line : string
    initialized : bool = false
    pending : ClientMessage
    has_pending : bool = false
    failed_spawns : int = 0
    done : bool = false
    result : int = 0
    sin : file
    sout : file

    def StdioFront(var config : WatchdogConfig) {
        cfg <- config
        log = new Emitter(cfg.log)
        log.tee = false
        command <- child_command(cfg)
        sin = fstdin()
        sout = fstdout()
    }

    def finalize() {
        unsafe {
            delete log
        }
    }

    def private reply(line : string) {
        fprint(sout, line)
        fprint(sout, "\n")
        fflush(sout)
    }

    def private write_child(w : file; line : string) : bool {
        fprint(w, line)
        fprint(w, "\n")
        fflush(w)
        return !feof(w)
    }

    //! the child's next JSON line; anything else on its stdout is a diagnostic - a compile failure,
    //! a module that did not load - logged and skipped, and carried in the error text if the child
    //! then dies. Empty means the child is gone.
    def private read_child(r : file; var noise : array<string>&) : string {
        while (true) {
            let line = read_line(r)
            if (empty(line)) {
                if (feof(r)) return ""
                continue
            }
            if (character_at(line, 0) == '{') { // nolint:PERF003
                var err : string
                var js = read_json(line, err)
                let ok = js != null
                unsafe {
                    delete js
                }
                if (ok) return line
            }
            log->emit("child_noise", JV({"line" => JV(slice(line, 0, 400))}))
            if (length(noise) < NOISE_LINES) {
                noise |> push(line)
            }
        }
        return ""
    }

    def private death_message(reason : string; noise : array<string>) : string {
        let text = empty(noise) ? reason : "{reason}; daslang child said: {join(noise, " | ")}"
        return slice(text, 0, 2000)
    }

    //! one client line; false at end of input. Everything but `tools/*` is answered here; a
    //! notification reaches the child only while one is up (`w` is null between children), since
    //! the handshake replay re-establishes its state on the next spawn.
    def private serve_client_line(w : file) : bool {
        let line = read_line(sin)
        if (empty(line)) return !feof(sin)
        let msg = classify(line)
        if (empty(msg.method) && !msg.has_id) {
            log->emit("client_noise", JV({"line" => JV(slice(line, 0, 200))}))
            return true
        }
        if (msg.method == "initialize") {
            init_line = line
            reply(jsonrpc_result(msg.id_text, stdio_init_result()))
        } elif (msg.method == "notifications/initialized" || msg.method == "initialized") {
            initialized = true
            if (w != null) {
                write_child(w, line)
            }
        } elif (msg.method == "ping" && msg.has_id) {
            reply(jsonrpc_result(msg.id_text, "\{\}"))
        } elif (!msg.has_id) {
            if (w != null) {
                write_child(w, line)
            }
        } elif (msg.method |> starts_with("tools/")) {
            pending = msg
            has_pending = true
        } else {
            reply(jsonrpc_error(msg.id_text, -32601, "method not found: {msg.method}"))
        }
        return true
    }

    //! the client's `initialize` and, when it followed, `initialized`, so the child's session state
    //! matches the client's before the first real request
    def private replay_handshake(w, r : file; var noise : array<string>&) : bool {
        if (empty(init_line)) return true
        if (!write_child(w, init_line) || empty(read_child(r, noise))) return false
        if (initialized) {
            return write_child(w, "\{\"jsonrpc\":\"2.0\",\"method\":\"notifications/initialized\"\}")
        }
        return true
    }

    //! one child lifetime: local service until a request needs the child, then the child until it
    //! dies or the client's input ends. True when the front is done.
    def tick() : bool {
        while (!done && !has_pending) {
            if (!serve_client_line(null)) {
                done = true
            }
        }
        if (done) return true
        var noise : array<string>
        var answered = false
        log->emit("child_started", JV({"command" => JV(join(command, " "))}))
        var rc : int
        unsafe {
            rc = popen_argv_pipe(command) $(w, r) {
                if (!replay_handshake(w, r, noise)) return
                while (!done) {
                    if (has_pending) {
                        if (!write_child(w, pending.line)) return
                        has_pending = false
                        answered = true
                        let response = read_child(r, noise)
                        if (empty(response)) {
                            reply(jsonrpc_error(pending.id_text, -32000, death_message("the daslang child died while answering", noise)))
                            return
                        }
                        reply(response)
                    }
                    if (!serve_client_line(w)) {
                        done = true
                    }
                }
            }
        }
        log->emit("child_exited", JV({"code" => JV(rc), "answered" => JV(answered)}))
        if (done) return true
        if (answered) {
            failed_spawns = 0
        } else {
            failed_spawns++
            if (failed_spawns >= SPAWN_ATTEMPTS && has_pending) {
                reply(jsonrpc_error(pending.id_text, -32000, death_message("the daslang child died {failed_spawns} times before answering", noise)))
                has_pending = false
                failed_spawns = 0
            }
        }
        return false
    }
}

//! `args` is the command line after the executable; `program_dir` is where the watchdog sits. A
//! configuration error goes to stderr - stdout belongs to the client - and sets `exit_code`. The
//! process enters `--cwd` here: the child inherits it, as it does from the supervisor's spawn,
//! and the client spawned the front from wherever the client runs.
def public stdio_front_start(args : array<string>; program_dir : string; var exit_code : int&) : StdioFront? {
    var resolved <- resolve_config(args, program_dir)
    if (!empty(resolved.error)) {
        fprint(fstderr(), "{resolved.error}\n")
        exit_code = 2
        return null
    }
    if (empty(resolved.cfg.program) && empty(resolved.cfg.script)) {
        fprint(fstderr(), "watchdog --stdio: nothing to serve; pass --program <server> or --script <main.das>\n")
        exit_code = 2
        return null
    }
    if (!chdir(resolved.cfg.cwd)) {
        fprint(fstderr(), "watchdog --stdio: cannot enter {resolved.cfg.cwd}\n")
        exit_code = 2
        return null
    }
    return new StdioFront(resolved.cfg)
}
