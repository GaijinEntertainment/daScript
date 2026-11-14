# AST Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `src/ast/ARCHITECTURE.md` (repo root).

**A diff that changes a node field or a flag bit declared under this folder applies
`src/REVIEW.md` (repo root) too.**

**A diff to `ast_handle.h` that changes the home module or the das-visible name the header itself
gives an already-bound C++ type or function - in `typeName<vector<TT>>::name`, `vectorHomeModule`,
`typeFactory<vector<TT>>::make` or `registerVectorFunctions` - bumps `LLVM_JIT_CODEGEN_VERSION` in
`modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root), in the same change
(`ARCHITECTURE.md#jit-dll-cache-key`).** The JIT's DLL cache key leaves the name out, so
without the bump a cached DLL keeps the old name and crashes. The name string a
`ManagedVectorAnnotation` or `ManagedStructureAnnotation` constructor takes is chosen where the
annotation is created, so that rename answers to the creating folder's checklist.

**A diff to a C++ type declared under this folder that some `Managed*Annotation` is instantiated
over - or to a base of that type, or to a struct it holds by value - that changes the type's
memory layout (its size, a member's offset or a member's type) bumps `LLVM_JIT_CODEGEN_VERSION` in
`modules/dasLLVM/daslib/llvm_jit_plan.das` (repo root) and applies `src/builtin/REVIEW.md` (repo
root) too, in the same change (`ARCHITECTURE.md#jit-dll-cache-key`).** The das-visible
name can differ, so check the annotation's template argument, not the name string.

**A diff that adds a virtual function overriding no base virtual, or removes, reorders or
re-signs a virtual function, in any class declared under this folder bumps `DAS_ABI_VERSION`
(`ast.h`) and applies `skills/internal/abi_break_sweep.md` (repo root), in the same change.** A
slot added, moved or re-signed in an existing vtable changes no class size, so only that counter
makes the loader refuse a shared module built against the old vtable, and an out-of-tree
subclass that omits `override` keeps compiling and silently stops being called.

**A diff that adds, removes, reorders or retypes a data member of a class declared under this
folder whose `sizeof` is listed in `das_abi_vintage` (`ast.h`), of a base of that class, or of
a struct declared under this folder that the class holds by value, bumps `DAS_ABI_VERSION`
(`ast.h`) in the same change.** A member reordered, retyped, or added into padding moves no
size, so only that counter makes the loader refuse the stale shared module.

**A diff that adds a field to `Function` or `Variable` (`ast.h`) whose value one compiled
program decides, so two programs sharing the module would want different values, is a defect;
the value goes into a per-program table on `Program` instead.** A shared module's objects are
one instance for every program in the process, and a compile that runs inside another would
otherwise overwrite the outer program's answer before the outer program has finished reading
it. `src/ast/ARCHITECTURE.md#program-scoped-symbol-state` lists the per-compile fields
`Function` and `Variable` carry and why each is safe.
