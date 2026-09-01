options gen2

require daslib/fio
require dastest/review_gate

// The mechanical half of web/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang web/REVIEW.das - exit 0 clean, 1 with findings, 2 when
// not run from the repo root.

// Every authored tree under this folder that the deploy or `daspkg release --web` copies
// into a page a visitor loads. Everything else here is emsdk, a build tree, or a generated
// output that no diff hand-edits.
let private DEPLOYED_ROOTS <- ["web/examples/ui/src", "web/examples/ui/samples", "web/templates"]

// Empty on purpose: neither shell fetches anything off-site, and a browser opening a page
// built from them contacts only the host that served it.
let private ALLOWED_HOSTS : table<string>

[export]
def main() : int {
    for (root in DEPLOYED_ROOTS) {
        if (!fexist(root)) {
            to_log(LOG_ERROR, "web/REVIEW.das: run from the repo root\n")
            return 2
        }
    }
    let no_skipped_dirs : table<string>
    gate_web_third_party(DEPLOYED_ROOTS, no_skipped_dirs, ALLOWED_HOSTS)
    return gate_verdict("web")
}
