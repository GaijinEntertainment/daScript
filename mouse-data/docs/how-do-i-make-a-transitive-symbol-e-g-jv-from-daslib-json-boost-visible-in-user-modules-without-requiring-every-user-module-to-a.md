---
slug: how-do-i-make-a-transitive-symbol-e-g-jv-from-daslib-json-boost-visible-in-user-modules-without-requiring-every-user-module-to-a
title: How do I make a transitive symbol (e.g. `JV` from daslib/json_boost) visible in user modules without requiring every user module to add the require line?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Use `require X public` in a module that all user modules already require.** The `public` re-exports X transitively to consumers.

**Concrete dasImgui Phase 1.1 use case:** the `[widget]` macro emits a serializer lambda whose body is plain `JV($i(identLit))`. JV's struct-reflection overload lives in `daslib/json_boost`, which user modules don't import directly. Two fixes considered:

1. (rejected) Generic trampoline `widget_state_jv<T>(state) { return JV(state) }` in `imgui_boost_runtime` — works via daslang's "unqualified resolves at defining module" rule, but adds an indirection.

2. (chosen, Boris's suggestion) `require daslib/json_boost public` in `imgui_boost_runtime`. User modules require `imgui/imgui_boost_runtime` (already part of every example), so `JV(state)` in the macro-emitted lambda just resolves directly.

Trampoline dropped, chain became `user → imgui_boost_v2 → imgui_boost_runtime → json_boost`. Cleaner.

**When NOT to use:** if the re-exported module pollutes user namespace with names you'd rather they use a qualifier for. For widely-used utility modules like `daslib/json` / `daslib/json_boost` / `daslib/strings_boost`, transitive re-export is fine — these names are de-facto stdlib.

Note `require X public` already shows up in CLAUDE.md's "Generic function dispatch" section, but the concrete pattern of "use it to expose JV from a runtime support module" is the discovery worth caching.

## Questions
- How do I make a transitive symbol (e.g. `JV` from daslib/json_boost) visible in user modules without requiring every user module to add the require line?
