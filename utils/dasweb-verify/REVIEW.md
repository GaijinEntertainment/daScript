# dasweb-verify Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**Every core behavior has a dastest test in this directory** — `main.das` stays thin argv/exit
glue over tested pieces, so it needs none of its own.

**Every pure helper in `browser/` has a `node:test` case in `browser/protocol.test.mjs`.**
`runner.mjs` and `probe.mjs` don't run under node; the nightly proves them.

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

**The verifier reads the manifest the playground ships (`data.json`), never a hand-maintained
copy.** A second sample list in this directory is a defect.

**The generated-sample mapping in `verify_core.das` mirrors
`web/stage_playground_imgui_samples.cmake`:** a change to either lands with the other in the
same change.

**Manifest handling fails closed.** A missing, unparseable, or degenerate manifest (or sample
entry) is a named error and a non-zero exit, never a silent skip.

**Every failure line names the sample and carries the underlying message.** A failure a reader
cannot act on from the log alone is a defect.

**Expectations live in `expectations.json`, never in sample sources.** A budget or pattern
spelled as a `// verify:` line in a sample would change its content hash and throw away the
build-cache entry the nightly depends on.

**A deployed sample with no expectations row is a FAIL.** Silently skipping an unknown sample
would let a new sample ship unverified.

**Manifest drift between the deployed and repo lists is a WARN, never a FAIL.** Deploy lag
mid-merge is legitimate.

**The verifier never cache-busts a build.** A cached FAILED build for identical content and
toolchain is a correct verdict; re-minting the hash to "get a fresh result" hides the bug.

**The embedded page-shape artifact frame is never probed.** It is cross-origin by design;
verification opens the artifact URL as a top-level page instead.

**Exactly one glGetError poller per context.** `getError` clears the flag, so `probe.mjs`
polls only where the page has no watcher of its own (artifact pages, `pollGl: true`) — never
on the playground, where `run-frame.html` already polls.

**Placement — one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` — launcher: clargs parsing, per-sample reporting, exit-code mapping. No
  manifest parsing, no compilation.
- `verify_core.das` — manifest parsing and compile execution. Zero network.
- `browser/runner.mjs` — driving loop: browser lifecycle, page hooks, polling, recovery.
  No verdict rules.
- `browser/protocol.mjs` — pure data in, pure data out: expectations lookup, output
  classification, verdicts, the report. No playwright, no network, no DOM.
- `browser/probe.mjs` — browser-side only, installed via `addInitScript`. Self-contained,
  no node API.
- `browser/expectations.json` — the per-sample table. Data only.
