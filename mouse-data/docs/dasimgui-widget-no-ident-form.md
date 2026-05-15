---
slug: dasimgui-widget-no-ident-form
title: Can dasImgui `[widget]` calls drop the IDENT first-arg?
created: 2026-05-14
last_verified: 2026-05-15
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

## Form 3 positional gate — what it accepts

The call_macro at `imgui_boost.das:222` matches `head is ExprConstString || head is ExprStringBuilder`. So:

    text("hello")                ✓ ExprConstString
    text("foo {x}")              ✓ ExprStringBuilder (interpolation builds at parse time)
    text("a" + "b")              ✗ ExprOp2 (binary +) — falls through to Form 1, treats `"a"` as IDENT, errors
    text(LIPSUM)                 ✗ ExprVar — falls through to Form 1, treats LIPSUM as widget IDENT, errors
    text(some_local)             ✗ same

**Variables and string concatenations must use Form 2 (named-tuple):**

    text((text = LIPSUM))                       // ✓ ExprVar inside named-tuple is fine
    text((text = "a" + "b" + "c"))              // ✓ multi-line concat inside named-tuple
    text((text = "{x} of {y}"))                 // ✓ but STYLE001 will flag this — use positional

## STYLE001: positional is mandatory when it works (PR #33)

After PR #33, `text((text = "literal"))` is a `macro_error` (50503) when the positional form would compile. Rule applies to single-arg narrative widgets where the named-tuple's lone field-value is `ExprConstString` or `ExprStringBuilder`. Concat / variable forms are unaffected. Per-file opt-out: `options _allow_imgui_legacy = true`.

## Other limitations

**Zero-arg widgets still require an explicit ident.** `separator()` / `spacing()` / `bullet()` / `new_line()` / zero-arg `same_line()` all error with `missing widget body — pass (named=arg, …)`. The `[widget]` macro hard-errors on empty argument list. Workaround: pass an explicit ident. Future work: special-case the macro to accept zero-arg.

**`edit_*` family is unchanged** — `id="STABLE_IDENT"` stays mandatory inside the named-tuple. The whole reason `edit_*` exists is stable telemetry paths for caller-owned data; auto-gen would defeat the purpose.

## Questions
- Can dasImgui `[widget]` calls drop the IDENT first-arg?
- What's the no-ident form for dasImgui v2 widgets?
- Does `text("hello")` work in dasImgui boost v2?
- Can I call `separator()` without an ident?
- Does `text("foo {x}")` work as positional form?
