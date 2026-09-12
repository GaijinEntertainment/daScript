options gen2
options indenting = 4

require daslib/fio
require daslib/command_line
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require daslib/strings_convert
require strings
require math
require uriparser
require watchdog

//! The LSP front: this process is the client's Content-Length framed JSON-RPC pipe and the
//! language server. It holds the session - the `{uri -> text}` document shadow, the per-file
//! debounce, the initialize options - and knows no language: every operation spawns a fresh
//! daslang running a subtool (`utils/lsp/subtools/*.das`) that prints LSP-shaped JSON and exits,
//! the client's buffer riding along as a `--overlay` file. One loop polls stdin with the time to
//! the next due validate as its timeout, so a validate fires while the client is silent; a
//! validate runs as a child the loop polls between frames, and an edit of the file kills one
//! still running, so an edit never waits on a compile and a stale buffer's diagnostics never
//! publish. Stdout is the protocol; the log goes to its file only.

let private DEBOUNCE_MS = 100
let private SUBTOOL_TIMEOUT_SEC = 60.0
let private CONTENT_LENGTH_HEADER = "content-length:"
let private MAX_FRAME_BYTES = 64 * 1024 * 1024
let private SERVER_NAME = "daslang-lsp"
let private SERVER_VERSION = "0.5.0"
let private CAPABILITIES = "\{\"textDocumentSync\":\{\"openClose\":true,\"change\":1,\"save\":\{\"includeText\":false\}\},\"definitionProvider\":true,\"hoverProvider\":true,\"referencesProvider\":true,\"documentSymbolProvider\":true,\"workspaceSymbolProvider\":true,\"implementationProvider\":true,\"callHierarchyProvider\":true\}"

def private json_str(s : string) : string {
    return write_json_compact(JV(s))
}

// ===== paths and URIs =====

//! `file://` URI -> local path for this platform; a string uriparser cannot read stays as it is
def public uri_to_path(uri : string) : string {
    let path = uri_to_file_name(uri)
    return empty(path) ? uri : path
}

def public path_to_uri(path : string) : string {
    return file_name_to_uri(get_full_file_name(path))
}

//! the nearest ancestor holding a `.git` entry - a directory in the main tree, a file in a
//! linked worktree; empty when none
def public git_tree_root(start_dir : string) : string {
    if (empty(start_dir)) return ""
    var d = get_full_file_name(start_dir)
    for (_i in range(64)) {
        if (fexist(path_join(d, ".git"))) return d
        let up = dir_name(d)
        if (up == d || empty(up)) return ""
        d = up
    }
    return ""
}

//! the file may not exist yet (an unsaved buffer), so its directory is what canonicalizes
def private under(path, root : string) : bool {
    let p = to_generic_path(path_join(get_full_file_name(dir_name(path)), base_name(path)))
    var r = to_generic_path(get_full_file_name(root))
    if (!(r |> ends_with("/"))) {
        r = "{r}/"
    }
    return get_running_platform_name() == "windows" ? (to_lower(p) |> starts_with(to_lower(r))) : (p |> starts_with(r))
}

//! a diagnostic that carries the endpoint's own trouble, on the file's first character
def public tool_diagnostic(message : string; severity : int = 1) : string {
    return "\{\"range\":\{\"start\":\{\"line\":0,\"character\":0\},\"end\":\{\"line\":0,\"character\":1\}\},\"severity\":{severity},\"source\":\"daslang-lsp\",\"message\":{json_str(message)}\}"
}

//! the warning for a file in another git worktree than the compiler serving it: that binary's
//! compiled-in bindings and module sources are its own tree's. Empty when the trees agree, the
//! compiler has no tree, or `project_root` scopes the file on purpose.
def public crosstree_diagnostic(path, compiler, project_root : string) : string {
    let binary_root = empty(compiler) ? "" : git_tree_root(dir_name(compiler))
    let file_root = git_tree_root(dir_name(path))
    if (empty(binary_root) || empty(file_root) || binary_root == file_root || (!empty(project_root) && under(path, project_root))) return ""
    let has_bin = fexist(path_join(file_root, "bin/daslang")) || fexist(path_join(file_root, "bin/daslang.exe"))
    var hint = "that worktree isn't bootstrapped - run `daslang utils/mcp/setup.das -- --root {file_root}`"
    if (has_bin && fexist(path_join(file_root, ".mcp.json"))) {
        hint = "that worktree is bootstrapped - open a session rooted in {file_root}"
    } elif (has_bin) {
        hint = "that worktree has a binary but no MCP config - run `daslang utils/mcp/setup.das -- --root {file_root} --no-build`"
    }
    return tool_diagnostic("CROSS-TREE: this file is in a different git worktree ({file_root}) than the daslang binder serving it ({binary_root}). Diagnostics use that tree's module sources and its binary's compiled-in C++ bindings, so they may be STALE or wrong here. {hint}.", 2)
}

