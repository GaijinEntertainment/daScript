# Examples

Runnable daslang examples organized by topic.

## Getting Started

```
daslang.exe examples/hello_world.das
```

| File | Description |
|------|-------------|
| `hello_world.das` | Minimal hello-world program |

## debugapi/ — Debug Agent Examples

Debug agents let you inspect and control program execution at runtime:
allocation tracking, breakpoints, instrumentation, and stack walking.

All examples require `options debugger = true`.

| File | Description |
|------|-------------|
| `allocation_tracking.das` | Heap allocation callbacks (`onAllocate`, `onReallocate`, `onFree`, `onAllocateString`, `onFreeString`) |
| `heartbeat.das` | Periodic callback injection via `daslib/heartbeat` — watchdog, progress reporting |
| `hw_breakpoint.das` | Hardware data breakpoints that fire when a memory address is written |
| `instrumentation.das` | Line-level and function-level instrumentation hooks for profiling and tracing |
| `stack_walker.das` | `DapiStackWalker` for call-stack inspection and diagnostic collection |

## dasbind/ — Foreign Function Interface

| File | Description |
|------|-------------|
| `dasbind_example.das` | Calling external C functions (Win32 API) via `[extern]` annotations |

## pathTracer/ — Path Tracer Demo

A toy path tracer implemented in daslang, with OpenGL visualization variants.
Requires the `stbimage` module and (for OpenGL variants) `dasGlfw` / `dasImgui`.

| File | Description |
|------|-------------|
| `path_tracer.das` | Core path-tracing module (shared code) |
| `toy_path_tracer.das` | Console path tracer — renders to a `.bmp` file |
| `toy_pathtracer_opengl_basic.das` | Real-time path tracer with basic OpenGL display |
| `toy_path_tracer_opengl.das` | Real-time path tracer with progressive accumulation |
| `toy_path_tracer_opengl_hdr.das` | Real-time path tracer with HDR tonemapping |
| `toy_path_tracer_profile.das` | Path tracer performance benchmark |

## profile/ — Performance Benchmarks

A benchmark suite comparing daslang against C++ and other languages.
Built as a C++ executable that compiles and runs `.das` benchmark scripts.

## uncategorized/ — Miscellaneous Examples

Standalone examples that don't fit neatly into other categories.

| File | Description |
|------|-------------|
| `base64_generators.das` | Base64 encoder built as a chain of composable generators (deliberately slow — demonstrates generator composition, not production encoding) |
| `shader_like_validation.das` | Shader-like mode demo — shows how `options shader_like` rejects heap-allocating constructs (arrays, lambdas, `new`) at simulation time |

## hv/ — WebSocket Chat Example

A WebSocket chat room powered by the `dashv` module.
The server serves an HTML page and speaks WebSocket on the same port.

| File | Description |
|------|-------------|
| `ws_chat_server.das` | Chat server — serves `chat.html` at `/`, WebSocket at `/chat` |
| `ws_chat_client.das` | Terminal chat client — type messages in the console |
| `chat.html`          | Standalone browser chat client |

Start the server, open `http://localhost:9090` in a browser,
and/or run `ws_chat_client.das` in a terminal — all clients share the
same chat room. See `examples/hv/README.md` for details.

## daspkg/ — Package Manager Examples

Example projects demonstrating the `daspkg` package manager. Each subdirectory
is a self-contained project with a `.das_package` descriptor.

| Directory | Description |
|-----------|-------------|
| `daspkg-example/` | Minimal project that installs a package from the index |
| `daspkg-build-example/` | Project with a C++ build step (`cmake_build`) |
| `daspkg-version-1/` | Package versioning — pinning to version 1.x |
| `daspkg-version-2/` | Package versioning — pinning to version 2.x |
| `packages/daspkg-example-c/` | Template for a daspkg package with C sources |
| `packages/daspkg-example-cpp/` | Template for a daspkg package with C++ shared module |
| `packages/daspkg-example-mixed/` | Template for a mixed daslang + C++ package |

## telegram/ — Telegram Bot Example

A Telegram echo bot using the `das-telegram` daspkg package. Requires setup:

```
cd examples/telegram
daslang.exe ../../utils/daspkg/main.das -- install
```

| File | Description |
|------|-------------|
| `echo_bot.das` | Telegram echo bot — replies with the same message |

## claude/ — Claude API Example

A daslang helper bot using the `das-claude` daspkg package (Anthropic API bindings). Requires setup:

```
cd examples/claude
daslang.exe ../../utils/daspkg/main.das -- install
```

| File | Description |
|------|-------------|
| `daslang_helper_bot.das` | Interactive Claude-powered daslang coding assistant |

## graphics/ — ImGui Examples

ImGui examples using the `dasImgui` daspkg package. Requires setup:

```
cd examples/graphics
daslang.exe ../../utils/daspkg/main.das -- install
daslang.exe -project_root . furier_opengl_imgui_example.das
```

| File | Description |
|------|-------------|
| `furier_opengl_imgui_example.das` | Fourier series visualization with ImGui controls |

## node-editor/ — Node Editor Example

[imgui-node-editor](https://github.com/thedmd/imgui-node-editor) example using
`dasImguiNodeEditor` and `dasImgui` daspkg packages. Requires setup:

```
cd examples/node-editor
daslang.exe ../../utils/daspkg/main.das -- install
daslang.exe -project_root . imgui_node_editor_basic.das
```

| File | Description |
|------|-------------|
| `imgui_node_editor_basic.das` | Two linked nodes in a fullscreen node-editor canvas |

