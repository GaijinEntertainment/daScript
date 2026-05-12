---
slug: why-does-clone-type-func-result-stored-on-a-call-macro-class-field-get-the-wrong-type-at-visit-time-random-aliased-types-like-do
title: Why does clone_type(func.result) stored on a call_macro class field get the wrong type at visit time (random aliased types like double/tuple<>/GroupState)?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Don't store cloned TypeDecls on call_macro class fields. Store the FunctionPtr and call `clone_type(fn.result)` at visit time instead.**

Symptom: at `[widget]`/`[container]` apply time you do `inst.render_fn_result = clone_type(func.result)`. The clone returns a valid TypeDecl pointer. By the time the call_macro's `visit()` fires (later compile pass, possibly across many other macros applying), `render_fn_result` points at a TypeDecl that's been recycled to hold something else — `double const`, `tuple<>`, `GroupState`, `JsonValue`, varies per session. The wrapper function ends up with the wrong return type and the user's `let x = wrapper(...)` errors as "incompatible return type; double = bool" (or whatever).

Root cause (suspected): the cloned TypeDecl gets GC-collected because the `class WidgetCallMacro` field reference doesn't keep gc_node TypeDecls rooted (or the rooting is lost between compile passes). Other gc_node pointers stored on the class — `state_struct : Structure const?`, `render_fn_args : array<VariablePtr>` — also seemed fine in practice; only `TypeDeclPtr` got aliased. Didn't dig further once the workaround held.

Fix: stash the `FunctionPtr` (long-lived in its home module) instead. Clone the result at visit time when emitting the wrapper:

```das
// At [widget] apply (imgui_boost.das:apply_widget_or_container):
inst.render_fn = func  // FunctionPtr — stable across passes

// At visit time inside emit_indexed_call:
let retType = clone_type(render_fn.result)
var fn = qmacro_function(wrapperName) $(k : $t(keyType); $a(wrapperArgs)) : $t(retType) { … }
```

Same pattern applies for any TypeDecl you want available at visit time — clone from the owning FunctionPtr/StructurePtr/VariablePtr, not from a long-stored TypeDeclPtr field.

Discovered 2026-05-12 building dasImgui indexed-form widgets — wrapper signatures emitted bool, then double, then tuple<>, then GroupState across rebuilds with the same source. Switching to stash-FunctionPtr fixed it instantly.</body>
<parameter name="slug">daslang-call-macro-typedeclptr-field-gets-pointer-aliased-store-functionptr-instead

## Questions
- Why does clone_type(func.result) stored on a call_macro class field get the wrong type at visit time (random aliased types like double/tuple<>/GroupState)?
