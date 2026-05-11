---
slug: why-does-cop-jit-enabled-true-on-an-inner-compile-file-call-not-actually-jit-what-s-missing-besides-the-policy
title: Why does cop.jit_enabled = true on an inner compile_file call not actually JIT — what's missing besides the policy?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

The `cop.jit_enabled` policy flag is necessary but not sufficient. The actual JIT pass is provided by the `[simulate_macro(name="jit_llvm")]` class `JIT_LLVM` in `modules/dasLLVM/daslib/llvm_macro.das`, which is only registered when the `just_in_time` module is loaded. The top-level daslang `-jit` flag does this in `utils/daScript/main.cpp:262`:

```cpp
access->addExtraModule("just_in_time", getDasRoot() + "/daslib/just_in_time.das");
```

For host programs that dynamically compile other `.das` files via `compile_file` (rtti), you have to repeat this on the inner `FileAccess`:

```das
cop.jit_enabled = jit_enabled()  // propagate from outer policy
if (jit_enabled()) {
    access |> add_extra_module("just_in_time", "{get_das_root()}/daslib/just_in_time.das")
}
compile_file(fileName, access, unsafe(addr(mg)), cop) <| $(ok, program, issues) { ... }
```

Without `add_extra_module`, the inner program compiles cleanly with `jit_enabled=true` in its policies, simulate succeeds, but no `AstSimulateMacro` ever runs the LLVM codegen pass — so every function executes in the interpreter. The symptom is "DAS JIT" and "DAS INTERPRETER" timings being identical, and `is_jit_function(@@foo)` returning false inside the inner program.

Canonical patterns to copy: `dastest/suite.das:167-169`, `tests/jit_tests/jit_exe.das:65`, `tests/jit_tests/dll_cache.das:41`. (`utils/dascov/main.das:68` is the same pattern for `coverage` rather than JIT.)

The `add_extra_module` builtin lives in `rtti_core` (declared as `rtti_add_extra_module` in `src/builtin/module_builtin_rtti.cpp`). `require daslib/rtti` brings it into scope.

## Questions
- Why does cop.jit_enabled = true on an inner compile_file call not actually JIT — what's missing besides the policy?
