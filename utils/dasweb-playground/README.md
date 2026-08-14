# dasweb-playground

The daslang.io playground backend: permanent share links (`daslang.io/s/<hash>`) over a
content-addressed sample store. Phase 1 of `plans/dasweb_backend.md`; later phases add the
curated-sample listing and the wasm build queue. Review rules: `REVIEW.md` (binding).

## Run

```bash
# development (interpreted or -jit)
daslang utils/dasweb-playground/main.das -- --port 8101 --db samples.db

# supervised production form (inside a daspkg release bundle)
python3 watchdog.py

# release bundle
daslang utils/daspkg/main.das -- release --root utils/dasweb-playground --out <dir>

# box-side install of a shipped bundle (build recipe in the script header)
sudo deploy.sh <short-sha> /tmp/dasweb-playground-<short-sha>.tar.gz
```

`-?` prints flag help (the daslang host eats `--help`). Config: `dasweb-playground.toml` in cwd
or beside the tool, keys = flag names; precedence defaults < toml < explicit CLI flags, flipped
by `authoritative = true`. A key whose value has the wrong type is a startup error, not a
silent zero. The effective config is logged at startup with per-key provenance.

The checked-in toml holds development values, because it is discovered automatically beside the
module. The deployed values on `dasweb-1` are `db = "/srv/dasweb-playground/samples.db"`,
`base_url = "https://daslang.io"`,
`curated_dir = "/srv/daslang.io/current/playground/samples"`,
`blobs_dir = "/srv/dasweb-playground/blobs"`, and a real `build_token` (toml-only there —
a CLI flag would put the secret in the process list); that file and `watchdog.json` are
operator-owned — shipped only when absent, and carried across upgrades by `deploy.sh`.

The public route boundary is `caddy.snippet`, the authoritative copy of what the daslang.io
vhost forwards here. It also carries the transport body cap: the service can only bounds-check
a body already buffered in full, so that limit cannot live in this process.

## Endpoints (port 8101, loopback only — Caddy fronts the internet)

| Route | Behavior |
|---|---|
| `POST /api/samples` | body = raw `.das` text → `{hash, url, created}`; 400 empty/non-utf8/embedded NUL, 413 over size cap, 429 over per-IP ceiling |
| `GET /api/samples/listed` | the curated dropdown: `[{hash, title, category, path, slug}]`; manifest order within a category, categories alphabetical |
| `GET /api/samples/<hash>` | the stored source, `text/plain`, `nosniff`, immutable cache headers; 400 malformed hash, 404 unknown |
| `GET /s/<hash>` | 302 → `/playground/index.html?s=<hash>`; 404 unknown |
| `GET /healthz` | `ok` (watchdog poll target) |
| `POST /shutdown` | graceful stop, exit 0 (watchdog contract); loopback peers only, else 403 |
| `POST /admin/reimport` | re-run the curated importer; 503 when `curated_dir` unset; loopback peers only, else 403 |
| `GET /api/build/info` | `{enabled, toolchain}` — whether builds are available at all; the playground asks once at load to decide whether to offer the wasm engine |
| `POST /api/build/request/<hash>` | enqueue a wasm build of a stored sample under the current toolchain → the status payload; 404 unknown sample, 503 while builds are unavailable |
| `GET /api/build/status/<hash>` | `{state}` + state-specific fields: `position` (queued), `error` (failed), `files` = artifact URLs (done); 404 when never requested |
| `GET /api/build/artifact/<toolchain>/<hash>/<file>` | a built artifact file, immutable cache headers + `nosniff`; every component validated, 404 otherwise |
| `POST /api/build/toolchain` | **bearer** — the builder announces its toolchain id (body = git sha) after its ABI canary ran clean; this is the toolchain's go-live |
| `POST /api/build/next` | **bearer** — claim the oldest queued job for the builder's toolchain (body = its git sha) → `{hash, toolchain_id, mode, source}`; 204 when idle |
| `POST /api/build/result` | **bearer** — multipart result: fields `hash`, `toolchain`, `ok`; `ok=0` carries `error` (stored, shown to the user), `ok=1` carries `manifest` (`[{name, sha256}]`) plus one file part per name; 409 when the job is not building |

Client identity for the rate ceiling is the **last** `X-Forwarded-For` hop — the one Caddy
appends. Earlier hops are client-authored. The operator routes verify the transport peer
instead, which no header can forge; no route enables CORS.

Listing order is reproducible rather than authored: `read_json` parses the manifest object into
a table, so the file's category key order is gone before the importer sees it. Categories are
therefore visited sorted, and entries keep the manifest's order within their category.

