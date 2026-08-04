# dasweb-playground Code Review Checklist

Run this list on every dasweb-playground change before it ships — including changes to this
file.

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
  plan; planned work lives in `plans/dasweb_backend.md`.

---

## Tests

**Every route, every store operation, and every config or limit behavior has a dastest test in
this directory.** A behavior that ships without its test is a defect. Exempt: `main.das` and
`admin.das` argv/dispatch glue — their behaviors live in the modules they call, which are the
tested surface.

**Every bug fix lands with the regression test that fails without it, in the same change.**

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**HTTP tests go through `with_playground_server_at` in `test_playground_server.das`, on this
directory's reserved test ports 19011 and 19012; store tests call `samples_store` directly with
no server.** A store behavior proven only through HTTP, or an HTTP behavior proven only against
the store, is a defect.

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
  per-key provenance, and the startup banner payload. No HTTP, no SQL, no filesystem beyond
  reading the config file.
- `playground_server.das` — the `HvWebServer` class: the route table and handlers. A handler
  validates transport-level shape, translates HTTP to one store call, and formats the response.
  A SQL statement, a hash computation, or a policy decision (size, rate, listing) in this file
  is a defect.
- `samples_store.das` — the store: schema structs, migrations, store operations, content
  hashing, and every policy decision (size cap, rate ceiling, listing). Zero HTTP: a require
  of `dashv` here is a defect.
- `curated_import.das` — the data.json-driven curated importer: manifest parsing, sample-file
  reads, bundle assembly, calling the store. The only file besides the config loader that reads
  the filesystem; a store mutation here that bypasses `samples_store` functions is a defect.
- `admin.das` — the operator CLI (listing curation). Talks to the store the same way the
  server does; a second implementation of a store operation here is a defect.
- `.das_package`, `watchdog.json`, `dasweb-playground.toml`, `deploy.sh` — packaging and
  deployment. A behavior change hidden in these files without a note in `README.md` (Run
  section) is a defect.

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
stored.** (dasHV buffers the body before any handler runs, so the check happens in
`put_sample` ahead of hashing — a hash or insert reachable without the size check is a defect.)

**Client identity comes from `X-Forwarded-For` and is treated as data, never parsed into
behavior beyond the rate ceiling.** Logging it is required; branching on it anywhere else is a
defect.

**No shell-out anywhere in the service.**

**No filesystem path derived from request data.** The importer's paths come from config plus
the deploy-tree manifest only.

**No `unsafe` in any route handler.** An `unsafe` elsewhere takes a same-line reason comment;
one without the comment is a defect.

**`POST /shutdown` and every admin operation stay unrouted by Caddy.** A Caddy config change in
this directory that forwards them, or an admin endpoint added to the public route set, is a
defect.

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
