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
script — see `CODEREVIEW.md`).

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

## Scope, and the follow-up

This tier is deterministic and local — no browser, no network, no emsdk. It
catches source rot (syntax/require breakage, manifest entries pointing at
missing files) before a commit ships them to the site.

The follow-up leg is a nightly runner against real daslang.io: drive the live
playground in headless Chromium, run each picker sample interpreted and in wasm
mode, and assert boot + frames advancing + a clean GL-error watcher. It targets
production deliberately — this arc's worst bugs (artifact truncation behind
caddy, per-box missing wasm archives) were production state no local rebuild
can see. Artifacts are content-addressed, so most nights hit the build cache.
