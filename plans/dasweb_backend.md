# dasweb backend — samples store, curated unification, wasm pipeline

Companion to `E:\DASWEB\plan.md` (infrastructure; boxes are live) — this doc covers the software.
Branch: `bbatkin/dasweb-arc`, worktree `D:\Work\daScript-dasweb`.

State when written (2026-08-04): daslang.io + dasllama.io serve from the web VPS (`dasweb-1`,
89.167.63.131, Debian 13) behind Caddy; every master push mirrors the CI-built site via
`pages.yml`; COOP/COEP headers already serve the threaded playground/examples correctly.
Compute box (`zen4`, 65.108.238.44, Debian 12) is Boris's profiling rig for now; phase 3 claims
it. GH Pages remains deploy-mirrored as catastrophe insurance only.

Boris's phasing (agreed):

1. `/s/<hash>` for user samples, live. Everything else unchanged.
2. Curated samples through the same store; wasm ("jit") button disabled. Web box fully unified.
3. wasm build infrastructure on the compute box; "building" UI; rename jit → wasm.
   Split: 3a machinery first, 3b UX.

Phase 0 (backups) is a parallel track, NOT a gate (Boris 2026-08-04): pre-backup, a lost
samples.db strands only freshly-minted links — no worse than the third-party shortener chain
it replaces, which can go stale any second. Land it when the Storage Box lands (procedural,
Boris chasing IT); litestream bolts onto the live service with zero migration. One cheap
honesty rule until then: the share UI says "share link", not "permanent link" — we don't
advertise permanence before backups can honor it.

---

## Architecture invariants (all phases)

- Caddy is the only public listener. Services bind `127.0.0.1` only, via `set_bind_host`
  (call after `init(port)`, before `start()` — dasHerd watcher pattern, `utils/dasHerd/watcher/main.das:209`).
- Port registry = the Caddyfile. Convention: **81xx daslang.io services, 82xx dasllama.io**.
  `dasweb-playground` = **8101**. (`dasllama-ladder` = 8201, later, own plan.)
- One process per service; merge only if reality forces it.
- Every service follows the watchdog contract (below). systemd keeps the *watchdog* alive;
  the watchdog keeps the *service* alive.
- Sources are the archive (SQLite, litestream-replicated, precious). wasm artifacts are a cache
  (files on disk, regenerable from source hash × toolchain, evictable — phase 3).

## Service anatomy (the reusable shape — dasweb-playground is the first instance)

Reference: `examples/hv/ws_chat_server.das` ("the canonical daslang server shape") +
`utils/dasllama-server/` conventions + `examples/telegram/dictation/.das_package` (the deployed-
service manifest template).

```
daslib/sha_256.das       real SHA-256 (bytes -> hex) — new daslib module, see Hashing
tests/daslib/test_sha_256.das  NIST vectors

utils/dasweb-playground/
  main.das               launcher: clargs + toml config, logger, lifecycle, GC loop
  samples_server.das     module: SamplesServer : HvWebServer — route table + handlers only
  samples_store.das      module: SQLite + hashing + validation, ZERO HTTP (unit-testable seam,
                         like llm_scheduler.das)
  test_samples_store.das no network; store roundtrips, migration tests
  test_samples_server.das end-to-end HTTP on port 19011 (19001..19010 are taken)
  .das_package           release manifest
  watchdog.json          { name, health_url, shutdown_url }
  dasweb-playground.toml    starter config (shipped release_include_if_missing)
  README.md
```

Key shape decisions, each grounded in what the repo already does:

