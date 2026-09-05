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

- **A diff that changes what `module_builtin_ast_serialize.cpp` streams - a field added,
  removed, reordered, re-typed, or given a new meaning - bumps the version `getVersion()`
  returns in `include/daScript/ast/ast_serializer.h`, in the same change** - a reader accepts a
  stream only when its stored version equals `getVersion()`, so without the bump an older cache
  passes that check and decodes the changed bytes as something else.

- **A diff that adds a field to `CodeOfPolicies` (`include/daScript/simulate/code_of_policies.h`)
  adds it to `DAS_MODULE_CACHE_POLICY_FIELDS` in `module_builtin_ast_serialize.cpp`, in the same
  change** - the list is every field of the struct: it is both the record's policy stream and the
  compare that refuses a record written under other policies, and a field missing from it is a
  policy the cache silently ignores.

- **`module_builtin_ast_serialize.cpp` streams and compares the cached policies only through
  `DAS_MODULE_CACHE_POLICY_FIELDS`; a diff that names a policy field elsewhere in that file's
  stream or compare is a defect** - a field streamed outside the list is one the compare does not
  see, and the reverse.

- **Every diagnostic the module-cache read path in `module_builtin_ast_serialize.cpp` prints is
  gated on the serializer's `quietCache`; a diff that adds an ungated one is a defect** - the
  default cache is on unasked for an ordinary run, so an ungated line becomes output every user
  sees. The reader's other half, `trySerializeProgramModule`, answers to `src/ast/REVIEW.md`.
