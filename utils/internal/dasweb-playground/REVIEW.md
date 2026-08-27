# dasweb-playground Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasweb_backend.md`.

**A diff that adds or changes a route, a store operation, or a config or limit behavior covers
it with a dastest test in this directory, in the same change.** `main.das` and `admin.das` stay
argv/dispatch glue over tested modules, so they need no test of their own.

**Never register a `[test]` file in a `CMakeLists.txt`, and never put one under the repo-root
`tests/` tree - `[test]` files live in this directory and require their siblings by bare name.**

**A diff that adds an HTTP test drives it through a local `with_*_server` harness
(`test_playground_server.das`, `test_build_endpoints.das`) on this directory's reserved test
ports 19011, 19012, and 19013; a store test calls `samples_store` or `build_queue` directly,
with no server.** A store behavior proven only through HTTP, or an HTTP behavior proven only
against the store, is a defect.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.**

**Never remove `set_bind_host("127.0.0.1")`, move it past `start`, or make it conditional - it
stays between `init` and `start`, so the server binds loopback only.**

**A diff that adds a SQL statement puts it through the `daslib/sql_linq` rail (`_sql` /
`insert` / `_sql_update`) or bound parameters.** A query assembled by string interpolation or
`format` from any request-derived value is a defect.

**A route that accepts a body without a `request_body` limit in `caddy.snippet` in front of it
is a defect, as is a hash or insert reachable without the size check against the configured
cap.** The service can only check a body already buffered in full.

**Never use a request body whose byte count disagrees with its string length - read the body as
bytes and reject the mismatch.** A das string ends at the first NUL, so a body used without that
guard can be stored truncated under a hash that does not describe it.

**Never derive client identity from any `X-Forwarded-For` hop but the last, and never parse
that hop into behavior beyond the rate ceiling - log it, and otherwise treat it as data.** A
proxy appends the peer it accepted, so every earlier hop is client-authored.

**An operator route (`POST /shutdown`, `/admin/*`) that does not verify the transport peer is
loopback is a defect.** No header can carry that proof - a browser on any page can drive a
cross-origin POST to a proxied route.

**A remote-builder route (`/api/build/toolchain`, `/api/build/next`, `/api/build/result`) that
does not authenticate by the config bearer token compared with `constant_time_equal` is a
defect, as is one that logs, echoes, or short-circuit-compares the token.** An empty configured
token disables the surface, never opens it.

**Never assemble an artifact-cache path from anything but components validated in
`build_artifacts`, and never land an upload in the served tree other than by renaming a stage
directory into place after every file's sha256 verifies.**

**A diff that publishes an upload checks its file set against the set its job's mode declares,
instead of taking the builder's word.** A build runs the user's own compile-time code, so
accepting whatever set the builder sends would let a build put an extra file on this origin.

**Never put SQL, hashing, or policy (size, rate, listing) in a route handler, and never require
`dashv` in `samples_store.das` - a handler validates transport shape, makes one store call, and
formats the response, and those three live in `samples_store.das`.**

**Never enable CORS on any route.** The middleware reflects the caller's `Origin` on every route
at once, which would make stored samples and the operator surface cross-origin readable.

**Never send a response that echoes stored user input without `X-Content-Type-Options:
nosniff`.**

**Never shell out anywhere in the service.**

**Never derive a filesystem path from request data, and never read a manifest-supplied path
without checking that it stays under its configured directory.** `path_join` discards the base
when the right side is absolute, so an unchecked manifest path reads any file the service user
can - and the importer publishes what it reads.

**Never write `unsafe` in a route handler, and never write one elsewhere without a reason
comment on the unsafe operation's own line.**

**Never forward `POST /shutdown` or any admin operation in `caddy.snippet`, and never add an
admin endpoint to the public route set.**

**Never put a secret - a token or a credential - in a log line, a response body, or an error
message.**

**A route that does not emit one structured line - method, path, status, duration, client ip,
bytes - is a defect; `GET /healthz` emits none.** The watchdog polls `/healthz` every few
seconds, and logging it would bury the signal.

**A diff that adds a store mutation or a job/state transition also emits a structured line for
it.** A failure path that can trigger without leaving a log line is a defect.

**Never serve the first request before the startup banner has logged the full effective config
with per-key provenance.**

**Never keep state the `init`/`update`/`shutdown` lifecycle owns anywhere but a module global,
and never leave a collectable value in a `main`-loop local across `maybe_collect_gc()`.**

**Never create a `SqlRunner` on one thread and use it on another - the store opens inside the
thread that serves it.**

**Never retain a route callback with `emplace` - use `push`.**

**A behavior change in a box-side file - `.das_package`, `watchdog.json`,
`dasweb-playground.toml`, `deploy.sh`, `caddy.snippet` - lands with its note in the
`README.md` Run section.**

**Never ship a file an operator edits on the box with plain `release_include` - ship it with
`release_include_if_missing` and have `deploy.sh` carry it forward across upgrades.**

**Never put a production value in the checked-in `dasweb-playground.toml` - it holds
development values.** The config file is discovered automatically beside the module, so a
production path in it makes an in-repo run open the live database.

**A diff that edits a shipped `[sql_migration]` body is a defect - a schema change adds a new
version instead.**

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` - launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no SQL, no hashing, no merge logic.
- `playground_config.das` - the config schema (`ServerArgs`), the defaults/toml/CLI merge with
  per-key provenance, normalization of merged config values, the startup banner payload. No
  HTTP, no SQL, no filesystem beyond reading the config file.
- `playground_server.das` - the `HvWebServer` class: the route table and handlers. No SQL,
  no hashing, no policy.
- `samples_store.das` - the store: schema structs, migrations, store operations, content
  hashing, and every policy decision (size cap, rate ceiling, listing). Zero HTTP.
- `build_queue.das` - the build queue: `BuildJob`/`BuildMeta` schema, the migrations it owns,
  the job state machine, mode selection from a job's source document, and queue policy (claim
  timeout, attempt ceiling). Zero HTTP and zero filesystem.
- `build_artifacts.das` - the artifact cache: blobs layout, toolchain/hash/filename validation
  (including which suffixes are documents, for the origin gate), integrity verification,
  stage-then-rename placement, serving-path resolution, and every path the cache assembles. The
  only build-side file that touches the filesystem; zero HTTP, zero SQL.
- `curated_import.das` - the data.json-driven curated importer: manifest parsing, sample-file
  reads, bundle assembly, calling the store. The only file besides the config loader that
  reads the filesystem; no store mutation that bypasses `samples_store` functions.
- `admin.das` - operator CLI (listing curation). Talks to the store the same way the server
  does; no second implementation of a store operation.
- `.das_package`, `watchdog.json`, `dasweb-playground.toml`, `deploy.sh`, `caddy.snippet` -
  packaging, deployment, and the public route boundary.