- **Options**: `options gen2`, `options gc`, `options persistent_heap`, `options stack = 262144`
  (plain handlers are fine at 16K per skills/dashv.md, but SQL+JSON chains are not "plain";
  262144 is cheap insurance, half of dasllama-server's).
- **Lifecycle**: module-global state only (`var g_server : SamplesServer?` etc.) — JIT GC cannot
  see stack locals as roots; `[export] init/update/shutdown` + standalone `main` looping
  `update(); maybe_collect_gc()`. Stock `maybe_collect_gc` from `require live/live_gc` to start;
  graduate to dasllama-server's rate-limited/logged variant only if GC noise shows up.
- **Tick**: `g_server->tick()` + `sleep(2u)` idle pacer. Handlers are serialized by design
  (single tick thread) — fine for this workload, do not assume concurrency.
- **Route lambdas**: retained with `push` semantics (never `emplace`) — GC-visibility rule from
  skills/dashv.md.
- **Tests — the session rules (Boris 2026-08-04):**
  1. **The server gets tests for ALL functionality from the get-go** — every route, every
     store operation, every config/limit behavior has a dastest test before it's called done.
     We have a test framework; we are not afraid to use it.
  2. **Every bug found during the arc gets a regression test**, in the same change as the fix.
  3. **Tests live in-dir** — `utils/dasweb-playground/test_*.das`, like other utils
     (hyphenated dir ⇒ bare-name sibling `require`, the dasllama-server convention). NOT under
     global `tests/`. (Exception already landed: `daslib/sha_256` is a daslib module, so its
     tests are correctly global at `tests/daslib/test_sha_256.das`.)
  `with_test_server` from `tests/dasHV/_dashv_test_common.das` does NOT fit (needs per-thread
  DB open + `set_bind_host`) — write the local variant of the harness whose thread lambda
  opens the store first. Server on `new_thread` = own context ⇒ all setup inside the thread.

### Config

`[CommandLineArgs] struct ServerArgs` (clargs) + TOML merge, the dasllama-server precedence:
defaults < toml < CLI (drop the `authoritative` flip — unneeded here). Auto-discover
`dasweb-playground.toml` in cwd then beside the module. Keys, phase 1:

```
port = 8101
data_dir = "/srv/dasweb-playground"       # samples.db + blobs/ (blobs used in phase 3)
max_source_bytes = 262144              # dasHV exposes no body cap — enforced in handler
```

### Logging + watchdog integration (the "watcher" story)

**Design rule (Boris): the server stores just about everything it does — any server should.**
"If it happens, it's in the log" — the profiling/tuning sessions repeatedly went inconclusive
for lack of checks/canaries/logs; this service does not repeat that. Concretely, ndjson lines
for: every request (method, path, status, ms, client ip, bytes in/out); every insert / dedup-hit
/ import / promote / reimport; every build-job state transition with timings (phase 3); every
GC cycle; canary/ABI-check outcomes; and a startup banner dumping the full effective config
WITH provenance (default/toml/cli — the dasllama-server pattern). Logs are cheap; archaeology
is not.

- `logger_init_tee("dasweb-playground")` in init → ndjson to `logs/dasweb-playground.log` + stdout.
  No rotation in daslang; rotation is the watchdog's (20 MB × 5 — raise if request volume
  makes the window too short; the request log must cover at least days, not hours).
- Supervisor = the shared `utils/watchdog/watchdog.py` (same one dasllama-server and the
  dictation bot ship). Contract the service honors:
  - exit **0** = intentional shutdown (watchdog stops), **4** = config-restart request,
    anything else = crash → notify + bounded-backoff restart + crash bundle.
    (**3** is the tune-restart code — unused here, no `[tune]` kernels.)
  - `GET /healthz` (cheap 200) — watchdog polls every 5 s, logs transitions only.
  - `POST /shutdown` — graceful stop; flips the service's own context flag.
- `watchdog.json`: `{ "name": "dasweb-playground", "health_url": "http://127.0.0.1:8101/healthz",
  "shutdown_url": "http://127.0.0.1:8101/shutdown" }`.
- systemd unit `dasweb-playground.service`: `ExecStart=python3 watchdog.py`, `WorkingDirectory=`
  the release bundle, `Restart=on-failure` (guards the watchdog itself), `User=dasweb`.
  Watchdog does the child restarts, stages, crash bundles; journald gets the watchdog's stdout.

### Release + deploy (the daspkg story)

This app has no `[tune]`/`[llvm_code]` kernels ⇒ it bakes to a standalone exe cleanly
(dasllama-server is headed the same way — its baked-exe bringup exists in parallel work, not
yet merged; the anatomy here assumes baked-exe as the normal service form). Deploy as a
**baked `daspkg release` bundle, built on the web box** (no cross-compile exists; `-exe` needs
dasLLVM + a `c++` driver on PATH — the published Linux SDK bundle carries dasLLVM and can run
`daspkg release` itself):

```
# on dasweb-1, one-time: apt install g++; unzip daslang-bundle-linux-x86_64.zip to /opt/daslang
# deploy script (checked in as utils/dasweb-playground/deploy.sh):
git -C /srv/src/daScript fetch && git checkout <ref>
/opt/daslang/bin/daslang utils/daspkg/main.das -- release \
    --root utils/dasweb-playground --out /srv/apps/dasweb-playground/releases/<sha>
ln -sfn releases/<sha> /srv/apps/dasweb-playground/current && systemctl restart dasweb-playground
```

