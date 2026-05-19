---
slug: daslang-table-safe-index-safe-only-with-null-coalescing
title: Is `table?[key]` safe in daslang — do I need `unsafe(...)` around it, and when is the safe-index form actually safe?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Rule:** `tab?[k]` on a `table<>` is SAFE only when immediately consumed by null-coalescing `?? default`. Bare `tab?[k]` (or `let x = tab?[k]` followed by a manual null-check) is UNSAFE and the compiler enforces it with:

```
error[31034]: safe-index of table<> must be inside the 'unsafe' block
```

Why: `?[k]` returns a pointer into the table's storage. If the table mutates while you hold the pointer (e.g. another `insert` rehashes), the pointer dangles. `?? default` consumes the pointer immediately (extracts the value or substitutes the default), so the danger window is gone. Manual `if (p == null) return; ... p.field` still holds the pointer across the check, so the compiler keeps it unsafe.

**Safe form:**
```daslang
let v = tab?[k] ?? default_value   // OK — value extracted, no escaping pointer
let n = tab?[k] ?? 0
```

**Unsafe form (must wrap):**
```daslang
let meta = unsafe(g_widgets?[path])   // pointer escapes; manually null-check next
if (meta == null) return
return invoke(meta.state_addr_getter)
```

**STYLE024 false-positive caveat (as of 2026-05-19):** `daslib/style_lint.das` STYLE024 fires on `unsafe(tab?[k])` claiming the wrap is redundant. It is NOT — the compiler error above proves it. STYLE024's "inherently-unsafe shapes" allowlist needs to include `ExprSafeAt` of `table<>` (the `?[k]` form on a table type). Until that fix lands, this remains a known false positive on consumer files that hold the pointer past the safe-coalesce window.

Distinct from `tab[k]` (regular access): `tab[k]` is auto-insert on tables and IS safe (no `unsafe` wrap needed for either read or assign). The pointer issue only arises with the safe-index `?[k]` form.

## Questions
- Is `table?[key]` safe in daslang — do I need `unsafe(...)` around it, and when is the safe-index form actually safe?