Curated samples: when `curated_dir` points at the deployed playground samples tree, boot (and
`/admin/reimport`) imports every `data.json` entry through the store — single-file entries as
raw source, multi-file as the `{files, active}` bundle user shares use. Unchanged files dedup;
changed files repoint the listing; vanished entries demote (permalinks always survive). A
manifest path that would escape `curated_dir` is refused, and an import in which nothing was
readable keeps the previous listings rather than emptying the dropdown. The site deploy nudges
`/admin/reimport` so a newly shipped sample appears without a restart.

Manual curation and build-queue operation: `admin.das`
(`--op list | promote | demote | enqueue | build-status | set-toolchain`), shipped in the
bundle and run with a daslang SDK on the box (the baked exe has no interpreter):

```bash
/opt/daslang/bin/daslang admin.das -- --db /srv/dasweb-playground/samples.db --op list
```

A promoted sample is operator-owned from then on — promotion clears the importer's ownership
marker, so a later import cannot silently demote it.

Dedup is content-addressed: same source ⇒ same hash ⇒ same URL, first writer's metadata wins.

## The wasm build pipeline (phase 3 of `plans/dasweb_wasm_pipeline.md`)

The compute box **pulls**: `utils/dasweb-buildd` on zen4 polls `/api/build/next` over HTTPS
with a bearer token (`build_token` in the toml; empty disables the whole surface — set it the
same on both boxes). Nothing here ever connects to the builder, so builder death degrades to a
backed-up queue, never an outage. A build job is a `(source hash × toolchain id)` pair;
`toolchain_id` = the builder worktree's git sha, so a toolchain bump lazily invalidates the
artifact cache by construction. A claimed job the builder never resolves returns to the queue
after `build_claim_timeout_seconds`; `max_build_attempts` claims later it fails for good. A
failed build is terminal — compile errors are deterministic — until an operator re-enqueues
with `admin.das --op enqueue --force`.

Artifacts are **file sets** (a playground sample is one `.wasm`; a game page is
`html + js + wasm`), stored under `<blobs_dir>/<toolchain>/<hash>/` and served static with
immutable headers. An upload is staged under `blobs/tmp/`, every file verified against its
manifest sha256, and the whole set renamed into place atomically — a half-written artifact is
never served. `caddy.snippet` carries the 64MB transport cap for `/api/build/*` uploads.

A build's **mode** is decided here at request time by scanning the stored source's requires
(`detect_build_mode` in `build_queue.das`): a native graphics/audio namespace (glfw, opengl,
audio, …) makes it a `page` build — a standalone `sample.{html,js,wasm}` emscripten page,
because GL and audio live in emscripten's JS glue that a bare wasi module cannot carry —
everything else stays a `module` build (one `sample.wasm` the playground runtime
instantiates). Status responses carry `kind` so the playground knows how to run the result.

**Page artifacts are user-influenced html/js and are never daslang.io content.** `page_origin`
in the toml (production: `https://run.daslang.io`, a cookie-less grey-cloud A record to this
box) names the only Host the service serves `.html`/`.js` artifacts on — Caddy routes that
vhost to artifact GETs only, and the service enforces the Host itself, so a Caddyfile drift
fails closed. Page-artifact urls in status responses go out absolute on that origin; the
playground embeds them in an un-sandboxed iframe (`sandbox` would opaque the origin and break
emscripten's worker spawning — the separate origin is the boundary) with
`allow="cross-origin-isolated"`. Artifact responses carry `Cross-Origin-Resource-Policy:
cross-origin` (embeddable under the playground's COEP) and `Cross-Origin-Embedder-Policy:
require-corp` (the page's own agent cluster isolates, so `-pthread` programs get their
SharedArrayBuffer; the js carries it too because a COEP'd document may only spawn workers
whose script response itself carries an enforcing COEP).

The builder's protocol on a toolchain bump: build, run the `--jit-check-abi` canary clean,
**then** `POST /api/build/toolchain` — announcing is the go-live. Queued jobs of earlier
toolchains are simply never claimed again; rows stay for the audit trail.

## Files

One file, one rule — the authoritative placement table is in `REVIEW.md`. Data lives in
SQLite (`samples` table, append-only `[sql_migration]` stream); every request and store
mutation is logged as ndjson to `logs/dasweb-playground.log` (rotation is the watchdog's job).

## Tests

```bash
daslang dastest/dastest.das -- --test utils/dasweb-playground/test_samples_store.das
```

CI runs all seven suites in `extended_checks.yml` ("Test dasweb-playground") — a hyphenated
directory means siblings are required by bare name, so `--test ./tests` never reaches them.

In-dir per the session rules: stores (no HTTP: samples, build queue), artifact cache (temp-dir
fixtures), servers (real HTTP on reserved ports 19011/19012/19013, including the multipart
artifact-upload round trip), importer (temp-dir fixtures), config (pure merge). Every bug fix
lands with its regression test.
