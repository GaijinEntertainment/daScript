---
slug: dastest-failed-cant-invalid-prefix-expected-compile-failure-tests
title: How does dastest treat `failed_*`, `cant_*`, `invalid_*` test files?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Files in `tests/` whose basename starts with `failed_`, `cant_`, or `invalid_` are expected-compile-failure tests.** They declare which compile errors must fire, not what runtime behavior to test.

**dastest skip:** `dastest/dastest.das:160-166` (inside `serializeAst`):

```daslang
let base = base_name(file)
if (base |> starts_with("cant_") || base |> starts_with("failed_") || base |> starts_with("invalid_")) {
    continue   // skip AST serialization — they don't compile
}
```

**CMake skip:** Most `tests/<area>/CMakeLists.txt` GLOB filters exclude them from AOT compilation:
```cmake
list(FILTER AOT_FOO_FILES EXCLUDE REGEX "failed_")
```
because trying to AOT-compile a file that intentionally fails to compile would just break the build.

**The `expect` line** sits at the top of the file (after `options gen2`):

```daslang
options gen2
expect 30106:3, 30146, 30148, 30152, 30240, 30282
```

- Comma-separated list of expected error codes.
- `code:N` syntax = "exactly N occurrences" (default 1). Above: three 30106 errors plus one each of the rest.
- Order doesn't matter; line numbers don't matter; what matters is that the compiler emits exactly those codes those many times.
- PASS when the actual error set matches the expect set.

**Run via MCP:**

```
mcp__daslang__run_test tests/language/failed_reserved_names.das
```

emits the compile errors AND reports `PASS` (1 test, 1 passed) when codes match.

**Maintenance when you relax or remove a compile-time guard:**

1. If the now-passing case lives inside a `failed_*` file, delete that case from the file.
2. Drop the corresponding code from the `expect` line.
3. (Ideal) add a positive test elsewhere that exercises the now-legal form, ideally with AOT round-trip if codegen could break.

Example (2026-05-14): when the function-name C++-keyword guard was dropped, `def do(a : int)` moved from `tests/language/failed_reserved_names.das` (with `30163` dropped from expect) to a new `tests/aot/test_cpp_keyword_names.das` as a positive case (exercising interpreter + AOT + JIT).

The prefix is just convention so dastest and CMake know to skip them. `invalid_*` and `cant_*` behave identically to `failed_*`.

## Questions
- What's the convention for daslang tests that expect a compile error?
- How does the `expect` line work in dastest?
- Why do `failed_*.das` files in tests/ not get AOT-compiled?
- What does `expect 30106:3` mean in a daslang test file?
- Where in dastest are expected-failure tests skipped from AST serialization?
- How do I write a test that asserts the compiler emits a specific error code?
- What should I do with a `failed_*` test after I relax the guard it was checking?
