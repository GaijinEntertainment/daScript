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

## Adding a New AOT Test

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
