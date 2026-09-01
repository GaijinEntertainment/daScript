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
    gate_prefixed_names_defined(files, DL_PREFIX, defined, SHARED_SHEET)
}

// three censuses know the served pages - the deploy's copy lines, build_news.py's sitemap
// list and test_metadata.py's page list - and a page in none of them deploys unlisted,
// uncrawled and unchecked
let private PAGES_YML = ".github/workflows/pages.yml"
let private BUILD_NEWS = "site-dasllama/build_news.py"
let private TEST_METADATA = "site-dasllama/test_metadata.py"

def private check_page_census {
    var inscope staged <- shell_cp_sources(fread(PAGES_YML), "_site_dasllama/")
    var inscope staged_set : table<string>
    for (src in staged) {
        staged_set |> insert(src)
    }
    let news = fread(BUILD_NEWS)
    let meta = fread(TEST_METADATA)
    dir("site-dasllama") $(name) {
        let served = name |> ends_with(".html") || name |> ends_with(".xml") || name |> ends_with(".txt")
        return if (!served)
        let path = "site-dasllama/{name}"
        if (!key_exists(staged_set, path)) {
            gate_finding(path, "has no copy line in {PAGES_YML}'s dasllama.io deploy step - the step names each top-level file, so an unlisted one never reaches the site")
        }
        return if (!(name |> ends_with(".html")))
        // index.html is the sitemap's root entry, spelled as the empty path
        if (name != "index.html" && find(news, "'{name}'") < 0) {
            gate_finding(path, "is not in {BUILD_NEWS}'s sitemap page list - the page deploys uncrawled")
        }
        if (find(meta, "\"{name}\"") < 0) {
            gate_finding(path, "is not in {TEST_METADATA}'s page list - the page deploys unchecked")
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
    check_page_census()
    gate_web_third_party(["site-dasllama"], SKIP_DIRS, ALLOWED_HOSTS)
    return gate_verdict("site-dasllama")
}
