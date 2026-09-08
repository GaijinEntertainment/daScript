# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root).

- **A diff that changes which module a bind registers into, or the name it registers under -
  `vectorHomeModule`, `typeFactory<vector<TT>>::make`, `registerVectorFunctions`, the name a
  `ManagedVectorAnnotation` or `ManagedStructureAnnotation` takes (`ast_handle.h`) - bumps
  `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in the
  same change.** The JIT's DLL cache key folds the codegen version and each function's AST hash,
  never the module an extern lives in, so a cached DLL binds the old name and crashes on the hit.

- **A property one program's compile decides about a `Function` or a `Variable` - whether the
  program uses it, the slot it holds in that program's context - never lands as a field on the
  object; it goes into `Program`'s symbol tables (`usedFunctions`, `functionIndices` and their
  variable twins, `ast.h`).** A shared module's objects are one instance for every program in the
  process, and a compile that runs inside another - a macro's `compile`, a late `require`, the
  folding program - would otherwise overwrite the outer program's answer mid-simulate.
