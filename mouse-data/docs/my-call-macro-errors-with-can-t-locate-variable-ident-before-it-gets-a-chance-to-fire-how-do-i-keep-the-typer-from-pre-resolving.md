---
slug: my-call-macro-errors-with-can-t-locate-variable-ident-before-it-gets-a-chance-to-fire-how-do-i-keep-the-typer-from-pre-resolving
title: My call_macro errors with "can't locate variable 'IDENT'" before it gets a chance to fire — how do I keep the typer from pre-resolving an argument?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Override `canVisitArgument(expr, argIndex) : bool` on your `AstCallMacro` derivative and return `false` for the arguments you want to see RAW (un-typer-resolved):

```das
class WidgetCallMacro : AstCallMacro {
    def override canVisitArgument(expr : ExprCallMacro?; argIndex : int) : bool {
        // First arg is an IDENT this macro will declare as a global on the
        // fly. Telling the typer not to pre-resolve it lets us see the raw
        // ExprVar in visit(). Other args resolve normally.
        return argIndex != 0
    }

    def override visit(prog; mod; var expr : ExprCallMacro?) : ExpressionPtr {
        // expr.arguments[0] is now ExprVar("SAVE_BTN"), not yet resolved
        let identName = string((expr.arguments[0] as ExprVar).name)
        // ...emit the global, rewrite call, etc.
    }
}
```

**Default is `true` for all arg indices** (i.e. typer pre-resolves everything). That's fine for macros whose args are normal expressions (numbers, strings, already-defined refs) — the macro sees the resolved AST.

**You need to override when:** any arg is an identifier the macro will *declare* (e.g. an emit-on-first-use global) or otherwise replace before typer cares. Without the override, the typer fails on the unresolved ident and aborts before your macro runs.

**Bonus:** by default, `canVisitArgument=true` means inner call_macros expand BEFORE the outer macro fires — see related cache entry on `canVisitArgument` inner-first ordering. This is the same knob, used for a different reason.

The full hook list per `daslib/ast.das` `class AstCallMacro`:
- `preVisit(prog, mod, expr)` — before args are visited
- `canVisitArgument(expr, argIndex) : bool` — gate per-arg resolution
- `visit(prog, mod, expr) : ExpressionPtr` — main rewrite, returns the replacement expression
- `canFoldReturnResult(expr) : bool` — after-visit folding

**Surfaced 2026-05-09** during dasImgui Phase 0a `[widget]` work — the call_macro intercepts `button(SAVE_BTN, [text="Save"])` where `SAVE_BTN` doesn't exist yet. Without `canVisitArgument` returning false for index 0, the typer reported "can't locate variable 'SAVE_BTN'" before the macro could create it.

last_verified: 2026-05-09

## Questions
- My call_macro errors with "can't locate variable 'IDENT'" before it gets a chance to fire — how do I keep the typer from pre-resolving an argument?
