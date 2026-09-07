# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `review_nttp.das`'s bind-flavor scan, which `REVIEW.das` runs, is a defect - fix a
  bind the scan reports by switching the bind.** The Inline modules are `$` (builtin), `math`,
  `strings` and `jit`. In those, a plain-value bind - one returning nothing, or a value that is
  neither a reference nor written into the caller's result slot - registers through
  `addExternInline` or `addExternInlineEx`; in every other module it registers through
  `addExtern`.

- **A diff that adds or changes a bind in a module the scan covers rebuilds the binary from
  that diff before the folder's gate runs** - the scan reads the binds compiled into the running
  binary, so a stale binary is a false green.

- **A diff that adds a module under this folder adds it to `review_nttp.das`'s `require` list in
  the same change - directly, or through the daslib wrapper that requires it - and never drops a
  module from the list.** A module the list does not reach is a module the scan never sees.

- **A diff that changes what `module_builtin_ast_serialize.cpp` streams - a field added,
  removed, reordered, re-typed, or given a new meaning - bumps the version `getVersion()`
  returns in `include/daScript/ast/ast_serializer.h`, in the same change** - a reader accepts a
  stream only when its stored version equals `getVersion()`, so without the bump an older cache
  passes that check and decodes the changed bytes as something else.

- **A diff that streams or compares a `CodeOfPolicies` field in `module_builtin_ast_serialize.cpp`
  outside `DAS_MODULE_CACHE_POLICY_FIELDS` is a defect - put the field on the list instead** - the
  list drives both the record's policy stream and the compare that refuses a record written under
  other policies, so a field handled outside it is written without being compared, or compared
  without being written.

- **A diff that adds a diagnostic to `AstSerializer::serializeProgram` or
  `AstSerializer::serializeProgramImpl` in `module_builtin_ast_serialize.cpp`, or drops the
  `quietCache` gate from one already there, is a defect - gate every line those two functions
  print on the serializer's `quietCache`** - the default cache is on unasked for an ordinary run,
  so an ungated line becomes output every user sees.

- **A diff that adds a builtin a `.das_module` descriptor can call to change the require
  resolver or the module registry records the call between `begin_dynamic_module_recording`
  and `end_dynamic_module_recording` (`module_builtin_fio.cpp`) as a row `read_manifest`
  reads back and `init_dyn_modules` replays (`src/ast/dyn_modules.cpp`), in the same change.**
  A replayed start never runs the descriptor, so an effect the recorder does not see is an
  effect every warm start silently lacks.

- **A diff that changes what `ModuleFileCache::defaultPath` folds into the module-cache key -
  the binary, the command line, the environment names, or which script arguments count - updates
  the cache-key paragraph of `ARCHITECTURE.md` in the same change.** The key is what stops a
  native-compiled module serving a cross compile, so a wrong description of it gets trusted.
