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

The examples under `examples/standalone/`, next to the equivalent program on the
full runtime. `.exe` is x64 MSVC; `.text` is arm-none-eabi-gcc 13.2 for a
cortex-m4 with `-Os --gc-sections` and newlib.

| program | tier | x64 `.exe` | cortex-m4 `.text` |
|---|---|---|---|
| `01_pure` | POD compute, no heap | 136 KB | 75,540 |
| `02_heap` | arrays, tables, `new`/`delete` | 148 KB | 80,612 |
| `03_closures` | lambdas, function pointers, generators | 154 KB | 84,420 |
| `04_c_binding` | C drives the loop, das decides, `print` | 166 KB | 91,520 |
| `05_compile_time_table` | a CSV baked into a table by the compiler | 144 KB | 78,028 |
| full-runtime standalone context, for scale | | 472 KB | - |

`06_full_runtime` beside those five is not a nano program: the same `-ctx` output
linked against the full runtime, because its script reaches C++ modules (dasHV, fio).
Its README says what that keeps and what it costs; it is not in the table above.

The x64 column includes the platform's C runtime, so what it measures is the
difference nano makes on a host, not an embedded footprint. The cortex-m4 column
is the real one: of `01_pure`'s 75,540 bytes, roughly 29 KB is the nano runtime,
10 KB the compiled script, and the rest the C library. `ARCHITECTURE.md` says
where the C library goes and what is worth trimming.

Most of what is left is decided by the program, not by nano. A script that says
`var x : double` links double soft-float; one that says `is Foo` needs RTTI. The
runtime is the floor, and the floor is not where the interesting savings are -
which is what `05_compile_time_table` is about.

## Doing the work at compile time

`05_compile_time_table` reads a thermistor calibration CSV **while the script
compiles**, inverts the curve and bakes an evenly spaced lookup table into the
generated C++. The file, the parser, the search and the interpolation stay on the
workstation; the device gets a table, an index and a lerp. Nothing in the linked
image can parse anything - `strtol`, `strtod`, `sscanf`, `atoi`, `fopen` and
`fread` are all absent from it.

That is the argument for this tier. The runtime being small is worth something,
but a macro that moves a whole computation to build time is worth more, and
daslang macros are ordinary daslang.

## What it leaves out

A script that reaches past what nano carries fails to **link** - the boundary
announces itself at build time rather than becoming a program that quietly grew.
Absent: string interpolation and the string builders (this is where fmt comes
back), every builtin module including `math`, the GC, the debugger, the JIT, the
serializer, the profiler, threads, and the file system.

Panic and fatal messages format through nano's own integer-only formatter: `%s`,
`%d`/`%i`, `%u`, `%x`, `%p` and `%%`, with `l`/`ll` widths - no floats, no field
width or precision. Scripts printing floats still work (that path uses the C
library's `snprintf`, and only links it when a float is actually printed); it is
the embedder-facing `das_fatal_log`/panic formatting that is deliberately this
narrow.

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
