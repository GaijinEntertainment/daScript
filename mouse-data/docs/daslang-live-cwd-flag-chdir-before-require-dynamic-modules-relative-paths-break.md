---
slug: daslang-live-cwd-flag-chdir-before-require-dynamic-modules-relative-paths-break
title: Why does daslang-live with relative `-project_root` fail to resolve daspkg modules?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**daslang-live's `-cwd` flag changes the working directory to the script's folder BEFORE `require_dynamic_modules` runs**, so any relative `-project_root <P>` passed alongside it gets re-resolved against the *new* cwd and fails to find `<P>/modules/`.

Code path (utils/daslang-live/main.cpp):
- Line 696-707: `-cwd` handler calls `SetCurrentDirectoryA(dir)` / `chdir(dir)` where `dir = dirname(scriptFile)`.
- Line 725-730: AFTER chdir, `require_dynamic_modules(access, getDasRoot(), project_root, tout)` runs. If `project_root` was relative, it now points at `<new_cwd>/<relative_project_root>` — which is almost never what the caller meant.

Concrete failure mode: spawning daslang-live with `-cwd -project_root utils/mcp/tests/_pretend_root <abs>/consumer.das` chdir's to `utils/mcp/tests/_pretend_root/`, then tries to scan `utils/mcp/tests/_pretend_root/utils/mcp/tests/_pretend_root/modules/` (the relative path doubled). Result: `error[20605]: missing prerequisite 'pretend_mod/greet'; file not found` even though the registration would succeed with the absolute path.

**Fix on the caller side**: resolve all paths to absolute BEFORE composing daslang-live's argv. In `utils/mcp/tools/live.das` `do_live_launch`:

```das
let abs_file = resolve_path(file)
let abs_project = empty(project) ? "" : resolve_path(project)
let abs_project_root = empty(project_root) ? "" : resolve_path(project_root)
let script_args = build_live_script_args(abs_file, abs_project, abs_project_root, is_windows)
```

`resolve_path()` lives in `utils/mcp/tools/common.das` and joins relative paths to `get_das_root()`.

Surfaced 2026-05-18 during MCP test_live_tools.das implementation; the bug was latent until anyone actually fed `do_live_launch` a relative project_root (real-world callers had been passing absolute paths). Fix shipped in PR for the same project_root work.

**Same gotcha applies to plain `daslang.exe`?** No — daslang.exe doesn't have a `-cwd` flag, so the cwd stays wherever the caller spawned from and relative `-project_root` resolves naturally.

## Questions
- Why does daslang-live with relative `-project_root` fail to resolve daspkg modules?
- What happens when daslang-live `-cwd` and `-project_root` are both set with relative paths?

## Questions
- Why does daslang-live with relative `-project_root` fail to resolve daspkg modules?
