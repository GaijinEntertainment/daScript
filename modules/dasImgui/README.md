# dasImgui

![license](https://img.shields.io/badge/license-MIT-blue)

[Dear ImGui](https://github.com/ocornut/imgui) bindings for [daslang](https://daslang.io/) —
part of the daslang tree, built in by default.

Provides the `imgui` binding, the v2 `widgets/` macro layer (`[widget]`/`[container]`/`with_*`), and the `imgui_harness` runtime for building GUI applications with daslang.

## Part of the daslang tree

dasImgui lives at `modules/dasImgui` and builds with the normal daslang build —
no package install. It is enabled by default (root CMake option
`DAS_IMGUI_DISABLED`, default `OFF`) and needs the in-tree dasGlfw and
dasClipboard modules (also on by default).

Projects whose `.das_package` declares `require_package("dasImgui")` keep
working unchanged: `daspkg install` recognizes the in-tree module and reports
it as *part of this daslang tree — nothing to install*.

The previous standalone repo ([github.com/borisbat/dasImgui](https://github.com/borisbat/dasImgui))
is archived with full history; development continues in the daslang tree.

### Requirements

- CMake 3.16+
- C++17 compiler (MSVC, GCC, Clang)
- OpenGL

## Usage

The canonical pattern uses `imgui/imgui_harness` — it hides the GLFW/GL backend
boilerplate behind five helpers, re-exports the backend-agnostic v2 stack, and
supports `--headless` for tests and CI. See the [imgui tutorials](https://daslang.io/doc/reference/tutorials/imgui/index.html)
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
when the script's own logic calls `request_exit()`. See the
[headless-mode tutorial](https://daslang.io/doc/reference/tutorials/imgui/harness_headless_mode.html)
for what gets dispatched in either mode and the limits (`screenshot` /
`record_*` and the live-API HTTP endpoint stay windowed-only).

Run scripts with the tree's `daslang` binary from the daslang repo root
(the in-tree recipes pass `-project_root .`); external daspkg projects keep
the usual `-project_root <project>` invocation.

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
- `examples/features/with_indent.das` — smallest single-file harness example (drives [test_with_indent.das](../../modules/dasImgui/tests/test_with_indent.das))
- `examples/features/` — 90+ small focused demos, one widget/helper per file
- `examples/imgui_demo/imgui_demo.das` — full Dear ImGui demo port (90+ scenes)
- `examples/tutorial/` — annotated step-by-step tutorials matching the [docs site](https://daslang.io/doc/reference/tutorials/imgui/index.html)
- `examples/save_demo/` — save/load round-trip demo

## Tests

Tests live at the repo root under [`modules/dasImgui/tests/`](../../modules/dasImgui/tests/)
and run in the nightly CI lane (`nightly_imgui.yml`).

## imgui version

v1.92.6-docking (fetched via CMake FetchContent at build time).

## Documentation

Published at [daslang.io/doc](https://daslang.io/doc) as part of the main
daslang Sphinx tree: the [GUI stdlib section](https://daslang.io/doc/stdlib/sec_imgui.html)
and the [imgui tutorials](https://daslang.io/doc/reference/tutorials/imgui/index.html)
(sources under `doc/source/stdlib/` and `doc/source/reference/tutorials/imgui/`).

Local build, from the daslang root:

```bash
daslang modules/dasImgui/utils/imgui2rst.das
sphinx-build -b html doc/source doc/_build/html
```

The first step runs the RST emitter (parallel to daslang's `das2rst`) — the
imgui modules carry macro-generated surfaces das2rst's reflection pass can't
group — writing the GUI stdlib pages into `doc/source/stdlib/generated/`
(gitignored). Re-run it whenever a public `//!` comment changes. Tutorial
recordings are not in git; they stage from the rolling `docs-assets` GitHub
release via `utils/docs_assets/fetch.sh` / `fetch.ps1`. CI builds Sphinx with
`-W` (warnings-as-errors); a local `--keep-going` invocation is enough for
spot-checking.

## License

MIT
