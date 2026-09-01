options gen2

require daslib/fio
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

[export]
def main() : int {
    if (!fexist(INDEX_HTML)) {
        to_log(LOG_ERROR, "site-dasllama/REVIEW.das: run from the repo root\n")
        return 2
    }
    gate_web_third_party(["site-dasllama"], SKIP_DIRS, ALLOWED_HOSTS)
    return gate_verdict("site-dasllama")
}
