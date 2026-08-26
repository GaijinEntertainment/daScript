# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A bind added or changed under this folder binds by module.** The Inline modules are
  `$` (builtin), `math`, `strings` and `jit`. In those, a bind whose result is a plain
  value - not a reference, and not a result the callee writes into the caller's result
  slot - and that is not an interop bind (`addInterop`) uses `addExternInline` or
  `addExternInlineEx`; every other module uses an `addExtern...` entry point whose name
  does not contain `Inline`. Binds of the generic container and equality helpers declared
  in `include/daScript/ast/ast_handle.h` (repo root) - the `das_vector_*`, `das_equ*`,
  `das_nequ*`, `das_handle_equ*` and `das_handle_nequ*` families - belong to that header,
  not to the module that instantiates them, and this rule does not decide their flavor.
  Binds in the Inline modules sit on interpreted inner loops, so each gets its own node the
  callee inlines into, while elsewhere binds share one node per signature to keep binaries
  small.

- **Weakening `src/builtin/REVIEW.das`'s bind-flavor scan is a defect** - a bind the gate
  reports is fixed by switching the bind, never by editing the gate and never by dropping
  a module from `review_nttp.das`'s coverage requires, which are the scan's reach.
