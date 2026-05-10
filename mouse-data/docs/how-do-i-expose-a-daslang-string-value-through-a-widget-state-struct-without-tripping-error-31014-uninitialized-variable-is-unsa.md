---
slug: how-do-i-expose-a-daslang-string-value-through-a-widget-state-struct-without-tripping-error-31014-uninitialized-variable-is-unsa
title: How do I expose a daslang `string` value through a `[widget]` state struct without tripping error 31014 (uninitialized variable is unsafe) at compile time?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Two equivalent options. Lead with the default literal:**

```das
struct InputTextState {
    @live value : string = ""                             //! current text
    @live capacity : int = 256
    @optional has_pending : bool
    @optional pending_value : string = ""
    @optional changed : bool
    @optional buffer : array<uint8>
}
```

`= ""` makes the field initialized at the type level, so the `[widget]` macro's auto-emitted `var IDENT : T = T()` produces a fully-formed struct value, no error 31014.

**Alternative: `@safe_when_uninitialized`** on the field. Before the `[widget]` auto-init fix (dasImgui commit `111f7dc`), this was the only option because the macro emitted a bare `var IDENT : T` with no constructor. Post-fix, the constructor runs and `= ""` is the cleaner answer; `@safe_when_uninitialized` still works for fields that genuinely should start as null/zero (rare for strings, more common for handle-typed fields):

```das
struct LegacyShape {
    @safe_when_uninitialized @live value : string         //! starts as null pointer
}
```

After this, `IDENT.value` is a daslang null string at first read. Most string ops are null-safe (`length(null) == 0`, `"{x}"` interpolation prints empty), so reads work before any explicit set. JV serialization handles null strings cleanly too.

**Companion gotchas (same pattern, same struct):**

1. **Cross-context string lifetime.** When dispatcher receives a `string` payload from `imgui_set` (decoded by `from_JV` against a JV the live_command machinery owns), the bytes drop when the dispatcher returns. Use `:=` (clone), NOT `=`, when storing to the state field: `state.pending_value := from_JV(payload?["value"], type<string>, "")`. This forces a clone into the widget context heap so the next frame's read is safe.
2. **Default `+` annotations compose.** Numeric / vector / enum / struct-ctor defaults all fire on the auto-emitted global post-fix — see card `dasimgui-widget-state-struct-field-defaults-fire`. Stack `@live` / `@optional` / `@safe_when_uninitialized` with the default expression as needed.
3. **`array<T>` doesn't need this annotation.** Bit-zero array means empty array; daslang doesn't refuse to compile. `array<uint8>` working buffers are the right "raw bytes" companion to a `string` value field.

**Found 2026-05-10**, dasImgui Phase 0b.4 (`InputTextState` design). Updated 2026-05-10 post auto-init fix — `= ""` is now the recommended primary form.

## See also
- `dasimgui-widget-state-struct-field-defaults-fire` — auto-emit invokes `T()` so field defaults fire on the global
- `daslang-strings-cannot-carry-embedded-nulls-use-array-uint8-and-reinterpret-string-temp-for-c-api-zero-separated-buffers` — when you need to *avoid* `string` and use `array<uint8>` for the same payload

## Questions
- How do I expose a daslang `string` value through a `[widget]` state struct without tripping error 31014 (uninitialized variable is unsafe) at compile time?
