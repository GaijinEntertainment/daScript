# dasweb-verify Code Review Checklist

Run this list on every dasweb-verify change before it ships — including changes to this file.
Entries must be checkable against a diff alone; anything needing prior knowledge or another
document belongs in `README.md` with a one-line criterion here.

## Tests

**Every core behavior has a dastest test in this directory.** Exempt: `main.das` argv/exit
glue — its pieces are the tested module.

**Every pure helper in `browser/` has a `node:test` case in `browser/protocol.test.mjs`.**
Exempt: `runner.mjs` orchestration and `probe.mjs` browser-side code — neither runs under
node, and the nightly itself is what proves them.

**Every bug fix lands with the regression test that fails without it, in the same change.**

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

## Placement — one file, one rule

- `main.das` — the launcher: clargs parsing, per-sample reporting, exit-code mapping. No
  manifest parsing, no compilation.
- `verify_core.das` — manifest parsing and compile execution. Zero network.
- `browser/runner.mjs` — the driving loop: browser lifecycle, page hooks, polling, recovery.
  No verdict rules.
- `browser/protocol.mjs` — pure data in, pure data out: expectations lookup, output-pane
  classification, verdicts, the report. No playwright, no network, no DOM.
- `browser/probe.mjs` — browser-side only, installed via `addInitScript`. Self-contained (no
  imports, no closure over module state), no node API.
- `browser/expectations.json` — the per-sample table. Data only.

**A new file ships with its rule here and its tests, in the same change.**

## Behavior

**The verifier reads the manifest the playground ships (`data.json`), never a hand-maintained
copy.** A second sample list in this directory is a defect.

**The generated-sample mapping in `verify_core.das` mirrors
`web/stage_playground_imgui_samples.cmake`.** A change to either without the other, in the same
change, is a defect.

**Manifest handling fails closed.** A missing, unparseable, or degenerate manifest (or sample
entry) is a named error and a non-zero exit, never a silent skip.

**Every failure line names the sample and carries the underlying message.** A failure a reader
cannot act on from the log alone is a defect.

## Behavior — browser leg

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
