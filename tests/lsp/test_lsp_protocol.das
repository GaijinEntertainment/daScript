options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require daslib/fio
require daslib/command_line
require daslib/json_boost
require daslib/strings_boost
require strings
require uriparser

//! Protocol tests for the LSP front (utils/watchdog/lsp_front.das, `watchdog --lsp`), driven
//! over a stdio pipe through both hosts (precedent: tests/mcp/test_popen_argv_pipe.das). One
//! live session covers:
//! initialize handshake, didOpen with broken buffer text while the disk file
//! stays clean (publishDiagnostics must reflect the buffer — the --overlay
//! path), didChange back to clean (empty publish), a definition request, the
//! call-hierarchy loop (prepare -> incomingCalls with the item's data
//! round-tripped verbatim -> outgoingCalls from the returned caller item),
//! an implementation request (base method -> derived override), and
//! shutdown/exit.

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

struct CursorRequestMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string
    params : CursorParams
}

// call-hierarchy item as the client echoes it back: the server only reads
// `data`, so the round-trip envelope carries just that
struct HierItemData {
    uri : string
    file : string
    line : int
    name : string
}

struct HierItemMin {
    data : HierItemData
}

struct HierCallsParams {
    item : HierItemMin
}

struct HierCallsMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string
    params : HierCallsParams
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

// call-hierarchy response entry whose from/to item carries `name`
def private find_call_entry(resp : JsonValue?; key, name : string) : JsonValue const? {
    let arr = resp?["result"]
    if (arr != null && arr is _array) {
        for (i in range(length(arr as _array))) {
            let e = resp?["result"]?[i]
            if ((e?[key]?["name"] ?? "") == name) {
                return e
            }
        }
    }
    return null
}

def private echo_item_data(item : JsonValue const?) : HierItemData {
    return HierItemData(
        uri = item?["data"]?["uri"] ?? "",
        file = item?["data"]?["file"] ?? "",
        line = item?["data"]?["line"] ?? -1,
        name = item?["data"]?["name"] ?? "")
}

def private das_exe() : string {
    return get_full_file_name(get_das_exe())
}

//! the static exe beside the daslang binary, when it is built
def private watchdog_exe() : string {
    let beside = path_join(dir_name(das_exe()), "watchdog{get_platform_name() == "windows" ? ".exe" : ""}")
    return fexist(beside) ? beside : ""
}

struct FrontHost {
    name : string
    argv : array<string>
}

//! the two hosts of the LSP front: the interpreter running utils/watchdog/main.das, and the
//! static exe where it is built
def private front_hosts() : array<FrontHost> {
    var hosts : array<FrontHost>
    hosts |> emplace(FrontHost(name = "interpreter host", argv <- [das_exe(), path_join(get_das_root(), "utils/watchdog/main.das"), "--", "--lsp"]))
    let exe = watchdog_exe()
    if (!empty(exe)) {
        hosts |> emplace(FrontHost(name = "static exe", argv <- [exe, "--lsp"]))
    }
    return <- hosts
}

def private path_to_uri(path : string) : string {
    return file_name_to_uri(path)
}

def private uri_for_compare(uri : string) : string {
    // Windows paths are case-insensitive, and pathlib.resolve() may return
    // different component casing than get_das_root() for the same file.
    return get_platform_name() == "windows" ? to_lower(uri) : uri
}

// ===== the test =====

