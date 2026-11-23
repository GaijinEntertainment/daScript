# src/ast architecture notes - compiling the required modules

Companion to `ARCHITECTURE.md` in this folder.

## The module DAG (`ast_compile_modules.cpp`) {#compile-dag}

`compileModules` compiles every module the prerequisite walk found; every caller goes through it,
and it returns the errors of the module that failed. The walk lists modules in post-order, so
dependencies come first. A module's edges are the `require`s in its file.
A parallel schedule takes ready modules by the longest remaining chain first.

`options log_compile_dag` prints the graph with measured times and the best possible schedule.

## Deadlock prevention {#deadlock-prevention}

A macro call holds its context's lock. A macro that calls `compile`, `require_module_now`,
`invoke_in_context` or a similar builtin can need a second context, so two such macros on two
threads could wait for each other.

Such builtins are marked `nestsCompile` where they are registered; a function that calls one gets
the flag too, and a macro module holding such a function is `macroNestsCompile`.
A schedule compiles every module that uses a nesting module alone.
