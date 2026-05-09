---
slug: why-does-find-call-macro-crash-with-exception-access-violation-when-called-with-a-name-that-isn-t-registered
title: Why does find_call_macro crash with EXCEPTION_ACCESS_VIOLATION when called with a name that isn't registered?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

**Fixed 2026-05-09.** Pre-fix, `findModuleCallMacro` in `src/builtin/module_builtin_ast_adapters.cpp` did:

```cpp
auto exprCallMacro = static_cast<ExprCallMacro*>((*module->findCall(name))(*at));
//                                                ^^^^^^^^^^^^^^^^^^^^^^^^^
//                                                Module::findCall returns nullptr
//                                                on miss; deref crashes.
```

Now it null-checks the factory and returns `nullptr` cleanly:

```cpp
auto factory = module->findCall(name);
if ( !factory ) return nullptr;
auto exprCallMacro = static_cast<ExprCallMacro*>((*factory)(*at));
```

So `find_call_macro(mod, "absent_name")` is now a safe soft probe — returns null if the name isn't registered. Regression test: `tests/macro_call/test_find_call_macro_null.das`.

**Surfaced 2026-05-09** during dasImgui Phase 0a `[widget]` macro work; useful for any function-annotation that wants to dedup or extend an existing call_macro (e.g. multi-overload `[widget] def slider(...)` cases).

## Questions
- Why does find_call_macro crash with EXCEPTION_ACCESS_VIOLATION when called with a name that isn't registered?
