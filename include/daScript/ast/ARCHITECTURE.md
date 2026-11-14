# AST headers - architecture notes

This document follows `ARCHITECTURE_COMMON.md`.

## What the JIT's DLL cache key leaves out {#jit-dll-cache-key}

The JIT caches each compiled DLL under a key computed from `LLVM_JIT_CODEGEN_VERSION` and each
function's AST hash. The key covers neither the module an extern lives in, nor the name a C++
binding registers under, nor the offsets of a bound C++ type. A header change to one of those
leaves the key as it was, so a cache hit binds the old name or the old offsets and crashes.
Bumping the version makes the JIT recompile instead of loading a cached DLL.

A bound type's memory layout changes with a data member added, removed, reordered or retyped in
the type, in a base of it, or in a struct it holds by value, and with an added base class, a
first virtual function, an `alignas` or a packing change. A changed signature on an existing
virtual changes no layout.
