---
slug: how-do-i-ship-aot-stubs-for-an-out-of-tree-daslang-project-via-a-companion-shared-module-loaded-at-runtime
title: How do I ship AOT stubs for an out-of-tree daslang project via a companion shared_module loaded at runtime?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Pattern: have a host program (using daslang SDK + `find_package(DAS)`) that does `compile_file` with `cop.aot=true` on user `.das` files. The AOT C++ stubs need to be loaded *before* the inner `simulate` calls `linkCppAot` / `getGlobalAotLibrary`. Two AOT-specific traps in addition to the obvious "generate the cpps and link them":

**1. Drive AOT generation via `daslang utils/aot/main.das`, not the standalone `aot.exe`**

The standalone `aot.exe` (built via `standalone_aot`) skips daslang's `require_dynamic_modules` scan, so any `require <dynamic_module_name>` in a test/script fails with `error[20605]: missing prerequisite 'X'; file not found`. Driving the same script through daslang preserves the auto-discovery of `<sdk>/modules/*/.das_module`:

```cmake
add_custom_command(
    OUTPUT ${AOT_GEN_SRC}
    COMMAND $<TARGET_FILE:DAS::daslang> "${DAS_SDK_ROOT}/utils/aot/main.das" --
            -in_file "${AOT_MANIFEST}" -force-overwrite
    ...
)
```

The manifest file is one `aot <in.das> <out.cpp>` line per script.

**2. Companion `.shared_module` MUST link `DAS::libDaScriptDyn`, not `DAS::libDaScript`**

The AOT stubs register themselves into `AotListBase::head` via static initializers fired on `dlopen`. `AotListBase::head` is a global owned by libDaScript. If your companion module links the **static** `DAS::libDaScript` (a.k.a. `liblibDaScript.a`), the static initializers register into the companion module's *own private copy* of `head` — invisible to the daslang host. Symptom: AOT cpp content looks correct (matching hashes in the file comments), `testProfileAot.shared_module` is dlopened cleanly, but `linkCppAot` still reports `error[50101]: AOT link failed`.

Daslang's `bin/daslang` binary uses `liblibDaScriptDyn.dylib` (otool confirms `@rpath/liblibDaScriptDyn.dylib`). Linking your companion against `DAS::libDaScriptDyn` so it shares the same dylib gives a single `AotListBase::head` instance.

On macOS also pass `-undefined dynamic_lookup` so the AOT cpps' references to user C++ helpers (e.g. `testFibI`) defer to dlopen-time resolution from the sibling shared module that defines them — don't compile the binding cpp into the AOT companion (double `REGISTER_DYN_MODULE`, double `MAKE_TYPE_FACTORY`, and inline-variable replication across dylibs gets messy).

**3. The companion needs its own `Module` class so `register_dynamic_module` finds an entrypoint**

```cpp
#include "daScript/daScript.h"
using namespace das;
class Module_TestProfileAot : public Module {
public:
    Module_TestProfileAot() : Module("testProfileAot") {}
};
REGISTER_DYN_MODULE(Module_TestProfileAot, testProfileAot);
```

Note `REGISTER_DYN_MODULE` token-pastes `module_##ClassName` — pass the unqualified class name (`Module_TestProfileAot`), not `das::Module_TestProfileAot`, or you get `module_das::Module_TestProfileAot` (invalid).

**4. Wire the companion into `.das_module`**

```das
[export]
def initialize(project_path : string) {
    if (das_is_dll_build()) {
        register_dynamic_module("{project_path}/testProfile.shared_module", "testProfile")
        register_dynamic_module("{project_path}/testProfileAot.shared_module", "testProfileAot")
    }
}
```

`register_dynamic_module` (2-arg variant) is silent on failure — so first build, before the companion exists, doesn't break anything.

**5. SDK header gap: `daScript/builtin/ast_gen.inc` is missing**

If any of your scripts transitively `require daslib/ast` (e.g. via `daslib/unroll`), the generated cpp `#include`s `daScript/simulate/aot_builtin_ast.h` → `daScript/builtin/ast_gen.inc` — not shipped in the SDK install. Workaround: add the daslang source-tree `include/` as a fallback include path on the AOT module target.

**Host main.das wiring (for completeness)**

Per-mode CodeOfPolicies inside `compile_and_run`:
```das
if (useJit) {
    cop.jit_enabled = jit_enabled()
    if (jit_enabled()) {
        access |> add_extra_module("just_in_time", "{get_das_root()}/daslib/just_in_time.das")
    }
} elif (useAot) {
    cop.aot = true
    cop.fail_on_no_aot = true     // not cop.aot_module — that's the AOT generation flag
}
```

Reference: dasProfile's CMakeLists.txt is a working example.

## Questions
- How do I ship AOT stubs for an out-of-tree daslang project via a companion shared_module loaded at runtime?
