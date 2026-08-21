# site (daslang.io) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**Every hand-written shell command shown on a page runs verbatim and produces the result
the page shows.** A command, flag, or output line invented for illustration is a defect; if
the implementation does not exist yet, the page does not show the command.

**The `cmd` field of any run in `files/dasllama/bench_records.json` is the exact argv
`modules/dasLLAMA/performance/gen_bench_records.das` (or the released bench exe it spawns)
ran.** A receipt is per run — one `cmd` + `date` run object — not per rendered row:
when one run produced several rows, `files/dasllama.js` shows that run's receipt line on
every one of them.

**`files/dasllama/bench_records.json` is generator output: `modules/dasLLAMA/performance/gen_site_records.das`
merges every `modules/dasLLAMA/performance/records/<box>.json` and applies
`modules/dasLLAMA/performance/records/annotations.json`, so re-running the generator leaves
the file byte-identical.** A diff where it does not is a hand edit and a defect.

**Every code sample shown on a page compiles and runs with the current toolchain.** daslang
samples are gen2 and compile with the current binary; no pseudo-code presented as code.

**A test under `tests/playground/` (this folder) that needs the daslang runtime carries `@wasm` in its
title.** The per-PR lane stages the site without WASM artifacts and runs the suite with
`--grep-invert '@wasm'` (`playground-e2e.yml`), so an untagged runtime-dependent test fails
every PR.

**A change to the deployed playground sources — `playground/` (this folder), `web/examples/ui/src/`, or
`web/examples/ui/samples/` — ships with a stated run of the WASM-staged Playwright suite**
(`tests/playground/`, this folder), naming the result — passes and any failures — in the PR body or
commit message. The run also names its runtime artifacts: built from this change when the
change feeds `web/output/daslang_static.*`, the deployed ones otherwise. The no-WASM lane
cannot see a broken runtime path, and every sample on the page runs through that path.

**Every measured number shown is either rendered from live data or copied from a checked-in
measurement record; any other measurement-shaped number is a placeholder and carries a source
comment naming it as one.** A placeholder that could be mistaken for a fact is a defect. A
constant describing a fixture or a run parameter (an image's dimensions, a token budget) is
neither — it names the artifact it is a property of.

**A `dl-*` selector defined in BOTH `files/dasllama-table.css` (the file dasllama.io loads)
and `dasllama.html`'s inline copy keeps identical bodies — changing only one is a defect.**
A selector used by markup in `dasllama.html` is defined in `dasllama.html` (and in the css
too when dasllama.io's own pages also use it); a selector only the css-served pages use
lives in the css alone.

**News entries state real, shipped events.** An entry in `_news/*.md` for something not yet
true at publish time is a defect.
