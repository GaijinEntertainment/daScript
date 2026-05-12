---
slug: shared-module-cannot-require-native-path-module-error-20115
title: Why does daspkg release fail with error[20115] "Shared module X has incorrect dependency type. Can't require Y because its not shared"?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

A module registered as a **shared module** (via `register_dynamic_module("X.shared_module", "Module_X")` in `.das_module`) is a different dependency type from a module registered as a **native path** (via `register_native_path("namespace", "name", "...das")`). The standalone-exe build (`daspkg release`, which CI's `extended_checks` lane runs) forbids a shared module from `require`-ing a native-path module — only shared→shared.

Concrete failure: dasGlfw's `.das_module` registers `dasModuleGlfw.shared_module` as a shared module AND `register_native_path("live", "glfw_live", ...)` for the `.das` companion file. If `glfw_live.das` does `require live/something_else_native_path`, the dasModuleGlfw shared module's transitive require chain crosses the type boundary at release-exe build time:

```
error[20115]: Shared module glfw_live has incorrect dependency type.
Can't require something_else_native_path because its not shared
```

In-tree `dastest` and `test_aot` don't catch this — only `daspkg release` (which the CI matrix's `extended_checks (linux, 64)` / `(darwin15, arm64)` jobs run via `examples/games/sequence/ci_smoke_test.sh`) exercises the standalone exe path.

**Workarounds, by preference:**

1. **Inline the helper** back into the parent file. Cheapest if the helper is small.
2. **Make both shared.** Add another `register_dynamic_module` and a C++ Module subclass for the helper. Heavy plumbing for daslang-only logic.
3. **Don't register at all** — relative path require (`require ./helper.das` from within the same module file). Works inside the module dir but not from outside callers.

The wrong move is treating it as a require-path-naming problem; it's a dep-type-graph problem.

## Questions
- Why does daspkg release fail with error[20115] "Shared module X has incorrect dependency type. Can't require Y because its not shared"?
