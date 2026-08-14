# dasweb-verify

Batch verifier for the curated playground samples. Tier 1 — the compile check —
lives here today: it reads the manifest the playground ships
(`web/examples/ui/samples/data.json`), and compile+simulates every sample's
entry file with the host binary's own compiler, in process. Every listed file
must exist; a sample that fails names itself and carries the compiler's message.

The dasImgui showcase samples (fourier, path tracer, physarum) have no
committed copies — `web/stage_playground_imgui_samples.cmake` generates them
into the staged site tree from canonical `examples/graphics` sources. For those
entries the verifier compiles the canonical source in its real directory
instead, via the mapping in `verify_core.das` (kept in lockstep with the cmake
script — see `REVIEW.md`).

## Run

From the repo root:

```
daslang utils/dasweb-verify/main.das
```

Exit code 0 = every sample compiles; 1 = at least one failure. Flags:
`--samples-root <dir>` (a different manifest root), `--filter <substr>`
(subset by sample name or entry path), `-?` (help).

Tests run in-dir:

```
daslang dastest/dastest.das -- --test utils/dasweb-verify/test_verify_core.das
```

Tier 1 is deterministic and local — no browser, no network, no emsdk. It
catches source rot (syntax/require breakage, manifest entries pointing at
missing files) before a commit ships them to the site. It runs per-PR in
`extended_checks` (linux).

## Tier 2 — the browser leg (`browser/`)

A node + playwright runner that drives the LIVE playground in headless
Chromium and reports, per sample and per engine, whether the program actually
runs. It targets production deliberately: this arc's worst bugs (page
artifacts truncated behind caddy, wasm archives missing on one build box, a
failed build cached under its content hash) were production state no local
rebuild can see. Sources and toolchain id are content-addressed, so a night
with no sample edit and no toolchain bump is mostly cache hits.

```
cd utils/dasweb-verify/browser
npm ci && npx playwright install chromium
node runner.mjs                          # both engines, every sample
node runner.mjs --mode interpreter -f gl # one engine, name substring
node runner.mjs --list                   # the plan, no browser
node --test                              # unit tests for the pure helpers
```

`--base-url` points it at a staging box; `--headed` shows the browser.
Exit code 0 = every planned row passed, 1 = at least one FAIL. In CI it runs
as `nightly_playground.yml` (04:30) and appends its table to the step summary.

### What "it runs" means

A single frame counter is not enough, and reasoning through the failure modes
is what fixes the checks:

- **Wedge** (the jobque class): the main thread hot-spins, so frames stop AND
  `page.evaluate` never answers. Every call into the page is raced against a
  deadline; a miss is a `wedge` verdict, a screenshot, and a rebuilt context.
- **Crash / panic / missing asset**: the emscripten loop dies but the host
  page's own frame loop keeps ticking — a bare rAF counter would call this
  alive. So the probe also counts the *program's* WebGL draw calls, and the
  verdict reads the output pane (`EXCEPTION`, `runtime aborted`, `error[…]`),
  playwright's uncaught-exception events, and the GL-error watcher.
- **Alive**: frames and draws both advance past the sample's thresholds inside
  its budget, with none of the above firing.

Console samples are checked by a stdout pattern against the output pane
instead; audio samples only have to boot without wedging (interpreted audio
breakup is a known parked issue, not a failure here).

### expectations.json

One row per sample, keyed by the picker name the deployed manifest carries,
merged over `defaults`. Thresholds and budgets live here rather than in sample
sources on purpose: tuning a timeout must not edit sample content and throw
away its build-cache entry. A deployed sample with no row is a FAIL — "add an
expectations row" — so a new sample cannot ship unverified.

### Engines

**Interpreter**: one persistent playground tab, the sample picked through the
page's own `selectSample`, run in the same run frame a visitor gets.

**Wasm**: the playground stores the source and polls the build. A console
program comes back as a bare wasi module and prints into the same output pane.
A graphics or audio program comes back as a page-shape artifact on the run
origin, embedded cross-origin — that frame is deliberately unprobeable, so the
runner takes its URL and opens the artifact as a top-level page, where the
GL-error watcher is installed by init script before anything loads.