// ===== the compiler =====

def private exe_name(base : string) : string {
    return get_running_platform_name() == "windows" ? "{base}.exe" : base
}

//! `name` resolved through PATH to an absolute path, or empty
def private find_on_path(name : string) : string {
    let windows = get_running_platform_name() == "windows"
    for (dir in split(get_env_variable("PATH"), windows ? ";" : ":")) {
        continue if (empty(dir))
        let cand = path_join(dir, exe_name(name))
        if (fexist(cand)) return get_full_file_name(cand)
    }
    return ""
}

//! the daslang binary the subtools run under, absolute: the option, the environment, the
//! interpreter host itself, the exe beside the watchdog, the workspace's and the tree's
//! layouts, else PATH; empty when none resolves
def private find_compiler(option, program_dir : string) : string {
    if (!empty(option) && fexist(option)) return get_full_file_name(option)
    let env = get_env_variable("DASLANG_LSP_COMPILER")
    if (!empty(env) && fexist(env)) return get_full_file_name(env)
    let self_exe = get_full_file_name(get_das_exe())
    if (base_name(self_exe) |> starts_with("daslang")) return self_exe
    let beside = path_join(program_dir, exe_name("daslang"))
    if (fexist(beside)) return beside
    for (root in [get_env_variable("CLAUDE_PROJECT_DIR"), get_das_root()]) {
        continue if (empty(root))
        for (rel in ["bin/Release/daslang.exe", "bin/Release/daslang", "bin/daslang", "bin/daslang.exe", "build/daslang", "build/bin/daslang"]) {
            let cand = path_join(root, rel)
            if (fexist(cand)) return get_full_file_name(cand)
        }
    }
    return find_on_path("daslang")
}

let private NO_COMPILER = "daslang binary not found - set initializationOptions.compiler, $DASLANG_LSP_COMPILER, or put daslang on PATH"

// ===== the frame reader =====

enum private FrameRead {
    ok
    closed
    bad_header
}

//! one Content-Length frame's body from `sin`: `closed` at the end of input, `bad_header` on
//! a header block with no usable length - the stream is out of sync, and the next bytes would
//! be read as headers
def private read_frame(sin : file; var body : array<uint8>&) : FrameRead {
    var content_length = -1
    while (true) {
        let raw = fgets(sin)
        if (empty(raw)) return FrameRead.closed
        let line = rtrim(raw, "\r\n")
        break if (empty(line))
        if (to_lower(line) |> starts_with(CONTENT_LENGTH_HEADER)) {
            let parsed = try_to_int(strip(slice(line, length(CONTENT_LENGTH_HEADER))))
            content_length = is_ok(parsed) ? unwrap(parsed) : -1
        }
    }
    if (content_length <= 0 || content_length > MAX_FRAME_BYTES) return FrameRead.bad_header
    body |> resize(content_length)
    return fread(sin, body) == content_length ? FrameRead.ok : FrameRead.closed
}

// ===== the front =====

struct private Frame {
    method : string
    has_id : bool
    id_text : string
}

//! a validate in flight: the child, its overlay file, when it started, what it printed so far
struct private Validate {
    proc : process
    overlay : string
    started : int64
    out : array<string>
}

class public LspFront {
    log : Emitter?
    compiler : string
    subtools : string
    program_dir : string
    docs : table<string; string>
    due : table<string; int64>
    inflight : table<string; Validate>
    active_uri : string
    opt_project : string
    opt_project_root : string
    opt_load_modules : array<string>
    done : bool = false
    result : int = 0
    sin : file
    sout : file

    def LspFront(log_path, compiler_option, at : string) {
        log = new Emitter(log_path)
        log.tee = false
        program_dir = at
        compiler = find_compiler(compiler_option, at)
        subtools = path_join(get_das_root(), "utils/lsp/subtools")
        sout = fstdout()
        sin = fstdin()
        funbuffered(sin)
        log->emit("watchdog_started", JV({"cwd" => JV(getcwd()), "compiler" => JV(compiler), "subtools" => JV(subtools)}))
    }

