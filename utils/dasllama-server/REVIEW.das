options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/json
require dastest/review_gate

// The mechanical half of utils/dasllama-server/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang utils/dasllama-server/REVIEW.das.
// Fixtures are captured wire bytes, swept by KIND: a text capture must carry no latin-1
// round-trip mojibake (the raw Â pair or its \u00c2 escape spelling — the PowerShell/cp1252
// trap) and no machine-local username (the rail normalizes captures to C:/Users/user), a JSON
// one must parse, the wav must carry the RIFF/WAVE header the route answers with, and a kind
// with no arm here is itself a finding. Every path-shaped route registration appears in the
// README and rst endpoint tables, and no surface documents or consumes a route the server
// does not register.

let FIXTURES_DIR = "utils/dasllama-server/tests/fixtures"
let SERVER_DAS = "utils/dasllama-server/openai_server.das"
let README_MD = "utils/dasllama-server/README.md"
let SERVER_RST = "doc/source/reference/utils/dasllama_server.rst"
let CONTROL_HTML = "utils/dasllama-server/control.html"

var DOC_EXEMPT_ROUTES <- {
    "/",
    "/favicon.ico"
}

def is_route_byte(b : int) : bool {
    return is_alnum(b) || b == '/' || b == '_' || b == '-'
}

// every path registered in onInit; the scan reads GET( / POST( / STREAM( / ANY( rows
def served_routes() : table<string> {
    var routes : table<string>
    for (raw in split(fread(SERVER_DAS), "\n")) {
        let line = strip(raw)
        var open_at = -1
        for (m in ["GET(\"", "POST(\"", "STREAM(\"", "ANY(\""]) {
            if (line |> starts_with(m)) {
                open_at = length(m)
                break
            }
        }
        continue if (open_at < 0)
        let close_at = find(line, "\"", open_at)
        continue if (close_at <= open_at)
        let path = slice(line, open_at, close_at)
        if (path |> starts_with("/")) {
            routes |> insert(path)
        }
    }
    return <- routes
}

// route-shaped inline literals between the given quote marks
def quoted_routes(text : string; quote : string; var out : table<string>) {
    let ql = length(quote)
    var lits : array<string>
    peek_data(text) $(d) {
        var from = 0
        while (true) {
            let open_at = find(d, quote, from)
            break if (open_at < 0)
            let close_at = find(d, quote, open_at + ql)
            break if (close_at < 0)
            from = close_at + ql
            continue if (close_at - (open_at + ql) < 2 || int(d[open_at + ql]) != '/')
            var ok = true
            for (i in range(open_at + ql + 1, close_at)) {
                if (!is_route_byte(int(d[i]))) {
                    ok = false
                }
            }
            if (ok) {
                var lit = ""
                for (i in range(open_at + ql, close_at)) {
                    lit += to_char(int(d[i]))
                }
                lits |> push(lit)
            }
        }
    }
    for (lit in lits) {
        out |> insert(lit)
    }
}

def check_routes_in_surfaces() {
    var inscope served <- served_routes()
    let readme = fread(README_MD)
    let rst = fread(SERVER_RST)
    for (path in keys(served)) {
        continue if (key_exists(DOC_EXEMPT_ROUTES, path))
        if (find(readme, "`{path}`") < 0) {
            gate_finding(README_MD, "served route {path} has no `{path}` mention - the README documents every route the server registers (the endpoint table is its home)")
        }
        if (find(rst, "``{path}``") < 0) {
            gate_finding(SERVER_RST, "served route {path} has no ``{path}`` mention - the shipped page documents every route the server registers (the endpoint table is its home)")
        }
    }
    var inscope in_readme : table<string>
    quoted_routes(readme, "`", in_readme)
    for (path in keys(in_readme)) {
        if (!key_exists(served, path) && !key_exists(DOC_EXEMPT_ROUTES, path)) {
            gate_finding(README_MD, "documents route {path}, which the server does not register - a documented-but-dead route misleads every client")
        }
    }
    var inscope in_rst : table<string>
    quoted_routes(rst, "``", in_rst)
    for (path in keys(in_rst)) {
        if (!key_exists(served, path) && !key_exists(DOC_EXEMPT_ROUTES, path)) {
            gate_finding(SERVER_RST, "documents route {path}, which the server does not register - a documented-but-dead route misleads every client")
        }
    }
    var inscope in_control : table<string>
    let control = fread(CONTROL_HTML)
    quoted_routes(control, "\"", in_control)
    quoted_routes(control, "'", in_control)
    quoted_routes(control, "`", in_control)
    for (path in keys(in_control)) {
        if (!key_exists(served, path) && !key_exists(DOC_EXEMPT_ROUTES, path)) {
            gate_finding(CONTROL_HTML, "the control page consumes route {path}, which the server does not register")
        }
    }
}

