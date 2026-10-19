# doc/source/stdlib/handmade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** A diff to a
stub here - a `.rst` file in this folder whose text `doc/reflections/das2rst.das` (repo root)
splices into the generated stdlib page - is also reviewed with
`skills/internal/documentation_rst.md` (repo root).

**A diff that adds or edits an `enumeration-`, `structure-`, `structure_annotation-`, `class-`,
or `typedef-` stub file, wherever it sits, opens it with the type description and ends it with one line
per member, in the order the generated `doc/source/stdlib/generated/<module>.rst` page (repo
root) lists them.** A member is one row of the type's Values, Fields or Variants table on that
page. Read the order from a page regenerated with `daslang -documentation
doc/reflections/das2rst.das` (repo root). The generator matches member lines to members by
counting from the file's end, so a reordered line, or a line missing under a multi-line type
description, puts a wrong description on the page and the build still passes.

**A diff that documents a property - an `addProperty` accessor - writes its description in its own
`function-<module>-_dot__rq_<name>-*.rst` stub, never as a member line in the type's stub file.**
