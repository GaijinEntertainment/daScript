# dasweb-buildd Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasweb_wasm_pipeline.md`.

**A diff that adds or changes behavior in `buildd_config.das`, `buildd_core.das`, or
`buildd_client.das` ships a dastest test in this directory, in the same change.** A diff that
puts behavior beyond glue and orchestration over those modules into `main.das` or
`buildd_service.das` is a defect.

**Never put a `[test]` file under the global `tests/` tree, and never register one in a
`CMakeLists.txt` - a `[test]` file for this directory lives here and requires its siblings by
bare name.**

**A test that reaches any host or port other than the stub playground in
`test_buildd_client.das` on this directory's reserved test port 19014 is a defect, and so is a
test of `buildd_core.das` that reaches the network at all.** The playground's real endpoints are
proven in its own directory.

**A diff that adds or changes a test that touches the filesystem roots that test's paths in
`temp_directory` and deletes what the test creates.**

**Never let the health server bind an address other than loopback - it calls
`set_bind_host("127.0.0.1")` between `init` and `start`.** A diff that removes that call, moves
it past `start`, or makes it conditional is a defect.

**Never let `POST /shutdown` act on a request without checking that the transport peer address
is loopback.** No header can carry that proof.

**Never write the bearer token into a log line, an error message, or the startup banner - the
banner reports set or unset and the provenance instead.**

**A diff that assembles a filesystem path from a request- or build-derived name - a bundle
source or a build output alike - without passing that name through `is_valid_source_filename`,
`is_valid_asset_path`, or `validate_asset_paths` (`buildd_core.das`) first is a defect.**

**A diff that lets a build publish a file its mode does not declare by name - through a suffix
filter, a glob, or any rule that widens the build's own output set - is a defect.** The build
runs the user's own compile-time code and can write anything into the output directory.

**A diff that adds a mount to `run_build.sh` without a stated reason, that gives a build a host
path which is not an explicit mount in that script, or that adds a way to run a build outside
the container sandbox defined by `Containerfile` and `run_build.sh`, is a defect.** Mounting
anything that holds a secret, a key, or another service's data is a defect.

**A change justified by `no_unsafe`, `no_init`, or any compile-time policy flag standing in for
the sandbox's isolation is a defect.** Compile-time code reads files with no `unsafe` at all.

**A diff that spawns a process through a shell-interpreted string is a defect - spawn it with
`popen_argv` instead.**

**Never let the service start with an empty configured token - refuse startup instead.**

**A diff that leaves a claimed job able to end without structured log lines for its start and
its outcome (done or failed, duration, exit code) is a defect.**

**A diff that leaves a config key out of the startup log, drops that key's provenance, or moves
that log after the first poll is a defect.**

**Never put lifecycle-owned state anywhere but a module-global, and never hold a collectable
value - one the GC can free - in a `main`-loop local across a call to `maybe_collect_gc()`.**

**A diff that adds a code path where a claimed job ends without an upload - success or failure -
is a defect.** Leaving the claim for the stale-requeue sweep is not a resolution.

**Never let a job resolve without removing its per-job scratch directory.**

**Never retain a route callback with `emplace` - use `push` instead.**

**A diff that changes the behavior of a box-side file - `run_build.sh`, `roll_toolchain.sh`,
`.das_package`, `watchdog.json`, `dasweb-buildd.toml` - also updates the matching `README.md`
section (The sandbox / The toolchain-bump protocol / Run), in the same change.**

**Never let a toolchain roll (`roll_toolchain.sh`) move the worktree without rebuilding both
the cross-compile host and the runtime archive.**

**A diff that changes `Containerfile` also bumps the image tag in `run_build.sh` (`IMAGE=`), in
the same change.**

**A diff that changes the wasm-archive step's build command or archive list in
`roll_toolchain.sh`, or `modules/dasImgui/.das_package`, also changes the other to match, in
the same change - the two differ only in `liblibDasModuleClipboard.a`, which the wasm-archive
list leaves out.**

**Never ship a file an operator edits on the box with plain `release_include` - use
`release_include_if_missing`, so an upgrade keeps the operator's edits.**

**Never commit a production value to `dasweb-buildd.toml` - the checked-in file holds
development values.** The config file is discovered automatically beside the module, so a
production token or URL there would make an in-repo run claim production jobs.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` - launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no HTTP client call, no process spawning.
- `buildd_config.das` - the config schema (`BuilddArgs`), the defaults/toml/CLI merge with
  per-key provenance, the startup banner payload. No HTTP, no process spawning, no
  filesystem beyond reading the config file.
- `buildd_service.das` - lifecycle and orchestration: the loopback health/shutdown server,
  module-global state, the poll -> build -> upload loop, per-job scratch dirs. No direct
  process spawning and no direct outbound HTTP - those go through `buildd_core` and
  `buildd_client`.
- `buildd_core.das` - build execution: source materialization, name validation, the build
  command invocation, artifact collection and hashing, toolchain derivation. Zero network.
- `buildd_client.das` - the outbound HTTP calls (claim, result upload, announce). Zero
  filesystem beyond handing paths to the multipart uploader.
- `run_build.sh` - the box-side build recipe and its sandbox invocation; the only place a
  build command line or a sandbox mount lives.
- `roll_toolchain.sh` - the box-side toolchain-bump recipe; the only place a roll step lives.
- `Containerfile` - the sandbox image: only what a system must provide the toolchain;
  anything a read-only mount can supply belongs in `run_build.sh`.
- `.das_package`, `watchdog.json`, `dasweb-buildd.toml` - packaging and deployment.
