---
slug: daslang-generic-instance-detect-via-fromgeneric
title: How do I detect that an ExprCall is to a daslang generic (e.g. each, length, find) when func.name is the mangled instance name and not the original generic's name?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

When a daslang generic function (`def each(a : array<auto(TT)>) : iterator<TT&>`, `def length(a : auto | #) : int`, etc.) is resolved against a concrete type at infer time, the resolved `Function?` instance gets a **mangled name** like `` `builtin`each`30908`12345 ``. Macro code that compares `call.func.name == "each"` will never match a typed instance.

The original generic's identity lives in `call.func.fromGeneric`:

```das
[macro_function]
def private is_each_call(call : ExprCall?) : bool {
    if (call == null || call.func == null) return false
    return (call.func.name == "each"
        || (call.func.fromGeneric != null && call.func.fromGeneric.name == "each"))
}
```

The `name == "each"` branch covers the unusual case where you see the call before the typer has specialized it (e.g. inside a custom call_macro that runs early). The `fromGeneric.name` branch is the normal case for any post-infer chain.

**When this bites:** writing a `[macro_function]` that pattern-matches on a stdlib helper by name — `each`, `length`, `key_exists`, `find`, `set_insert`, all the generic `to_array`/`to_table` variants. Without the `fromGeneric` check, every typed chain silently falls through your match and your macro behaves as if the helper wasn't there.

**Generalizes beyond function calls:** same applies to method overload resolution. `call.func.fromGeneric` is the canonical "which generic was this instantiated from?" link. There's no `originalName` field — the chain is `func → func.fromGeneric → fromGeneric.name`.

**Doesn't apply to:** C++ builtins from `addExtern<>` (no fromGeneric, the `func.name` is the bound name directly). Builtins also have `func.flags.builtIn = true` if you need to distinguish.

See [[my-fold-macro-emits-a-loop-with-for-it-in-source-acc-reserve-length-source-but-the-reserve-doesn-t-fire-when-the-chain-starts-wi]] for the concrete case where this broke `peel_each` in `daslib/linq_fold.das`.

## Questions
- How do I detect that an ExprCall is to a daslang generic (e.g. each, length, find) when func.name is the mangled instance name and not the original generic's name?
