---
slug: when-to-consult-mouse
title: When should I consult blind-mouse?
created: 2026-05-08
last_verified: 2026-05-08
links: []
---

Reach for `mouse__ask` for **how-do-I** / **what's-the-pattern-for** / **why-does-X-behave-Y** shaped questions — the long-tail "discovered facts" that don't live in `skills/CLAUDE.md` and aren't direct symbol lookups.

Do NOT use the mouse for:
- Symbol definitions / call sites — use the daslang MCP (`find_symbol`, `grep_usage`, `find_references`).
- Categorical conventions (gen2 syntax, formatting, build flags) — these belong in `skills/*.md` or CLAUDE.md.
- Project state (in-progress work, branch status, who's doing what) — use git, the issue tracker, or session memory.

If a question genuinely doesn't fit any of those slots, ask the mouse first. If the answer is missing, do the research, then `mouse__add` it back so the next session doesn't redo the work.

## Questions
- When should I consult blind-mouse?
- mouse vs grep
- when to use mouse
- when to ask mouse vs MCP vs skills
