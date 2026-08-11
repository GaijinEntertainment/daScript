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
6. **Client integration** — dasllama-server startup sidecar lookup + consent-to-submit;
   lcpp_bench/gen_bench_records stamped submission.
7. **Deploy** — service to dasweb-1, Caddy vhost flip from mirror to real site, backups
   wired, live verification.

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
- **Cross-box winner matrix**: family × architecture → winner, aggregated server-side;
  the grid-pruning view (a variant that wins nowhere is a deletion candidate).
- **Version diff view**: roster changes + winner flips between adjacent versions.
- **daslang.io/dasllama.html adopts the shared table component** once the ladder ships it.
