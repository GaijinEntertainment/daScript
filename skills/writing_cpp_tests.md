# Writing C++ tests for daslang

Companion to `skills/writing_tests.md` (which covers `.das` tests under `tests/`). Read BEFORE writing or editing any C++ test under `tests-cpp/`.

C++ tests live under `tests-cpp/`, use [doctest](https://github.com/doctest/doctest) (vendored in `tests-cpp/3rdparty/doctest/`), and are wired into `ninja test` / `ctest`.

## Layout & where things go

```
tests-cpp/
  CMakeLists.txt              entry point — top-level CMakeLists.txt has ONE line: add_subdirectory(tests-cpp)
  3rdparty/doctest/           vendored single-header doctest + cmake helpers (MIT/BSD-3)
  doctest_main.cpp            owns DOCTEST_CONFIG_IMPLEMENT, Module::Initialize/Shutdown, suite-end leak verification
  small/
    test_*.cpp                drop-in doctest tests; globbed into the single `tests-cpp-small` exe
    test_*.das                companion fixtures next to their .cpp (loaded via getDasRoot() + "/tests-cpp/small/...")
  big/
    <name>/
      CMakeLists.txt          auto-picked-up via glob; one or more standalone exes per dir
      *.cpp                   custom int main(), custom deps, custom link variants
```

**Companion `.das` fixtures live next to their `.cpp`** — never under `tests/` (that's dastest territory; mixing forces dastest-side maintenance for non-dastest files).

## Adding a small test (golden path)

Create `tests-cpp/small/test_my_feature.cpp`:

```cpp
#include <doctest/doctest.h>
#include "daScript/daScript.h"
using namespace das;

TEST_CASE("my feature does X") {
    CHECK(1 + 1 == 2);
}
```

Optionally drop `tests-cpp/small/test_my_feature.das` next to it; load via `getDasRoot() + "/tests-cpp/small/test_my_feature.das"`.

Rebuild — `cmake --build build`. The new test appears in `ctest -N` automatically (no CMake edit needed).

**Don't include doctest with `DOCTEST_CONFIG_IMPLEMENT`** — that's owned exclusively by `doctest_main.cpp`. Including the header without that macro is the right pattern for every other TU.

## Adding a big test

A "big" test is anything that needs a custom `int main()`, custom link deps, or multiple link variants (static + dynamic). The canonical example is `tests-cpp/big/concurrent_init/` — it has a thread-startup-barrier harness and is built as both `test_concurrent_init` (static) and `test_concurrent_init_dyn` (dynamic).

To add one:

```sh
mkdir tests-cpp/big/my_stress_test
# write CMakeLists.txt + sources, using big/concurrent_init/CMakeLists.txt as a template
```

The CMakeLists ends with:

```cmake
add_test(NAME my_stress_test COMMAND my_stress_test
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(my_stress_test PROPERTIES LABELS "big")
add_dependencies(test-big my_stress_test)
```

Big tests **don't include doctest** — they keep their own `int main()` returning 0/1 to ctest. They handle `Module::Initialize()` / `Module::Shutdown()` themselves.

Big tests are **not in CI yet** (V1) — local-only via `ninja test-big`.

## Doctest assertion cheat sheet

| Macro | Behavior |
|---|---|
| `CHECK(cond)` | Non-fatal; test continues on failure |
| `REQUIRE(cond)` | Fatal; test stops on failure |
| `CHECK_FALSE(cond)` / `REQUIRE_FALSE(cond)` | Negated form |
| `CHECK_MESSAGE(cond, "msg")` | Adds a context message on failure |
| `CHECK_EQ(a, b)` / `CHECK_NE(a, b)` / `CHECK_LT` / `CHECK_LE` / `CHECK_GT` / `CHECK_GE` | Comparison helpers — better failure messages than `CHECK(a == b)` because doctest stringifies both sides |
| `CHECK_THROWS(expr)` / `CHECK_NOTHROW(expr)` | Exception assertions |
| `MESSAGE("...")` / `INFO("...")` | Log without checking |

## TEST_CASE / SUBCASE pattern

One `TEST_CASE` per logical scenario. `SUBCASE`s for variations sharing setup. Each SUBCASE re-runs the TEST_CASE body from the top — so per-subcase fresh `Context`/state is automatic.

```cpp
TEST_CASE("compile + run my script") {
    auto program = compileDaScript(...);   // runs once per SUBCASE
    Context ctx(...);
    REQUIRE(program->simulate(ctx, tout));

    SUBCASE("path A") { /* uses fresh ctx */ }
    SUBCASE("path B") { /* uses fresh ctx */ }
}
```

Don't try to share a `Context` across `TEST_CASE`s — modules can survive (they're process-global), but Context-level state will leak between tests and the leak guard will catch it.

See `tests-cpp/small/test_run_with_catch_clear.cpp` for a 5-subcase example.

## Environment & init (what's free, what's yours)

- `Module::Initialize()` runs **once** in `doctest_main.cpp` before any TEST_CASE. Don't call it again.
- `NEED_*` macros also live in `doctest_main.cpp::main()`. Don't add them at file scope — they're statement-form (expand to expression statements + a brace block) and won't compile there. If you need a non-default module, add the `NEED_MODULE(...)` line to `doctest_main.cpp`.
- YOU own `compileDaScript`, `Context` creation, and `simulate` in your TEST_CASE body.

## Leak detection (automatic, will fail your test)

The framework auto-checks three counters at the suite-end of every test process:

1. **gc_node count** — AST nodes (TypeDecl, Expression, Function, etc.) tracked by gc_node.
2. **JobQue / Channel / LockBox / Stream / Feature count** — anything in the `JobStatus` tracking list.
3. **Module-registered handle count** — sum of every `HandleRegistry<T>::live_count()` across every module that registered via `addHandleAnnotation<T>` (HV resources, dasSQLITE statements, etc.). Picked up automatically — no per-module wiring.

When run via ctest, each TEST_CASE is invoked in its own process (`bin/tests-cpp-small --test-case="..."`), so the suite-end check fires per-TEST_CASE and ctest attributes the failure to the right test name. When run directly (`bin/tests-cpp-small`), all tests share one process and the suite-end check reports cumulative state.

Implications when writing a test:
- Don't forget to clean up AST nodes you build in C++.
- Close any `Channel`/`LockBox`/`Feature`/`JobStatus` you create.
- Free any `HandleRegistry<T>` handles you `acquire`.

If a leak shows up that you believe is a false positive: suspect lazy initialization (JIT/RTTI growth on first use). The fix is usually to "warm up" once at suite start, not to disable the check.

## Running tests

| Command | What it does |
|---|---|
| `ninja test` (or `ctest` from build dir) | All small tests (CI runs this). |
| `ninja test-small` / `ninja test-big` | Subset by label. |
| `ctest -R "my feature"` | Single test by name regex. |
| `ctest -V` | Verbose; shows full output and per-test leak attribution. |
| `bin/tests-cpp-small --test-case="*my feature*"` | Direct binary, doctest filter syntax (also `--list-test-cases`). |

VSCode integration: install `matepek.vscode-catch2-test-adapter`, point at `build/`. Per-`TEST_CASE` Run/Debug gutter icons appear in the editor; subcase nesting in the Test Explorer panel.

## Sanitizers

Build with `-DDAS_USE_SANITIZER=address` (or `ubsan`, `tsan`) in the `cmake` step. CI runs ASAN + UBSAN (and TSAN as future tests touch threading) on Linux Debug for every PR. If your test fails under a sanitizer, fix the sanitizer error — don't disable.

## Common gotchas

- `getDasRoot()` returns the project root path baked at configure time. All `.das` fixture loads should use `getDasRoot() + "/tests-cpp/..."`.
- Don't put `NEED_*` macros at file scope. They go in `doctest_main.cpp::main()`.
- The doctest header is large (~7000 lines). Don't include it in non-test code.
- For SUBCASE-heavy tests, remember the body re-runs from the top — each subcase gets a fresh setup. This is the desired behavior; just be aware of the cost (5 subcases = 5 `compileDaScript` calls).
- Big-test `int main()`s must call `Module::Initialize()` / `Module::Shutdown()` themselves — they don't go through `doctest_main.cpp`.
