# dasllama-ladder — the dasllama.io exchange service

The store (and, in later slices, the HTTP surface) behind dasllama.io: community tune
sidecars are the product, bench-record measurements are the community bonus riding along.
The site plan and trust model live in `plans/dasllama_io_site.md`; the checklist for every
change here is `REVIEW.md`.

## 1. Layout

- `main.das` — the launcher (clargs → config → `init`/`update`/`shutdown` lifecycle, GC loop).
- `ladder_config.das` — config schema + the defaults < toml < CLI merge (`dasllama-ladder.toml`).
- `ladder_server.das` — the HTTP surface (routes, transport shape checks, official-dir import).
- `ladder_store.das` — schema, migrations, and every policy decision (size caps, rate
  ceiling, source stamping, the sidecar lookup ladder). Zero HTTP.
- `admin.das` — the operator CLI (verify/delete levers, manual official/sidecar imports; needs
  a daslang on the box — the HTTP admin surface below is the interpreter-free twin).
- `admin.html` — the loopback `/admin/` operator page (sidecar queue, promote/demote/delete,
  plant, the submit-gate toggle), served beside the sources like the dasllama-server control page.
- `caddy.snippet` / `watchdog.json` / `dasllama-ladder.toml` — the deploy contract.
- `_ladder_test_common.das` + `test_ladder_*.das` — fixtures and the three dastest suites
  (store, config, server; the server suite owns reserved port 19015).

The anatomy mirrors `utils/internal/dasweb-playground`.

### 1.1 Routes

Public (proxied by `caddy.snippet`): `GET /api/versions`, `GET /api/runs[?version=N]`,
`GET /api/submission/:id` (the verbatim receipt), `GET /api/sidecars[?version=N]` (the
browse listing; absent/0 version = all), `GET /api/sidecars?version=N&box=<encoded>` (the
lookup ladder — a `box` switches modes and then version is required), `GET /api/sidecar/:sha`
(download), `POST /api/submit/records`, `POST /api/submit/sidecar`. Loopback-only:
`GET /admin/` (the operator page) + `GET /admin/sidecars` (its listing, gate state included),
`POST /admin/sidecar/verify` / `POST /admin/sidecar/delete`, `POST /admin/import-sidecar`
(plant official — stored verified; re-plant promotes), `POST /admin/import-official`,
`POST /admin/submit` (the gate flip), `POST /shutdown`, plus `GET /healthz` for the watchdog.
Operator callers pass a three-layer gate: a loopback peer, a loopback `Host` authority, and
same-origin-or-header-less. The `Host` and `Origin` layers together are the CSRF guard for
the `/admin/` page over an ssh tunnel — same-origin refuses a cross-origin fetch, and the
loopback-`Host` requirement refuses DNS rebinding (a rebound hostname would satisfy an
Origin-equals-Host check but still send its own non-loopback `Host`). The `box` query value
must be percent-encoded — box strings carry `|`, `,` and spaces.

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
  `dasllama_exchange_schema`'s `parse_sidecar_info` and drive the lookup ladder: exact box → same
  platform/arch/cpu → same platform/arch, verified first within a tier, newest first.

Validation is `dasllama/dasllama_exchange_schema` (required through the registered package
name, re-exported `public`): submission grade for community uploads, shape grade for official imports (official
history predates `DASLLAMA_VERSION`). `Source` and `Verified` are always stamped by the store;
nothing submitter-supplied reaches those columns. Sidecar submissions are privacy-stripped
server-side before hashing and storage (`exchange_strip_private` — `provenance.binary` and any
path-shaped value; the submitting client runs the same strip, so a clean upload round-trips
byte-identical and dedups).

## 3. Engine-free rule

The service builds and runs without dasLLAMA, dasLLVM, or any model machinery — dasweb-1 has
no GPU and no reason to compile an inference engine. `dasllama_exchange_schema` carries the same rule
on its side (`modules/dasLLAMA/REVIEW.md`).

## 4. Running the tests

