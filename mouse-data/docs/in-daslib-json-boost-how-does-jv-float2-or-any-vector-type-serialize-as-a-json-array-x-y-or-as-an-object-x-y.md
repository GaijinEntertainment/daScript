---
slug: in-daslib-json-boost-how-does-jv-float2-or-any-vector-type-serialize-as-a-json-array-x-y-or-as-an-object-x-y
title: In daslib/json_boost, how does `JV(float2)` (or any vector type) serialize — as a JSON array `[x, y]` or as an object `{"x":..., "y":...}`?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**As an OBJECT** with named fields, NOT as an array. `JV(float2(1.0, 2.0))` produces `{"x": 1.0, "y": 2.0}`. Same for `int2`/`int3`/`float3`/`float4` — each component becomes a named field (`x`/`y`/`z`/`w` for vectors, `from`/`to` for ranges).

Source: `daslib/json_boost.das:372` — `def JV(v : auto(VecT) const) : auto` walks the vector via daslang RTTI and emits one named insert per component, returning `JV(table)` not `JV(array)`.

**Practical impact for tests / playwright:**
```das
// WRONG — returns null because float2 isn't an array.
let max_y = from_JV(scroll_max?[1], type<float>, 0.0f)

// RIGHT — navigate by named field.
let max_y = from_JV(scroll_max?["y"], type<float>, 0.0f)
```

This bit me writing the dasImgui Phase 1.2 scroll capture test — `WindowState.scroll_max : float2` came back as `{"x": 262, "y": 763.5}` not `[262, 763.5]`, and the `?[1]` index returned `JV(null)` (silently → `from_JV` default → 0).

**Tuples are different**: `tuple<float; float>` serializes as `{"_0": ..., "_1": ...}` (numeric-suffix field names). `array<float>` serializes as `[a, b, c, ...]` (true JSON array). When in doubt, debug-print with `to_log(LOG_INFO, sprint_json(jv, true))` to see the actual shape.

## Questions
- In daslib/json_boost, how does `JV(float2)` (or any vector type) serialize — as a JSON array `[x, y]` or as an object `{"x":..., "y":...}`?
