# src/ast architecture notes - compiling the required modules

Companion to `ARCHITECTURE.md` in this folder.

## The module DAG (`ast_compile_modules.cpp`) {#compile-dag}

`compileModules` compiles every module the prerequisite walk found; every caller goes through it,
and it returns the errors of the module that failed. The walk lists modules in post-order, so
dependencies come first. A module's edges are the `require`s in its file.
With `compile_threads` above 1 the modules run as `JobQue` jobs: a module starts once its
dependencies are done, longest remaining chain first. No job waits for another, so the queue
cannot deadlock.

`options log_compile_dag` prints the graph with measured times and the best possible schedule.

## Compile workers {#compile-workers}

Each job compiles one module on its own `daScriptEnvironment` copy and writes only its own result.
When all jobs finish, the main thread registers the modules in walk order, so shared tables have one
writer. Jobs share the compile's `FileAccess`: it is locked, since the walk already loaded every
file, and jobs only borrow the pointer, since its reference count is not atomic. A locked access
loads no new file, and a `.das_project` access runs its project context under that context's lock.
A frozen module is read-only: a job reads its globals' access flags but never stamps them.

## The cached prefix {#cache-prefix}

The module cache is one stream in walk order. Modules are read from it in order until the first
stale record; the rest compile as jobs, and the main thread then appends their records in walk
order. A warm compile is a sequential read.

## Deadlock prevention {#deadlock-prevention}

A macro call holds its context's lock. A macro that calls `compile`, `require_module_now`,
`invoke_in_context` or a similar builtin can need a second context, so two such macros on two
threads could wait for each other.

Such builtins are marked `nestsCompile` where they are registered; a function that calls one gets
the flag too, and a macro module holding such a function is `macroNestsCompile`.
A nesting module builds its macro context on the main thread after the jobs, and the modules that
use it compile after that, in walk order.