// a fixture is read as BYTES: a string read stops at the first NUL, so the captured wav's tail
// never reached the scan at all
def private read_bytes(path : string; size : uint64; var out : array<uint8>) : bool {
    out |> resize(int64(size))
    var got = 0
    fopen(path, "rb") $(f) {
        if (f != null) {
            got = fread(f, out)
        }
    }
    return got == length(out)
}

def private has_mojibake_bytes(d : array<uint8>) : bool {
    let n = length(d)
    for (i in range(n - 1)) {
        if (int(d[i]) == 0xC3 && int(d[i + 1]) == 0x82) return true
    }
    return false
}

// the ascii-lowered copy the escape search runs over, so \u00C2 reads the same as \u00c2
def private lowered(d : array<uint8>; var out : array<uint8>) {
    out |> resize(length(d))
    for (b, o in d, out) {
        o = uint8(int(b) >= 'A' && int(b) <= 'Z' ? int(b) + 32 : int(b))
    }
}

def private foreign_username(d : array<uint8>) : string {
    var bad = ""
    let n = length(d)
    var from = 0
    while (empty(bad)) {
        let idx = find(d, "Users", from)
        break if (idx < 0)
        var i = idx + 5
        while (i < n && (int(d[i]) == '\\' || int(d[i]) == '/')) {
            i++
        }
        let start = i
        while (i < n && (is_alnum(int(d[i])) || int(d[i]) == '_' || int(d[i]) == '.' || int(d[i]) == '-')) {
            i++
        }
        let name = build_string() $(writer) {
            writer |> write_string(d, start, i)
        }
        if (!empty(name) && name != "user") {
            bad = name
        }
        from = idx + 5
    }
    return bad
}

def private scan_text(path : string; d : array<uint8>) {
    var inscope low : array<uint8>
    lowered(d, low)
    if (has_mojibake_bytes(d) || find(low, "\\u00c2") >= 0) {
        gate_finding(path, "latin-1 mojibake (Â) in a captured fixture — re-capture per tests/fixtures/README.md with a UTF-8-safe writer, never hand-edit")
    }
    let uname = foreign_username(d)
    if (!empty(uname)) {
        gate_finding(path, "machine-local username '{uname}' — the capture rail normalizes home paths to C:/Users/user")
    }
}

// the wav the speech route answers with is PCM, so the text scans say nothing about it; its
// header is what a hand-written body would not carry
def private is_riff_wave(d : array<uint8>) : bool {
    return false if (length(d) < 12)
    return find(d, "RIFF") == 0 && find(d, "WAVE") == 8
}

[export]
def main() : int {
    if (!fexist("utils/dasllama-server/REVIEW.das")) {
        to_log(LOG_ERROR, "utils/dasllama-server/REVIEW.das: run from the repo root\n")
        return 2
    }
    dir(FIXTURES_DIR) $(name) {
        return if (name == "." || name == "..")
        let p = "{FIXTURES_DIR}/{name}"
        let st = stat(p)
        return if (!st.is_valid || st.is_dir)
        var inscope bytes : array<uint8>
        if (!read_bytes(p, st.size, bytes)) {
            gate_finding(p, "could not be read as bytes - the sweep must see every byte of every capture")
            return
        }
        // the captured text bodies, plus the folder's own rail doc and capture script
        if (ends_with(name, ".json") || ends_with(name, ".txt") || ends_with(name, ".md")
            || ends_with(name, ".das") || ends_with(name, ".gitattributes")) {
            scan_text(p, bytes)
        } elif (ends_with(name, ".wav")) {
            if (!is_riff_wave(bytes)) {
                gate_finding(p, "no RIFF/WAVE header - a captured speech body carries one; re-capture per tests/fixtures/README.md")
            }
        } else {
            gate_finding(p, "fixture kind this gate cannot scan - teach REVIEW.das what to check for it in the same change that adds it")
        }
        if (ends_with(name, ".json")) {
            var jerr = ""
            var js = read_json(fread(p), jerr)
            if (js == null) {
                gate_finding(p, "fixture does not parse as JSON: {jerr}")
            } else {
                unsafe {
                    delete js
                }
            }
        }
    }
    check_routes_in_surfaces()
    return gate_verdict("dasllama-server")
}
