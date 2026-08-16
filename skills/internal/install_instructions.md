# Maintaining Install AI Instructions

Read this before updating `install/CLAUDE.md` or changing what the SDK ships for AI
assistants. The shipping decision itself is the folder (`skills/` root and
`skills/daslang/` ship, `skills/internal/` never does) — that law and the move/split
rules live in `skill_taxonomy.md`; the content rules a shipped file must satisfy (and
the `repo-only` marker mechanics) live in `writing_skills.md`. This file carries only
what neither does: the SDK-side registry and the sync duties between the two CLAUDE.md
heads.

## What ships where

| Source (repo) | Installed to | Purpose |
|---|---|---|
| `install/CLAUDE.md` | `CLAUDE.md` (root) | Main AI instructions for SDK users |
| `skills/*.md` + `skills/daslang/` | `skills/` | Shipped skills + the language bundle (minus `daslang/README.md`) |
| `skills/daslang/` (again) | `.claude/skills/daslang` | Auto-pickup for agent sessions started at the SDK root |
| `.claude/agents/dragon.md` | `.claude/agents/` | The rule-document auditor |
| `REVIEW_COMMON.md` | `REVIEW_COMMON.md` (root) | The review-checklist constitution `review_md.md` routes to |
| `GETTING_STARTED.md` | `GETTING_STARTED.md` (root) | First-run walkthrough |
| `utils/mcp/`, `utils/lsp/` | same paths | MCP / LSP servers (the `.claude/skills/daslang-lsp` manifest auto-loads the LSP) |

The install rules live in the root `CMakeLists.txt` (search for "the FOLDER is the
shipping decision"); `ci/check_shipped_skills.py <bundle>` is the per-PR content gate,
run from `ci/smoke_test_bundle.sh`.

## Registry duties

- A skill moving between root and `internal/` changes what ships — move the file, then
  fix BOTH CLAUDE.md tables: the top-level one always has a row; `install/CLAUDE.md`
  has a row exactly for shipped root skills. The gate fails the bundle when a shipped
  skill has no `install/CLAUDE.md` row.
- The two CLAUDE.md heads share the language half by construction (the fails-silently
  digest, the idiom table, the bundle pointer) — an edit to that half lands in BOTH
  files, same wording. Repo-dev content (build, CI, PR workflow) goes to the top-level
  head only.
- Skill content edits touch `skills/<name>.md` only; shipping is automatic.

## Verification

1. Install: `cmake --install build --config Release --prefix <prefix>`
2. `<prefix>/CLAUDE.md` reads as the SDK-facing head; `<prefix>/skills/` has no
   `internal/`, and `<prefix>/skills/daslang/SKILL.md` + `references/` exist.
3. `python3 ci/check_shipped_skills.py <prefix>` exits 0.
