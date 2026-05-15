---
slug: imgui2rst-strict-struct-fields-docstring-count
title: imgui2rst CI panics "has less documentation than values, expected at least N lines" — what's the requirement?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# imgui2rst strict_struct_fields — N //! lines required per N fields

Symptom: dasImgui docs CI panics with:

```
|detail/class-<module>-<ClassName>| has less documentation than values.
Expected at least N lines ([ field1, field2, field3 ]), got M.
PANIC: invalid documentation
```

(Real example from PR #29: `EditExternalCallMacro` had 1 `//!` line above 3 fields → "Expected at least 3 lines, got 1".)

## Rule

A class/struct documented with a `//!` block above the declaration must have **at least one `//!` line per public field**. The check lives at `daslib/rst.das:835` (the `strict_struct_fields` rule), and `utils/imgui2rst.das` enables it. It does NOT count blank `//!` lines specially — each field needs its own descriptive line.

## Fix shape

Per-field `//!` lines BETWEEN the class-level docstring and the field declarations:

```das
class EditExternalCallMacro : AstCallMacro {
    //! Call macro installed by ``[edit_widget]``. Recognizes ...
    //! Name of the [edit_widget] function — used as the rewritten call's target.
    //! Underlying [edit_widget] function pointer; lives in the user module, read at visit time.
    //! User-facing args (positions 2..N after ``ptr`` + injected ``widget_ident``); cloned from render_fn.
    kind_name : string
    render_fn : FunctionPtr
    render_fn_args : array<VariablePtr>
}
```

Order matches field declaration order. First `//!` line is the class-level summary; the next N lines map 1:1 to the N fields.

## Why CI catches it but local builds usually don't

Most local dev runs skip the imgui2rst step. The Sphinx docs build (CI `docs.yml`) is where it surfaces, often hours into a long PR cycle. To reproduce locally:

```
daslang utils/imgui2rst.das -- --detail_output doc/source/stdlib/generated
```

No output = pass. PANIC = the rule fired.

## Applies to

- Any `class` or `struct` in `modules/dasImgui/widgets/*.das` with a `//!` block — `[widget]`/`[container]`/`[call_macro]` infrastructure classes especially. Private classes are exempt (no public docs generated).
- Same rule fires on `enum` and `variant` field counts.

If you don't care to document a field, the rule still requires a `//!` line — give it a one-line "internal — see <function>" pointer. Don't delete the class-level docstring to dodge the rule; that just makes the class undocumented in the published index.

## Questions
- imgui2rst CI panics "has less documentation than values, expected at least N lines" — what's the requirement?