    def finalize() {
        cancel_validates()
        unsafe {
            delete log
        }
    }

    // ----- wire -----

    def private write_frame(body : string) {
        fprint(sout, "Content-Length: {length(body)}\r\n\r\n{body}")
        fflush(sout)
    }

    def private reply(id_text, result_json : string) {
        let id = empty(id_text) ? "null" : id_text
        write_frame("\{\"jsonrpc\":\"2.0\",\"id\":{id},\"result\":{result_json}\}")
    }

    def private reply_error(id_text : string; code : int; message : string) {
        let id = empty(id_text) ? "null" : id_text
        write_frame("\{\"jsonrpc\":\"2.0\",\"id\":{id},\"error\":\{\"code\":{code},\"message\":{json_str(message)}\}\}")
    }

    def private publish(uri, diagnostics_json : string) {
        write_frame("\{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/publishDiagnostics\",\"params\":\{\"uri\":{json_str(uri)},\"diagnostics\":{diagnostics_json}\}\}")
        log->emit("published", JV({"uri" => JV(uri)}))
    }

    // ----- subtools -----

    def private subtool_argv(name : string; args : array<string>) : array<string> {
        var argv <- [compiler]
        argv |> reserve(4 + 2 * length(opt_load_modules) + length(args))
        if (!empty(opt_project_root)) {
            argv |> push("-project_root")
            argv |> push(opt_project_root)
        }
        for (lm in opt_load_modules) {
            argv |> push("-load_module")
            argv |> push(lm)
        }
        argv |> push(path_join(subtools, "{name}.das"))
        argv |> push("--")
        argv |> push_from(args)
        return <- argv
    }

    //! the document shadow of `uri` as a file for the subtool's `--overlay`; empty when the
    //! document is not open. The caller removes it.
    def private write_overlay(uri : string) : string {
        if (!(docs |> key_exists(uri))) return ""
        let text = docs[uri]
        var err : string
        let path = create_temp_file("daslang_lsp_overlay_", ".das", err)
        if (empty(path) || !fwrite(path, text)) {
            log->emit("overlay_error", JV({"uri" => JV(uri), "error" => JV(err)}))
            return ""
        }
        return path
    }

    //! the JSON object a subtool printed, found past any noise before it; null with the tail of
    //! what it printed instead
    def private parse_payload(out : string; rc : int; var tail : string&) : JsonValue? {
        var js : JsonValue? = null
        peek_data(out) $(d) {
            var at = find(d, "\{")
            while (at >= 0 && js == null) {
                var err : string
                js = read_json(slice(d, at), err)
                at = find(d, "\{", at + 1)
            }
        }
        if (js == null) {
            let text = strip(out)
            tail = length(text) > 400 ? slice(text, length(text) - 400) : text
            if (rc != 0 && empty(tail)) {
                tail = "exit code {rc}"
            }
        }
        return js
    }

    //! runs a subtool to completion and returns its JSON payload, or null with the tail of what
    //! it printed
    def private run_subtool(var argv : array<string>; overlay_uri : string; var tail : string&) : JsonValue? {
        let overlay = write_overlay(overlay_uri)
        if (!empty(overlay)) {
            argv |> push("--overlay")
            argv |> push(overlay)
        }
        var out : string
        let rc = run_and_capture(argv, out, SUBTOOL_TIMEOUT_SEC)
        if (!empty(overlay)) {
            remove(overlay)
        }
        return parse_payload(out, rc, tail)
    }

    // ----- validate -----

    def private schedule_validate(uri : string) {
        due[uri] = ref_time_ticks()
    }

    //! ends a validate in flight for `uri`, unpublished
    def private cancel_validate(uri : string) {
        if (!(inflight |> key_exists(uri))) return
        unsafe(close_process(inflight[uri].proc))
        if (!empty(inflight[uri].overlay)) {
            remove(inflight[uri].overlay)
        }
        inflight |> erase(uri)
    }

    def private cancel_validates() {
        for (uri in [for (u in keys(inflight)); u]) {
            cancel_validate(uri)
        }
    }

