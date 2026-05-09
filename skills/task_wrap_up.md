# Task wrap-up: blind-mouse curation

Run this **whenever a meaningful chunk of work is done** — a feature shipped, a refactor wrapped, a multi-file change ready to commit, a session about to end. Not just before PRs. Goal: keep the personal blind-mouse cache aligned with what was just learned, before the session evaporates and you (or future-Claude) re-research the same gotcha next week.

Skip if you don't keep a personal blind-mouse cache. The PR pre-flight (`skills/make_pr.md`) defers to this skill — there's no separate mouse step in make_pr.

## 1. Review the query log

```bash
bin/daslang utils/mouse/main.das -- log --misses
```

(On Windows MSVC layout the binary is `bin/Release/daslang.exe` — same args after.)

For each recent miss:
- **Did this session answer it?** If yes — `mouse__add` (or `mouse add` from CLI). Next session won't redo the work.
- **Did you _almost_ ask mouse this session but didn't?** Try asking now — misses-you-skipped don't show up in `--misses`. If the work you just did has the answer, add it.

```bash
bin/daslang utils/mouse/main.das -- log
```

For recent hits:
- **Did this session invalidate a cached answer?** If yes, edit `mouse-data/docs/<slug>.md` directly and bump `last_verified` (or delete if the entry is no longer relevant).

## 2. Surface the un-asked

The harder pass. Misses-you-skipped never show up anywhere — they only exist as research you did that *should have been* a mouse hit. Cast back over the session:

- **Patterns you discovered.** "How do I X?" / "What's the right shape for Y?" — if you researched it from scratch, write it up. A 5-line `.md` is enough.
- **Gotchas that took >5 min to diagnose.** The fix lives in code; the *why it broke* belongs in mouse so the next person hits it for 30 seconds, not 30 minutes.
- **Decisions worth remembering.** "We picked A over B because…" — if the reasoning isn't obvious from the code or commit message, it's mouse fodder.
- **Cross-cutting facts** that span multiple skills/files and don't fit any single `skills/*.md` slot.

The bar for adding: would a future fresh session benefit from finding this in 1 ask, vs. re-researching? If yes → add. A rough draft beats nothing — the next session that hits it will refine.

## 3. Cost calculus

This is curation, not verification — the work doesn't depend on it. But the math is asymmetric:

- Writing a brief `.md`: ~30 seconds.
- Re-researching the same question next session: 5–30 minutes plus context-switch cost.
- Forgetting altogether: silently regressing the same gotcha.

When in doubt, add.
