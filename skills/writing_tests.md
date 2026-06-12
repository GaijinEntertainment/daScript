# Testing Conventions (dastest)

Tests use the `dastest` framework. Test files live in `tests/` with per-module subfolders.

## AOT registration (REQUIRED for new test directories)

CI's `test_aot` binary runs EVERY test under `tests/` with AOT enabled (`fail_on_no_aot`).
Creating a new test directory ⇒ register it in `tests/aot/CMakeLists.txt` (5-step pattern in
`skills/aot_testing.md` § "Registering a New Test Directory"), or CI fails with
`error[50101]: AOT link failed`.

If a specific file genuinely can't AOT (emitter bug, interpreted-only by design): put
`options no_aot` IN THE FILE **and** exclude it from the directory's AOT glob, with a
comment + issue link on both. Glob exclusion alone is NOT enough — test_aot still *runs*
the file and trips 50101 on its missing stubs; `options no_aot` is what makes the runtime
skip AOT linking for it. (2026-06-11: in-file `options no_aot` currently fails in the AOT
hash itself — fix incoming on master; until it lands, interp-only tests are gated by the
directory filter below instead.)

## The `tests/.das_test` directory filter — and its root-path caveat

`tests/.das_test` is a daslang script dastest compiles per run; its `can_visit_folder`
pinvoke gates whole directories per mode — e.g. `no_aot/`, `ast/`, `ast_match/` are
skipped under `--use-aot`, module dirs (dasSQLITE, dasPUGIXML…) skip when the
module isn't built in. **The filter is looked up only at the `--test` ROOT path** —
`--test tests` finds and applies it, but `--test tests/flatten` looks for
`tests/flatten/.das_test` (absent) and walks into `no_aot/` unfiltered, producing
false `error[50101]` / JIT failures. For AOT/JIT validation, sweep `--test tests`
(CI's form) or target individual files — never a subtree that contains gated dirs.

## Per-folder sweep gating (`tests/.das_test`)

`tests/.das_test` defines `can_visit_folder(folder_name, result)` — dastest consults it
per subfolder during file collection (only for the `.das_test` at the `--test <root>`
argument; directly naming a child folder bypasses it). It gates folders on module
availability (`dasHV`, `dasSQLITE`, …) and on sweep mode by scanning argv — currently
only `--use-aot` checks remain (e.g. `ast`, `ast_match`, `no_aot`); the former `-jit`
skips were lifted once `jit_enabled` started triggering daslib/quote lowering. Two traps:
a whole-folder JIT/AOT failure usually means a missing entry here, NOT a per-file fix; and
the `jit_cache_all_tests` prewarm target (utils/CMakeLists.txt) does NOT consult it — its
`--exclude` list mirrors the skips manually and must be updated in the same change.
Per-function `[no_jit]` is the finer-grained alternative when only some functions in a
kept folder can't JIT — put it on the function whose CODE diverges under JIT, not just the
`[test]` wrapper (JITted callees replace their SimNode bodies, so an interpreted wrapper
still calls jitted workers); see the `force_escape_free` workers in tests/gc.

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

## Test-first bug verification

When fixing bugs, write the failing test BEFORE applying the fix:

1. Write a test that demonstrates the expected behavior
2. Run it — confirm it FAILS (this proves the bug exists and the test is valid)
3. Apply the fix
4. Run the test again — confirm it PASSES
5. Run the full test suite to ensure no regressions

This ensures the test actually catches the bug and isn't accidentally passing due to a tautology.
