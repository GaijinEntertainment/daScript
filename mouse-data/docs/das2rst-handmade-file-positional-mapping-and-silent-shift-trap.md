---
slug: das2rst-handmade-file-positional-mapping-and-silent-shift-trap
title: How does daslib/rst.das map handmade .rst description lines to struct fields, and what is the silent-misalignment trap when one line is missing?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

`doc/source/stdlib/handmade/structure_annotation-<mod>-<Struct>.rst` files (and similar `class_annotation-*.rst`) are read **positionally** by `daslib/rst.das:817-880` (`document_topic`):

1. Reader splits the file on `\n`, strips trailing blank lines → `got` lines.
2. Compares to `expected = length(tab) - 1` (one row per field, after header row).
3. Computes `headerLen = got - expected`.
4. First `headerLen` lines printed verbatim as preamble (typically the struct description).
5. **Last `expected` lines** mapped 1:1 to fields in **offset-sorted order** (= C++ declaration order for non-virtual structs).

So for a struct with N fields, you want N+1 non-blank lines (1 struct description + N field descriptions). The mapping is by POSITION, not by field name — there's no field-name annotation in the .rst, just one description per line.

**The silent shift trap:** if the file ends up with N lines instead of N+1 (one missing description anywhere in the middle), every field from the gap onwards inherits its successor's description. No compile error, no warning — `das2rst` happily produces garbage docs. The only way to detect:

```bash
bin/daslang doc/reflections/das2rst.das
grep -A 1 "field_name_you_know" doc/source/stdlib/generated/rtti.rst  # description text on next line must match
```

If the description belongs to a different field, your handmade file is short by one line somewhere between the file start and that field. Binary-search backwards to find where the shift begins — the first field with a wrong description marks the slot where one line is missing.

**Why this is fragile:** the file uses no field-name markers, no JSON, no positional separators. A future contributor adds a new C++ field, forgets the handmade entry → the next field's description (and everything after) drifts. Has happened multiple times to `CodeOfPolicies`.

**Fix recipe when you spot it:** read the offset of the misalignment in the rendered file, count back to find which C++ field has no description, add a line at the matching position in handmade. Repeat until rendered matches expected.

If `bin/daslang doc/reflections/das2rst.das` panics with "has less documentation than values" — that's the FRIENDLY case (file has FEWER lines than fields). The silent case is when somebody added the right number of lines but in the wrong positions, or when an EARLIER field's description is missing.

PR #2677 fixed a pre-existing silent shift in `structure_annotation-rtti-CodeOfPolicies.rst` (missing description for `max_call_depth`). About 30 fields downstream had their previous-field's description showing.

## Questions
- How does daslib/rst.das map handmade .rst description lines to struct fields, and what is the silent-misalignment trap when one line is missing?
