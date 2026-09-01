options gen2

require strings
require daslib/fio
require daslib/strings_boost
require dastest/review_gate

// The mechanical half of site-dasllama/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang site-dasllama/REVIEW.das - exit 0 clean, 1 with
// findings, 2 when not run from the repo root.

let private INDEX_HTML = "site-dasllama/index.html"

// Hosts a page here may reach without a click. dasllama.io and daslang.io are the two sites;
// gc.zgo.at and the goatcounter endpoint are the cookieless hit counter, and api.github.com
// serves the star count the deploy stages site/files/github-star.js in to fetch.
let private ALLOWED_HOSTS <- {
    "dasllama.io",
    "daslang.io",
    "gc.zgo.at",
    "dasllama.goatcounter.com",
    "api.github.com"
}

let private SKIP_DIRS : table<string>

let private SHARED_SHEET = "site/files/dasllama-table.css"
let private DL_PREFIX = "dl-"

def private check_dl_names_come_from_shared_sheet {
    let defined <- prefixed_tokens(fread(SHARED_SHEET), DL_PREFIX)
    if (empty(defined)) {
        gate_finding(SHARED_SHEET, "missing, unreadable, or defines no dl-* selector")
        return
    }
    var files : array<string>
    dir("site-dasllama") $(name) {
        if (name |> ends_with(".html")) {
            files |> push("site-dasllama/{name}")
        }
    }
    dir("site-dasllama/files") $(name) {
        if (name |> ends_with(".js") || name |> ends_with(".css")) {
            files |> push("site-dasllama/files/{name}")
        }
    }
    for (path in files) {
        let text = fread(path)
        var inscope used <- prefixed_tokens(text, DL_PREFIX)
        for (name in keys(used)) {
            continue if (key_exists(defined, name))
            gate_finding(path, find_line(text, name),
                "uses {name}, which {SHARED_SHEET} does not define - dl-* names are that sheet's; a style of this directory's own takes the dio- prefix")
        }
    }
}

[export]
def main() : int {
    if (!fexist(INDEX_HTML)) {
        to_log(LOG_ERROR, "site-dasllama/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_dl_names_come_from_shared_sheet()
    gate_web_third_party(["site-dasllama"], SKIP_DIRS, ALLOWED_HOSTS)
    return gate_verdict("site-dasllama")
}
