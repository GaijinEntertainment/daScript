# Playground UI (web/examples/ui) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`site/README.md`.

**A boulder-dash sample file - a `.das` source, a dastest suite, or that sample's
`ARCHITECTURE.md` - answers to `samples/examples/boulder-dash/REVIEW.md`, wherever the diff
puts it.**

**A diff that changes a file of this folder applies `site/REVIEW.md` (repo root) too.** The
deploy copies these files into the site (`.github/workflows/pages.yml`, repo root), so
`site/playground/` never shows the change.

**A diff that changes `src/` or `samples/` states a run of the WASM-staged Playwright suite
(`site/tests/playground/`, repo root) in its PR body or commit message, naming the passes and
any failures, in the same change.** The no-WASM lane cannot see a broken runtime path, and
every sample on the page runs through that path.

**A `.das` file under `samples/` not written in gen2, or not a full program that compiles and
runs with the current toolchain, is a defect.**

**Never write a `// verify:` line into a `.das` file under `samples/` - put the budget or the expected pattern
in `utils/internal/dasweb-verify/browser/expectations.json` instead.** A sample-source change
voids that sample's nightly build-cache entry.
