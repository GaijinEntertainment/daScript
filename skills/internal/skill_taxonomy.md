# Skill taxonomy — who reads what lives where (repo-only)

Read this before adding, moving, or shipping any skill, and before any skills cleanup.

## The folder is the shipping decision

- `skills/` (root) — ships with the SDK: task and module skills an SDK reader can act on.
- `skills/daslang/` — ships with the SDK and stands alone for third-party agents: the
  language, runtime, and stdlib reference. SDK-free: no repo paths, no `bin/`, no MCP,
  no CI, no lint rule IDs.
- `skills/internal/` — never ships: repo workflow, CI, review machinery, compiler
  internals.

There is no shipping list. The gate (`ci/check_shipped_skills.py`) enforces the split:
no shipped file links into `internal/`, no unmarked repo-only path in a shipped file
(the `repo-only` line/heading marker is the escape — `skills/internal/writing_skills.md`),
`daslang/` purity.

## One fact, one home

Language, runtime, and stdlib truth lives in `skills/daslang/`; house policy (lint
bindings, repo conventions) in `CLAUDE.md`; a workflow in its skill. A fact told in
full in two homes is a defect: keep the truest telling — the newer probe wins — and
point from the other or delete it. A one-line digest plus a pointer to the home is
routing, not a second telling.

## Rules for any move or merge

0. **Shorten and simplify, a lot.** Every move is a rewrite downward; never add a claim
   that carries no probe.
1. `probe-verified <date>` tags travel with their claims.
2. Every link to a moved file is retargeted in the same change.
3. A stray repo-only line in a shipped file takes the `repo-only` marker; a file whose
   repo-only content is more than stray lines splits (`writing_tests.md` /
   `tests_in_repo.md` is the pattern).
4. History encountered in a move dies or goes to `/history`.
5. In doubt — audience, contested duplication, a claim that looks wrong — ask the user
   directly and leave the file working.
