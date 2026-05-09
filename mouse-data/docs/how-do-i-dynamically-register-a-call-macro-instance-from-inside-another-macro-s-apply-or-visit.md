---
slug: how-do-i-dynamically-register-a-call-macro-instance-from-inside-another-macro-s-apply-or-visit
title: How do I dynamically register a call_macro instance from inside another macro's apply or visit?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Daslang **does not** support defining new annotation CLASSES at runtime (you can't write a "macro that emits a new `[call_macro(name=foo)]` class"). But you CAN instantiate an existing `AstCallMacro`-derived class multiple times with different per-instance `.name` fields and register each from inside another macro's apply/visit:

```das
// daslib/ast.das line ~931
[macro_function]
def add_new_call_macro(name : string; var someClassPtr) {
    var ann = make_call_macro(name, someClassPtr)
    this_module() |> add_call_macro(ann)
}
```

Or the explicit form (what dasImgui's `[widget]` macro uses):

```das
[function_macro(name="widget")]
class WidgetFunctionMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; ...) : bool {
        // ...build a per-kind dispatcher instance...
        var inst = new WidgetCallMacro(
            kind_name = string(func.name),
            state_type_name = describe(func.arguments[0]._type),
            render_fn_name = string(func.name))
        compiling_module() |> add_call_macro(make_call_macro(string(func.name), inst))
        return true
    }
}

// One single class declaration, instantiated per kind:
class WidgetCallMacro : AstCallMacro {
    kind_name : string
    state_type_name : string
    render_fn_name : string
    def override visit(prog; mod; var expr : ExprCallMacro?) : ExpressionPtr { ... }
}
```

After registration, `kind(IDENT, ...)` user calls dispatch to the right WidgetCallMacro instance via name lookup. The C++ side dedupes by `.name` string only (`include/daScript/ast/ast.h:1198` `addCallMacro`), so multiple instances of the same class with different names coexist fine.

**Two gotchas:**

1. **`find_call_macro` returns null if the name isn't registered** (fixed 2026-05-09; pre-fix it null-deref'd — separate cache entry). Either soft-probe with `find_call_macro` first, or just call `add_call_macro` and let it panic on duplicates with its own message.
2. **Generated annotation isn't visible in the same source file that declared the meta-macro class** (separate cache entry). Put `[widget]`-decorated functions in a sibling file.

**Timing:** registrations are visible to subsequent files in the same compile unit. The macro pass runs once per module, registrations land before the typer needs them downstream.

**Surfaced 2026-05-09** during dasImgui Phase 0a; replaces the (impossible) "meta-macro emits new annotation class" pattern that the original API_REWORK doc sketched.

last_verified: 2026-05-09

## Questions
- How do I dynamically register a call_macro instance from inside another macro's apply or visit?
