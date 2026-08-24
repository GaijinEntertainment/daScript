# dasweb-verify Browser Leg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../README.md`.

**Every pure helper in this folder - data in, data out, no playwright, no DOM, no network -
has a `node:test` case in `protocol.test.mjs`, wherever the diff puts the helper.**
`runner.mjs` and `probe.mjs` hold only browser- and playwright-bound code, which the nightly
proves.

**Expectations live in `expectations.json`, never in sample sources.** A budget or pattern
spelled as a `// verify:` line in a sample would change its content hash and throw away the
build-cache entry the nightly depends on.

**Weakening the fail-closed checks in `protocol.test.mjs` is a defect** - an unknown sample
stays a FAIL, manifest drift stays a WARN, and the manifest-coverage case keeps reading the
shipped manifest `web/examples/ui/samples/data.json` (repo root).

**The verifier never cache-busts a build.** A cached FAILED build for identical content and
toolchain is a correct verdict; re-minting the hash to "get a fresh result" hides the bug.

**The wasm build's artifact page - the HTML page a graphics or audio build returns, which the
playground embeds as a cross-origin frame - is never probed through that frame.** It is
cross-origin by design; verification opens the artifact URL as a top-level page instead.

**Exactly one glGetError poller per context.** `getError` clears the flag, so `probe.mjs`
polls only where the page has no watcher of its own (artifact pages, `pollGl: true`) - never
on the playground, where `site/playground/run-frame.html` (repo root) already polls.

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