```
bin/Release/daslang.exe dastest/dastest.das -- --test utils/internal/dasllama-ladder/test_ladder_store.das
```

Plain dastest — the same lane `extended_checks` runs the dasweb-playground suites in.

## 5. The client side

The consuming half lives with dasLLAMA: `modules/dasLLAMA/dasllama/dasllama_exchange.das`
(boot-time sidecar lookup/apply as llvm_tune's scope resolver, the privacy-stripped submit
rails, the control-page surface), wired into `utils/dasllama-server` (the `/exchange`
endpoints + `exchange_*` config keys) and `lcpp_bench --submit`. The follow-up ledger (partial
re-race, cross-box matrix, version diff) is in `plans/dasllama_io_site.md`.

## 6. Deploy

Mirrors dasweb-playground: a `daspkg release` bundle, supervised by `systemd → watchdog.py`,
fronted by Caddy on the `dasllama.io` vhost. `dasllama-deploy.sh` is the box-side tool, run
through the scoped sudoers drop-in (`dasllama-deploy.sudoers`) — the one privileged surface.

Build the bundle on the builder box (zen4) from the arc worktree:

```
cd ~/daScript-dasweb && git pull --ff-only origin <branch>
bin/daslang utils/daspkg/main.das -- release --root utils/internal/dasllama-ladder --out ~/dasllama_release
SHA=$(git rev-parse --short HEAD)
cd ~/dasllama_release && tar czf dasllama-ladder-$SHA.tar.gz dasllama-ladder
# scp dasllama-ladder-$SHA.tar.gz to dasweb-1:/tmp
```

On the box, in order — `provision` before `install` before `caddy` (install needs the unit
`provision` writes; `caddy` needs the `caddy.snippet` `install` stages):

```
sudo dasllama-deploy.sh provision                                  # user, dirs, systemd unit, restic snapshot line
sudo dasllama-deploy.sh install $SHA /tmp/dasllama-ladder-$SHA.tar.gz
sudo dasllama-deploy.sh caddy                                      # splice /api into the dasllama.io vhost, validate, reload
```

The unit `provision` writes is **sandboxed** (`ProtectSystem=strict`, all capabilities dropped,
loopback-only, `ReadWritePaths` limited to the data dir + release tree) — it contains a
hypothetical parser/runtime RCE to a process that can only write its own db and logs, while the
host filesystem and the other `/srv` services stay read-only. It is defence-in-depth, not a
substitute for validating what the public submit endpoints accept. `provision` is idempotent, so
re-running it after a unit change re-applies the sandbox and restarts a running service.

The board launches **read-only**: `submit_open` defaults false, so `/api/submit/*` returns 403
until an operator opens it. Toggle for a test window over the loopback tunnel with
`sudo dasllama-deploy.sh open-submit` / `close-submit`; the permanent open (after the security
audit) is `submit_open = true` in the deployed toml, or the `--submit-open` boot flag.

**Sidecar curation** has two equivalent operator surfaces, both driving the loopback admin
routes. The browser one: `ssh -L 8201:127.0.0.1:8201 dasweb-1`, then open
`http://localhost:8201/admin/` — the queue with promote/demote/delete, a plant picker, and
the gate toggle. The scripted one: `dasllama-deploy.sh sidecars` / `promote <sha>` /
`demote <sha>` / `plant <sidecar.json>` (plain curl wrappers — no root needed, they live in
the deploy script only so the operator has one tool). Planting stores the sidecar verified;
planting content that already exists promotes the existing row, so promote-by-content and
promote-by-sha are both available.

The official boards are the record stores under `modules/dasLLAMA/performance/records/`
(not in the release). The site deploy (`.github/workflows/pages.yml`, after the dasllama.io
flip) POSTs every `records/<box>.json` to `/admin/import-official` over loopback, so a merged
store refresh publishes itself — the same commit-is-publish contract the news has. Manual
paths for a box without a deploy: the deployed toml's `official_dir` (imported at every
start), or on the box
`daslang admin.das -- --db /srv/dasllama-ladder/ladder.db --op import-official --file records/<box>.json`.
