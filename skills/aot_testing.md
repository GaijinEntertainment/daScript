# AOT Testing

This skill covers the Ahead-Of-Time (AOT) compilation test infrastructure: how to add AOT tests, how the pipeline works, and common pitfalls.

## AOT Compilation Overview

daslang supports three execution tiers: interpreter → AOT (to C++) → JIT (LLVM). AOT compiles `.das` scripts to C++ at build time, which is compiled and statically linked into the host binary. At runtime, the simulator matches functions by **semantic hash** — if an AOT stub matches, the native code runs; otherwise the interpreter is used.

### Key concepts

- **Semantic hash**: a hash of a function's AST shape. Must match between AOT generation time and runtime for the AOT stub to link.
- **`AotListBase`**: static registration object. Each AOT-generated `.cpp` file defines one; its constructor registers function pointers in a global `AotLibrary` map keyed by semantic hash.
- **`CodeOfPolicies`**: compilation settings that affect AOT behavior:
  - `cop.aot = true` — enable AOT stub lookup during simulation
  - `cop.fail_on_no_aot = true` — error (code 50101) if any function lacks an AOT stub
  - `cop.aot_module = true` — set during AOT generation (not at runtime)
  - `cop.aot_lib = true` — library AOT mode (for daslib modules)

### AOT C++ emitter location

The AOT C++ emitter lives in **`daslib/aot_cpp.das`** (a visitor written in daslang). The old `src/ast/ast_aot_cpp.cpp` was deleted by commit `581363ebc`. When codegen output changes shape, edit `daslib/aot_cpp.das` and rebuild `libDaScriptAot`.

Key helpers used by the emitter:

- **`preVisitExprBlockFinal(blk)`** — emits `auto <finallyName> = das_finally([&]() { ... });` at the start of the block's finally region. The RAII guard fires on normal fall-through, `continue`, `break`, and `return`. For `for`/`while` loop bodies the guard sits **inside** the generated C++ body braces, so finally runs once per iteration — not once after the loop.
- **`das_finally`** — C++ RAII scope guard defined in the runtime. Destructor runs the captured finally lambda at any scope exit.
- **`finallyName(blk)`** — generates a unique C++ identifier for the per-block `das_finally` guard variable.

## The test_aot Binary

`tests/aot/CMakeLists.txt` defines the `test_aot` executable — a standalone binary with AOT stubs compiled in. It uses the same `main.cpp` as `daslang` but links additional AOT-generated object files.

**Two C++-AOT binaries** (a third, LLVM-backed one is covered under "Registering a New Test Directory"):

| Binary | Contents | In default build (ALL)? | Who builds/runs it |
|---|---|---|---|
| `test_aot_subset` | `tests/language` + its `_*` fixtures + `dastest/testing.das` + the minimal daslib runtime closure (`AOT_SUBSET_DASLIB_MODULE_FILES` — random/regex/faker/fuzzer/coroutines/archive/strings_boost etc., ~15 small files; a language test requiring another runtime daslib module fails 50101 → extend that list) — ~230 AOT TUs total | YES — every CI lane, Debug included | Per-PR CI, as a **compile+link gate only** (no AOT test run on PRs); locally via `run_tests_aot_subset` |
| `test_aot` | everything (~1080 AOT TUs) | NO — `EXCLUDE_FROM_ALL` | Nightly CI cron (all Release matrix lanes incl. sanitizers, plus mingw/clang-cl) and `preflight --full`, both via `--target test_aot` / `run_tests_aot` |

