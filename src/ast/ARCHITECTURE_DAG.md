# src/ast architecture notes - the compile DAG

Companion to `ARCHITECTURE.md` in this folder.

## Scheduling the module compile DAG (`ast_compile_dag.cpp`) {#compile-dag}

`compileModules` is the one entry: it compiles the modules a walk found - parse, inference and
macro-module simulation, one module at a time - on `CodeOfPolicies::compile_threads` threads (0 is
the `JobQue` thread count), and every caller of the walk goes through it. One thread, a compile
inside a worker, or a module-cache stream compiles them in walk order on the calling thread.

The prerequisite walk returns `walk.req` in post-order, so a module's dependencies always come
before it. `buildModuleDag` turns that list into each node's dependency list, indices kept in
that order: a dependency always has a smaller index, and the passes over the graph are single
forward or backward sweeps instead of a sort. A
node's edges are the requires the text collector (`getAllRequire`) finds in its file, each
resolved through the compile's `FileAccess` from that file, as the walk resolves it, and matched
to a node by file name; a require of a module already in the process (C++, promoted shared) has
no node and needs no scheduling. Every node after an extra-dependency module (`builtin` and the
access's extra modules) depends on it, since those are visible to every module without a
`require`.

Exact minimum makespan for a DAG on several workers is NP-hard, so the order is fixed before any
thread starts: the modules sorted by bottom level - the node's cost plus the longest cost path
through the modules that require it - because a chain is sequential and every unit it waits adds a
unit to the makespan, while modules off the chain fill idle workers at any time. A dependency's
bottom level is always above its dependents', so the sorted list is a topological order; ties keep
the `req` order, so equal inputs always produce the same order. Each worker thread takes the next
module in that order and waits on the futures of the modules it requires. The lowest unfinished
module a thread holds has every dependency done, so the threads never wait on each other in a
cycle.

`options log_compile_dag` in the program's file prints the graph its modules compiled on, timed
with each module's measured compile, the critical path, and for 2 to 32 workers the greedy
schedule's makespan beside the lower bound `max(critical path, total / workers)`, which no
schedule beats.

## Compile workers (`ast_compile_dag.cpp`) {#compile-workers}

A worker writes nothing another module reads before its future is set. Its compile runs on its
thread's own `daScriptEnvironment` copy, made by the main thread before the threads start (the
environment holds the compiling `Program` behind a reference count that is not atomic), so the
current-compile fields (`g_Program`, the compiler log, the compiling names) are its own. A
module's result - its `Program`, its log, the root that owns its nodes, and the modules a module
requiring it adds to its group - is the value of its future: the worker sets it once, every module
requiring it reads it, and nothing else touches it. Before setting it the worker names and freezes
its own module, since a module requiring it finds it by name in its group. The main thread joins
the threads and then, alone, reads every future in walk order and registers each module
(`registerCompiledModule`: promotion, `libGroup`), so the shared tables have one writer and the
order a sequential compile has.

Every job parses through the compile's one `FileAccess`. The prerequisite walk has already loaded
every file a module or its includes name, so the executor locks the access (`FileAccess::lock`)
while the threads run and a lookup only reads the file table. The access's reference count is not
atomic either, so no worker copies or releases a pointer to it: the main thread creates each
job's `Program` with its `access` already set before the threads start, `bindCompilingProgram`
leaves an access the program already holds alone, and the parser state borrows the access
(`DasParserState::g_Access` is a plain pointer - the caller's reference outlives the parse).

## Deadlock prevention (`ast_compile_dag.cpp`) {#deadlock-prevention}

A macro module's context keeps its globals and heap between macro calls, so two threads never run
one context at once: a call holds the context for its whole length, nested work included, since
the paused call keeps frames on the context's stack and references into its memory. A macro whose
code runs a builtin that compiles, simulates or invokes other code (`isNestingCompileBuiltin` in
`ast_export.cpp` - `compile`, `require_module_now`, `invoke_in_context` and the rest) can need a
second context while
holding the first, and two such threads waiting on each other's contexts would never finish - a
valid program, so the scheduler makes the wait impossible instead of reporting it.

Each function records, while its own module compiles, whether its body calls or takes the address
of such a builtin (`Function::callsNestingBuiltin`, set by the symbol-use pass); a macro module is
nesting when a function its macro context holds has the flag (`Module::macroNestsCompile`, set by
`markMacroSymbolUse`, which runs before the macro context is built, both after a compile and after
a module-cache read). The single-expression `eval_single_expression(expr, ok)` runs in a fresh
context of its own and is not one of the builtins; the form that takes a context is.

A nesting module never runs its macro code on a worker. Its worker stops before the macro context
is built (`parseDaScriptEx` with `deferNestingMacroModule`), and every module that requires it,
directly or through other modules, skips its compile. The main thread builds the deferred macro
context after the join (`buildDeferredMacroModule`), registers the module, and then compiles the
skipped modules in walk order, one at a time, as a sequential compile does. When a module already
loaded before the compile nests, the whole compile runs in walk order. Every module a worker
compiles only runs macros that hold one context and never wait for a second, so no cycle of waits
can form.
