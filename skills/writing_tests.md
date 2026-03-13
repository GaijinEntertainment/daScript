# Testing Conventions (dastest)

Tests use the `dastest` framework. Test files live in `tests/` with per-module subfolders.

## Test file structure

```das
options gen2
require dastest/testing_boost public
require daslib/module_under_test

[test]
def test_something(t : T?) {
    t |> run("description") @(t : T?) {
        t |> equal(actual, expected)
        t |> success(condition)
    }
}
```

## Key test functions

- `t |> equal(actual, expected)` — value equality assertion (reports "values differ" with expected/got)
- `t |> success(condition)` — boolean assertion (reports "expected success, got failure")
- `t |> success(condition, "message")` — boolean assertion with custom message
- `t |> failure("message")` — unconditional failure with message
- `t |> run("name") @(t : T?) { ... }` — named subtest
- `t |> strictEqual(actual, expected)` — strict equality assertion (fatal on fail)
- `t |> numericEqual(actual, expected)` — numeric equality handling NaN

## Use `feint` instead of `print` in tests

**Always use `feint` instead of `print`** in test code, unless the test is specifically verifying print/logging behavior.

`feint` has the same signature and side-effect annotations as `print` (`SideEffects::modifyExternal`), so the compiler will not optimize it out. However, it produces no output, keeping test runs clean.

```das
// WRONG — produces noise in test output
print("x = {x}\n")

// RIGHT — same effect for testing, no output
feint("x = {x}\n")
```

**When to use `print`:** Only when the test specifically validates logging, output redirection, or `context.to_out` behavior.

## NEVER use `assert` / `verify` in test files

**Do NOT use `assert(...)` or `verify(...)` in `[test]` functions or their helpers.**
Always use the dastest API (`t |> equal(...)`, `t |> success(...)`, `t |> failure("...")`) instead.

- `assert(a == b)` → `t |> equal(a, b)`
- `verify(a == b)` → `t |> equal(a, b)`
- `assert(condition)` → `t |> success(condition)`
- `assert(condition, "msg")` → `t |> success(condition, "msg")`
- `assert(false)` (guard) → `t |> failure("reason")`

**Why?** `assert`/`verify` crash the process on failure, giving no detailed report.
The dastest API records failures with file/line info and continues running other tests.

**Exception:** Files under `tests/` that use `expect` directives (compilation-failure tests)
are designed to trigger compiler errors — these do NOT use `[test]` or `t : T?` at all.

## Threading `t : T?` through helpers

When a `[test]` function calls helper functions that need assertions:

1. Add `t : T?` as the **first** parameter to the helper
2. Use `t |> equal(...)` / `t |> success(...)` inside the helper
3. Pass `t` from the test function: `my_helper(t, other_args)`

Avoid naming local variables `t` in helpers — it shadows the test object parameter.
Use `ii`, `idx`, `val`, `sptr` etc. instead.

## Common test options

- `options no_unused_function_arguments = false` — suppress warnings for test params
- `options no_unused_block_arguments = false` — suppress warnings for block params
- Shared test helpers go in `_common.das` module files (e.g., `tests/linq/_common.das`)

## Running tests

- Single file: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/linq/test_linq_aggregation.das`
- Directory: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/linq/`
- All tests: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/`

## Testing non-copyable types

When testing operations on types containing non-copyable fields (`array<T>`, `table<K;V>`):

- **Use `emplace`** to add structs with non-copyable fields to arrays: `arr |> emplace <| make_value()` — NOT `arr |> push <| make_value()` (push copies, which fails for non-copyable types)
- **Verify source preservation**: after `push(container, named_var)`, assert the source variable is still intact — if the implementation uses `emplace` internally instead of `push_clone`, the source will be destroyed
- **Test non-copyable fields explicitly**: include at least one struct with an `array<T>` or `table<K;V>` field to catch accidental copies
- **Helper functions**: write `make_xxx()` factory helpers that construct test values — keeps test code concise and avoids repetition

## Test index (`tests/README.md`)

The file `tests/README.md` is a comprehensive index of every `.das` test file. **When you add, remove, or rename a test file, update `tests/README.md`** to keep it in sync:

- Add a row to the correct directory table with the filename, a short description, and `**expect** <codes>` if the file uses `expect` directives.

## AOT tests (`tests/aot/`)

Tests under `tests/aot/` are compiled ahead-of-time into the `test_aot` binary. They verify that daslang functions compile to C++ correctly and that AOT stubs link at runtime.

- **Adding AOT tests**: see `skills/aot_testing.md` for full details
- **Running**: `bin/Release/test_aot.exe dastest/dastest.das -- --test tests/aot`
- AOT tests also run under the regular interpreter: `bin/Release/daslang.exe dastest/dastest.das -- --test tests/aot`
- **Semantic hash pitfall**: avoid using `aot_enabled()` or `is_in_aot()` in assertion logic — these are constant-folded differently at AOT generation time vs runtime, causing hash mismatches (error 50101)
- If creating a new subdirectory, add a new section header and table.
- Mark helper/module files (non-test) with *(helper)*.

## IMPORTANT: Register ALL test directories in CMake for AOT

**Every test directory under `tests/` must be registered in `tests/aot/CMakeLists.txt`** so that the `test_aot` binary generates and links AOT stubs for those tests. If a test file is not registered, `test_aot` will fail with `error[50101]: AOT link failed` when it encounters functions without AOT stubs.

When adding a new test directory (e.g., `tests/foo/`), follow the pattern in `tests/aot/CMakeLists.txt`:

1. Add a `FILE(GLOB ...)` to collect test files:
   ```cmake
   FILE(GLOB AOT_FOO_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/foo/*.das")
   ```
2. Add a custom target and `DAS_AOT` call:
   ```cmake
   add_custom_target(test_aot_foo)
   SET(FOO_AOT_GENERATED_SRC)
   DAS_AOT("${AOT_FOO_FILES}" FOO_AOT_GENERATED_SRC test_aot_foo daslang)
   ```
3. Add `SOURCE_GROUP_FILES`:
   ```cmake
   SOURCE_GROUP_FILES("aot generated" FOO_AOT_GENERATED_SRC)
   ```
4. Add `${FOO_AOT_GENERATED_SRC}` to the `add_executable(test_aot ...)` source list
5. Add `test_aot_foo` to the `ADD_DEPENDENCIES(test_aot ...)` list

**Do NOT use `options no_aot`** to work around AOT link failures — fix the root cause by ensuring the test is registered in the AOT build.

## Test-first bug verification

When fixing bugs, write the failing test BEFORE applying the fix:

1. Write a test that demonstrates the expected behavior
2. Run it — confirm it FAILS (this proves the bug exists and the test is valid)
3. Apply the fix
4. Run the test again — confirm it PASSES
5. Run the full test suite to ensure no regressions

This ensures the test actually catches the bug and isn't accidentally passing due to a tautology.
