# dasweb sample verifier — nightly browser leg against real daslang.io

Status: IMPLEMENTED in #3649 (P1–P4 all landed). Tier 1 (compile check,
`utils/dasweb-verify/`) shipped in #3645. This plan is the browser leg: nightly
verification that every curated sample on the LIVE site works in both engines, with
failures named per sample.

Three things the implementation added that this plan did not anticipate, each because a
measurement contradicted it — see #3649 for the evidence:

- **The rAF counter alone is not a liveness signal.** A threaded program (path tracer)
  leaves rAF firing at display rate while presenting under 1 frame/s. The probe also
  counts the *program's* WebGL draw calls.
- **The wedge deadline must be the sample's own remaining budget, not a fixed cap.** A
  daslang program owns the main thread while it runs, so a console benchmark computing
  for 40s is indistinguishable from a wedge until its budget is spent (the tree benchmark:
  40.5s locally, 55.4s on a CI runner).
- **`min_observe_ms`.** Stopping the moment thresholds are met makes the verdict depend on
  machine speed — the same sample passed twice and failed once on an identical GL error,
  purely on whether the loop was still watching when the line landed.

## Rulings already made (do not re-litigate)

- **Target is production daslang.io**, not a local rebuild. Half the samples arc's bugs were
  production state invisible locally: libhv truncation only on the caddy path, missing wasm
  archives per-box, cached build failures. Artifacts are content-addressed (source hash ×
  toolchain id), so most nights hit the build cache; fresh builds only after toolchain bumps
  or sample edits.
- **Expectations live verifier-side** (a table in the repo), NOT in-source `// verify:` lines.
  Budgets/regexes are properties of the check; tuning a timeout must not require redeploying
  sample content and busting its build cache.
- **Failure delivery = red nightly workflow** with a per-sample summary table. No auto-filed
  issues in v1.
- **No pixel/canvas checks in v1** (Boris dropped them — determinism). The missing-asset
  black-canvas class dies via panic → caught by the output-pane/error checks below.
