# doc/source/stdlib/handmade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/documentation_rst.md`.

**A diff that adds or edits an `enumeration-`, `structure-`, `structure_annotation-`, `class-`,
or `typedef-` file in this folder opens it with the type description and ends it with one line
per member, in the order the generated `doc/source/stdlib/generated/<module>.rst` page (repo
root) lists them.** A member is one row of the type's Values, Fields or Variants table on that
page - enum values, fields (a class's static fields first), bits and a flag typedef's named
constants, variant alternatives; a `structure_annotation-` page lists fields in C++ declaration
order, not `addField` call order.
The generator pairs member lines by position from the end, so a reordered line, or a
line missing under a multi-line type description, ships a wrong description with a green build.

**A property's description - a property is an `addProperty` accessor, whose stub is
`function-<module>-_dot__rq_<name>-*.rst` - is its own `function-` file, never a member line in
the type's file.**
