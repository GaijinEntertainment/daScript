# dasweb-buildd Code Review Checklist

Run this list on every dasweb-buildd change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code, without prior
knowledge of the service, and without opening another document.** If an entry needs any of
those, it is not a review criterion — move it to `README.md` and leave a one-line criterion
here.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how to
  write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything that
  needs a stable reference lives in `README.md`.
- **Cite files by name; cite `README.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.**
- **No history, no rationale, no direction of travel.** The reason lives in `README.md` or the
  plan; planned work lives in `plans/dasweb_wasm_pipeline.md`.

---

## Tests

**Every config, core, and client behavior has a dastest test in this directory.** Exempt:
`main.das` argv/lifecycle glue and `buildd_service.das` orchestration — their pieces are the
tested modules, and the whole loop is proven by the end-to-end checkpoint on the real boxes.

**Every bug fix lands with the regression test that fails without it, in the same change.**

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**HTTP tests run against the stub playground in `test_buildd_client.das` on this directory's
reserved test port 19014; core tests touch no network.** (The playground's real endpoints are
proven in its own directory.)

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

---

## Placement — one file, one rule

Every file states what it holds. Code that belongs to a file and is written anywhere else is a
defect, and this section is the whole test.

**A new file ships with its rule here and its tests, in the same change.**

- `main.das` — the launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no HTTP client call, no process spawning.
- `buildd_config.das` — the config schema (`BuilddArgs`), the defaults/toml/CLI merge with
  per-key provenance, and the startup banner payload. No HTTP, no process spawning, no
  filesystem beyond reading the config file.
- `buildd_service.das` — lifecycle and orchestration: the loopback health/shutdown server,
  module-global state, the poll → build → upload loop, per-job scratch dirs. No direct
  process spawning and no direct outbound HTTP — those go through `buildd_core` and
  `buildd_client`.
- `buildd_core.das` — build execution: source materialization, name validation, the build
  command invocation, artifact collection and hashing, toolchain derivation. Zero network.
- `buildd_client.das` — the outbound HTTP calls (claim, result upload, announce). Zero
  filesystem beyond handing paths to the multipart uploader.
- `run_build.sh` — the box-side build recipe and its sandbox invocation; the only place a build
  command line or a sandbox mount lives. A behavior change here without a note in `README.md`
  (The sandbox section) is a defect.
- `Containerfile` — the sandbox image. It holds only what a system must provide the toolchain;
  anything a read-only mount can supply instead belongs in `run_build.sh`. A change here
  without bumping the image tag in both files, in the same commit, is a defect.
- `_fake_build.das` — the cross-platform test stand-in for `run_build.sh`; test fixture only.
- `.das_package`, `watchdog.json`, `dasweb-buildd.toml` — packaging and deployment. A behavior
  change hidden in these files without a note in `README.md` (Run section) is a defect.

**A file an operator edits on the box is preserved across upgrades**: shipped
`release_include_if_missing`. Shipping one with plain `release_include` is a defect.

**Checked-in config holds development values.** The config file is discovered automatically
beside the module, so a production token or URL in it would make an in-repo run claim
production jobs.

---

## Security

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

---

## Logging

**Every claimed job logs its start and its outcome (done/failed, duration, exit code) as
structured lines.** A job path that can end without a log line is a defect.

**Startup logs the full effective config with per-key provenance before the first poll.**

---

## Lifecycle and memory

**Lifecycle-owned state is module-global; no collectable value lives in a `main`-loop local
across `maybe_collect_gc()`.**

**Every claimed job is resolved by an upload — success or failure — on every code path.** A
path that drops a claim for the stale-requeue sweep to mop up is a defect.

**Per-job scratch directories are removed when the job resolves.**

**Route callbacks are retained with `push`, never `emplace`.**
