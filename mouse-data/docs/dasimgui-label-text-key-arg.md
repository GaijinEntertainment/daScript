---
slug: dasimgui-label-text-key-arg
title: Why does dasImgui `label_text` use `key=` instead of `label=`?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**`label` is a reserved word in daslang gen2.** It can't be used as a struct field name, record name in a named-tuple, or function parameter. The dasImgui boost surface renames `label` → `key` to dodge it.

ImGui's C++ signature is `LabelText(const char* label, const char* fmt, ...)`. The boost wrapper is:

    [widget]
    def label_text(var state : LabelTextState; key : string; value : string)

Call site:

    label_text((key = "Version", value = "v2.0"))     // ✓
    label_text((label = "Version", value = "v2.0"))   // ✗ error[30151]: syntax error, unexpected label

The state struct `LabelTextState` likewise uses `key : string` + `value : string` (not `label`). The snapshot payload mirrors the field names — `{key: "Version", value: "v2.0"}`.

## Other places `label` bites

- Any `let label = ...` / `var label = ...` local variable — error 30151.
- `def foo(label : string)` parameter — error 30151. Common in helper functions that ImGui-style accept a label. Rename to `lbl` or pick a more specific name.
- `for (label in someEnum)` — error 30151.

Hit Boris ≥2× during the v2 boost mega-detour PR. Both times in widget-helper functions that initially named a string param `label`.

## Workaround precedence

1. **For data-model fields** (snapshot payloads, struct fields): use `key` for the descriptive-label-side of a label/value pair, since that's the conceptual role. Avoid `label` everywhere in the data model.
2. **For local variables / parameters**: rename to `lbl` (3 chars, unambiguous in context) or pick a domain-specific name (`button_text`, `caption`, `prompt`).

## Questions
- Why does dasImgui `label_text` use `key=` instead of `label=`?
- Is `label` a reserved word in daslang?
- What's the daslang error 30151 about?
- Can I name a struct field `label` in daslang?