    //! spawns the validate child for `uri`
    def private start_validate(uri : string) {
        cancel_validate(uri)
        if (empty(compiler)) {
            let missing = tool_diagnostic(NO_COMPILER)
            publish(uri, "[{missing}]")
            return
        }
        var args <- [uri_to_path(uri)]
        if (!empty(opt_project)) {
            args |> push(opt_project)
        }
        var argv <- subtool_argv("validate", args)
        let overlay = write_overlay(uri)
        if (!empty(overlay)) {
            argv |> push("--overlay")
            argv |> push(overlay)
        }
        let noenv : array<string>
        var p = unsafe(spawn_process(argv, "", noenv))
        if (p == null) {
            if (!empty(overlay)) {
                remove(overlay)
            }
            let failed = tool_diagnostic("daslang validate crashed: spawn failed")
            publish(uri, "[{failed}]")
            return
        }
        inflight[uri] <- Validate(proc = p, overlay = overlay, started = ref_time_ticks())
    }

    //! the diagnostics for `uri` from what its validate printed, the cross-tree warning first
    def private finish_validate(uri, out : string; rc : int) {
        var tail : string
        var js = parse_payload(out, rc, tail)
        var diags = "[]"
        if (js == null) {
            let crashed = tool_diagnostic("daslang validate crashed: {tail}")
            diags = "[{crashed}]"
        } else {
            let node = js?["diagnostics"]
            if (node != null && node.value is _array) {
                diags = write_json_compact(node)
            }
        }
        let xt = crosstree_diagnostic(uri_to_path(uri), compiler, opt_project_root)
        if (!empty(xt)) {
            diags = diags == "[]" ? "[{xt}]" : "[{xt},{slice(diags, 1)}"
        }
        delete_json(js)
        publish(uri, diags)
    }

    //! drains every validate in flight and publishes the ones that ended; one past the timeout
    //! is killed and reported as such
    def private poll_validates() {
        for (uri in [for (u in keys(inflight)); u]) {
            var lines : array<string>
            unsafe(process_drain(inflight[uri].proc) $(line) {
                lines |> push(clone_string(line))
            })
            inflight[uri].out |> push_from(lines)
            let rc = unsafe(process_poll(inflight[uri].proc))
            if (rc == process_running) {
                if (get_time_usec(inflight[uri].started) > int(SUBTOOL_TIMEOUT_SEC) * 1000000) {
                    cancel_validate(uri)
                    let late = tool_diagnostic("daslang validate timed out after {int(SUBTOOL_TIMEOUT_SEC)}s")
                    publish(uri, "[{late}]")
                }
                continue
            }
            var rest : array<string>
            unsafe(process_drain(inflight[uri].proc) $(line) {
                rest |> push(clone_string(line))
            })
            inflight[uri].out |> push_from(rest)
            let out = join(inflight[uri].out, "\n")
            cancel_validate(uri)
            finish_validate(uri, out, rc)
        }
    }

    //! starts the validates whose debounce has elapsed
    def private run_due_validates() {
        var ready <- [for (uri, at in keys(due), values(due)); uri; where get_time_usec(at) >= DEBOUNCE_MS * 1000]
        for (uri in ready) {
            due |> erase(uri)
            start_validate(uri)
        }
    }

    //! how long the next wait on stdin may last: to the next due validate, a short poll while one
    //! is in flight, else a second
    def private wait_ms() : int {
        var wait = empty(inflight) ? 1000 : 10
        for (at in values(due)) {
            let left = DEBOUNCE_MS - get_time_usec(at) / 1000
            wait = clamp(left, 1, wait)
        }
        return wait
    }

    // ----- navigation -----

    def private locations_json(payload : JsonValue?) : string {
        return build_string() $(var w) {
            write(w, "[")
            var first = true
            let locs = payload?["locations"]
            if (locs != null && locs.value is _array) {
                for (l in locs.value as _array) {
                    if (!first) {
                        write(w, ",")
                    }
                    first = false
                    let uri = json_str(path_to_uri(l?["path"] ?? ""))
                    let span = write_json_compact(l?["range"])
                    write(w, "\{\"uri\":{uri},\"range\":{span}\}")
                }
            }
            write(w, "]")
        }
    }

