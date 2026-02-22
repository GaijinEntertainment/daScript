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
        t |> success()
    }
}
```

## Key test functions

- `t |> equal(actual, expected)` — value equality assertion
- `t |> success()` — mark subtest as passed
- `t |> run("name") @(t : T?) { ... }` — named subtest
- `t |> equal(actual, true)` / `t |> equal(actual, false)` — boolean assertions (there is no `expect_true`/`expect_false`)
- `t |> strictEqual(actual, expected)` — strict equality assertion

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

## Test-first bug verification

When fixing bugs, write the failing test BEFORE applying the fix:

1. Write a test that demonstrates the expected behavior
2. Run it — confirm it FAILS (this proves the bug exists and the test is valid)
3. Apply the fix
4. Run the test again — confirm it PASSES
5. Run the full test suite to ensure no regressions

This ensures the test actually catches the bug and isn't accidentally passing due to a tautology.
