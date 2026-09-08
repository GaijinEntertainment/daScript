# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root).

- **A diff that changes which module a bind registers into, or the name it registers under -
  `vectorHomeModule`, `typeFactory<vector<TT>>::make`, `registerVectorFunctions`, the name a
  `ManagedVectorAnnotation` or `ManagedStructureAnnotation` takes (`ast_handle.h`) - bumps
  `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_run.das` (repo root), in the
  same change.** The JIT's DLL cache key folds the codegen version and each function's AST hash,
  never the module an extern lives in, so a cached DLL binds the old name and crashes on the hit.
