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

## AOT usage

AOT compiles daslang scripts to C++ for native performance. Generate the C++ stub, then compile and link it with your host application:

```sh
./bin/daslang -aot main.das aot_main.cpp
clang++ host.cpp aot_main.cpp -Iinclude lib/liblibDaScript.a lib/liblibUriParser.a -o app
```

For a complete walkthrough, see the [AOT tutorial](doc/source/reference/tutorials/integration_cpp_13_aot.rst) and [integration_cpp_13.cpp](tutorials/integration/cpp/integration_cpp_13.cpp).

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

## Tree-sitter grammar

A full tree-sitter grammar for daslang lives in [`tree-sitter-daslang/`](tree-sitter-daslang/). It parses 99.4% of the codebase (all valid files) and is built automatically by CMake as a shared library.

Use it for:
- **Syntax highlighting** — `queries/highlights.scm` included, works in editors that support tree-sitter (Neovim, Helix, Zed)
- **Parse-aware search** — via [ast-grep](https://ast-grep.github.io/) (`sg`) for structural code search across `.das` files
- **Editor extensions** — `tree-sitter-daslang/editors/zed/` includes a Zed extension

Build the grammar:
```sh
cmake --build build --target tree_sitter_daslang
```

## MCP server (AI tool integration)

An [MCP](https://modelcontextprotocol.io/) server in [`utils/mcp/`](utils/mcp/) exposes 19 compiler-backed tools to AI coding assistants (Claude Code, etc.): compilation diagnostics, type inspection, go-to-definition, find-references, AST dump, expression evaluation, parse-aware grep, and more.

Requires dasHV (`-DDAS_HV_DISABLED=OFF`). See [`utils/mcp/README.md`](utils/mcp/README.md) for setup and configuration.

## VS Code extensions

- [daScript](https://marketplace.visualstudio.com/items?itemName=eguskov.dascript) — syntax highlighting for `.das` files
- [daScript language support](https://marketplace.visualstudio.com/items?itemName=profelis.dascript-plugin) — language server, linting, debugging, and snippets
