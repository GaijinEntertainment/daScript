---
slug: does-init-lateinit-re-fire-on-hot-reload-but-plain-init-only-fires-on-cold-start-or-do-both-re-fire
title: Does `[init, lateInit]` re-fire on hot-reload but plain `[init]` only fires on cold start, or do both re-fire?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Both re-fire on every `runInitScript()` call.** `lateInit` is purely a sort-order modifier — late-init functions run AFTER regular init functions and get topologically sorted relative to each other. Neither flag affects WHETHER the function fires on reload.

Source: `src/runtime/context.cpp:548-552` — `runInitScript` unconditionally iterates `initFunctions[0..totalInitFunctions]` and calls each. `src/ast/ast_simulate.cpp:3858-3883` — the `lateInit` flag is only consulted to find `firstLateInit` for sort partitioning.

Hot-reload mechanics (per `what-s-the-exact-firing-order-...` card): daslang-live creates a NEW context on reload and calls `ctx->restart()` (which runs `runInitScript`) — so EVERY `[init]` function fires fresh, regardless of `lateInit`.

**Practical implication:** if you're using `setup_call_list(name, at, isInit=true, isPrivate=true)` and considering whether to also pass `isLateInit=true`, the answer is "no, it's redundant for re-fire purposes". Only pass it if you need the function to run AFTER some other init function for ordering reasons. `daslib/decs_boost.das:585` doesn't use lateInit; dasImgui Phase 1.1 doesn't either.

## Questions
- Does `[init, lateInit]` re-fire on hot-reload but plain `[init]` only fires on cold start, or do both re-fire?
