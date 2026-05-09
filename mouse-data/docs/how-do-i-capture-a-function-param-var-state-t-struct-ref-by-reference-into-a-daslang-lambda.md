---
slug: how-do-i-capture-a-function-param-var-state-t-struct-ref-by-reference-into-a-daslang-lambda
title: How do I capture a function param (var state : T struct ref) by reference into a daslang lambda so the lambda can mutate the underlying module global from outside the function?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

## Pattern

The capture clause goes BEFORE `<|`, the `&` marks each ref-captured name, and the WHOLE move into the storage requires `unsafe`:

```das
def register_widget(var state : State; ident : string) {
    unsafe {
        var d <- @ capture(& state) (payload : int) {
            state.pending = true
            state.value = payload
        }
        g_dispatchers[ident] <- d
    }
}
```

## Three syntax landmines

1. **`@ <| capture(& x) (args) { ... }` is a parse error.** Capture clause attaches to `@`, not to `<|`. Correct form: `@ capture(& x) (args) { ... }` — drop the `<|`.
2. **`g_dispatchers[ident] <- @capture(...) (args) {...}` errors as `error[30941]: can't move from a constant value`.** The lambda literal is const-typed; you can't move directly into a table-indexing lvalue. Move into a temp `var d <- ...` first, then `<- d` into the table.
3. **`error[31003]: capture by reference requires unsafe`.** Wrap the move-and-store in `unsafe { ... }`. Capture-by-reference is unsafe because the lambda outlives the function frame; daslang requires you to opt in.

## Why this works for module-global state

When the caller passes a module global (`register_widget(G, "G")`), the function param `var state : State` is a stable ref to G. `capture(& state)` smuggles that ref into the closure. Since G lives forever (module-global), the captured ref stays valid for the lambda's lifetime — invoking the lambda from anywhere mutates G in place.

Verified end-to-end (2026-05-09, daslang Opus 4.7 session): module-global state mutation through a lambda stored in a module-global `table<string; lambda<(int) : void>>`, invoked from outside the registering function.

## Source

Canonical patterns in `dastest/dastest.das:306` (`new_thread <| @capture(& res, & mainCtx) {...}`) and `dastest/suite.das:451-475` (multiple `testing.onFail <- @ capture(& testing, & failed, ...)` shapes — note `<-` works for struct-field assignment without unsafe; only table-indexing assignment needs the `unsafe` wrapper).

## Questions
- How do I capture a function param (var state : T struct ref) by reference into a daslang lambda so the lambda can mutate the underlying module global from outside the function?
