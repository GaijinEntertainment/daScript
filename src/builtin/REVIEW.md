# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A bind added or changed under this folder uses `addExternInline` or
  `addExternInlineEx` when its module is `$` (builtin), `math`, `strings` or `jit`, its
  result is a plain value - not a reference, and not a result the callee writes into the
  caller's result slot - and it is not an interop bind (`addInterop`); a bind in any other
  module uses an `addExtern...` entry point whose name does not contain `Inline`.** This
  rule does not decide the flavor of binds of the generic container and equality helpers
  declared in `include/daScript/ast/ast_handle.h` (repo root) - the `das_vector_*`,
  `das_equ*`, `das_nequ*`, `das_handle_equ*` and `das_handle_nequ*` families. Binds in
  `$`, `math`, `strings` and `jit` run inside interpreted inner loops, so each gets its own
  node the callee inlines into, while binds elsewhere share one node per signature to keep
  binaries small.

- **Weakening `src/builtin/REVIEW.das`'s bind-flavor scan is a defect** - fix a bind the
  gate reports by switching the bind, never by editing the gate and never by dropping a
  module from `review_nttp.das`'s `require` list, which sets the modules the scan covers.
