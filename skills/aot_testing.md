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

The AOT C++ emitter lives in **`daslib/aot_cpp.das`** (a visitor written in daslang). The old `src/ast/ast_aot_cpp.cpp` was emptied by commit `581363ebc` — do not edit it. When codegen output changes shape, edit `daslib/aot_cpp.das` and rebuild `libDaScriptAot`.

Key helpers used by the emitter:

- **`preVisitExprBlockFinal(blk)`** — emits `auto <finallyName> = das_finally([&]() { ... });` at the start of the block's finally region. The RAII guard fires on normal fall-through, `continue`, `break`, and `return`. For `for`/`while` loop bodies the guard sits **inside** the generated C++ body braces, so finally runs once per iteration — not once after the loop.
- **`das_finally`** — C++ RAII scope guard defined in the runtime. Destructor runs the captured finally lambda at any scope exit.
- **`finallyName(blk)`** — generates a unique C++ identifier for the per-block `das_finally` guard variable.

## The test_aot Binary

`tests/aot/CMakeLists.txt` defines the `test_aot` executable — a standalone binary with AOT stubs compiled in. It uses the same `main.cpp` as `daslang` but links additional AOT-generated object files.

### Structure

```
tests/aot/
    CMakeLists.txt          # Build rules for test_aot
    test_arithmetic.das     # AOT test: math, functions, function pointers
    test_strings.das        # AOT test: string operations
    test_lambdas.das        # AOT test: lambdas, captures, higher-order functions
    test_structures.das     # AOT test: structs, classes, arrays, tables
    _aot_generated/         # Auto-generated C++ (gitignored)
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
| `DAS_AOT_EXT(files, genList, target, tool, extra)` | Core macro — others call this | `-aot` + `extra` |
| `DAS_AOT_CTX(files, genList, target, tool)` | AOT with custom context | `-aot -ctx` |

**Target name collision**: `DAS_AOT_EXT` creates a custom target named `${mainTarget}_genaot`. Multiple calls with the same `mainTarget` will collide. Use distinct target names (e.g., `test_aot_testing` and `test_aot_tests`).

## Running AOT Tests

```bash
# Build the test_aot binary
cmake --build build --config Release --target test_aot

# Run all AOT tests (test_aot detects its own AOT stubs via is_in_aot())
bin/Release/test_aot.exe dastest/dastest.das -- --test tests/aot

