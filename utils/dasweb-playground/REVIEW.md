# dasweb-playground Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasweb_backend.md`.

## Tests

**Every route, every store operation, and every config or limit behavior has a dastest test in
this directory.** A behavior that ships without its test is a defect. Exempt: `main.das` and
`admin.das` argv/dispatch glue — their behaviors live in the modules they call, which are the
tested surface.

**Every bug fix lands with the regression test that fails without it, in the same change.**

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**HTTP tests go through a local `with_*_server` harness (`test_playground_server.das`,
`test_build_endpoints.das`) on this directory's reserved test ports 19011, 19012, and 19013;
store tests call `samples_store` or `build_queue` directly with no server.** A store behavior
proven only through HTTP, or an HTTP behavior proven only against the store, is a defect.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

---

## Placement — one file, one rule

Every file states what it holds. Code that belongs to a file and is written anywhere else is a
defect, and this section is the whole test.

**A new file ships with its rule here and its tests, in the same change.**

- `main.das` — the launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no SQL, no hashing, no merge logic.
- `playground_config.das` — the config schema (`ServerArgs`), the defaults/toml/CLI merge with
  per-key provenance, normalization of merged config values, and the startup banner payload. No
  HTTP, no SQL, no filesystem beyond reading the config file.
- `playground_server.das` — the `HvWebServer` class: the route table and handlers. A handler
  validates transport-level shape, translates HTTP to one store call, and formats the response.
  A SQL statement, a hash computation, or a policy decision (size, rate, listing) in this file
  is a defect.
- `samples_store.das` — the store: schema structs, migrations, store operations, content
  hashing, and every policy decision (size cap, rate ceiling, listing). Zero HTTP: a require
  of `dashv` here is a defect.
- `build_queue.das` — the build queue: `BuildJob`/`BuildMeta` schema, the migrations it owns,
  the job state machine, mode selection from a job's source document, and queue policy (claim
  timeout, attempt ceiling). Zero HTTP and zero filesystem.
- `build_artifacts.das` — the artifact cache: blobs layout, toolchain/hash/filename validation
  (including which suffixes are documents, for the origin gate), integrity verification,
  stage-then-rename placement, serving-path resolution, and every path the cache assembles. The
  only build-side file that touches the filesystem; zero HTTP, zero SQL.
- `curated_import.das` — the data.json-driven curated importer: manifest parsing, sample-file
  reads, bundle assembly, calling the store. The only file besides the config loader that reads
  the filesystem; a store mutation here that bypasses `samples_store` functions is a defect.
- `admin.das` — the operator CLI (listing curation). Talks to the store the same way the
  server does; a second implementation of a store operation here is a defect.
- `.das_package`, `watchdog.json`, `dasweb-playground.toml`, `deploy.sh`, `caddy.snippet` —
  packaging, deployment, and the public route boundary. A behavior change hidden in these files
  without a note in `README.md` (Run section) is a defect.

**A file an operator edits on the box is preserved across upgrades**: shipped
`release_include_if_missing` and carried forward by `deploy.sh`. Shipping one with plain
`release_include`, or adding one `deploy.sh` does not carry, is a defect.

**Checked-in config holds development values.** The config file is discovered automatically
beside the module, so a production path in it makes an in-repo run open the live database.

**Migrations are append-only.** A diff that edits a shipped `[sql_migration]` body is a defect;
schema change means a new version.

---

## Security

**The server binds loopback only** — `set_bind_host("127.0.0.1")` between `init` and `start`.
A diff that removes, reorders past `start`, or conditionalizes the bind is a defect.

**Every SQL statement goes through the `daslib/sql_linq` rail (`_sql` / `insert` /
`_sql_update`) or bound parameters.** A query assembled by string interpolation or `format`
from any request-derived value is a defect.

**Every request body is bounds-checked against the configured cap before it is hashed or
stored, and the transport cap that precedes it lives in `caddy.snippet`.** The service can only
check a body already buffered in full, so a route accepting a body without a `request_body`
limit in front of it is a defect, as is a hash or insert reachable without the size check.

**A request body is read as bytes, and a byte count that disagrees with its string length is
rejected.** A das string ends at the first NUL, so a body used without that guard can be stored
truncated under a hash that does not describe it.

**Client identity comes from the LAST `X-Forwarded-For` hop and is treated as data, never
parsed into behavior beyond the rate ceiling.** A proxy appends the peer it accepted; every
earlier hop is client-authored, so keying anything on one is a defect. Logging it is required.

**Operator routes (`POST /shutdown`, `/admin/*`) verify the transport peer is loopback.** No
header can carry that proof — a browser on any page can drive a cross-origin POST to a proxied
route. A new operator route without the check is a defect.

**Remote-builder routes (`/api/build/toolchain`, `/api/build/next`, `/api/build/result`)
authenticate by the bearer token from config, compared with `constant_time_equal`.** A route
that logs, echoes, or short-circuit-compares the token is a defect; an empty configured token
must disable the surface, never open it.

**Every artifact-cache path is assembled only from components validated in
`build_artifacts`, and an upload reaches the served tree only through a stage directory
renamed into place after every file's sha256 verifies.** A served path built from request
data anywhere else, or a write landing directly in the served tree, is a defect.

**An upload publishes exactly the file set its job's mode declares, checked here and not
taken on the builder's word.** A build runs the user's own compile-time code, so accepting
whatever set the builder sends would let a build put an extra file on this origin.

**No route enables CORS.** The middleware reflects the caller's `Origin` on every route at once,
which would make stored samples and the operator surface cross-origin readable.

**Responses that echo stored user input carry `X-Content-Type-Options: nosniff`.**

**No shell-out anywhere in the service.**

**No filesystem path derived from request data, and a manifest-supplied path is checked to stay
under its configured directory before it is read.** `path_join` discards the base when the right
side is absolute, so an unchecked manifest path reads any file the service user can — and the
importer publishes what it reads.

**No `unsafe` in any route handler.** An `unsafe` elsewhere carries a reason comment on the line
of the unsafe operation itself — the `unsafe(...)` expression, or the operation line inside an
`unsafe { }` block; one without the comment is a defect.

**`POST /shutdown` and every admin operation stay unrouted in `caddy.snippet`.** Forwarding one
there, or adding an admin endpoint to the public route set, is a defect.

**Secrets (tokens, credentials) never appear in a log line, a response body, or an error
message.** The config banner logs key names and provenance, never values marked secret.

---

## Logging

**Every request emits one structured line: method, path, status, duration, client ip, bytes.**
A route without it is a defect. Exempt: `GET /healthz` — the watchdog polls it every few
seconds and logging it would bury the signal.

**Every store mutation and every job/state transition emits a structured line.** A failure path
that can trigger without leaving a log line is a defect.

**Startup logs the full effective config with per-key provenance before the first request is
served.**

---

## Lifecycle and memory

**Lifecycle-owned state is module-global; no collectable value lives in a `main`-loop local
across `maybe_collect_gc()`.**

**The store opens inside the thread that serves it.** A `SqlRunner` created on one thread and
used on another is a defect.

**Route callbacks are retained with `push`, never `emplace`.**
