---
slug: how-do-i-special-case-bool-or-any-specific-type-inside-a-generic-static_if-at-compile-time
title: How do I special-case `bool` (or any specific concrete type) inside a generic / static_if at compile time in daslang?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

When you need to discriminate one workhorse type from the others inside a generic — typically because the lint forbids one shape on one type but the generic must work for all — use `static_if (typeinfo stripped_typename(field) == "bool")`.

## Why `stripped_typename`, not `typename`

`typeinfo typename(x)` returns the FULL type name including const/ref decorations. For a struct field accessed in a loop, `typename(field)` may yield `"bool#"` or `"bool const&"`, not `"bool"`. `stripped_typename` returns the clean canonical name without decorations — matches what you'd write in source.

Used widely in the daslang test suite:
```das
static_if (typeinfo stripped_typename(field) == "int") { ... }
static_elif (typeinfo stripped_typename(field) == "string") { ... }
```
(see `tests/apply/test_apply.das`).

## Real example: avoiding STYLE018 in a generic

`daslib/json_boost.das` originally had this generic-zero compare, which works for `int`/`float`/etc. but lowers to `if (field == false)` for `bool` and trips the new STYLE018 lint:

```das
static_if (typeinfo is_workhorse(field)) {
    if (field == default<typedecl(field)>) { return ; }
}
```

The fix splits the workhorse path:

```das
static_if (typeinfo is_workhorse(field)) {
    static_if (typeinfo stripped_typename(field) == "bool") {
        if (!field) { return ; }
    } else {
        if (field == default<typedecl(field)>) { return ; }
    }
}
```

`bool` uses the idiomatic `!field`; numeric workhorses keep the generic-zero compare. The STYLE018 false positive disappears, semantics are unchanged.

## What `is_*` typeinfo traits exist

There IS no `typeinfo is_bool` in daslang core (as of 2026-05-09). The full set in `src/ast/ast_infer_type.cpp` is roughly: `is_dim`, `is_struct`, `is_tuple`, `is_variant`, `is_class`, `is_lambda`, `is_enum`, `is_bitfield`, `is_string`, `is_handle`, `is_ref`, `is_ref_type`, `is_ref_value`, `is_const`, `is_temp`, `is_temp_type`, `is_pointer`, `is_smart_ptr`, `is_iterator`, `is_iterable`, `is_vector`, `is_array`, `is_table`, `is_numeric`, `is_numeric_comparable`, `is_local`, `is_function`, `is_void`, `is_void_pointer`, `is_workhorse`, `is_pod`, `is_raw`, plus `can_*` and `need_*` family. For anything not in the list (bool, specific enum, specific struct), use `stripped_typename(x) == "name"`.

Verified 2026-05-09 in PR #2620 (`daslib/json_boost.das:597-611`).

## Questions
- How do I special-case `bool` (or any specific concrete type) inside a generic / static_if at compile time in daslang?
