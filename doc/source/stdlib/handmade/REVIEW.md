# doc/source/stdlib/handmade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/documentation_rst.md`.

**A diff that adds or edits a handmade file in this folder for a type with members gives it one
type-description line, then one line per member, in the order the type declares them; a
property's description is its own `function-` file, never a line here.** A member is an enum
value, a field the type exposes to daslang, or a bit of a flag typedef. The generator pairs
those lines by position from the end, so a reordered line, or a line missing under a multi-line
type description, ships a wrong description with a green build.
