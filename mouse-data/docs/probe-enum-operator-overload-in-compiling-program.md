---
slug: probe-enum-operator-overload-in-compiling-program
title: How do I probe whether an enum type has `operator |` (or any operator) defined in the compiling program — for a lint rule that needs to gate on overload existence?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

Walk every module in the program, iterate functions named `"|"` (or whatever operator), and match the argument types against the target enum. Cache per-enum-type — naïve probing is O(modules × functions) and gets called once per ExprOp2 visit.

```das
def enum_has_or_operator(t : TypeDecl?) : bool {
    if (t == null || t.enumType == null) return false
    let key = uint64(intptr(t.enumType))                 // cache key
    for (idx in range(length(probed))) {
        return yes_cache |> has_value(key) if (probed[idx] == key)
    }
    var found = false
    program_for_each_module(compiling_program()) $(mod) {
        return if (found)
        for_each_function(mod, "|") $(var fn) {
            if (length(fn.arguments) == 2
                && fn.arguments[0]._type != null && fn.arguments[1]._type != null
                && (fn.arguments[0]._type |> is_same_type(t, RefMatters.no, ConstMatters.no, TemporaryMatters.no))
                && (fn.arguments[1]._type |> is_same_type(t, RefMatters.no, ConstMatters.no, TemporaryMatters.no))) {
                found = true
            }
        }
    }
    probed |> push(key)
    if (found) { yes_cache |> push(key) }
    return found
}
```

**Why this shape:**
- `program_for_each_module(prog) $(mod) { ... }` from `daslib/ast.das` iterates every module the program can see (including upstream `require`s). Don't restrict to `compiling_module()` — user can define `operator |` for an external enum in their own module.
- `for_each_function(mod, name)` is in `daslib/ast.das:40` — iterates the named function bucket, no name filtering needed.
- `is_same_type(a, b, RefMatters.no, ConstMatters.no, TemporaryMatters.no)` is the canonical AST type comparator (cf. `daslib/async_boost.das:222`, `daslib/match.das`). Don't use `describe(a) == describe(b)` — it conflates ref/const/temp modifiers and is slower.
- Cache key is `uint64(intptr(t.enumType))` — different enums have different Enumeration AST pointers.

Reference implementation: `daslib/perf_lint.das` `enum_has_or_operator` (added for PERF019). Same pattern generalizes to any `operator X` overload probe.

## Questions
- How do I probe whether an enum type has `operator |` (or any operator) defined in the compiling program — for a lint rule that needs to gate on overload existence?
