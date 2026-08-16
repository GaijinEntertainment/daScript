# Module plumbing — static binary and CMake (repo-only)

Read this before adding a module under this repo's `modules/` tree. Descriptor authoring
(`.das_module` format, the registration calls) is `skills/dynamic_modules.md`; this file
is the build side.

## Two binaries, two resolution paths

| Binary | Modules resolve via |
|---|---|
| `daslang_static` | `NATIVE_MODULE` macros in the generated `modules/external_resolve.inc` + static linking of `libDasModule*` |
| `daslang` (default) | `.das_module` descriptors + `.shared_module` DLLs |

The static binary never reads `.das_module`. A new module therefore needs **both** the
CMake registration and the descriptor, and works-in-one-binary is the usual symptom of
having done only half.

## CMake macros

- `ADD_MODULE_LIB(libName dllName sources...)` — static lib plus `dllName.shared_module`
- `ADD_MODULE_CPP(ClassName)` — appends `NEED_MODULE(Module_ClassName);` to `external_need.inc`
- `ADD_MODULE_DAS(category subfolder native)` — appends
  `NATIVE_MODULE(category, subfolder, module_dir, native);` to `external_resolve.inc`

Every `.das` file of a pure-das module needs its own `ADD_MODULE_DAS` line, exactly as it
needs its own `register_native_path` line.

## Checklist per module type

| Module | CMake | `.das_module` |
|---|---|---|
| C++ | `ADD_MODULE_LIB` + `ADD_MODULE_CPP` | `register_dynamic_module` per C++ class |
| Pure-das | `ADD_MODULE_DAS` per file | `register_native_path` per file |
| Mixed | both | both |
| `daslib/*.das` | none | none — resolved by directory convention |

## Debugging

If `require foo/bar` fails under `daslang_static`, the missing piece is the
`ADD_MODULE_DAS` line plus a rebuild; under `daslang`, it is the descriptor entry.
