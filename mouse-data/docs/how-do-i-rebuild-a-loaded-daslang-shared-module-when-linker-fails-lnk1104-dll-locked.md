---
slug: how-do-i-rebuild-a-loaded-daslang-shared-module-when-linker-fails-lnk1104-dll-locked
title: How do I rebuild dasModuleImgui (or another loaded daslang shared module) when the linker fails with LNK1104 because MCP / daslang-live has the DLL mapped?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Rename the locked output, then build.** Windows lets you rename a file that's been `LoadLibrary`'d (the open handle keeps using the old path's inode), so MSBuild can write a fresh file at the original name. Existing daslang processes keep using the old code until they restart.

```powershell
cd D:/Work/daScript/modules/dasImgui
mv dasModuleImgui.shared_module dasModuleImgui.shared_module.locked
cmake --build build --config Release --target dasModuleImgui -j 8
# new dasModuleImgui.shared_module exists; .locked stays until MCP restarts
rm dasModuleImgui.shared_module.locked   # optional, after MCP restart
```

**Important caveats:**

- The MCP server (and any other daslang process that did a `require imgui`-chain compile) keeps the old code in its address space. **Fresh daslang subprocesses load the new code** — so `bin/Release/daslang.exe -compile-only path/to/file.das` from a shell sees the new behavior, but `mcp__daslang__compile_check` (in-process to MCP) still sees the old. Restart MCP to refresh.
- Test by spawning a fresh subprocess first to confirm the rebuild took effect, *then* ask the user to restart MCP — saves a kill cycle if the build actually regressed.
- Don't try to delete the `.locked` file before MCP restarts. Windows blocks `rm` of a memory-mapped file until every loader process releases it. To identify which process is holding a given DLL: `Get-Process | Where-Object { $_.Modules.FileName -like "*dasModuleImgui*" }` (uses the `System.Diagnostics.Process.Modules` collection, NOT the `Win32_Process` CIM class — the latter doesn't expose loaded modules).

**Alternative: ask the user to kill MCP first.** Cleaner but ends the session — every subsequent `mcp__*` call fails until they restart. Use the rename trick when you want to keep working while iterating on a C++ change.

**Default arg pre-check.** Before kicking off a multi-minute rebuild, run `Get-CimInstance Win32_Process -Filter "name='daslang.exe'" | Select ProcessId, CommandLine | Format-List` to see who's holding modules. Often it's only `utils/mcp/main.das` + `utils/mouse/main.das`; both can be restarted by the user. If a `daslang-live` is open, the rename trick is the *only* path that doesn't kick the user out of their live session.

**Found 2026-05-15** during dasImgui ImGuiTextFilter binding patch work — needed three rebuild cycles (one per discovered override), MCP was holding the DLL each time.

## See also
- `feedback_kill_before_build.md` — companion: which processes to kill if you go the kill-first route
- `feedback_never_rm_build.md` — DON'T nuke `build/` to "fix" a lock; libhv + OpenSSL take an hour

## Questions
- How do I rebuild dasModuleImgui (or another loaded daslang shared module) when the linker fails with LNK1104 because MCP / daslang-live has the DLL mapped?
