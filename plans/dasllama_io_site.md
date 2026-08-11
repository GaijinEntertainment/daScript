# dasllama.io — site + ladder/sidecar service

Arc plan. Design brief lives in the Claude Design forge (`consumers/dasllama-io/BRIEF.md`,
project "daslang.io Forge"); Boris reviews the design there. This doc carries the build plan
and the decisions that shape it. Scope: the site and the service. Tune-framework work
(partial re-race) is ledgered below as follow-up, NOT this arc.

## What this is

dasllama.io stops mirroring daslang.io and becomes dasLLAMA's working site:

- **home** — news-first (news IS the page, unlike daslang.io); identity strip, how-to-get,
  Atom feed. News pipeline = the daslang.io `_news/*.md` shape, own feed.
- **ladder** — ALL measurements (official + community), no promo. daslang.io/dasllama.html
  stays official-only promo. Rows = METHODOLOGY.md bench records verbatim; community rows
  may be das-only (no llama.cpp ref raced) — absolute numbers stand alone, empty ratio cell
  reads "not raced", never implied parity. Version filter, latest default, "all" available.
- **sidecars** — the point of the whole thing: an exchange of per-box tune sidecars so a
  tool downloads one for its (version × architecture) instead of paying ~20 quiet-box
  minutes. Finder + per-sidecar kernel-win viewer + download. Measurements are the
  community bonus riding along with sidecar submissions.

Hosting: dasweb-1 (Caddy vhost + `/srv/dasllama.io` already provisioned). Site source:
`site-dasllama/` beside `site/`, second rsync step in pages.yml. Service:
`dasllama-ladder` on 127.0.0.1:8201 behind Caddy, dasHV + dasSQLITE, same anatomy as
`utils/dasweb-playground` (config/store/server/launcher split, in-dir tests, CODEREVIEW.md
from the first commit, watchdog contract, ndjson logging).

## Versioning decisions (Boris, 2026-08-10)

- **dasLLAMA gets its own version**, decoupled from daslang's version AND from
  `LLVM_JIT_CODEGEN_VERSION`. **Any kernel work bumps it.** The rule is enforced in
  `modules/dasLLAMA/CODEREVIEW.md`; the constant (`DASLLAMA_VERSION`) lands in the same
  commit as the rule. Bench records and sidecar provenance carry it.
- v1 sidecar validity keys on (dasLLAMA version × box). Because any kernel work bumps the
  version, "same version" ⇒ same kernel roster — the version IS the roster key in v1.
- A new kernel does NOT 100% invalidate old sidecars — it means one family needs re-racing
  locally. Supporting that honestly is the partial re-race follow-up (below); v1 states
  version compatibility plainly and offers full tune as the local action.

## Site content rules (Boris, 2026-08-10)

- **Home order: get-it first, then news, then the ladder teaser.** Get-it is git links only
  for now (daslang repo + the modules/dasLLAMA folder); package managers later. No `.das`
  integration card on the home page.
- **Only real news.** Entries state shipped events; the seed entry is
  `_news/2026-08-10-dasllama-io-is-live.md`.
- **No invented shell commands or code samples anywhere on either site** — every command
  shown must run verbatim and produce the shown result. Enforced by `site/CODEREVIEW.md`
  and `site-dasllama/CODEREVIEW.md` (template: `skills/codereview_md.md`). The sidecars-page
  startup transcript is comment-marked as the client-integration contract and gets verified
  verbatim against the real dasllama-server before ship.
- **Ladder table fix rides the data hookup (slice 5):** the dasllama.html tables on
  daslang.io show a vertical scrollbar — fix it in the shared `dasllama-table.css` AND in
  dasllama.html's inline copy (or fold the adoption forward if it stays trivial).

## Trust model (settled)

Anonymous submissions; schema validation; per-IP rate limit (Caddy body cap + service
limiter, playground pattern); everything enters **unverified**; `verified` is set
server-side only (loopback admin, promote pattern); delete-by-id lever. Client startup UX:
"verified sidecar exists / unverified exists / nothing — tune for your exact box anyway?".
The sidecar's own noise-gate provenance (`noise: ok`, cv floor) is surfaced and is the
free garbage auto-flag. Backups (litestream/restic) live BEFORE the submit endpoint goes
public — the box stops being CI-reproducible the moment it holds community data.

Build-time gate before serving downloads: verify malicious-sidecar blast radius — kernel
entries are selections from the binary's own fixed variant set and runtime knobs are
clamped scalars, so a hostile sidecar must degrade to slow-at-worst, never incorrect.
Validate on submit AND client-side on consume (unknown kernel name → fallback, out-of-range
knob → clamp).

## Build slices (each lands tested; one PR per the no-small-PRs maxim)

1. **`DASLLAMA_VERSION`** + CODEREVIEW.md bump rule + records/sidecar provenance carry it.
2. **Schema module** — bench-record + sidecar parse/validate (shared by service and
   submitting tools), tests.
3. **Ladder store** — sqlite: records, sidecars (content-addressed), verified flag,
   version/box indexes; migrations; tests.
4. **HTTP surface** — public: list/filter records, sidecar lookup by (version, box) with
   exact-box → same-CPU → same-arch fallback, sidecar download, submit (records+sidecar);
   loopback admin: verify/delete. caddy.snippet routes + caps. Tests via with_test_server.
5. **Site** — `site-dasllama/` three pages + news pipeline + feed + OG/sitemap/robots;
   pages.yml rsync step; playwright coverage (rig pattern from the playground).
6. **Client integration + watchdog** — per the settled spec below.
7. **Deploy** — service to dasweb-1, Caddy vhost flip from mirror to real site, backups
   wired, live verification.