    def private hier_item_json(it : JsonValue?; data_uri : string) : string {
        let name = it?["name"] ?? ""
        let symbol = it?["symbol"] ?? ""
        let detail = it?["detail"] ?? ""
        let name_json = json_str(name)
        let kind = write_json_compact(it?["kind"])
        let uri = json_str(path_to_uri(it?["path"] ?? ""))
        let span = write_json_compact(it?["range"])
        let selection = write_json_compact(it?["selectionRange"])
        let data_uri_json = json_str(data_uri)
        let file = json_str(it?["file"] ?? "")
        let line = write_json_compact(it?["line"])
        let data_name = json_str(empty(symbol) ? name : symbol)
        return build_string() $(var w) {
            write(w, "\{\"name\":{name_json},\"kind\":{kind},\"uri\":{uri},\"range\":{span},\"selectionRange\":{selection}")
            write(w, ",\"data\":\{\"uri\":{data_uri_json},\"file\":{file},\"line\":{line},\"name\":{data_name}\}")
            if (!empty(detail)) {
                let detail_json = json_str(detail)
                write(w, ",\"detail\":{detail_json}")
            }
            write(w, "\}")
        }
    }

    def private calls_json(payload : JsonValue?; key, data_uri : string) : string {
        return build_string() $(var w) {
            write(w, "[")
            var first = true
            let calls = payload?["calls"]
            if (calls != null && calls.value is _array) {
                for (c in calls.value as _array) {
                    if (!first) {
                        write(w, ",")
                    }
                    first = false
                    let item = hier_item_json(c?["item"], data_uri)
                    let has_ranges = c.value is _object && ((c.value as _object) |> key_exists("fromRanges"))
                    let ranges_json = has_ranges ? write_json_compact(c?["fromRanges"]) : "[]"
                    write(w, "\{\"{key}\":{item},\"fromRanges\":{ranges_json}\}")
                }
            }
            write(w, "]")
        }
    }

    def private symbols_json(payload : JsonValue?; workspace : bool) : string {
        let syms = payload?["symbols"]
        if (syms == null || !(syms.value is _array)) return "[]"
        if (!workspace) return write_json_compact(syms)
        return build_string() $(var w) {
            write(w, "[")
            var first = true
            for (s in syms.value as _array) {
                if (!first) {
                    write(w, ",")
                }
                first = false
                let name = json_str(s?["name"] ?? "")
                let kind = write_json_compact(s?["kind"])
                let container = json_str(s?["containerName"] ?? "")
                let uri = json_str(path_to_uri(s?["path"] ?? ""))
                let span = write_json_compact(s?["range"])
                write(w, "\{\"name\":{name},\"kind\":{kind},\"containerName\":{container},\"location\":\{\"uri\":{uri},\"range\":{span}\}\}")
            }
            write(w, "]")
        }
    }

    //! the subtool's payload as the LSP result the operation promises
    def private shape_nav_result(op : string; payload : JsonValue?; data_uri : string) : string {
        if (op == "definition") {
            let locs = locations_json(payload)
            return locs == "[]" ? "null" : locs
        } elif (op == "references" || op == "implementation") {
            return locations_json(payload)
        } elif (op == "prepareCallHierarchy") {
            let items = payload?["items"]
            if (items == null || !(items.value is _array) || empty(items.value as _array)) return "null"
            return build_string() $(var w) {
                write(w, "[")
                var first = true
                for (it in items.value as _array) {
                    if (!first) {
                        write(w, ",")
                    }
                    first = false
                    write(w, hier_item_json(it, data_uri))
                }
                write(w, "]")
            }
        } elif (op == "incomingCalls") {
            return calls_json(payload, "from", data_uri)
        } elif (op == "outgoingCalls") {
            return calls_json(payload, "to", data_uri)
        } elif (op == "hover") {
            let contents = payload?["contents"] ?? ""
            if (empty(contents)) return "null"
            let has_range = payload?["hasRange"] ?? false
            let range_json = write_json_compact(payload?["range"])
            let span = has_range ? ",\"range\":{range_json}" : ""
            let value = json_str(contents)
            return "\{\"contents\":\{\"kind\":\"markdown\",\"value\":{value}\}{span}\}"
        } elif (op == "documentSymbol") {
            return symbols_json(payload, false)
        } elif (op == "workspaceSymbol") {
            return symbols_json(payload, true)
        }
        return "null"
    }

