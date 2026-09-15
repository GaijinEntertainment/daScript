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
//! an implementation request (base method -> derived override), hover / definition /
//! references on declarations (a class name, its parent, a user finalizer, a lambda's
//! argument and capture, a for loop variable), and shutdown/exit. Every frame header
//! is checked byte-exact.

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

struct ReferenceContext {
    includeDeclaration : bool
}

struct ReferencesParams {
    textDocument : DocId
    position : Position0
    context : ReferenceContext
}

struct ReferencesMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string = "textDocument/references"
    params : ReferencesParams
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

struct DocSymbolParams {
    textDocument : DocId
}

struct DocSymbolMsg {
    jsonrpc : string = "2.0"
    id : int
    method : string = "textDocument/documentSymbol"
    params : DocSymbolParams
}

// ===== wire helpers =====

def private write_frame(w : file; body : string) {
    fprint(w, "Content-Length: {length(body)}\r\n\r\n{body}")
    fflush(w)
}

//! one Content-Length frame -> parsed JSON; null on EOF / bad frame. Header lines are checked
//! byte-exact: the pipe is binary on every platform, and a text-mode stdout on Windows sends \r\r\n
def private read_frame(t : T?; r : file) : JsonValue? {
    var content_length = 0
    while (true) {
        let raw = fgets(r)
        if (empty(raw)) {
            return null  // EOF — supervisor died or closed stdout
        }
        t |> success(raw |> ends_with("\r\n") && !(rtrim(raw, "\n") |> ends_with("\r\r")), "header line ends in exactly CRLF: {replace(replace(raw, "\r", "\\r"), "\n", "\\n")}")
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
def private read_until(t : T?; r : file; pred : block<(js : JsonValue?) : bool>) : JsonValue? {
    for (_attempt in range(50)) {
        var js = read_frame(t, r)
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

struct HoverProbe {
    text : string
    line : int
    character : int
}

//! one hover round trip: the markdown text and the start of the returned range
def private probe_hover(t : T?; w, r : file; id : int; uri : string; line, character : int) : HoverProbe {
    write_frame(w, sprint_json(CursorRequestMsg(id = id, method = "textDocument/hover",
        params = CursorParams(textDocument = DocId(uri = uri), position = Position0(line = line, character = character))), false))
    let resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == id
    return HoverProbe(text = resp?["result"]?["contents"]?["value"] ?? "",
        line = resp?["result"]?["range"]?["start"]?["line"] ?? -1,
        character = resp?["result"]?["range"]?["start"]?["character"] ?? -1)
}

//! one definition round trip: the start of the first location, (-1, -1) when there is none
def private probe_definition(t : T?; w, r : file; id : int; uri : string; line, character : int) : int2 {
    write_frame(w, sprint_json(DefinitionMsg(id = id, params = CursorParams(
        textDocument = DocId(uri = uri), position = Position0(line = line, character = character))), false))
    let resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == id
    let loc = resp?["result"]?[0]
    return int2(loc?["range"]?["start"]?["line"] ?? -1, loc?["range"]?["start"]?["character"] ?? -1)
}

//! one references round trip with the declaration included: every location as "line:character"
def private probe_references(t : T?; w, r : file; id : int; uri : string; line, character : int) : array<string> {
    write_frame(w, sprint_json(ReferencesMsg(id = id, params = ReferencesParams(
        textDocument = DocId(uri = uri), position = Position0(line = line, character = character),
        context = ReferenceContext(includeDeclaration = true))), false))
    let resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == id
    var keys : array<string>
    let arr = resp?["result"]
    if (arr != null && arr.value is _array) {
        keys |> reserve(length(arr.value as _array))
        for (loc in arr.value as _array) {
            keys |> push("{loc?["range"]?["start"]?["line"] ?? -1}:{loc?["range"]?["start"]?["character"] ?? -1}")
        }
    }
    return <- keys
}

// ===== the test =====

[test]
def test_lsp_protocol(t : T?) {   // nolint:STYLE037,STYLE038 one ordered LSP session on one pipe; splitting it breaks the request/response choreography
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
    var base_line0 = -1
    var base_name_char = -1
    var derived_line0 = -1
    var derived_parent_char = -1
    var final_line0 = -1
    var final_name_char = -1
    var final_parent_char = -1
    var finalize_line0 = -1
    var finalize_char = -1
    var new_final_line0 = -1
    var new_final_char = -1
    var lambda_line0 = -1
    var lambda_body_char = -1
    var lambda_arg_char = -1
    var self_line0 = -1
    var self_name_char = -1
    var self_call_char = -1
    var generic_line0 = -1
    var generic_name_char = -1
    var generic_call_char = -1
    var enum_line0 = -1
    var enum_char = -1
    var paint_line0 = -1
    var paint_char = -1
    var count_line0 = -1
    var count_char = -1
    var spoken_line0 = -1
    var spoken_char = -1
    var count_use_line0 = -1
    var count_use_char = -1
    var enum_use_line0 = -1
    var enum_use_char = -1
    var typedef_line0 = -1
    var typedef_char = -1
    var pair_line0 = -1
    var pair_char = -1
    var global_line0 = -1
    var global_char = -1
    var global_use_line0 = -1
    var global_use_char = -1
    var yield_line0 = -1
    var yield_char = -1
    var let_base_line0 = -1
    var let_base_char = -1
    var for_line0 = -1
    var for_char = -1
    var for_use_line0 = -1
    var for_use_char = -1
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
        } elif (clean_lines[i] |> starts_with("class LspProbeBase")) {
            base_line0 = i
            base_name_char = find(clean_lines[i], "LspProbeBase")
        } elif (clean_lines[i] |> starts_with("class LspProbeDerived")) {
            derived_line0 = i
            derived_parent_char = find(clean_lines[i], "LspProbeBase")
        } elif (clean_lines[i] |> starts_with("class LspProbeFinalized")) {
            final_line0 = i
            final_name_char = find(clean_lines[i], "LspProbeFinalized")
            final_parent_char = find(clean_lines[i], "LspProbeBase")
        } elif (clean_lines[i] |> starts_with("    def finalize")) {
            finalize_line0 = i
            finalize_char = find(clean_lines[i], "finalize")
        } elif (clean_lines[i] |> starts_with("    var f = new LspProbeFinalized")) {
            new_final_line0 = i
            new_final_char = find(clean_lines[i], "LspProbeFinalized")
        } elif (clean_lines[i] |> starts_with("    let add = @")) {
            lambda_line0 = i
            lambda_body_char = find(clean_lines[i], "x + base")
            lambda_arg_char = find(clean_lines[i], "x : int")
        } elif (clean_lines[i] |> starts_with("def lsp_probe_self")) {
            self_line0 = i
            self_name_char = find(clean_lines[i], "lsp_probe_self")
            self_call_char = find(clean_lines[i], "lsp_probe_self(n - 1)")
        } elif (clean_lines[i] |> starts_with("def lsp_probe_generic")) {
            generic_line0 = i
            generic_name_char = find(clean_lines[i], "lsp_probe_generic")
            generic_call_char = find(clean_lines[i], "lsp_probe_generic(n - 1)")
        } elif (clean_lines[i] |> starts_with("enum LspProbeColor")) {
            enum_line0 = i
            enum_char = find(clean_lines[i], "LspProbeColor")
        } elif (clean_lines[i] |> starts_with("def lsp_probe_paint")) {
            paint_line0 = i
            paint_char = find(clean_lines[i], "LspProbeColor")
        } elif (clean_lines[i] |> starts_with("    probe_count : int")) {
            count_line0 = i
            count_char = find(clean_lines[i], "probe_count")
        } elif (clean_lines[i] |> starts_with("    let spoken = d->probe_speak()")) {
            spoken_line0 = i
            spoken_char = find(clean_lines[i], "probe_speak")
        } elif (clean_lines[i] |> starts_with("    print(\"\{spoken\} \{d.probe_count\}")) {
            count_use_line0 = i
            count_use_char = find(clean_lines[i], "probe_count")
        } elif (clean_lines[i] |> starts_with("    pair.first = int(LspProbeColor")) {
            enum_use_line0 = i
            enum_use_char = find(clean_lines[i], "LspProbeColor")
        } elif (clean_lines[i] |> starts_with("typedef LspProbePair")) {
            typedef_line0 = i
            typedef_char = find(clean_lines[i], "LspProbePair")
        } elif (clean_lines[i] |> starts_with("    var pair : LspProbePair")) {
            pair_line0 = i
            pair_char = find(clean_lines[i], "LspProbePair")
        } elif (clean_lines[i] |> starts_with("let LSP_PROBE_LIMIT")) {
            global_line0 = i
            global_char = find(clean_lines[i], "LSP_PROBE_LIMIT")
        } elif (clean_lines[i] |> starts_with("        for (i in range(LSP_PROBE_LIMIT")) {
            global_use_line0 = i
            global_use_char = find(clean_lines[i], "LSP_PROBE_LIMIT")
        } elif (clean_lines[i] |> starts_with("            yield i")) {
            yield_line0 = i
            yield_char = find(clean_lines[i], "yield i") + 6
        } elif (clean_lines[i] |> starts_with("    let base = 10")) {
            let_base_line0 = i
            let_base_char = find(clean_lines[i], "base")
        } elif (clean_lines[i] |> starts_with("    for (i in range")) {
            for_line0 = i
            for_char = find(clean_lines[i], "i in")
        } elif (clean_lines[i] |> starts_with("        print(\"\{i\}")) {
            for_use_line0 = i
            for_use_char = find(clean_lines[i], "i\}")
        }
    }
    delete clean_lines
    t |> success(def_line0 >= 0 && call_line0 >= 0, "fixture contains helper def and call")
    t |> success(base_speak_line0 >= 0 && derived_speak_line0 >= 0, "fixture contains probe_speak base and override")
    t |> success(base_line0 >= 0 && derived_line0 >= 0 && final_line0 >= 0 && finalize_line0 >= 0 && new_final_line0 >= 0, "fixture contains the class with a user finalizer and its new site")
    t |> success(lambda_line0 >= 0 && lambda_body_char >= 0, "fixture contains the capturing lambda")
    t |> success(for_line0 >= 0 && for_use_line0 >= 0, "fixture contains the for loop and its body use")
    t |> success(self_line0 >= 0 && self_call_char > self_name_char, "fixture contains the one-line self-calling function")
    t |> success(generic_line0 >= 0 && generic_call_char > generic_name_char, "fixture contains the one-line self-calling generic")
    t |> success(enum_line0 >= 0 && paint_line0 >= 0 && enum_use_line0 >= 0 && typedef_line0 >= 0 && pair_line0 >= 0 && global_line0 >= 0 && global_use_line0 >= 0 && yield_line0 >= 0 && let_base_line0 >= 0, "fixture contains the enum, typedef, global, generator and let probes")
    t |> success(count_line0 >= 0 && spoken_line0 >= 0 && count_use_line0 >= 0, "fixture contains the field, the method call and the field use")
    return if (def_line0 < 0 || call_line0 < 0 || base_speak_line0 < 0 || derived_speak_line0 < 0 || base_line0 < 0 || derived_line0 < 0 || final_line0 < 0 || finalize_line0 < 0 || new_final_line0 < 0 || lambda_line0 < 0 || for_line0 < 0 || for_use_line0 < 0 || self_line0 < 0 || generic_line0 < 0 ||
        enum_line0 < 0 || paint_line0 < 0 || enum_use_line0 < 0 || typedef_line0 < 0 || pair_line0 < 0 || global_line0 < 0 || global_use_line0 < 0 || yield_line0 < 0 || let_base_line0 < 0 || count_line0 < 0 || spoken_line0 < 0 || count_use_line0 < 0)
    let uri = path_to_uri(fixture)

    for (host in front_hosts()) {
      let front_name = host.name
      let front_argv := host.argv
      t |> run("front protocol end-to-end through the {front_name}") @capture(:= front_argv) (t : T ?) {
        let rc = unsafe(popen_argv_pipe(front_argv) $(w, r) {
                if (w == null || r == null) {
                    t |> failure("could not spawn the LSP front: {front_argv}")
                    return
                }
                // initialize — the compiler override makes the run hermetic; absolute path,
                // since the subtool argv must name the binary wherever it is run from
                write_frame(w, sprint_json(InitializeMsg(id = 1, params = InitializeParams(
                    initializationOptions = InitializeOptions(
                        compiler = get_full_file_name(get_das_exe())))), false))
                let init_resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == 1
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
                let pub1 = read_until(t, r) $(js) => (js?["method"] ?? "") == "textDocument/publishDiagnostics"
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
                let pub2 = read_until(t, r) $(js) => (js?["method"] ?? "") == "textDocument/publishDiagnostics"
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
                let pub3 = read_until(t, r) $(js) {
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
                let def_resp = read_until(t, r) $(js) {
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
                let prep_resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == 4
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
                let inc_resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == 5
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
                let out_resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == 6
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
                let impl_resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == 7
                t |> success(impl_resp != null, "implementation response received")
                let impl0 = impl_resp?["result"]?[0]
                t |> equal(uri_for_compare(impl0?["uri"] ?? ""), uri_for_compare(uri))
                t |> equal(impl0?["range"]?["start"]?["line"] ?? -1, derived_speak_line0)
                t |> equal(impl0?["range"]?["start"]?["character"] ?? -1, derived_speak_char)

                let hover_class = probe_hover(t, w, r, 8, uri, final_line0, final_name_char + 2)
                t |> success(hover_class.text |> find("class LspProbeFinalized : LspProbeBase") >= 0, "hover on a class name shows the class: {hover_class.text}")
                t |> success(hover_class.text |> find("__finalize") < 0, "hover on a class name is not the synthesized finalizer: {hover_class.text}")
                t |> equal(hover_class.line, final_line0)
                t |> equal(hover_class.character, final_name_char)

                let hover_parent = probe_hover(t, w, r, 9, uri, final_line0, final_parent_char + 2)
                t |> success(hover_parent.text |> find("class LspProbeBase") >= 0, "hover on the parent name shows the parent: {hover_parent.text}")
                t |> equal(hover_parent.character, final_parent_char)
                t |> equal(probe_definition(t, w, r, 10, uri, final_line0, final_parent_char + 2), int2(base_line0, base_name_char), "definition of the parent name lands on the parent class")

                let hover_final = probe_hover(t, w, r, 11, uri, finalize_line0, finalize_char + 2)
                t |> success(hover_final.text |> find("def finalize(self : LspProbeFinalized") >= 0, "hover on a user finalizer shows it: {hover_final.text}")
                t |> success(hover_final.text |> find("__finalize") < 0, "hover on a user finalizer is not the synthesized one: {hover_final.text}")
                t |> equal(hover_final.character, finalize_char)

                let ref_keys <- probe_references(t, w, r, 12, uri, final_line0, final_name_char + 2)
                t |> equal(length(ref_keys), 2, "references to a class: declaration + new site, got {ref_keys}")
                t |> success(ref_keys |> find_index("{final_line0}:{final_name_char}") >= 0, "references include the declaration once")
                t |> success(ref_keys |> find_index("{new_final_line0}:{new_final_char}") >= 0, "references include the new site")

                let base_keys <- probe_references(t, w, r, 13, uri, base_line0, base_name_char + 2)
                t |> equal(length(base_keys), 3, "references to a base class: declaration + two derived-class lines, got {base_keys}")
                t |> success(base_keys |> find_index("{base_line0}:{base_name_char}") >= 0, "base references include the declaration")
                t |> success(base_keys |> find_index("{derived_line0}:{derived_parent_char}") >= 0, "base references include the first derived class line")
                t |> success(base_keys |> find_index("{final_line0}:{final_parent_char}") >= 0, "base references include the second derived class line")

                let hover_arg = probe_hover(t, w, r, 14, uri, lambda_line0, lambda_body_char)
                t |> success(hover_arg.text |> find("x : int") >= 0, "hover on a lambda argument inside its body: {hover_arg.text}")
                let hover_cap = probe_hover(t, w, r, 15, uri, lambda_line0, lambda_body_char + 4)
                t |> success(hover_cap.text |> find("base : int") >= 0, "hover on a captured variable inside a lambda: {hover_cap.text}")
                t |> success(hover_cap.text |> find("__this") < 0, "a capture is not the lambda's __this: {hover_cap.text}")

                let hover_for = probe_hover(t, w, r, 16, uri, for_line0, for_char)
                t |> success(hover_for.text |> find("i : int") >= 0, "hover on a for loop variable: {hover_for.text}")
                t |> equal(hover_for.character, for_char)
                t |> equal(probe_definition(t, w, r, 17, uri, for_use_line0, for_use_char), int2(for_line0, for_char), "definition of a loop variable's use lands on the header")

                let for_keys <- probe_references(t, w, r, 18, uri, for_line0, for_char)
                t |> equal(length(for_keys), 2, "references to a for loop variable: header + body use, got {for_keys}")
                t |> success(for_keys |> find_index("{for_line0}:{for_char}") >= 0, "for references include the header")
                t |> success(for_keys |> find_index("{for_use_line0}:{for_use_char}") >= 0, "for references include the body use")

                let hover_larg = probe_hover(t, w, r, 19, uri, lambda_line0, lambda_arg_char)
                t |> success(hover_larg.text |> find("x : int") >= 0, "hover on a lambda argument in its header: {hover_larg.text}")
                t |> equal(hover_larg.character, lambda_arg_char)

                // a call of the function on its own def line is a call, not the declaration
                t |> equal(probe_definition(t, w, r, 20, uri, self_line0, self_call_char + 2), int2(self_line0, self_name_char), "definition of a same-line self-call lands on the name in the def")
                t |> equal(probe_definition(t, w, r, 21, uri, generic_line0, generic_call_char + 2), int2(generic_line0, generic_name_char), "definition of a same-line self-call in a generic lands on the name in the def")

                // the other declaration kinds: an enum, a typedef, a module-level let; a cursor in
                // the middle of a declared name; a generator's loop variable under its source name
                let hover_enum = probe_hover(t, w, r, 22, uri, enum_line0, enum_char + 2)
                t |> success(hover_enum.text |> find("enum LspProbeColor") >= 0, "hover on an enum name: {hover_enum.text}")
                t |> equal(hover_enum.character, enum_char)
                let hover_typedef = probe_hover(t, w, r, 23, uri, typedef_line0, typedef_char + 2)
                t |> success(hover_typedef.text |> find("typedef LspProbePair = tuple") >= 0, "hover on a typedef name: {hover_typedef.text}")
                t |> equal(hover_typedef.character, typedef_char)
                let hover_global = probe_hover(t, w, r, 24, uri, global_line0, global_char + 2)
                t |> success(hover_global.text |> find("LSP_PROBE_LIMIT : int") >= 0, "hover on a global's declaration: {hover_global.text}")
                t |> equal(hover_global.character, global_char)
                let hover_yield = probe_hover(t, w, r, 25, uri, yield_line0, yield_char)
                t |> success(hover_yield.text |> find("i : int") >= 0, "hover on a generator's loop variable: {hover_yield.text}")
                t |> success(hover_yield.text |> find("_rename_at_") < 0, "a generator's loop variable shows its source name: {hover_yield.text}")
                let hover_mid = probe_hover(t, w, r, 26, uri, let_base_line0, let_base_char + 2)
                t |> success(hover_mid.text |> find("base : int") >= 0, "hover in the middle of a let variable's name: {hover_mid.text}")
                t |> equal(hover_mid.character, let_base_char)

                let enum_keys <- probe_references(t, w, r, 27, uri, enum_line0, enum_char + 2)
                t |> equal(length(enum_keys), 3, "references to an enum: declaration, the argument type and the constant's use, got {enum_keys}")
                t |> success(enum_keys |> find_index("{enum_line0}:{enum_char}") >= 0, "enum references include the declaration")
                t |> success(enum_keys |> find_index("{enum_use_line0}:{enum_use_char}") >= 0, "enum references include the use")
                t |> success(enum_keys |> find_index("{paint_line0}:{paint_char}") >= 0, "enum references include the argument type")
                let global_keys <- probe_references(t, w, r, 28, uri, global_line0, global_char + 2)
                t |> equal(length(global_keys), 2, "references to a global: declaration + its use, got {global_keys}")
                t |> success(global_keys |> find_index("{global_use_line0}:{global_use_char}") >= 0, "global references include the use inside the generator")
                t |> equal(probe_definition(t, w, r, 29, uri, enum_use_line0, enum_use_char + 2), int2(enum_line0, enum_char), "definition of an enum use lands on the enum")

                let hover_new = probe_hover(t, w, r, 30, uri, new_final_line0, new_final_char + 2)
                t |> success(hover_new.text |> find("class LspProbeFinalized") >= 0, "hover on the type name in new: {hover_new.text}")
                t |> equal(hover_new.character, new_final_char)
                t |> equal(probe_definition(t, w, r, 31, uri, new_final_line0, new_final_char + 2), int2(final_line0, final_name_char), "definition from the type name in new lands on the class")
                t |> equal(probe_definition(t, w, r, 32, uri, pair_line0, pair_char + 2), int2(typedef_line0, typedef_char), "definition from a declared variable's type lands on the typedef")

                t |> equal(probe_definition(t, w, r, 33, uri, paint_line0, paint_char + 2), int2(enum_line0, enum_char), "definition from an argument's enum type lands on the enum")
                let paint_keys <- probe_references(t, w, r, 34, uri, paint_line0, paint_char + 2)
                t |> success(paint_keys |> find_index("{enum_line0}:{enum_char}") >= 0 && paint_keys |> find_index("{paint_line0}:{paint_char}") >= 0 && paint_keys |> find_index("{enum_use_line0}:{enum_use_char}") >= 0, "references from an enum type name: the declaration, the argument type and the constant's use, got {paint_keys}")
                write_frame(w, sprint_json(CursorRequestMsg(id = 35, method = "textDocument/prepareCallHierarchy",
                    params = CursorParams(textDocument = DocId(uri = uri), position = Position0(line = def_line0, character = 6))), false))
                let prep_def = read_until(t, r) $(js) => (js?["id"] ?? -1) == 35
                t |> equal(prep_def?["result"]?[0]?["name"] ?? "", "lsp_probe_helper", "prepareCallHierarchy on the name in the def line")

                t |> equal(probe_definition(t, w, r, 36, uri, spoken_line0, spoken_char + 2), int2(derived_speak_line0, derived_speak_char), "definition of d->probe_speak() lands on the override the receiver runs")
                let hover_call = probe_hover(t, w, r, 37, uri, spoken_line0, spoken_char + 2)
                t |> success(hover_call.text |> find("LspProbeDerived`probe_speak") >= 0, "hover on a method call shows the method: {hover_call.text}")
                t |> equal(hover_call.character, spoken_char, "the hover range starts at the method name, not the arrow")
                let speak_keys <- probe_references(t, w, r, 38, uri, base_speak_line0, base_speak_char + 2)
                t |> equal(length(speak_keys), 2, "references to the base method: its declaration and the call through the class that does not override it, got {speak_keys}")
                t |> success(speak_keys |> find_index("{base_speak_line0}:{base_speak_char}") >= 0, "base method references include the declaration")
                let hover_field = probe_hover(t, w, r, 39, uri, count_line0, count_char + 2)
                t |> success(hover_field.text |> find("probe_count : int") >= 0 && hover_field.text |> find("field") >= 0, "hover on a field's declaration: {hover_field.text}")
                let count_keys <- probe_references(t, w, r, 40, uri, count_line0, count_char + 2)
                t |> success(count_keys |> find_index("{count_line0}:{count_char}") >= 0 && count_keys |> find_index("{count_use_line0}:{count_use_char}") >= 0, "references from a field's declaration: itself and d.probe_count, got {count_keys}")
                write_frame(w, sprint_json(DocSymbolMsg(id = 41, params = DocSymbolParams(textDocument = DocId(uri = uri))), false))
                let syms = read_until(t, r) $(js) => (js?["id"] ?? -1) == 41
                var base_children : array<string>
                let top = syms?["result"]
                if (top != null && top.value is _array) {
                    for (sym in top.value as _array) {
                        continue if ((sym?["name"] ?? "") != "LspProbeBase")
                        let kids = sym?["children"]
                        if (kids != null && kids.value is _array) {
                            for (kid in kids.value as _array) {
                                base_children |> push(kid?["name"] ?? "")
                            }
                        }
                    }
                }
                t |> success(length(base_children) == 2 && base_children[0] == "probe_count" && base_children[1] == "probe_speak", "a class lists its own field and its bare method names, not __rtti, __finalize or the method fields: {base_children}")

                // clean teardown
                write_frame(w, sprint_json(PlainRequestMsg(id = 3, method = "shutdown"), false))
                let shutdown_resp = read_until(t, r) $(js) => (js?["id"] ?? -1) == 3
                t |> success(shutdown_resp != null, "shutdown response received")
                write_frame(w, sprint_json(PlainMsg(method = "exit"), false))
            })
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
