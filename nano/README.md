# libDaScriptNano

The daslang runtime with no compiler in it - for programs that run
already-compiled daslang and never compile any.

You compile your script to C++ ahead of time on your workstation:

```bash
daslang utils/aot/main.das -- -ctx my_script.das generated/
```

and link the result against nano instead of `libDaScript`. The generated
`my_script.das.h` gives you a plain C++ class:

```cpp
#include "daScript/nano_print.h"
#include "my_script.das.h"

int main () {
    das::das_nano_set_print(&my_log_sink);     // where print goes
    my_script::Standalone ctx;                 // construct it
    int answer = ctx.my_exported_function(21); // call it
}
```

No `daScript.h`, no module registration, no `Program`, no file system. The
context is an object with a lifetime.

## What it costs

The four examples under `examples/standalone/`, built for x64 with MSVC, next to
the equivalent program on the full runtime:

| program | tier | size |
|---|---|---|
| `01_pure` | POD compute, no heap | 120 KB |
| `02_heap` | arrays, tables, `new`/`delete` | 129 KB |
| `03_closures` | lambdas, function pointers, generators | 139 KB |
| `04_c_binding` | C drives the loop, das decides, `print` | 151 KB |
| full-runtime standalone context, for scale | | 463 KB |

Those numbers include the platform's C runtime, so what they measure is the
difference nano makes on a host - not an embedded footprint.

**nano does not cross-compile freestanding yet.** It runs on the platforms the
full runtime runs on, minus the compiler. Building it for a bare-metal target
needs portability work in the shared headers; `ARCHITECTURE.md` lists what.

## What it leaves out

A script that reaches past what nano carries fails to **link** - the boundary
announces itself at build time rather than becoming a program that quietly grew.
Absent: string interpolation and the string builders (this is where fmt comes
back), every builtin module including `math`, the GC, the debugger, the JIT, the
serializer, the profiler, threads, and the file system.

`ARCHITECTURE.md` beside this file explains what nano is made of and lists the
trades it makes.

## Building it

In this repository nano builds as part of the normal CMake configure, and the
examples build with it. Outside it, point `DASLANG_NANO_ROOT` at an installed
SDK root and `add_subdirectory` this folder:

```cmake
set(DASLANG_NANO_ROOT "/path/to/daslang-sdk")
add_subdirectory("${DASLANG_NANO_ROOT}/nano" nano_build)

# Anything linking nano lets nano decide the header search order.
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")
add_executable(my_program main.cpp generated/my_script.das.cpp)
target_include_directories(my_program PRIVATE generated)
target_link_libraries(my_program PRIVATE libDaScriptNano)
```

That `set_property` line is load-bearing, not tidiness: without it your target
compiles against the full runtime's headers while linking nano's library.
`ARCHITECTURE.md` says why under "The include-order trap".
