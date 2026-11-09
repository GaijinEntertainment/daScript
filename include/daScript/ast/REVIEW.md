# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root).

- **A diff that changes the module or the name `ast_handle.h` itself registers an existing C++
  binding under (`vectorHomeModule`, `typeFactory<vector<TT>>::make`, `registerVectorFunctions`,
  and any name those templates compute) bumps `LLVM_JIT_CODEGEN_VERSION` in
  `modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in the same change.** The JIT's DLL
  cache key folds the codegen version and each function's AST hash, never the module an extern
  lives in, so a cache hit binds the old name and crashes. The name string a
  `ManagedVectorAnnotation` or `ManagedStructureAnnotation` constructor takes is chosen where the
  annotation is created, so that rename answers to the creating folder's checklist.

- **A diff that changes the memory layout of a C++ type declared under this folder that some
  `Managed*Annotation` is instantiated over - a data member added, removed, reordered or retyped
  in the type, in a base of it, or in a struct it holds by value, and an added base class, first
  virtual function, `alignas` or packing change, all count; a changed signature on an existing
  virtual does not - bumps `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_plan.das`
  (repo root) in the same change, and applies `src/builtin/REVIEW.md` (repo root) too.** The
  das-visible name can differ, so check the annotation's template argument, not the name string.
  The JIT's DLL cache key folds the codegen version and each function's AST hash, never a bound
  type's offsets, so a cache hit binds the old offsets and crashes.

- **A diff that adds a virtual function overriding no base virtual, or removes, reorders or
  re-signs a virtual function, in any class declared under this folder bumps `DAS_ABI_VERSION`
  (`ast.h`) and applies `skills/internal/abi_break_sweep.md` (repo root), in the same change.** A
  slot added, moved or re-signed in an existing vtable changes no class size, so only that counter
  makes the loader refuse a shared module built against the old vtable, and an out-of-tree
  subclass that omits `override` keeps compiling and silently stops being called.

- **A diff that adds a field to `Function` or `Variable` (`ast.h`) whose value one compiled
  program decides, so two programs sharing the module would want different values, is a defect;
  the value goes into a per-program table on `Program` instead.** A shared module's objects are
  one instance for every program in the process, and a compile that runs inside another would
  otherwise overwrite the outer program's answer before the outer program has finished reading
  it. `src/ast/ARCHITECTURE.md#program-scoped-symbol-state` lists the per-compile fields
  `Function` and `Variable` carry and why each is safe.
