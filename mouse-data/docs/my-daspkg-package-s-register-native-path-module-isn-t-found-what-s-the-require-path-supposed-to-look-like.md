---
slug: my-daspkg-package-s-register-native-path-module-isn-t-found-what-s-the-require-path-supposed-to-look-like
title: My daspkg package's register_native_path module isn't found — what's the require path supposed to look like?
created: 2026-05-09
last_verified: 2026-05-18
links: []
---

`register_native_path(group, modname, file_path)` in a `.das_module` file registers a module path resolvable as **`require <group>/<modname>`** — the slash form is mandatory. A bare `require <modname>` (no slash) does NOT consult the dynamic-module table.

Example `.das_module`:

```das
[export]
def initialize(project_path : string) {
    register_native_path("imgui", "imgui_boost", "{project_path}/daslib/imgui_boost.das")
    register_native_path("imgui", "imgui_boost_v2", "{project_path}/widgets/imgui_boost.das")
}
```

User code:

```das
require imgui/imgui_boost           // ✓ resolves via dynamic-module table
require imgui/imgui_boost_v2        // ✓
require imgui_boost                 // ✗ falls through to FileAccess fallback,
                                    // typically fails with "missing prerequisite"
```

**Why:** the resolver in `src/simulate/fs_file_info.cpp` `FsFileAccess::getModuleInfo` only consults `g_dyn_modules_resolve` when the require string contains `/` (or `.`):

```cpp
auto np = req.find_first_of("./");
if ( np != string::npos ) {
    string top = req.substr(0,np);
    // ... walk g_dyn_modules_resolve, match mod.name == top, then mod.paths from_path
}
```

For bare names, only the `FileAccess::getModuleInfo` fallback runs (relative file lookup).

**Cross-module requires inside your package** also need the slash form:

```das
// widgets/imgui_widgets_builtin.das
require imgui/imgui_boost_v2 public        // ✓
require imgui_boost_v2                     // ✗
```

**The dynamic-module table is populated when:**
- `daslang-live` starts and walks `<dasroot>/modules/<each_pkg>/.das_module`
- `daslang -dasroot <root>` similarly walks via `require_dynamic_modules`
- daspkg install during build

**The MCP daslang server** supports daspkg-style modules via the `project_root` argument (added 2026-05-18; commit 26e1407c1). Every MCP tool that already takes `project` also accepts `project_root`, equivalent to daslang's `-project_root <dir>` CLI flag. For external dev work without `daspkg install`, point `project_root` at a dummy root with a `modules/<pkg>` junction — see [external_module_debugging.md](../../skills/external_module_debugging.md) for the workflow. Before that change, MCP couldn't resolve dynamic modules and end-to-end testing fell back to daslang-live or shell.

**Surfaced 2026-05-09** during dasImgui Phase 0a; spent ~30 minutes wondering why `require imgui_boost_v2` couldn't find the just-registered module. Adding the `imgui/` prefix fixed it instantly. Updated 2026-05-18 with MCP `project_root` support.

## Questions
- My daspkg package's register_native_path module isn't found — what's the require path supposed to look like?
