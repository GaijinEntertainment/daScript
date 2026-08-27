# dasweb-verify Browser Leg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../README.md`.

**A diff that leaves a pure helper in this folder - data in, data out, no playwright, no DOM,
no network - without a `node:test` case in `protocol.test.mjs` is a defect, wherever the diff
puts the helper.** `runner.mjs` and `probe.mjs` hold only browser- and playwright-bound code,
which the nightly CI run proves.

**Never put a budget or a pattern in a sample source - put it in `expectations.json`
instead.** A `// verify:` line in a sample changes the sample's content hash and throws away
the build-cache entry the nightly depends on.

**Weakening the fail-closed checks in `protocol.test.mjs` is a defect** - an unknown sample
stays a FAIL, manifest drift stays a WARN, and the manifest-coverage case keeps reading the
shipped manifest `web/examples/ui/samples/data.json` (repo root).

**Never make the verifier change a build's hash to force a rebuild - report the cached result
instead.** A cached FAILED build for identical content and toolchain is a correct verdict, so
a fresh hash only hides the bug.

**Never probe the wasm build's artifact page - the HTML page a graphics or audio build
returns - through the frame the playground embeds it in; open the artifact URL as a top-level
page instead.** That frame is cross-origin by design, so nothing inside it can be read.

**Never make `probe.mjs` poll `glGetError` on a page that has its own GL-error watcher - poll
only where the page has none (artifact pages, `pollGl: true`), never on the playground, where
`site/playground/run-frame.html` (repo root) already polls.** `getError` clears the flag, so
only one poller per context can see an error.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `runner.mjs` - driving loop: browser lifecycle, page hooks, polling, recovery, plus the
  in-flight stop condition and wedge/drift rows that mirror `protocol.mjs` verdicts.
- `protocol.mjs` - pure data in, pure data out: expectations lookup, output classification,
  verdicts, the report. No playwright, no network, no DOM.
- `protocol.test.mjs` - the `node:test` suite over the pure helpers. No browser.
- `probe.mjs` - browser-side only, installed via `addInitScript`. Self-contained, no node
  API.
- `expectations.json` - the per-sample table. Data only.
- `package.json` - module type, the `verify`/`test` scripts, the playwright pin. No
  dependencies beyond playwright.
- `.gitignore` - `node_modules/`, `artifacts/`, logs. Data only.
