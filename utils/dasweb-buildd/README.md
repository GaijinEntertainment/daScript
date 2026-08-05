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
   killed after `build_timeout_seconds`. `run_build.sh` is the recipe — see **The sandbox**
   below — and per mode it runs the exact CI command the queue replaced: `module` = host
   daslang `-exe --jit-target=wasm64-unknown-emscripten` against the `web/output64` runtime
   archive (→ `sample.wasm`); `page` (standalone game html+js+wasm) lands with the game
   checkpoint.
5. **Collect**: exactly the files the mode declares, by name (`expected_artifact_names`).
   The build runs the user's compile-time code and can write anything into the output
   directory, so anything else there is logged and dropped — a build cannot widen its own
   output set. The server re-checks the same rule rather than trusting this service.
6. **Upload**: success = multipart `POST /api/build/result` with a `[{name, sha256}]` manifest
   plus one file part per artifact (libhv reads the files itself — bytes never pass through a
   das string); failure = the build output as the error. Every claim this service sees resolves
   with one of the two — the queue's stale-requeue sweep is for builder *death*, not builder
   code paths.

## The sandbox

Compiling a user's `.das` is remote code execution by design: macros and `[init]` run at
compile time, and plain `fio` reads files with **no `unsafe` anywhere**. So daslang's own
policies (`no_unsafe`, `no_init`) are defense in depth and never the boundary. The boundary is
a container.

The choice of a container over a namespace-carving tool like bubblewrap is about which way a
mistake fails. Carving starts from the whole host and subtracts, so a forgotten flag leaves a
hole open silently. A container starts from nothing, so a forgotten mount fails the *build*,
loudly, on the next job. Everything a build may touch is one of the `-v` lines in
`run_build.sh`, and that list is the entire review surface: the wasm worktree, emsdk, the LLVM
runtime libraries, the job's source (read-only) and its output directory (writable). Mounts are
identity-mapped because emsdk and the daslang host resolve paths relative to themselves. No
`$HOME`, no `/srv`, no config file, no key is inside.

Also applied: `--network=none`, a read-only root with a tmpfs `/tmp`, `--memory` /
`--pids-limit` / `--cpus` caps, `--cap-drop=ALL`, `--security-opt=no-new-privileges`, and
`--userns=keep-id` so the build runs as the unprivileged account this service runs as. The
emcc cache is mounted read-only with `EM_FROZEN_CACHE=1`, so one job cannot poison what later
jobs link against; the toolchain-bump protocol warms it with one build outside the sandbox.
Podman passes no host environment through, so a token in this service's environment is not
visible to a build. There is deliberately **no** unsandboxed fallback: no podman or no image
means no build.

Three deployment requirements that are part of the boundary, not decoration. This service must
run under a **dedicated, sudo-less account** — on a box where the login user has passwordless
sudo, a sandbox escape as that user is a root compromise. Its bearer token must reach it
without becoming a readable file inside any mount (systemd `LoadCredential`, or the
environment, which podman does not forward). And `scratch_dir` should point at a
**service-owned directory with mode 0700**: each job gets an unguessable subdirectory of it, and
the parent's permissions are what keep other local accounts from reading a build's sources or
planting anything in its output. Leaving `scratch_dir` empty uses the system temp directory
instead, which is fine on a single-purpose box and not on a shared one.

Verified on zen4 by building a hostile sample: a compile-time `[init]` that `fread`s
`~/SETUP.md`, `~/.ssh/authorized_keys` and `/etc/shadow` gets **0 bytes** from each, a planted
token environment variable comes back empty, `popen` runs but resolves no hostnames, and a
`leak.js` it wrote beside the real output never leaves the box.

A build blocks the tick thread, so `/healthz` goes dark for the build's duration; the watchdog
only logs health transitions (it restarts on process exit, never on health), and
`build_timeout_seconds` bounds the window.

The box environment (`DASWEB_WASM_WORKTREE`, pinned emsdk, the worktree's own
`web/build_wasm_host.sh` host build, the sandbox image) is documented in `~/SETUP.md` on zen4 —
the wasm worktree is dedicated because wasm and native builds poison each other's `bin/` and
`lib/` (`plans/dasweb_wasm_pipeline.md` has the postmortem). Build the sandbox image once per
`Containerfile` change:

```bash
podman build -t dasweb-builder:1 -f utils/dasweb-buildd/Containerfile .
```

## Tests

```bash
daslang dastest/dastest.das -- --test utils/dasweb-buildd/test_buildd_core.das
```

In-dir: config (pure merge, provenance, token masking), core (source materialization, name and
job-identity validation, command invocation via `popen_argv`, artifact collection + hashing,
the symlinked-artifact refusal, toolchain derivation), client (claim/announce/result against
the stub playground on reserved port 19014, including the multipart round trip verified
byte-exact on the receiving side). CI runs the suites in `extended_checks.yml`
("Test dasweb-buildd").
