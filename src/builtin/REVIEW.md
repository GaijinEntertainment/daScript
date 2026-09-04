# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A diff that adds or changes an `addExtern...`/`addInterop` registration under this folder
  uses, for a plain-value bind, `addExternInline` or `addExternInlineEx` when the name its
  file passes to `Module(...)` is an Inline module, and an `addExtern...` entry point whose
  name does not contain `Inline` in every other module.** A bind is a C++ function registered
  into a module with an `addExtern...` or `addInterop` entry point. The Inline modules are `$`
  (builtin), `math`, `strings` and `jit`. A plain-value bind returns nothing at all, or a value
  that is neither a reference nor a result the callee writes into the caller's result slot; it
  is not an interop bind (`addInterop`), and not a generic container or equality helper
  (`das_vector_*`, `das_equ*`, `das_nequ*`, `das_handle_equ*`, `das_handle_nequ*`).

- **Weakening `review_nttp.das`'s bind-flavor scan, which `REVIEW.das` runs, is a defect** -
  fix a bind the scan reports by switching the bind, and never drop a module from
  `review_nttp.das`'s `require` list, which sets the modules the scan covers.

- **A diff that adds a module under this folder adds it to `review_nttp.das`'s `require`
  list, in the same change** - a module off the list is a module the scan never sees.

- **A diff that adds a streamed field to any type `module_builtin_ast_serialize.cpp`
  serializes bumps the version `getVersion()` returns in
  `include/daScript/ast/ast_serializer.h`, in the same change** - a reader accepts a stream
  only when its stored version equals `getVersion()`, so without the bump an older cache
  passes that check and every field after the new one decodes shifted.

- **Weakening the default-cache silence check in
  `tests/module_cache/test_default_cache_path.das` (repo root) is a defect** - it pins that a
  run on the default cache path prints nothing, cold or warm, and that an explicit
  `-module-cache` still prints its verdict.

- **A diff that adds a diagnostic to a module-cache read - `trySerializeProgramModule`
  (`src/ast/ast_parse.cpp`, repo root) or the module cumulative-hash check in
  `module_builtin_ast_serialize.cpp` - prints it only when the serializer's `quietCache` is
  false** - the default cache is on for every run, so an ungated line becomes output every
  user sees.
