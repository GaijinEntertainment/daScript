# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root). A diff that adds, reorders or retypes a data member of a
C++ type that das binds through an annotation applies
`src/builtin/REVIEW.md` too; checklist discovery walks changed paths only, so a header edit
never opens that checklist on its own.

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