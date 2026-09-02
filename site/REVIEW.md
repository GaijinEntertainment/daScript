# site (daslang.io) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. **A Playwright spec (`*.spec.js`), wherever the diff puts it, answers to the
`tests/playground/` checklist.** A page is an `.html` or `.md` file under this folder that a
visitor navigates to, together with what the scripts it loads render into it - not a
machine-only harness document (`playground/run-frame.html`), and not editor content - what a
visitor types into the playground or hero editor, and the sample bundles the playground loads
into that editor.

**Never show on a page a hand-written shell command, flag, or output line invented for
illustration - show only a command the run actually executed, with every home directory
spelled `~`.** A rendered `cmd` identifies
the RUN, not the row: where one run produced several rendered rows (an ASR run's per-clip
rows), each row's receipt is the run's command.

**A diff that writes a `cmd` field in `files/dasllama/bench_records.json` that is not the
argv `modules/dasLLAMA/performance/gen_bench_records.das` (or the released bench exe it
spawns) ran, with every home directory spelled `~` outside the quotes, is a defect.**

**A row that `files/dasllama.js` renders without its run's `cmd` + `date` line is a defect -
render that line on every row the run produced.**

**Weakening `REVIEW.das`'s receipt check - the gate that requires `cmd` and `date` on every
run object of `files/dasllama/bench_records.json` and reports either inside a `tests` entry -
is a defect.**

**Weakening `REVIEW.das`'s home-path check - the gate that reports a home directory spelled
out, instead of `~`, in any `.json` under `files/dasllama/` - is a defect.**

**Weakening `REVIEW.das`'s bench-cell parity check - the gate that compares every
family-and-lane cell of `files/performance_bench.json` and `files/performance_engines.json`
with the `benchmarks/sql/results.md` and `examples/benchmarks/sql/results.md` (repo root)
tables - is a defect.** `_update_results.das --site-json` beside each table writes the record
from the sweep that regenerates it, so the pair moves together.

**A benchmark number hand-carried into a page that renders dasProfile numbers is a defect -
render every such number only from the vendored dasProfile records, the
`files/profile_results_<platform>.json` files `.github/workflows/pages.yml` (repo root)
fetches from borisbat/dasProfile at deploy time.**

**A claim on a page about how a dasProfile number was captured - its statistic, its sample
count, its spread - that the record the page renders does not itself carry is a defect -
render the claim from the record or drop it.**

**A second record file for data a dasProfile record already holds is a defect - read the
dasProfile record.** Provenance - the `das_capture` and `ext_capture` stamps - travels in the
dasProfile files themselves.

**Weakening `REVIEW.das`'s embedded-sample check - the gate that compiles every daslang
sample of a page outside `blog/` with the running binary (a `language-daslang` block on an
`.html` page, a ```` ```daslang ```` fence in a `_news/` post), requires an exported `main` in
each, and requires a playground link beside it (in its `<section>` on a page, anywhere in a
post) - is a defect.**

**A diff that marks a sample `data-playground="none"` also ledgers the block in `README.md`,
in the same change.** The gate still compiles such a block; it owes no link.

**A PR whose diff changes `playground/` (this folder) or `examples/_interp.html` (this folder)
states, in the PR body, a run of the WASM-staged Playwright suite (`tests/playground/`, this
folder) against the branch tip, naming the passes and any failures; a later edit to those
files restates the run.** The no-WASM lane cannot see a broken runtime path, and a run
recorded mid-branch describes a tree that no longer ships.

**A stated Playwright run names every playground sample the diff changed: for each, the spec
that loaded it, or - when no spec loads it - that it was opened and run by hand in the
playground.**

**A stated Playwright run names the runtime artifacts it used: built from this change when
the diff touches any source compiled into the WASM runtime (`daslang_static` - its `main()`
lives in `utils/daslang/`) or `web/` (repo root); the deployed ones otherwise.**

**A diff that puts a measurement number - a rate, a duration, a size, a score some run
produced, never a fixture or run parameter - on a page without rendering it from live data,
copying it from a checked-in measurement record, or marking it a placeholder in a source
comment is a defect.**

**A placeholder number a page reader could take for a fact is a defect - mark it as a
placeholder on the page itself.**

**A diff that makes `REVIEW.das`'s cookie-and-host check read fewer files, or report less
than it did before the change with the same `ALLOWED_HOSTS`, is a defect** - the gate bans
`document.cookie` and reports any resource a page loads from a host the list does not
name.

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**

**Weakening `REVIEW.das`'s `dl-*` selector-parity check - the gate that compares each
selector body defined in BOTH `files/dasllama-table.css` and `dasllama.html`'s inline
`<style>` - is a defect.**

**Weakening `REVIEW.das`'s `dl-*` census - the gate that reports a `dl-*` name `dasllama.html`
or `files/dasllama.js` uses that no selector of the page's inline `<style>` or of
`files/dasllama-table.css` defines and no `id="..."` declares - is a defect.**

**An entry in `_news/*.md` for something not shipped at publish time is a defect.**
