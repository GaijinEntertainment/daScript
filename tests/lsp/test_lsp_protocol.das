options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require daslib/fio
require daslib/command_line
require daslib/json_boost
require daslib/strings_boost
require strings

//! Protocol tests for utils/lsp/lsp_supervisor.py, driven over a stdio pipe
//! (precedent: tests/mcp/test_popen_argv_pipe.das). One live session covers:
//! initialize handshake, didOpen with broken buffer text while the disk file
//! stays clean (publishDiagnostics must reflect the buffer — the --overlay
//! path), didChange back to clean (empty publish), a definition request, and
//! shutdown/exit. Skips with a log notice when python is not on PATH.

// ===== outgoing LSP messages (sprint_json envelopes) =====

struct InitializeOptions {
    compiler : string
}

struct InitializeParams {
    initializationOptions : InitializeOptions
}

struct InitializeMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string = "initialize"
    params : InitializeParams
}

struct TextDocumentItem {
    uri : string
    languageId : string
    version : int
    text : string
}

struct DidOpenParams {
    textDocument : TextDocumentItem
}

struct DidOpenMsg {
    jsonrpc : string = "2.0"
    method : string = "textDocument/didOpen"
    params : DidOpenParams
}

struct VersionedDoc {
    uri : string
    version : int
}

struct ContentChange {
    text : string
}

struct DidChangeParams {
    textDocument : VersionedDoc
    contentChanges : array<ContentChange>
}

struct DidChangeMsg {
    jsonrpc : string = "2.0"
    method : string = "textDocument/didChange"
    params : DidChangeParams
}

struct DocId {
    uri : string
}

struct Position0 {
    line : int
    character : int
}

struct CursorParams {
    textDocument : DocId
    position : Position0
}

struct DefinitionMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string = "textDocument/definition"
    params : CursorParams
}

struct PlainMsg {
    jsonrpc : string = "2.0"
    method : string
}

struct PlainRequestMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string
}

// ===== wire helpers =====

def private write_frame(w : file; body : string) {
    fprint(w, "Content-Length: {length(body)}\r\n\r\n{body}")
    fflush(w)
}

// one Content-Length frame -> parsed JSON; null on EOF / bad frame
def private read_frame(r : file) : JsonValue? {
    var content_length = 0
    while (true) {
        let raw = fgets(r)
        if (empty(raw)) {
            return null  // EOF — supervisor died or closed stdout
        }
        let line = rtrim(raw, "\r\n")
        break if (empty(line))
        if (line |> starts_with("Content-Length:")) {
            content_length = to_int(strip(slice(line, 15)))
        }
    }
    return null if (content_length <= 0)
    var buf : array<uint8>
    buf |> resize(content_length)
    let got = fread(r, buf)
    var jerr = ""
    var js = got == content_length ? read_json(buf, jerr) : null
    delete buf
    return js
}

// frames until pred matches (skipping unrelated notifications); null on EOF
def private read_until(r : file; pred : block<(js : JsonValue?) : bool>) : JsonValue? {
    for (_attempt in range(50)) {
        var js = read_frame(r)
        return null if (js == null)
        return js if (invoke(pred, js))
    }
    return null
}

def private diagnostics_of(js : JsonValue?) : int {
    let d = js?["params"]?["diagnostics"]
    if (d != null && d is _array) {
        return length(d as _array)
    }
    return -1
}

// "python3" everywhere except Windows installs that only ship "python";
// probe both, verify the output to dodge the Windows Store python3 alias
def private find_python() : string {
    for (cand in ["python3", "python"]) {
        var looks_like_python = false
        var rc = -1
        unsafe {
            rc = popen_argv_pipe([cand, "--version"]) $(stdin_w, stdout_r) {
                return if (stdin_w == null || stdout_r == null)
                let line = fgets(stdout_r)
                looks_like_python = !empty(line) && (line |> starts_with("Python"))
            }
        }
        return cand if (rc == 0 && looks_like_python)
    }
    return ""
}

def private path_to_uri(path : string) : string {
    let gp = to_generic_path(path)
    return gp |> starts_with("/") ? "file://{gp}" : "file:///{gp}"
}

// ===== the test =====

