# dasweb-verify Browser Leg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../README.md`.

**A diff that leaves a pure helper in a non-test file in this folder - data in, data out, no
playwright, no DOM, no network - without a `node:test` case in `protocol.test.mjs` is a defect,
wherever the diff puts the helper.**

**Never make the verifier take a per-sample time budget or an expected-output pattern from a
sample source - read both from `expectations.json`.** A `// verify:` line in a sample changes the
sample's content hash and throws away the build-cache entry the nightly depends on.

**Weakening the fail-closed checks in `protocol.test.mjs` is a defect** - a deployed sample
with no `expectations.json` row stays a FAIL, a name in the deployed manifest but not the repo
manifest (or the reverse) stays a WARN, and the manifest-coverage case keeps reading the
repo manifest `web/examples/ui/samples/data.json` (repo root).

**Never make the verifier change a build's hash to force a rebuild - report the cached result
instead.** A cached FAILED build for identical content and toolchain is a correct verdict, so
a fresh hash only hides the bug.

**Never probe the wasm build's artifact page - the HTML page a graphics or audio build
returns - through the frame the playground embeds it in; open the artifact URL as a top-level
page instead.** That frame is cross-origin by design, so nothing inside it can be read.

**A diff that makes `probe.mjs` poll `glGetError` on the playground page is a defect - poll
only on artifact pages, which get the probe with `pollGl: true`.**
`site/playground/run-frame.html` (repo root) already polls the playground, and `getError` clears
the error it returns, so two pollers on one WebGL context each miss errors.

**Never let importing `runner.mjs` drive a browser - `main()` stays behind the entry-point
check.** `node --test` imports every file it discovers, so deleting that guard makes the suite
launch Chromium against the live playground and exit the test runner.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `runner.mjs` - driving loop: browser lifecycle, page hooks, polling, recovery, plus the
  in-flight stop condition and wedge/drift rows that mirror `protocol.mjs` verdicts.
- `protocol.mjs` - pure data in, pure data out: expectations lookup, output-pane
  classification, console-message classification, navigation-error classification, verdicts,
  the report. No playwright, no network, no DOM.
- `protocol.test.mjs` - the `node:test` suite over the pure helpers. No browser.
- `runner.test.mjs` - the `node:test` suite over `runner.mjs`, driven by a stubbed page. No
  browser, no assertion on a value a `protocol.mjs` function returns.
- `probe.mjs` - browser-side only, installed via `addInitScript`. Self-contained, no node
  API.
- `expectations.json` - the per-sample table. Data only.
- `package.json` - module type, the `verify`/`test` scripts, the playwright pin. No
  dependencies beyond playwright.
- `package-lock.json` - npm's resolution of that pin. Generated; never hand-edited.
- `.gitignore` - `node_modules/`, `artifacts/`, logs. Data only.
