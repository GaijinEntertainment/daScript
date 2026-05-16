---
slug: when-do-i-need-unsafe-reinterpret-lt-jsonvalue-gt-around-a-body-key-null-lookup-and-when-can-i-just-use-the-const-view-directly
title: 'When do I need `unsafe(reinterpret<JsonValue?>(...))` around a `body?["key"] ?? null` lookup, and when can I just use the const view directly?'
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

**Only when you store the pointer somewhere that wants `JsonValue?` (non-const-data).** For read-only operations (`is _string` / `as _string` / `write_json(v)` / `v != null` / `v.value is _object`) the const view from `?[]` works directly — no `unsafe` cast needed.

The `?[]` operator from `daslib/json_boost` has two overloads (json_boost.das:29-37):

```das
def operator ?[] (a : JsonValue const? ==const; key : string) : JsonValue? const { ... }
def operator ?[] (var a : JsonValue? ==const; key : string) : JsonValue? { ... }
```

Indexing a `const`-bound view returns `JsonValue? const` (const-pointer wrapper, data is mutable). For reading the value out (the common case in any parser), this is fine. The `unsafe(reinterpret<JsonValue?>(...))` strips the outer const so you can:

- **Assign to a non-const struct field** — the load-bearing case. Example: `result.params = params_v` in `daslib/jsonrpc.das:188` requires `var params_v = unsafe(reinterpret<JsonValue?>(body?["params"] ?? null))`. Drop the reinterpret and you get `error[30915]: can only copy compatible type; JsonValue?& = JsonValue? const&`.
- **Pass to a function that takes `var` JsonValue?** — same root cause.

What does NOT need the cast (idiomatic):

```das
let id_v = body?["id"] ?? null              // const view
if (id_v != null && id_v.value is _string)  // read OK
let s = id_v.value as _string               // read OK
let j = write_json(id_v)                    // takes JsonValue? const, OK
```

Historical context: `live_api_stdio.das` (PR #2674, before the `daslib/jsonrpc` extraction in PR #2679) wrapped every `?[]` lookup in `unsafe(reinterpret<JsonValue?>(...))` defensively. When the same code moved into `daslib/jsonrpc.das`, four of the six call sites turned out to be read-only and the cast was dropped. Watch for this pattern next time you migrate JSON-traversal code into a library — the `unsafe` is usually noise.

## Questions
- When do I need `unsafe(reinterpret&lt;JsonValue?&gt;(...))` around a `body?[\"key\"] ?? null` lookup, and when can I just use the const view directly?
