# Archiving Completed-Arc Docs

When an arc ships, its planning/audit/findings docs move under `/history` so live
directories carry only operative docs. Read this before archiving any doc, and when a
sweep turns up stale plan docs.

## What archives, what stays

- **ARCHIVE:** plans, audits, findings, measurement records, session notes, research
  briefs, and design mockups of a **shipped** arc — anything whose remaining value is the
  record ("why is it built this way"), not operation.
- **STAY:** docs cited as *current* spec / recipe / contract by shipped code, tests,
  tutorials, skills, or docs — living contracts (e.g.
  `modules/dasSQLITE/PROVIDER_CONTRACT.md`), durable records that self-describe as
  canonical (`MASTERPLAN.md`), README-indexed operative runbooks
  (`tune_for_this_box.md`), active-arc plans, and results ledgers (`results.md`).
- **The test, per inbound reference:** is it *operative* (a reader needs the doc to act
  now) or *historical context* (a comment narrating provenance)? Only docs whose
  remaining references are historical context get archived.

## Process

1. Confirm the arc is complete — shipped PRs, and no active plan or backlog uses the doc
   as a working plan.
2. `git grep -n "<basename>"` for every candidate; classify each reference by the test
   above.
3. `git mv` into `history/<area>/`, mirroring the origin area (`compiler/`, `ci/`,
   `dasSQLITE/`, `dasSpirv/`, `dasLLAMA/`, `linq_fold/`, `examples/<name>/`, `tests/`);
   create new area folders as needed. Companion artifacts that exist only for the doc set
   (design mockups, figures) move with it so their relative links survive.
4. Update every **path-qualified** reference to the new `history/...` path. Bare-name
   mentions in code comments (e.g. `FIXED_ARRAY_REWORK.md`) may stay — they resolve by
   search plus the ledger.
5. Fix the moved docs' own relative links that point back at staying files (e.g. a moved
   doc's `results.md` link becomes `../../benchmarks/sql/results.md`).
6. If the area keeps a living index doc (`MASTERPLAN.md`, a module `README.md`, a staying
   contract doc), add a short note **in that doc**: what was archived and where.
7. Append one line per doc to the **Archive log** at the bottom of `history/README.md`:
   `- YYYY-MM-DD \`old/path\` → \`history/new/path\` — one-line hook`. The ledger is how
   archived docs stay findable.
8. Verify: `git grep` the old paths — zero hits outside `history/`.
