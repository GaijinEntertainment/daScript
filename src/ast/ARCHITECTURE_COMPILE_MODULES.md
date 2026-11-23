# src/ast architecture notes - compiling the required modules

Companion to `ARCHITECTURE.md` in this folder.

## The module DAG (`ast_compile_modules.cpp`) {#compile-dag}

`compileModules` compiles every module the prerequisite walk found; every caller goes through it,
and it returns the errors of the module that failed. The walk lists modules in post-order, so
dependencies come first. A module's edges are the `require`s in its file.
A parallel schedule takes ready modules by the longest remaining chain first.

`options log_compile_dag` prints the graph with measured times and the best possible schedule.
