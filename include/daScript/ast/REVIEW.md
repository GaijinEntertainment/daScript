# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root). Apply `src/builtin/REVIEW.md` too when a diff changes the
memory layout of a C++ type some `Managed*Annotation` is instantiated over. Checklist discovery
walks changed paths only, so a header edit never opens that checklist on its own.

- **A diff that changes the module an existing bind registers into, or the name it registers
  under (`vectorHomeModule`, `typeFactory<vector<TT>>::make`, `registerVectorFunctions`, the name
  a `ManagedVectorAnnotation` or `ManagedStructureAnnotation` takes - all in `ast_handle.h`),
  bumps `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in
  the same change.** The JIT's DLL cache key folds the codegen version and each function's AST
  hash, never the module an extern lives in, so a cache hit binds the old name and crashes.

- **A diff that changes the memory layout of a C++ type declared under this folder that some
  `Managed*Annotation` is instantiated over - a member added, removed, reordered or retyped in
  the type, in a base of it, or in a struct it holds by value, and an added base class, first
  virtual function, `alignas` or packing change, all count - bumps `LLVM_JIT_CODEGEN_VERSION` in
  `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in the same change.** The das-visible
  name can differ, so check the annotation's template argument, not the name string. The JIT's
  DLL cache key folds the codegen version and each function's AST hash, never a bound type's
  offsets, so a cache hit binds the old offsets and crashes.

- **A diff that adds a field to `Function` or `Variable` (`ast.h`) holding something one
  program's compile decides - whether the program uses it, the slot it holds in that program's
  context, whether this program's JIT selected it - is a defect; the value goes into `Program`'s
  symbol tables (`usedFunctions`, `functionIndices`, `usedVariables`, `variableIndices`,
  `jitSelected`) instead.** A shared module's objects are one instance for every program in the
  process, and a compile that runs inside another - a macro's `compile`, a late `require`, the
  folding program - would otherwise overwrite the outer program's answer mid-simulate.
  `src/ast/ARCHITECTURE.md` sec.4 lists the per-compile fields that remain and why each is
  tolerated.
