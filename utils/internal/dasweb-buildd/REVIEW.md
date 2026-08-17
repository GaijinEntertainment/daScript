# dasweb-buildd Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasweb_wasm_pipeline.md`.

**Every config, core, and client behavior has a dastest test in this directory** — `main.das`
and `buildd_service.das` stay glue and orchestration over tested modules; the whole loop is
proven by the end-to-end checkpoint on the real boxes.

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**HTTP tests run against the stub playground in `test_buildd_client.das` on this directory's
reserved test port 19014; core tests touch no network.** The playground's real endpoints are
proven in its own directory.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

**The health server binds loopback only** — `set_bind_host("127.0.0.1")` between `init` and
`start`. A diff that removes, reorders past `start`, or conditionalizes the bind is a defect.

**`POST /shutdown` verifies the transport peer is loopback.** No header can carry that proof.

**The bearer token never appears in a log line, an error message, or the startup banner.** The
banner logs set/unset and provenance only.

**Every file name that reaches the filesystem — bundle sources and build outputs alike — passes
this directory's name validation first.** A path assembled from an unvalidated request- or
build-derived name is a defect.

**A build publishes exactly the file set its mode declares, by name.** The build executes the
user's own compile-time code and can write anything into the output directory, so a
suffix filter, a glob, or any rule that lets the build widen its own output set is a defect.

**Builds run in the container sandbox defined by `Containerfile` and `run_build.sh`, and there
is no unsandboxed path.** Every host path the build may touch is an explicit mount in that
script; a change that adds a mount without a stated reason, or that reintroduces a
run-outside-the-sandbox fallback, is a defect. Mounting anything that holds a secret, a key, or
another service's data is a defect.

**The sandbox is the boundary, and daslang's own policies are not.** A change justified by
`no_unsafe`, `no_init`, or any compile-time policy flag standing in for isolation is a defect:
compile-time code reads files with no `unsafe` at all.

**Processes are spawned via `popen_argv` (no shell).** A build or git invocation through a
shell-interpreted string is a defect.

**An empty configured token refuses startup, never starts an unauthenticated poller.**

**Every claimed job logs its start and its outcome (done/failed, duration, exit code) as
structured lines.** A job path that can end without a log line is a defect.

**Startup logs the full effective config with per-key provenance before the first poll.**

**Lifecycle-owned state is module-global; no collectable value lives in a `main`-loop local
across `maybe_collect_gc()`.**

**Every claimed job is resolved by an upload — success or failure — on every code path.** A
path that drops a claim for the stale-requeue sweep to mop up is a defect.

**Per-job scratch directories are removed when the job resolves.**

**Route callbacks are retained with `push`, never `emplace`.**

**A behavior change in a box-side file — `run_build.sh`, `roll_toolchain.sh`, `.das_package`,
`watchdog.json`, `dasweb-buildd.toml` — lands with its note in the matching `README.md`
section** (The sandbox / The toolchain-bump protocol / Run).

**A toolchain roll that moves the worktree rebuilds both the cross-compile host and the
runtime archive.**

**A `Containerfile` change lands with an image-tag bump in `run_build.sh` (`IMAGE=`), same
commit.**

**The wasm-archive step's build command and archive list mirror
`modules/dasImgui/.das_package`, minus `liblibDasModuleClipboard.a`:** a change to either side
lands with the other.

**A file an operator edits on the box is preserved across upgrades:** shipped
`release_include_if_missing`, never plain `release_include`.

**Checked-in config holds development values.** The config file is discovered automatically
beside the module, so a production token or URL in it would make an in-repo run claim
production jobs.

**Placement — one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` — launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no HTTP client call, no process spawning.
- `buildd_config.das` — the config schema (`BuilddArgs`), the defaults/toml/CLI merge with
  per-key provenance, the startup banner payload. No HTTP, no process spawning, no
  filesystem beyond reading the config file.
- `buildd_service.das` — lifecycle and orchestration: the loopback health/shutdown server,
  module-global state, the poll → build → upload loop, per-job scratch dirs. No direct
  process spawning and no direct outbound HTTP — those go through `buildd_core` and
  `buildd_client`.
- `buildd_core.das` — build execution: source materialization, name validation, the build
  command invocation, artifact collection and hashing, toolchain derivation. Zero network.
- `buildd_client.das` — the outbound HTTP calls (claim, result upload, announce). Zero
  filesystem beyond handing paths to the multipart uploader.
- `run_build.sh` — the box-side build recipe and its sandbox invocation; the only place a
  build command line or a sandbox mount lives.
- `roll_toolchain.sh` — the box-side toolchain-bump recipe; the only place a roll step lives.
- `Containerfile` — the sandbox image: only what a system must provide the toolchain;
  anything a read-only mount can supply belongs in `run_build.sh`.
- `.das_package`, `watchdog.json`, `dasweb-buildd.toml` — packaging and deployment.
