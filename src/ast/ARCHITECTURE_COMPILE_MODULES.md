# src/ast architecture notes - compiling the required modules

Companion to `ARCHITECTURE.md` in this folder.

## The module DAG (`ast_compile_modules.cpp`) {#compile-dag}

`compileModules` compiles every module the prerequisite walk found; every caller goes through it,
and it returns the errors of the module that failed. The walk lists modules in post-order, so
dependencies come first. A module's edges are the `require`s in its file, and every `!inscope`
module before it in walk order (`#comment-readers`).
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
the flag too, and a macro module holding such a function is `macroNestsCompile`. A job never
builds such a module's macro context: the module and every module after it in walk order
compile on the main thread after the jobs (`#walk-order-tail`), where no other macro runs.

## Comment readers {#comment-readers}

A comment reader is one object per module, and every parse takes the readers of every built-in
module and of every module in its group, required or not: a reader sees every module parsed after
it in walk order, and holds that parse's state between its callbacks. So a module that installs a
reader is declared `!inscope`, which the require scan reads from its `module` line, and every
module after it in walk order depends on it in the DAG: each of them is parsed after the reader
exists, as in the sequential compile, while they still run in parallel with each other. A parse
that takes a reader holds one lock for the whole parse, so parses with readers take turns while
inference and macros stay parallel; the lock is recursive, for a parse nested on the same thread.

## The walk-order tail {#walk-order-tail}

A job learns that its module is nesting only when that module's compile ends. From then on no job
at or after that position starts, and the results of later modules that finished without it are
dropped. The main thread then compiles every module not yet registered in walk order, each
through the whole sequential pipeline, so from that position on the compile is the sequential
one: no module's macro context is built later than in it.