- **Nightlies consume no cache** (#3646 policy) — the new workflow caches nothing.
- Verify the RENDER-adjacent liveness, not exit codes; UI-presence (imgui panels) is
  acknowledged out of scope for v1 (pathtracer's discarded-draw-list class would pass).

## Components

```
utils/dasweb-verify/
  main.das, verify_core.das, test_verify_core.das   # tier 1, shipped
  browser/
    runner.mjs          # orchestrator (node + playwright)
    protocol.mjs        # pure helpers: expectations lookup, output-pane parsing,
                        #   artifact-url extraction, verdict assembly (node:test covered)
    expectations.json   # per-sample table (below)
    package.json        # pinned playwright
    protocol.test.mjs   # node:test for the pure helpers
.github/workflows/nightly_playground.yml
```

CODEREVIEW.md + README.md gain the browser-leg rows (placement rule per file; pure helpers
get node:test coverage; the driving loop is proven by the nightly itself — mirror the
existing "orchestration exempt" wording).

## Sample list acquisition

- Deployed manifest is the truth users click: `https://daslang.io/playground/samples/data.json`
  (confirmed live, 200, same shape as repo `web/examples/ui/samples/data.json` — `examples[]`
  of `{name, files[], slug?}`).
- **Drift check**: diff deployed list vs the checked-out repo copy → report-only WARN rows
  (deploy lag is legitimate mid-merge).
- **Fail closed on unknown samples**: a deployed sample with no expectations entry is a FAIL
  ("add an expectations row"), so new samples can't silently skip verification.

## expectations.json shape

```json
{
  "defaults": { "kind": "graphics", "modes": ["interpreter", "wasm"],
                "budget_ms": 30000, "min_rafs": 30 },
  "samples": {
    "OpenGL: rotating triangle": {},
    "Path Tracer Lab (jobque + threads + GPU)": { "budget_ms": 120000 },
    "Physarum Lab (threads + audio)": { "budget_ms": 60000 },
    "Hello world": { "kind": "console", "stdout": "hello" },
    "Tests": { "kind": "console", "stdout": "passed" },
    "Audio: sine beep": { "kind": "audio" },
    "Game: Arcanoid (3D, arrow keys + space)": {}
  }
}
```

Keyed by picker `name` (what the deployed list carries). Kinds: `graphics` (frame checks),
`console` (stdout regex against the output pane, both engines — interpreter prints there,
wasm module-rail wasi stdout lands there too), `audio` (boot + no-wedge only; interpreted
audio breakup is a KNOWN parked issue, not a failure). Fill all 39 rows at implementation
time from the tier-1 manifest.

## The liveness protocol (why these exact checks)

A single rAF counter is not enough — reason it through:
- **Wedge** (jobque/strudel class): main thread hot-spins → rAF stops firing AND
  `page.evaluate` hangs. Detect: evaluate deadline → verdict `wedge`.
- **Crash/panic/exit** (missing asset class): emscripten loop stops but the page's own rAF
  keeps ticking → a generic rAF counter would PASS. Detect: pageerror/console-error events +
  output-pane text (panic/abort/build-failure markers).
- **Alive**: injected rAF counter advances past `min_rafs` within budget AND none of the
  error detectors fired AND no GL-watcher lines.

So per graphics sample, all of: (a) evaluate stays responsive, (b) injected rAF count ≥
min_rafs, (c) zero playwright pageerror / console.error events, (d) zero GL-error lines,
(e) output pane free of panic/abort markers. Budgets from the table.

## Per-engine flow

**Interpreter**: one persistent playground tab; per sample: select the picker option
(`select` element, dispatch change), click run, find `iframe.pg-run-frame` (SAME-origin →
`frame.evaluate` works), inject the rAF probe, apply the protocol. The GL watcher lines
appear in the parent `#output` pane (#3640's watcher, live on prod since #3645's pages
deploy — verify presence during P1).

**Wasm**: select the wasm radio, run. The playground POSTs the source and polls the build.
- *Console samples* → module (wasi) rail: stdout lands in `#output`; regex it.
- *Graphics/audio* → page rail: poll for `iframe.pg-page-frame` src (artifact URL on
  run.daslang.io) OR failure text in `#output` (builder message becomes the FAIL detail).
  The embedded frame is CROSS-origin (deliberate security boundary) → do NOT probe it;
  **open the artifact URL as a top-level playwright page** (the arc's verification method —
  full access, `crossOriginIsolated: true` confirmed live). Install the GL-error watcher via
  `context.addInitScript` (hook canvas.getContext → wrap the returned GL context's getError /
  poll per frame) BEFORE navigation — the page shell has no built-in watcher and adding one
  is a toolchain bump; init-script injection avoids that entirely.
- Build budget: fresh page builds measured 2.5–4 min (INITIAL_MEMORY asset embeds); poll
  every 4–5 s, budget 6 min, cache hits resolve in seconds.
- Serial, one sample at a time — don't hammer buildd.
- A cached FAILED build for identical content+toolchain is a correct verdict, not staleness —
  never cache-bust in the verifier.

## Recovery + reporting

- Wedge/timeout: screenshot → attach path in the report row → close the browser CONTEXT and
  rebuild it (headless CI needs no renderer-kill dance) → continue with the next sample.
- Report: one row per sample × mode (PASS/FAIL/WARN + detail + duration) appended to
  `$GITHUB_STEP_SUMMARY`; exit 1 on any FAIL → red workflow → default GitHub notification.
- Local runs: `node utils/dasweb-verify/browser/runner.mjs [--filter substr] [--mode m]`
  against prod from any box; keep `--base-url` overridable for a future staging box.

## nightly_playground.yml

- `schedule: cron '30 4 * * *'` (after extended_checks' 04:00 — boxes idle) +
  `workflow_dispatch` (with optional `filter` input). Canonical-repo guard on schedule like
  the other nightlies. ubuntu-latest.
- Steps: checkout → setup-node (NO npm cache — nightly policy) → `npm ci` in
  `utils/dasweb-verify/browser` → `npx playwright install chromium --with-deps` → runner.
  Use `--headless=new`; runner throttling of occluded tabs was an arc trap — CI has a single
  focused tab, but budgets must not assume >30fps.
- Wall clock: ~39 samples × 2 modes × 30–60 s ≈ 40–80 min on cache-hit nights. Fine.

## Tier-1 wiring rides along (cheap, per-PR)

`extended_checks` (linux) gains one step after the build:
`$BIN/daslang utils/dasweb-verify/main.das` — the 39-sample compile check runs in seconds
with the just-built binary and catches sample rot per-PR. Currently tier 1 runs nowhere in CI.

## Phases (verify live before advancing)

1. **P1 — interpreter leg**: runner skeleton + protocol + expectations rows for graphics
   samples; run locally against daslang.io; confirm the GL watcher is live on prod; tune
   budgets (pathtracer interpreted is legitimately slow — hundreds of ms per row).
2. **P2 — wasm leg**: module-rail stdout, page-rail artifact top-page + addInitScript GL
   watcher, build polling + failure surfacing.
3. **P3 — completeness**: console/audio kinds, all 39 rows, unknown-sample fail-closed,
   drift check, wedge recovery path exercised (physarum with the old source hash is a natural
   wedge fixture if needed — or accept untested-until-it-fires).
4. **P4 — ship**: `nightly_playground.yml`, extended_checks tier-1 step, README/CODEREVIEW
   rows, node:test for protocol.mjs, PR (one PR — this is the samples arc's follow-up).

## Traps carried from the samples arc (implementation must respect)

- Occluded/headless rAF throttles to 1 Hz — `--headless=new`, single tab, generous budgets.
- The playground tab is heavy; opening the artifact page directly (not via the iframe)
  avoids re-running it — same recipe used to verify gl_10/gl_12.
- `pgState`/`runCode`/engine radios (`input[name=engine]`), `#output`, `iframe.pg-run-frame`
  / `iframe.pg-page-frame` are the established hooks (used throughout the arc).
- Prod serves COOP/COEP on artifacts; top-level artifact pages ARE crossOriginIsolated.
- Playground rate limits on POST /api/samples exist — serial pacing keeps under them.
- daslang.io Host vs run.daslang.io Host: html/js artifacts 404 on the main origin BY DESIGN.

## After this ships (separate, still parked)

- WSL/local pre-push build leg (the emsdk mirror) — explicitly deferred.
- Runtime bugs: jobque-on-wasm-interpreter wedge; arcanoid threaded strudel → tick port.
- imgui-UI-presence checking (the discarded-draw-list blind spot) — needs a structured
  snapshot rail in the web harness, not v1.
