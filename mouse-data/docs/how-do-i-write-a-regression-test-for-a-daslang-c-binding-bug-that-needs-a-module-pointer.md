---
slug: how-do-i-write-a-regression-test-for-a-daslang-c-binding-bug-that-needs-a-module-pointer
title: How do I write a regression test for a daslang C++-binding bug that needs a Module pointer?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Use a `[call_macro]` as the bridge. The macro receives `mod : Module?` in `visit`, so you can call any `ast`-module C++ binding (`find_call_macro`, `find_unique_function_ptr`, `module_find_structure`, …) directly at compile time. Fold the pass/fail outcome to an `ExprConstBool` literal that a regular `[test]` then asserts on with `t |> success(...)`.

**Recipe — two files:**

`_helper.das` (leading underscore so dastest's file walker skips it — see `feedback_dastest_underscore_skip`):

```daslang
options gen2
options indenting = 4

module _my_helper public

require daslib/ast public
require daslib/ast_boost
require daslib/templates_boost

[call_macro(name = "probe_some_binding")]
class private ProbeSomeBinding : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        // Drive the C++ binding under test. mod is the current module.
        let result = find_call_macro(mod, "definitely_not_registered_xyz")
        let ok = result == null  // pre-fix: would crash; post-fix: null
        return new ExprConstBool(at = call.at, value = ok)
    }
}
```

`test_thing.das`:

```daslang
options gen2
options indenting = 4

require dastest/testing_boost public
require _my_helper

[test]
def test_binding_does_not_crash(t : T?) {
    t |> success(probe_some_binding(),
                 "find_call_macro on missing name should return null, not crash")
}
```

**Why this works:**

- `[call_macro]` runs at compile time during the visit phase, with full access to `prog : ProgramPtr` and `mod : Module?` — the same pointers the C++ binding wants.
- Returning a `new ExprConstBool(...)` makes the macro fold to a literal — the `[test]` becomes a plain runtime call that doesn't carry any compile-time machinery.
- The macro lives in a `public` helper module so the test file can `require` it; the `_` prefix on the filename keeps dastest from trying to run it as a test in its own right.

**Verifying the test catches the bug:** revert the C++ fix, kill all `daslang.exe` processes (`Get-Process daslang | Stop-Process -Force`), `cmake --build build --config Release --target daslang`, then rerun. If the test passes against the unfixed binary, the test is too weak.

**Alternatives considered:**

- `ast_gc_guard() { ... }` from a regular `[test]` body works for AST construction (`new ExprFoo(...)`, `qmacro(...)`) but doesn't give you a `Module?` — you'd have to `find_module(prog, "name")` and you don't have a `prog` outside a macro. Use this only when the binding doesn't need a module/program pointer.
- A `[function_macro]` (apply-time) also has `prog` access and could work, but `[call_macro]` is simpler because the call site IS the trigger — no separate annotation needed.

**Surfaced 2026-05-09** while writing the regression test for the `find_call_macro` null-deref fix; pattern reused from `tests/ast_match/_qmatch_r2v_helper.das`.

## Questions
- How do I write a regression test for a daslang C++-binding bug that needs a Module pointer?
- How do I drive an ast C++ binding from a [test] function?
- What's the pattern for testing find_call_macro / find_unique_function_ptr / module_find_structure from a daslang test?
