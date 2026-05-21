# Debugging an external daslang module locally

When you're iterating on a daslang module that lives OUTSIDE the main daslang repo — dasImgui, dasPUGIXML, dasSQLITE, dasCards, dasTelegram, your own daspkg package, etc. — you need a way to run/lint/test from a standalone `daslang.exe` (or via the MCP server) without a full `daspkg install`.

The **recommended approach** is `-load_module <path>` (repeatable). The older **junction trick** still works and is documented below as a fallback for workflows that need to exercise the `<project_root>/modules/<name>` scan path itself.

## The problem

Each external module ships a `.das_module` descriptor (e.g. `D:/DASPKG/dasImgui/.das_module`) whose `initialize(project_path)` callback registers native paths like `register_native_path("imgui", "imgui_widgets_builtin", "<P>/widgets/imgui_widgets_builtin.das")`. daslang only fires those callbacks when it discovers the module — either by scanning `<project_root>/modules/<name>/` or via an explicit `-load_module <path>`.

So running `daslang.exe path/to/your-module/main.das` directly fails with `missing prerequisite 'imgui'` — daslang has no module-discovery hook firing, and the `.das_module` `initialize()` never runs.

Running via `daspkg install` works but cuts off the live edit loop: every `.das` or C++ change needs a reinstall.

## The recommended fix: `-load_module <path>`

Pass `-load_module <path>` to daslang.exe / daslang-live.exe. The path is the module folder itself (the one containing `.das_module`). Repeat the flag for multiple modules. No filesystem setup, no dummy root.

### daslang.exe CLI

```sh
daslang.exe -load_module D:/DASPKG/dasImgui your_script.das
```

For multiple modules:

```sh
daslang.exe -load_module D:/DASPKG/dasImgui -load_module D:/DASPKG/dasHV your_script.das
```

### daslang-live

```sh
daslang-live.exe -load_module D:/DASPKG/dasImgui your_script.das
```

### MCP tools (preferred — keeps everything inside the editor)

Every MCP tool that already accepts `project_root` also accepts `load_modules` (a JSON array of paths). Pass the module folder(s) directly:

```jsonc
mcp__daslang__compile_check {
  "file": "D:/DASPKG/dasImgui/examples/imgui_demo/layout.das",
  "load_modules": ["D:/DASPKG/dasImgui"]
}
```

Same shape for `lint`, `find_symbol`, `goto_definition`, `find_references`, `type_of`, `ast_dump`, `list_requires`, `program_log`, `list_module_api`, `describe_type`, `run_test`, `run_script`, `eval_expression`, `aot`, `live_launch`.

## Path semantics

- Each `-load_module <P>` is treated as a single module folder. daslang looks for `<P>/.das_module` and runs its `initialize(<P>)` callback.
- The path's **basename** becomes the shadow-skip key. If a module with the same basename also exists under `<project_root>/modules/<basename>/` or `<das_root>/modules/<basename>/`, the explicit `-load_module` entry wins (the others are skipped with a `local '<name>' shadows global` warning).
- Edits to the module's source (`.das` or `.cpp`) are picked up live — daslang reads from the path directly on every invocation.

## When to use this

- Iterating locally on an external module's daslang code before pushing to CI.
- Iterating on the module's C++ binding — rebuild `dasModule<X>.shared_module`, the next MCP/daslang invocation picks it up (the MCP server itself does not load user-side bindings, so DLL stays unlocked between calls).
- Running the module's harnesses / integration tests outside the `daspkg install` flow.
- Cross-module lint/compile sanity checks before a PR.

## When NOT to use this

- Production daspkg installs — those use `daspkg install <pkg>` which sets up the `<root>/modules/<pkg>/` layout natively, and the consuming code resolves modules without any extra flag.
- Anything that depends on a built dasModule being copied to a specific install path — `-load_module` is a path, not a file copy, and the C++ binary still lives in the dev checkout.

## Combining with `-project_root` / `.das_project`

All three can be set on the same call. They're independent:

- `-project` (a `.das_project` file) — custom `module_get` callbacks.
- `-project_root` (a directory) — scan its `modules/*` for `.das_module` files.
- `-load_module` (one path per flag, repeatable) — initialize this exact folder.

Precedence on basename collision: `-load_module` shadows `<project_root>/modules/<name>`, which shadows `<das_root>/modules/<name>`.

## Fallback: the junction trick

Before `-load_module`, the canonical workaround was to create a throwaway directory whose only contents are a `modules/<your-module>` junction (Windows) or symlink (POSIX) pointing at your dev checkout, then use it as `-project_root`. This still works — useful if you want to exercise the `<project_root>/modules/<name>` scan path itself (rather than the direct `init_dyn_modules` path).

### Windows

```cmd
mkdir D:\IMGUI_DEMO\modules
mklink /J D:\IMGUI_DEMO\modules\dasImgui D:\DASPKG\dasImgui
daslang.exe -project_root D:/IMGUI_DEMO your_script.das
```

`mklink /J` creates a directory junction — same effect as a symlink for daslang's purposes and doesn't need admin.

### POSIX

```sh
mkdir -p /tmp/IMGUI_DEMO/modules
ln -s /path/to/dasImgui /tmp/IMGUI_DEMO/modules/dasImgui
daslang -project_root /tmp/IMGUI_DEMO your_script.das
```

For most workflows `-load_module` is shorter and avoids the per-module mklink step.

## Worked example: dasImgui

Recommended:

```cmd
daslang.exe -load_module D:/DASPKG/dasImgui ^
  D:/DASPKG/dasImgui/examples/imgui_demo/harness_layout.das
```

Via MCP (inside Claude Code / editor):

```jsonc
mcp__daslang__compile_check {
  "file": "D:/DASPKG/dasImgui/examples/imgui_demo/layout.das",
  "load_modules": ["D:/DASPKG/dasImgui"]
}
```

Junction fallback (only when you need to exercise the modules/* scan path):

```cmd
mkdir D:\IMGUI_DEMO\modules
mklink /J D:\IMGUI_DEMO\modules\dasImgui D:\DASPKG\dasImgui
daslang.exe -project_root D:/IMGUI_DEMO D:/DASPKG/dasImgui/examples/imgui_demo/harness_layout.das
```

## Gotchas

- **The module's `.das_module` initializer is invoked with `project_path = <the path you passed to -load_module>`.** Inside the initializer, `{project_path}/widgets/foo.das` resolves against that path directly. (With the junction trick, the path is `<dummy_root>/modules/<module_name>` — which junctions to the real dev-checkout path.)
- **Shadow-skip uses path basenames.** `-load_module D:/DASPKG/dasImgui` shadows entries named `dasImgui` in dasroot and project_root. If you renamed the module folder locally (e.g. `dasImgui-experimental`), the basename mismatch means it won't shadow the canonical `dasImgui`.
- **`-load_module` is independent of `-dasroot`.** `-dasroot` points at the daslang stdlib (`daslib/`); `-load_module` points at user modules. They don't conflict.
- **NEVER put a junction inside `D:\Work\daScript\modules\`** (daslang's own modules/ tree). That's reserved for daslang's stdlib; user-side modules belong outside via `-load_module` or a dummy root.
- **On Windows, `mklink /J` is a junction (NTFS), not a symbolic link.** Junctions don't need admin; symlinks (`mklink /D` without `/J`) do. Junctions work for daslang's path resolution.
