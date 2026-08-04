# dasweb-playground

The daslang.io playground backend: permanent share links (`daslang.io/s/<hash>`) over a
content-addressed sample store. Phase 1 of `plans/dasweb_backend.md`; later phases add the
curated-sample listing and the wasm build queue. Review rules: `CODEREVIEW.md` (binding).

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
`base_url = "https://daslang.io"`, and
`curated_dir = "/srv/daslang.io/current/playground/samples"`; that file and `watchdog.json` are
operator-owned — shipped only when absent, and carried across upgrades by `deploy.sh`.

The public route boundary is `caddy.snippet`, the authoritative copy of what the daslang.io
vhost forwards here. It also carries the transport body cap: the service can only bounds-check
a body already buffered in full, so that limit cannot live in this process.

## Endpoints (port 8101, loopback only — Caddy fronts the internet)

| Route | Behavior |
|---|---|
| `POST /api/samples` | body = raw `.das` text → `{hash, url, created}`; 400 empty/non-utf8/embedded NUL, 413 over size cap, 429 over per-IP ceiling |
| `GET /api/samples/listed` | the curated dropdown: `[{hash, title, category, path, slug}]`, in manifest order |
| `GET /api/samples/<hash>` | the stored source, `text/plain`, `nosniff`, immutable cache headers; 400 malformed hash, 404 unknown |
| `GET /s/<hash>` | 302 → `/playground/index.html?s=<hash>`; 404 unknown |
| `GET /healthz` | `ok` (watchdog poll target) |
| `POST /shutdown` | graceful stop, exit 0 (watchdog contract); loopback peers only, else 403 |
| `POST /admin/reimport` | re-run the curated importer; 503 when `curated_dir` unset; loopback peers only, else 403 |

Client identity for the rate ceiling is the **last** `X-Forwarded-For` hop — the one Caddy
appends. Earlier hops are client-authored. The operator routes verify the transport peer
instead, which no header can forge; no route enables CORS.

Curated samples: when `curated_dir` points at the deployed playground samples tree, boot (and
`/admin/reimport`) imports every `data.json` entry through the store — single-file entries as
raw source, multi-file as the `{files, active}` bundle user shares use. Unchanged files dedup;
changed files repoint the listing; vanished entries demote (permalinks always survive). A
manifest path that would escape `curated_dir` is refused, and an import in which nothing was
readable keeps the previous listings rather than emptying the dropdown. The site deploy nudges
`/admin/reimport` so a newly shipped sample appears without a restart.

Manual curation: `admin.das` (`--op list | promote | demote`), shipped in the bundle and run
with a daslang SDK on the box (the baked exe has no interpreter):

```bash
/opt/daslang/bin/daslang admin.das -- --db /srv/dasweb-playground/samples.db --op list
```

A promoted sample is operator-owned from then on — promotion clears the importer's ownership
marker, so a later import cannot silently demote it.

Dedup is content-addressed: same source ⇒ same hash ⇒ same URL, first writer's metadata wins.

## Files

One file, one rule — the authoritative placement table is in `CODEREVIEW.md`. Data lives in
SQLite (`samples` table, append-only `[sql_migration]` stream); every request and store
mutation is logged as ndjson to `logs/dasweb-playground.log` (rotation is the watchdog's job).

## Tests

```bash
daslang dastest/dastest.das -- --test utils/dasweb-playground/test_samples_store.das
```

CI runs all four suites in `extended_checks.yml` ("Test dasweb-playground") — a hyphenated
directory means siblings are required by bare name, so `--test ./tests` never reaches them.

In-dir per the session rules: store (no HTTP), server (real HTTP on reserved ports
19011/19012), importer (temp-dir fixtures), config (pure merge). Every bug fix lands with its
regression test.
