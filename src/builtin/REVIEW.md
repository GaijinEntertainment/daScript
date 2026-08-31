# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A diff that adds or changes an `addExtern...`/`addInterop` registration under this folder
  uses, for a plain-value bind, `addExternInline` or
  `addExternInlineEx` when its module is an Inline module, and an `addExtern...` entry point
  whose name does not contain `Inline` in every other module.** A bind is a C++ function
  registered into a module with an `addExtern...` or `addInterop` entry point; annotation,
  type, and structure registrations are not binds. The Inline modules are `$`
  (builtin), `math`, `strings` and `jit`. A plain-value bind returns nothing at all, or a value
  that is neither a reference nor a result the callee writes into the caller's result slot; it
  is not an interop bind (`addInterop`), and not a generic container or equality helper
  (`das_vector_*`, `das_equ*`, `das_nequ*`, `das_handle_equ*`, `das_handle_nequ*`).

- **Weakening `review_nttp.das`'s bind-flavor scan, which `REVIEW.das` runs, is a defect** -
  fix a bind the scan reports by switching the bind, and never drop a module from
  `review_nttp.das`'s `require` list, which sets the modules the scan covers.

- **A diff that adds a module under this folder adds it to `review_nttp.das`'s `require`
  list, in the same change** - a module off the list is a module the scan never sees.
