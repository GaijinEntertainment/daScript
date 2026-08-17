# daslib module conventions

Read before working inside `daslib/` or adding a module to it. What each module *offers* is
`skills/daslang/references/modules-and-stdlib.md`; how to format the file is
`skills/das_formatting.md`. This file is only the conventions a contributor must match.

## Base + boost

Many modules come in pairs: `daslib/foo.das` (runtime functions, iterators, the pure API) plus
`daslib/foo_boost.das` (macro sugar, compile-time rewrites). Most boost modules re-export their base
with `require daslib/foo public`, so requiring the boost alone suffices — but **check the boost
module's own require line before assuming it**: `profiler_boost` requires `daslib/profiler`
*non*-publicly, and `templates_boost` has no `daslib/templates` at all (it re-exports
`daslib/quote`). Several `*_boost` modules have no base sibling either — some extend a builtin
(`strings_boost` → `strings`, `jobque_boost` → `jobque`, `math_boost` → `math`), others stand alone
(`array_boost`, `class_boost`, `sort_boost`, `with_boost`, …).

## Naming

- A name that collides with a keyword takes a trailing underscore — `where_`, `having_` in `linq`.
- A module whose short verbs would collide unqualified prefixes all of them: `logger_init`,
  `logger_flush`, never a bare `flush`.
- `//!` doc-comments go on public symbols only — see `skills/comment_style_hygiene.md`.

## Iterator functions

- `foo_impl` is the private generator, `foo` the public wrapper returning `iterator<T>`,
  `foo_to_array` the `to_array` convenience, plus an in-place overload taking `var arr : array<T>`
  wherever the operation has one.
- Name it **`each`** when the type should work as `for (v in x)`: only that name is auto-inserted by
  the compiler, and it wants `[unsafe_outside_of_for]`
  (`skills/daslang/references/closures.md`).

## Modules outside daslib

A module under `modules/` (rather than `daslib/`) needs a `.das_module` descriptor for the dynamic
binary — pure-das modules use `register_native_path`, C++ modules `register_dynamic_module`. See
`skills/dynamic_modules.md`.
