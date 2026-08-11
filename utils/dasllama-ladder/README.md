# dasllama-ladder — the dasllama.io exchange service

The store (and, in later slices, the HTTP surface) behind dasllama.io: community tune
sidecars are the product, bench-record measurements are the community bonus riding along.
The site plan and trust model live in `plans/dasllama_io_site.md`; the checklist for every
change here is `CODEREVIEW.md`.

## 1. Layout

- `main.das` — the launcher (clargs → config → `init`/`update`/`shutdown` lifecycle, GC loop).
- `ladder_config.das` — config schema + the defaults < toml < CLI merge (`dasllama-ladder.toml`).
- `ladder_server.das` — the HTTP surface (routes, transport shape checks, official-dir import).
- `ladder_store.das` — schema, migrations, and every policy decision (size caps, rate
  ceiling, source stamping, the sidecar lookup ladder). Zero HTTP.
- `admin.das` — the operator CLI (verify/delete levers, manual official import; run on the box).
- `caddy.snippet` / `watchdog.json` / `dasllama-ladder.toml` — the deploy contract.
- `_ladder_test_common.das` + `test_ladder_*.das` — fixtures and the three dastest suites
  (store, config, server; the server suite owns reserved port 19015).

The anatomy mirrors `utils/dasweb-playground`.

### 1.1 Routes

Public (proxied by `caddy.snippet`): `GET /api/versions`, `GET /api/runs[?version=N]`,
`GET /api/submission/:id` (the verbatim receipt), `GET /api/sidecars?version=N&box=<encoded>`
(the lookup ladder), `GET /api/sidecar/:sha` (download), `POST /api/submit/records`,
`POST /api/submit/sidecar`. Loopback-only: `POST /admin/import-official`, `POST /shutdown`,
plus `GET /healthz` for the watchdog. The `box` query value must be percent-encoded — box
strings carry `|`, `,` and spaces.

## 2. Data model

Three tables, one migration stream (`ladder_migration_*`):

- `submissions` — one row per accepted upload, the document kept **verbatim** (`Doc`),
  content-hashed (`DocSha`, unique — byte-identical resubmits dedup). Kinds: `records`,
  `sidecar`, `official`.
- `runs` — one row per BenchRun, flat columns for filtering; the truth stays in the
  submission `Doc` at (`ModelIdx`, `RunIdx`). Identity per `modules/dasLLAMA/METHODOLOGY.md`
  is (`Gguf`, `Box`, `Engine`, `Backend`, `Flavor`, `Workload`); official imports replace on
  it, community rows append.
- `sidecars` — content-addressed by the document's own sha256; identity columns come from
  `exchange_schema`'s `parse_sidecar_info` and drive the lookup ladder: exact box → same
  platform/arch/cpu → same platform/arch, verified first within a tier, newest first.

Validation is `modules/dasLLAMA/performance/exchange_schema.das` (required by path, re-exported
`public`): submission grade for community uploads, shape grade for official imports (official
history predates `DASLLAMA_VERSION`). `Source` and `Verified` are always stamped by the store;
nothing submitter-supplied reaches those columns.

## 3. Engine-free rule

The service builds and runs without dasLLAMA, dasLLVM, or any model machinery — dasweb-1 has
no GPU and no reason to compile an inference engine. `exchange_schema` carries the same rule
on its side (`modules/dasLLAMA/CODEREVIEW.md`).

## 4. Running the tests

```
bin/Release/daslang.exe dastest/dastest.das -- --test utils/dasllama-ladder/test_ladder_store.das
```

Plain dastest — the same lane `extended_checks` runs the dasweb-playground suites in.

## 5. Follow-ups

The HTTP surface (routes, rate/size enforcement at the transport layer, admin levers over
loopback), site wiring, and client integration are later slices of the arc plan; the
follow-up ledger (partial re-race, cross-box matrix, version diff) is in
`plans/dasllama_io_site.md`.
