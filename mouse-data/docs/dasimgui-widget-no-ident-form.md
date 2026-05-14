---
slug: dasimgui-widget-no-ident-form
title: Can dasImgui `[widget]` calls drop the IDENT first-arg?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Yes — three call forms accepted as of commit 19 of the v2 boost mega-detour.** The `[widget]` macro auto-synthesizes the state-global + telemetry path from `{__FILE__}:{__LINE__}:{__COLUMN__}` when the user doesn't supply an explicit ident.

Three forms:

    // 1. EXPLICIT — leading ident drives auto-emitted state global + path key.
    text(MY_LABEL, (text = "hello"))         // state global: var MY_LABEL : NarrativeState
                                              // path key:     <window>/MY_LABEL

    // 2. AUTO-GEN — first arg is the named-tuple, no ident.
    text((text = "hello"))                    // state global: __widget_<line>_<col>
                                              // path key:     <window>/<mod>:<line>:<col>

    // 3. POSITIONAL — single-string sugar for the text family only.
    text("hello")                             // same auto-synth as form 2

The convention: **state where it matters, no-ident where it doesn't.** If you read `IDENT.value` / `IDENT.click_count` / etc., name it. Otherwise drop the ident — it's noise. Synthesized names shift on edits, so test drivers driving widgets via stable telemetry paths should keep explicit idents.

## Positional shorthand scope

Single-string text family only:

    text("...")              ✓
    text_unformatted("...")  ✓
    text_wrapped("...")      ✓
    text_disabled("...")     ✓
    bullet_text("...")       ✓
    separator_text("...")    ✓

NOT:
- `text_colored("...")` — has `color` arg too. Use `text_colored((color = ..., text = ...))`.
- `label_text("...")` — has `key` + `value`. Use `label_text((key = ..., value = ...))`.
- `button("Save")` — interactive; explicit ident encouraged.

## Limitations

**Interpolated strings cannot use positional form.** `text("foo {x}")` parses to `string_builder("foo ", x)` (an `ExprOp2`), not `ExprConstString`. The positional-string gate only matches bare literals. For interpolation, use named-tuple no-ident form:

    text((text = "foo {x}"))     // ✓
    text("foo {x}")              // ✗ error: first argument must be identifier, named-tuple, ...; got string_builder(...)

**Zero-arg widgets still require an explicit ident.** `separator()` / `spacing()` / `bullet()` / `new_line()` / zero-arg `same_line()` all error with `missing widget body — pass (named=arg, …)`. The `[widget]` macro hard-errors on empty argument list. Workaround: pass an explicit ident. Future work: special-case the macro to accept zero-arg.

**`edit_*` family is unchanged** — `id="STABLE_IDENT"` stays mandatory inside the named-tuple. The whole reason `edit_*` exists is stable telemetry paths for caller-owned data; auto-gen would defeat the purpose.

## Questions
- Can dasImgui `[widget]` calls drop the IDENT first-arg?
- What's the no-ident form for dasImgui v2 widgets?
- Does `text("hello")` work in dasImgui boost v2?
- Can I call `separator()` without an ident?
- Does `text("foo {x}")` work as positional form?
