# Testing Conventions (dastest)

Tests use the `dastest` framework, which ships with the SDK under `dastest/`. Everything
below applies wherever you write tests — in your own project or in the daslang repo.

Working inside the daslang repository? Its own harness is a separate concern and no part
of it applies to an SDK install, so none of it is shipped here — AOT registration for new
test directories, the `tests/.das_test` gating filter and deep-engine model-test rules are
all covered by `skills/internal/tests_in_repo.md` (repo-only).

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

`[test]` also accepts a no-argument form (`[test] def test_x()`) when the body needs no assertions object.

## Key test functions

- `t |> equal(actual, expected)` — value equality assertion (reports "values differ"). **Label caveat:** on failure it prints `expected:` for argument 1 and `got:` for argument 2 — the reverse of XUnit. Keep writing `equal(actualExpr, wantValue)` (house idiom); just read `expected:` as *the live result*
- `t |> success(condition)` — boolean assertion (reports "expected success, got failure")
- `t |> success(condition, "message")` — boolean assertion with custom message
- `t |> failure("message")` — unconditional failure with message
- `t |> run("name") @(t : T?) { ... }` — named subtest
- `t |> strictEqual(actual, expected)` — strict equality assertion (**fatal on fail**: it aborts the enclosing `run` arm; later arms still run)
- `t |> numericEqual(actual, expected)` — numeric equality that treats NaN == NaN as equal (**fatal on fail**, like `strictEqual`)
- `t |> skip("reason")` — mark the arm skipped (not passed) and print the reason. Does NOT unwind; use `if (gate) { t |> skip("..."); return }`
- Every assertion above takes an optional trailing message: `t |> equal(a, b, "msg")`

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

**Exception:** files that use `expect` directives (compilation-failure tests) are designed
to trigger compiler errors — these do NOT use `[test]` or `t : T?` at all.

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
- Shared test helpers go in `_common.das` module files, kept in the same directory as the
  tests that require them. A require path cannot contain a hyphen, so a test living in a
  hyphenated directory must require its siblings by bare name.
- The leading `_` is load-bearing: dastest's walker (`dastest/fs.das`) skips every file **and
  directory** whose name starts with `_`, which is why a helper module is never picked up as a
  test. The same rule is how you disable a test — rename `test_foo.das` to `_test_foo.das`, or
  prefix the whole directory.

## Running tests

```bash
# one file
bin/daslang dastest/dastest.das -- --test path/to/test_something.das
# a directory
bin/daslang dastest/dastest.das -- --test path/to/tests/
```

The SDK ships dastest's own suite at `dastest/tests/`, which doubles as a worked example:

```bash
bin/daslang dastest/dastest.das -- --test dastest/tests
```

Useful flags: `--failures-only` to quieten passing output, `--color`, `--timeout <seconds>`,
`--compile-only` to compile the test files without executing them, and `--isolated-mode` to
run each file in its own process (slower, but survives a crash in one file).

## Benchmarks

The same framework runs benchmarks. Write `[benchmark] def bench_x(b : B?)`, one or more
`b |> run("name") { ... }` arms (a **block**, not a lambda), and `b |> accept(value)` on the
result so the optimizer cannot delete the work:

```das
[benchmark]
def bench_sum(b : B?) {
    b |> run("sum 1..100") {
        var acc = 0
        for (i in range(1, 101)) {
            acc += i
        }
        b |> accept(acc)
    }
}
```

Run with `--bench`: `bin/daslang dastest/dastest.das -- --bench --test path/to/bench.das`.
Filter with `--bench-names`, repeat with `--count N`, and change the output shape with
`--bench-format native|go|json`.

## Measuring heap use — `options persistent_heap` is required

The default heap is a chain of linear (bump) chunks, and freeing retreats a
chunk's bump pointer **only when the freed block is the last allocation in that
chunk** — any other free is simply not reclaimed. So whether
`heap_bytes_allocated()` drops depends on allocation order and where the chunk
boundaries happen to fall, not on whether you actually freed anything. The same
assertion can pass by hand and fail inside a test, or vice versa, with no change
to the code under test.

Put `options persistent_heap` in the test file to get an allocator that records
every individual free; the counter is then exact regardless of order. Any test
that asserts on `heap_bytes_allocated()` needs it.

```das
options persistent_heap        // exact free accounting
options force_inscope_pod      // required for clear()/erase() to collect element-owned heap

var outer : array<array<int>>  // elements own heap; a flat array<int> has nothing to collect
// ... fill outer ...
let before = heap_bytes_allocated()
outer |> clear()               // collects the inner buffers; `outer`'s own buffer survives
t |> success(heap_bytes_allocated() < before, "inner buffers came back")
```

Two things to keep straight about `clear()`. It releases only what the ELEMENTS
own — the container's own buffer is capacity, and comes back on `delete`, not
`clear`. And that element collection rides the `force_inscope_pod` policy, which
is OFF by default: without it the counter does not move at all, even under
`persistent_heap`.

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
