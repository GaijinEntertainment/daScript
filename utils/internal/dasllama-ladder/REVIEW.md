# dasllama-ladder Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasllama_io_site.md`.

**Every route, every store operation, and every config or limit behavior has a dastest test in
this directory** — `main.das` and `admin.das` stay argv/dispatch glue over tested modules, so
they need none of their own.

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**HTTP tests go through the local `with_ladder_server` harness (`test_ladder_server.das`) on
this directory's reserved test port 19015; store tests call `ladder_store` directly with no
server.** A store behavior proven only through HTTP, or an HTTP behavior proven only against
the store, is a defect.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates; store tests run against `:memory:`.** A test writing into the repo tree is a defect.

**Operator routes (`/admin/*`, `/shutdown`) never appear in `caddy.snippet` — or in any
Caddyfile route reaching this service, a catch-all included.** Caddy proxies from the same box,
so `is_loopback_peer` sees `127.0.0.1` for every proxied request — the Caddyfile is the only
real boundary. Operators reach these routes on the service port itself, over the ssh tunnel;
no Caddyfile entry exists for them.

**`is_operator_caller` requires ALL THREE of a loopback transport peer, a loopback `Host`
authority, and same-origin-or-headerless (the request carries no `Origin` header, or its
`Origin` names the same authority as its `Host`), and every operator route gates through it.**
Dropping any of the three, or gating an operator route on the peer alone, is a defect. The
`Host` and `Origin` halves together are the CSRF guard for the `/admin/` page over an ssh
tunnel: same-origin refuses a plain cross-origin fetch, and the loopback-`Host` requirement
refuses DNS rebinding (comparing `Origin` to `Host` alone does not — a rebound name owns both).

**`caddy.snippet` is the authoritative copy of the public route boundary: the deployed
Caddyfile is edited to match it, never the reverse, and a route a public caller needs lands in
it in the same change as its handler.**

**Every route that serves board data, mutates the store, or refuses a caller logs one
`ladder.req` line through `log_request`, including the refusals.** A response path on such a
route that skips the log is a defect. (The bare liveness probe `GET /healthz` serves no data
and is silent by design.)

**Every handler that consumes a request body checks `body_is_byte_faithful` before using the
string view.** A body used without the NUL guard is a defect.

**A store-backed handler does exactly four things — validate transport shape, gate the request
(operator gate, submit-open, attempt-limit), make one store call, format the response.** SQL,
hashing, and store policy live in `ladder_store.das`, and HTTP never does — no `dashv` require
there. (The `/admin/` page and `/healthz` reach no store and are not store-backed handlers.)

**Public submissions default closed: `submit_open` is false in both `LadderArgs` and
`LadderPolicy`, the `/api/submit/sidecar` and `/api/submit/records` handlers return 403 while it
is false, and only the loopback `/admin/submit` route flips it.** A default of true, or an opener
reachable from any non-loopback path, is a defect.

**In `caddy.snippet` the large-body allowance appears on the `/api/submit/records` and
`/api/submit/sidecar` matcher only; every other proxied route carries the small read cap.** A
large cap on any other route, or a read cap on a submit route, is a defect.

**Nothing in this directory requires the dasLLAMA engine, dasLLVM, or any model machinery; the
only dasLLAMA module required is the engine-free `dasllama/dasllama_exchange_schema` (a public
entry of that module's facade lint), and only `ladder_store.das` requires it** (`README.md`
§3). Its `dasllama_lint` carrier is a compile-time macro, not engine code.

**Every sidecar document — community or planted — passes `validate_sidecar_submission` before
any row is written (a sidecar without the version stamp can never be served, so storing one is
always an error); every community record store passes `validate_record_submission`;
`import_official_store` validates with `validate_record_store` (shape only — official record
history predates the release counter).** A write path that skips validation is a defect.

**`Source` and `Verified` are written only from store code, and no value from a public
(proxied) request reaches either column.** The loopback operator surface chooses `Verified`
by design — through store calls, never through SQL in a handler.

