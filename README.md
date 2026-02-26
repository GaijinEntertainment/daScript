# Daslang

Daslang (formerly daScript) is a high-performance, statically typed programming language built for games and real-time applications.

[![build](https://github.com/GaijinEntertainment/daScript/actions/workflows/build.yml/badge.svg)](https://github.com/GaijinEntertainment/daScript/actions/workflows/build.yml)
[![wasm_build](https://github.com/GaijinEntertainment/daScript/actions/workflows/wasm_build.yml/badge.svg)](https://github.com/GaijinEntertainment/daScript/actions/workflows/wasm_build.yml)

[Website](https://dascript.org/) · [Documentation](https://dascript.org/doc/) · [Blog](https://borisbat.github.io/dascf-blog) · [Try Online](https://gaijinentertainment.github.io/try-dascript/)

## Why Daslang?

Daslang was created at [Gaijin Entertainment](https://gaijin.net/) to solve a real problem: **interop overhead** between scripting languages and C++ was eating the frame budget in their ECS game engine. Lua (via LuaJIT) and Squirrel both hit the same wall — marshaling data back and forth was too expensive.

Daslang's data layout matches C++. There is no marshaling, no boxing, no conversion — script↔C++ calls are near-zero cost.

**Core principles:**
- **Iteration speed is king** — a full production game recompiles in ~5 seconds; hot reload is built in
- **Explicit, not implicit** — no hidden type conversions, no silent allocations; `options log` shows exactly what the compiler produces
- **99% safe, not 100%** — eliminates real-world C++ bugs pragmatically, without Rust-level restrictions
- **If it gets slow, you can fix it** — manual `delete` to reduce GC pressure, AOT to C++ for native speed
- **The language reflects the problem** — a compile-time macro system lets libraries reshape syntax to match the domain

**Three execution tiers** (all planned from day one):
fast tree-based **interpreter** → **AOT** compilation to C++ (required for consoles) → **JIT** via LLVM.
Hybrid mode uses semantic hashing: unchanged functions stay AOT, changed ones fall back to the interpreter — ship a hotfix without a full rebuild.

See the [design philosophy](doc/source/reference/design_philosophy.rst) for the full story.

## Installation

```sh
git clone https://github.com/GaijinEntertainment/daScript.git daslang
cd daslang
git submodule update --init --recursive
```

## Building

```sh
cmake -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --target daslang --config RelWithDebInfo
```

## Aot usage
For detailed usage see [integration_cpp_13.cpp](tutorials/integration/cpp/integration_cpp_13.cpp) and the [AOT tutorial](doc/source/reference/tutorials/integration_cpp_13_aot.rst).
Here's a short version:
First compile `daslang`:
```sh
cmake -Bbuild
cmake --build build --target daslang
```
Now let's create a `main.das` file with the following content:
```sh
[export]
def main() { print("Hello world\n"); }
```
Create a `C++` application, called `test_aot.cpp`. For simplicity we don't
use error checking here (based on the [AOT integration tutorial](tutorials/integration/cpp/integration_cpp_13.cpp)):
```cpp
#include "daScript/daScript.h"
int main(int argc, char **argv) {
    using namespace das;
    NEED_ALL_DEFAULT_MODULES;
    Module::Initialize();
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    CodeOfPolicies policies = {.aot = true, .version_2_syntax=true};
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript("main.das", fAccess, tout, dummyLibGroup, policies);
    assert(!program->failed());
    Context ctx(program->getContextStackSize());
    program->simulate(ctx, tout);
    auto fnTest = ctx.findFunction("main");
    assert(fnTest != nullptr);
    ctx.evalWithCatch(fnTest, nullptr);
    Module::Shutdown();
}
```
Perform AOT compilation, then compile and run the binary:
```sh
./bin/daslang -aot main.das aot_main.cpp
clang++ test_aot.cpp aot_main.cpp -Iinclude lib/liblibDaScript.a lib/liblibUriParser.a -o aot_example
./aot_example
```
In a similar manner, you can link against a shared library instead of a static one:
```sh
cmake --build build --target daslang
./bin/daslang -aot main.das aot_main.cpp
clang++ test_aot.cpp aot_main.cpp -Iinclude lib/liblibDaScriptDyn.so -o aot_example
LD_LIBRARY_PATH=./lib ./aot_example
```

## JIT usage
To use JIT, you need the `LLVM 16.0.6` shared library at the path
`lib/LLVM.dll`, this name is fixed because of [dynamic bindings](modules/dasLLVM/bindings/llvm_func.das).

Once you have `LLVM.dll`, all you need is:
```sh
./bin/daslang -jit main.das
```
For more details on how JIT works and what can be configured, see the
[JIT Readme](modules/dasLLVM/README.md).

## Embedding into cmake
To embed daslang into your CMake application, simply call `find_package(DAS)`.
This will provide the targets `libDaScript`
and `libDaScriptDyn`. For an example of using daslang as an external project
(including usage for dynamic modules) see [this demo](https://github.com/aleksisch/dascript-demo).

## VS Code extensions

- [daScript](https://marketplace.visualstudio.com/items?itemName=eguskov.dascript) — syntax highlighting for `.das` files
- [daScript language support](https://marketplace.visualstudio.com/items?itemName=profelis.dascript-plugin) — language server, linting, debugging, and snippets