## Slice 6 spec — exchange client + watchdog (Boris, 2026-08-11)

**No console prompt anywhere; one code path.** Config is persistence only — the casual
user (package-manager install, clicked `.sh`, tray icon in the corner) never sees toml.
The layering: defaults that need no decision → balloon when a decision exists → control
page where acting on it is one click → config remembers the choice (e.g. the "always
share after a tune" checkbox writes `exchange.submit = always` through the existing
POST /config rail).

**Policy knobs** (dasllama-server config; exchange URL baked in, on by default):

- `exchange.accept = verified | any | off`, default `verified`. At untuned boot a
  verified hit → download, client-validate (schema; `DASLLAMA_VERSION` — the load-bearing
  staleness gate, since the mtime rail reads any download as fresh; box match; unknown
  kernel name → fallback, out-of-range knob → clamp), write the sidecar, print the
  marker, **exit 3** — riding the existing tune-bootstrap rail. Lookup failure is NEVER
  fatal: fall through to the local tune.
- `exchange.submit = ask | always | never`, default `ask`. `ask` surfaces as a
  pending-submit offer via watchdog balloon + control page, never a console question.

**Unverified sidecars never auto-apply and are never a boot question** (after a
successful own tune they have negative value). They surface exactly three ways: a manual
pick on the control page's exchange card; a proactive offer after repeated noise-gate
aborts (the noisy-box case); and DURING a tune via the live tray menu — where doing
nothing means the tune just finishes.

**Watchdog changes** (`utils/watchdog/watchdog.py`; backoff/health/crash logic UNCHANGED):

- The tray menu becomes a pure function of STATE — pystray re-evaluates callables on
  menu open, `icon.update_menu()` on transitions. Items: "Use available sidecar instead
  (stops tuning)" [visible: tune in flight AND an offer exists], "Stop tuning, run
  untuned" [tune in flight — untuned serving is legitimate], "Share this box's tune…"
  [pending submit offer → opens the control page at the exchange card].
- `@sidecar` structured events server→watchdog (same contract as `@tune`), folded into
  STATE: offer / applied / pending_submit. Balloons stay announce-only (a PowerShell
  balloon can't carry actions).
- A **tune-control FILE** watchdog→server (the stop-file/env pattern): during a tune
  there is NO HTTP (listening = stage 7, tuning = stage 3). The tuner polls it at kernel
  boundaries (the `@tune step` points), so an interrupt takes effect within one kernel
  and never mid-measurement.
- Second exit-3 immediate-relaunch marker: `exchange: sidecar applied, restart to apply
  the winners`.
- `exchange_lookup` startup stage.

**Control page** gets the exchange card: current sidecar (sha / source / verified /
tier), the matches list, [apply this one] / [retune] / [submit mine], the always-share
checkbox — all on the existing POST /config + exit-4 restart rails.

**Privacy:** strip `binary` (a full local path with the username in it) and any other
path-shaped provenance from submitted sidecars client-side; the ladder also
refuses/strips server-side. Official records already publish cmd paths — Boris's call
there, out of scope here.

**Also in this slice:** lcpp_bench stamped record submission; a client-side `url_encode`
(dasHV has none); the sidecars.html startup transcript re-verified verbatim against the
real server — the current mock shows an interactive `[use/tune/skip]` prompt, which this
spec removes, so the page gets rewritten to the real output.

## Follow-up ledger (documented, deliberately not this arc)

- **Partial re-race** (tune framework, dasLLVM + dasllama tuner): per-family variant-set
  fingerprint (perm suffixes + gen args); freshness states fresh / stale (unraced
  challenger exists; winner still usable) / broken (winner gone; fallback chain, untuned);
  race only not-fresh families with their own noise bracket; provenance `mode: partial` +
  family list; upsert merge already exists (`tune_manifest_set`). Needs: per-release
  **roster manifest** (family → fingerprint + variants + fallback chain, derivable via the
  `tune_scopes_status`-style AST walk, CI-published) so the service computes freshness of
  every sidecar against every version statically. Note: the mtime staleness rail reads a
  downloaded sidecar as fresh (download time > binary mtime) — the roster mechanism is
  what makes exchange sidecars honest.
- **Codegen soft-staleness**: `LLVM_JIT_CODEGEN_VERSION` bump shifts winners without
  changing rosters → mark affected sidecars "re-race recommended", never invalid.
- **Hardware-identity normalization (the "which boxes are the same hardware" tier)**: the
  lookup ladder's middle tier matches on the raw cpu identity field, whose grain differs by
  platform — Windows `PROCESSOR_IDENTIFIER` ("AMD64 Family 23 Model 49 Stepping 0") groups a
  whole µarch (every Zen 2 Threadripper/EPYC/Ryzen of that model), while darwin/linux brand
  strings are per-SKU ("Apple M1 Max" ≠ "Apple M1 Pro"; "3970X" ≠ "3960X") — so µarch
  siblings never match on those platforms. Curated names like "zen2" exist only as
  `DASLLAMA_BOX` records tags, not derived from hardware. Fix: a curated cpu-identity → µarch
  map (schema- or ladder-side) adding a "same µarch" tier between same-cpu and same-arch;
  possibly a CPU-features axis (the `requires=`/`cpu_supports` vocabulary) as the honest
  compatibility floor beneath it.
- **Cross-box winner matrix**: family × architecture → winner, aggregated server-side;
  the grid-pruning view (a variant that wins nowhere is a deletion candidate).
- **Version diff view**: roster changes + winner flips between adjacent versions.
- **daslang.io/dasllama.html adopts the shared table component** once the ladder ships it.
