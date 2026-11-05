options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of examples/dasLLAMA/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang examples/dasLLAMA/REVIEW.das - exit 0 clean, 1 with findings.

let EXAMPLES_DIR = "examples/dasLLAMA"

def private das_files_under(root : string; var out : array<string>) {
    dir(root) $(name) {
        return if (name |> starts_with("."))
        let path = path_join(root, name)
        let st = stat(path)
        if (st.is_valid && st.is_dir) {
            das_files_under(path, out)
        } elif (name |> ends_with(".das")) {
            out |> push(path)
        }
    }
}

// the variable a `stream_create()` lands in: `g_ask = unsafe(stream_create())` -> "g_ask",
// `var output = unsafe(stream_create())` -> "output"; "" when the line is not an assignment
def private created_stream_name(line : string) : string {
    let eq = find(line, "=")
    return "" if (eq < 0)
    var lhs = strip(slice(line, 0, eq))
    if (lhs |> starts_with("var ") || lhs |> starts_with("let ")) {
        lhs = strip(slice(lhs, 4, length(lhs)))
    }
    let colon = find(lhs, ":")
    if (colon >= 0) {
        lhs = strip(slice(lhs, 0, colon))
    }
    return lhs
}

// a stream popped in a loop needs a producer on its books, or the pop answers empty at once
// and the loop spins (ARCHITECTURE.md#speech-thread-stream); a thread pops the stream
// under another name,
// so every created stream books one, whichever side pops it
def private check_created_streams(path : string) {
    let text = fread(path)
    return if (find(text, "stream_create()") < 0)
    for (raw in split(text, "\n")) {
        let line = strip(raw)
        continue if (find(line, "stream_create()") < 0 || line |> starts_with("//"))
        let name = created_stream_name(line)
        continue if (empty(name))
        if (find(text, "{name} |> append(") < 0 && find(text, "append({name},") < 0) {
            gate_finding(path, find_line(text, line),
                "`{name}` is created with stream_create() and nothing books a producer on it (`{name} |> append(1)`) - a stream with none answers a blocking pop at once, empty, and a thread that pops it in a loop spins a core")
        }
    }
}

let private GATE_MARKER = "<!-- @wasm-gate -->"

def private web_shells() : array<string> {
    var shells : array<string>
    dir(EXAMPLES_DIR) $(name) {
        let shell = "{EXAMPLES_DIR}/{name}/web_shell.html"
        if (fexist(shell)) {
            shells |> push(shell)
        }
    }
    shells |> sort
    return <- shells
}

def private check_browser_gate(path : string) {
    let text = fread(path)
    let marker = find(text, GATE_MARKER)
    let first_call = find(text, "dasWasmGate.")
    if (marker < 0 || find(text, GATE_MARKER, marker + 1) >= 0) {
        let how = marker < 0 ? "nowhere" : "more than once"
        gate_finding(path,
            "carries `{GATE_MARKER}` {how} - the release inlines the browser gate at one marker")
    } elif (first_call >= 0 && first_call < marker) {
        gate_finding(path, find_line(text, "dasWasmGate."),
            "calls `dasWasmGate` above `{GATE_MARKER}`, where the release inlines it - too early")
    }
    for (needle in ["dasWasmGate.reason()", "dasWasmGate.loadProgram(", "dasWasmGate.explain("]) {
        if (find(text, needle) < 0) {
            gate_finding(path, "never calls `{needle}` - a shell asks and starts through the gate")
        }
    }
    for (own in ["WebAssembly.validate", "crossOriginIsolated"]) {
        if (find(text, own) >= 0) {
            gate_finding(path, find_line(text, own),
                "tests `{own}` itself - a shell refuses a browser on the shared gate alone")
        }
    }
    let loader = find(text, "<template id=\"loader\">")
    let loader_end = loader < 0 ? -1 : find(text, "</template>", loader)
    let program = find(text, "\{\{\{ SCRIPT \}\}\}")
    if (loader < 0 || program < loader || program > loader_end) {
        gate_finding(path,
            "emcc's SCRIPT placeholder is outside the `loader` template - it loads ungated")
    }
}

[export]
def main : int {
    var files : array<string>
    das_files_under(EXAMPLES_DIR, files)
    files |> sort
    for (f in files) {
        check_created_streams(f)
    }
    for (shell in web_shells()) {
        check_browser_gate(shell)
    }
    return gate_verdict("examples/dasLLAMA")
}
