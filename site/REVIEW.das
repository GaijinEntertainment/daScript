options gen2

require strings
require daslib/fio
require daslib/strings_boost
require dastest/review_gate

// The mechanical half of site/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang site/REVIEW.das - exit 0 clean, 1 with findings, 2 when
// not run from the repo root.

let private TABLE_CSS = "site/files/dasllama-table.css"
let private PAGE_HTML = "site/dasllama.html"

// Hosts a page under this folder may reach without a click. daslang.io is the site itself;
// the rest are the reviewed third parties - jsDelivr serves the DocSearch bundle, gc.zgo.at
// and the goatcounter endpoint are the cookieless hit counter, and the github hosts serve the
// star count, the package index and the source links the pages build.
let private ALLOWED_HOSTS <- {
    "daslang.io",
    "cdn.jsdelivr.net",
    "gc.zgo.at",
    "borisbat.goatcounter.com",
    "api.github.com",
    "raw.githubusercontent.com",
    "github.com"
}

// tests/ is the Playwright harness and doc/ is generated Sphinx output - neither is a page
// this folder authors, and neither ships from this tree
let private SKIP_DIRS <- { "tests", "doc", "node_modules" }

//! one top-level selector -> its normalized body. An at-rule (`@media ...`) block is skipped
//! whole: its nested rules are responsive OVERRIDES of the same selectors by design
def private css_blocks(text : string; var out : table<string; string>) {
    peek_data(text) $(d) {
        var i = 0
        let n = length(d)
        while (i < n) {
            let ob = d |> find("\{", i)
            break if (ob < 0)
            let sel = strip(slice(d, i, ob))
            if (sel |> starts_with("@")) {
                var depth = 1
                var j = ob + 1
                while (j < n && depth > 0) {
                    let c = int(d[j])
                    if (c == '{') {
                        depth++
                    } elif (c == '}') {
                        depth--
                    }
                    j++
                }
                i = j
                continue
            }
            let cb = d |> find("\}", ob)
            break if (cb < 0)
            let body = slice(d, ob + 1, cb)
            // normalize: collapse whitespace runs so formatting differences do not read as drift
            let parts <- [for (p in split_by_chars(body, " \t\r\n")); p; where !empty(p)]
            if (!empty(sel) && (sel |> find(".dl-") >= 0)) {
                out[sel] = join(parts, " ")
            }
            i = cb + 1
        }
    }
}

// dasllama.html carries an inline copy of some dl-* selectors beside the loaded css file;
// a body that drifts between the two copies styles the page and the injected table
// differently depending on load order
def private check_dl_selector_parity {
    let css = fread(TABLE_CSS)
    if (empty(css)) {
        gate_finding(TABLE_CSS, "missing or unreadable")
        return
    }
    let html = fread(PAGE_HTML)
    if (empty(html)) {
        gate_finding(PAGE_HTML, "missing or unreadable")
        return
    }
    let s0 = html |> find("<style>")
    let s1 = html |> find("</style>")
    return if (s0 < 0 || s1 < 0)
    var css_map : table<string; string>
    var html_map : table<string; string>
    css_blocks(css, css_map)
    css_blocks(slice(html, s0 + length("<style>"), s1), html_map)
    for (sel, body in keys(html_map), values(html_map)) {
        let other = css_map?[sel] ?? ""
        if (!empty(other) && other != body) {
            gate_finding(PAGE_HTML, "`{sel}` differs from the {TABLE_CSS} copy - give both copies the same body")
        }
    }
}

[export]
def main() : int {
    if (!fexist(TABLE_CSS)) {
        to_log(LOG_ERROR, "site/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_dl_selector_parity()
    gate_web_third_party(["site"], SKIP_DIRS, ALLOWED_HOSTS)
    return gate_verdict("site")
}
