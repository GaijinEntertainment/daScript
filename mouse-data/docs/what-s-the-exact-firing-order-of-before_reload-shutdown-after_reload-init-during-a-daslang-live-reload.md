---
slug: what-s-the-exact-firing-order-of-before_reload-shutdown-after_reload-init-during-a-daslang-live-reload
title: What's the exact firing order of [before_reload] / shutdown() / [after_reload] / init() during a daslang-live reload, and is `is_reload()` true throughout?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Reload orchestration lives in `utils/daslang-live/main.cpp:432-532` (the `if (needsReload)` block inside `run_lifecycle()`). Six steps, in order:

1. `dll_set_is_reload(true)` (line 433) — `is_reload()` flips to `true` and stays true through ALL subsequent steps.
2. `[before_reload]` annotated functions fire in the OLD context (line 443).
3. User `shutdown()` runs in the OLD context (line 444). `is_reload()` is `true` here — guard with `if (!is_reload()) { ... }` to skip teardown work that should only happen on process exit.
4. Recompile (line 453), swap to NEW context (line 472), `ctx->restart()` (line 506).
5. `[after_reload]` annotated functions fire in the NEW context (line 511) — **before** `init()`. This is the right place to restore preserved state.
6. User `init()` runs in the NEW context (line 522). `is_reload()` is still `true`, so init code can branch on cold-start vs. reload.

`is_reload()` only flips back to `false` after the reload completes. Practical implication: in `init()`, the canonical idempotent-setup pattern is a state check (`if (live_window != null) return`) rather than `if (!is_reload())` — the state already reflects whatever `[after_reload]` restored.

`[before_reload]` and `[after_reload]` are registered as function annotations in `modules/dasLiveHost/src/dasLiveHost.cpp:329-330`. Same family as `[before_update]` / `[live_command]`. No arguments, void return. Multiple per module are allowed and run in declaration order.

**On reload-failure paths:** if recompile fails (line 454), the OLD context is re-init'd (after_reload + init re-run) and the runtime stays paused. If `[after_reload]` itself panics (line 511-519), the persistent store is cleared and the runtime stops. So `[after_reload]` should be defensive about partially-corrupt persistent state.

**Surfaced 2026-05-09** during dasImgui Phase 0a's reload-contract investigation; before this I had to dispatch 3 Explore agents to map the lifecycle when one mouse_ask should have done it.

## Questions
- What's the exact firing order of [before_reload] / shutdown() / [after_reload] / init() during a daslang-live reload, and is `is_reload()` true throughout?
- When does `is_reload()` become true and when does it flip back to false?
- Does daslang-live call my shutdown() before or after recompiling on reload?
- In what context (old or new) do [before_reload] and [after_reload] run?