[test]
def test_lsp_protocol(t : T?) {   // nolint:STYLE038 one ordered LSP session on one pipe; splitting it breaks the request/response choreography
    let fixture = path_join(get_das_root(), "tests/lsp/_fixture_clean.das")
    let clean = fread(fixture)
    let broken = replace(clean, "    lsp_probe_helper()", "    zzz(1)\n    lsp_probe_helper()")

    // expected positions derive from the fixture text, so fixture edits don't
    // silently break the assertions
    var def_line0 = -1
    var call_line0 = -1
    var base_speak_line0 = -1
    var base_speak_char = -1
    var derived_speak_line0 = -1
    var derived_speak_char = -1
    var clean_lines <- split(clean, "\n")
    for (i in range(length(clean_lines))) {
        if (clean_lines[i] |> starts_with("def lsp_probe_helper")) {
            def_line0 = i
        } elif (clean_lines[i] |> starts_with("    lsp_probe_helper")) {
            call_line0 = i
        } elif (clean_lines[i] |> starts_with("    def probe_speak")) {
            base_speak_line0 = i
            base_speak_char = find(clean_lines[i], "probe_speak")
        } elif (clean_lines[i] |> starts_with("    def override probe_speak")) {
            derived_speak_line0 = i
            derived_speak_char = find(clean_lines[i], "probe_speak")
        }
    }
    delete clean_lines
    t |> success(def_line0 >= 0 && call_line0 >= 0, "fixture contains helper def and call")
    t |> success(base_speak_line0 >= 0 && derived_speak_line0 >= 0, "fixture contains probe_speak base and override")
    return if (def_line0 < 0 || call_line0 < 0 || base_speak_line0 < 0 || derived_speak_line0 < 0)
    let uri = path_to_uri(fixture)

    for (host in front_hosts()) {
      let front_name = host.name
      let front_argv := host.argv
      t |> run("front protocol end-to-end through the {front_name}") @capture(:= front_argv) (t : T ?) {
        var rc = -1
        unsafe {
            rc = popen_argv_pipe(front_argv) $(w, r) {
                if (w == null || r == null) {
                    t |> failure("could not spawn the LSP front: {front_argv}")
                    return
                }
                // initialize — the compiler override makes the run hermetic; absolute path,
                // since the subtool argv must name the binary wherever it is run from
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
                t |> success(init_resp?["result"]?["capabilities"]?["callHierarchyProvider"] ?? false,
                    "callHierarchy capability advertised")
                t |> success(init_resp?["result"]?["capabilities"]?["implementationProvider"] ?? false,
                    "implementation capability advertised")
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

                // an edit kills the compile in flight for its file: broken, then clean again
                // once the broken compile has started. A broken publish may land before the
                // clean edit is received (the compile can finish inside the gap on a fast box);
                // the clean publish lands, and nothing stale lands after it
                write_frame(w, sprint_json(DidChangeMsg(params <- DidChangeParams(
                    textDocument = VersionedDoc(uri = uri, version = 3),
                    contentChanges <- [ContentChange(text = broken)])), false))
                sleep(150u)
                write_frame(w, sprint_json(DidChangeMsg(params <- DidChangeParams(
                    textDocument = VersionedDoc(uri = uri, version = 4),
                    contentChanges <- [ContentChange(text = clean)])), false))
                var early = 0
                let pub3 = read_until(r) $(js) {
                    return false if ((js?["method"] ?? "") != "textDocument/publishDiagnostics")
                    if (diagnostics_of(js) != 0) {
                        early++
                        return false
                    }
                    return true
                }
                t |> success(pub3 != null, "publishDiagnostics after the superseding didChange")
                t |> success(early <= 1, "at most the broken compile that finished before the clean edit published")

                // definition at the call site resolves to the def line; a stale publish arriving
                // after the clean one would show up here
                write_frame(w, sprint_json(DefinitionMsg(id = 2, params = CursorParams(
                    textDocument = DocId(uri = uri),
                    position = Position0(line = call_line0, character = 4))), false))
                var stale = 0
                let def_resp = read_until(r) $(js) {
                    if ((js?["method"] ?? "") == "textDocument/publishDiagnostics" && diagnostics_of(js) != 0) {
                        stale++
                    }
                    return (js?["id"] ?? -1) == 2
                }
                t |> success(def_resp != null, "definition response received")
                t |> equal(stale, 0, "no stale publish after the clean one")
                let loc0 = def_resp?["result"]?[0]
                t |> equal(uri_for_compare(loc0?["uri"] ?? ""), uri_for_compare(uri))
                t |> equal(loc0?["range"]?["start"]?["line"] ?? -1, def_line0)
                t |> equal(loc0?["range"]?["start"]?["character"] ?? -1, 4)

                // prepareCallHierarchy at the helper call site -> one item at
                // the def line, data carrying the daslang-side re-id keys
                write_frame(w, sprint_json(CursorRequestMsg(id = 4,
                    method = "textDocument/prepareCallHierarchy",
                    params = CursorParams(
                        textDocument = DocId(uri = uri),
                        position = Position0(line = call_line0, character = 4))), false))
                let prep_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 4
                t |> success(prep_resp != null, "prepareCallHierarchy response received")
                let item0 = prep_resp?["result"]?[0]
                t |> equal(item0?["name"] ?? "", "lsp_probe_helper")
                t |> equal(item0?["kind"] ?? -1, 12)
                t |> equal(item0?["selectionRange"]?["start"]?["line"] ?? -1, def_line0)
                t |> equal(item0?["data"]?["uri"] ?? "", uri)
                t |> equal(item0?["data"]?["line"] ?? -1, def_line0 + 1)  // daslang lines are 1-based
                t |> equal(item0?["data"]?["name"] ?? "", "lsp_probe_helper")

                // incomingCalls with the prepared item's data echoed verbatim
                // (the client round-trips items between prepare and expand)
                write_frame(w, sprint_json(HierCallsMsg(id = 5,
                    method = "callHierarchy/incomingCalls",
                    params = HierCallsParams(item = HierItemMin(
                        data = echo_item_data(item0)))), false))
                let inc_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 5
                t |> success(inc_resp != null, "incomingCalls response received")
                let inc_main = find_call_entry(inc_resp, "from", "main")
                t |> success(inc_main != null, "incomingCalls lists main as a caller")
                t |> equal(inc_main?["fromRanges"]?[0]?["start"]?["line"] ?? -1, call_line0)
                t |> equal(inc_main?["fromRanges"]?[0]?["start"]?["character"] ?? -1, 4)

                // outgoingCalls from the caller item incoming just returned —
                // closes the loop back at the helper
                write_frame(w, sprint_json(HierCallsMsg(id = 6,
                    method = "callHierarchy/outgoingCalls",
                    params = HierCallsParams(item = HierItemMin(
                        data = echo_item_data(inc_main?["from"])))), false))
                let out_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 6
                t |> success(out_resp != null, "outgoingCalls response received")
                let out_helper = find_call_entry(out_resp, "to", "lsp_probe_helper")
                t |> success(out_helper != null, "outgoingCalls lists the helper as a callee")
                t |> equal(out_helper?["to"]?["selectionRange"]?["start"]?["line"] ?? -1, def_line0)
                t |> equal(out_helper?["fromRanges"]?[0]?["start"]?["line"] ?? -1, call_line0)

                // implementation on the base method resolves the derived override
                write_frame(w, sprint_json(CursorRequestMsg(id = 7,
                    method = "textDocument/implementation",
                    params = CursorParams(
                        textDocument = DocId(uri = uri),
                        position = Position0(line = base_speak_line0, character = base_speak_char))), false))
                let impl_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 7
                t |> success(impl_resp != null, "implementation response received")
                let impl0 = impl_resp?["result"]?[0]
                t |> equal(uri_for_compare(impl0?["uri"] ?? ""), uri_for_compare(uri))
                t |> equal(impl0?["range"]?["start"]?["line"] ?? -1, derived_speak_line0)
                t |> equal(impl0?["range"]?["start"]?["character"] ?? -1, derived_speak_char)

                // clean teardown
                write_frame(w, sprint_json(PlainRequestMsg(id = 3, method = "shutdown"), false))
                let shutdown_resp = read_until(r) $(js) => (js?["id"] ?? -1) == 3
                t |> success(shutdown_resp != null, "shutdown response received")
                write_frame(w, sprint_json(PlainMsg(method = "exit"), false))
            }
        }
        t |> equal(rc, 0, "front exit code")
      }
    }
    t |> run("the validated file's record sits in the workspace's module cache") @(t : T?) {
        var records = 0
        fio::dir(".jitted_scripts/module_cache") $(name) {
            if (name |> starts_with("_fixture_clean-") && name |> ends_with(".dascache")) {
                records++
            }
        }
        t |> success(records > 0, "the validated file's record sits in the workspace's module cache")
    }
}
