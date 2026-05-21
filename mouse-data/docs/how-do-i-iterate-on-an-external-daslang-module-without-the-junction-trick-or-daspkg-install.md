---
slug: how-do-i-iterate-on-an-external-daslang-module-without-the-junction-trick-or-daspkg-install
title: How do I iterate on an external daslang module without the junction trick or daspkg install?
created: 2026-05-20
last_verified: 2026-05-20
links: []
---

Use **`-load_module <path>`** (repeatable). Each path is the module folder itself (the one containing `.das_module`); daslang runs that folder's `initialize(<path>)` directly, skipping the `<project_root>/modules/<name>` scan. No junction, no daspkg install.

```sh
daslang.exe -load_module D:/DASPKG/dasImgui your_script.das
daslang.exe -load_module D:/DASPKG/dasImgui -load_module D:/DASPKG/dasHV your_script.das
daslang-live.exe -load_module D:/DASPKG/dasImgui your_live.das
```

**MCP equivalent**: every tool that takes `project_root` also takes `load_modules` (JSON array of paths):

```jsonc
mcp__daslang__compile_check {
  "file": "your_script.das",
  "load_modules": ["D:/DASPKG/dasImgui"]
}
```

**Available across**: `compile_check`, `lint`, `find_symbol`, `goto_definition`, `find_references`, `type_of`, `ast_dump`, `list_requires`, `program_log`, `list_module_api`, `describe_type`, `run_test`, `run_script`, `eval_expression`, `aot`, `live_launch`.

**Path semantics**: the path's basename becomes the shadow-skip key. If the same name (basename) appears under `<project_root>/modules/` or `<das_root>/modules/`, the `-load_module` entry wins with a `local 'name' shadows global` warning. Precedence: load_module > project_root > dasroot.

**The C++ join point** is `require_dynamic_modules(file_access, das_root, project_root, load_modules, tout)` in [src/ast/dyn_modules.cpp](../../src/ast/dyn_modules.cpp). The `-load_module` paths are also exposed via the C API as `das_register_dynamic_modules(file_access, project_root, load_module_paths, num_load_module_paths, tout)` in [src/misc/daScriptC.cpp](../../src/misc/daScriptC.cpp).

**Works with modules that register C++ shared modules too** (dasImgui, dasHV, dasPUGIXML, etc.) — the `.das_module/initialize()` callback fires the same way `-project_root` would. Caveat: the module's `.shared_module` DLLs must be ABI-compatible with the daslang.exe you're running. If you see `missing prerequisite 'imgui'` (or similar) after the `.das_module` initialize prints fire, the shared modules are stale — rebuild the module's CMake project against your current daslang sources:

```sh
cd D:/DASPKG/dasImgui
cmake -B build -DDASLANG_DIR=D:/Work/daScript -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

This re-emits `dasModuleImgui.shared_module` etc. into the module folder, which `-load_module` then picks up directly. No `daspkg install` step needed for the daslang-side iteration loop.

**Legacy alternative**: the junction trick (`<DummyRoot>/modules/<name>` symlink + `-project_root <DummyRoot>`) still works — see [external_module_debugging.md](../../skills/external_module_debugging.md). Useful when you specifically want to exercise the `<project_root>/modules/*` scan path.

## Questions
- How do I iterate on an external daslang module without the junction trick or daspkg install?
- What does `-load_module <path>` do?
- What's the difference between `-load_module` and `-project_root`?
- How do I pass multiple modules to daslang.exe?
- What's the MCP equivalent of `-load_module`?
