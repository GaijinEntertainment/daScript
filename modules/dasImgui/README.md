# dasImgui

[Dear ImGui](https://github.com/ocornut/imgui) bindings for [daslang](https://dascript.org/).

Provides the `imgui` and `imgui_app` modules for building GUI applications with daslang.

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

```das
options gen2

require imgui_app
require imgui/imgui_boost
require glfw/glfw_boost

[export]
def main() {
    imgui_app("My App") <| $() {
        NewFrame()
        if (Begin("Hello", null, ImGuiWindowFlags.None)) {
            Text("Hello from daslang!")
        }
        End()
        Render()
    }
}
```

Run with `-project_root` pointing to the directory containing `modules/`:

```bash
daslang.exe -project_root . my_app.das
```

## Modules

| Module | Require | Description |
|--------|---------|-------------|
| `imgui` | `require imgui/imgui_boost` | Core Dear ImGui bindings |
| `imgui_app` | `require imgui_app` | GLFW + OpenGL3 application runtime |

## Examples

- `example/imgui_demo.das` — full ImGui demo window
- `example/imgui_opengl2.das` — minimal OpenGL example

## imgui version

v1.90.6-docking (fetched via CMake FetchContent at build time).

## License

MIT
