# dasweb-buildd

The wasm build worker of the daslang.io playground pipeline
(`plans/dasweb_wasm_pipeline.md`, phase 3a). Runs on the compute box (`zen4`), **pulls** the
build queue from `dasweb-playground` over HTTPS with a bearer token, runs each job through a
sandboxed build command, and uploads the artifact file set (or the compiler error — that text
is what the playground shows the user). Nothing on the web box ever connects here: builder
death degrades to a backed-up queue, never a site outage. Review rules: `CODEREVIEW.md`
(binding).

## Run

```bash
# development against a local playground (start that first on :8101)
daslang utils/dasweb-buildd/main.das -- --token dev --toolchain <40-hex> --build-cmd "bash run_build.sh"

# supervised production form (inside a daspkg release bundle)
python3 watchdog.py

# release bundle
daslang utils/daspkg/main.das -- release --root utils/dasweb-buildd --out <dir>
```

`-?` prints flag help (the daslang host eats `--help`). Config: `dasweb-buildd.toml` in cwd or
beside the tool, keys = flag names; precedence defaults < toml < explicit CLI flags, flipped by
`authoritative = true`. A wrong-typed key is a startup error. The effective config is logged at
startup with per-key provenance; the token is reported set/unset only.

Deployed values on zen4: `server_url = "https://daslang.io"`, the real `token` (toml-only —
a CLI flag would put the secret in the process list; it must equal the playground's
`build_token`), `worktree` = the dedicated wasm worktree, `build_cmd = "bash run_build.sh"`.
The service refuses to start without a token and a build command.

## How a job runs

1. **Identity**: the toolchain id is the wasm worktree's HEAD sha (`--toolchain` overrides).
   At startup the service announces it via `POST /api/build/toolchain` — **that announce is
   the toolchain's go-live, so the ABI canary (`--jit-check-abi`) must have run clean on the
   worktree first**; `announce = false` leaves the go-live to the operator.
2. **Claim**: `POST /api/build/next` with the toolchain; 204 means idle (sleep
   `poll_interval_seconds`), a job means build now and poll again immediately after.
3. **Materialize**: the job's source document lands in a per-job scratch dir — raw `.das` text
   as `main.das`, or the `{files, active}` bundle with every file name validated (one boring
   `.das` path component each; anything else refuses the job).
4. **Build**: `<build_cmd> <src_dir> <out_dir> <mode> <entry>` via argv (no shell),
   killed after `build_timeout_seconds`. `run_build.sh` is the recipe: bwrap sandbox
   (no network, tmpfs scratch; fail-closed — `DASWEB_BUILDD_UNSANDBOXED=1` is the bring-up
   override), and per mode the exact CI command the queue replaced — `module` = host daslang
   `-exe --jit-target=wasm64-unknown-emscripten` against the `web/build64` runtime archive
   (→ `sample.wasm`); `page` (standalone game html+js+wasm) lands with the game checkpoint.
5. **Upload**: success = multipart `POST /api/build/result` with a `[{name, sha256}]` manifest
   plus one file part per artifact (libhv reads the files itself — bytes never pass through a
   das string); failure = the build output as the error. Every claim this service sees resolves
   with one of the two — the queue's stale-requeue sweep is for builder *death*, not builder
   code paths.

A build blocks the tick thread, so `/healthz` goes dark for the build's duration; the watchdog
only logs health transitions (it restarts on process exit, never on health), and
`build_timeout_seconds` bounds the window.

The box environment (`DASWEB_WASM_WORKTREE`, pinned emsdk, the worktree's own
`web/build_wasm_host.sh` host build) is documented in `~/SETUP.md` on zen4 — the wasm worktree
is dedicated because wasm and native builds poison each other's `bin/` and `lib/`
(`plans/dasweb_wasm_pipeline.md` has the postmortem).

## Tests

```bash
daslang dastest/dastest.das -- --test utils/dasweb-buildd/test_buildd_core.das
```

In-dir: config (pure merge), core (source materialization, name validation, command
invocation via `popen_argv`, artifact collection + hashing, toolchain derivation), client
(claim/announce/result against the stub playground on reserved port 19014, including the
multipart round trip verified byte-exact on the receiving side). `_fake_build.das` is the
cross-platform stand-in for `run_build.sh`. CI runs the suites in `extended_checks.yml`
("Test dasweb-buildd").
