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
```

`-?` prints flag help (the daslang host eats `--help`). Config: `dasweb-playground.toml` in cwd
or beside the tool, keys = flag names; precedence defaults < toml < explicit CLI flags, flipped
by `authoritative = true`. The effective config is logged at startup with per-key provenance.

## Endpoints (port 8101, loopback only — Caddy fronts the internet)

| Route | Behavior |
|---|---|
| `POST /api/samples` | body = raw `.das` text → `{hash, url, created}`; 400 empty/non-utf8, 413 over size cap, 429 over per-IP ceiling |
| `GET /api/samples/<hash>` | the stored source, `text/plain`, immutable cache headers; 400 malformed hash, 404 unknown |
| `GET /s/<hash>` | 302 → `/playground/index.html?s=<hash>`; 404 unknown |
| `GET /healthz` | `ok` (watchdog poll target) |
| `POST /shutdown` | graceful stop, exit 0 (watchdog contract; never routed by Caddy) |

Dedup is content-addressed: same source ⇒ same hash ⇒ same URL, first writer's metadata wins.

## Files

One file, one rule — the authoritative placement table is in `CODEREVIEW.md`. Data lives in
SQLite (`samples` table, append-only `[sql_migration]` stream); every request and store
mutation is logged as ndjson to `logs/dasweb-playground.log` (rotation is the watchdog's job).

## Tests

```bash
daslang dastest/dastest.das -- --test utils/dasweb-playground/
```

In-dir per the session rules: store (no HTTP), server (real HTTP on port 19011), config
(pure merge). Every bug fix lands with its regression test.