- `.das_package`: `release_main("main.das")`, `release_name("dasweb-playground")`,
  `release_include_if_missing("dasweb-playground.toml")` (preserve deployed edits),
  `release_include_from("utils/watchdog/watchdog.py")`, `release_include("watchdog.json")`.
- Bundle exe is named `dasweb-playground.exe` even on Linux (daspkg convention; CI's sequence
  smoke test relies on the same).
- **Launch with cwd = bundle dir** — a relocated exe's `get_das_root()` degrades to cwd
  (`src/misc/sysos.cpp:666-693`), which is what makes `logs/`, module resolution, and
  `get_this_module_dir()` land right. systemd `WorkingDirectory=` handles it.
- **No top-level `let` calling Context-allocating builtins** (`get_this_module_dir()` etc.) —
  known `-exe` ASLR bake bug; call from functions only.
- The build costs seconds-to-a-minute on the VPS (JIT codegen + link of a small server —
  NOT a C++ build); zen4 is not needed for deploys.

CPU-target note: with no `[llvm_code]` kernels the `-exe` targets generic x86-64 ⇒ the bundle
is portable across our boxes by construction.

---

## Phase 0 — backups (parallel track; lands when the Storage Box does)

1. **Storage Box credentials from IT** (handover checklist item — still owed; Boris chasing.
   Procedural — the Hetzner account is IT's; no self-serve alternative).
2. **litestream** on dasweb-1 replicating `samples.db` continuously (SFTP target on the Storage
   Box if litestream's SFTP replica works as documented — verify at implementation; fallback:
   file replica to a local dir covered by restic).
3. **restic nightly** systemd timer (`/etc`, `/home`, `/srv`) per the original IT plan — also
   still missing on both boxes.
4. Service user `dasweb` (no sudo), owns `/srv/dasweb-playground` + `/srv/apps/dasweb-playground`.
5. `apt install g++ python3` on dasweb-1 (release builds + watchdog).
6. Restore drill: prove `samples.db` restores from litestream + a blob restores from restic
   as part of landing this phase. A backup that never restored is a rumor. (Once this phase
   lands, the share UI may say "permanent".)

## Phase 1 — user samples: `/s/<hash>` live

### Storage

Source text lives IN SQLite (KB-scale; one litestream stream covers everything; the `blobs/`
dir stays empty until phase-3 wasm artifacts, which are MB-scale and stay files).

```das
[sql_table(name = "samples")]
struct Sample {
    @sql_primary_key Hash : string        // full lowercase hex sha256 of the source bytes
    Source : string                       // the .das text, verbatim
    Size : int
    CreatedAt : int64                     // unix seconds
    ClientIp : string                     // from X-Forwarded-For (Caddy is the only caller)
    Listed : bool = false                 // phase 2: curated/promoted samples
    Title : string = ""                   // phase 2
    Origin : string = "user"              // "user" | "curated"
}
```

Migrations via `sqlite/sqlite_migrate` from day one (`[sql_migration(version=1)]` raw-SQL body —
frozen once shipped). Worked precedent in-tree: the dictation bot
(`examples/telegram/dictation/cadmus_history.das`) — follow its shape, don't invent. Startup =
`with_latest_sqlite(path)` equivalent bound to the server thread (SqlRunner is context-bound —
open INSIDE the server thread).
Dedup is free: same source ⇒ same hash ⇒ INSERT OR IGNORE semantics (`insert` on PK conflict —
treat conflict as success, return the hash).

### Hashing

**New daslib module `daslib/sha_256`** (underscore — a require path cannot contain a hyphen).
Why a real crypto hash and not builtin 64-bit `hash`: content-addressed PUBLIC urls need
collision resistance — with a computable-collision hash an attacker pre-inserts a malicious
source colliding with a commonly-shared snippet, and every honest share of the real snippet
dedups onto the attacker's row (link poisoning). SHA-256 closes the class; KB inputs cost
microseconds, and a pure-daslang impl also runs in wasm (client-side hashing later, if wanted).

Port the compression core from the playground benchmark (`web/examples/ui/samples/examples/
sha256.das`) + add standards padding/length. **The benchmark itself is untouched** — its
non-conformance is deliberate (apples-to-apples with the other dasProfile language ports).
API: `sha256_hex(data : array<uint8>) : string` + string overload. NIST vectors
(empty, "abc", million-a's) in `tests/daslib/`. daslib addition ⇒ das2rst group + doc
registration per `skills/make_pr.md` step 4. No OS shell-outs in the request path (the
dasLLAMA `sha256sum` precedent is for multi-GB model files, wrong tool here).

### HTTP API (dasHV, port 8101, loopback)

```
POST /api/samples          body = raw .das text (text/plain; curl-friendly)
                           → 200 {"hash": "...", "url": "https://daslang.io/s/<hash>"}
                           → 413 over max_source_bytes (checked in handler — dasHV has no cap)
                           → 400 empty/non-utf8
GET  /api/samples/<hash>   → 200 raw source, text/plain; charset=utf-8; immutable cache headers
                           → 404 unknown
GET  /s/<hash>             → 302 /playground/index.html?s=<hash>  (permalink humans share)
GET  /healthz              → 200 "ok" (watchdog + Caddy probes)
POST /shutdown             → graceful stop (never routed by Caddy — loopback only)
```

Content-addressed responses are immutable ⇒ `Cache-Control: public, max-age=31536000, immutable`
on `GET /api/samples/<hash>`. Abuse guard, phase 1 minimal: size cap + per-IP hourly insert
ceiling — **both toml-configurable** (`max_inserts_per_ip_hour`, one SQL count on ClientIp,
429 over the ceiling). No auth, no captcha — samples are worthless to spam until listed.
(Phase-3 note, Boris: build requests need no extra limiter — the queue plus baking time IS the
natural throttle; the insert ceiling exists only because inserts are the cheap unqueued path.)

### Caddy routing (adds to the daslang.io vhost)

```
handle /s/* {
    reverse_proxy 127.0.0.1:8101
}
handle /api/samples* {
    reverse_proxy 127.0.0.1:8101
}
# NOTE: /api/samples* (no slash) — the slash form would not match the exact
# path POST /api/samples that share minting uses. This matches the deployed
# Caddyfile. /admin/* and /shutdown are deliberately NOT routed.
```

### Playground integration (site/ change, rides the same CI deploy)

- Share button: POST current editor text to `/api/samples`, put the returned `/s/<hash>` URL in
  the clipboard/share box. Kills the zip+tinyurl third-party chain.
- Loader: on `?s=<hash>` param, fetch `/api/samples/<hash>` into the editor.
- Old share links keep working (loader keeps the legacy param path); we just stop MINTING them.

### Definition of done (live-server test, Boris's checkpoint 1)

- `curl -d @file.das https://daslang.io/api/samples` returns a URL; opening it loads the
  playground with the code; same POST twice returns the same hash.
- Kill -9 the service: watchdog restarts it with backoff; `/s/<hash>` works after restart;
  static site never blinked.
- lint/format/tests green per `skills/make_pr.md`. (Backups are phase 0, parallel — not part
  of this checkpoint.)

## Phase 2 — curated samples through the store; wasm button off

- **Importer** — the "introduce existing files" process, idempotent by construction, no ETL:
  on boot (and on a `POST /api/samples/reimport` admin nudge from loopback), scan the deployed
  curated dir (`/srv/daslang.io/current/playground/samples/**.das` — already shipped by the
  site deploy); each file: read → hash → INSERT-OR-IGNORE as Origin="curated", Listed=true,
  Title from the existing samples manifest. Unchanged file ⇒ same hash ⇒ no-op; changed file
  next deploy ⇒ new hash row and the LISTING repoints while the old row stays resolvable
  (permalinks never break). Safe to re-run any number of times.
- **Dropdown**: `GET /api/samples/listed` → `[{hash, title, category}]`; playground dropdown
  consumes it instead of the static list. Dropdown stays, exactly as today, just fed by the store.
- **Disable the "jit" button** (one flag in playground JS) — honest until phase 3 serves real
  builds.
- **Promotion** ("add one of theirs"): `utils/dasweb-playground/admin.das` — a tiny clargs CLI run
  on the box over ssh (`admin.das -- promote <hash> --title "..."` → Listed=true). No public
  admin surface, no auth added to the API.
- Checkpoint 2 (live): dropdown loads from the store; every curated entry opens + runs
  interpreted; user flow from phase 1 unchanged; wasm button absent.

## Phase 3 — wasm pipeline (sketch; detailed plan when phase 2 ships)

3a machinery first, 3b UX second. New service on the compute box (working name
`utils/dasweb-buildd`) + queue/artifact endpoints added to dasweb-playground. This phase gets its
own plan doc before implementation; the contour:

- **Queue**: table in samples.db (hash, toolchain_id, state: queued/building/done/failed,
  attempts, timings). dasweb-playground exposes loopback-free authed endpoints for the builder:
  `GET /api/build/next` (claim, bearer token from toml), `POST /api/build/result` (artifact
  upload + status). Compute box PULLS over HTTPS — nothing on the web box ever connects to it
  (trust direction preserved; box death = queue backs up, site unaffected).
- **Builder** (compute box): same service anatomy (watchdog, logger, clargs+toml); polls the
  queue; each build in a throwaway sandbox (rootless podman or nsjail: no network, CPU/mem/time
  caps, tmpfs) running `daspkg release wasm`; uploads `.wasm` (+ `.br` precompressed sidecar)
  or the compiler error text.
- **Toolchain = latest master, built in a dedicated worktree on the box** (Boris's call:
  matches what the site ships). The worktree holds daslang + shared_modules + the wasm runtime
  archives; a toolchain bump = update the worktree + rebuild, which changes `toolchain_id`
  (= that worktree's git sha) and thereby lazily invalidates the artifact cache by construction.
  Version skew is tolerable mid-bump: each `release wasm` artifact is self-contained (bundles
  its runtime), so an older artifact still runs standalone.
- **The wasm64 build config is NOT free-form — replicate CI's recipe exactly.** Authority:
  `.github/workflows/pages.yml` "Build: Daslang host" step comments + `web/CMakeLists.txt`
  (`web/README.md` documents only the wasm32 path). The load-bearing specifics:
  - Cross-compile host daslang built with **libc++ + `-DDAS_ENABLE_EXCEPTIONS=1`** (clang, not
    gcc/libstdc++): the host JIT-codegens the samples and bakes C++ handled-type layouts —
    a libstdc++/setjmp host bakes WRONG offsets (std::string 32 vs 24 B, mutex 80 vs 40 B) ⇒
    wasm heap corruption. Host==target stdlib/exception config is the whole trick that makes
    the cross-compile "close". Host ptr width == target ptr width (wasm64 ⇒ 64-bit host).
  - `web/build64` configured with `DAS_WASM_MEMORY64=ON`, `DAS_WASM_PTHREADS=ON`,
    `-sMEMORY64=1` in C/CXX flags, `DAS_HOST_DASLANG_OVERRIDE` pointing at that host build.
  - **emsdk pinned to the same version pages.yml pins** (5.0.7 at time of writing — the pin
    history in pages.yml documents why: emsdk LLVM snapshots repeatedly broke ds_parser.cpp).
  - ⇒ zen4's PROFILING daslang (`~/zen4_build.sh`, native clang-19) can never serve as the
    wasm host; the wasm worktree gets its own host build with these flags. Deliverable: a
    checked-in `build_wasm_host.sh` capturing the recipe (extracted from pages.yml, kept in
    lockstep with it) + a SETUP.md section on the box; parity gate before first serve = build
    a curated sample on zen4 and diff behavior against the CI-built artifact of the same
    (source, toolchain) pair.
- **ABI canary on every toolchain bump — `--jit-check-abi`** (`modules/dasLLVM/daslib/
  llvm_exe.das:655-693`): emits a host-vs-target layout check for EVERY handled type into the
  minted binary — bakes host size + field offsets, resolves the target's at runtime, records
  every divergence, dumps all and aborts. Builder protocol: a new `toolchain_id` goes live
  ONLY after a canary sample built with `--jit-check-abi` runs clean; result (including the
  full divergence dump on failure) goes in the log. Guard the known silent path: web/
  CMakeLists.txt WARNS-not-fails when the host compiler isn't clang ("degraded but builds") —
  `build_wasm_host.sh` hard-requires clang so that degradation cannot happen quietly on zen4.
  - The box environment is Boris's profiling rig first — he leaves a .md on the box documenting
    what lives where; the wasm toolchain install (notably **emsdk**, pinned to the same version
    pages.yml uses) must be documented alongside it and not stomp the profiling setup. Nothing
    on zen4 gets touched until Boris signals the profiling session is done.
- **Artifacts**: `blobs/<toolchain_id>/<hash>.wasm[.br]` on the web box, served static by the
  samples service (or Caddy `handle_path` straight from disk); LRU eviction, curated pinned;
  toolchain_id in the key ⇒ a release bump lazily invalidates everything, warmup job re-enqueues
  the curated set post-deploy.
- **UX**: "building… (queue position N)" state in the playground; rename jit → wasm everywhere
  user-visible; failed-build shows the compiler error.
- **CI**: keep a compile-gate job proving the curated set builds (test, publishes nothing) — a
  broken sample reds CI, not the production queue.
- **pages.yml diet (folded in here per Boris — no separate "small PR", CI makes every PR an
  hour):** (a) DELETE the per-sample wasm64 compute builds + staging + artifact-verify from
  pages.yml — orphaned since phase 2 turned the radio off; the queue replaces them. (b) Fold
  the example-game wasm64 pages (arcanoid/pacman/furier/path_tracer_lab/physarum_lab) into the
  same content-addressed build/cache rail — they change rarely, so the store cache near-always
  hits and CI stops rebuilding them; design point: a game artifact is an html+js+wasm TRIPLE,
  so the artifact cache must carry multi-file bundles, not just single .wasm. (c)
  `daslang_static` (the emcc-built interpreter runtime) explicitly STAYS in CI — it is the
  toolchain, not a sample; not touched (Boris: "amazing it actually works").
- Checkpoint 3a: curated sample builds end-to-end through the real queue from a cold cache.
  Checkpoint 3b: user-visible flow with the building indicator, on a live server.

## Pre-PR gate — security-focused code review

Before the arc PR (and again before any later PR that touches the service): run `/code-review`
over the full changed set **with an explicit security focus** — input validation, SQL injection
surface, path handling, loopback binding, secrets in logs, the Caddy route boundary. Findings
follow the normal verify-then-fix discipline; a finding class that survives review gets a
REVIEW.md rule so it cannot recur silently.

`utils/dasweb-playground/REVIEW.md` exists from the get-go (same language/structure as
`modules/dasLLAMA/REVIEW.md`) and carries the binding per-diff rules: file-placement
(one file, one rule — keeps things from going all over the place), tests-in-dir + bug⇒test,
the security criteria, logging criteria, lifecycle rules. `skills/make_pr.md` step 0a
discovers it automatically for every future PR touching this directory.

## Phase 4 — documentation ("for all the good stuff")

Continuous minimum rides each PR anyway (CI-forced): das2rst group + doc registration for
`daslib/sha_256` (and any later daslib addition), README per new tool dir. The dedicated
phase at arc end covers the rest properly:

- `daslib/sha_256` — stdlib reference page prose (beyond the generated stub).
- `utils/dasweb-playground/README.md` — run/endpoints/config/supervised-deployment, the
  dasllama-server README shape.
- Ops runbook: box layout, Caddy routes + port registry, deploy/rollback procedure, backup
  restore procedure, "where are the logs / what do they contain".
- Playground user-facing bits: share-link behavior (`/s/<hash>`), wasm build states (phase 3).
- zen4 `~/SETUP.md` wasm-toolchain section + `build_wasm_host.sh` header prose (phase 3).
- A doc/source page only if any of this graduates to public reference material — decide then
  (`skills/documentation_rst.md` + `skills/tutorial_prose.md` apply).

## Open questions (decide before/during phase 1)

1. ~~Name~~ **DECIDED (Boris 2026-08-04): `dasweb-playground`** (dir, binary, watchdog name,
   systemd unit; port 8101). `dasllama-ladder` still the working name for the leaderboard.
2. litestream SFTP → Hetzner Storage Box: verify support at implementation; fallback documented
   in Phase 0.
3. ~~Per-IP rate ceiling~~ DECIDED: configurable via toml (default 100/h, tune live).
4. ~~restic mirror of samples.db~~ **DECIDED (Boris): yes, very much so.** Mechanism: a raw
   copy of a live SQLite file is not guaranteed consistent (mid-checkpoint), so the nightly
   timer produces a proper snapshot first — `sqlite3 samples.db ".backup <data_dir>/nightly/
   samples.db"` (the online-backup API; consistent by construction) — and restic picks up the
   snapshot file with the rest of `/srv`. Litestream stays the point-in-time stream; the restic
   snapshot is the independent belt-and-braces copy.

## Non-goals / deferred

- dasllama-ladder (leaderboard) — separate plan, same service anatomy, port 8201.
- Umami/analytics changes — GoatCounter stays as-is.
- daslang.io IPv6, Cloudflare orange-cloud — infra follow-ups, unrelated.
- Any moderation/abuse tooling beyond size+rate caps — revisit when there's traffic.
- Touching the playground sha256 BENCHMARK — stays non-conforming on purpose (dasProfile
  cross-language parity).
