# site (daslang.io) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**Never show on a page a hand-written shell command, flag, or output line invented for
illustration - show only a command that runs verbatim and produces the result the page
shows.** Never show a command whose implementation does not exist yet.

**A diff that writes a `cmd` field in `files/dasllama/bench_records.json` that is not the
exact argv `modules/dasLLAMA/performance/gen_bench_records.das` (or the released bench exe it
spawns) ran is a defect.** Never put a `cmd` + `date` pair on a rendered row - the pair sits
on the run object that produced the rows.

**A row `files/dasllama.js` renders without its run's receipt line is a defect - render that
line on every row the run produced.** One run's `cmd` + `date` covers several rendered rows.

**A diff that leaves `files/dasllama/bench_records.json` differing from what re-running
`modules/dasLLAMA/performance/gen_site_records.das` writes is a hand edit and a defect -
change the generator inputs and re-run it instead.** The generator merges every
`modules/dasLLAMA/performance/records/<box>.json` and applies
`modules/dasLLAMA/performance/records/annotations.json`.

**A diff that changes `files/performance_bench.json` without changing `benchmarks/sql/results.md`
in the same change, or that leaves any cell in the record differing from the same
family-and-lane cell in the `benchmarks/sql/results.md` tables, is a defect.**
`benchmarks/sql/_update_results.das --site-json` writes the record from the same sweep output
that regenerates those tables.

**A diff that changes `files/performance_engines.json` without changing
`examples/benchmarks/sql/results.md` in the same change, or that leaves any cell in the record
differing from the same family-and-lane cell in the `examples/benchmarks/sql/results.md`
tables, is a defect.** `examples/benchmarks/sql/_update_results.das --site-json` writes the
record from the same sweep output that regenerates those tables.

**A benchmark number hand-carried into `benchmarks.html`, or a second record file for data a
dasProfile record already holds, is a defect - render the page only from the vendored
dasProfile records, the `files/profile_results_<platform>.json` files pages.yml fetches from
borisbat/dasProfile at deploy time and `files/forge.js`'s front-page cycler reads.**
Provenance - the `das_capture` and `ext_capture` stamps - travels in the dasProfile files
themselves.

**A partial snippet shown as a code card, a code sample without its "try it on playground"
link, or pseudo-code presented as code is a defect - show every sample as a full program that
compiles and runs with the current toolchain, linked to "try it on playground".** A daslang
sample not written in gen2 is a defect.

**A diff that adds a test under `tests/playground/` (this folder) needing the daslang runtime,
or that makes an existing test there need it, puts `@wasm` in that test's title, in the same
change.** The per-PR lane stages the site without WASM artifacts and runs the suite with
`--grep-invert '@wasm'` (`playground-e2e.yml`), so an untagged runtime-dependent test fails
every PR.

**A diff that changes the deployed playground sources - `playground/` (this folder),
`web/examples/ui/src/`, or `web/examples/ui/samples/` - states a run of the WASM-staged
Playwright suite (`tests/playground/`, this folder) in its PR body or commit message, naming
the passes and any failures, in the same change.** The stated run also names the runtime
artifacts it used: built from this change when the change feeds `web/output/daslang_static.*`,
the deployed ones otherwise. The no-WASM lane cannot see a broken runtime path, and every
sample on the page runs through that path.

**A number on a page that reports a measurement - a rate, a duration, a size, a score some run
produced - is a defect when it is not rendered from live data, not copied from a checked-in
measurement record, and not named a placeholder by a source comment.** A placeholder that
could be mistaken for a fact is a defect. A number stating a fixture or a run parameter (an
image's dimensions, a token budget) reports no measurement and is outside this rule.

**A diff that changes one copy of a `dl-*` selector defined in BOTH
`files/dasllama-table.css` (the file dasllama.io loads) and `dasllama.html`'s inline `<style>`
copy, and not the other, is a defect - give both copies the same body.**

**A diff that adds or moves a `dl-*` selector defines it where its markup lives - the markup a
page renders, counting what `files/dasllama.js` injects: in `dasllama.html` when
`dasllama.html` renders it, in `files/dasllama-table.css` as well when `site-dasllama/`'s
pages (repo root - the dasllama.io site) render it too, and in `files/dasllama-table.css`
alone when only `site-dasllama/`'s pages render it.**

**An entry in `_news/*.md` for something not yet true at publish time is a defect - a news
entry states a real, shipped event.**
