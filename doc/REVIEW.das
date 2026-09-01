options gen2

require daslib/fio
require dastest/review_gate

// The mechanical half of doc/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang doc/REVIEW.das - exit 0 clean, 1 with findings, 2 when
// not run from the repo root.

let private CONF = "doc/source/conf.py"
let private SOURCE_ROOT = "doc/source"

// Hosts a built doc page may reach without a click. daslang.io is where the tree is served
// (html_baseurl) and jsDelivr serves the DocSearch bundle and its base stylesheet; the theme
// carries its own fonts and images, so nothing else is fetched off-site.
let private ALLOWED_HOSTS <- {
    "daslang.io",
    "cdn.jsdelivr.net"
}

// Sphinx writes these into every page's <head> at build time, so an address here is a load
// no template or stylesheet shows. Read as script text: the URLs are quoted literals, and a
// `#` comment carrying one is reported too - a commented-out CDN is still a decision to make.
def private check_conf_hosts {
    let text = fread(CONF)
    if (empty(text)) {
        gate_finding(CONF, "missing or unreadable")
        return
    }
    for (url in script_urls(text)) {
        let host = url_host(url)
        continue if (key_exists(ALLOWED_HOSTS, host))
        gate_finding(CONF, find_line(text, url),
            "names {host}, which the gate's allowed-host list does not name - add the host only once its data handling is reviewed")
    }
}

[export]
def main() : int {
    if (!fexist(CONF)) {
        to_log(LOG_ERROR, "doc/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_conf_hosts()
    let no_skipped_dirs : table<string>
    gate_web_third_party([SOURCE_ROOT], no_skipped_dirs, ALLOWED_HOSTS)
    return gate_verdict("doc")
}
