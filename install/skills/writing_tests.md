# Testing Conventions (dastest)

The SDK ships the `dastest` test framework at `dastest/dastest.das`. Use it to write tests for your own daslang code — no extra dependencies.

## Running tests

```bash
# Single file
bin/daslang dastest/dastest.das -- --test path/to/test_foo.das

# A directory
bin/daslang dastest/dastest.das -- --test path/to/tests/

# All tests in your project
bin/daslang dastest/dastest.das -- --test tests/
```

The `--` separator splits daslang flags (left) from script args (right). Everything after `--` is forwarded to `dastest`.

## Test file structure

```das
options gen2
require dastest/testing_boost public
require my/module_under_test

[test]
def test_something(t : T?) {
    t |> run("description") @(t : T?) {
        t |> equal(actual, expected)
        t |> success(condition)
    }
}
```

## Key assertion functions

- `t |> equal(actual, expected)` — value equality (reports "values differ" with both sides)
- `t |> success(condition)` — boolean assertion (reports "expected success, got failure")
- `t |> success(condition, "message")` — boolean assertion with a custom message
- `t |> failure("message")` — unconditional failure
- `t |> run("name") @(t : T?) { ... }` — named subtest
- `t |> strictEqual(actual, expected)` — strict equality, fatal on fail
- `t |> numericEqual(actual, expected)` — numeric equality with NaN handling

## Use `feint` instead of `print` in tests

`feint` has the same signature and side-effect annotations as `print` (`SideEffects::modifyExternal`), so the compiler will not optimize it out — but it produces no output, keeping test runs clean.

```das
// WRONG — produces noise in test output
print("x = {x}\n")

// RIGHT — same effect for testing, no output
feint("x = {x}\n")
```

Use real `print` only when the test specifically validates logging or output redirection.

## NEVER use `assert` / `verify` in test files

Do NOT use `assert(...)` or `verify(...)` in `[test]` functions. They crash the process on failure with no detailed report. Use the dastest API instead:

| Don't write | Write instead |
|---|---|
| `assert(a == b)` | `t |> equal(a, b)` |
| `verify(a == b)` | `t |> equal(a, b)` |
| `assert(condition)` | `t |> success(condition)` |
| `assert(condition, "msg")` | `t |> success(condition, "msg")` |
| `assert(false)` (guard) | `t |> failure("reason")` |

The dastest API records failures with file/line info and continues running other tests.

**Exception:** files that use `expect <code>:<count>` directives are compilation-failure tests — they don't use `[test]` or `t : T?` at all.

## Threading `t : T?` through helpers

When a `[test]` function calls helper functions that need to assert:

1. Add `t : T?` as the **first** parameter to the helper.
2. Use `t |> equal(...)` / `t |> success(...)` inside the helper.
3. Pass `t` from the test function: `my_helper(t, other_args)`.

Avoid naming local variables `t` in helpers — it shadows the test object parameter. Use `ii`, `idx`, `val`, `sptr` etc.

## Common test options

- `options no_unused_function_arguments = false` — suppress warnings for test params
- `options no_unused_block_arguments = false` — suppress warnings for block params
- Shared helpers go in a `_common.das` file inside the same test directory

## Testing non-copyable types

When testing operations on types containing non-copyable fields (`array<T>`, `table<K;V>`):

- **Use `emplace`** to add structs with non-copyable fields to arrays: `arr |> emplace <| make_value()`. Plain `push` copies and fails on non-copyable fields.
- **Verify source preservation** after `push(container, named_var)` — the source variable should still be intact. If the implementation accidentally calls `emplace` instead of `push_clone`, the source is destroyed.
- **Test non-copyable fields explicitly** — include at least one struct with an `array<T>` or `table<K;V>` field so accidental copies surface.
- **Factory helpers** — write `make_xxx()` functions that construct test values, keeping test code concise.

## Test-first bug verification

When fixing bugs, write the failing test BEFORE applying the fix:

1. Write a test that demonstrates the expected behavior.
2. Run it — confirm it FAILS (this proves the bug exists and the test is valid).
3. Apply the fix.
4. Run the test again — confirm it PASSES.
5. Run the full test suite to ensure no regressions.

This catches tests that pass for the wrong reason (a tautology that never exercised the bug).