**Every sidecar — community or planted — is privacy-cleaned through `exchange_strip_private`
before validation, hashing, and storage, and a community record store through
`redact_record_paths`; the cleaned text IS the document.** Hashing the cleaned text on both
sidecar paths is what makes re-plant promote instead of duplicate. `import_official_store`
alone stores its document as-is.

**After insert a document is never mutated.** Derived columns may be recomputed; the document
itself may only be inserted, or deleted with its submission.

**A shipped `[sql_migration]` body is never edited.** Schema change means a new, higher
version in the same stream.

**Sidecars are content-addressed: the row key is the sha256 of the stored document.** A
sidecar row whose `Sha` is not the hash of its `Doc` is a defect.

**A new operator-edited file the box runs from is added to `.das_package`'s `release()` in the
same change.**

**Every privileged (root) deploy action goes through `dasllama-deploy.sh`, and
`dasllama-deploy.sudoers` grants NOPASSWD for exactly `/usr/local/sbin/dasllama-deploy.sh` and
nothing else** — a second command, a wildcard target, a bare `ALL`, or a shell is a defect.

**The systemd unit that `provision` writes runs the service sandboxed** (`ProtectSystem=strict`, emptied
`CapabilityBoundingSet`, `ReadWritePaths` limited to the data dir and the release tree). A diff
that changes where the service or watchdog writes at runtime — log path, working directory,
database location — without a matching `ReadWritePaths` entry is a defect; so is relaxing
`ProtectSystem` or restoring a capability.

**Placement — one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` — launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no SQL, no hashing.
- `ladder_config.das` — the config schema (`LadderArgs`), the defaults/toml/CLI merge with
  per-key provenance, the startup banner payload. No HTTP, no SQL, no filesystem beyond
  reading the config file.
- `ladder_server.das` — the `HvWebServer` class: route table and handlers — transport shape,
  request gating, HTTP-to-one-store-call translation, response formatting — plus the server
  lifecycle (`init`/`update`/`shutdown`, the db open and migrate at boot), the official-dir
  boot import, and serving `admin.html`. No SQL statements, no hashing, no store policy.
- `ladder_store.das` — the store: schema structs, migrations, content hashing, and every
  policy decision (size caps, rate ceiling, source stamping, the sidecar lookup ladder).
  Zero HTTP.
- `admin.das` — operator CLI: argv dispatch onto `ladder_store` calls. No store mutation that
  bypasses `ladder_store` functions.
- `admin.html` — the loopback `/admin/` operator page: renders `GET /admin/sidecars`, drives
  the sidecar verify/delete/import routes and the gate flip. No route of its own, no direct
  db access, nothing public-facing.
- `caddy.snippet` — the public route boundary's authoritative copy.
- `.das_package` — the daspkg release manifest: package/release names and the
  `release_include*` set of operator files carried onto the box.
- `dasllama-deploy.sh` — the box-side deploy tool (`provision`/`caddy`/`install`/
  `open-submit`/`close-submit`/`sidecars`/`promote`/`demote`/`plant`/`status`), installed
  root-owned as the one privileged surface.
- `dasllama-deploy.sudoers` — the drop-in that scopes that privilege.
- `dasllama-ladder.toml` — the shipped default config: the on-box template whose keys mirror
  `LadderArgs`; operator-edited after deploy.
- `watchdog.json` — the watchdog deploy config (health and shutdown URLs);
  operator-edited after deploy.
- `_ladder_test_common.das` — shared test fixtures, required by bare name from the store and
  server suites (the leading `_` keeps dastest's walker away).
- `test_ladder_store.das` — the store suite: `ladder_store` calls against `:memory:`, no server.
- `test_ladder_server.das` — the HTTP suite: routes through `with_ladder_server` on port 19015.
- `test_ladder_config.das` — the config suite: the defaults/toml/CLI merge and its provenance.
