---
slug: perf018-false-positive-on-qmacro-t-fields-i-fix-with-count
title: PERF018 fires on `for (i in range(length(arr)))` even though my loop uses `i` inside `qmacro($t(other.field[i]._type))` — false positive? How do I fix without losing `i`?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Not technically a false positive — PERF018's `find_expr_path` walker doesn't descend into `qmacro_expr($t(...))` substitutions.** The `$t(...)` body is a quoted AST template; its uses of `i` don't show up in the visitor's "where is `i` used" scan. From the rule's perspective, `i` is only used as `fields[i]` (the explicit indexing), so the rule fires.

**Resulting code rejection**: you can't just write `for (info in fields) { ... qmacro($t(st.fields[i]._type)) ... }` — `i` is now undefined inside the macro, and compile fails with `can't locate variable 'i'`.

**Fix**: keep `i` available via the `count()` iterator:

```das
for (info, i in fields, count()) {
    let name = info.col_name
    push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
        name = $v(name),
        data_type = sql_storage_enum_for(type<$t(st.fields[i]._type)>),
        ...
    )); }))
}
```

This keeps both `info` (the value, used as a normal field accessor) and `i` (the index, used inside qmacro $t for compile-time type lookup). PERF018 is satisfied (`fields[i]` use is now gone — `info` replaces it; `st.fields[i]` was on a *different* path, never the trigger).

**Suppress route if the refactor isn't natural**: add `// nolint:PERF018` on the `for` line. Reasonable when the body is a tight macro template you don't want to restructure.

Reference fix sites: `modules/dasSQLITE/daslib/sqlite_boost.das` `_sql_column_info` / `make_view_column_info_fn` / `make_fts5_column_info_fn` — each iterates `fields` and emits qmacro_expr that needs `i` for `$t(st.fields[i]._type)`.

## Questions
- PERF018 fires on `for (i in range(length(arr)))` even though my loop uses `i` inside `qmacro($t(other.field[i]._type))` — false positive? How do I fix without losing `i`?
