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
// and the loop spins (ARCHITECTURE.md sec. 3.2); a thread pops the stream under another name,
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

[export]
def main : int {
    var files : array<string>
    das_files_under(EXAMPLES_DIR, files)
    files |> sort
    for (f in files) {
        check_created_streams(f)
    }
    return gate_verdict("examples/dasLLAMA")
}
