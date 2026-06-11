# ABI-break sweep — keeping external modules green

Read this when a PR changes what external module repos compile against:
public C++ API under `include/daScript/`, AST node layout (fields added,
deleted, renamed — e.g. the fixed-array rework deleting `TypeDecl::dim`),
signatures of daslib generics that externals instantiate, or the
`.das_module` / serialization surface.

## Why externals can red YOUR PR

`extended_checks` runs `daspkg install dasImgui --branch master` — it builds
the external repo's **master** against **your branch's** headers, at PR time.
So a breaking change produces a red CI step that looks unrelated
(`no member named 'dim'` inside dasImgui), and the naive fix creates a
deadlock: the daslang PR can't go green until externals are compatible, and an
externals fix that *requires* the new daslang can't merge until the daslang PR
lands.

The scope of "externals" is the [daspkg-index](https://github.com/borisbat/daspkg-index)
package list — that is the universe of repos any user (and the planned nightly
cron) builds against daslang master.

## The checklist

1. **Identify the breaking surface.** List every symbol removed, renamed, or
   re-typed. In-repo impact: `cpp_grep_usage` / `grep_usage` MCP tools.
2. **Sweep external usage.** Grep each daspkg-index package (your local
   checkouts, or GitHub code search) for those symbols — including
   field-access shapes (`->dim`, `.dim`, `dimExpr`), not just call sites.
3. **Pick the compatibility strategy** — in preference order:
   1. **Both-worlds spelling** (the `isArray()` precedent). Find a predicate or
      accessor that exists with the SAME meaning in both the old and new
      world — `TypeDecl::isArray()` meant "is a fixed array" both pre-rework
      (`(bool)dim.size()`) and post-rework (`baseType == tFixedArray`).
      Rewrite the externals to that spelling: their PRs merge FIRST,
      independently, green against both daslang versions. No feature macro,
      no lockstep, no red-master window.
   2. **Shim in daslang** — keep the old name as a forwarding alias for one
      release when no shared spelling exists.
   3. **Feature macro / version check** in the external — last resort; it
      forces lockstep merges and rots into dead branches.
4. **Land order: externals first.** Because extended_checks pulls externals'
   master, the external fix must be MERGED (not just PR'd) before the daslang
   PR's CI can go green. Merge externals → re-run the daslang PR's
   extended_checks → merge daslang.
5. **Verify locally before pushing** — the external-module junction pattern
   (`skills/external_module_debugging.md`), rebuild order **daslang first,
   then each module**. Stale-DLL trap: a `.shared_module` built against the
   old daslang fails to load *silently* and surfaces as a misleading
   `error[20605]: missing prerequisite` — after rebuilding daslang, delete the
   external module's `_build/` and rebuild before trusting any 20605.
6. **Post-merge sweep.** Build every remaining daspkg-index package against
   the new master and fix drift (unrelated rot surfaces here too — budget for
   it). The nightly index cron (`COVERAGE_GAP.md` Stage 4) will turn this
   into a standing signal.