# Run with the regular daslang binary (no AOT, interpreter mode)
bin/Release/daslang.exe dastest/dastest.das -- --test tests/aot
```

The `-use-aot` flag enables AOT for sub-compiled test files even when the host binary has stubs. But `test_aot` auto-detects via `is_in_aot()` in `dastest/suite.das`.

## Adding Tests to AOT — CRITICAL

**Every test directory under `tests/` must be registered in `tests/aot/CMakeLists.txt`**. The `test_aot` binary (built on Linux/macOS CI) runs ALL tests under `tests/` with AOT enabled. If a test directory's AOT stubs aren't generated and linked, `test_aot` will fail with `error[50101]: AOT link failed`.

This applies to ALL test directories (e.g., `tests/fio/`, `tests/fs/`, `tests/json/`), not just `tests/aot/`. See the "Registering a New Test Directory" section below.

**Do NOT use `options no_aot`** to suppress AOT link failures — register the tests properly in CMake instead.

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

2. Add the file to `tests/aot/CMakeLists.txt`:

```cmake
SET(AOT_TEST_FILES
    tests/aot/test_arithmetic.das
    tests/aot/test_strings.das
    tests/aot/test_lambdas.das
    tests/aot/test_structures.das
    tests/aot/test_foo.das          # <-- add here
)
```

3. Update `tests/README.md` with a row for the new file.

4. Rebuild: `cmake --build build --config Release --target test_aot`

> **Build timing**: Always use `timeout: 0` when running CMake builds — they can take 15-25 minutes for clean builds and 2-10 minutes for incremental builds. Never assume the build is stuck just because there's no output; MSVC is silent during compilation.

## How -use-aot / --use-aot Works

The AOT flag flows through three layers:

1. **`main.cpp`**: `-use-aot` sets `useAot = true`. Does NOT set `policies.aot` on the host (dastest framework functions don't have AOT stubs in every binary).
2. **`dastest/suite.das`**: `--use-aot` (or auto-detected via `is_in_aot()`) sets `ctx.use_aot = true`. When compiling test files, sets `cop.aot = true` and `cop.fail_on_no_aot = true`.
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
    ds_ast_boost.das.cpp
    ds_templates_boost.das.cpp
    ds_functional.das.cpp
    ds_math_boost.das.cpp
    ds_utf8_utils.das.cpp
    ds_printer_flags_visitor.das.cpp
    ... (11 modules total)
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

The generated files in `daslib/_aot_generated/` are tracked in git. Commit any changes.

## AOT Link Failures Are Real Failures

`test_aot` exists specifically to catch AOT failures, including **link failures** (error 50101). When a file like `_module_a.das` fails with "AOT link failed", that IS a real failure — it means the AOT stubs could not be linked at simulation time. Do NOT dismiss these as "not real tests" or "helper modules". If a `.das` file is picked up by dastest and fails under AOT, it needs to be fixed or excluded from the AOT test set.

Common causes of AOT link failures:
- Missing `Module::aotRequire()` headers
- Semantic hash mismatch (see below)
- Helper modules without `[test]` functions that weren't meant to run standalone — these should be excluded from the test directory or the CMake file list

## CI Integration

The AOT test step runs in `.github/workflows/build.yml` after the regular test step, Release builds only:

```yaml
- name: AOT Test
  if: matrix.preset != 'windows32' && matrix.config == 'Release'
  run: bin/${{ matrix.preset == 'windows' && 'Release/' || '' }}test_aot dastest/dastest.das -- --test tests/aot
```

## _aot_generated Directories

AOT-generated C++ files go into `_aot_generated/` subdirectories — all are gitignored:
- `daslib/_aot_generated/` — regenerated at build time by `DAS_AOT_LIB` (part of `libDaScriptAot`)
- `tests/aot/_aot_generated/` — build artifact for `test_aot`
- `dastest/_aot_generated/` — build artifact for `test_aot`

When creating a new AOT target that generates into a new directory, add it to `.gitignore`.
All `_aot_generated/` directories are covered by a single broad pattern in `.gitignore` — no need to add per-directory entries.

## Registering a New Test Directory for AOT

When you create a new test directory (e.g., `tests/foo/`), you MUST register it in `tests/aot/CMakeLists.txt` following this pattern:

**Step 1** — Add a glob to collect files (near the other `FILE(GLOB ...)` lines):
```cmake
# AOT for foo test files
FILE(GLOB AOT_FOO_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/foo/*.das")
```

**Step 2** — Add a custom target and AOT call (near the other `add_custom_target` blocks):
```cmake
add_custom_target(test_aot_foo)
SET(FOO_AOT_GENERATED_SRC)
DAS_AOT("${AOT_FOO_FILES}" FOO_AOT_GENERATED_SRC test_aot_foo daslang)
```

**Step 3** — Add a source group (near the other `SOURCE_GROUP_FILES` lines):
```cmake
SOURCE_GROUP_FILES("aot generated" FOO_AOT_GENERATED_SRC)
```

**Step 4** — Add `${FOO_AOT_GENERATED_SRC}` to the `add_executable(test_aot ...)` source list.

**Step 5** — Add `test_aot_foo` to the `ADD_DEPENDENCIES(test_aot ...)` list.

**Why this matters**: CI builds `test_aot` on Linux/macOS and runs ALL tests under `tests/` with AOT enabled (`cop.fail_on_no_aot = true`). Without registration, the test's functions won't have AOT stubs → `error[50101]: AOT link failed`. The `test_aot` target is only built on non-Windows platforms (`NOT WIN32` guard in root CMakeLists.txt), so this failure won't be caught locally on Windows.

Also ensure that wrapper functions in builtin `.das` files (like `src/builtin/fio.das`) are marked `[generic]` — otherwise AOT can't inline them and will try to link against a non-existent concrete stub from the builtin module.
