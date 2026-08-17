# site (daslang.io) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**Every AUTHORED shell command shown on a page runs verbatim and produces the result the page
shows.** A command, flag, or output line invented for illustration is a defect; if the
implementation does not exist yet, the page does not show the command. A DATA-RENDERED command
(a record row's receipt) is the argv the harness executed for that measurement — per RUN, so
one receipt line stands beside every clip of its run.

**Every code sample shown on a page compiles and runs with the current toolchain.** daslang
samples are gen2 and compile with the current binary; no pseudo-code presented as code.

**A test under `tests/playground/` (this folder) that needs the daslang runtime carries `@wasm` in its
title.** The per-PR lane stages the site without WASM artifacts and runs the suite with
`--grep-invert '@wasm'` (`playground-e2e.yml`), so an untagged runtime-dependent test fails
every PR.

**A change to the deployed playground sources — `playground/` (this folder), `web/examples/ui/src/`, or
`web/examples/ui/samples/` — ships with a stated run of the WASM-staged Playwright suite**
(`tests/playground/`), naming the result — passes and any failures — in the PR body or
commit message. The run also names its runtime artifacts: built from this change when the
change feeds `web/output/daslang_static.*`, the deployed ones otherwise. The no-WASM lane
cannot see a broken runtime path, and every sample on the page runs through that path.

**Every number shown is either rendered from live data or copied from a checked-in
measurement record; anything else is a placeholder and carries an HTML comment naming it as
one.** A placeholder that could be mistaken for a fact is a defect.

**A `dl-*` selector defined in BOTH `files/dasllama-table.css` (the file dasllama.io loads)
and `dasllama.html`'s inline copy keeps identical bodies — changing only one is a defect.**
Page-only components live in `dasllama.html` alone, dasllama.io-only ones in the css alone.

**News entries state real, shipped events.** An entry in `_news/*.md` (this folder's news dir) for something not yet
true at publish time is a defect.
