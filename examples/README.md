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

## test/ — Test Harness & Unit Tests

The compiler/runtime test suite (`daScriptTest`). Contains unit tests,
compilation failure tests, runtime error tests, optimization tests, and
module-specific test directories. This is a C++ test executable — not
standalone `.das` scripts. Will be reorganized later.
