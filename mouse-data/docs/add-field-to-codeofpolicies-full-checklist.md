---
slug: add-field-to-codeofpolicies-full-checklist
title: What's the full checklist for adding a new field to CodeOfPolicies so it works end-to-end (compiler reads it, options.das_root maps it, RTTI sees it, das2rst documents it)?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Adding a `CodeOfPolicies` field looks like one edit but is actually FOUR coordinated changes. Miss any of them and you get silent failures (no compile error, just wrong rendered docs / option not visible to RTTI).

**1. Declare the field** in `include/daScript/ast/ast.h` inside `struct CodeOfPolicies` (~line 1489). Annotate `/*option*/` if you want it overridable via per-file `options foo = true`:

```cpp
/*option*/ bool log_module_compile_time = false;   // short trailing comment is fine
```

**2. Register it for RTTI** in `src/builtin/module_builtin_rtti.cpp` (search for the existing `addField<DAS_BIND_MANAGED_FIELD(...)>("...")` calls in the CodeOfPolicies registration block, ~line 940):

```cpp
addField<DAS_BIND_MANAGED_FIELD(log_module_compile_time)>("log_module_compile_time");
```

Without this, `for_each_field` (used by `daslib/rst.das`) doesn't see the field and `das2rst` silently skips it.

**3. Add a description line** in `doc/source/stdlib/handmade/structure_annotation-rtti-CodeOfPolicies.rst`. This file is **strictly positional**: it maps line-by-line to RTTI-registered fields in *offset-sorted order* (which equals C++ declaration order for non-virtual structs). Place the new line at the position matching where you declared the field in step 1.

**Silent shift trap:** if the handmade file ends up short by ONE line vs registered fields, EVERY field after the gap gets its predecessor's description — but no error, no warning. The only way to spot it is reading the rendered `doc/source/stdlib/generated/rtti.rst` and confirming field name + description agree.

The reader code is `daslib/rst.das:817-880` (`document_topic`). It computes `headerLen = got - expected` and uses the first `headerLen` lines as preamble, then maps the remaining lines 1:1 to fields. So adding a line in the WRONG slot doesn't crash — it just silently mis-binds.

**4. Add a row** to the language-reference options table at `doc/source/reference/language/options.rst` (search for `log_total_compile_time` — it's a worked example near the bottom). Only needed if the field is `/*option*/` (user-overridable per file).

**Then regen + verify:**

```bash
cmake --build build --config Release --target daslang -j 64
bin/daslang doc/reflections/das2rst.das
grep -n "your_new_field" doc/source/stdlib/generated/rtti.rst   # must show with correct description
```

If the description shown next to your field is for the WRONG field, your handmade file is short by one — find the missing description and add it. (I hit this in PR #2677: `max_call_depth` had been missing a description for who-knows-how-long, silently shifting ~30 later fields' descriptions up by one in the generated rtti.rst.)

Bonus: if you also want a CLI flag for your option in `daslang`, the wiring is described separately under `add-cli-flag-to-daslang-executable`.

## Questions
- What's the full checklist for adding a new field to CodeOfPolicies so it works end-to-end (compiler reads it, options.das_root maps it, RTTI sees it, das2rst documents it)?
