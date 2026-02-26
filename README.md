# Daslang
Daslang - high-performance statically strongly typed programming language

MacOS/linux/win32/win64 build status [![build](https://github.com/GaijinEntertainment/daScript/actions/workflows/build.yml/badge.svg)](https://github.com/GaijinEntertainment/daScript/actions/workflows/build.yml)\
MacOS/linux/win64 wasm build status [![wasm_build](https://github.com/GaijinEntertainment/daScript/actions/workflows/wasm_build.yml/badge.svg)](https://github.com/GaijinEntertainment/daScript/actions/workflows/wasm_build.yml)

Read my [BLOG](https://borisbat.github.io/dascf-blog)

0.6 version is coming soon.

Serialization, JIT (via LLVM), and many more good language features are in.
We are putting in last finishing touches, documentation, and all that jazz.
See you soon...

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
