options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/json
require dastest/review_gate

// The mechanical half of utils/dasllama-server/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang utils/dasllama-server/REVIEW.das.
// Fixtures are captured wire bytes: every JSON fixture must parse, carry no latin-1 round-trip
// mojibake (the raw Â pair or its \u00c2 escape spelling — the PowerShell/cp1252 trap), and
// hold no machine-local username (the rail normalizes captures to C:/Users/user). Every
// path-shaped route registration appears in the README and rst endpoint tables, and no
// surface documents or consumes a route the server does not register.

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

// every path registered in onInit; the scan reads only GET( / POST( / STREAM( rows
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

def private has_mojibake_bytes(text : string) : bool {
    var found = false
    peek_data(text) $(d) {
        let n = length(d)
        for (i in range(n - 1)) {
            if (int(d[i]) == 0xC3 && int(d[i + 1]) == 0x82) {
                found = true
                break
            }
        }
    }
    return found
}

def private foreign_username(text : string) : string {
    var bad = ""
    peek_data(text) $(d) {
        let n = length(d)
        var from = 0
        while (empty(bad)) {
            let idx = find(d, "Users", from)
            break if (idx < 0)
            var i = idx + 5
            while (i < n && (int(d[i]) == '\\' || int(d[i]) == '/')) {
                i++
            }
            var name = ""
            while (i < n && (is_alnum(int(d[i])) || int(d[i]) == '_' || int(d[i]) == '.' || int(d[i]) == '-')) {
                name += to_char(int(d[i]))
                i++
            }
            if (!empty(name) && name != "user") {
                bad = name
            }
            from = idx + 5
        }
    }
    return bad
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
        let text = fread(p)
        if (has_mojibake_bytes(text) || find(to_lower(text), "\\u00c2") >= 0) {
            gate_finding(p, "latin-1 mojibake (Â) in a captured fixture — re-capture per tests/fixtures/README.md with a UTF-8-safe writer, never hand-edit")
        }
        let uname = foreign_username(text)
        if (!empty(uname)) {
            gate_finding(p, "machine-local username '{uname}' — the capture rail normalizes home paths to C:/Users/user")
        }
        if (ends_with(name, ".json")) {
            var jerr = ""
            var js = read_json(text, jerr)
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