    def private nav(id_text, op : string; args : array<string>; overlay_uri, data_uri : string) {
        if (empty(compiler)) {
            reply_error(id_text, -32000, NO_COMPILER)
            return
        }
        var full <- clone_to_move(args)
        if (!empty(opt_project)) {
            full |> push(opt_project)
        }
        var op_args <- [op]
        op_args |> push_from(full)
        var argv <- subtool_argv("nav", op_args)
        var tail : string
        var payload = run_subtool(argv, overlay_uri, tail)
        if (payload == null) {
            reply_error(id_text, -32000, "daslang {op} crashed: {tail}")
            return
        }
        log->emit("nav", JV({"op" => JV(op), "ok" => JV(payload?["ok"] ?? false)}))
        reply(id_text, shape_nav_result(op, payload, data_uri))
        delete_json(payload)
    }

    def private cursor_args(params : JsonValue?) : array<string> {
        let path = uri_to_path(params?["textDocument"]?["uri"] ?? "")
        let line = params?["position"]?["line"] ?? 0
        let character = params?["position"]?["character"] ?? 0
        return <- [path, "{line}", "{character}"]
    }

    // ----- dispatch -----

    def private read_init_options(params : JsonValue?) {
        let io = params?["initializationOptions"]
        let option_compiler = io?["compiler"] ?? ""
        if (!empty(option_compiler)) {
            compiler = find_compiler(option_compiler, program_dir)
        }
        opt_project = io?["project"] ?? ""
        if (!empty(opt_project)) {
            opt_project = get_full_file_name(opt_project)
        }
        opt_project_root = io?["project_root"] ?? ""
        if (!empty(opt_project_root)) {
            opt_project_root = get_full_file_name(opt_project_root)
        }
        opt_load_modules |> clear()
        let lm = io?["load_module"]
        if (lm != null && lm.value is _array) {
            for (m in lm.value as _array) {
                if (m.value is _string) {
                    opt_load_modules |> push(get_full_file_name(m.value as _string))
                }
            }
        }
    }

    def private handle(js : JsonValue?; frame : Frame) {
        let method = frame.method
        let params = js?["params"]
        let doc_uri = params?["textDocument"]?["uri"] ?? ""
        if (method == "initialize") {
            read_init_options(params)
            reply(frame.id_text, "\{\"capabilities\":{CAPABILITIES},\"serverInfo\":\{\"name\":\"{SERVER_NAME}\",\"version\":\"{SERVER_VERSION}\"\}\}")
        } elif (method == "shutdown") {
            reply(frame.id_text, "null")
        } elif (method == "exit") {
            cancel_validates()
            done = true
        } elif (method == "textDocument/didOpen") {
            docs[doc_uri] = params?["textDocument"]?["text"] ?? ""
            active_uri = doc_uri
            cancel_validate(doc_uri)
            schedule_validate(doc_uri)
        } elif (method == "textDocument/didChange") {
            active_uri = doc_uri
            cancel_validate(doc_uri)
            let changes = params?["contentChanges"]
            if (changes != null && changes.value is _array && !empty(changes.value as _array)) {
                let last = (changes.value as _array)[length(changes.value as _array) - 1]
                let ranged = last != null && last.value is _object && ((last.value as _object) |> key_exists("range"))
                if (!ranged) {
                    docs[doc_uri] = last?["text"] ?? ""
                }
            }
            schedule_validate(doc_uri)
        } elif (method == "textDocument/didSave") {
            return
        } elif (method == "textDocument/didClose") {
            docs |> erase(doc_uri)
            due |> erase(doc_uri)
            cancel_validate(doc_uri)
            publish(doc_uri, "[]")
        } else {
            handle_request(js, frame)
        }
    }

