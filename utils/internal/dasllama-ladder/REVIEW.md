# dasllama-ladder Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasllama_io_site.md`.

**A diff that adds or changes a route, a store operation, or a config or limit behavior also
adds a dastest test for it in this directory, in the same change.** `main.das` and `admin.das`
stay argv/dispatch glue over tested modules, so they need no test of their own.

**Never put a `[test]` file for this folder under the repo-root `tests/` tree, and never
register one in any `CMakeLists.txt` - a `[test]` file lives in this directory and requires its
siblings by bare name.**

**Never reach the service from a test except through the local `with_ladder_server` harness
(`test_ladder_server.das`) on this directory's reserved test port 19015, and never start a
server in a store test - a store test calls `ladder_store` directly.** A store behavior proven
only through HTTP, or an HTTP behavior proven only against the store, is a defect.

**Never let a test write into the repo tree or anywhere outside `temp_directory` - root every
path the test creates under `temp_directory` and delete it before the test ends; a store test
opens `:memory:`, never a file.**

**Operator routes (`/admin/*`, `/shutdown`) never appear in `caddy.snippet`, or in any
Caddyfile route that reaches this service, a catch-all included - operators reach them on the
service port itself, over the ssh tunnel.** Caddy proxies from the same box, so
`is_loopback_peer` sees `127.0.0.1` for every proxied request, which leaves the Caddyfile as
the only real boundary.

**Never drop one of the three checks in `is_operator_caller` - a loopback transport peer, a
loopback `Host` authority, and same-origin-or-headerless (the request carries no `Origin`
header, or its `Origin` names the same authority as its `Host`) - and never gate an operator
route on the peer alone: every operator route gates through `is_operator_caller`.** The `Host`
and `Origin` checks together are the CSRF guard for the `/admin/` page over an ssh tunnel -
same-origin refuses a cross-origin fetch, and the loopback `Host` refuses DNS rebinding.

**A diff that adds a route a public caller needs also adds it to `caddy.snippet`, in the same
change as its handler; never edit `caddy.snippet` to match the deployed Caddyfile - edit the
deployed Caddyfile to match `caddy.snippet` instead.**

**On a route that serves board data, mutates the store, or refuses a caller, a response path
that does not log one `ladder.req` line through `log_request` is a defect - refusals log too.**
The bare liveness probe `GET /healthz` is none of these and logs nothing.

**A handler that reads a request body without first checking `body_is_byte_faithful` is a
defect.**

**A store-backed handler that does anything beyond four things - check transport shape, gate
the request (operator gate, submit-open, attempt-limit), make one store call, format the
response - is a defect.** SQL, hashing, and store policy go in `ladder_store.das`, which never
requires `dashv`. The `/admin/` page and `/healthz` reach no store and are not store-backed
handlers.

**A diff that defaults `submit_open` to true in `LadderArgs` or `LadderPolicy`, lets
`/api/submit/sidecar` or `/api/submit/records` answer anything but 403 while `submit_open` is
false, or adds an opener reachable from a non-loopback path, is a defect** - only the loopback
`/admin/submit` route flips the gate.

**In `caddy.snippet` every proxied route other than the `/api/submit/records` and
`/api/submit/sidecar` matcher carries the small read cap; a large-body allowance on any other
route, or a read cap on either submit matcher, is a defect.**

