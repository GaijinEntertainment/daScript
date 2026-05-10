---
slug: how-do-i-make-one-shared-init-function-per-user-module-that-accumulates-contributions-from-multiple-call-macro-firings
title: How do I make ONE shared [init] function per user module that accumulates contributions from multiple call_macro firings?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

Use `daslib/ast_boost`'s `setup_call_list(name, at, isInit=true, isPrivate=true)`. It calls `compiling_module() |> find_unique_function(name)` to dedupe — first call creates a new private `[init] def <name>()`, subsequent calls return the same function's body block. Each macro firing appends one ExprCall via `reg.list |> push(qmacro(...))`.

**Production precedent:** `daslib/decs_boost.das:583-589` — every `[decs_pass]`-annotated function appends one line into `register\`decs\`passes()` (note the backtick-quoted name to avoid clashes with user identifiers).

**dasImgui Phase 1.1 use case** (`widgets/imgui_boost.das`): every `[widget]`/`[container]` invocation appends a `register_widget(...)` line into the user module's `register\`imgui\`widgets()`. First widget per module ALSO prepends a `clear_module_widgets("<modname>")` call (gated by `empty(reg.list)`) so reload re-fire drops removed widgets and replaces stale closures with fresh ones bound to post-reload state addresses.

**Skip `isLateInit`** — at first I passed `(name, at, true, true, true)` thinking lateInit was needed for reload re-fire, but [init] in the user's module re-fires on user-module reload anyway (lateInit is just a sort-order modifier; see `src/ast/ast_simulate.cpp:3858-3883`). decs_boost uses just `(name, at, true, true)` — match the precedent.

**`compiling_module()` resolves to the user's calling module at call_macro visit time** (not the macro-defining module). So `setup_call_list` inside a `WidgetCallMacro.visit()` puts the function in the user's source file — exactly where you want module-private init accumulation.

## Questions
- How do I make ONE shared [init] function per user module that accumulates contributions from multiple call_macro firings?
