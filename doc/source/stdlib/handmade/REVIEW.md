# doc/source/stdlib/handmade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/documentation_rst.md`.

**A diff that adds or edits an `enumeration-`, `structure-`, `structure_annotation-`, `class-`,
or `typedef-` file in this folder opens it with the type description and ends it with one line
per member, in the order the type declares them.** A member is an enum value, a field the type
exposes to daslang, or a bit of a flag typedef; no other kind of file here pairs lines by
position. The generator pairs member lines by position from the end, so a reordered line, or a
line missing under a multi-line type description, ships a wrong description with a green build.

**A property's description is its own `function-` file - never a member line in the type's
file.**
