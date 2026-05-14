---
slug: how-do-i-format-a-float-to-n-decimal-places-as-a-string-in-daslang
title: How do I format a float to N decimal places as a string in daslang?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

Use `build_string() <| $(var w) { fmt(w, ":.2f", value) }` (from `require strings`). The result is a plain `string`.

```das
require strings
let s  = build_string() <| $(var w) { fmt(w, ":.2f",  3.14159f) }   // "3.14"
let s2 = build_string() <| $(var w) { fmt(w, ":+.3f", -0.5f) }      // "-0.500"
let s3 = build_string() <| $(var w) { fmt(w, ":05d",  42) }         // "00042"
let s4 = build_string() <| $(var w) { fmt(w, ":x",    255) }        // "ff"
```

**Format spec is Python-style, not printf**: leading `:` and no `%`. Single-arg form only (each `fmt` call formats one value — no `"x={} y={}"` multi-arg interpolation; chain multiple `fmt` calls inside one `build_string` for that).

**Gotchas**:
- `format(w, "%.2f", value)` exists but is deprecated. The compiler's deprecation message *says* "use fmt() instead" implying drop-in replacement, but **the format spec syntax is different** (`:.2f` vs `%.2f`). Passing `%.2f` to `fmt` at compile time looks valid but throws `EXCEPTION: fmt error: invalid format string` at runtime. The deprecation message is misleading; the rename is a real spec change. (Reported upstream as a candidate fix per Boris's "fix misleading errors at source" preference.)
- Plain string interpolation `"{value}"` gives full float precision (e.g. `"0.913957"`) with no way to specify a format spec inline.
- For drawlist `AddText` on widgets where text width matters for layout — always fixed-decimal via `fmt` rather than interpolation, or the rendered width changes per frame.

**Where it lives**: `daslib/strings_boost.das` provides `fmt`/`build_string`; visible via `require strings` (which re-exports strings_boost). Reference tests: `tests/strings/test_cpp_functions.das:14+` exercises every spec combo.

**Example use site**: `modules/dasImgui/examples/tutorial/custom_widgets.das` — knob value readout under each drawlist-painted knob.

## Questions
- How do I format a float to N decimal places as a string in daslang?
