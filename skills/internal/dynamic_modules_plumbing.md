# Module plumbing (repo-only)

## Resolution

Both binaries try compiled-in `NATIVE_MODULE` rows first
(`#include "modules/external_resolve.inc"`, generated under `<build>/include`), the
`.das_module` scan second; the rows serve runs with no scan (`-no-dynamic-modules`, a host
that never calls `require_dynamic_modules`). Only the C++ arm splits: `daslang_static`
includes `external_pull.inc` and links `libDasModule*` statically; `daslang`
(`DAS_ENABLE_DLL`) dlopens `.shared_module` DLLs per `register_dynamic_module`.

## CMake macros

| Macro | Effect |
|---|---|
| `ADD_MODULE_LIB(libName dllName sources...)` | static lib + `dllName.shared_module` |
| `ADD_MODULE_CPP(ClassName)` | `NEED_MODULE`/`DECLARE_MODULE`/`PULL_MODULE` rows into the matching `external_*.inc`; descriptor: `register_dynamic_module` per class |
| `ADD_MODULE_DAS(category subfolder native)` | one `NATIVE_MODULE(category, subfolder, module_dir, native);` row; hand-written only for conditional rows (dasOpenGL's web-excluded `opengl`) |
| `ADD_MODULE_DAS_FROM_DESCRIPTOR(category subfolder)` | those rows derived from the descriptor's `register_native_path` per das file (`modules/<name>/daslib/` included); call once per (category, subfolder) |

Derivation candidates: the explicit `register_native_path("<category>", "<name>", ...)` calls
if the category has any, else every identifier-shaped quoted token in the file; a
multi-category descriptor writes explicit calls for all, else one category's names become
another's rows. A candidate becomes a row only if `<subfolder>/<name>.das` exists. A descriptor
edit re-derives (`CMAKE_CONFIGURE_DEPENDS`); until a rebuild, only the scan serves the new name.

## Gate

`REVIEW.das` calls `gate_descriptor_census(<descriptor>, [<folders>], [<non-module tokens>])`
(`dastest/review_gate.das`): disk ↔ descriptor equality both ways, one directory level, so name
every folder. Optional third argument: quoted tokens that are not module names (class names,
category strings).

## Debugging

- configure prints `<module>: N <category>/<subfolder> resolver rows derived from .das_module`
  — check N first; zero is a `FATAL_ERROR` (wrong `subfolder`, or `.das` renamed away from the
  descriptor names); a missing/renamed descriptor dies earlier, at `FILE(READ)`
- a failed `require foo/bar` means BOTH routes missed: descriptor absent or unscanned (module
  folder under neither dasroot nor `-project_root`), AND no compiled-in row (not
  reconfigured/rebuilt, or missing hand `ADD_MODULE_DAS`)
