# Testing Conventions (dastest)

Tests use the `dastest` framework. Test files live in `tests/` with per-module subfolders.

## Test file structure

```das
options gen2
require dastest/testing_boost public
require daslib/module_under_test

[test]
def test_something(t : T?) {
    t |> run("description") <| @(t : T?) {
        t |> equal(actual, expected)
        t |> success()
    }
}
```

## Key test functions

- `t |> equal(actual, expected)` — value equality assertion
- `t |> success()` — mark subtest as passed
- `t |> run("name") <| @(t : T?) { ... }` — named subtest
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