**Never require the dasLLAMA engine, dasLLVM, or any model machinery from a file in this
directory - the one dasLLAMA module allowed is the engine-free
`dasllama/dasllama_exchange_schema` (a public entry of that module's facade lint), required
from `ladder_store.das` and nowhere else** (`README.md` sec.3). Its `dasllama_lint` carrier is
a compile-time macro, not engine code.

**A write path that stores a document without validating it first is a defect: every sidecar -
community or planted - passes `validate_sidecar_submission`, every community record store
passes `validate_record_submission`, and `import_official_store` passes `validate_record_store`
(shape only - official record history predates the release counter).** A sidecar without the
version stamp can never be served, so storing one is always an error.

**Never write `Source` or `Verified` outside store code, and never let a value from a public
(proxied) request reach either column - the loopback operator surface sets `Verified` through
store calls, never through SQL in a handler.**

**Never validate, hash, or store sidecar text - community or planted - that has not been
through `exchange_strip_private`, and never store a community record store that has not been
through `redact_record_paths`.** Hashing the cleaned text on both sidecar paths is what makes
re-plant promote instead of duplicate. `import_official_store` stores its document as-is.

**Never update a document after its insert - derived columns may be recomputed, and a document
leaves only by deleting its submission.**

**Never edit a shipped `[sql_migration]` body - a schema change adds a new, higher version in
the same stream.**

**A sidecar row whose `Sha` is not the sha256 of its stored `Doc` is a defect.**

**A diff that adds an operator-edited file the box runs from also adds it to `.das_package`'s
`release()`, in the same change.**

**A privileged (root) deploy action that does not go through `dasllama-deploy.sh` is a defect,
and so is any `dasllama-deploy.sudoers` grant beyond NOPASSWD for exactly
`/usr/local/sbin/dasllama-deploy.sh`** - a second command, a wildcard target, a bare `ALL`, or
a shell.

**A diff that changes where the service or watchdog writes at runtime - log path, working
directory, database location - also adds a matching `ReadWritePaths` entry to the systemd unit
`provision` writes, in the same change.** Relaxing that unit's sandbox is a defect:
`ProtectSystem=strict`, an emptied `CapabilityBoundingSet`, and `ReadWritePaths` no wider than
the data dir and the release tree.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` - launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no SQL, no hashing.
- `ladder_config.das` - the config schema (`LadderArgs`), the defaults/toml/CLI merge with
  per-key provenance, the startup banner payload. No HTTP, no SQL, no filesystem beyond
  reading the config file.
- `ladder_server.das` - the `HvWebServer` class: route table and handlers - transport shape,
  request gating, HTTP-to-one-store-call translation, response formatting - plus the server
  lifecycle (`init`/`update`/`shutdown`, the db open and migrate at boot), the official-dir
  boot import, and serving `admin.html`. No SQL statements, no hashing, no store policy.
- `ladder_store.das` - the store: schema structs, migrations, content hashing, and every
  policy decision (size caps, rate ceiling, source stamping, the sidecar lookup ladder).
  Zero HTTP.
- `admin.das` - operator CLI: argv dispatch onto `ladder_store` calls. No store mutation that
  bypasses `ladder_store` functions.
- `admin.html` - the loopback `/admin/` operator page: renders `GET /admin/sidecars`, drives
  the sidecar verify/delete/import routes and the gate flip. No route of its own, no direct
  db access, nothing public-facing.
- `caddy.snippet` - the public route boundary's authoritative copy.
- `.das_package` - the daspkg release manifest: package/release names and the
  `release_include*` set of operator files carried onto the box.
- `dasllama-deploy.sh` - the box-side deploy tool (`provision`/`caddy`/`install`/
  `open-submit`/`close-submit`/`sidecars`/`promote`/`demote`/`plant`/`status`), installed
  root-owned as the one privileged surface.
- `dasllama-deploy.sudoers` - the drop-in that scopes that privilege.
- `dasllama-ladder.toml` - the shipped default config: the on-box template whose keys mirror
  `LadderArgs`; operator-edited after deploy.
- `watchdog.json` - the watchdog deploy config (health and shutdown URLs);
  operator-edited after deploy.
- `_ladder_test_common.das` - shared test fixtures, required by bare name from the store and
  server suites (the leading `_` keeps dastest's walker away).
- `test_ladder_store.das` - the store suite: `ladder_store` calls against `:memory:`, no server.
- `test_ladder_server.das` - the HTTP suite: routes through `with_ladder_server` on port 19015.
- `test_ladder_config.das` - the config suite: the defaults/toml/CLI merge and its provenance.