[test]
def test_lsp_protocol(t : T?) {
    let python = find_python()
    if (empty(python)) {
        to_log(LOG_WARNING, "lsp: no python3/python on PATH — skipping LSP protocol test\n")
        return
    }
    let supervisor = path_join(get_das_root(), "utils/lsp/lsp_supervisor.py")
    let fixture = path_join(get_das_root(), "tests/lsp/_fixture_clean.das")
    let clean = fread(fixture)
    let broken = replace(clean, "    lsp_probe_helper()", "    zzz(1)\n    lsp_probe_helper()")

    // expected positions derive from the fixture text, so fixture edits don't
    // silently break the assertions
    var def_line0 = -1
    var call_line0 = -1
    var clean_lines <- split(clean, "\n")
    for (i in range(length(clean_lines))) {
        if (clean_lines[i] |> starts_with("def lsp_probe_helper")) {
            def_line0 = i
        } elif (clean_lines[i] |> starts_with("    lsp_probe_helper")) {
            call_line0 = i
        }
    }
    delete clean_lines
    t |> success(def_line0 >= 0 && call_line0 >= 0, "fixture contains helper def and call")
    return if (def_line0 < 0 || call_line0 < 0)
    let uri = path_to_uri(fixture)

    t |> run("supervisor protocol end-to-end") @(t : T?) {
        var rc = -1
        unsafe {
            rc = popen_argv_pipe([python, supervisor]) $(w, r) {
                if (w == null || r == null) {
                    t |> failure("could not spawn LSP supervisor via {python}")
                    return
                }
                // initialize — the compiler override makes the run hermetic;
                // absolute path because subtools spawn with per-request cwd
                write_frame(w, sprint_json(InitializeMsg(id = 1, params = InitializeParams(
                    initializationOptions = InitializeOptions(
                        compiler = get_full_file_name(get_das_exe())))), false))
                let init_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 1
                t |> success(init_resp != null, "initialize response received")
                return if (init_resp == null)
                let server_name = init_resp?["result"]?["serverInfo"]?["name"] ?? ""
                t |> equal(server_name, "daslang-lsp")
                t |> success(init_resp?["result"]?["capabilities"]?["definitionProvider"] ?? false,
                    "definition capability advertised")
                write_frame(w, sprint_json(PlainMsg(method = "initialized"), false))

                // didOpen with BROKEN buffer text; disk stays clean — the
                // publish must reflect the buffer (--overlay end-to-end)
                write_frame(w, sprint_json(DidOpenMsg(params = DidOpenParams(
                    textDocument = TextDocumentItem(uri = uri, languageId = "daslang",
                        version = 1, text = broken))), false))
                let pub1 = read_until(r) $(js) => (js?["method"] ?? "") == "textDocument/publishDiagnostics"
                t |> success(pub1 != null, "publishDiagnostics after didOpen")
                return if (pub1 == null)
                t |> equal(pub1?["params"]?["uri"] ?? "", uri)
                t |> success(diagnostics_of(pub1) >= 1, "broken buffer produced diagnostics")
                let first = pub1?["params"]?["diagnostics"]?[0]
                t |> equal(first?["severity"] ?? -1, 1)
                t |> equal(first?["code"] ?? "", "30341")
                t |> equal(first?["range"]?["start"]?["line"] ?? -1, call_line0)
                t |> equal(first?["range"]?["start"]?["character"] ?? -1, 4)

                // didChange back to the clean text — expect an empty publish
                write_frame(w, sprint_json(DidChangeMsg(params <- DidChangeParams(
                    textDocument = VersionedDoc(uri = uri, version = 2),
                    contentChanges <- [ContentChange(text = clean)])), false))
                let pub2 = read_until(r) $(js) => (js?["method"] ?? "") == "textDocument/publishDiagnostics"
                t |> success(pub2 != null, "publishDiagnostics after didChange")
                t |> equal(diagnostics_of(pub2), 0)

                // definition at the call site resolves to the def line
                write_frame(w, sprint_json(DefinitionMsg(id = 2, params = CursorParams(
                    textDocument = DocId(uri = uri),
                    position = Position0(line = call_line0, character = 4))), false))
                let def_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 2
                t |> success(def_resp != null, "definition response received")
                let loc0 = def_resp?["result"]?[0]
                t |> equal(loc0?["uri"] ?? "", uri)
                t |> equal(loc0?["range"]?["start"]?["line"] ?? -1, def_line0)
                t |> equal(loc0?["range"]?["start"]?["character"] ?? -1, 4)

                // clean teardown
                write_frame(w, sprint_json(PlainRequestMsg(id = 3, method = "shutdown"), false))
                let shutdown_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 3
                t |> success(shutdown_resp != null, "shutdown response received")
                write_frame(w, sprint_json(PlainMsg(method = "exit"), false))
            }
        }
        t |> equal(rc, 0, "supervisor exit code")
    }
}