Neither binary uses LTO (the full binary's LTCG link alone was ~21 min on MSVC CI). Consequence: **an AOT regression outside `tests/language` does NOT fail PR CI** — it's caught by local `preflight --full` before push, or by the nightly.

### Structure

```
tests/aot/
    CMakeLists.txt              # Build rules for test_aot / test_aot_subset / test_llvm_aot
    test_arithmetic.das         # AOT test: math, functions, function pointers
    test_strings.das            # AOT test: string operations
    test_lambdas.das            # AOT test: lambdas, captures, higher-order functions
    test_structures.das         # AOT test: structs, classes, arrays, tables
    ...                         # ~27 test_*.das in all — codegen regressions get a file here
    _template_member_fixture.das  # `_`-prefixed fixture: excluded from the glob (see below)
    _aot_generated/             # Auto-generated C++ (gitignored)
```

### How it builds

1. `DAS_AOT_LIB` compiles `dastest/testing.das` to C++ (library mode — `-aotlib` flag)
2. `DAS_AOT` compiles each test `.das` file to C++ (script mode — `-aot` flag)
3. All generated `.cpp` files are compiled and linked into `test_aot` alongside `libDaScriptAot`
4. At runtime, `test_aot` is `daslang` with extra AOT stubs — runs tests with `cop.aot=true`

### CMake macros

| Macro | Purpose | AOT flag |
|---|---|---|
| `DAS_AOT(files, genList, target, tool)` | AOT for scripts (regular `.das` files) | `-aot` |
| `DAS_AOT_LIB(files, genList, target, tool)` | AOT for library modules (daslib, dastest) | `-aotlib` |
| `DAS_AOT_EXT(files, genList, target, tool, extra)` | Core macro — others call this | `extra` alone (no implicit `-aot`; `DAS_AOT` passes `-aot` as the extra) |
| `DAS_AOT_CTX(files, genList, target, tool)` | AOT with custom context | `-ctx` |
| `DAS_AOT_STANDALONE(files, genList, target, tool, extra)` | AOT for a standalone binary | `extra` |
| `DAS_LLVM_AOT_LIB(files, genList, target)` | LLVM-backend AOT — emits native `.o`, not C++; runs `utils/jit/main.das` in batches of 32 | `--aot-object` |

**Target name collision**: `DAS_AOT_EXT` creates a custom target named `${mainTarget}_genaot`. Multiple calls with the same `mainTarget` will collide. Use distinct target names (e.g., `test_aot_testing` and `test_aot_tests`).

## Running AOT Tests

```bash
# Build the test_aot binary
cmake --build build --config Release --target test_aot

# Run all AOT tests — --use-aot is REQUIRED, there is no auto-detection at runtime
bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --test tests/aot

# Run with the regular daslang binary (no AOT, interpreter mode)
bin/Release/daslang.exe dastest/dastest.das -- --test tests/aot
```

`--use-aot` (the dastest flag, after `--`) is what sets `cop.aot` + `cop.fail_on_no_aot` on each compiled test file. **Without it the stubs are simply never linked and every test silently interprets** — the run still passes, so a lane that forgets the flag measures/tests nothing. `is_in_aot()` is true only while the AOT emitter itself runs (`daslib/aot_cpp` calls `set_aot()`), never in a plain `test_aot` run, and `dastest`'s `[INTERP]`/`[JIT]` benchmark label is derived from it — so an AOT run still prints `[INTERP]`. The `run_tests_aot` / `run_tests_aot_subset` targets pass both `-use-aot` (host) and `--use-aot` (dastest); copy them.

## Adding Tests to AOT — CRITICAL

**Every test directory under `tests/` must be registered in `tests/aot/CMakeLists.txt`**. The full `test_aot` binary (nightly CI + `preflight --full`) runs ALL tests under `tests/` with AOT enabled. If a test directory's AOT stubs aren't generated and linked, `test_aot` will fail with `error[50101]: AOT link failed`. PR CI won't catch the missing registration (it only builds the `tests/language` subset) — the nightly and preflight will, so register up front.

This applies to ALL test directories (e.g., `tests/fio/`, `tests/fs/`, `tests/json/`), not just `tests/aot/`. See the "Registering a New Test Directory" section below.

**Do NOT use `options no_aot`** to mask a missing CMake registration — register the tests properly instead.

**Exception — a file that genuinely can't AOT** (codegen/emitter bug, interpreted-only by design): use BOTH markers together, each with a comment + issue link:
1. `options no_aot` in the file — makes test_aot's `fail_on_no_aot` skip AOT linking for it at runtime;
2. exclude it from the directory's AOT glob in `tests/aot/CMakeLists.txt` — skips generating stubs that wouldn't compile.

**Trap:** glob exclusion ALONE is not enough. `test_aot` runs every file under `tests/` regardless of what was stub-generated, so an excluded-but-not-`no_aot` file fails at runtime with `error[50101]` on all its functions (precedent: `tests/fixed_array/test_interop.das`, issue #3077).

**Trap — `_`-prefixed fixture MODULES in an existing test dir.** The per-dir AOT globs
exclude `_*` files (`EXCLUDE REGEX "/_"`), so a new same-dir fixture module (`require`d by a
test via the bare same-directory form) never gets AOT'd — the test then fails 50101 on the
fixture's functions ("module never AOT'd" looks identical to a stale stub). Register the
fixture explicitly in the dir's module list in `tests/aot/CMakeLists.txt` (e.g.
`AOT_LANGUAGE_MODULE_FILES` for `tests/language/_*.das`; precedent: `_lambda_vis_inner.das` /
`_lambda_vis_mid.das`).

**Intentionally-non-compiling `expect`-fixtures** (a `[macro]`/annotation that is *supposed* to fail compilation, driven by a sibling test via `compile_file` + asserting the error text — e.g. fail-closed codegen-rejection fixtures): `options no_aot` does NOT save these. `no_aot` skips *emission*, but the AOT generator (`utils/aot/main.das`) still *compiles* the program first, so a file that fails compilation breaks the AOT build before the no-emit skip applies. Put such fixtures in a **`_`-prefixed file inside a non-globbed subdir** (e.g. `tests/spirv/_fail_closed/_fc_*.das`): the `_` prefix keeps dastest/`test_aot` from discovering+running it, and the subdir keeps the non-recursive `tests/<dir>/*.das` AOT glob from trying to stub-generate it. Add an `expect <code>` directive too so the lint sweep skips it (precedent: `tests/spirv/_fail_closed/`, dasSpirv Phase 6.4 fail-closed gate).

**AOT-emit trap — raw-pointer indexing by int64 (FIXED, #3391).** `p[i]` where `p : T?` and `i : int64` used to be AOT-ambiguous for the non-`var` pointer form: `das_index<T * const>` had only `int32_t`/`uint32_t` `at`/`safe_at` overloads (the `T *` and `const T * const` specializations got 64-bit overloads in `c40b653d9`; this one was missed). Fixed by adding the int64/uint64 overloads in `include/daScript/simulate/aot.h`; regression test `tests/aot/test_int64_ptr_index.das`. Old workaround (`for (i in range(int(n)))` / `p[int(i)]`) is no longer needed.

## Adding a New AOT Test in `tests/aot/`

1. Create `tests/aot/test_foo.das`:

```das
options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public

[test]
def test_basic(t : T?) {
    t |> run("my test") @(t : T?) {
        t |> equal(1 + 1, 2)
    }
}
```

2. Nothing to do in CMake — `tests/aot/*.das` is globbed with `CONFIGURE_DEPENDS`. The only exception is a `_`-prefixed fixture module, which the glob filters out (`list(FILTER … EXCLUDE REGEX "/_")`) and which therefore needs an explicit entry if its functions must be AOT'd.

3. Update `tests/README.md` with a row for the new file.

4. Rebuild: `cmake --build build --config Release --target test_aot`

> **Build timing**: Always use `timeout: 0` when running CMake builds — they can take 15-25 minutes for clean builds and 2-10 minutes for incremental builds. Never assume the build is stuck just because there's no output; MSVC is silent during compilation.

## How -use-aot / --use-aot Works

The AOT flag flows through three layers:

1. **`main.cpp`**: `-use-aot` sets `useAot = true`. Does NOT set `policies.aot` on the host (dastest framework functions don't have AOT stubs in every binary).
2. **`dastest/suite.das`**: `--use-aot` sets `ctx.use_aot = true` (the `|| is_in_aot()` beside it never fires at run time — see above). When compiling test files, sets `cop.aot = true` and `cop.fail_on_no_aot = true`.
3. **`dastest/dastest.das`**: In isolated mode, forwards `-use-aot` to child processes when `is_in_aot()` is true.

**Critical**: The host program must NOT set `policies.aot = true` because the host compiles dastest framework scripts (like `suite.das`), which don't have AOT stubs in the host binary. Only the sub-compiled test files get `cop.aot = true`.

## Module::aotRequire() — Declaring AOT Headers

When a C++ module binds functions whose declarations live in specific headers, the module must emit `#include` directives so AOT-generated C++ compiles correctly.

Override `aotRequire()` in your module class:

```cpp
virtual ModuleAotType aotRequire(TextWriter & tw) const override {
    tw << "#include \"daScript/simulate/bin_serializer.h\"\n";
    tw << "#include \"daScript/misc/performance_time.h\"\n";
    return ModuleAotType::cpp;
}
```

**Forgetting a header here causes AOT compilation failures** — the generated C++ calls the function but the compiler can't find the declaration. This was a latent bug with `performance_time.h` (needed for `ref_time_ticks`, `get_time_usec`, `get_time_nsec` bound in `Module_BuiltIn::addTime()`).

## Block Locals and finally/defer

Block-typed locals (`let foo = $() {...}`) are init-only, non-copyable, self-referential (`das_make_block`: `body = this`, functor held by pointer to a stack lambda) — a hoisted one can't take the declare-then-assign shape other locals use (`auto x;` doesn't compile; assigning from a temporary copies self-pointers of a dead object). Instead the emitter (`emitBlockLet` in `daslib/aot_cpp.das`, called from the hoist site just above it) emits the **whole let** — named `_TempFunctor` lambda, `auto <name> = das_make_block<...>(...)`, const-ref alias — at the top of the nearest finally-carrying scope (`BlockVariableCollector.blockLets`, keyed by `getFinalBlock()`), *before* that scope's `das_finally` guard; the original `let` line renders as a no-op `name;`. Everything the block captures is already declared there (locals are storage-hoisted, loop iterators precede the body scope, `[&]` binds variables not values), and being constructed before the guard means the objects outlive it — a finally invoking the block reads live objects (clang-ASAN-clean; the `linux_asan` CI lane compiles `test_aot` with clang ASAN). That scope is the variable's *source* scope even after the optimizer's early-return→else restructuring (optimizer-made wrapper blocks carry no finalList), so it dominates every guard that can legally reference the variable. Consequence of initializing at the hoist point: on an early-exit path where the `let` never executed, an AOT finally invoking the block *runs it*, while the interpreter hits a zeroed slot (crash) — invoking a never-initialized block is undefined behavior; rely on neither. Tests: `tests/bare_block/test_block_in_finally.das`.

Also note: `dasAotStub_ast_boost.das.cpp` generation is nondeterministic across runs (the emitted representative of an identical-layout `Setup*` struct family varies) — a stub diff there does not imply your emitter change caused it; re-run twice with the same emitter to check.

## Semantic Hash Architecture

AOT linking works by **semantic hash matching**. The AOT tool generates C++ stubs keyed by a hash; at runtime the simulator computes the same hash and looks up the stub. If hashes don't match → error 50101 "AOT link failed".

### Two-level hashing

Every function has two hash values (see `src/simulate/simulate_fn_hash.cpp`):

1. **`hash`** (own hash) — the function's **SIM node tree** hash. Computed by `getFunctionHash()` which walks the simulated node tree and hashes every node type, constant value, and type descriptor. This captures the function's own behavior.

2. **`aotHash`** (AOT hash) — computed by `getFunctionAotHash()`:
   ```
   aotHash = hash_block64([own_hash, dep1_hash, dep2_hash, ...])
   ```
   Collects all **transitive non-builtin, non-noAot dependencies** via `DependencyCollector`, sorts them by mangled name for stability, then hashes the concatenation of all their `hash` values together with the function's own `hash`.

The registration table in generated C++ is keyed by `aotHash`. At runtime, `linkCppAot()` recomputes `aotHash` and does a lookup.

### What goes into `hash` (own hash)

`getFunctionHash()` hashes (via `SimHashVisitor`):
- The function's **result type** descriptor string
- Each **argument type** descriptor string
- The entire **SIM node tree** — every node class name, every constant value, every string literal, every field offset

**Any difference in the SIM tree causes a different `hash`**. This includes:
- Different constant values (e.g., different file path strings embedded in structs)
- Different field offsets (struct layout changes)
- Different node types (optimization differences)

### What goes into `aotHash`

`getFunctionAotHash()` adds dependency hashes:
- Collects transitive function dependencies (calls, lambda captures, etc.)
- Filters: skips `builtIn` and `noAot` functions
- Sorts by mangled name (deterministic ordering)
- Hashes: `[own_hash, dep1_hash, dep2_hash, ...]`

**A dependency's `hash` changing causes all callers' `aotHash` to change**, even if the callers themselves are unchanged.

### Hash comment diagnostics

Every AOT-generated C++ file includes a **hash comment** before each registration entry, showing the function's own hash and all dependency hashes with names:

```cpp
// my_function hash=0xabc123..., dep_func_1=0xdef456..., dep_func_2=0x789...
{ 0x<aotHash>, false, (void*)&my_function_abc123, &__wrap_my_function_abc123 },
```

When `linkCppAot()` fails to find a match, the error output includes the same hash comment for the runtime-computed values:

```
error[50101]: AOT link failed on my_function
semantic hash is <runtime_aotHash>
// my_function hash=0x<runtime_own_hash>, dep1=0x<runtime_dep1_hash>, ...
```

**To diagnose a mismatch**, compare the two hash comments:
1. Open the generated `.cpp` file in `_aot_generated/` and find the comment for the failing function
2. Compare with the runtime hash comment in the error output
3. If `hash` (own hash) differs → the SIM tree is different (see below)
4. If a dependency hash differs → that dependency's SIM tree changed

### C++ API for hash debugging

| Function | Location | Purpose |
|---|---|---|
| `getFunctionHash(fun, code, ctx)` | `simulate_fn_hash.cpp` | Compute own SIM hash |
| `getFunctionAotHash(fun)` | `simulate_fn_hash.cpp` | Compute AOT hash (own + deps) |
| `getAotHashComment(fun)` | `simulate_fn_hash.cpp` | Format diagnostic string with all dep hashes |
| `get_aot_hash_comment(fun)` | das binding in `module_builtin_ast.cpp` | Same, callable from das |

### Debug printf macros

In `src/simulate/simulate_fn_hash.cpp`, two macros control debug output:

```cpp
#if 1          // change to 0 to enable
#define debug_hash(...)           // per-SIM-node hash contributions
#else
#define debug_hash  printf
#endif

#if 1          // change to 0 to enable
#define debug_aot_hash(...)       // dependency list and final aotHash
#else
#define debug_aot_hash  printf
#endif
```

- **`debug_aot_hash`**: prints each dependency name + hash, then the final `aotHash`. Useful to see which dependencies are included and their hash values.
- **`debug_hash`**: prints every SIM node's hash contribution. Very verbose — use only when you need to find exactly which node differs.

To use: change `#if 1` to `#if 0` for the desired macro, rebuild daslang/test_aot.

### Printing the SIM tree

`options log_nodes` in a `.das` file prints the full SIM node tree at compile time. `options log_nodes_aot_hash` additionally prints each node's hash contribution. Useful for comparing trees between AOT generation and runtime.

**Warning**: `options log*` lines change the compilation (they set options that may affect code generation). Always remove them before generating final AOT stubs, or the generated hashes will be stale.

## Semantic Hash Stability — Critical Pitfall

Functions with `SideEffects::none` can be **constant-folded** at compile time. If a function's result depends on `CodeOfPolicies` fields that differ between AOT generation and runtime, the folded constant will differ → different AST → different semantic hash → AOT link failure (error 50101).

**Example**: `aot_enabled()` checks `context->thisProgram->policies.aot`:
- During AOT generation: `cop.aot = false` (AOT gen mode) → constant-folded to `false`
- At runtime: `cop.aot = true` → constant-folded to `true`
- Different hashes → error 50101 "no AOT"

**Rules**:
- Don't use `aot_enabled()` or `is_in_aot()` in test assertion logic that needs hash stability
- If `fail_on_no_aot = true` and tests pass, AOT is working — no need to check `aot_enabled()` explicitly
- Any compile-time-evaluable expression that depends on `cop.aot`, `cop.jit`, etc. is a hash stability risk

## Common AOT Hash Mismatch Causes

| Symptom | Cause | Fix |
|---|---|---|
| Own hash differs, path strings in SIM tree | File path separator mismatch (`/` vs `\`) between CMake and runtime | Normalize paths at the point they enter the SIM tree (e.g., `replace(path, "\\", "/")`) |
| Own hash differs, constant values differ | `SideEffects::none` function folded with different `CodeOfPolicies` | Remove the policy-dependent call, or mark function `SideEffects::modifyExternal` |
| Dependency hash differs | A dependency function's SIM tree changed (different module state in batch) | Compare dep hashes in the hash comment; inspect the specific dependency |
| AOT hash differs but own + all deps match | Dependency list differs (extra or missing deps) | Check if batch processing adds/removes function instantiations |
| All hashes match but still fails | Stale generated C++ — `.cpp` file wasn't regenerated | Delete `_aot_generated/*.cpp` and rebuild |
| Own hash differs after editing a quote-lowered file (`options aot_macros`) | ANY edit — even comment-only — shifts line numbers, and lowered code bakes LineInfos as integer constants | Purge that directory's `_aot_generated/` and rebuild `test_aot` (the custom commands don't track daslib deps either — same purge after `daslib/quote.das` edits) |

### Batch AOT processing

The AOT tool (`utils/aot/main.das`) can process multiple `.das` files in one invocation. CMake's `DAS_AOT_EXT` macro batches files this way for efficiency. **Batch processing can cause hash divergence** if:

- A macro in file A instantiates generic functions that share names/mangled names with instantiations from file B
- Module-level state from processing file A leaks into file B's compilation context

**Diagnosing batch issues**: If single-file AOT generation produces matching hashes but batch doesn't, use the hash comment diagnostics to find the diverging function. You can test single-file generation with:

```bash
daslang.exe utils/aot/main.das -- -aot path/to/test.das path/to/output.cpp
```

## libDaScriptAot — Standard Library AOT

`libDaScriptAot` is a static library containing pre-generated AOT stubs for core daslib modules. Defined in the root `CMakeLists.txt`:

```
daslib/_aot_generated/
    dasAotStub_ast_boost.das.cpp
    dasAotStub_templates_boost.das.cpp
    dasAotStub_functional.das.cpp
    dasAotStub_math_boost.das.cpp
    dasAotStub_utf8_utils.das.cpp
    ... (13 modules total — the AotDaslibList in the root CMakeLists.txt)
```

When adding a new AOT test binary, link `libDaScriptAot` to get these stubs:

```cmake
TARGET_LINK_LIBRARIES(my_aot_binary libDaScriptAot ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
```

### Regenerating libDaScriptAot stubs

After changing `Module::aotRequire()` or builtin bindings, regenerate:

```bash
cmake --build build --config Release --target libDaScriptAot
```

The generated files land in `daslib/_aot_generated/`, which is gitignored — nothing to commit.

## AOT Link Failures Are Real Failures

`test_aot` exists specifically to catch AOT failures, including **link failures** (error 50101). When a file like `_module_a.das` fails with "AOT link failed", that IS a real failure — it means the AOT stubs could not be linked at simulation time. Do NOT dismiss these as "not real tests" or "helper modules". If a `.das` file is picked up by dastest and fails under AOT, it needs to be fixed or excluded from the AOT test set.

Common causes of AOT link failures:
- Missing `Module::aotRequire()` headers
- Semantic hash mismatch (see below)
- Helper modules without `[test]` functions that weren't meant to run standalone — these should be excluded from the test directory or the CMake file list

## CI Integration

The full AOT sweep runs in `.github/workflows/build.yml` on **Release lanes of the nightly cron / `workflow_dispatch` only** — the step guard is `if: matrix.cmake_preset == 'Release' && (github.event_name == 'schedule' || github.event_name == 'workflow_dispatch')`. It is driven through the `run_tests_aot` CMake target (`tests/CMakeLists.txt`) rather than a direct test_aot invocation — look for the "Slow Release Tests" / "Slow Release Tests (AOT)" / "Test: AOT sweep" steps (`cmake --build ... --target run_tests_aot`). Per-PR lanes get only the `test_aot_subset` compile+link gate from the Build step. `windows32` is skipped (the matrix variable is `matrix.cmake_preset`).

## _aot_generated Directories

AOT-generated C++ files go into `_aot_generated/` subdirectories — all are gitignored:
- `daslib/_aot_generated/` — regenerated at build time by `DAS_AOT_LIB` (part of `libDaScriptAot`)
- `tests/aot/_aot_generated/` — build artifact for `test_aot`
- `dastest/_aot_generated/` — build artifact for `test_aot`

When creating a new AOT target that generates into a new directory, add it to `.gitignore`.
All `_aot_generated/` directories are covered by a single broad pattern in `.gitignore` — no need to add per-directory entries.

## Registering a New Test Directory for AOT

`tests/aot/CMakeLists.txt` is data-driven: three loops build every suite's scaffolding from a list, and they accumulate `TEST_AOT_TARGETS` / `TEST_AOT_GENVARS` / `TEST_AOT_ALL_DAS`, which in turn feed the source groups, the `add_executable(test_aot …)` sources and its `ADD_DEPENDENCIES`. There are no literal per-suite source lists to edit.

**Step 1 — a plain suite.** Add the directory name to `set(DAS_AOT_SUITES …)`. That is the whole registration: the loop globs `tests/<name>/*.das`, creates `add_custom_target(test_aot_<name>)`, calls `DAS_AOT`, and wires the generated sources plus the dependency into `test_aot` for you.

**Step 2 — only if the file set is irregular.** Define `AOT_<UPPERCASE>_FILES` **above** the loop and it is used instead of the convention glob — either a curated `SET(...)`, or a `FILE(GLOB ...)` followed by `list(FILTER ... EXCLUDE REGEX ...)` to drop `failed_*` / `cant_*` / `_`-prefixed fixtures. The loop only globs when the variable is not already defined.

**Step 3 — only if the directory has fixture modules.** `_`-prefixed modules that tests `require` are excluded by the `/_` filter and must be AOT'd as libraries: add the suite to `set(DAS_AOT_MODULE_SUITES …)` and define `AOT_<UC>_MODULE_FILES`. That loop calls `DAS_AOT_LIB` and creates `test_aot_<name>_modules`.

**Step 4 — only if the suite needs a CMake guard or extra dependencies.** Add a row to `set(DAS_AOT_IRREGULAR …)` instead of the plain list. The row format is `name|flavor|guard|EXTRA_DEPENDS|accumulate`, where flavor is `reg` (script AOT) or `mod`/`lib` (library AOT), guard is a CMake condition such as `NOT DAS_SQLITE_DISABLED` (`-` for none), `EXTRA_DEPENDS` names variables holding extra regen dependencies (`-` for none), and accumulate is `1` unless the row is subset-only.

**Why this matters**: the full `test_aot` runs ALL tests under `tests/` with `cop.fail_on_no_aot = true`. Without registration the test's functions have no AOT stubs → `error[50101]: AOT link failed`. Per-PR CI only builds `test_aot_subset`, so a missing registration passes PR CI and fails the nightly / `preflight --full`. `test_aot` builds and runs on Windows too — only 32-bit Windows is excluded (`NOT (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)` gate in the root CMakeLists.txt); it requires tests and AOT examples enabled in the CMake configure.

### The LLVM-AOT rail (`test_llvm_aot`)

There is a third binary beside `test_aot` / `test_aot_subset`. `test_llvm_aot` (LLVM-only, `EXCLUDE_FROM_ALL`, opt-in — not in ALL and not in CI) compiles each `.das` through the **LLVM backend** into a self-registering native `.o` (a `das_aot_register` load ctor), linked straight into the binary; `-use-aot` then binds each function as a `SimNode_Jit` via `linkCppAot`. It is built by `DAS_LLVM_AOT_LIB` (which drives `utils/jit/main.das --aot-object`, not the C++ AOT tool) and run through the `run_tests_llvm_aot` target in `tests/CMakeLists.txt`.

**Its corpus is derived, so registering a suite enrols it here too.** `LLVM_AOT_TEST_FILES` is the accumulated `TEST_AOT_ALL_DAS` (every `reg`-flavor suite's test bodies) plus `tests/jit_tests/*.das`, minus `_`-prefixed files and a filter list (`cant_`, `llvm_tune`, `llvm_code`, `llvm_compile_only`, `dll_cache`, `jit_fastpath`, `typeinfo`, and all of `tests/msl/` + `tests/metal/`, which decline the JIT via `lattice_fallback`). Adding a directory to `DAS_AOT_SUITES` therefore silently adds it to the LLVM-AOT corpus — if it can't survive that rail, it needs a filter entry as well.

Also ensure that wrapper functions in daslib `.das` files (like `daslib/fio.das`) are marked `[generic]` — otherwise AOT can't inline them and will try to link against a non-existent concrete stub from the builtin module.
