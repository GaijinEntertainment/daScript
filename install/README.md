# daslang

**daslang** (formerly daScript) is a high-performance statically strongly typed programming language. It can be used standalone or embedded into a C++ host application, and is designed for games and real-time applications.

- **Version:** 0.6.0 — Release Candidate 1
- **Release date:** February 2026
- **License:** BSD 3-Clause

## Links

- **Website:** <https://daslang.io/>
- **Repository:** <https://github.com/GaijinEntertainment/daScript>
- **Documentation:** <https://daslang.io/doc/>
- **Blog:** <https://borisbat.github.io/dascf-blog>
- **Playground:** <https://daslang.io/#sandbox>
- **Authors:** Gaijin Entertainment

## Key Features

- High-performance — close to C++ speed via ahead-of-time (AOT) compilation and optional JIT (via LLVM)
- Strong static typing with type inference
- Generics, macros, and compile-time meta-programming
- First-class iterators, generators, lambdas, and closures
- Pattern matching, LINQ-style queries via standard library
- Native C++ interop — easy embedding and binding
- Hot reloading support for rapid iteration
- Cross-platform — Windows, macOS, Linux, WASM

## What's in the Box

| Directory    | Contents                                              |
|--------------|-------------------------------------------------------|
| `bin/`       | Compiler and tool binaries (`daslang`, `daslang_dyn`) |
| `lib/`       | Static and shared libraries for embedding             |
| `include/`   | C++ headers for integration                           |
| `daslib/`    | Standard library modules (`.das` files)               |
| `modules/`   | Optional plugin modules                               |
| `examples/`  | Example scripts                                       |
| `tutorials/` | Language and integration tutorials                    |

## Quick Start

Run a script:

```sh
bin/daslang examples/hello_world.das
```

AOT-compile a script:

```sh
bin/daslang -aot examples/hello_world.das hello_world_aot.cpp
```

## Embedding via CMake

```cmake
find_package(DAS REQUIRED)
target_link_libraries(your_app PRIVATE DAS::libDaScript)
```

See `tutorials/integration/` for complete C and C++ embedding examples.

## Building from Source

```sh
git clone --recurse-submodules https://github.com/GaijinEntertainment/daScript.git daslang
cd daslang
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --target daslang --config Release
```

## License

BSD 3-Clause License — Copyright (c) 2019-2026, Gaijin Entertainment.
See `LICENSE` for the full text.
