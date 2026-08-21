options gen2

require strings
require daslib/fio
require daslib/json
require dastest/review_gate

// The mechanical half of utils/dasllama-server/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang utils/dasllama-server/REVIEW.das.
// Fixtures are captured wire bytes: every JSON fixture must parse, carry no latin-1 round-trip
// mojibake (the raw Â pair or its \u00c2 escape spelling — the PowerShell/cp1252 trap), and
// hold no machine-local username (the rail normalizes captures to C:/Users/user).

let FIXTURES_DIR = "utils/dasllama-server/tests/fixtures"

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
    return gate_verdict("dasllama-server")
}