    //! the requests that spawn the nav subtool, and the one nobody implements
    def private handle_request(js : JsonValue?; frame : Frame) {
        let method = frame.method
        let params = js?["params"]
        let doc_uri = params?["textDocument"]?["uri"] ?? ""
        if (method == "textDocument/definition" || method == "textDocument/hover" || method == "textDocument/implementation") {
            nav(frame.id_text, slice(method, 13), cursor_args(params), doc_uri, "")
        } elif (method == "textDocument/prepareCallHierarchy") {
            nav(frame.id_text, "prepareCallHierarchy", cursor_args(params), doc_uri, doc_uri)
        } elif (method == "callHierarchy/incomingCalls" || method == "callHierarchy/outgoingCalls") {
            let data = params?["item"]?["data"]
            let data_uri = data?["uri"] ?? ""
            let name = data?["name"] ?? ""
            if (empty(data_uri) || empty(name)) {
                reply(frame.id_text, "[]")
            } else {
                let op = method == "callHierarchy/incomingCalls" ? "incomingCalls" : "outgoingCalls"
                let data_line = data?["line"] ?? 0
                nav(frame.id_text, op, [uri_to_path(data_uri), data?["file"] ?? "", "{data_line}", name], data_uri, data_uri)
            }
        } elif (method == "textDocument/references") {
            let include_decl = params?["context"]?["includeDeclaration"] ?? false
            var args <- cursor_args(params)
            args |> push(include_decl ? "true" : "false")
            nav(frame.id_text, "references", args, doc_uri, "")
        } elif (method == "textDocument/documentSymbol") {
            nav(frame.id_text, "documentSymbol", [uri_to_path(doc_uri)], doc_uri, "")
        } elif (method == "workspace/symbol") {
            if (empty(active_uri)) {
                reply(frame.id_text, "[]")
            } else {
                nav(frame.id_text, "workspaceSymbol", [params?["query"] ?? "", uri_to_path(active_uri)], active_uri, "")
            }
        } elif (frame.has_id) {
            reply_error(frame.id_text, -32601, "daslang-lsp: method not implemented: {method}")
        }
    }

    def private handle_frame(bytes : array<uint8>) {
        var err : string
        var js = read_json(bytes, err)
        if (js == null || !(js.value is _object)) {
            log->emit("parse_error", JV({"error" => JV(err)}))
            delete_json(js)
            return
        }
        var frame = Frame(method = js?["method"] ?? "")
        if ((js.value as _object) |> key_exists("id")) {
            frame.has_id = true
            let id = js?["id"]
            frame.id_text = id == null ? "null" : write_json_compact(id)
        }
        log->emit("received", JV({"method" => JV(frame.method), "id" => JV(frame.id_text)}))
        try {
            handle(js, frame)
        } recover {
            log->emit("handle_error", JV({"method" => JV(frame.method)}))
            if (frame.has_id) {
                reply_error(frame.id_text, -32000, "daslang-lsp internal error handling {frame.method}")
            }
        }
        delete_json(js)
    }

    //! one wait: the next client frame or the next due validate. True when the front is done.
    def tick() : bool {
        if (done) return true
        if (fpoll(sin, wait_ms())) {
            var body : array<uint8>
            let got = read_frame(sin, body)
            if (got == FrameRead.ok) {
                handle_frame(body)
            } else {
                let reason = got == FrameRead.closed ? "stdin closed" : "a frame with no usable Content-Length"
                log->emit("watchdog_stopped", JV({"reason" => JV(reason)}))
                cancel_validates()
                done = true
            }
        }
        if (!done) {
            poll_validates()
            run_due_validates()
        }
        return done
    }
}

//! `args` is the command line after the executable: `--cwd <dir>` is the workspace the subtools
//! run in (the default is where the client started us), `--compiler <bin>` the daslang binary,
//! `--log <file>` the log (else `$DASLANG_LSP_LOG`, else `daslang_lsp.log` in the temp
//! directory). `program_dir` is where the watchdog sits. A configuration error goes to stderr:
//! stdout belongs to the client.
def public lsp_front_start(args : array<string>; program_dir : string; var exit_code : int&) : LspFront? {
    var cwd = ""
    var compiler = ""
    var log_path = get_env_variable("DASLANG_LSP_LOG")
    var i = 0
    while (i < length(args)) {
        let arg = args[i]
        break if (arg == "--")
        i++
        if (arg == "--cwd" || arg == "--compiler" || arg == "--log") {
            if (i >= length(args) || args[i] == "--") {
                fprint(fstderr(), "watchdog --lsp: {arg} needs a value\n")
                exit_code = 2
                return null
            }
            if (arg == "--cwd") {
                cwd = args[i]
            } elif (arg == "--compiler") {
                compiler = args[i]
            } else {
                log_path = args[i]
            }
            i++
        }
    }
    if (!empty(cwd) && !chdir(cwd)) {
        fprint(fstderr(), "watchdog --lsp: cannot enter {cwd}\n")
        exit_code = 2
        return null
    }
    if (empty(log_path)) {
        var err : string
        let tmp = temp_directory(err)
        log_path = path_join(empty(tmp) ? "." : tmp, "daslang_lsp.log")
    }
    return new LspFront(log_path, compiler, program_dir)
}
