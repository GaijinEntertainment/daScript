# site (daslang.io) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. **A Playwright spec (`*.spec.js`), wherever the diff puts it, answers to the
`tests/playground/` checklist.**

**Never show on a page a hand-written shell command, flag, or output line invented for
illustration - show only a command a run actually executed, producing the result the page
shows.**

**A diff that writes a `cmd` field in `files/dasllama/bench_records.json` that is not the
exact argv `modules/dasLLAMA/performance/gen_bench_records.das` (or the released bench exe it
spawns) ran is a defect.**

**A row `files/dasllama.js` renders without its run's `cmd` + `date` line is a defect - render
that line on every row the run produced.** One run's `cmd` + `date` covers several rendered
rows.

**A `cmd` or `date` inside a run's `tests` entry in `files/dasllama/bench_records.json` is a
defect - the pair belongs on the run object that produced the rows.**

**A diff that changes `files/performance_bench.json` also changes `benchmarks/sql/results.md`
(repo root), in the same change.** `benchmarks/sql/_update_results.das --site-json` writes the record from
the same sweep output that regenerates those tables.

**A cell in `files/performance_bench.json` differing from the same family-and-lane cell in the
`benchmarks/sql/results.md` tables is a defect.**

**A diff that changes `files/performance_engines.json` also changes
`examples/benchmarks/sql/results.md` (repo root), in the same change.**
`examples/benchmarks/sql/_update_results.das --site-json` writes the record from the same sweep
output that regenerates those tables.

**A cell in `files/performance_engines.json` differing from the same family-and-lane cell in
the `examples/benchmarks/sql/results.md` (repo root) tables is a defect.**

**A benchmark number hand-carried into `benchmarks.html` is a defect - render the page only
from the vendored dasProfile records, the `files/profile_results_<platform>.json` files
`.github/workflows/pages.yml` (repo root) fetches from borisbat/dasProfile at deploy time.**

**A second record file for data a dasProfile record already holds is a defect - read the
dasProfile record.** Provenance - the `das_capture` and `ext_capture` stamps - travels in the
dasProfile files themselves.

**A code sample shown as a partial snippet or as pseudo-code is a defect - show every sample as
a full program that compiles and runs with the current toolchain.**

**A code sample without a "try it on playground" link is a defect - link every sample.**

**A daslang sample not written in gen2 is a defect.**

**A diff that changes the deployed playground sources - `playground/` (this folder),
`web/examples/ui/src/`, or `web/examples/ui/samples/` - states a run of the WASM-staged
Playwright suite (`tests/playground/`, this folder) in its PR body or commit message, naming
the passes and any failures, in the same change.** The no-WASM lane cannot see a broken runtime
path, and every sample on the page runs through that path.

**A stated Playwright run names the runtime artifacts it used: built from this change when the
change feeds `web/output/daslang_static.*`, the deployed ones otherwise.**

**A diff that puts a measurement number - a rate, a duration, a size, a score some run
produced, never a fixture or run parameter such as an image's dimensions or a token budget - on
a page without rendering it from live data, copying it from a checked-in measurement record, or
marking it a placeholder in a source comment is a defect.**

**A placeholder number a page reader could take for a fact is a defect - mark it as a
placeholder on the page itself.**

**Weakening `REVIEW.das`'s `dl-*` selector-parity check - the gate that compares each
selector body defined in BOTH `files/dasllama-table.css` and `dasllama.html`'s inline
`<style>` - is a defect.**

**A diff that adds or moves a `dl-*` selector defines it in `dasllama.html` when
`dasllama.html`'s markup uses it, and in `files/dasllama-table.css` when a page under
`site-dasllama/` (repo root - the dasllama.io site) uses it.** Markup counts what
`files/dasllama.js` injects.

**An entry in `_news/*.md` for something not yet true at publish time is a defect - a news
entry states a real, shipped event.**
