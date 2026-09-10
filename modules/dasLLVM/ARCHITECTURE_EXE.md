# dasLLVM Architecture - the standalone exe

Companion of `ARCHITECTURE.md` (sec.10 routes here). Contract: `ARCHITECTURE_COMMON.md` (repo
root). This document carries what `inject_main` (`daslib/llvm_exe.das`) decides for a standalone
exe at build time and what the exe does at startup before its init script runs.

## 1. The require-resolver rows

`inject_main` decides the exe's link - the runtime-only library, or the whole compiler library
when the program registers every builtin module, ships a dynamic module, or reaches a
compiler-lib module - and emits the `register_native_path` rows the host's module scan loaded
only under the whole-lib link, once, after that decision. The rows feed the compile-time
require resolver (`FsFileAccess::getModuleInfo`), which lives in the compiler library: a
runtime-only exe has no compiler and can never reach them, and every row it would carry is a
startup `jit_register_native_path_resolve` call - one exe-file lookup and one stat per row, for
every row the host scan loaded - and a build-machine path baked into the binary. A whole-lib exe
(`dastest.exe`, which compiles test files at run time) still carries every row, re-rooted at run
time the way dynamic modules are.

## 2. A global initializer's addresses are filled at startup

The exe emitter leaves every address global - a `@@fn` value, a handled type's `new`, the
table accessors - null and private, since a JIT-process address means nothing in the exe, and
`collect_external_functions` walks the used functions and the used global initializers at
startup to store the exe-resident address into each, registering the function with the context
on the way. The walk covers a promoted das module - one declared `module X shared`, which the
front end promotes and marks `builtIn` like a C++ module - because its initializers are program
code `init_globals` emits like any other; only the C++ modules, which have no das initializer to
walk, are skipped. A shared module's global that holds `@@fn` is the shape daslib's dispatch
tables and icon catalogs take, and a walk that skips such a module leaves the Func null: nothing
references the function, LLVM drops it, and the first invoke throws.
`tests/llvm_exe_shared_addr.das` holds the shape.
