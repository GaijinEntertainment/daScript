---
slug: how-do-i-build-a-jsonvalue-object-inline-without-declaring-a-struct-or-hand-writing-a-table-i-e-what-s-the-laziest-jv-form-for-a
title: How do I build a JsonValue object inline without declaring a struct or hand-writing a table — i.e. what's the laziest JV form for a one-off object?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

## Use the named-tuple form

```das
return JV((kind = meta.kind, rendered = false, payload = invoke(meta.serializer)))
// → {"kind": "...", "rendered": false, "payload": {...}}
```

`(key = value, ...)` is a named-tuple constructor in daslang gen2 — produces a `tuple<key:T; ...>`. `JV(auto)` in `daslib/json_boost` has an `is_tuple` branch (around line 633) that walks the named fields via `apply()` and emits a JSON object using each name as the key. One expression, no `insert` ceremony, no intermediate `table<string; JsonValue?>`.

## What it replaces

```das
// Before — 4 lines + intermediate table
var tab : table<string; JsonValue?>
tab |> insert("kind", JV(meta.kind))
tab |> insert("rendered", JV(false))
tab |> insert("payload", invoke(meta.serializer))
return JV(tab)

// After — one expression
return JV((kind = meta.kind, rendered = false, payload = invoke(meta.serializer)))
```

## Limits — when to fall back

- **No `@optional` / `@rename` / `@embed`** — field annotations only attach to real struct fields, not named-tuple fields. Every key is unconditionally emitted; the JSON key always matches the daslang name. If you need conditional skip-when-zero, declare a small struct with `@optional` instead.
- **All keys are static**. If the keys themselves vary at runtime (mixed-type maps, schema-by-runtime), fall back to manual `table<string; JsonValue?>`.

## Const-pass-through gotcha (fixed 2026-05-10)

If your named tuple contains a `JsonValue?` field (e.g. `payload = invoke(serializer)`), `apply()` hands it to the recursive `JV(field)` as `JsonValue? const`. Pre-fix, `JV(JsonValue? const)` errored with `error[30343]: can't copy constant to non-constant pointer` because the typename pass-through at `daslib/json_boost.das:548` did `return value` without const-stripping.

Fixed in PR #2626: the pass-through now const-strips via `unsafe { return reinterpret<JsonValue?>(value) }`. Safe because `JsonValue?` is a pointer; the cast is a no-op at runtime.

If you hit that error on an older daslang, the workaround is the manual table form — until you can rebuild against post-PR-#2626 daslib.

## Tutorial / source pointers

- Skill: [skills/json.md](https://github.com/GaijinEntertainment/daScript/blob/master/skills/json.md) — "Inline named-tuple JV" section
- Implementation: [daslib/json_boost.das](https://github.com/GaijinEntertainment/daScript/blob/master/daslib/json_boost.das) — `is_tuple` branch in `def JV(value : auto(TT))`

## Questions
- How do I build a JsonValue object inline without declaring a struct or hand-writing a table — i.e. what's the laziest JV form for a one-off object?
