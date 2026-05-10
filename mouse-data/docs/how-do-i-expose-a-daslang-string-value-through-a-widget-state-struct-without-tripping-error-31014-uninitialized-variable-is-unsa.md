---
slug: how-do-i-expose-a-daslang-string-value-through-a-widget-state-struct-without-tripping-error-31014-uninitialized-variable-is-unsa
title: How do I expose a daslang `string` value through a `[widget]` state struct without tripping error 31014 (uninitialized variable is unsafe) at compile time?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Use `@safe_when_uninitialized` on the field.** The `[widget]` macro auto-emits the per-ident state global as a bare `var IDENT : StateStruct` — no constructor call, just bit-zero init. For `string`, bit-zero is null pointer; daslang refuses to compile a global with an uninitialized string heap-pointer field, raising error 31014: `Uninitialized variable IDENT is unsafe. Use initializer syntax or @safe_when_uninitialized when intended.`

The field annotation that suppresses this is `@safe_when_uninitialized`. It goes immediately before the field name (one line up or same line, `@`-form, NOT `[...]`):

```das
struct InputTextState {
    @safe_when_uninitialized @live value : string         //! current text
    @live capacity : int                                  //! widget body promotes 0 → default
    @optional has_pending : bool
    @safe_when_uninitialized @optional pending_value : string
    @optional changed : bool
    @optional buffer : array<uint8>                       // working buffer
}
```

After this, `IDENT.value` is a daslang null string at first read. Most string ops are null-safe (`length(null) == 0`, `"{x}"` interpolation prints empty), so the widget body can read `state.value` even before the user has set anything. JV serialization handles null strings cleanly too.

**Companion gotchas (same pattern, same struct):**

1. **Numeric defaults still don't fire.** `@live capacity : int = 256` does NOT mean the global starts at 256 — it starts at 0. Promote to the desired default in the widget body's first-frame init: `if (state.capacity == 0) { state.capacity = 256 }`. (See sibling card `dasimgui-widget-auto-emit-skips-struct-field-defaults`.)
2. **Cross-context string lifetime.** When dispatcher receives a `string` payload from `imgui_set` (decoded by `from_JV` against a JV the live_command machinery owns), the bytes drop when the dispatcher returns. Use `:=` (clone), NOT `=`, when storing to the state field: `state.pending_value := from_JV(payload?["value"], type<string>, "")`. This forces a clone into the widget context heap so the next frame's read is safe.
3. **`@safe_when_uninitialized` and `@optional` compose.** Apply both when the field is both initially-null AND skip-when-zero: `@safe_when_uninitialized @optional pending_value : string`.
4. **`array<T>` doesn't need this annotation.** Bit-zero array means empty array; daslang doesn't refuse to compile. `array<uint8>` working buffers are the right "raw bytes" companion to a `string` value field.

**Found 2026-05-10**, dasImgui Phase 0b.4 (`InputTextState` design). String fields were the blocker — without the annotation, the whole struct was unusable as a `[widget]` state, even though arrays and ints in the same struct were fine.

## See also
- `dasimgui-widget-auto-emit-skips-struct-field-defaults` — sibling: numeric defaults don't fire either, push to call-site
- `daslang-strings-cannot-carry-embedded-nulls-use-array-uint8-and-reinterpret-string-temp-for-c-api-zero-separated-buffers` — when you need to *avoid* `string` and use `array<uint8>` for the same payload

## Questions
- How do I expose a daslang `string` value through a `[widget]` state struct without tripping error 31014 (uninitialized variable is unsafe) at compile time?
