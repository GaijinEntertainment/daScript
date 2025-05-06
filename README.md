# Daslang
Daslang - high-performance statically strong typed scripting language

MacOS/linux/win32/win64 build status [![build](https://github.com/GaijinEntertainment/daScript/actions/workflows/build.yml/badge.svg)](https://github.com/GaijinEntertainment/daScript/actions/workflows/build.yml)\
MacOS/linux/win64 wasm build status [![wasm_build](https://github.com/GaijinEntertainment/daScript/actions/workflows/wasm_build.yml/badge.svg)](https://github.com/GaijinEntertainment/daScript/actions/workflows/wasm_build.yml)

Read my [BLOG](https://borisbat.github.io/dascf-blog)

0.5 version is coming soon.

Serialization, JIT (via LLVM), and many more good language features are in.
We are putting in last finishing touches, documentation, and all that jazz.
See you soon...

## Installation

```sh
git clone https://github.com/GaijinEntertainment/daScript.git daScript
cd daScript
git submodule update --init --recursive
```

## Building

```sh
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --target daslang --config RelWithDebInfo
```
