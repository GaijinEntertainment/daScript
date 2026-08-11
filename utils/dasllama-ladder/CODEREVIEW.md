# dasllama-ladder Code Review Checklist

Run this list on every dasllama-ladder change before it ships — including changes to this
file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code, without prior
knowledge of the service, and without opening another document.** If an entry needs any of
those, it is not a review criterion — move it to `README.md` and leave a one-line criterion
here.

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how to
  write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything that
  needs a stable reference lives in `README.md`, which is numbered for that purpose.
- **Cite files by name; cite `README.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.**
- **No history, no rationale, no direction of travel.** The reason lives in `README.md` or the
  plan; planned work lives in `plans/dasllama_io_site.md`.

---

## Tests

**Every route, every store operation, and every config or limit behavior has a dastest test in
this directory.** A behavior that ships without its test is a defect. Exempt: `main.das` and
`admin.das` argv/dispatch glue — their behaviors live in the modules they call, which are the
tested surface.

**Every bug fix lands with the regression test that fails without it, in the same change.**

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**HTTP tests go through the local `with_ladder_server` harness (`test_ladder_server.das`) on
this directory's reserved test port 19015; store tests call `ladder_store` directly with no
server.** A store behavior proven only through HTTP, or an HTTP behavior proven only against
the store, is a defect.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates; store tests run against `:memory:`.** A test writing into the repo tree is a defect.

---

## Placement — one file, one rule

Every file states what it holds. Code that belongs to a file and is written anywhere else is a
defect, and this section is the whole test.

**A new file ships with its rule here and its tests, in the same change.**

- `main.das` — the launcher: argv parsing into globals, logger init, the exported
  `init`/`update`/`shutdown` lifecycle, the standalone GC loop, exit-code mapping. No route,
  no SQL, no hashing.
- `ladder_config.das` — the config schema (`LadderArgs`), the defaults/toml/CLI merge with
  per-key provenance, and the startup banner payload. No HTTP, no SQL, no filesystem beyond
  reading the config file.
- `ladder_server.das` — the `HvWebServer` class: the route table and handlers, plus the
  startup official-dir import. A handler validates transport-level shape, translates HTTP to
  one store call, and formats the response. A SQL statement, a hash computation, or a policy
  decision (size, rate, source) in this file is a defect.
- `ladder_store.das` — the store: schema structs, migrations, content hashing, and every
  policy decision (size caps, rate ceiling, source stamping, the sidecar lookup ladder). Zero
  HTTP: a require of `dashv` here is a defect.
- `admin.das` — the operator CLI: argv dispatch onto `ladder_store` calls. A store mutation
  here that bypasses `ladder_store` functions is a defect.
- `caddy.snippet` — the authoritative copy of the public route boundary; the deployed
  Caddyfile is edited to match it, never the reverse. A route added to `ladder_server.das`
  that a public caller needs lands here in the same change.

---

## Transport

**`/admin/*` and `/shutdown` answer only to a loopback transport peer (`is_loopback_peer`,
exact match), and never appear in `caddy.snippet`.** A proxy route to an operator surface is
a defect.

**Every route logs one `ladder.req` line through `log_request`, including refusals.** A
response path that skips the log is a defect.

**Every handler that consumes a request body checks `body_is_byte_faithful` before using the
string view.** A body used without the NUL guard is a defect.

---

## Store

**Nothing in this directory requires dasLLAMA, dasLLVM, or any engine module.** The one
sanctioned cross-tree require is `modules/dasLLAMA/performance/exchange_schema.das`, itself
engine-free; see `README.md` §3.

**Every community document is validated by `exchange_schema` at submission grade before any
row is written**, and `import_official_store` validates at shape grade. A write path that
skips validation is a defect.

**`Source` and `Verified` are written only from store code.** A submitter-supplied value
reaching either column is a defect.

**A sidecar submission is privacy-stripped (`exchange_strip_private`) before validation,
hashing, and storage — the stripped text IS the document; record submissions are stored
verbatim. After insert no document is ever mutated.** Derived columns may be recomputed; the
document itself may only be inserted or deleted with its submission.

**A shipped `[sql_migration]` body is never edited.** Schema change means a new, higher
version in the same stream.

**Sidecars are content-addressed: the row key is the sha256 of the stored document.** A
sidecar row whose `Sha` is not the hash of its `Doc` is a defect.
