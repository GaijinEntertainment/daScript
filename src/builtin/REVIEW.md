# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `review_nttp.das`'s bind-flavor scan, which `REVIEW.das` runs, is a defect - a bind
  the scan reports is rebound with the other call: `addExtern` becomes `addExternInline` in an
  Inline module, and `addExternInline` becomes `addExtern` in any other module.** A bind's flavor
  is which of the two calls registers it; an Inline module is one of the modules the next sentence
  lists. The Inline modules are `$` (builtin), `math`, `strings` and `jit`. Which
  shared generic helpers the scan exempts is read from the scan itself.

- **A diff that adds or changes a bind - any `addExtern*` call - under this folder states in the
  PR description that the folder's gate ran on a binary built from the diff** - the scan reads
  the binds compiled into the running binary, so a stale binary is a false green.

- **A diff that adds a module under this folder adds it to `review_nttp.das`'s `require` list in
  the same change - directly, or through the daslib wrapper that requires it - and a diff never
  drops a module from that list: it keeps the `require`, or removes the module itself.** The list
  sets the modules the scan covers.

- **A diff after which a module-cache record an older build wrote no longer matches what this
  build writes bumps `getVersion()` (`include/daScript/ast/ast_serializer.h`) in the same change -
  a field added, removed, reordered or re-typed in `module_builtin_ast_serialize.cpp`, an encoding
  changed there, or a streamed annotation, function or type moved to another module.** A reader
  discards a record only on a version mismatch, an explicit `-module-cache <path>` key included. A
  C++ layout change to a handled type or an AST class is not a record byte.

- **A diff that streams or compares a `CodeOfPolicies` field in `module_builtin_ast_serialize.cpp`
  outside `DAS_MODULE_CACHE_POLICY_FIELDS` is a defect - put the field on the list instead** - the
  list drives both the record's policy stream and the compare that refuses a record written under
  other policies, so a field handled outside it is written without being compared, or compared
  without being written.

- **A diff that adds to `AstSerializer::serializeProgram`, `AstSerializer::serializeProgramImpl`
  or `ModuleFileCache` (`module_builtin_ast_serialize.cpp`) a diagnostic the default build
  compiles, without the serializer's `quietCache` gate - or drops that gate from one already
  there - is a defect** - the module cache is on by default, so an ungated line prints on an
  ordinary run.

- **A diff that adds a builtin a `.das_module` descriptor can call whose effect outlives the
  descriptor's own program - a row in a process-wide registration table a warm start must
  reproduce without running the descriptor - records the call between
  `begin_dynamic_module_recording` and `end_dynamic_module_recording` (`module_builtin_fio.cpp`)
  as a row `read_manifest` reads back and `init_dyn_modules` replays (`src/ast/dyn_modules.cpp`),
  in the same change.** A replayed start never runs the descriptor, so an effect the recorder
  does not see is an effect every warm start silently lacks.

- **A diff that moves a bind between modules - any `addExtern*` call whose module changes, or a
  builtin whose `vector<T>` functions follow a type to another module - bumps
  `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in the
  same change.** The JIT's DLL cache key folds the codegen version and each function's AST
  hash, never the module an extern lives in, so a cached DLL binds the old name and crashes on
  the hit.

- **A diff that changes how an `[extern]` call reaches its bind - the wrapper tables, the
  `systemV_extra` list, the arm64 layout, the `das_arm64_call` trampoline, or when and from
  what the `__dasbind__` proxy is registered and the call retargeted - updates
  `ARCHITECTURE.md#interpreter-extern-call` in the same change.** Comments in `module_builtin_dasbind.cpp` cite that
  section instead of restating it, so a stale section is what the next reader trusts.

- **A diff that changes what `ModuleFileCache::defaultPath` or `ModuleFileCache::embeddedHostOptions`
  folds into the module-cache key - the binary, the command line, the environment names, or
  which script arguments count - updates `ARCHITECTURE.md#default-module-cache-path` in the
  same change.** The key is what stops a native-compiled module serving a cross compile, so a
  wrong description of it gets trusted.

- **A diff that adds a `std::filesystem` call in `module_builtin_fio.cpp` passes every path into
  it through `das_to_path` and every path out of it through `path_to_das`.** On Windows a path
  built from a narrow string decodes through the ANSI codepage, so a UTF-8 name the codepage
  cannot represent is misread on the way in and throws on the way out.

- **A diff that changes a C++ type das binds through an annotation with `addField` in this folder
  and leaves a member whose type is a standard-library class other than `std::string`, or a
  platform struct embedded by value, before the last das-visible field is a defect - declare that
  member after that field.** A cross-compiled exe bakes the host's field offsets into the code it
  generates, and the target's standard library sizes such a member differently, so every
  das-visible field behind one is read at the wrong address.

- **A diff that adds a member whose type is a standard-library class other than `std::string`, or
  a platform struct embedded by value, to a type das holds by value (`isLocal`, `canCopy` or
  `canMove` true in its annotation), or makes a type holding one by-value, is a defect - build the
  type from fixed-width scalars and `std::string`, and copy what das needs out of a platform struct
  instead of embedding one.** The exe bakes the host's `sizeof` for such a type, so a target that
  sizes an embedded member differently gives every das local of it the wrong length.

- **A diff that changes the data-member order or data-member set of a C++ type das binds
  through an annotation states its `--jit-check-abi` result for a cross target in its own PR
  description.** The check reports a mismatch at the bundle's first launch, for the types the
  bundle links; nothing native can observe one.

- **A `string` streamed in `module_builtin_ast_serialize.cpp` whose storage does not outlive the
  module-cache record being written or read - a computed mangled name, a lookup key, a container
  element, a field of a local - goes through `serializeTemp`, never plain `operator<<`.** The
  string table holds pointers into the caller's own bytes, so an entry left pointing at a dead
  local is what every later mention of that string writes or reads.

- **A diff that adds a per-record table or vector the serializer numbers into
  (`module_builtin_ast_serialize.cpp`) clears it in `AstSerializer::clearNodeIds`, in the same
  change.** A later compile reuses a freed address, so a table that outlives its record answers a
  new node with the old node's number or name.

- **A diff that reads a writing `SerializationStorageVector`'s `buffer` outside
  `AstSerializer::serializeProgram` (`module_builtin_ast_serialize.cpp`) - to hand the bytes out,
  hash them, or write them - calls `flush()` first.** The writer grows the vector by doubling and
  counts the bytes in `writePos`, so before a flush the vector is longer than the stream.
