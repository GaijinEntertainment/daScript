# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `review_nttp.das`'s bind-flavor scan, which `REVIEW.das` runs, is a defect - fix a
  bind the scan reports by switching the bind.** The Inline modules are `$` (builtin), `math`,
  `strings` and `jit`. What the scan enforces, and which shared generic helpers it exempts, is
  read from the scan itself.

- **A diff that adds or changes a bind in a module the scan covers rebuilds the binary from
  that diff before the folder's gate runs** - the scan reads the binds compiled into the running
  binary, so a stale binary is a false green.

- **`review_nttp.das`'s `require` list gains a row in the change that adds a module under this
  folder - directly, or through the daslib wrapper that requires it - and loses one only in the
  change that removes the module.** The list is what sets the modules the scan covers: a module
  the list does not reach is a module the scan never sees.

- **A diff that changes the bytes a module-cache record carries or what they resolve to - a
  field added, removed, reordered, re-typed or given a new meaning, wherever the edit lives, or
  a change anywhere to which module owns a streamed annotation, function or type - bumps the
  version `getVersion()` returns in `include/daScript/ast/ast_serializer.h`, in the same
  change** - a reader accepts a stream only when its stored version equals `getVersion()`, so
  without the bump an older cache passes that check and decodes the changed bytes as something
  else.

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

- **A diff that adds a builtin a `.das_module` descriptor can call whose effect outlives the
  descriptor's own program - a row in a process-wide registration table a warm start must
  reproduce without running the descriptor - records the call between
  `begin_dynamic_module_recording` and `end_dynamic_module_recording` (`module_builtin_fio.cpp`)
  as a row `read_manifest` reads back and `init_dyn_modules` replays (`src/ast/dyn_modules.cpp`),
  in the same change.** A replayed start never runs the descriptor, so an effect the recorder
  does not see is an effect every warm start silently lacks.

- **A diff that moves a bind between modules - an `addExtern` or `addExternInline` call whose
  module changes, or a builtin whose `vector<T>` functions follow a type to another module -
  bumps `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in
  the same change.** The JIT's DLL cache key folds the codegen version and each function's AST
  hash, never the module an extern lives in, so a cached DLL binds the old name and crashes on
  the hit.

- **A diff that changes what `ModuleFileCache::defaultPath` folds into the module-cache key -
  the binary, the command line, the environment names, or which script arguments count - updates
  the cache-key paragraph of `ARCHITECTURE.md` in the same change.** The key is what stops a
  native-compiled module serving a cross compile, so a wrong description of it gets trusted.
