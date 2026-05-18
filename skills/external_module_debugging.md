# Debugging an external daslang module locally

When you're iterating on a daslang module that lives OUTSIDE the main daslang repo — dasImgui, dasPUGIXML, dasSQLITE, dasCards, dasTelegram, your own daspkg package, etc. — you need a way to run/lint/test from a standalone `daslang.exe` (or via the MCP server) without a full `daspkg install`. The trick is the **dummy-project-root junction pattern**.

## The problem

Each external module ships a `.das_module` descriptor (e.g. `D:/DASPKG/dasImgui/.das_module`) whose `initialize(project_path)` callback registers native paths like `register_native_path("imgui", "imgui_widgets_builtin", "<P>/widgets/imgui_widgets_builtin.das")`. daslang only fires those callbacks when it can locate the module under a `<project_root>/modules/<name>/` directory and you pass `-project_root <project_root>` on the CLI (or `project_root` via the MCP tool).

So running `daslang.exe path/to/your-module/main.das` directly fails with `missing prerequisite 'imgui'` — daslang has no `modules/` tree to scan, and the `.das_module` `initialize()` never fires.

Running via `daspkg install` works but cuts off the live edit loop: every `.das` or C++ change needs a reinstall.

## The fix: dummy-project-root junction

Create a throwaway directory whose only contents are a `modules/<your-module>` junction (Windows) or symlink (POSIX) pointing at your dev checkout. Then use it as `-project_root`.

### Windows

```cmd
mkdir D:\IMGUI_DEMO\modules
mklink /J D:\IMGUI_DEMO\modules\dasImgui D:\DASPKG\dasImgui
```

`mklink /J` creates a directory junction — same effect as a symlink for daslang's purposes and doesn't need admin.

### POSIX

```sh
mkdir -p /tmp/IMGUI_DEMO/modules
ln -s /path/to/dasImgui /tmp/IMGUI_DEMO/modules/dasImgui
```

## Using it

Once the junction exists, the dummy root works with every tool that takes `-project_root` / `project_root`.

### daslang.exe CLI

```sh
daslang.exe -project_root D:/IMGUI_DEMO modules/dasImgui/examples/imgui_demo/harness_layout.das
```

### daslang-live

```sh
daslang-live.exe -project_root D:/IMGUI_DEMO modules/dasImgui/examples/imgui_demo/harness_layout.das
```

### MCP tools (preferred — keeps everything inside the editor)

Every MCP tool that already accepts `project` also accepts `project_root`. Pass it the dummy-root path:

```jsonc
mcp__daslang__compile_check {
  "file": "D:/DASPKG/dasImgui/examples/imgui_demo/layout.das",
  "project_root": "D:/IMGUI_DEMO"
}
```

Same shape for `lint`, `find_symbol`, `goto_definition`, `find_references`, `type_of`, `ast_dump`, `list_requires`, `program_log`, `list_module_api`, `describe_type`, `run_test`, `run_script`, `eval_expression`, `aot`, `live_launch`.

## Why a junction, not a copy

The junction is the dev checkout. Edits to the module's source (`.das` or `.cpp`) are picked up live — no sync step. A copy would need rebuilding on every change.

## When to use this

- Iterating locally on an external module's daslang code before pushing to CI.
- Iterating on the module's C++ binding — rebuild `dasModule<X>.shared_module`, the next MCP/daslang invocation picks it up (the MCP server itself does not load user-side bindings, so DLL stays unlocked between calls).
- Running the module's harnesses / integration tests outside the `daspkg install` flow.
- Cross-module lint/compile sanity checks before a PR.

## When NOT to use this

- Production daspkg installs — those use `daspkg install <pkg>` which sets up the same `<root>/modules/<pkg>/` layout natively.
- Anything that depends on a built dasModule being copied to a specific install path — junctions are paths, not file copies, and the C++ binary still lives in the dev checkout.

## Combining with `.das_project`

Both `project` (a `.das_project` file path) and `project_root` (a directory) can be set on the same MCP call or CLI invocation. daslang loads them independently — `.das_project` for custom `module_get` callbacks, `-project_root` for daspkg-style `modules/*/` discovery. Use both when the module has its own `.das_project` AND you want daspkg dispatch.

## Worked example: dasImgui

```cmd
:: One-time setup
mkdir D:\IMGUI_DEMO\modules
mklink /J D:\IMGUI_DEMO\modules\dasImgui D:\DASPKG\dasImgui

:: Direct CLI
daslang.exe -project_root D:/IMGUI_DEMO ^
  modules/dasImgui/examples/imgui_demo/harness_layout.das

:: Via MCP (inside Claude Code / editor)
mcp__daslang__compile_check {
  "file": "D:/DASPKG/dasImgui/examples/imgui_demo/layout.das",
  "project_root": "D:/IMGUI_DEMO"
}
```

Same pattern for any other daspkg-style module — adjust the `mklink` target and the `-project_root` path.

## Gotchas

- **NEVER put a junction inside `D:\Work\daScript\modules\`** (daslang's own modules/ tree). That's reserved for daslang's stdlib; user-side modules belong in the dummy root.
- **The module's `.das_module` initializer is invoked with `project_path = <dummy_root>/modules/<module_name>`** (the junction path). Inside the initializer, `{project_path}/widgets/foo.das` resolves through the junction to the real dev-checkout path. This is by design — the `.das_module` doesn't need to know it's being used via a junction.
- **On Windows, `mklink /J` is a junction (NTFS), not a symbolic link.** Junctions don't need admin; symlinks (`mklink /D` without `/J`) do. Junctions work for daslang's path resolution.
- **`-project_root` is independent of `-dasroot`.** `-dasroot` points at the daslang stdlib (`daslib/`); `-project_root` points at your user modules. They don't conflict.
