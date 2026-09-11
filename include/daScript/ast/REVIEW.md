# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root).

- **A diff that changes what a cached JIT DLL binds - the module a bind registers into or the
  name it registers under (`vectorHomeModule`, `typeFactory<vector<TT>>::make`,
  `registerVectorFunctions`, the name a `ManagedVectorAnnotation` or
  `ManagedStructureAnnotation` takes - all in `ast_handle.h`), or the field layout of a C++ type
  das code reads through a binding - bumps `LLVM_JIT_CODEGEN_VERSION` in
  `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in the same change.** The key is built
  from the codegen version and each function's AST hash, never from the module an extern lives
  in nor a bound type's offsets, so a cached DLL binds the old name or the old offset and
  crashes on the hit.

- **A diff that adds a field to `Function` or `Variable` (`ast.h`) holding something one
  program's compile decides - whether the program uses it, the slot it holds in that program's
  context, whether its JIT selected it - is a defect; the value goes into `Program`'s symbol
  tables (`usedFunctions`, `functionIndices` and their variable twins, `jitSelected`)
  instead.** A shared module's objects are one
  instance for every program in the process, and a compile that runs inside another - a macro's
  `compile`, a late `require`, the folding program - would otherwise overwrite the outer
  program's answer mid-simulate. The per-compile fields that remain, and why each is tolerated,
  are `src/ast/ARCHITECTURE.md` sec.4's.

- **In a C++ type with `addField` in its annotation (`Program` in
  `src/builtin/module_builtin_rtti.cpp`), a member whose size differs between the standard
  libraries the repo's targets use (`mutex`, `std::function`, `condition_variable`) is declared
  after the last such field.** A cross-compiled exe bakes the host's field offsets into the
  code it generates, and the target's standard library sizes such a member differently (a
  `std::function` is 48 bytes under Linux libc++ and 32 under emscripten's), so every
  das-visible field behind one is read at the wrong address.
