# dasImgui

[![tests](https://github.com/borisbat/dasImgui/actions/workflows/tests.yml/badge.svg)](https://github.com/borisbat/dasImgui/actions/workflows/tests.yml)
[![docs](https://github.com/borisbat/dasImgui/actions/workflows/docs.yml/badge.svg)](https://github.com/borisbat/dasImgui/actions/workflows/docs.yml)
[![docs site](https://img.shields.io/badge/docs-live-blue)](https://borisbat.github.io/dasImgui)
![license](https://img.shields.io/badge/license-MIT-blue)

[Dear ImGui](https://github.com/ocornut/imgui) bindings for [daslang](https://dascript.org/).

Provides the `imgui` binding, the v2 `widgets/` macro layer (`[widget]`/`[container]`/`with_*`), and the `imgui_harness` runtime for building GUI applications with daslang.

## Install

```bash
daslang.exe utils/daspkg/main.das -- install github.com/borisbat/dasImgui
```

Or add to your project's `.das_package`:

```das
[export]
def dependencies(version : string) {
    require_package("github.com/borisbat/dasImgui")
}
```

Then run `daspkg install`.

## Build

The C++ build step runs automatically during `daspkg install`. To rebuild manually:

```bash
daspkg build dasImgui
```

Or with CMake directly:

```bash
cmake -B modules/dasImgui/_build -S modules/dasImgui -DDASLANG_DIR=<path-to-daslang-root>
cmake --build modules/dasImgui/_build --config Release
```

### Requirements

- daslang SDK (built with `DAS_GLFW_DISABLED=OFF`)
- CMake 3.16+
- C++17 compiler (MSVC, GCC, Clang)
- OpenGL

## Usage

The canonical pattern uses `imgui/imgui_harness` — it hides the GLFW/GL backend
boilerplate behind five helpers, re-exports the backend-agnostic v2 stack, and
supports `--headless` for tests and CI. See the [dasImgui tutorials](https://borisbat.github.io/dasImgui/tutorials/index.html)
starting at `boost_basics` for a complete walkthrough.

```das
options gen2

require imgui/imgui_harness

[export] def init() {
    harness_init("My Example", 800, 600)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export] def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    window(MAIN_WIN, (text = "Hello")) {
        text("Hello from daslang!")
    }
    harness_end_frame()
}

[export] def shutdown() { harness_shutdown() }

[export] def main() {
    init()
    while (!exit_requested()) { update() }
    shutdown()
}
```

Same script runs headless (no window, no GL context) by passing
`--headless` after the daslang `--` separator:

```bash
daslang.exe my_example.das -- --headless --headless-frames=600
```

Headless mode skips the GLFW + OpenGL chain entirely (the harness uses a
parallel `imguiAppHeadless.shared_module` C++ backend with a CPU-only
font atlas). `--headless-frames=N` auto-exits after `N` frames; omit it
when the script's own logic calls `request_exit()`. See
[doc/source/tutorials/harness_headless_mode.rst](doc/source/tutorials/harness_headless_mode.rst)
for what gets dispatched in either mode and the limits (`screenshot` /
`record_*` and the live-API HTTP endpoint stay windowed-only).

Run with `-project_root` pointing to the directory containing `modules/`:

```bash
daslang.exe -project_root . my_app.das
```

## Modules

| Module | Require | Description |
|--------|---------|-------------|
| `imgui_harness` | `require imgui/imgui_harness` | Canonical wrapper for apps/examples/tests — hides GLFW/GL boilerplate, re-exports the backend-agnostic v2 stack, dispatches windowed vs `--headless` at runtime |
| `imgui` | `require imgui` | Core Dear ImGui bindings (raw surface; most code goes through the v2 `widgets/` macro layer rather than calling these directly) |
| `imgui_app` | (used by harness) | GLFW + OpenGL3 application runtime |
| `imgui_app_headless` | (used by harness) | Display-less ImGui backend (CPU font atlas, no GLFW, no GL) for `--headless` runs |
| `imgui_terminal` | `require imgui/imgui_terminal` | Interactive cell renderer for the renderless `dasTerminal` snapshot/PTY module, with keyboard, paste, resize, scrollback, selection, and clipboard routing |

## Examples

- `examples/features/embedded_terminal.das` - live PowerShell/ConPTY terminal view (also runs through the headless harness)
- `examples/features/with_indent.das` — smallest single-file harness example (drives [test_with_indent.das](tests/integration/test_with_indent.das))
- `examples/features/` — 90+ small focused demos, one widget/helper per file
- `examples/imgui_demo/imgui_demo.das` — full Dear ImGui demo port (90+ scenes)
- `examples/tutorial/` — annotated step-by-step tutorials matching the [docs site](https://borisbat.github.io/dasImgui/tutorials/index.html)
- `examples/save_demo/` — save/load round-trip demo

## imgui version

v1.92.6-docking (fetched via CMake FetchContent at build time).

## Documentation

Published at https://borisbat.github.io/dasImgui (built by `.github/workflows/docs.yml` on push to master).

Local build:

```bash
daslang.exe utils/imgui2rst.das -- --detail_output doc/source/stdlib/generated
sphinx-build -b html doc/source doc/_build/html
```

The first step runs the RST emitter (parallel to daslang's `das2rst`); the
`--detail_output` flag points the per-function `//!` detail files at this
tree's `generated/detail/` so module pages render the captured docstrings.
The second step invokes Sphinx.

`doc/source/stdlib/generated/` and `doc/_build/` are gitignored. Tracked
inputs are `doc/source/conf.py`, `daslang.py`, `index.rst`, the section
landings (`sec_*.rst`), `external_types.rst`, `handmade/module-*.rst`,
`tutorials/`, and the emitter itself (`utils/imgui2rst.das`).

Re-run `utils/imgui2rst.das` whenever a public `//!` comment changes. CI runs `-W` (warnings-as-errors); the local `--keep-going` invocation is enough for spot-checking.

## License

MIT
