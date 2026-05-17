---
slug: how-do-i-run-a-dasimgui-demo-locally-what-s-the-build-daspkg-install-dance
title: How do I run a dasImgui demo locally — what's the build + daspkg install dance?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Verified 2026-05-16.

dasImgui is a **sibling repo** (`/Users/borisbatkin/Work/dasImgui`, not under `daScript/modules/`). To run any dasImgui example or test locally:

**1. Build daslang first** (provides `lib/liblibDaScriptDyn.dylib`, `bin/daslang`):
```
cd /Users/borisbatkin/Work/daScript
cmake --build build --config Release -j 8
```

**2. Build dasImgui out-of-tree, pointing DASLANG_DIR at daScript:**
```
cd /Users/borisbatkin/Work/dasImgui
mkdir -p build && cd build
cmake .. -DDASLANG_DIR=/Users/borisbatkin/Work/daScript -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build . --config Release -j 8
```
This drops `dasModuleImgui.shared_module`, `imguiApp.shared_module`, `imguiAppHeadless.shared_module` into `/Users/borisbatkin/Work/dasImgui/`.

**3. daspkg install (the critical step):**
```
cd /Users/borisbatkin/Work/daScript
bin/daslang utils/daspkg/main.das -- install /Users/borisbatkin/Work/dasImgui --global --force
```
This COPIES (not symlinks) the entire dasImgui repo into `/Users/borisbatkin/Work/daScript/modules/dasImgui/`. The `--global` flag puts it in the das_root modules dir so `require imgui` resolves. The `--force` re-syncs after every edit (without `--force`, repeat installs no-op).

**4. Run:**
```
bin/daslang modules/dasImgui/examples/imgui_demo/main.das
```
**Do NOT use `-project_root /path/to/dasImgui`** — that flag exists but doesn't trigger `.das_module` initialize for `register_dynamic_module`. The canonical resolver path is via `das_root/modules/<name>` populated by daspkg.

**Gotcha — edits don't propagate without re-install.** Editing a file in `/Users/borisbatkin/Work/dasImgui/widgets/foo.das` is invisible to daslang until you re-run `daspkg install --force`. Working directly in `/Users/borisbatkin/Work/daScript/modules/dasImgui/` skips the sync but means edits land in the daspkg-copied tree, not the source repo — easy to lose work.

**Compile-check via MCP** also needs the install: `mcp__daslang__compile_check` resolves `require imgui` from `daScript/modules/dasImgui/`, not from the dasImgui source repo directly.

## Questions
- How do I run a dasImgui demo locally — what's the build + daspkg install dance?
